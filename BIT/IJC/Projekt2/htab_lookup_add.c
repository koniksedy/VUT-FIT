/**
 * htab_lookup_add.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 */

#include <stdlib.h>
#include "htab.h"
#include "privatedata.h"

/**
 * Funkce v tabulce t vyhledá záznam odpovídající řetězci key a
 * - pokud jej nalezne, vrátí iterátor na záznem
 * - pokud jej nenalezne, automaticky přidá záznam a vrátí iterátor.
 */
htab_iterator_t htab_lookup_add(htab_t *t, const char *key)
{
	htab_iterator_t it = {.ptr = NULL, .t = t, .idx = (htab_hash_function(key) % htab_bucket_count(t))};
	struct htab_item *tmp;	// pomocná proměnna pro iterace

	// Procházení seznamu na daném indexu a vyhledávání
	for(tmp = t->ptr[it.idx]; tmp != NULL; tmp = tmp->next)
	{
		if(strcmp(tmp->key, key) == 0)
		{
			it.ptr = tmp;
			return it;
		}
	}

	// NENALEZENO
	struct htab_item *new = malloc(sizeof(struct htab_item));
	if(new == NULL)
	{
		it.ptr = NULL;
		return it;
	}

	memcpy((void*)new->key, (void*)key, MAX_LEN);	
	new->data = 0;
	new->next = NULL;

	// Seznam je zatím prázdný - vložit jako první
	if(t->ptr[it.idx] == NULL)
	{	
		it.ptr = new;
		t->ptr[it.idx] = new;
	}
	// Je třeba projít seznamem a přidat až na konec
	else
	{
		for(tmp = t->ptr[it.idx]; tmp->next != NULL; tmp = tmp->next)
			;
		it.ptr = new;
		tmp->next = new;
	}

	return it;
}
