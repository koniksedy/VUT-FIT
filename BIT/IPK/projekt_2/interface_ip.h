/**
 * Hlavičková soubor modulu načítajícího interface a jeho ip.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#ifndef __INTERFACE_IP_H__
#define __INTERFACE_IP_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>  // getifaddrs, getaddrinfo
#include <ifaddrs.h>    // ifaddrs, getifaddrs
#include <sys/socket.h> // getaddrinfo
#include <arpa/inet.h>  // inet_ntop
#include <net/if.h>     // IFF_UP, IFF_RUNNING, IFF_LOOPBACK
#include <netdb.h>      // getaddrinfo
#include "error.h"      // chybové kódy


#define MAX_IP_LEN 46
#define MAX_INTERF_LEN 257

typedef struct {
    int family;
    char addr[MAX_IP_LEN];
} ip_t;

typedef struct {
    ip_t ip;
    char interface_name[MAX_INTERF_LEN];
} interface_ip_t;


/**
 * Nalezne neloopbackové interface z dané rodiny ip.
 * ------------------------------------
 * interf_name   - ukazatel pro nahrání jména získaného interface
 * exp_ip_family - hledaný typ ip
 * ------------------------------------
 * V případě uspěšného získání jména neloopbaskového interface z dané rodiny ip,
 * jej funkce nahraje do interf_name a vrátí true. V případě neúspěchu vrátí false.
 */
bool find_device(char **interf_name, int exp_ip_family);

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
void load_interface_ip(interface_ip_t *inter_ip, char in[], int exp_ip_family);

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
void load_ip(ip_t *ip_dst, char ip_raw[]);

#endif