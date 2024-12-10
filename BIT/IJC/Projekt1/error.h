/**
 * error.h
 * Řešení IJC-DU1, příklad a), b), 13.3.2019
 * Autor: Michal Šedý, FIT
 * Přeloženo: gcc 5.4.0
 * Popis: Hlavičkový soubor deklarací funkce pro výpis chyby
 * 		  a funkce pro výpis chyby a ukončení programu.
 */

#ifndef ERROR_H
#define ERROR_H

/* FCE vypíše chybu v závislosti na fmt */
void warning_msg(const char *fmt, ...);

/* FCE vypíše chybu v závislosti na fmt a ukonší program */
void error_exit(const char *tmt, ...);

#endif
