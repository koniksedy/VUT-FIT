/**
 * htabsize.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 */

#include "htab.h"
#include "privatedata.h"

/**
 * Fukce vrátí počet všech prvků tabulky (data+info o tabulce).
 */
size_t htab_size(const htab_t * t)
{
	return t->size;
}
