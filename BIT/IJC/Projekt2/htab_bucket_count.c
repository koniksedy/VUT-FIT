/**
 * htab_bucket_count.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 */

#include "htab.h"
#include "privatedata.h"

/**
 * Funkce vrátí počet prvků pole.
 */
size_t htab_bucket_count(const htab_t * t)
{
	return t->arr_size;
}
