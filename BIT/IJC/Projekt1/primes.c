/**
 * primes.c 
 * Řešení IJC-DU1, příklad a), b), 13.3.2019
 * Autor: Michal Šedý, FIT
 * Přeloženo: gcc 5.4.0
 * Popis: Zdrojový text programu pro vyhledání posledních 10-ti prvočísel
 *		  v rozmezí 0..123000000 (123 miniónů). Postedních 10 prvočísel
 *        byde vypsáno na samostatný řádek vzestupně.
 */

#include <stdio.h>
#include "eratosthenes.h"
#include "bit_array.h"

#define N 123000000

int main()
{
	unsigned long prim[10];

	// Vytvoření bitového pole
	bit_array_create(array, (N+1));

	// Aplikace eratosthenova síta
	Eratosthenes(array);

	// Hledání posledních 10-ti prvočísel
	short p = 0;
	for(unsigned long i = N; i > 1; i--)
	{
		if(bit_array_getbit(array, i) == 0)
		{
			prim[9-p] = i;
			p++;
			if(p == 10)
				break;
		}
	}
	//Výpis prvočísel
	for(int j = 0; j < 10; j++)
	{
		printf("%lu\n", prim[j]);
	}

	return 0;
}

