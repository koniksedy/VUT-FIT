/**
 * htab_free.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 * Popis: volá funkci htab_clear.
 */

#include <stdlib.h>
#include "htab.h"
#include "privatedata.h"

/**
 * Funkce zruší tabulku. Volá funkci htab_clear.
 */
void htab_free(htab_t * t)
{
	htab_clear(t);
	free(t);
}
