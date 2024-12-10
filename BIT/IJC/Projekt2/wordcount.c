/**
 * wordcount.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 * Popis: Program pořítá četnost slov ve vstupním textu,
 *		  slovo je cokoli oddělené "bílým znakem".
 */

#include <stdio.h>
#include <stdbool.h>
#include "htab.h"
#include "io.h"

/******************************ZVOLENÍ VELIKOSTI HASHOVACÍ TABULKY*******************************
 *																								
 * Pro velikost tabulky byla zvolenan velikost 6521 z následujících důvodů:						
 * Průměrná aktivní slovní zásoba člověka se pohybuje v rozmezí 3000-10000 slov,				
 * proto byla zvolena hrubá hodnota na 5000. Tabulky by ale měla obsahovat						
 * 1.3-krát více položek, než je očekávaných prvků. Dále počet prvků v tabulce měl být prvočíslo,
 * tedy 6521. Výsledná hodnota byla stanovenan itaké na základě testů:
 *
 * HODNOTA 30011 , počet slov 4287200
 * ----------------------------------
 *	- time:	1.738s
 *	- mem: 3,848,932 bytes
 *
 * HODNOTA 30011 , počet slov 0
 * ----------------------------
 *	- mem:   121,076 bytes
 *
 * HODNOTA 6521, počet slov 4284200
 * --------------------------------
 *	- time: 1.837s
 *	- mem: 3,754,972
 *
 * HODNOTA 6521, počet slov 0
 * --------------------------
 *	-mem: 27,116 bytes
 *
 *	
 * Vididíme, že potřebný čas, ani paměť při velkém počtu slov se téměř nemění.
 * Pokud však bude tabulka nevyužita, pak dochází k poměrně značnému rozdílu ztrát.
 * Hodnota 6521 je ideální pro zpracovávání textů(knih, článku, ...), nikoiv slovníků,
 * a nebo uměle vytvořených případů, poté by byl zapotřebí větší rozměr.
 ************************************************************************************************/

#define TAB_SIZE 6521


// Moje hashovací funkce se použije pri překladu s HASHTEST
// Funkce je asi 40-krát pomalejší než funkce doporučovaná literaturou
#ifdef HASHTEST
#include <stdint.h>
unsigned int htab_hash_function(const char *str)
{
	uint32_t h=0;
	for(const unsigned char *p = (const unsigned char*)str; *p != '\0'; p++)
	{
		h *= *p;
	}
	return h;
}
#endif

int main()
{
	bool fail_len = false;			// pro detekci chyby - dlouhé slovo
	htab_t *table;					// vytvoření tabulky
	int len = 0;					// délky slova
	char string[MAX_LEN];			// pro dočasné ukládání slov
	htab_iterator_t iter;
	htab_iterator_t end;			// zarážka
	
	table = htab_init(TAB_SIZE);
	if(table == NULL)
	{
		fprintf(stderr, "CHYBA: Nepovedla se alokace paměti pro tabulku.\n");
		return -1;
	}

	iter = htab_begin(table);	
	
	while((len = get_word(string, MAX_LEN, stdin)) != EOF)
	{
		// Chyba parametrů ve funkci
		if(len == -2)
		{
			htab_free(table);
			return -1;
		}

		// Překročena maximální délka slova
		if(len == MAX_LEN && !fail_len)
		{
			fail_len = true;
			fprintf(stderr, "CHYBA: slovo je delší než %d znaků, podračuji dále se zkrácenými.\n", MAX_LEN-1);
		}

		iter = htab_lookup_add(table, string);
		if(iter.ptr == NULL)
		{
			fprintf(stderr, "CHYBA: Nepovedla se alokace paměti pro nový prvek.\n");
			htab_free(table);
			return -1;
		}
		// zvýšíme počítadlo slova
		htab_iterator_set_value(iter, htab_iterator_get_value(iter)+1);
	}

	// výpis
	iter = htab_begin(table);
	end = htab_end(table);
	while(!htab_iterator_equal(iter, end))
	{
		printf("%s\t%d\n", htab_iterator_get_key(iter), htab_iterator_get_value(iter));
		iter = htab_iterator_next(iter);
	}

	// Funkce htab_move se zavolá pouze při podmíněném překladu TEST
	#ifdef TEST
	table = htab_move(30011, table);
	if(table == NULL)
	{
		fprintf(stderr, "CHYBA: Nepodařil se přesun tabulky.\n");
		return -1;
	}
	#endif
	htab_free(table);
	return 0;
}
