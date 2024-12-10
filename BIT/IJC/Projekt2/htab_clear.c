/**
 * htab_clear.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 */

#include <stdlib.h>
#include <stdbool.h>
#include "htab.h"
#include "privatedata.h"

/**
 * Funkce zruší všechny prvky tabulky.
 */
void htab_clear(htab_t * t)
{
	htab_iterator_t it_dtor = htab_begin(t);		// Nastavení iterátoru na první prvek
	htab_iterator_t it_end = htab_end(t);			// Nestavení zarážky
	struct htab_item *tmp = NULL;

	// Kontrola, zda-li je co rušit 
	if(it_dtor.ptr == NULL)
	{
		return;
	}
	// rušení prvků
	while(!(htab_iterator_equal(it_dtor, it_end)))
	{
		tmp = it_dtor.ptr;
		it_dtor = htab_iterator_next(it_dtor);
		free(tmp);
		tmp = NULL;
	}
}
