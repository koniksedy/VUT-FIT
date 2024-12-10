/**
 * htab_end.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 */

#include "htab.h"
#include "privatedata.h"

/**
 * Funkce vrací neexistující iterátor, za posledním prvkem tabulky.
 */

htab_iterator_t htab_end(const htab_t * t)
{
	htab_iterator_t it_end = {.ptr = NULL, .t = t, .idx = htab_bucket_count(t)};
	return it_end;
}
