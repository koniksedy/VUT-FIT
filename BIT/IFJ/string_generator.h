/**
 * Hlavičkový soubor modulu generujícího řetězce
 * Napsal: Michal Šedý <xsedym02>
 * Přeloženo: gcc 7.4.0
 * Standard: C99
 * Poslední úprava: 05.12.19
 */

#ifndef __STRING_GENERATOR_H__
#define __STRING_GENERATOR_H__

#include <stdio.h>
#include "string_dynamic.h"

// Maximální řád počtu vygenerovaných řetězců
#define MAX_ORDER 7

/**
 * Funkce pro vytvoření jména pomocné proměnné
 * -------------------------------------------
 * - Funkce vytvoří pokaždé různé jméno s konstantním počátečním znakem $
 */
string_t *string_rand(void);

/**
 * Funkce pro vytvoření jména návěští
 * -------------------------------------------
 * - Funkce vytvoří pokaždé různé jméno s konstantním počátečním znakem %
 */
string_t *generate_label(void);

/**
 * Funkce pro přepis textového řetězce na string_t
 * -----------------------------------------------
 * str - řetězec pro přepis
 *
 * Funkce vzací ukazatel na takto vzniklý řetězec, pokud se vytváření
 * nepovede, vrátí NULL.
 */
string_t *string_make(const char *str);

#endif