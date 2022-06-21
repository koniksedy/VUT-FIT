/**
 * Hlavní mudul programu skenování portů.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#include "ipk-scan.h"


int main(int argc, char *argv[])
{
    port_queue_t *tcp_ports = NULL;
    port_queue_t *udp_ports = NULL;
    char *ip_raw = NULL;
    char *inter_raw = NULL;

    // Načtení a zpracování argumentů spuštění.
    // Do proměnné type_of_run se nahraje mod vypisování výsledku (ALL/OPEN/CLOSED/UNCLOSED).
    // V případě chyby vypíše chybovou hlášku a ukončí program.
    short int type_of_run = parse_arg(&tcp_ports, &udp_ports, &ip_raw, &inter_raw, argc, argv);

    // Kontrola, zda byly zadány alespoň nějaké porty pro skenování.
    if(tcp_ports == NULL && udp_ports == NULL)
    {
        fprintf(stderr, "[ERROR]: Nebyly zadány žádné parametry protokolu nebo porty pro skenování.\n");
        PRINT_HELP;
        exit(ERR_ARG);
    }

    // názvev interface a jeho ip
    interface_ip_t inter_ip;
    memset(inter_ip.interface_name, 0, MAX_INTERF_LEN);
    memset(inter_ip.ip.addr, 0, MAX_IP_LEN);

    // ip adresa cíle
    ip_t ip_dst;
    memset(ip_dst.addr, 0, MAX_IP_LEN);

    // Načte interface s jeho ip, a také ip cíle. Nejdříve načte ip cíle.
    // Na základě verze ip adresy cíle se vyhledá neloopbackový interface
    // se stejnou rodinou ip. Pokud je cílovou adresou localhost a při spuštění
    // nebylo interface explicitně stanoveno, je vybráno právě interface s loopbackem.
    ip_dst.family = AF_UNSPEC;
    load_ip(&ip_dst, ip_raw);
    if(inter_raw == NULL)
    {   
        // Interface nebylo uvedeno, musí se načíst.
        if(strcmp(ip_dst.addr, "127.0.0.1") == 0)
        {
            inter_ip.ip.family = AF_INET;
            strcpy(inter_ip.ip.addr, "127.0.0.1");
            strcpy(inter_ip.interface_name, "lo");
        }
        else if(strcmp(ip_dst.addr, "::1") == 0)
        {
            inter_ip.ip.family = AF_INET6;
            strcpy(inter_ip.ip.addr, "::1");
            strcpy(inter_ip.interface_name, "lo");
        }
        else
        {
            load_interface_ip(&inter_ip, NULL, ip_dst.family);
        }
    }
    else
    {
        // Interface bylo uvedeno, kontrola a následné načtení.
        load_interface_ip(&inter_ip, inter_raw, ip_dst.family);
    }

    // Skenování tcp portů, pokud byly zadány.
    if(tcp_ports != NULL)
    {
        scan_tcp(inter_ip, ip_dst, tcp_ports, type_of_run);
    }
    // Skenování udp portů, pokud byly zadány.
    if(udp_ports != NULL)
    {
        scan_udp(inter_ip, ip_dst, udp_ports, type_of_run);
    }

    port_queue_dtor(&tcp_ports);
    port_queue_dtor(&udp_ports);
    
    return 0;    
}