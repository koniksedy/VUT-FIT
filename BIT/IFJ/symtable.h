/*Hlavičkový soubor k modulu: symtable
  Vypracoval: Ondřej Pavlacký (xpavla15) 
  Přeloženo gcc 7.4.0, standard C99
  Poslední úprava 11.12.2019, 2.BIT 2019/20  */

#ifndef	__SYMTABLE_H__
#define __SYMTABLE_H__

#include "instr_tape.h"
#include "string_dynamic.h"
#include "scanner.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#define HASH_SIZE 313  //Prvočíslo pro lepší vyhledávání a rovnoměrné rozložení

typedef union {
	char ch;   
	int i;
	double fl;
	string_t *str;
	struct fce_tt *fce;
}data_item_t; //Slouží k uložení správného typu dat

typedef struct htab_item_tt{
	string_t *k;  //Ukazatel na dynamický řetězec
	char type; //Určení o který typ jde
	bool defined; //Jestli je proměnná definovaná
	data_item_t *it_data; //Uložená data
	struct htab_item_tt *next; //Ukazatel na další položku
}htab_item_t;

typedef struct param_tt{
	htab_item_t *ptr; //Ukazatel na identifikátor
	struct param_tt *next; //Ukazatel na další
}param_t;

typedef struct { 
	unsigned short params_cnt; //Počet parametrů
	 param_t *params; //Parametry funkce
	 htab_item_t *ptr[]; //Pole ukazatelů na položky
}htab_t;

typedef struct fce_tt{
	htab_t *h_table; //Vlastní tabulka symbolů pro funkci
}fce_t;

unsigned int htab_hash_function(char *str); //Funkce pro výpočet adresy 

htab_t *htab_ctor(void); //Funkce pro vytvoření tabulky symbolů

bool htab_insert(htab_t *table, string_t *key, token_t *token); //Vložení prvku do tabulky 

void type_defined(htab_t *table,string_t *key); //Nastavuje defined na true

bool htable_make_func(htab_t *table, string_t *key); //Vytvoření nové tabulky pro funkci na určitém místě v první tabulce

htab_item_t *htab_get_item(htab_t *table, string_t *key); //Vrací ukazatel na prvek v tabulce

void htab_dtor(htab_t *table); //Dealokuje tabulku

#endif //__SYMTABLE_H__