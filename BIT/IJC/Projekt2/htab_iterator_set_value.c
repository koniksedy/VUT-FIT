/**
 * htab_iterator_set_value.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#include "privatedata.h"

/**
 * Fce přepisuje hodnotu prvku (pokud existuje) , na který ukazuje iterátor.
 */
int htab_iterator_set_value(htab_iterator_t it, int val)
{
	if(htab_iterator_valid(it))
	{
		it.ptr->data = val;
	}
	else
	{	
		fprintf(stderr, "CHYBA htab_iterator_set_value: neplatný iterátor.\n");
		htab_free((htab_t*)it.t);
		exit(1);
	}

	return 0;
}
