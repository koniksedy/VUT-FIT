/**
 * htab_iterator_get_value.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#include "privatedata.h"

/**
 * Fce vrátí hodnotu prvku (pokud existuje) , na který ukazuje iterátor.
 */
int htab_iterator_get_value(htab_iterator_t it)
{
	if(htab_iterator_valid(it))
	{
		return it.ptr->data;
	}
	else
	{	
		fprintf(stderr, "CHYBA htab_iterator_get_value: neplatný iterátor.\n");
		htab_free((htab_t*)it.t);
		exit(1);
	}
}
