/**
 * Hlavní zdrojový soubor překladače jazyka IFJ19
 * Autor: Michal Šedý <xsedym02>
 * Přeloženo: gcc 7.4.0
 * Standard: C99
 * Poslední změna: 05.12.2019
 */
#define INFO ;// fprintf(stderr, "---------------------------------\nmodul-----> %s\nfunkce ------> %s\nline-----> %d\n---------------------------------\n", __FILE__, __func__, __LINE__);
#include <stdio.h>
#include "stack.h"
#include "symtable.h"
#include "error.h"
#include "parse.h"
#include "generator.h"

// Makro pro uvolňování zdrojú po vzniku chyby při postupné alokaci
#define DISPOSE(i) switch(i){\
						case(2): htab_dtor(global_table);\
						case(1): stack_dtor(&GLOBALNI_STACK);\
					};

stack_t *GLOBALNI_STACK = NULL;

int main(int argc, char **argv)
{
	
	htab_t *global_table = NULL;
	instr_tape_t *main_tape = NULL;

	// Kontrola zda nebyly zadány parametry při spuštění
	if(argc != 1)
	{
		error_msg("CHYBA: program má být spouštěn bez argunemtů, vstup je jen stdin\n");
		return ERR_INTER_RUN;
	}

	// Alokace zásobníku pro odsazení
	GLOBALNI_STACK = stack_ctor();
	if(GLOBALNI_STACK == NULL)
	{
		error_msg("CHYBA: Chyba malloku GLOBALNI_STACK\n");
		return ERR_INTER_RUN;
	}

	// Alokace globální tabulky
	global_table = htab_ctor();
	if(global_table == NULL)
	{
		error_msg("CHYBA: Chyba malloku global_table\n");
		DISPOSE(1);
		return ERR_INTER_RUN;
	}

	// Alokace instrukční pásky
	main_tape = tape_ctor();
	if(main_tape == NULL)
	{
		error_msg("CHYBA: Chyba malloku main_tape\n");
		DISPOSE(2);
		return ERR_INTER_RUN;
	}
	//********************************************//

	// Parsování a překlad vstupního programu do tříadresného kódu
	int ret_val = parse(global_table, &main_tape);

	// Kontrola, zda můžeme překládat
	if(ret_val != ERR_OK)
	{
		error_msg("Došlo k chybě během syntaktické analýzy.\n");
		return ret_val;
	}
	
	// Provádím překlad do ifjcode19
	ret_val = compile(main_tape);
	

	// Kontrola, zda proběhl překlad bez chyb
	if(ret_val != ERR_OK)
	{
		error_msg("Došlo k chybě během generovéní kódu.\n");
	}

	return ret_val;
}
