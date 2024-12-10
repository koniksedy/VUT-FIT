/**
 * htab_begin.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 */

#include "htab.h"
#include "privatedata.h"

/**
 * Funkce vrací iterátor označující první záznam.
 */
htab_iterator_t htab_begin(const htab_t * t)
{
	htab_iterator_t it_begin = {.ptr = t->ptr[0], .t = t, .idx = 0};
	
	// fce se snaží najít záznam
	for(size_t i = 0; i < htab_bucket_count(t); i++)
	{
		if(t->ptr[i] != NULL)
		{
			it_begin.ptr = t->ptr[i];
			it_begin.idx = i;
			break;
		}
	}

	return it_begin;
}
