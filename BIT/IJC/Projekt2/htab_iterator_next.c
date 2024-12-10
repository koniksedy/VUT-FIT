/**
 * htab_iterator_next.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 */

#include "htab.h"
#include "privatedata.h"

/**
 * Funkce posune iterátor na další záznam v tabulce, nebo htab_ned.
 */
htab_iterator_t htab_iterator_next(htab_iterator_t it)
{
	// snaha pokročit dále v seznamu o stejném indexu
	if(it.ptr->next != NULL)
	{
		it.ptr = it.ptr->next;
		return it;
	}
	else
	{	// procházení tabulky a vyhledávání dalšího prvku
		for(size_t i = (it.idx)+1; i < htab_bucket_count(it.t); i++)
		{
			if(it.t->ptr[i] != NULL)
			{
				it.ptr = it.t->ptr[i];
				it.idx = i;
				return it;
			}
		}
	}
	
	// pokud iterátor dojde až na konec vrací htab_end
	return htab_end(it.t);
}
