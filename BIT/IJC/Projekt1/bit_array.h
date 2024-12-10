/**
 * bit_array.h
 * Řešení IJC-DU1, příklad a), b), 13.3.2019
 * Autor: Michal Šedý
 * Přeloženo: gcc 5.4.0
 * Popis: Hlavičkový soubor, který definuje strukturu pro pole bitu bit_array_t.
 *		  Pro pole bitu je nastaven LIMIT. Obsahuje inline funkce, které se zařadí
 *		  do programu definicí USE_INLIN při překladu, v opačném případě jsou pužita
 *		  alternativní makra. Funkce a makra kontrolují rozměry polí. Při chybě se volá
 *		  funkce error_exit, která vypíše chybovou hlášku a ukončí program.
 */

#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "error.h"

#define LIMIT 300000000L

typedef unsigned long bit_array_t[];

#define szoful (sizeof(unsigned long)*CHAR_BIT)

/* Definuje a nuluje proměnnou jmeno_pole, zaroven kontroluje maximalni velikost poel. */
#define bit_array_create(jmeno_pole, velikost) static_assert((velikost) <= LIMIT, "bit_array_alloc: Chyba překročení LIMITU pole.\n");\
		unsigned long jmeno_pole[(((velikost)-1L)/szoful)+2L] = {velikost, 0L,};

/* Definuje proměnnou jemno_pole kompatibilní s polem vytvořeným pomocí bit_array_create,
toto pole je ale alokováno dynamicky. Pokud alokace selže, vypíše chybovou hlášku a ukončí program. */
#define bit_array_alloc(jmeno_pole, velikost) assert((velikost) <= LIMIT);\
		unsigned long *jmeno_pole = calloc((((velikost)-1L)/szoful)+2L, sizeof(unsigned long));\
		if(jmeno_pole == NULL) error_exit("bit_array_alloc: Chyba alokace paměti.\n");\
		jmeno_pole[0] = velikost;


#ifndef USE_INLINE


/* Uvolní paměť dynamicky alokovaného pole */
#define bit_array_free(jmeno_pole) free(jmeno_pole)

/* Vrátí deklarovanou velikost pole v bitech */
#define bit_array_size(jmeno_pole) (jmeno_pole[0])

/* Nastaví zadaný bit v poli na hodnotu zadanou výrazem */
#define bit_array_setbit(jmeno_pole, index, vyraz) if((index) > bit_array_size(jmeno_pole)) error_exit("bit_array_setbit: Index %lu mimo rozsah 0..%lu.\n", (unsigned long)(index), (unsigned long)bit_array_size(jmeno_pole));\
		(vyraz) ? (jmeno_pole[1L+(index/szoful)] |= (1L << (index%szoful))) : (jmeno_pole[1L+(index/szoful)] &= (~(1L << (index%szoful)))) 

/* Získá hodnotu zadaného bitu, vrací hodnotu 0 nebo 1 */
#define bit_array_getbit(jmeno_pole, index) ((index) > bit_array_size(jmeno_pole)) ? (error_exit("bit_array_getbit: Index %lu mimo rozsah 0..%lu.\n", (unsigned long)(index), (unsigned long)bit_array_size(jmeno_pole)), 0) \
		: ((int)((jmeno_pole[1L+(index/szoful)] >> (index%szoful)) & 1L))


#else	// USE_INLINE


/* FCE uvolní paněť dynamicky alokovaného pole */
static inline void bit_array_free(bit_array_t jmeno_pole)
{
	free(jmeno_pole);
}

/* FCE vrátí deklarovanou velikost pole v bitech. */
static inline unsigned long bit_array_size(bit_array_t jmeno_pole)
{
	return (jmeno_pole[0L]);
}

/* FCE nastaví zadaný bit v poli na hodnotu zadanou výrazem. */
static inline void bit_array_setbit(bit_array_t jmeno_pole, unsigned long index, int vyraz)
{
	//Kontrola mazí
	if(index > bit_array_size(jmeno_pole))
	{
		error_exit("bit_array_setbit: Index %lu mimo rozsah 0..%lu", (unsigned long)index, (unsigned long)bit_array_size(jmeno_pole));
	}

	//Nastavení bitu na 0
	if(!vyraz)
	{
		jmeno_pole[1L+(index/szoful)] &= (~(1L << (index%szoful)));
	}
	//nastavení bitu na 1
	else if(vyraz)
	{
		jmeno_pole[1L+(index/szoful)] |= (1L << (index%szoful));
	}
}

/* FCE získá hodnotu zadanéo bitu, vrací hodnotu 0 nabo 1. */
static inline int bit_array_getbit(bit_array_t jmeno_pole, unsigned long index)
{
	// Kontrola mezí
	if(index > bit_array_size(jmeno_pole))
	{
		error_exit("bit_array_getbit: Index %lu mino rozsah 0..%lu", (unsigned long)index, (unsigned long)bit_array_size(jmeno_pole));
	}

	return (int)((jmeno_pole[1L+(index/szoful)] >> (index%szoful)) & 1L);
}

#endif // !USE_INLINE
#endif // !BIT_ARRAY_H
