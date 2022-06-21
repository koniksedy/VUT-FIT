/**
 * Hlavičkový soubor UDP skeneru portů.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */
#ifndef __UDP_SCANNER_H__
#define __UDP_SCANNER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>           // usleep
#include <sys/types.h>        // socket
#include <netinet/ip.h>       // iphdr
#include <netinet/udp.h>      // udphdr
#include <netinet/tcp.h>      // tcphdr 
#include <netinet/ip_icmp.h>  // icmphdr
#include <linux/icmpv6.h>     // icmp6hdr
#include <sys/socket.h>       // socket
#include <arpa/inet.h>        // ntohs
#include <pthread.h>          // pthread
#include "sniffer.h"          // pcap
#include "interface_ip.h"
#include "arg_parser.h"       // mod výpisu
#include "port_queue.h"
#include "error.h"            // chybové kódy


/**
 * Nastaví stavy všech portů pro UDP skenování na skenované a neskenované.
 * ----------------------------------------
 * src - fronta portů, určených ke skenování
 * ----------------------------------------
 * Funkce vytvoří hloubkovou kopii fronty portů a každý port,
 * který bude skenován označí v udp_port_staus jako SCANNED_PORT,
 * osatní označí jako UNSCANNED_PORT. Pokud kopie fronty selže,
 * vrátí false, v opačné případě true.
 */
bool reset_udp_status(port_queue_t *src);

/**
 * Callback UDP snifferu UDP skenování.
 * ----------------------------------------
 * packet - přijatý packet
 * Ostatní parametry jsou pouze kvůli dodržení deklarace funkce, využívá se je packet.
 * ----------------------------------------
 * Funkce obdrží ICMP packet (zprávu) typu 3. ICMP neobsahuje
 * informaci o portu, ze kterého byl odeslán, ale obsahuje hlavičku
 * UDP protokolu, který zprávu vyvolal. Původní cílový port v této
 * hlavičce je nyní označen v globální proměnné tcp_port_status
 * jako CLOSED_PORT. Jedná se o kritickou sekci, a proto se pro přískup ke
 * globální proměnné používá mutex.
 */
void detect_ICMP3(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);

/**
 * Začátek vlákna UDP (ICMP3) snifferu.
 * ----------------------------------------
 * h - ukazatel na sniffer
 * ----------------------------------------
 * Funkce vrací vždy úspěšně NULL.
 */
void *run_udp_scanner(void *h);

/**
 * Sestaví UDP packet a nastaví ip adresy a porty.
 * ----------------------------------------
 * src  - ip adresa zdroje
 * dst  - ip adresa cíle
 * port - port na který bude packet odeslán
 * ----------------------------------------
 * V případě úspěchu vrácí funkce ukazatel na UDP packet,
 * pokud pojde k chybě (chyba alokace, chyba ip adresy, ...) vrátí NULL).
 */
char *build_udp_packet(ip_t src, ip_t dst, int port);

/**
 * Odešle UDP packet na zadaný port cílové ip adresy.
 * ----------------------------------------
 * socket_tcp - UDP socket
 * src        - ip adresa zdroje
 * dst        - ip adresa cíle
 * port       - cílový port
 * ----------------------------------------
 * Funkce vytvoří UDP packet a spolu s UDP socketem jej odešle na cílový port.
 * Pokud dojde během vytváření packetu, nebo jeho odesílání k chybě, bude
 * funkcí vrácená hodnota false, v případě úspěchu true.
 */
bool send_udp_packet(int socket_udp, ip_t src, ip_t dst, int port);

/**
 * Provede UDP skenování specifikovaných portů.
 * ----------------------------------------
 * inter_ip  - interface s ip adresou
 * ip_dst    - adresa skenovaného serveru
 * udp_ports - porty pro skenování
 * run_type  - mod výpisu výsledku skenování (ALL|OPEN|CLOSED|UNCLOSED)
 * ----------------------------------------
 * Funkce vytvoří UDP a RST sniffer a spustí jej v odděleném vláknu.
 * Pro každý skenovyný port vytvoří socket a UDP packet, který následně 
 * odešle na skenovaný server. Případné vracející se odpovědi jsou zachytávána
 * a zpracovávána ve vláknu snifferu. Funkce se postará o odeslání packetů
 * na všechny poty najednou (v cyklu) a sniffer zatím zpracovává odpovědi. 
 * Následně podle hodnoty v globální proměnné udp_pasket_state výpíše
 * výsledky skenování (OPEN, CLOSED).
 * Pokud dojde během funkce k vážné chybě (nemožnost vytvořit socket, 
 * interní chyba, ...), bude program ukončen s příslušným návratovým kódem.
 * Pokud se nepodaří odeslat packet na daný port (nemožnost vytvořit UDP
 * packet pro daný port, chyba odesílání, ...),je vypsaná chybová hláška
 * a daný port je přeskočen.
 */
void scan_udp(interface_ip_t inter_ip, ip_t ip_dst, port_queue_t *udp_ports, short int run_type);

#endif