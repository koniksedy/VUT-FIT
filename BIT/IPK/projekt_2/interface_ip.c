/**
 * Implementace modulu načítajícího interface a jeho ip.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#include "interface_ip.h"


/**
 * Nalezne neloopbackové interface z dané rodiny ip.
 * ------------------------------------
 * interf_name   - ukazatel pro nahrání jména získaného interface
 * exp_ip_family - hledaný typ ip
 * ------------------------------------
 * V případě uspěšného získání jména neloopbaskového interface z dané rodiny ip,
 * jej funkce nahraje do interf_name a vrátí true. V případě neúspěchu vrátí false.
 */
bool find_device(char **interf_name, int exp_ip_family)
{
    struct ifaddrs *dev;
    struct ifaddrs *to_del; // pomocná proměnná pro dealokaci zdrojů

    // Získání interface iterátoru
    getifaddrs(&dev);

    to_del = dev;

    // Hledání interface vyhovujícího požadavkům
    while(dev != NULL)
    {   
        // Neloopbashoké s dané rodiny ip.
        if((dev->ifa_flags & IFF_LOOPBACK) == 0 && (dev->ifa_flags & IFF_UP) != 0 && (dev->ifa_flags & IFF_RUNNING) != 0)
        {
            // Kontrola očekávané ip rodiny, pokud nesedí, vezme se další interface
            if(exp_ip_family == AF_INET && dev->ifa_addr->sa_family != AF_INET)
            {
                dev = dev->ifa_next;
                continue;
            }
            if(exp_ip_family == AF_INET6 && dev->ifa_addr->sa_family != AF_INET6)
            {
                dev = dev->ifa_next;
                continue;
            }
            if(dev->ifa_addr == NULL)
            {
                dev = dev->ifa_next;
                continue;
            }
            
            // Alokace místa pro jméno interface
            *interf_name = (char*) malloc(strlen(dev->ifa_name)+1);
            if(*interf_name == NULL)
            {
                fprintf(stderr, "[ERROR]: Chyba alokace paměti pro jméno defaultního interface.\n");
                exit(ERR_INTER);
            }
            strcpy(*interf_name, dev->ifa_name);
            break;
        }
        // Nenalezena shoda, získej další interface
        dev = dev->ifa_next;
    }

    // Uvolnění zdrojů.
    freeifaddrs(to_del);

    return (*interf_name == NULL) ? false : true;
}

/**
 * Našte jméno a ip interface, dle požadavků.
 * ------------------------------------
 * inter_ip      - ukazatel na strukturu interface (jméno, ip, rodina ip)
 *               - na základě získaného interface funkce vyplní data struktůry
 * in            - vstupní jméno hledaného interface
 *               - NULL znamemá, že má funkce sama najít interface
 * exp_ip_family - požadovaná rodina ip u interface
 * ------------------------------------
 * Funkce na základě hodnoty in a exp_ip_family načte příslušné interface a jeho ip.
 * Tyto získané hodnoty uloží do struktůry odkazované ukazatelem inter_ip.
 * Pokud během načítání dojde k chybě (nenalezene interface, chyba alokace, ...)
 * Ukončí funkce program s příspušným návratovým kódem.
 */
void load_interface_ip(interface_ip_t *inter_ip, char in[], int exp_ip_family)
{
    char *interface_name = NULL;
    if(in == NULL)
    {
        // Interface není zadánu, potřeba najít.
        // Do interface_name se nahraje jeho jméno.
        if(!find_device(&interface_name, exp_ip_family))
        {
            fprintf(stderr, "[ERROR]: Chyba, nenalezeno základní rozhraní.\n");
            exit(ERR_INTERFACE);
        }
    }
    else
    {   
        // Interface známe.
        // Alokace interface_name a kopie jména interface.
        interface_name = (char*) malloc(strlen(in)+1);
        if(interface_name == NULL)
        {
            fprintf(stderr, "[ERROR]: Chyba, nepovedla se alokace paměti pro jméno interface.\n");
            exit(ERR_INTER);
        }
        strcpy(interface_name, in);
    }

    struct ifaddrs *interface;
    struct ifaddrs *to_del;     // pomocná proměnná pro dealokaci zdrojů

    // Získání interface iterátoru
    getifaddrs(&interface);

    to_del = interface;

    // Hledání interface daného jména.
    // Pro interface nalezené v předchozím kroku se jedná o formalitu.
    while(interface != NULL)
    {
        // Hledání interface se shodným názvem. Na velikosti písmen nezáleží.
        if(strcasecmp(interface->ifa_name, interface_name) == 0)
        {
            // Pokud nesedí požadovaná rodina ip, další interface.
            if(exp_ip_family == AF_INET && interface->ifa_addr->sa_family != AF_INET)
            {
                interface = interface->ifa_next;
                continue;
            }
            if(exp_ip_family == AF_INET6 && interface->ifa_addr->sa_family != AF_INET6)
            {
                interface = interface->ifa_next;
                continue;
            }
            if(interface->ifa_addr == NULL)
            {
                interface = interface->ifa_next;
                continue;
            }

            // Interface nalezemo.
            // Do struktury se nahrají ip adresy dle rodiny ip.
            if(interface->ifa_addr->sa_family == AF_INET)
            {
                inter_ip->ip.family = AF_INET;

                if(inet_ntop(AF_INET, &((struct sockaddr_in *)(interface->ifa_addr))->sin_addr, inter_ip->ip.addr, 45) == NULL)
                {
                    fprintf(stderr, "[ERROR]: Chyba při získávání lokální IPv4 adresy.\n");
                }
                break;
            }
            else if (interface->ifa_addr->sa_family == AF_INET6)
            {
                inter_ip->ip.family = AF_INET6;
                if(inet_ntop(AF_INET6, &((struct sockaddr_in6 *)(interface->ifa_addr))->sin6_addr, inter_ip->ip.addr, 45) == NULL)
                {
                    fprintf(stderr, "[ERROR]: Chyba při získávání lokální IPv6 adresy.\n");
                }
                break;
            }
        }

        // Žádné interface nevyhovuje, získej další.
        interface = interface->ifa_next;
    }

    // Žádné interface s daným jménem nebylo nalezeno.
    if(interface == NULL)
    {
        fprintf(stderr, "[ERROR]: Neexisutjící rozhraní %s\n.", interface_name);
        exit(ERR_INTERFACE);
    }

    // Přípiš dlouhé jméno interface. Interní chyba malý buffer.
    if(sizeof(interface_name) > 257)
    {
        fprintf(stderr, "[ERROR]: Přípiš velké jméno rozhraní.\n");
        exit(ERR_INTER);
    }

    // Kopie jména interface do požadované struktury.
    strcpy(inter_ip->interface_name, interface_name);

    // Uvolnění zdrojů.
    free(interface_name);
    freeifaddrs(to_del);
}

/**
 * Oveří zadané ip/jméno hosta a do struktury nahrajej ip adresu a ip rodinu.
 * ----------------------------------------
 * ip_dst - ukazatel na výstupní struktůru, do které se nahraní získaná data o ip/jménu hosta
 * ip_raw - textový řetězec ip/jména hosta, není jisté, že se nejedná o nesmysl.
 * ----------------------------------------
 * Funkce v případě uspěšného ověření a získání ip adresy nahraje tuto 
 * informaci spolu s ip rodinou do struktury, na kterou ukazuje ip_dst.
 * Pokud se vyskytne chyba ukončí funkce program s příslušným návratovým kódem.
 */
void load_ip(ip_t *ip_dst, char ip_raw[])
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    struct addrinfo *res = NULL;

    hints.ai_family = ip_dst->family;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;  // ip s jakým koliv protokolem

    // Získaní adresy na základě ip_raw
    if(getaddrinfo(ip_raw, NULL, &hints, &res) != 0)
    {
        fprintf(stderr, "[ERROR]: Chyba ip adresy, nebo jména hosta, zkontrolujte formát.\n");
        exit(ERR_IP);
    }

    // Nahrání dat do výstupní struktůry podle rodiny ip.
    if(res->ai_family == AF_INET)
    {
        if(inet_ntop(AF_INET, &((struct sockaddr_in*)res->ai_addr)->sin_addr, ip_dst->addr, 45) == NULL)
        {
            fprintf(stderr, "[ERROR]: Cyba při transformování IPv4 adresy.\n");
            exit(ERR_INTER);
        }
        ip_dst->family = AF_INET;
    }
    else if(res->ai_family == AF_INET6)
    {
        if(inet_ntop(AF_INET6, &((struct sockaddr_in6*)res->ai_addr)->sin6_addr, ip_dst->addr, 45) == NULL)
        {
            fprintf(stderr, "[ERROR]: Cyba při transformování IPv6 adresy.\n");
            exit(ERR_INTER);
        }
        ip_dst->family = AF_INET6;
    }
    else
    {
        fprintf(stderr, "[ERROR]: Interní chyba při získávání IP. Korupce IP verzí.\n");
        exit(ERR_INTER);
    }

    // Uvolnění zdrojů
    freeaddrinfo(res);
}