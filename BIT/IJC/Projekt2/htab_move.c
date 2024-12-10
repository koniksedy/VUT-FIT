/**
 * htab_move.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 * Poznámka: Funkce z tohoto moudlu se použije pouze s podmíněným překladem TEST.
 */

#include <stdlib.h>
#include "htab.h"
#include "privatedata.h"

/**
 * Vytvoří a inicializuje novou tabulku přesunem dat z tabulky from.
 * from nakonec zůstane prázdná a alokovaná.
 */
htab_t *htab_move(size_t n, htab_t *from)
{
	htab_iterator_t it_tmp = htab_begin(from);		// začátek from
	htab_iterator_t it_end = htab_end(from);		// zarážka from
	struct htab_item *tmp;							// pomocná proměná pro iteraci v seznamu
	size_t index;									// index do tabulky tab_to

	// Alokace tabulky a kontrola
	htab_t *tab_to = htab_init(n);
	if(tab_to == NULL)
	{
		return NULL;
	}

	// Kopírování z from
	while(!htab_iterator_equal(it_tmp, it_end))
	{
		index = htab_hash_function(htab_iterator_get_key(it_tmp))%n;

		// Celý seznam je volný, přidat jako první
		if(tab_to->ptr[index] == NULL)
		{
			tab_to->ptr[index] = malloc(sizeof(struct htab_item));
			if(tab_to->ptr[index] == NULL)
			{
				htab_free(tab_to);
				return NULL;
			}

			tab_to->ptr[index]->data = htab_iterator_get_value(it_tmp);
			memcpy((void*)tab_to->ptr[index]->key, (void*)htab_iterator_get_key(it_tmp), MAX_LEN);
			tab_to->ptr[index]->next = NULL;
		}
		
		// Seznam není volný, je třeba se dostat na konec
		for(tmp = tab_to->ptr[index]; tmp->next != NULL; tmp = tmp->next)
			;
		tmp->next = malloc(sizeof(struct htab_item));
		if(tmp->next == NULL)
		{
			htab_free(tab_to);
			return NULL;
		}
		
		tmp->next->data = htab_iterator_get_value(it_tmp);
		memcpy((void*)tmp->next->key, (void*)htab_iterator_get_key(it_tmp), MAX_LEN);
		tmp->next->next = NULL;

		it_tmp = htab_iterator_next(it_tmp);
	}

	htab_clear(from);
	return tab_to;
}
