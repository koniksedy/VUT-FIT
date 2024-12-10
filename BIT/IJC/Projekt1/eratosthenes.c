/**
 * eratosthenes.c
 * Řešení IJC-DU1, příklad a), b), 13.3.2019
 * Autor: Michal Šedý, FIT
 * Přeloženo: gcc 5.4.0
 * Popis: Modul pro vyhledávání prvočísel s využitím eratosthenova síta.
 */

#include <math.h>
#include "eratosthenes.h"
#include "bit_array.h"

void Eratosthenes(bit_array_t pole)
{
	// Vyškrtuní 0 a 1 ze seznamu kandidátu na prvočísla
	bit_array_setbit(pole, 0, 1);
	bit_array_setbit(pole, 1, 1);

	for(unsigned long i = 2; i <= sqrt(bit_array_size(pole)); i++)
	{
		if(bit_array_getbit(pole, i) == 0)
		{	
			// Nastavení bitu na 1 pro vyšechny násobky čísla i
			for(unsigned long n = (i*2); n < bit_array_size(pole); n += i)
			{
				bit_array_setbit(pole, n, 1);
			}
		}
	}
}
