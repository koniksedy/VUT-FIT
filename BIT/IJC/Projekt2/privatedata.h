/**
 * privatedata.h
 * Řešení: IJC-DU2 b), 13.4.2019
 * Autor: Michal Šedý, FIT
 * Přeloženo: gcc 7.4.0
 * Popis: Privátní hlavičkový soubor.
 */

#ifndef __PRIVATEDATA_H__
#define __PRIVATEDATA_H__

#include "htab.h"

/**
 * Struktura pro prvek tabulky
 */
struct htab_item {
	const char key[MAX_LEN];
	int data;
	struct htab_item *next;
};

/**
 * Struktura pro tabulku
 */
struct htab {
	size_t size;
	size_t arr_size;
	struct htab_item *ptr[];
};

#endif	// !__PRIVATEDATA_H__
