/**
 * eratosthenes.h 
 * Řešení IJC-DU1, příklad a), b), 13.3.2019
 * Autor: Michal Šedý, FIT
 * Přeloženo: gcc 5.4.0
 * Popis: Hlavičkový soubor, obsahující deklaraci funkce pro vyhledávání prvočísel
 *		  s využitím eratosthenova síta.
 */

#ifndef ERATOSTHENES_H
#define ERATOSTHENES_H

#include "bit_array.h"

/* FCE pro vyhlédávání prvočísel s využitím ERATOSTHENOVA SÍTA */
void Eratosthenes(bit_array_t pole);

#endif //ERATOSTHENES_H
