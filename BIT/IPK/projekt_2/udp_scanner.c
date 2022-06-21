/**
 * Modul implementující UDP skener portů.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#include "udp_scanner.h"


pthread_mutex_t mutex_udp;  // synchronizační semafor
char udp_port_status[MAX_PORT_COUNT];   // všechny možné porty

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
bool reset_udp_status(port_queue_t *src)
{
    // Vynulování všech portů
    memset(udp_port_status, UNSCANNED_PORT, MAX_PORT_COUNT);

    // Hloubková kopie fronty portuů
    port_queue_t *tmp = port_queue_cpy(src);
    if(tmp == NULL)
    {
        return false;
    }

    // Každý port ve frontě se označí jako SCANNED_PORT.
    unsigned short port;
    while(!is_empty(tmp))
    {
        port = get(tmp);

        udp_port_status[port] = SCANNED_PORT;
    }

    // Uvolnění zdrojů.
    port_queue_dtor(&tmp);

    return true;
}

/**
 * Callback UDP snifferu UDP skenování.
 * ----------------------------------------
 * packet - přijatý packet
 * Ostatní parametry jsou pouze kvůli dodržení deklarace funkce, využívá se je packet.
 * ----------------------------------------
 * Funkce obdrří ICMP packet (zprávu) typu 3. ICMP neobsahuje
 * informaci o portu, ze kterého byl odeslán, ale obsahuje hlavičku
 * UDP protokolu, který zprávu vyvolal. Původní cílový port v této
 * hlavičce je nyní označen v globální proměnné tcp_port_status
 * jako CLOSED_PORT. Jedná se o kritickou sekci, a proto se pro přískup ke
 * globální proměnné používá mutex.
 */
void detect_ICMP3(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{   
    // ZAČÁTEK PŘEVZATÉ ČÁSTI
    // Následující kód byl převzat z: https://code.google.com/p/pcapsctpspliter/issues/detail?id=6
    // Autor: Joshua Robinson, (joshua.robinson@gmail.com)
    // Detekce IPv4 nebo IPv6
    int packet_type = ((int) (packet[12]) << 8) | (int) packet[13];
    // KONEC PŘEVZATÉ ČÁSTI

    // Nalezení portu, který vyvolal zprávu ICMP typu 3.
    unsigned int port;
    const struct udphdr *udp_header;
    if(packet_type == ETHER_TYPE_IP4)
    {
        udp_header = (struct udphdr*)(packet + ETHER_SIZE + IPV4_SIZE + sizeof(struct icmphdr) + IPV4_SIZE);
        port = ntohs(udp_header->dest);
    }
    else if(packet_type == ETHER_TYPE_IP6)
    {
        udp_header = (struct udphdr*)(packet + ETHER_SIZE + IPV6_SIZE + sizeof(struct icmp6hdr) + IPV6_SIZE);
        port = ntohs(udp_header->dest);
    }
    else
    {
        fprintf(stderr, "[INFO]: Neznámý ETHER_TYPE %x.\n", packet_type);
        return;
    }

    // Začátek kritické části
    pthread_mutex_lock(&mutex_udp);
    // Budou nastavovány pouze skoumané porty.
    if(udp_port_status[port] != UNSCANNED_PORT)
    {   
        // Označení portu jako uzavřený.
        udp_port_status[port] = CLOSED_PORT;
    }
    pthread_mutex_unlock(&mutex_udp);
    // Konec kritické části
}

/**
 * Začátek vlákna UDP (ICMP3) snifferu.
 * ----------------------------------------
 * h - ukazatel na sniffer
 * ----------------------------------------
 * Funkce vrací vždy úspěšně NULL.
 */
void *run_udp_scanner(void *h)
{
    pcap_loop((pcap_t*)h, -1, detect_ICMP3, NULL);
    return NULL;
}

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
char *build_udp_packet(ip_t src, ip_t dst, int port)
{
    struct iphdr *ip_header;
    struct udphdr *udp_header;

    // Alokuje prostor pro packet.
    char *datagram = (char*) calloc(4096, 1);
    if(datagram == NULL)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se postavit udp packet. Chyba mallocu.\n");
        return NULL;
    }

    // Pro IPv4 nastaví IP hlavičku, pro IPv6 nechá hlavičku vyplnit systémem.
    if(dst.family == AF_INET)
    {   
        // Nastavení začátku UPD hlavičky v packetu pro IPv4
        udp_header = (struct udphdr*)(datagram + sizeof(struct iphdr));
        ip_header = (struct iphdr*)(datagram);

        ip_header->ihl = 5;
        ip_header->version = 4;
        ip_header->tos = 0;
        ip_header->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr);
        ip_header->id = htons(rand());  // Nezáleží na hodnotě, jeden fragment
        ip_header->frag_off = 0; 
        ip_header->ttl = 64;
        ip_header->protocol = IPPROTO_UDP;
        ip_header->check = 0;   // Bude doplněno systémem

        // Nahrání adresy zdroje.
        if(inet_pton(AF_INET, src.addr, &ip_header->saddr) != 1)
        {
            fprintf(stderr, "[ERROR]: Chyba převodu IPv4 zdrojové adresy při vytváření UDP packetu.\n");
            free(datagram);
            return NULL;
        }

        // Nahrání adresy cíle.
        if(inet_pton(AF_INET, dst.addr, &ip_header->daddr) != 1)
        {
            fprintf(stderr, "[ERROR]: Chyba přeovdu IPv4 cílové adresy při vytváření UDP packetu.\n");
            free(datagram);
            return NULL;
        }
    }
    else if(dst.family == AF_INET6)
    {   
        // Nastavení začátku UPD hlavičky v packetu pro IPv6
        udp_header = (struct udphdr*)(datagram);
    }
    else
    {
        fprintf(stderr, "[ERROR]: Zvláštní chyba, korupce ip.family při vytváření UDP packetu.\n");
        free(datagram);
        return NULL;
    }

    udp_header->source = htons(1234);  // Na čísle nezáleží
    udp_header->len = htons(sizeof(struct udphdr)); // Velikost zprávy (pouze hlavička)
    udp_header->dest = htons(port); // Cílový skenovaný port
    udp_header->check = 0;  // Dopočítá systém.

    return datagram;

}

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
bool send_udp_packet(int socket_udp, ip_t src, ip_t dst, int port)
{
    // Vytvoří UDP packet
    char *packet = build_udp_packet(src, dst, port);
    if(packet == NULL)
    {
        fprintf(stderr, "[ERROR]: Nepodařil se vytvořit UDP packet.\n");
        return false;
    }

    // V zzávislosti na rodině ip nastaví socket.
    if(src.family == AF_INET)
    {
        struct sockaddr_in socket_addr;
        memset(&socket_addr, 0, sizeof(socket_addr));

        // Nastavení portu a ip rodiny cílové adresy.
        socket_addr.sin_port = htons(port);
        socket_addr.sin_family = src.family;

        // Nastavení cílové ip adresy.
        if(inet_pton(src.family, dst.addr, &socket_addr.sin_addr) != 1)
        {
            fprintf(stderr, "[ERROR]: Chyba při kopírování IPv4 adresy cíle do socket_addr.\n");
            return false;
        }

        // Odeslání UDP packetu pomocí IPv4.
        if((sendto(socket_udp, packet, sizeof(struct iphdr) + sizeof(struct udphdr), 0,
            (struct sockaddr*)&socket_addr, sizeof(socket_addr))) == -1)
        {
            fprintf(stderr, "[ERROR]: Nepodařilo se odeslat UDP packet při IPv4.\n");
            return false;
        }
    }
    else
    {
        struct sockaddr_in6 socket_addr;
        memset(&socket_addr, 0, sizeof(socket_addr));

        // Nastavení portu a ip rodiny cílové adresy.
        socket_addr.sin6_port = htons(port);
        socket_addr.sin6_family = src.family;

        // Nastavení cílové ip adresy.
        if(inet_pton(src.family, dst.addr, &socket_addr.sin6_addr) != 1)
        {
            fprintf(stderr, "[ERROR]: Chyba při kopírování IPv6 adresy cíle do socket_addr.\n");
            return false;
        }

        // Odeslání UDP packetu pomocí IPv6.
        if((sendto(socket_udp, packet, sizeof(struct udphdr), 0, (struct sockaddr*)&socket_addr, sizeof(socket_addr))) == -1)
        {
            fprintf(stderr, "[ERROR]: Nepodařilo se odeslat UDP packet při IPv6.\n");
            return false;
        }
    }

    // Uvolnění zdrojů.
    free(packet);

    return true;
}

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
void scan_udp(interface_ip_t inter_ip, ip_t ip_dst, port_queue_t *udp_ports, short int run_type)
{
    // Vytvoření mutexu
    if(pthread_mutex_init(&mutex_udp, NULL) != 0)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se vytvořit MUTEX pro UDP scanner.\n");
        exit(ERR_INTER);
    }

    pthread_t udp_tid;

    // Vytvoření snifferu.
    pcap_t *udp_sniffer = make_sniffer(inter_ip, ip_dst, UDP_SNIFFER);

    // Vytvoření socketu v závislosti na rodině ip.
    int socket_udp;
    if(inter_ip.ip.family == AF_INET)
    {
        socket_udp = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    }
    else if(inter_ip.ip.family == AF_INET6)
    {
        socket_udp = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    }
    else
    {
        fprintf(stderr, "[ERROR]: Nezmámá chyba. Korupce ip_family v udp_scanneru.\n");
        exit(ERR_SENDER);
    }

    // Kontrola úspěšnosti vytvoření socketu.
    if(socket_udp == -1)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se vytvořit UDP socket.\n");
        exit(ERR_SENDER);
    }

    // Připojení socketu na interface.
    if(setsockopt(socket_udp, SOL_SOCKET, SO_BINDTODEVICE, inter_ip.interface_name, strlen(inter_ip.interface_name)) != 0)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se nabindovat interface na socket při UDP skenování.\n");
        exit(ERR_SENDER);
    }

    // Pro IPv4 socket bude IP hlavička vytvořena matuálně
    if(inter_ip.ip.family == AF_INET)
    {
        const int option = 1;
        if(setsockopt(socket_udp, IPPROTO_IP, IP_HDRINCL, &option, sizeof(option)) != 0)
        {
            fprintf(stderr, "[ERROR]: Nepodařilo se nastavit socket na IPv4 při UDP sknenování.\n");
            exit(ERR_SENDER);
        }
    }

    // Spuštění vlákna UDP snifferu.
    if(pthread_create(&udp_tid, NULL, run_udp_scanner, (void*)udp_sniffer) != 0)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se vatvořit vlákno pro udp_sniffer.\n");
        exit(ERR_THREAD);
    }

    // Reset informací o skenovaných portech
    reset_udp_status(udp_ports);

    // Odeslání packetů na všechny skenované porty z udp_ports.
    unsigned short port;
    while(!is_empty(udp_ports))
    {   
        port = get(udp_ports);
        if(!send_udp_packet(socket_udp, inter_ip.ip, ip_dst, port))
        {
            fprintf(stderr, "[INFO]: Nepodařilo se zaslat UDP packet na port %u.\n", port);
        }
    }

    // Počká se na zachycení poslední ho packetu 1,5 sec.
    usleep(1500000);

    // Ukončení snifferu.
    pcap_breakloop(udp_sniffer);
    // Pokud by sniffer nepřijal žádný packet, pořád by běžel. Ukončení vlákna.
    pthread_cancel(udp_tid);
    // Spojení vláken.
    pthread_join(udp_tid, NULL);

    // Uvolnění zdrojů.
    pcap_close(udp_sniffer);
    pthread_mutex_destroy(&mutex_udp);

    // Výpis výsledků UDP skenování podle modu (ALL|OPEN|CLOSED).
    printf("Výsledky UDP skenování portů %s\n", ip_dst.addr);
    printf("PORT\t\tSTATE\n");
    for(int i = 0; i < MAX_PORT_COUNT; i++)
    {
        if(udp_port_status[i] != UNSCANNED_PORT)
        {
            if(udp_port_status[i] == CLOSED_PORT)
            {
                if(run_type != GET_OPEN && run_type != GET_UNCLOSED)
                {
                    printf("%u/udp\t%sclosed\n", i, (i < 999) ? "\t" : "");
                }
            }
            else
            {
                if(run_type != GET_CLOSED)
                {
                    printf("%u/udp\t%sopen\n", i, (i < 999) ? "\t" : "");
                }
            }
        }
    }
}