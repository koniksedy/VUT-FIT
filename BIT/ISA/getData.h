/*
 * Hlavičkový soubor modulu getData.c.
 * --------------------------
 * Soubor: getData.h
 * Projekt: ISA - rozšíření SNMP agenta.
 * Autor: Michal Šedý <xsedym02>
 * Datum úpravy: 30.10.2020
 */

#ifndef _GETDATA_H_
#define _GETDATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


/*
 * Funkce vrací booleovskou hodnotu, zda obsahuje řetězec daný prefix.
 * -------------------------------------------------------------------
 * string - testovaný textový řetězec
 * prefix
 */
bool starts_with(const char *string, const char *prefix);

/*
 * Z daného souboru získá první řádek začínající prefixem.
 * Získaný řádek nahraje do dst.
 * ------------------------------------------------------
 * dst - ukazatel na proměnnou, kam se nahraje získaný řádek
 * prefix - hledaný prefix
 * fiel_name - prohledávaný soubor
 */
void get_line_starts_with(char *dst, const char *prefix, const char *file_name);

/*
 * Získá hodnotu z textu ve formátu: "VAR_NAME  :  12345 KB"
 * Vrací všechny hodnoty za dvojtečkou: "12345 KB".
 * ---------------------------------------------------------
 * line - parsovaný řádek
 * dst - ukazatel kam má být zkopírována získaný hodnota.
 */
void get_value_from_line(char line[], char *dst);

/*
 * Funkce pro získání hodnoty proměnné nacházející se v daném souboru.
 * -------------------------------------------------------------------
 * fiel_name - jméno prohledávaného souboru
 * var_name  - jméno proměnné
 * dst - ukazatel kam se má výsledek uložit
 * is_int - informace o typu očekávaného výsledku.
 *        - TRUE  -> int
 *        - FALSE -> char[]
 */
void get_data(const char *file_name, const char *var_name, void *dst, bool is_int);


#endif	// _GETDATA_H_
