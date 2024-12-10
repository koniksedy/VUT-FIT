/**
 * htab_iterator_get_key.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#include "privatedata.h"

/**
 * Fce vrátí klíč prvkui (pokud existuje) , na který ukazuje iterátor.
 */
const char * htab_iterator_get_key(htab_iterator_t it)
{
	if(htab_iterator_valid(it)) 
	{
		return it.ptr->key;
	}
	else
	{	
		fprintf(stderr, "CHYBA htab_iterator_get_key: neplatný iterátor.\n");
		htab_free((htab_t*)it.t);
		exit(1);
	}
}
