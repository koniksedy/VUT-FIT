/**
 * Hlavičkový soubor TCP skeneru portů.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#ifndef __SYN_ACK_SNIFFER__
#define __SYN_ACK_SNIFFER__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         // usleep
#include <sys/types.h>      // socket
#include <netinet/ip.h>     // iphdr
#include <netinet/udp.h>    // udpder
#include <netinet/tcp.h>    // tcphdr  
#include <sys/socket.h>     // socket
#include <arpa/inet.h>      // ntohs
#include <pthread.h>        // pthread
#include "sniffer.h"        // pcap
#include "interface_ip.h"
#include "arg_parser.h"     // mod výpisu
#include "port_queue.h"
#include "error.h"          // chybové kódy


/**
 * Pseudo IPv4 hlavička pro kontrolní součet TCP.
 */
typedef struct {
    unsigned int src_addr;
    unsigned int dst_addr;
    unsigned char res;
    unsigned char protocol;
    unsigned short len;
    struct tcphdr tcp;
} pseudo_header_t;

/**
 * Nastaví stavy všech portů pro tcp skenování na skenované a neskenované.
 * ----------------------------------------
 * src - fronta portů, určených ke skenování
 * ----------------------------------------
 * Funkce vytvoří hloubkovou kopii fronty portů a každý port,
 * který bude skenován označí v tcp_port_staus jako SCANNED_PORT,
 * osatní označí jako UNSCANNED_PORT. Pokud kopie fronty selže,
 * vrátí false, v opačné případě true.
 */
bool reset_tcp_status(port_queue_t *src);

/**
 * Callback SYN-ACK snifferu TCP skenování.
 * ----------------------------------------
 * packet - přijatý packet
 * Ostatní parametry jsou pouze kvůli dodržení deklarace funkce, využívá se je packet.
 * ----------------------------------------
 * Funkce na základě portu, ze kterého byl poslán SYN-ACK nebo ACK packet
 * pozná, který port je otevřený a označí jej v globální proměnné tcp_port_status
 * jako OPEN_PORT. Jedná se o kritickou sekci, a proto se pro přískup ke
 * globální proměnné používá mutex.
 */
void syn_ack_detected(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);

/**
 * Callback RST snifferu TCP skenování.
 * ----------------------------------------
 * packet - přijatý packet
 * Ostatní parametry jsou pouze kvůli dodržení deklarace funkce, využívá se je packet.
 * ----------------------------------------
 * Funkce na základě portu, ze kterého byl poslán RST  packet
 * pozná, který port je zavřený a označí jej v globální proměnné tcp_port_status
 * jako CLOSED_PORT. Jedná se o kritickou sekci, a proto se pro přískup ke
 * globální proměnné používá mutex.
 */
void rst_detected(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);

/**
 * Začátek vlákna SYN-ACK snifferu.
 * ----------------------------------------
 * h - ukazatel na sniffer
 * ----------------------------------------
 * Funkce vrací vždy úspěšně NULL.
 */
void *run_syn_ack_scanner(void *h);

/**
 * Začátek vlákna RST snifferu.
 * ----------------------------------------
 * h - ukazatel na sniffer
 * ----------------------------------------
 * Funkce vrací vždy úspěšně NULL.
 */
void *run_rst_scanner(void *h);

/**
 * Výpočet kontrolního součtu TCP/IP
 * ----------------------------------------
 * !! Celá funkce je převzata z: https://www.winpcap.org/pipermail/winpcap-users/2007-July/001984.html
 * Autor: He Tao, (hetaozju@gmail.com)
 * ----------------------------------------
 * buffer - TCP/IP hlavička
 * size   - velikost bufferu
 * ----------------------------------------
 * Funkce vypočte kontrolní součet TCP/IP hlavičky.
 */
unsigned short check_sum(unsigned short *buffer, int size);

/**
 * Sestaví TCP packet a nastaví ip adresu a port.
 * ----------------------------------------
 * src  - ip adresa zdroje
 * dst  - ip adresa cíle
 * port - port na který bude packet odeslán
 * ----------------------------------------
 * V případě úspěchu vrácí funkce ukazatel na TCP packet,
 * pokud pojde k chybě (chyba alokace, chyba ip adresy, ...) vrátí NULL).
 */
char *build_tcp_packet(ip_t src, ip_t dst, int port);

/**
 * Odešle SYN packet TCP protokolem na zadaný port cílové ip adresy.
 * ----------------------------------------
 * socket_tcp - TCP socket
 * src        - ip adresa zdroje
 * dst        - ip adresa cíle
 * port       - cílový port
 * ----------------------------------------
 * Funkce vytvoří TCP packet a spolu se TCP socketem jej odešle na cílový port.
 * Pokud dojde během vytváření packetu, nebo jeho odesílání k chybě, bude
 * funkcí vrácená hodnota false, případě úspěchu true.
 */
bool send_syn_packet(int socket_tcp, ip_t src, ip_t dst, int port);

/**
 * Provede TCP skenování specifikovaných portů.
 * ----------------------------------------
 * inter_ip  - interface s ip adresou
 * ip_dst    - adresa skenovaného serveru
 * tcp_ports - porty pro skenování
 * run_type  - mod výpisu výsledku skenování (ALL|OPEN|CLOSED|UNCLOSED)
 * ----------------------------------------
 * Funkce vytvoří ACK-SYN a RST sniffer a spustí je v oddělených vláknech.
 * Pro každý skenovyný port vytvoří socket a TCP packet, který následně 
 * odešle na skenovaný server. Případné vracející se odpovědi jsou zachytávána
 * a zpracovávána ve sniffer vláknech. Funkce se postará o odeslání packetů
 * na všechny poty najednou (v cyklu) a sniffery zatím zpracují odpovědi. 
 * Zkontroluje se, jestli existují porty, ze kterých nepřišla odpověď
 * a na tyto porty funkce znovu zašle TCP packet. Následně podle hodnoty
 * v globální proměnné tcp_pasket_state výpíše výsledky skenování (OPEN,
 * CLOSED, FILTERED).
 * Pokud dojde během funkce k vážné chybě (nemožnost vytvořit socket, 
 * interní chyba, ...), bude program ukončen s příslušným návratovým kódem.
 * Pokud se nepodaří odeslat packet na daný port (nemožnost vytvořit TCP
 * packet pro daný port),je vypsaná chybová hláška a daný port je přeskočen.
 */
void scan_tcp(interface_ip_t inter_ip, ip_t ip_dst, port_queue_t *tcp_ports, short int run_type);

#endif