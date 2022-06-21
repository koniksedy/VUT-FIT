/**
 * Modul implementující TCP skener portů.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#include "tcp_scanner.h"


pthread_mutex_t mutex_tcp;  // synchronizační semafor
char tcp_port_status[MAX_PORT_COUNT];   // veškeré možné porty

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
bool reset_tcp_status(port_queue_t *src)
{
    // Vynulování všech portů.
    memset(tcp_port_status, UNSCANNED_PORT, MAX_PORT_COUNT);

    // Hloubková kopie fronty
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

        tcp_port_status[port] = SCANNED_PORT;
    }

    // Uvolnění zdrojů.
    port_queue_dtor(&tmp);

    return true;
}

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
void syn_ack_detected(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    // ZAČÁTEK PŘEVZATÉ ČÁSTI
    // Následující kód byl převzat z: https://code.google.com/p/pcapsctpspliter/issues/detail?id=6
    // Autor: Joshua Robinson, (joshua.robinson@gmail.com)
    // Detekce IPv4 nebo IPv6
    int packet_type = ((int) (packet[12]) << 8) | (int) packet[13];
    // KONEC PŘEVZATÉ ČÁSTI

    // Nalezení portu, ze kterého přišla odpověď
    unsigned int port;
    const struct tcphdr *tcp_header;
    if(packet_type == ETHER_TYPE_IP4)
    {
        tcp_header = (struct tcphdr*)(packet + ETHER_SIZE + IPV4_SIZE);
        port = ntohs(tcp_header->source);
    }
    else if(packet_type == ETHER_TYPE_IP6)
    {
        tcp_header = (struct tcphdr*)(packet + ETHER_SIZE + IPV6_SIZE);
        port = ntohs(tcp_header->source);
    }
    else
    {
        fprintf(stderr, "[ERROR]: Neznámý ETHER_TYPE %x.\n", packet_type);
        return;
    }

    // Občas filtr snifferu nefunguje správně.
    // Nevím kde je chyba. Potřeba korekce.
    if(!tcp_header->ack || tcp_header->rst)
    {
        return;
    }

    // Začátek kritické sekce
    pthread_mutex_lock(&mutex_tcp);
    // Jako otevřený lze označit jakýkoliv skenovaný port.
    if(tcp_port_status[port] != UNSCANNED_PORT)
    {
        // Označení portu jako otevřený.
        tcp_port_status[port] = OPEN_PORT;
    }
    pthread_mutex_unlock(&mutex_tcp);
    // Konec kritické sekce
}

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
void rst_detected(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    // ZAČÁTEK PŘEVZATÉ ČÁSTI
    // Následující kód byl převzat z: https://code.google.com/p/pcapsctpspliter/issues/detail?id=6
    // Autor: Joshua Robinson, (joshua.robinson@gmail.com)
    // Detekce IPv4 nebo IPv6
    int packet_type = ((int) (packet[12]) << 8) | (int) packet[13];
    // KONEC PŘEVZATÉ ČÁSTI

    // Nalezení portu, ze kterého přišla odpověď
    unsigned int port;
    if(packet_type == ETHER_TYPE_IP4)
    {
        const struct tcphdr *tcp_header;
        tcp_header = (struct tcphdr*)(packet + ETHER_SIZE + IPV4_SIZE);
        port = ntohs(tcp_header->source);
    }
    else if(packet_type == ETHER_TYPE_IP6)
    {
        const struct tcphdr *tcp_header;
        tcp_header = (struct tcphdr*)(packet + ETHER_SIZE + IPV6_SIZE);
        port = ntohs(tcp_header->source);
    }
    else
    {
        fprintf(stderr, "[ERROR]: Neznámý ETHER_TYPE %x.\n", packet_type);
        return;
    }

    // Začátek kritické sekce.
    pthread_mutex_lock(&mutex_tcp);
    // Pokud již byl port označen jako otevřený, nachá jej tak.
    if(tcp_port_status[port] != UNSCANNED_PORT && tcp_port_status[port] != OPEN_PORT)
    {
        // Označení portu jako zavřený.
        tcp_port_status[port] = CLOSED_PORT;
    }
    pthread_mutex_unlock(&mutex_tcp);
    // Konec kritické sekce.
}

/**
 * Začátek vlákna SYN-ACK snifferu.
 * ----------------------------------------
 * h - ukazatel na sniffer
 * ----------------------------------------
 * Funkce vrací vždy úspěšně NULL.
 */
void *run_syn_ack_scanner(void *h)
{
    pcap_loop((pcap_t*)h, -1, syn_ack_detected, NULL);
    return NULL;
}

/**
 * Začátek vlákna RST snifferu.
 * ----------------------------------------
 * h - ukazatel na sniffer
 * ----------------------------------------
 * Funkce vrací vždy úspěšně NULL.
 */
void *run_rst_scanner(void *h)
{
    pcap_loop((pcap_t*)h, -1, rst_detected, NULL);
    return NULL;
}


/**
 * Výpočet kontrolního součtu TCP/IP
 * ----------------------------------------
 * !! Celá funkce je převzata z: https://www.winpcap.org/pipermail/winpcap-users/2007-July/001984.html
 * Autor: He Tao (hetaozju@gmail.com)
 * ----------------------------------------
 * buffer - TCP/IP hlavička
 * size   - velikost bufferu
 * ----------------------------------------
 * Funkce vypočte kontrolní součet TCP/IP hlavičky.
 */
unsigned short check_sum(unsigned short *buffer, int size)
{
    unsigned long cksum=0;
    while(size >1)
    {
        cksum+=*buffer++;
        size -=sizeof(unsigned short);
    }
    if(size)
        cksum += *(unsigned char*)buffer;

    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return (unsigned short)(~cksum);
}

/**
 * Sestaví TCP packe a nastaví ip adresy a porty.
 * ----------------------------------------
 * src  - ip adresa zdroje
 * dst  - ip adresa cíle
 * port - port na který bude packet odeslán
 * ----------------------------------------
 * V případě úspěchu vrácí funkce ukazatel na TCP packet,
 * pokud pojde k chybě (chyba alokace, chyba ip adresy, ...) vrátí NULL).
 */
char *build_tcp_packet(ip_t src, ip_t dst, int port)
{
    struct iphdr *ip_header;
    struct tcphdr *tcp_header;
    pseudo_header_t pseudo_header; 

    // Alokuje prostor pro packet
    char *datagram = (char*) calloc(4096, 1);
    if(datagram == NULL)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se postavit tcp packet. Chyba mallocu.\n");
        return NULL;
    }

    // Podle rodiny ip nastaví začátek TCP hlavičky v packetu
    if(dst.family == AF_INET)
    {
        ip_header = (struct iphdr*)datagram;
        tcp_header = (struct tcphdr*)(datagram + sizeof(struct iphdr));
    }
    else if(dst.family == AF_INET6)
    {
        // Pro IPv6 nebude sestavována IP hlavička, to se ponechá na systému.
        tcp_header = (struct tcphdr*)datagram;
    }
    else
    {
        fprintf(stderr, "[ERROR]: Zvláštní chyba, korupce ip.family při vytváření TCP packetu.\n");
        free(datagram);
        return NULL;
    }

    // Vynulování packetu
    memset(datagram, 0, 4096);

    tcp_header->source = htons(1234);   // Nezáleží na hodnotě
    tcp_header->dest = htons(port);     // Cílový port
    tcp_header->seq = htonl(rand());    // Nezáleží na hodnotě
    tcp_header->ack_seq = 0; 
    tcp_header->doff = sizeof(struct tcphdr) / 4;
    tcp_header->syn = 1;    // SYN
    tcp_header->window = htons(1024);   // Nezáleží na hodnotě
    tcp_header->check = 0;      // Kontrolní součet se přidá později
    tcp_header->urg_ptr = 0;    // Bez urgentní značky

    // IPv4 hlavička
    if(dst.family == AF_INET)
    {
        tcp_header->doff = 5;
        ip_header->ihl = 5;
        ip_header->version = 4;
        ip_header->tos = 0;
        ip_header->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
        ip_header->id = htons(rand());  // Nezáleží na hodnotě, jeden fragment
        ip_header->frag_off = 0; 
        ip_header->ttl = 64;
        ip_header->protocol = IPPROTO_TCP;
        ip_header->check = 0;   // Bude doplněno systémem

        // Nahrání adresy zdroje.
        if(inet_pton(AF_INET, src.addr, &ip_header->saddr) != 1)
        {
            fprintf(stderr, "[ERROR]: Chyba převodu IPv4 zdrojové adresy při vytváření TCP packetu.\n");
            free(datagram);
            return NULL;
        }

        // Nahrání adresy cíle.
        if(inet_pton(AF_INET, dst.addr, &ip_header->daddr) != 1)
        {
            fprintf(stderr, "[ERROR]: Chyba přeovdu IPv4 cílové adresy při vytváření TCP packetu.\n");
            free(datagram);
            return NULL;
        }

        // Vytvoření pseudohlavičky IP pro výpočet kontrolního součtu.
        pseudo_header.src_addr = ip_header->saddr;
        if(inet_pton(AF_INET, dst.addr, &pseudo_header.dst_addr) != 1)
        {
            fprintf(stderr, "[ERROR]: Chyha převodu IPv4 cílové adresy pro pseudo_header.\n");
            free(datagram);
            return NULL;
        }
        pseudo_header.res = 0;
        pseudo_header.protocol = IPPROTO_TCP;
        pseudo_header.len = htons(sizeof(struct tcphdr));
        memcpy(&pseudo_header.tcp, tcp_header, sizeof(struct tcphdr));

        // Výpočet kontrolního součtu.
        tcp_header->check = check_sum((unsigned short *)&pseudo_header, sizeof(pseudo_header_t));
    }

    return datagram;    
}

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
bool send_syn_packet(int socket_tcp, ip_t src, ip_t dst, int port)
{   
    // Vytvoří TCP packet
    char *packet = build_tcp_packet(src, dst, port);
    if(packet == NULL)
    {
        fprintf(stderr, "[ERROR]: Nepodařil se vytvořit TCP packet.\n");
        return false;
    }

    // Podle rodina ip nastaví socket a odešle pasket.
    if(src.family == AF_INET)
    {
        struct sockaddr_in socket_addr;
        memset(&socket_addr, 0, sizeof(socket_addr));

        // Nastavení cílovéhoh portu a rodiny ip 
        socket_addr.sin_port = htons(port);
        socket_addr.sin_family = src.family;

        // Nastavení ip adresy cíle.
        if(inet_pton(src.family, dst.addr, &socket_addr.sin_addr) != 1)
        {
            fprintf(stderr, "[ERROR]: Chyba při kopírování IPv4 adresy cíle do socket_addr.\n");
            return false;
        }

        // Odestání TCP packetu pomocí IPv4.
        if((sendto(socket_tcp, packet, sizeof(struct iphdr) + sizeof(struct tcphdr), 0,
            (struct sockaddr*)&socket_addr, sizeof(socket_addr))) == -1)
        {
            fprintf(stderr, "[ERROR]: Nepodařilo se odeslat TCP packet při IPv4.\n");
            return false;
        }
    }
    else
    {
        struct sockaddr_in6 socket_addr;
        memset(&socket_addr, 0, sizeof(socket_addr));

        // Nastavení rodiny ip.
        // Pro IPv6 musí být port 0, informace o portu je v packetu.
        socket_addr.sin6_port = 0;  
        socket_addr.sin6_family = src.family;

        // Nastavení ip adresy zdroje.
        if(inet_pton(src.family, dst.addr, &socket_addr.sin6_addr) != 1)
        {
            fprintf(stderr, "[ERROR]: Chyba při kopírování IPv6 adresy cíle do socket_addr.\n");
            return false;
        }

        // Odeslání TCP packetu pomocí IPv6.
        if((sendto(socket_tcp, packet, sizeof(struct tcphdr), 0, (struct sockaddr*)&socket_addr, sizeof(socket_addr))) == -1)
        {
            fprintf(stderr, "[ERROR]: Nepodařilo se odeslat TCP packet při IPv6.\n");
            return false;
        }

    }

    // Uvolnění zdrojů.
    free(packet);

    return true;
}

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
void scan_tcp(interface_ip_t inter_ip, ip_t ip_dst, port_queue_t *tcp_ports, short int run_type)
{
    // Vytvoření mutexu
    if(pthread_mutex_init(&mutex_tcp, NULL) != 0)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se vytvořit MUTEX pro TCP scanner.\n");
        exit(ERR_INTER);
    }

    pthread_t syn_ack_tid;
    pthread_t rst_tid;

    // Vytvoření snifferů (SYN-ACK a RST)
    pcap_t *syn_ack_sniffer = make_sniffer(inter_ip, ip_dst, SYN_ACK_SNIFFER);
    pcap_t *rst_sniffer = make_sniffer(inter_ip, ip_dst, RST_SNIFFER);

    // Vytvoření socketu v závislosti na rodině ip.
    int socket_tcp;
    if(inter_ip.ip.family == AF_INET)
    {
        socket_tcp = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    }
    else if(inter_ip.ip.family == AF_INET6)
    {
        socket_tcp = socket(AF_INET6, SOCK_RAW, IPPROTO_TCP);
    }
    else
    {
        fprintf(stderr, "[ERROR]: Nezmámá chyba. Korupce ip_family v tcp_scanneru.\n");
        exit(ERR_SENDER);
    }

    // Kontrola úspěšného vytvoření socketu
    if(socket_tcp == -1)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se vytvořit TCP socket.\n");
        exit(ERR_SENDER);
    }

    // Připojení socketu na interface
    if(setsockopt(socket_tcp, SOL_SOCKET, SO_BINDTODEVICE, inter_ip.interface_name, strlen(inter_ip.interface_name)) != 0)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se nabindovat interface na socket při TCP skenování.\n");
        exit(ERR_SENDER);
    }


    // Nastavení hlavičky socketu podle rodiny ip
    if(inter_ip.ip.family == AF_INET)
    {
        // IP hlavička bude vytvořena manuálně
        const int option = 1;
        if(setsockopt(socket_tcp, IPPROTO_IP, IP_HDRINCL, &option, sizeof(option)) != 0)
        {
            fprintf(stderr, "[ERROR]: Nepodařilo se nastavit socket na IPv4 při TCP sknenování.\n");
            exit(ERR_SENDER);
        }
    }
    else
    {
        // IP hlavička bude vytvořena systémem
        const int option = 16;
        if(setsockopt(socket_tcp, IPPROTO_IPV6, IPV6_CHECKSUM, &option, sizeof(option)) != 0)
        {
            fprintf(stderr, "[ERROR]: Nepodařilo se nastavit socket na IPv6 při TCP sknenování.\n");
            exit(ERR_SENDER);
        }
    }


    // Spuštění vlákna SYN-ACK sniffer
    if(pthread_create(&syn_ack_tid, NULL, run_syn_ack_scanner, (void*)syn_ack_sniffer) != 0)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se vatvořit vlákno pro syn_ack_sniffer.\n");
        exit(ERR_THREAD);
    }

    // Spuštění vlákna RST sniffer
    if(pthread_create(&rst_tid, NULL, run_rst_scanner, (void*)rst_sniffer) != 0)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se vytvořit vlákno pro rst_sniffer.\n");
        pcap_breakloop(syn_ack_sniffer);
        pthread_cancel(syn_ack_tid);
        pthread_join(syn_ack_tid, NULL);
    }

    // Reset informací o skenovaných portech
    reset_tcp_status(tcp_ports);

    // Odeslání packetů na všechny skenované porty v tcp_ports
    unsigned short port;
    while(!is_empty(tcp_ports))
    {
        port = get(tcp_ports);
        if(!send_syn_packet(socket_tcp, inter_ip.ip, ip_dst, port))
        {
            fprintf(stderr, "[INFO]: Nepodařilo se zaslat TCP packet na port %u.\n", port);
        }
    }

    // Počká se na zachycení posledního packetu 1,5 sec
    usleep(1500000);

    // Pro poty, ze kterých nepřišla odpověď bude packet znovu odeslán.
    bool repeat_tcp = false;
    for(int i = 0; i < 65536; i++)
    {
        // Začátek kritické části.
        pthread_mutex_lock(&mutex_tcp);
        if(tcp_port_status[i] == SCANNED_PORT)
        {
            repeat_tcp = true;
            if(!send_syn_packet(socket_tcp, inter_ip.ip, ip_dst, port))
            {
                fprintf(stderr, "[INFO]: Nepodařilo se zaslat TCP packet na port %u.\n", port);
            }
        }
        pthread_mutex_unlock(&mutex_tcp);
        // Konec kritické části.
    }

    // Pokud se opakovalo odesílání, počká se na zachycení posledního packetu 1,5 sec
    if(repeat_tcp)
    {
        usleep(1500000);
    }

    // Ukončení snifferů
    pcap_breakloop(syn_ack_sniffer);
    pcap_breakloop(rst_sniffer);

    // Pokud by sniffer nepřijal žádný packet, pořád by běžel. Ukončení vlákna.
    pthread_cancel(syn_ack_tid);
    pthread_cancel(rst_tid);

    // Spojení vláken
    pthread_join(syn_ack_tid, NULL);
    pthread_join(rst_tid, NULL);

    // Uvolnění zdrojů
    pcap_close(syn_ack_sniffer);
    pcap_close(rst_sniffer);
    pthread_mutex_destroy(&mutex_tcp);

    // Výpis výsledků v závislosti na modu výpisu.
    printf("Výsledky TCP skenování portů %s\n", ip_dst.addr);
    printf("PORT\t\tSTATE\n");
    for(int i = 0; i < MAX_PORT_COUNT; i++)
    {
        if(tcp_port_status[i] == OPEN_PORT)
        {
            if(run_type != GET_CLOSED)
            {
                printf("%u/tcp\t%sopen\n", i, (i < 999) ? "\t" : "");
            }
        }
        else if(tcp_port_status[i] == CLOSED_PORT)
        {
            if(run_type != GET_OPEN && run_type != GET_UNCLOSED)
            {
                printf("%u/tcp\t%sclosed\n", i, (i < 999) ? "\t" : "");
            }
        }
        else if(tcp_port_status[i] == SCANNED_PORT)
        {
            if(run_type != GET_OPEN && run_type != GET_CLOSED)
            {
                printf("%u/tcp\t%sfiltered\n", i, (i < 999) ? "\t" : "");
            }
        }   
    }   
}