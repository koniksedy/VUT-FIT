/**
 * Modul chybových kódů a nápovědy.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdio.h>


#define PRINT_HELP  fprintf(stderr, \
                "Příklad spuštění:\n"\
                "./ipk-scan {-i <interface>} --pu <port-ranges> --pt <port-ranges> {<option>} [<domain-name> | <IP-address>]\n"\
                "    -i          Přepínač umožňující manuální nastavení interface, kde argument\n"\
                "                představuje identifikátor tohoto rozhraní. Pokud není přepínač\n"\
                "                použit, je systémem vybráno první neloopbaskové interface\n"\
                "                v případě, že není skenovaný localhost. Pokud je scanovaný \n"\
                "                localhost, je standartně vybráno loopbackové interface.\n\n"\
                "    -u, --pu    Je možno použít zkrácenou variantu -u. Argumentem jsou\n"\
                "                skenované UDP porty. Rezervonavý port 0 je povoleno skenovat.\n"\
                "                Rozsah skenovaných portů může být zadán například následovně:\n"\
                "                1,5,9,7 nebo 1-3600 nebo 1-10,22,36-75. Hranice intervalů jsou\n"\
                "                odděleny pomlčkou. Jednotlivá čísla a intervaly jsou odděleny\n"\
                "                čárkou. Nepoužívají se mezery. Za posledním číslem není čárka.\n\n"\
                "    -t, --pt    Je možno použít zkrácenou variantu -t. Argumentem jsou skenované\n"\
                "                TCP porty. Rezervonavý port 0 je povoleno skenovat.\n"\
                "                Rozsah skenovaných portů může být zadán například následovně:\n"\
                "                1,5,9,7 nebo 1-3600 nebo 1-10,22,36-75. Hranice intervalů jsou\n"\
                "                odněleny pomlčkou. Jednotlivá čísla a intervaly jsou odděleny\n"\
                "                čárkou. Nepoužívají se mezery. Za posledním číslem není čárka.\n\n"\
                "    --only-open \n"\
                "                Ve výsledcích budou zobrazeny pouze otevřené porty.\n"\
                "                Nelze kombinovat s ostatními přepínači filtrujícími výsledky.\n\n"\
                "    --only-closed\n"\
                "                Ve výsledcích budou zobrazeny pouze zavřené porty.\n"\
                "                Nelze kombinovat s ostatními přepínači filtrujícími výsledky.\n\n"\
                "    --only-unclosed\n"\
                "                Ve výsledcích budou zobrazeny pouze otevřené a filtrované porty.\n"\
                "                Nelze kombinovat s ostatními přepínači filtujícími výsledky.\n"\
                "    -h, --help  Zobrazení nápovědy programu ips-scan.\n\n"\
                "    Pořadí přepínačů lze zaměňovat, ale skenovaná adresa/jméno domény musí být \n"\
                "    vždy jako posledním argumentem spuštění. Vždy musí být zadán alespoň \n"\
                "    jeden port pro skeování.\n"\
                "    Pro spušnění je potřeba mít root oprávnění.\n\n");

#define ERR_OK 0            // OK
#define ERR_ARG 1           // chyba argumentů spuštění
#define ERR_INTERFACE 2     // chyba interface (nenalezeno, ...)
#define ERR_IP 3            // chyba ip (špatná ip, nenalezeno, ...)
#define ERR_SNIFFER 4       // chyba vytvoření snifferu
#define ERR_THREAD 5        // chyba vytvoření vlákna
#define ERR_SENDER 6        // chyba odesílání packetu
#define ERR_INTER 99        // vnitřní chyba (malloc, ...)

#endif