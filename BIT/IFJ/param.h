/**
 * Hlavičkový soubor modulu param.c
 * Napsal: Michal Šedý <xsedym02>
 * Přeloženo: gcc 7.4.0
 * Standard: C99
 * Poslední změna: 05.12.19
 */

#ifndef __PARAM_H__
#define __PARAM_H__

#include <stdlib.h>
#include "error.h"
#include "scanner.h"
#include "symtable.h"

/**
 * Funkce alokuje místo v tabulke pro parametry.
 * ---------------------------------------------
 * table - tabulka, ve které vytvoříme parametry
 *
 * V případě úspěchu vrací true, jinak false.
 */
bool params_ctor(htab_t *table);

/**
 * Funkce pro přidání parametru do tabulky na konec jejich seznamu
 * ---------------------------------------------------------------
 * table - tabulka pro vložení parametru
 * token - token obsahující jmého parametru
 *
 * V případě úspěchu vrací funkce 0, jinak patřičný chybový kód
 */
int params_append(htab_t *table, token_t *token);

/**
 * Funkce vrací ukazatel na parametry 
 * ----------------------------------
 * table - tabulky, ze které jsou parametry čteny
 * 
 * Funkce vrací ukazatel na tento seznam parametrů.
 */
param_t *params_get(htab_t *table);

/**
 * Funkce dealokuje parametry
 * --------------------------
 * table - tabulka, ze které budou odstraněny parametry
 */
void params_dtor(htab_t *table);

#endif