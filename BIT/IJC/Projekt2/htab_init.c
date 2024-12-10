/** 
 * htab_init.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 */

#include <stdlib.h>
#include "htab.h"
#include "privatedata.h"

/**
 * Funkce vytvoří a inicializuje tabulku.
 */
htab_t *htab_init(size_t n)
{
	if(n <= 0)
	{
		return NULL;
	}

	htab_t *t = malloc(sizeof(struct htab) + sizeof(struct htab_item*)*n);
	if(t == NULL)
	{
		return NULL;
	}

	// inicializace alokované tabulky
	for(size_t i = 0; i < n; i++)
	{
		t->ptr[i] = NULL;
	}

	t->size = n+2;
	t->arr_size = n;
	return t;
}
