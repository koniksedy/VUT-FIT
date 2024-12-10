/**
 * htab_hash_function.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 * Popis: Modul s hashovací funkcí.
 */

#include <stdint.h>

/**
 * Rozptylovací funkce, její výsledek modulo arr_size určuje index do tabulky
 */
unsigned int htab_hash_function(const char *str)
{
	uint32_t h = 0;			// musí mít 32 bitů
	const unsigned char *p;
	for(p = (const unsigned char*)str; *p != '\0'; p++)
		h = 65599*h + *p;
	
	return h;
}
