/**
 * steg-decode.c
 * Řešení IJC-DU1, příklad b), 16.3.2019
 * Autor: Michal Šedý, FIT
 * Přeloženo: gcc 5.4.0
 * Popis: Zdrolový soubor programu pro dekódování tajně zprávy uložené v ppm souboru.
 *		  Zpráva je uložený po jednotlivých bitech na nejnižších bitech vybraných
 *		  bajtů barebných složek v datech obrázku. Dekódování se ukončí po dosažení '\0'.
 *		  Vypraní bajty jsou určeny prvočísly počínaje číslen 19. Program kontroluje
 *		  maximální možnou velikost dat pro dekódování , která je nastavena na 192000000.
 *		  limits.h je includovan v bit_array.h.
 */

#include <stdio.h>
#include "bit_array.h"
#include "eratosthenes.h"
#include "error.h"
#include "ppm.h"

#define M 200000000

int main(int argc, char *argv[])
{
	struct ppm *p = NULL;

	// Kontrola argumentu
	if(argc != 2)
	{
		error_exit("Jako argument zadejte jméno souboru pro dekodování.\n");
	}
	
	// Načtení souboru do ppm sturktury
	p = ppm_read(argv[1]);
	if(p == NULL)
	{
		error_exit("Během načítání do ppm struktury došlo k chybě.\n");
	}
	
	// Kontrola limitní velikosti dat pro dekódování
	if(M < 3*(p->xsize)*(p->ysize))
	{
		ppm_free(p);
		error_exit("Velikost dat pro dekódování přesahuje nastavený limit %lu.\n", M);
	}

	// Vyhledání potřebných prvočísel
	bit_array_alloc(prim, 3*(p->xsize)*(p->ysize));
	Eratosthenes(prim);

	int chr = 0;
	short n = 0;
	for(unsigned long i = 19; i < 3*(p->xsize)*(p->ysize); i++)
	{
		if(!(bit_array_getbit(prim, i)))
		{
			chr |= (p->data[i] & 1) << n++;
			if(n == CHAR_BIT)
			{
				if(chr == '\0')
				{	
					putchar('\n');
					chr = EOF;
					break;
				}
				n = 0;
				putchar(chr);
				chr = 0;
			}
		}
	}
	
	if(chr != EOF)
	{
		bit_array_free(prim);
		ppm_free(p);
		error_exit("Zpráva není korektně ukončena.\n");
	}
	
	bit_array_free(prim);
	ppm_free(p);
	return 0;
}
