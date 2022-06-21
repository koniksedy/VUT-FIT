/**
 * Hlavičnový soubor modulu sniffer.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#ifndef __SNIFFER_H__
#define __SNIFFER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include "interface_ip.h"   // interface_ip_t, ip_t


// maximální počet portů 0-65535
#define MAX_PORT_COUNT 65536    

// Ether types pro IPv4 a IPv6
// viz: https://www.iana.org/assignments/ieee-802-numbers/ieee-802-numbers.xhtml
#define ETHER_TYPE_IP4 0x800
#define ETHER_TYPE_IP6 0x86dd

// Informace o stavu skenovanách portů
#define CLOSED_PORT -1      // port uzavřen
#define UNSCANNED_PORT 0    // port nebude skenován
#define SCANNED_PORT 1      // port bude skenován
#define OPEN_PORT 2         // port otevřen

// Mody snifferu 
#define SYN_ACK_SNIFFER 3   // zachytává SYN-ANC
#define RST_SNIFFER 4       // zachytává RST a RST-ACK
#define UDP_SNIFFER 5       // zachytává ICMP3

// Pomocné definice velikostí hlaviček
#define ETHER_SIZE 14
#define IPV4_SIZE 20
#define IPV6_SIZE 40

/**
 * Vytvoří sniffer v daném zachytávacího modu, odpospouchávající konkrétní ip.
 * ----------------------------------------
 * int_ip - interface a jeho ip na které poběží sniffer
 * dst    - ip které bude sniffer detekovat
 * sniffer_type - mod snifferu (SYN-ACK|RST|UDP)
 * ----------------------------------------
 * Funkce vytvoří sniffer připojený k interface specifikovaným int_ip,
 * poslouchající pouze packety přicházející z ip adresy uložené v dst,
 * a to pouze takové pakety, které specifikuje mod vybraný konstantou 
 * sniffer_type (SYN-ACK, RST, ICMP3).
 * Pokud se vytvoření povede, bude navrácen ukazatel na sniffer. Pokud dojde
 * během vytváření k vybě (chyba alokace, nemožnost otevřít interface, ...),
 * bude program ukončen s příslušným návratovým kódem.
 */
pcap_t *make_sniffer(interface_ip_t int_ip, ip_t dst, const int sniffer_type);

#endif