/**
 * Hlavičkový soubor modulu zpracovávajícího argumenty programu.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // strlen
#include "port_queue.h"
#include "error.h"      // chybové kódy

// Definice modu výpisu výsledků skeneru
#define GET_CLOSED 0    // pouze uzavřené porty
#define GET_OPEN 1      // pouze otevřené porty
#define GET_UNCLOSED 2  // pouze otevřené a filtrované porty
#define GET_ALL 3       // všechny výsledky optevřené + filterované + zavřené

/**
 * Zpracuje vstupní argumenty programu.
 * ----------------------------------------
 * př: {-i <interface>} --pu(-u) <port-ranges> --pt(-t) <port-ranges> 
 *     { --only-open |--only-closed | --only-unclosed} [<domain-name> | <IP-address>]
 * ----------------------------------------
 * tcp    - ukazatel který bude odkazovat na případně vatvořenou frontu,
 *          pro naplnění porty pro TCP skenování
 * udp    - ukazatel který bude odkazovat na případně vatvořenou frontu,
 *          fronta, pro naplnění porty pro UDP skenování
 * ip_dst - ukazatel na textový řetězec cílové ip pro návrat získaného ip
 * inter  - ukazatel na textový řetězec jména interface pro návrat získaného jména
 * argc   - počet argumetů spuštění programu
 * argv   - argumenty spuštění programu
 * ---------------------------------------
 * V případě úspěchu nahraje funkce do parametrů předaných odkazem
 * příslušné získané hodnoty a vrátí mod výpislu dat ze skenování portů (ALL/OPEN/CLOSED/UNCLOSED).
 * Pokud během parsování narazí na chybu parametrů, ukončí program
 * s návratovým kódem 1.
 */
short int parse_arg(port_queue_t **tcp, port_queue_t **udp, char **ip_dst, char **inter, const int argc, char *argv[]);

/**
 * Z textového řetězce ve formátu naplní frontu číselnými hodnotami,
 * a to i v zadaných rozsazích. např. 1,9-210,987.
 * Ke zpracování je použit automat.
 * ----------------------------------------
 * queue - fronta pro naplnění
 * str   - text obsahující číselné hodnoty ke zpracování
 * ----------------------------------------
 * V případě úspěšného zpracování bude fronta queue naplněna
 * získanýmí hodnotami z řetězce str a vrátí true. Pokud ale
 * vstupní řetězec str nebude ve zprávném formátu (např. A,4 nebo 1,,34)
 * vrátí funkce false a queue obsahovat hodnoty uložené před detekcí.
 */
bool load_range(port_queue_t *queue, const char str[]);

#endif