/**
 * Modul implementující vytvoření snifferu.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#include "sniffer.h"


/**
 * Vytvoří sniffer v daném zachytávacího modu, odpospouchávající konkrétní ip.
 * ----------------------------------------
 * int_ip - interface a jeho ip na které poběží sniffer
 * dst    - ip které bude sniffer detekovat
 * sniffer_type - mod snifferu (SYN-ACK|RST|UDP)
 * ----------------------------------------
 * Funkce vytvoří sniffer připojený k interface specifikovaným int_ip,
 * poslouchající pouze packety přicházející z ip adresy uložené v dst,
 * a to pouze takové pakety, které specifikuje mod vybraný konstantou sniffer_type.
 * Pokud se vytvoření povede, bude navrácen ukazatel na sniffer. Pokud dojde
 * během vytváření k vybě (chyba alokace, nemožnost otevřít interface, ...),
 * bude program ukončen s příslušným návratovým kódem.
 */
pcap_t *make_sniffer(interface_ip_t int_ip, ip_t dst, const int sniffer_type)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 mask;
    bpf_u_int32 ip;
    pcap_t *handle;
    struct bpf_program compiled_filter;
    char filter[256] = {0,};

    // Získání masky pro daný interface
    if(pcap_lookupnet(int_ip.interface_name, &ip, &mask, errbuf) == -1)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se získat masku rozhraní %s.\n", int_ip.interface_name);
        ip = 0;
        mask = 0;
    }

    // Otevření interface
    handle = pcap_open_live(int_ip.interface_name, BUFSIZ, 1, 1000, errbuf);
    if(handle == NULL)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se otevřít zařízení %s.\n", int_ip.interface_name);
        exit(ERR_INTERFACE);
    }

    // Vytváření filtru snifferu podle jeho modu a rodiny poslouchané ip.
    if(int_ip.ip.family == AF_INET)
    {
        // IPv4 filtry
        if(sniffer_type == SYN_ACK_SNIFFER)
        {
            // filte SYN-ACK a nesmí být RST
            strcpy(filter, "tcp[tcpflags] & (tcp-syn|tcp-ack) != 0 and tcp[tcpflags] & (tcp-rst) == 0 and src ");
        }
        else if(sniffer_type == RST_SNIFFER)
        {
            // filtr RST-ACK nebo RST
            strcpy(filter, "tcp[tcpflags] & (tcp-rst|tcp-ack) != 0 or tcp[tcpflags] & (tcp-rst) != 0 and src ");
        }
        else if(sniffer_type == UDP_SNIFFER)
        {
            // filtr ICMP3
            strcpy(filter, "icmp[icmpcode]=3 and src ");
        }
        else
        {
            fprintf(stderr, "[ERROR]: Nezmámá chyba při vytváření snifferu. Korupce sniffer_type.\n");
            exit(ERR_INTER);
        }
        strcat(filter, dst.addr);
        
    }
    else
    {
        // IPv6 filtry
        if(sniffer_type == SYN_ACK_SNIFFER)
        {
            // filtr SYN-ACK nebo ACK
            strcpy(filter, "((tcp[13] == 0x12) or (ip6 and (ip6[6] == 0x06) and (ip6[53] == 0x12))) or ((tcp[13] == 0x10) or (ip6 and (ip6[6] == 0x06) and (ip6[53] == 0x10))) and src ");
        }
        else if(sniffer_type == RST_SNIFFER)
        {
            // filtr RST
            strcpy(filter, "((ip6[6] == 6 && ip6[53] & 0x04 == 0x04) || (ip6[6] == 6 && tcp[13] & 0x04 == 0x04)) and src ");
        }
        else if(sniffer_type == UDP_SNIFFER)
        {
            // filtr DESTINATION UNREACHED --> ICMP3 (PORT UNREACHED)
            strcpy(filter, "(ip6[6] == 0x3a) and (ip6[40] == 0x01) and src ");
        }
        else
        {
            fprintf(stderr, "[ERROR]: Nezmámá chyba při vytváření snifferu. Kprupce sniffer_type.\n");
            exit(ERR_INTER);
        }
        // Přídání source do filtru
        strcat(filter, dst.addr);
    }

    // Sestavení filtru
    if(pcap_compile(handle, &compiled_filter, filter, 0, ip) == -1)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se vytvořit ACK-SYN filter.\n");
        exit(ERR_SNIFFER);
    }

    // Přidání filtru do snifferu
    if(pcap_setfilter(handle, &compiled_filter) == -1)
    {
        fprintf(stderr, "[ERROR]: Nepodařilo se aplikovat ACK-SYN filter.\n");
        exit(ERR_SNIFFER);
    }

    // Uvolnění zdrojů.
    pcap_freecode(&compiled_filter);

    return handle;
}