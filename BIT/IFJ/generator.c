/**
 * Modul generující cílový kód IFJcode19
 * Mapsal: Ondřej Pavlacký <xpavla15> a Michal Šedý <xsedym02>
 * Přeloženo: gcc 7.4.0
 * Standard: C99
 * Poslední úprava 9.12.19
 */
#include "generator.h"

#define TOK ;//fprintf(stderr, "##################################\nradek-->%d\n##################################\n", __LINE__);

// Prototyp funkce
int make_statements(instr_t *instrukce, instr_tape_t *main_tape, string_t *parrent);
int make_statements_f(instr_t *instrukce, instr_tape_t *main_tape, string_t *parrent);


/**
 * Funkce pro převod stringu na hexadecimální formu
 * ------------------------------------------------
 * Funkce převání spesiální bílé znaky do formátu escape sekvence.
 * 
 * old - starý string, který je potřeba přepsat
 *
 * Funkce vrací nový řetězec s přepsanými speciálními znaky.
 */
string_t *string_to_hex(string_t* old)
{
	char *str = string_get(old);
	char hex[5] =  {0,};
	char ch = 0;

	// Alokace nového řetězce
	string_t* new = string_ctor();
	if(new == NULL)
	{
		return NULL;
	}


	// Přepisování znak po znaku
	int len = strlen(str);
	for(int i = 0; i < len; i++)
	{
		ch = str[i];
		// Spesiální přepis pokud se jedná o bilý znak
		if(isspace(ch))
		{
			sprintf(hex, "\\0%02d", ch);
			for(int j = 0; j < 4; j++)
			{
				string_put_char(&new, hex[j]);
				if(new == NULL)
				{
					return NULL;
				}
			}
		}
		else
		{
			string_put_char(&new, ch);
			if(new == NULL)
			{
				return NULL;
			}
		}
	}

	return new;
}

/**
 * Funkce pro generování definice proměnných
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// DEF_COM | *name | ---- | ----- |
//				^
//				|
//				|--- ukazatel do tabulky na jméno proměnné, která se bude definovat.
int gen_def_var(instr_t* instrukce, string_t *parrent){
	htab_item_t* item1 = NULL;
	item1 =instrukce->addr1;
	
	if(instrukce->addr1 == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n",string_get(item1->k));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n",string_get(item1->k));
	}

	return ERR_OK;
}

/**
 * Funkce pro kopírování hodnot
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// CPY_COM | dst | src | --- |
//			  ^     ^
//			  |     |
//			  |     | -- ukazatel na R hodnotu
//			  | -- ukazatel do tabulky na L hodnotu
int gen_cpy_com(instr_t* instrukce){
	htab_item_t* item1 = NULL;
 	htab_item_t* item2 = NULL;
	item1 = (htab_item_t*)(instrukce->addr1);
	item2 = (htab_item_t*)(instrukce->addr2);
	string_t *strin = NULL;

	// Pokud je src NULL, pak dojde k nahrání hodnoty nill
	if(instrukce->addr1 != NULL && instrukce->addr2 == NULL)
	{
		printf("MOVE GF@%s nil@nil\n", string_get(item1->k));
	}
	else if(instrukce->addr1 != NULL && instrukce->addr2 != NULL)
	{
		// Rozhodování o různých způsobech nahrávání
		if(item2->type == INT)				printf("MOVE GF@%s int@%d\n",string_get(item1->k),item2->it_data->i);
		else if(item2->type == FLOAT)		printf("MOVE GF@%s float@%a\n",string_get(item1->k),item2->it_data->fl);
		else if(item2->type == STRING || item2->type == COMMENT)
		{
			strin = string_to_hex(item2->it_data->str);
			if(strin == NULL)
			{
				return ERR_INTER_RUN;
			}

			printf("MOVE GF@%s string@%s\n",string_get(item1->k),string_get(strin));
		}
		else if(item2->type == IDENTIFIER)	printf("MOVE GF@%s GF@%s\n",string_get(item1->k),string_get(item2->k));
	}
	else
	{
		return ERR_INTER_RUN;
	}

	return ERR_OK;
}

/**
 * Funkce pro tisk
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// PRINT_CALL | PARAM | --- | --- |
//				 ^
//				 |
//				 | -- ukazatel do tabulky na první parametr k vytisknutí
// PRINT_CALL | --- | PARAM | --- |
//						 ^
//						 |
//						 | -- ukazatel do tabulky další prvek k vytisknutí
// PRINT_CALL | --- | --- | --- |
//     -- Vytiskne odřádkování
int gen_print_call(instr_t* instrukce){
	htab_item_t* item = NULL;

	string_t *vypis_normalne = generate_label();

	if(vypis_normalne == NULL)
	{
		return ERR_INTER_RUN;
	}


	if(instrukce->addr1 == NULL && instrukce->addr2 == NULL && instrukce->addr3 == NULL)
	{
		//Pokud jsou vsechny NULL, mam dat odradkovani
		printf("WRITE string@\\010\n");
	}
	else if(instrukce->addr1 == NULL && instrukce->addr2 != NULL && instrukce->addr3 == NULL)
	{
		item = (htab_item_t*)(instrukce->addr2);
		printf("WRITE string@\\032\n");
		printf("JUMPIFNEQ %s GF@%s nil@nil\n", string_get(vypis_normalne), string_get(item->k));
		printf("WRITE string@None\n");

		printf("LABEL %s\n", string_get(vypis_normalne));

		printf("WRITE GF@%s\n", string_get(item->k));
	}
	else if(instrukce->addr1 != NULL && instrukce->addr2 == NULL && instrukce->addr3 == NULL)
	{ 
		//Pokud je to identifikator:
		item = (htab_item_t*)(instrukce->addr1);

		printf("JUMPIFNEQ %s GF@%s nil@nil\n", string_get(vypis_normalne), string_get(item->k));
		printf("WRITE string@None\n");

		printf("LABEL %s\n", string_get(vypis_normalne));
		printf("WRITE GF@%s\n",string_get(item->k));
	}
	else 
	{
		return ERR_INTER_RUN;
	}

	return ERR_OK;
}

/**
 * Funkce pro načtení int
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// INPUTI_CALL | DST | --- | --- |
//				  ^
//  			  |
//				  |--- ukazatel na do tabulky na levou stranu přiřazení
int gen_inputi_call(instr_t *instrukce)
{
	htab_item_t *dst = (htab_item_t*)instrukce->addr1;		// okazatel do tabulky na dest

	if(dst->type !=  IDENTIFIER)
	{
		return ERR_INTER_RUN;	
	}

	printf("READ GF@%s int\n", string_get(dst->k));

	return ERR_OK;
}

/**
 * Funkce pro načtení flaot
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// INPUTF_CALL | DST | --- | --- |
//				  ^
//  			  |
//				  |--- ukazatel na do tabulky na levou stranu přiřazení
int gen_inputf_call(instr_t *instrukce)
{
	htab_item_t *dst = (htab_item_t*)instrukce->addr1;		// okazatel do tabulky na dest

	if(dst->type !=  IDENTIFIER)
	{
		return ERR_INTER_RUN;	
	}

	printf("READ GF@%s float\n", string_get(dst->k));

	return ERR_OK;
}

/**
 * Funkce pro načtení string
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// INPUTS_CALL | DST | --- | --- |
//				  ^
//  			  |
//				  |--- ukazatel na do tabulky na levou stranu přiřazení
int gen_inputs_call(instr_t *instrukce)
{
	//INFO;
	htab_item_t *dst = (htab_item_t*)instrukce->addr1;		// okazatel do tabulky na dest

	if(dst->type !=  IDENTIFIER)
	{
		return ERR_INTER_RUN;	//TODO
	}

	printf("READ GF@%s string\n", string_get(dst->k));

	return ERR_OK;
}

/**
 * Funkce pro zjištění déky řetězce
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// LEN_CALL | DST | STR | --- |
//				^	 ^
//  			|	 |
//				|	 | --- ukazatel do tabulky na řetězec, ze kterého se počítá délka
//				|--- ukazatel na do tabulky na levou stranu přiřazení
int gen_len_call(instr_t *instrukce, string_t* parrent)
{
	string_t *label = generate_label();
	string_t *type = generate_label();
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *string = (htab_item_t*)(instrukce->addr2);

	if(label == NULL || type == NULL || dst == NULL || string == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n",string_get(type));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(type));
	}


	printf("TYPE GF@%s GF@%s\n", string_get(type), string_get(string->it_data->str));
	printf("JUMPIFEQ %s GF@%s string@string\n", string_get(label), string_get(type));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label));
	printf("STRLEN GF@%s GF@%s\n", string_get(dst->it_data->str), string_get(string->it_data->str));

	return ERR_OK;
}

/**
 * Funkce pro zjištění ordinální hodnoty
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// ORD_CALL | DST | STR | EXPR |
//				^	 ^		^
//				|	 |		|
//				|	 |		| --- ukazatel do tabulky na vypočtený číselný výraz
//				| 	 |--- ukazatel do tabulky na zpracovávaný řetězec
//				| -- ukazatel do tabulky na L hodnotu
int gen_ord_call(instr_t *instrukce, string_t *parrent)
{
	string_t *label_string = generate_label();
	string_t *label_float = generate_label();
	string_t *label_int = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_end = generate_label();
	string_t *label_dolnimez = generate_label();
	string_t *label_hornimez = generate_label();

	string_t *typeS = generate_label();
	string_t *typeI = generate_label();
	string_t *len = generate_label();
	string_t *tmp = generate_label();
	string_t *boolean = generate_label();

	if(label_string == NULL || label_float == NULL || label_int == NULL ||
	   label_vypocet == NULL || label_vypocet == NULL || label_end == NULL ||
	   typeS == NULL || typeI == NULL || len == NULL || boolean == NULL || 
	   tmp == NULL || label_hornimez == NULL || label_dolnimez == NULL)
	{
		return ERR_INTER_RUN;
	}

	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *string = (htab_item_t*)(instrukce->addr2);
	htab_item_t *idx = (htab_item_t*)(instrukce->addr3);

	if(dst == NULL || string == NULL || idx == NULL)
	{
		return ERR_INTER_RUN;
	}


	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(typeS));
		printf("DEFVAR GF@%s\n", string_get(typeI));
		printf("DEFVAR GF@%s\n", string_get(len));
		printf("DEFVAR GF@%s\n", string_get(tmp));
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(typeS));
		printf("DEFVAR GF@%s\n", string_get(typeI));
		printf("DEFVAR GF@%s\n", string_get(len));
		printf("DEFVAR GF@%s\n", string_get(tmp));
		printf("DEFVAR GF@%s\n", string_get(boolean));
	}


	printf("TYPE GF@%s GF@%s\n", string_get(typeS), string_get(string->k));
	printf("JUMPIFEQ %s GF@%s string@string\n", string_get(label_string), string_get(typeS));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_string));

	printf("STRLEN GF@%s GF@%s\n", string_get(len), string_get(string->k));

	printf("TYPE GF@%s GF@%s\n", string_get(typeI), string_get(idx->k));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_int), string_get(typeI));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_float), string_get(typeI));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_float));
	printf("FLOAT2INT GF@%s GF@%s\n", string_get(tmp), string_get(idx->k));
	printf("JUMP %s\n", string_get(label_dolnimez));

	printf("LABEL %s\n", string_get(label_int));
	printf("MOVE GF@%s GF@%s\n", string_get(tmp), string_get(idx->k));

	printf("LABEL %s\n", string_get(label_dolnimez));

	printf("GT GF@%s GF@%s int@-1\n", string_get(boolean), string_get(tmp));
	printf("JUMPIFEQ %s GF@%s bool@true\n", string_get(label_hornimez), string_get(boolean));
	printf("MOVE GF@%s nil@nil\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_hornimez));

	printf("LT GF@%s GF@%s GF@%s\n", string_get(boolean), string_get(tmp), string_get(len));
	printf("JUMPIFEQ %s GF@%s bool@true\n", string_get(label_vypocet), string_get(boolean));
	printf("MOVE GF@%s nil@nil\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_vypocet));
	printf("STRI2INT GF@%s GF@%s GF@%s\n", string_get(dst->k), string_get(string->k), string_get(tmp));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Funkce pro součet
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// | ADD/SUB_COM | dst | levy oper | pravy oper  |
//					^		^			^
//					|		|			|--ukazatel do tabluky na zdroj
//					|		| -- ukazatel do tabulky na zdroj
//					| --- ukazatel do tabulky na nové umístění
int gen_add_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_test7 = generate_label();
	string_t *label_float1 = generate_label();
	string_t *label_vypocet = generate_label();

	string_t *label_end = generate_label();
	string_t *label_concat = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(typeA == NULL || typeB == NULL || tmpA == NULL ||
	   tmpB == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_test7 == NULL || label_float1 == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_concat == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeB), string_get(b->k));

	printf("JUMPIFNEQ %s GF@%s string@string\n", string_get(label_test2), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_concat), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("JUMPIFNEQ %s GF@%s string@string\n", string_get(label_test3), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test3));
	printf("JUMPIFNEQ %s GF@%s string@nil\n", string_get(label_test4), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test4));
	printf("JUMPIFNEQ %s GF@%s string@nil\n", string_get(label_test5), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFNEQ %s GF@%s string@bool\n", string_get(label_test6), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test6));
	printf("JUMPIFNEQ %s GF@%s string@bool\n", string_get(label_test7), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test7));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));

	printf("JUMPIFNEQ %s GF@%s string@float\n", string_get(label_float1), string_get(typeA));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));

	printf("JUMP %s\n", string_get(label_vypocet));
	printf("LABEL %s\n", string_get(label_float1));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("ADD GF@%s GF@%s GF@%s\n", string_get(dst->k), string_get(tmpA), string_get(tmpB));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_concat));
	printf("CONCAT GF@%s GF@%s GF@%s\n", string_get(dst->k), string_get(a->k), string_get(b->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Funkce pro rozdíl
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
//  SUB_COM | dst | levy oper | pravy oper  |
//				^		^			^
//				|		|			|--ukazatel do tabluky na zdroj
//				|		| -- ukazatel do tabulky na zdroj
//				| --- ukazatel do tabulky na nové umístění
int gen_sub_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_test7 = generate_label();
	string_t *label_float1 = generate_label();
	string_t *label_vypocet = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(typeA == NULL || typeB == NULL || tmpA == NULL ||
	   tmpB == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_test7 == NULL || label_float1 == NULL || label_vypocet == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeB), string_get(b->k));

	printf("JUMPIFNEQ %s GF@%s string@string\n", string_get(label_test2), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("JUMPIFNEQ %s GF@%s string@string\n", string_get(label_test3), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test3));
	printf("JUMPIFNEQ %s GF@%s string@nil\n", string_get(label_test4), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test4));
	printf("JUMPIFNEQ %s GF@%s string@nil\n", string_get(label_test5), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFNEQ %s GF@%s string@bool\n", string_get(label_test6), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test6));
	printf("JUMPIFNEQ %s GF@%s string@bool\n", string_get(label_test7), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test7));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));

	printf("JUMPIFNEQ %s GF@%s string@float\n", string_get(label_float1), string_get(typeA));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));

	printf("JUMP %s\n", string_get(label_vypocet));
	printf("LABEL %s\n", string_get(label_float1));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("SUB GF@%s GF@%s GF@%s\n", string_get(dst->k), string_get(tmpA), string_get(tmpB));

	return ERR_OK;

}

/**
 * Funkce pro součin
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
//  MUL | dst | levy oper | pravy oper  |
//			^		^			^
//			|		|			|--ukazatel do tabluky na zdroj
//			|		| -- ukazatel do tabulky na zdroj
//			| --- ukazatel do tabulky na nové umístění
int gen_mul_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_test7 = generate_label();
	string_t *label_float1 = generate_label();
	string_t *label_vypocet = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(typeA == NULL || typeB == NULL || tmpA == NULL ||
	   tmpB == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_test7 == NULL || label_float1 == NULL || label_vypocet == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeB), string_get(b->k));

	printf("JUMPIFNEQ %s GF@%s string@string\n", string_get(label_test2), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("JUMPIFNEQ %s GF@%s string@string\n", string_get(label_test3), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test3));
	printf("JUMPIFNEQ %s GF@%s string@nil\n", string_get(label_test4), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test4));
	printf("JUMPIFNEQ %s GF@%s string@nil\n", string_get(label_test5), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFNEQ %s GF@%s string@bool\n", string_get(label_test6), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test6));
	printf("JUMPIFNEQ %s GF@%s string@bool\n", string_get(label_test7), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test7));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));

	printf("JUMPIFNEQ %s GF@%s string@float\n", string_get(label_float1), string_get(typeA));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));

	printf("JUMP %s\n", string_get(label_vypocet));
	printf("LABEL %s\n", string_get(label_float1));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("MUL GF@%s GF@%s GF@%s\n", string_get(dst->k), string_get(tmpA), string_get(tmpB));

	return ERR_OK;

}

/**
 * Funkce pro podíl
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// DIV_COM | dst | levy oper | pravy oper  |
//				^		^			^
//				|		|			|--ukazatel do tabluky na zdroj
//				|		| -- ukazatel do tabulky na zdroj
//				| --- ukazatel do tabulky na nové umístění
int gen_div_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_test7 = generate_label();
	string_t *label_float1 = generate_label();
	string_t *label_testB = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_test_zero = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(typeA == NULL || typeB == NULL || tmpA == NULL || label_test_zero == NULL ||
	   tmpB == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_test7 == NULL || label_float1 == NULL || label_vypocet == NULL || label_testB == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));TOK
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeB), string_get(b->k));

	printf("JUMPIFNEQ %s GF@%s string@string\n", string_get(label_test2), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("JUMPIFNEQ %s GF@%s string@string\n", string_get(label_test3), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test3));
	printf("JUMPIFNEQ %s GF@%s string@nil\n", string_get(label_test4), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test4));
	printf("JUMPIFNEQ %s GF@%s string@nil\n", string_get(label_test5), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFNEQ %s GF@%s string@bool\n", string_get(label_test6), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test6));
	printf("JUMPIFNEQ %s GF@%s string@bool\n", string_get(label_test7), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test7));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_testB), string_get(typeA));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));

	printf("LABEL %s\n", string_get(label_testB));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_test_zero), string_get(typeB));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("LABEL %s\n", string_get(label_test_zero));
	printf("JUMPIFNEQ %s GF@%s float@0x0.0p+0\n", string_get(label_vypocet), string_get(tmpB));
	printf("EXIT int@9\n");

	printf("LABEL %s\n", string_get(label_vypocet));
	printf("DIV GF@%s GF@%s GF@%s\n", string_get(dst->k), string_get(tmpA), string_get(tmpB));

	return ERR_OK;
}

/**
 * Funkce pro celočíselné dělení
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// IDIV_COM | dst | levy oper | pravy oper  |
//				^		^			^
//				|		|			|--ukazatel do tabluky na zdroj
//				|		| -- ukazatel do tabulky na zdroj
//				| --- ukazatel do tabulky na nové umístění
int gen_idiv_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_test7 = generate_label();
	string_t *label_float1 = generate_label();
	string_t *label_testB = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_test_zero = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(typeA == NULL || typeB == NULL || tmpA == NULL || label_test_zero == NULL ||
	   tmpB == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_test7 == NULL || label_float1 == NULL || label_vypocet == NULL || label_testB == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeB), string_get(b->k));

	printf("JUMPIFNEQ %s GF@%s string@string\n", string_get(label_test2), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("JUMPIFNEQ %s GF@%s string@string\n", string_get(label_test3), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test3));
	printf("JUMPIFNEQ %s GF@%s string@nil\n", string_get(label_test4), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test4));
	printf("JUMPIFNEQ %s GF@%s string@nil\n", string_get(label_test5), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFNEQ %s GF@%s string@bool\n", string_get(label_test6), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test6));
	printf("JUMPIFNEQ %s GF@%s string@bool\n", string_get(label_test7), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test7));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_testB), string_get(typeA));
	printf("FLOAT2INT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));

	printf("LABEL %s\n", string_get(label_testB));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_test_zero), string_get(typeB));
	printf("FLOAT2INT GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("LABEL %s\n", string_get(label_test_zero));
	printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_vypocet), string_get(tmpB));
	printf("EXIT int@9\n");

	printf("LABEL %s\n", string_get(label_vypocet));
	printf("IDIV GF@%s GF@%s GF@%s\n", string_get(dst->k), string_get(tmpA), string_get(tmpB));

	return ERR_OK;
}
/**
 * Následuje soubor funkcí pro porovnání
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// | MUL/DIV_COM | dst | levy oper | pravy oper  |
//					^		^			^
//					|		|			|--ukazatel do tabluky na zdroj
//					|		| -- ukazatel do tabulky na zdroj
//					| --- ukazatel do tabulky na nové umístění
// | CMP_GEQ/GRE/EQ/LES/LEQ/NEQ/ | dst | levy oper | pravy oper  |
//									^		^			^
//									|		|			|--ukazatel do tabluky na zdroj
//									|		| -- ukazatel do tabulky na zdroj
//									| --- ukazatel do tabulky na nové umístění
int gen_les_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s GF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("EXIT int@4\n");
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_none));
	//printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("LT GF@%s GF@%s GF@%s\n", string_get(boolean), string_get(tmpA), string_get(tmpB));

	printf("JUMPIFEQ %s GF@%s bool@false\n", string_get(label_false), string_get(boolean));
	printf("MOVE GF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE GF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Popis funkce viz výše
 */
// | CMP_GEQ/GRE/EQ/LES/LEQ/NEQ/ | dst | levy oper | pravy oper  |
//									^		^			^
//									|		|			|--ukazatel do tabluky na zdroj
//									|		| -- ukazatel do tabulky na zdroj
//									| --- ukazatel do tabulky na nové umístění
int gen_gr_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s GF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("EXIT int@4\n");
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_none));
	//printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("GT GF@%s GF@%s GF@%s\n", string_get(boolean), string_get(tmpA), string_get(tmpB));

	printf("JUMPIFEQ %s GF@%s bool@false\n", string_get(label_false), string_get(boolean));
	printf("MOVE GF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE GF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Popis funkce viz výše
 */
// | CMP_GEQ/GRE/EQ/LES/LEQ/NEQ/ | dst | levy oper | pravy oper  |
//									^		^			^
//									|		|			|--ukazatel do tabluky na zdroj
//									|		| -- ukazatel do tabulky na zdroj
//									| --- ukazatel do tabulky na nové umístění
int gen_leq_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean1 = generate_label();
	string_t *boolean2 = generate_label();
	string_t *boolean3 = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean1 == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL || boolean2 == NULL || boolean3 == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(boolean1));
		printf("DEFVAR GF@%s\n", string_get(boolean2));
		printf("DEFVAR GF@%s\n", string_get(boolean3));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(boolean1));
		printf("DEFVAR GF@%s\n", string_get(boolean2));
		printf("DEFVAR GF@%s\n", string_get(boolean3));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s GF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("EXIT int@4\n");
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_none));
	//printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("LT GF@%s GF@%s GF@%s\n", string_get(boolean1), string_get(tmpA), string_get(tmpB));
	printf("EQ GF@%s GF@%s GF@%s\n", string_get(boolean2), string_get(tmpA), string_get(tmpB));
	printf("OR GF@%s GF@%s GF@%s\n", string_get(boolean3), string_get(boolean1), string_get(boolean2));


	printf("JUMPIFEQ %s GF@%s bool@false\n", string_get(label_false), string_get(boolean3));
	printf("MOVE GF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE GF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Popis funkce viz výše
 */
// | CMP_GEQ/GRE/EQ/LES/LEQ/NEQ/ | dst | levy oper | pravy oper  |
//									^		^			^
//									|		|			|--ukazatel do tabluky na zdroj
//									|		| -- ukazatel do tabulky na zdroj
//									| --- ukazatel do tabulky na nové umístění
int gen_geq_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean1 = generate_label();
	string_t *boolean2 = generate_label();
	string_t *boolean3 = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean1 == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL || boolean2 == NULL || boolean3 == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(boolean1));
		printf("DEFVAR GF@%s\n", string_get(boolean2));
		printf("DEFVAR GF@%s\n", string_get(boolean3));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(boolean1));
		printf("DEFVAR GF@%s\n", string_get(boolean2));
		printf("DEFVAR GF@%s\n", string_get(boolean3));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s GF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("EXIT int@4\n");
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_none));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("GT GF@%s GF@%s GF@%s\n", string_get(boolean1), string_get(tmpA), string_get(tmpB));
	printf("EQ GF@%s GF@%s GF@%s\n", string_get(boolean2), string_get(tmpA), string_get(tmpB));
	printf("OR GF@%s GF@%s GF@%s\n", string_get(boolean3), string_get(boolean1), string_get(boolean2));


	printf("JUMPIFEQ %s GF@%s bool@false\n", string_get(label_false), string_get(boolean3));
	printf("MOVE GF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE GF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Popis funkce viz výše
 */
int gen_eq_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s GF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE GF@%s int@0\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("MOVE GF@%s int@0\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("MOVE GF@%s int@0\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE GF@%s int@0\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_none));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE GF@%s int@0\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("EQ GF@%s GF@%s GF@%s\n", string_get(boolean), string_get(tmpA), string_get(tmpB));

	printf("JUMPIFEQ %s GF@%s bool@false\n", string_get(label_false), string_get(boolean));
	printf("MOVE GF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE GF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Popis funkce viz výše
 */
int gen_neq_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(typeB));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(tmpB));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s GF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE GF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("MOVE GF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("MOVE GF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT GF@%s GF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE GF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_none));
	printf("JUMPIFEQ %s GF@%s GF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE GF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("EQ GF@%s GF@%s GF@%s\n", string_get(boolean), string_get(tmpA), string_get(tmpB));

	printf("JUMPIFNEQ %s GF@%s bool@false\n", string_get(label_false), string_get(boolean));
	printf("MOVE GF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE GF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Funkce pro získání písmena z číselné hodnoty
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// CHR_CALL | DST | EXPR | --- |
//				^	 ^		
//				|	 |		
//				|	 |	--- ukazatel do tabulky na číselný parametr funkce chr	
//				| -- zkazatel do tabulky na L hodnotu	
int gen_chr_call(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *tmpA = generate_label();

	string_t *label_string = generate_label();
	string_t *label_test1 = generate_label();
	string_t *label_test2 = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_pretypovani = generate_label();

	if(dst == NULL || a == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || tmpA == NULL || label_string == NULL ||
	   label_test1 == NULL || label_test2 == NULL || label_vypocet == NULL || label_pretypovani == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(tmpA));
		printf("DEFVAR GF@%s\n", string_get(typeA));
	}

	printf("MOVE GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(a->k));

	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_test1), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_pretypovani), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_pretypovani));
	printf("FLOAT2INT GF@%s GF@%s\n", string_get(tmpA), string_get(a->k));

	printf("LABEL %s\n", string_get(label_test1));
	printf("LT GF@%s GF@%s int@0\n", string_get(boolean), string_get(a->k));
	printf("JUMPIFNEQ %s GF@%s bool@true\n", string_get(label_test2), string_get(boolean));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("GT GF@%s GF@%s int@255\n", string_get(boolean), string_get(a->k));
	printf("JUMPIFNEQ %s GF@%s bool@true\n", string_get(label_vypocet), string_get(boolean));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_vypocet));
	printf("INT2CHAR GF@%s GF@%s\n", string_get(dst->k), string_get(tmpA));

	return ERR_OK;

}

/**
 * Funkce generující strukturu if-else
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// IF_COM | expr | else(false) | konec
//           ^			^			^
//			 |			|			|
//			 |			|			|-- ukazatel do pásky na poslední příkaz elsu (poslední if-else)
//			 |			|-- ukazatel do pásky na poslední příkaz if části před else
//			 |-- ukazatel do tabulky na hodnotu řídicího výrazu
int gen_if_call(instr_t *instrukce, instr_tape_t *main_tape, string_t *parrent)
{
	htab_item_t *exp = (htab_item_t*)(instrukce->addr1);
	instr_t *el = (instr_t*)(instrukce->addr2);
	instr_t *en = (instr_t*)(instrukce->addr3);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_calc = generate_label();
	string_t *label_else = generate_label();
	string_t *label_end = generate_label();

	if(exp == NULL || el == NULL || en == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_else == NULL || label_calc == NULL ||
	   label_end == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(typeA));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(exp->k));

	printf("JUMPIFEQ %s GF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("MOVE GF@%s bool@false\n", string_get(boolean));
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_int));
	printf("EQ GF@%s GF@%s int@0\n", string_get(boolean), string_get(exp->k)); 
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_float));
	printf("EQ GF@%s GF@%s float@0x0.0p+0\n", string_get(boolean), string_get(exp->k)); 
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_bool));
	printf("MOVE GF@%s GF@%s\n", string_get(boolean), string_get(exp->k));
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_none));
	printf("MOVE GF@%s int@0\n", string_get(boolean));

	printf("LABEL %s\n", string_get(label_calc));
	printf("JUMPIFEQ %s GF@%s bool@true\n",string_get(label_else), string_get(boolean));

	while(instrukce != (instr_t*)el)
	{

		instrukce=tape_read(main_tape);	
	    make_statements(instrukce, main_tape, parrent);	

	    if(!tape_is_active(main_tape) || en->next == tape_actual(main_tape))
	    {
	    	break;
	    }
	}
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_else));
	while(instrukce != (instr_t*)en)
	{

		instrukce=tape_read(main_tape);	
	    make_statements(instrukce, main_tape, parrent);	

	    if(!tape_is_active(main_tape) || en->next == tape_actual(main_tape))
	    {
	    	break;
	    }
	}
	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;
}

/**
 * Funkce generující strukturu while
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// WHILE_COM | expr | begin | end
//				^		^		^
//				|		|		|
//				|		|		|-- ukazatel do pásky na poslední příkaz whilu
//				|		|-- ukazatel do pásky na první příkaz před tělem whilu
//				|-- ukazatel do tabulky na řídící výraz
int gen_while_call(instr_t *instrukce, instr_tape_t *main_tape, string_t *parrent)
{
	htab_item_t *expr = (htab_item_t*)(instrukce->addr1);
	instr_t *begin = (instr_t*)(instrukce->addr2);
	instr_t *end = (instr_t*)(instrukce->addr3);


	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *A = generate_label();
	string_t *counter = generate_label();TOK

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_calc = generate_label();

	string_t *label_begin = generate_label();
	string_t *label_end = generate_label();

	if(expr == NULL || begin == NULL || end == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_begin == NULL || label_calc == NULL ||
	   label_end == NULL || A == NULL || counter == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(A));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(counter));
		printf("MOVE GF@%s int@0\n", string_get(counter));TOK
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(A));
		printf("DEFVAR GF@%s\n", string_get(typeA));
		printf("DEFVAR GF@%s\n", string_get(counter));
		printf("MOVE GF@%s int@0\n", string_get(counter));TOK
	}

	printf("LABEL %s\n", string_get(label_begin));
	while(instrukce != begin)
	{
		instrukce=tape_read(main_tape);	
	    make_statements(instrukce, main_tape, counter);

	    if(!tape_is_active(main_tape) || end->next == tape_actual(main_tape))
	    {
	    	break;
	    }
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeA), string_get(expr->k));

	printf("JUMPIFEQ %s GF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s GF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("MOVE GF@%s bool@false\n", string_get(boolean));
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_int));
	printf("EQ GF@%s GF@%s int@0\n", string_get(boolean), string_get(expr->k)); 
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_float));
	printf("EQ GF@%s GF@%s float@0x0.0p+0\n", string_get(boolean), string_get(expr->k)); 
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_bool));
	printf("MOVE GF@%s GF@%s\n", string_get(boolean), string_get(expr->k));
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_none));
	printf("MOVE GF@%s int@0\n", string_get(boolean));

	printf("LABEL %s\n", string_get(label_calc));
	printf("JUMPIFEQ %s GF@%s bool@true\n",string_get(label_end), string_get(boolean));	// true a false je naopak

	while(instrukce != end)
	{
		instrukce=tape_read(main_tape);	
	    make_statements(instrukce, main_tape, counter);	

	    if(!tape_is_active(main_tape) || end->next == tape_actual(main_tape))
	    {
	    	break;
	    }
	}

	printf("ADD GF@%s GF@%s int@1\n", string_get(counter), string_get(counter));TOK
	printf("JUMP %s\n", string_get(label_begin));
	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;	
}

/**
 * Funkce kontroluje parametry a spouští vestavěnou funkci substr
 * instrukce - aktuální vykonávaná instrukce
 * main_tape - instrukční páska
 * parrent - pro kontrolu zanoření příkazu, 
 * 
 * Funkce vrací 0 v případě úspěchu, jinak kód chyby.
 */
 // SUBSTR_CALL | DST | --- | --- |
//				  ^
//  			  |
//				  |--- ukazatel na do tabulky na levou stranu přiřazení
int gen_substr_call(instr_tape_t *main_tape, instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);

	if(dst == NULL)
	{
		return ERR_INTER_RUN;
	}

	instrukce = tape_read(main_tape);
	if(instrukce == NULL)
	{
		return ERR_INTER_RUN;
	}
	if(instrukce->instr_code != SUBSTR_PAR)
	{
		return ERR_INTER_RUN;
	}

	htab_item_t *s = (htab_item_t*)(instrukce->addr1);
	htab_item_t *i = (htab_item_t*)(instrukce->addr2);
	htab_item_t *n = (htab_item_t*)(instrukce->addr3);

	if(s == NULL || i == NULL || n == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *typeS = generate_label();
	string_t *typeI = generate_label();
	string_t *typeN = generate_label();
	string_t *strLen = generate_label();
	string_t *tmp = generate_label();
	string_t *tmp2 = generate_label();
	string_t *boolean = generate_label();
	string_t *ch = generate_label();
	string_t *toend = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_pretyp1 = generate_label();
	string_t *label_pretyp2 = generate_label();
	string_t *label_end = generate_label();


	if(typeS == NULL || typeI == NULL || typeN == NULL || strLen == NULL ||
	   tmp == NULL || tmp2 == NULL || boolean == NULL || ch == NULL ||
	   toend == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_vypocet == NULL || label_pretyp1 == NULL || label_pretyp2 == NULL || 
	   label_end == NULL)
	{
		return ERR_INTER_RUN;
	}


	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}
	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s GF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF%s\n", string_get(typeS));
		printf("DEFVAR GF%s\n", string_get(typeI));
		printf("DEFVAR GF%s\n", string_get(typeN));
		printf("DEFVAR GF%s\n", string_get(strLen));
		printf("DEFVAR GF%s\n", string_get(tmp));
		printf("DEFVAR GF%s\n", string_get(tmp2));
		printf("DEFVAR GF%s\n", string_get(boolean));
		printf("DEFVAR GF%s\n", string_get(ch));
		printf("DEFVAR GF%s\n", string_get(toend));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR GF@%s\n", string_get(typeS));
		printf("DEFVAR GF@%s\n", string_get(typeI));
		printf("DEFVAR GF@%s\n", string_get(typeN));
		printf("DEFVAR GF@%s\n", string_get(strLen));
		printf("DEFVAR GF@%s\n", string_get(tmp));
		printf("DEFVAR GF@%s\n", string_get(tmp2));
		printf("DEFVAR GF@%s\n", string_get(boolean));
		printf("DEFVAR GF@%s\n", string_get(ch));
		printf("DEFVAR GF@%s\n", string_get(toend));
	}

	printf("TYPE GF@%s GF@%s\n", string_get(typeS), string_get(s->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeI), string_get(i->k));
	printf("TYPE GF@%s GF@%s\n", string_get(typeN), string_get(n->k));

	printf("JUMPIFEQ %s GF@%s string@string\n", string_get(label_test2), string_get(typeS));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("STRLEN GF@%s GF@%s\n", string_get(strLen), string_get(s->k));
	
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_test3), string_get(typeI));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_pretyp1), string_get(typeI));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_pretyp1));
	printf("FLOAT2INT GF@%s GF@%s\n", string_get(tmp), string_get(i->k));
	printf("MOVE GF@%s GF@%s\n", string_get(i->k), string_get(tmp));

	printf("LABEL %s\n", string_get(label_test3));
	printf("GT GF@%s GF@%s int@-1\n", string_get(boolean), string_get(i->k));
	printf("JUMPIFEQ %s GF@%s bool@true\n", string_get(label_test4), string_get(boolean));
	printf("MOVE GF@%s nil@nil\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_test4));
	printf("LT GF@%s GF@%s GF@%s\n", string_get(boolean), string_get(i->k), string_get(strLen));
	printf("JUMPIFEQ %s GF@%s bool@true\n", string_get(label_test5), string_get(boolean));
	printf("MOVE GF@%s nil@nil\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFEQ %s GF@%s string@int\n", string_get(label_test6), string_get(typeN));
	printf("JUMPIFEQ %s GF@%s string@float\n", string_get(label_pretyp2), string_get(typeN));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_pretyp2));
	printf("FLOAT2INT GF@%s GF@%s\n", string_get(tmp), string_get(n->k));
	printf("MOVE GF@%s GF@%s\n", string_get(n->k), string_get(tmp));

	printf("LABEL %s\n", string_get(label_test6));
	printf("MOVE GF@%s GF@%s\n", string_get(tmp), string_get(i->k));
	printf("MOVE GF@%s int@0\n", string_get(tmp2));
	printf("GETCHAR GF@%s GF@%s GF@%s\n", string_get(dst->k), string_get(s->k), string_get(tmp));
	printf("ADD GF@%s GF@%s int@1\n", string_get(tmp), string_get(tmp));
	printf("ADD GF@%s GF@%s int@1\n", string_get(tmp2), string_get(tmp2));
	printf("MOVE GF@%s GF@%s\n", string_get(toend), string_get(strLen));
	printf("SUB GF@%s GF@%s GF@%s\n", string_get(toend), string_get(toend), string_get(i->k));

	printf("GT GF@%s GF@%s int@0\n", string_get(boolean), string_get(n->k));
	printf("JUMPIFEQ %s GF@%s bool@true\n", string_get(label_vypocet), string_get(boolean));
	printf("MOVE GF@%s nil@nil\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_vypocet));
	printf("LT GF@%s GF@%s GF@%s\n", string_get(boolean), string_get(tmp2), string_get(n->k));
	printf("JUMPIFEQ %s GF@%s bool@false\n", string_get(label_end), string_get(boolean));
	printf("LT GF@%s GF@%s GF@%s\n", string_get(boolean), string_get(tmp2), string_get(toend));
	printf("JUMPIFEQ %s GF@%s bool@false\n", string_get(label_end), string_get(boolean));

	printf("GETCHAR GF@%s GF@%s GF@%s\n", string_get(ch), string_get(s->k), string_get(tmp));
	printf("CONCAT GF@%s GF@%s GF@%s\n", string_get(dst->k), string_get(dst->k), string_get(ch));
	printf("ADD GF@%s GF@%s int@1\n", string_get(tmp), string_get(tmp));
	printf("ADD GF@%s GF@%s int@1\n", string_get(tmp2), string_get(tmp2));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

// DEF_COM | params | end | ----- |
//				^		^
//				|		|
//				|		|-- ukazatel do pásky na poslední příkaz v definici
//				|-- na funkci
int gen_def_call(instr_t *instrukce, instr_tape_t *main_tape, string_t* parrent)
{
	string_t *fce = (string_t*)(instrukce->addr3);
	instr_t *end_par = (instr_t*)(instrukce->addr1);
	instr_t *end = (instr_t*)(instrukce->addr2);

	string_t *label_end_def = generate_label();

	if(fce == NULL || end == NULL || label_end_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	printf("JUMP %s\n", string_get(label_end_def));
	printf("LABEL %s\n", string_get(fce));
	printf("PUSHFRAME\n");

	while(instrukce != end_par) instrukce=tape_read(main_tape);	

	while(instrukce != end)
	{
		instrukce=tape_read(main_tape);	
	    make_statements_f(instrukce, main_tape, NULL);	

	    if(!tape_is_active(main_tape) || end->next == tape_actual(main_tape))
	    {
	    	break;
	    }
	}

	printf("POPFRAME\n");
	printf("RETURN\n");
	printf("LABEL %s\n", string_get(label_end_def));
}

//  CALL | *fce | id_last | --- |
//			^		^
//			|		|
//			|		|
//			|		|--- ukazatel do pásky na poslední operaci s parametrem
//			|--- ukazatel do tabulky na funkci,(ne jeji tabulku)
int gen_call_call(instr_tape_t *main_tape, instr_t *instrukce, string_t *parrent)
{
	htab_item_t *fce = (htab_item_t*)(instrukce->addr1);
	instr_t *end = (instr_t*)(instrukce->addr2);
	htab_item_t *param1 = NULL;

	if(fce == NULL || end == NULL)
	{
		return ERR_INTER_RUN;
	}

	printf("CREATEFRAME\n");

	param_t *tmp = fce->it_data->fce->h_table->params;
	while(instrukce != end)
	{
		printf("DEFVAR TF@%s\n", string_get(tmp->ptr->k));
		

		string_t *strin = NULL;

		instrukce=tape_read(main_tape);	
		if(instrukce->instr_code == PARAM)
		{
			// PARAM | *id | --- | --- |
			//			^
			//			|
			//			|--- parametr je ukazatelem na prvek do tabulky
			param1 = (htab_item_t*)(instrukce->addr1);
			if(param1 == NULL)
			{
				return ERR_INTER_RUN;
			}

							// Pokud je src NULL, pak dojde k nahrání hodnoty nill
			if(param1->type == NONE)
			{
				printf("MOVE TF@%s nil@nil\n", string_get(tmp->ptr->k));
			}
			else
			{
				// Rozhodování o různých způsobech nahrávání
				if(param1->type == INT)				printf("MOVE TF@%s int@%d\n",string_get(tmp->ptr->k),param1->it_data->i);
				else if(param1->type == FLOAT)		printf("MOVE TF@%s float@%a\n",string_get(tmp->ptr->k),param1->it_data->fl);
				else if(param1->type == STRING || param1->type == COMMENT)
				{
					strin = string_to_hex(param1->it_data->str);
					if(strin == NULL)
					{
						return ERR_INTER_RUN;
					}
						printf("MOVE TF@%s string@%s\n",string_get(tmp->ptr->k),string_get(strin));
				}
				else if(param1->type == IDENTIFIER)	printf("MOVE TF@%s GF@%s\n",string_get(tmp->ptr->k),string_get(param1->k));
			}

		//printf("MOVE TF@%s GF@%s\n", string_get(tmp->ptr->k), string_get(param1->k));
		}
		else
		{
		    make_statements_f(instrukce, main_tape, NULL);	
		}

	    if(!tape_is_active(main_tape) || end->next == tape_actual(main_tape))
	    {
	    	break;
	    }

	    tmp = tmp->next;
	}

	printf("CALL %s\n", string_get(fce->k));
}

// DOK_COM |  str | ---- | ----|
//				^	
//				|
//				|-- ukazatel do tabulky na dokumentační řetězec
int gen_com(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *str_item = (htab_item_t*)(instrukce->addr1);

	if(str_item == NULL)
	{
		return ERR_INTER_RUN;
	}

	char *str = string_get(str_item->it_data->str);

	// Alokace nového řetězce
	string_t* new = string_ctor();
	if(new == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_put_char(&new, '#');
	if(new == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Přepisování znak po znaku
	char ch;
	int len = strlen(str);
	for(int i = 0; i < len; i++)
	{
		ch = str[i];
		// Spesiální přepis pokud se jedná o bilý znak
		if(ch == '\n')
		{
			string_put_char(&new, '\n');
			if(new == NULL)
			{
				return ERR_INTER_RUN;
			}

			string_put_char(&new, '#');
			if(new == NULL)
			{
				return ERR_INTER_RUN;
			}
		}
		else
		{
			string_put_char(&new, ch);
			if(new == NULL)
			{
				return ERR_INTER_RUN;
			}
		}
	}

	printf("%s\n", string_get(new));

	return ERR_OK;
}

/** 
 * Funkce rozdělující příkazy na písce jednotlivým funkcím podle jejich kódu
 * -------------------------------------------------------------------------
 * instrukce - aktuální vykonávaná instrukce
 * main_tape - instrukční páska
 * parrent - pro kontrolu zanoření příkazu, 
 * 
 * Funkce vrací 0 v případě úspěchu, jinak kód chyby.
 */
int make_statements(instr_t *instrukce, instr_tape_t *main_tape, string_t *parrent)
{
	if(instrukce == NULL)
	{
		return ERR_OK;
	}

	switch(instrukce->instr_code)
	{
		case(IF_COM):				gen_if_call(instrukce, main_tape, parrent);		break; //OK
		case(WHILE_COM):			gen_while_call(instrukce, main_tape, parrent);	break; //OK
		case(DEF_COM):				gen_def_call(instrukce, main_tape, parrent); break;

		case(PRINT_CALL): 			gen_print_call(instrukce);	break;	// OK
		case(INPUTS_CALL):			gen_inputs_call(instrukce); break;	// OK
		case(INPUTI_CALL):			gen_inputi_call(instrukce); break;	// OK
		case(INPUTF_CALL): 			gen_inputf_call(instrukce); break;	// OK
		case(LEN_CALL):				gen_len_call(instrukce, parrent);	break;
		case(SUBSTR_CALL):			gen_substr_call(main_tape, instrukce, parrent); break;
		case(ORD_CALL): 			gen_ord_call(instrukce, parrent); 	break; // OK
		case(CHR_CALL):				gen_chr_call(instrukce, parrent);	break; // OK

		case(DOK_COM):				gen_com(instrukce, parrent);	break;

		case(CPY_COM): 				gen_cpy_com(instrukce); 	break; // OK

		case(CMP_NEQ):				gen_neq_call(instrukce, parrent);	break; // OK
		case(CMP_EQ):				gen_eq_call(instrukce, parrent);		break; // OK
		case(CMP_LES):				gen_les_call(instrukce, parrent);	break; // OK
		case(CMP_LEQ):				gen_leq_call(instrukce, parrent);	break; // OK
		case(CMP_GRE):				gen_gr_call(instrukce, parrent);		break; // OK
		case(CMP_GEQ):				gen_geq_call(instrukce, parrent);	break; // OK

		case(MUL_COM):				gen_mul_call(instrukce, parrent);	break; // OK
		case(DIV_COM): 				gen_div_call(instrukce, parrent);	break; // OK
		case(DIV_INT_COM):			gen_idiv_call(instrukce, parrent);	break; // OK
		case(ADD_COM): 				gen_add_call(instrukce, parrent); 	break; // OK
		case(SUB_COM): 				gen_sub_call(instrukce, parrent);	break; // OK
	
		case(CALL):					gen_call_call(main_tape, instrukce, parrent); break;

		case(DEF_VAR): 				gen_def_var(instrukce, parrent); 	break; //OK
	}

	return ERR_OK;
}
int compile(instr_tape_t* main_tape){
	printf(".IFJcode19\n");
 	printf("DEFVAR GF@a$type\n");
 	printf("DEFVAR GF@b$type\n");
 	instr_t* instrukce = NULL;


	if(!tape_activate(main_tape)) return ERR_OK;    // Pokud se nepodari aktivovat pasku vrátí se error
	while(tape_is_active(main_tape)){ 				// Probíhá zpracování
	    instrukce=tape_read(main_tape);	
	    make_statements(instrukce, main_tape, NULL);	
	}

	return ERR_OK;
}


int gen_def_var_f(instr_t* instrukce, string_t *parrent){
	htab_item_t* item1 = NULL;
	item1 =instrukce->addr1;
	
	if(instrukce->addr1 == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n",string_get(item1->k));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n",string_get(item1->k));
	}

	return ERR_OK;
}

/**
 * Funkce pro kopírování hodnot
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// CPY_COM | dst | src | --- |
//			  ^     ^
//			  |     |
//			  |     | -- ukazatel na R hodnotu
//			  | -- ukazatel do tabulky na L hodnotu
int gen_cpy_com_f(instr_t* instrukce){
	htab_item_t* item1 = NULL;
 	htab_item_t* item2 = NULL;
	item1 = (htab_item_t*)(instrukce->addr1);
	item2 = (htab_item_t*)(instrukce->addr2);
	string_t *strin = NULL;

	// Pokud je src NULL, pak dojde k nahrání hodnoty nill
	if(instrukce->addr1 != NULL && instrukce->addr2 == NULL)
	{
		printf("MOVE LF@%s nil@nil\n", string_get(item1->k));
	}
	else if(instrukce->addr1 != NULL && instrukce->addr2 != NULL)
	{
		// Rozhodování o různých způsobech nahrávání
		if(item2->type == INT)				printf("MOVE LF@%s int@%d\n",string_get(item1->k),item2->it_data->i);
		else if(item2->type == FLOAT)		printf("MOVE LF@%s float@%a\n",string_get(item1->k),item2->it_data->fl);
		else if(item2->type == STRING || item2->type == COMMENT)
		{
			strin = string_to_hex(item2->it_data->str);
			if(strin == NULL)
			{
				return ERR_INTER_RUN;
			}

			printf("MOVE LF@%s string@%s\n",string_get(item1->k),string_get(strin));
		}
		else if(item2->type == IDENTIFIER)	printf("MOVE LF@%s LF@%s\n",string_get(item1->k),string_get(item2->k));
	}
	else
	{
		return ERR_INTER_RUN;
	}

	return ERR_OK;
}

/**
 * Funkce pro tisk
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// PRINT_CALL | PARAM | --- | --- |
//				 ^
//				 |
//				 | -- ukazatel do tabulky na první parametr k vytisknutí
// PRINT_CALL | --- | PARAM | --- |
//						 ^
//						 |
//						 | -- ukazatel do tabulky další prvek k vytisknutí
// PRINT_CALL | --- | --- | --- |
//     -- Vytiskne odřádkování
int gen_print_call_f(instr_t* instrukce){
	htab_item_t* item = NULL;

	string_t *vypis_normalne = generate_label();

	if(vypis_normalne == NULL)
	{
		return ERR_INTER_RUN;
	}


	if(instrukce->addr1 == NULL && instrukce->addr2 == NULL && instrukce->addr3 == NULL)
	{
		//Pokud jsou vsechny NULL, mam dat odradkovani
		printf("WRITE string@\\010\n");
	}
	else if(instrukce->addr1 == NULL && instrukce->addr2 != NULL && instrukce->addr3 == NULL)
	{
		item = (htab_item_t*)(instrukce->addr2);
		printf("WRITE string@\\032\n");
		printf("JUMPIFNEQ %s LF@%s nil@nil\n", string_get(vypis_normalne), string_get(item->k));
		printf("WRITE string@None\n");

		printf("LABEL %s\n", string_get(vypis_normalne));

		printf("WRITE LF@%s\n", string_get(item->k));
	}
	else if(instrukce->addr1 != NULL && instrukce->addr2 == NULL && instrukce->addr3 == NULL)
	{ 
		//Pokud je to identifikator:
		item = (htab_item_t*)(instrukce->addr1);

		printf("JUMPIFNEQ %s LF@%s nil@nil\n", string_get(vypis_normalne), string_get(item->k));
		printf("WRITE string@None\n");

		printf("LABEL %s\n", string_get(vypis_normalne));
		printf("WRITE LF@%s\n",string_get(item->k));
	}
	else 
	{
		return ERR_INTER_RUN;
	}

	return ERR_OK;
}

/**
 * Funkce pro načtení int
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// INPUTI_CALL | DST | --- | --- |
//				  ^
//  			  |
//				  |--- ukazatel na do tabulky na levou stranu přiřazení
int gen_inputi_call_f(instr_t *instrukce)
{
	htab_item_t *dst = (htab_item_t*)instrukce->addr1;		// okazatel do tabulky na dest

	if(dst->type !=  IDENTIFIER)
	{
		return ERR_INTER_RUN;	
	}

	printf("READ LF@%s int\n", string_get(dst->k));

	return ERR_OK;
}

/**
 * Funkce pro načtení flaot
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// INPUTF_CALL | DST | --- | --- |
//				  ^
//  			  |
//				  |--- ukazatel na do tabulky na levou stranu přiřazení
int gen_inputf_call_f(instr_t *instrukce)
{
	htab_item_t *dst = (htab_item_t*)instrukce->addr1;		// okazatel do tabulky na dest

	if(dst->type !=  IDENTIFIER)
	{
		return ERR_INTER_RUN;	
	}

	printf("READ LF@%s float\n", string_get(dst->k));

	return ERR_OK;
}

/**
 * Funkce pro načtení string
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// INPUTS_CALL | DST | --- | --- |
//				  ^
//  			  |
//				  |--- ukazatel na do tabulky na levou stranu přiřazení
int gen_inputs_call_f(instr_t *instrukce)
{
	//INFO;
	htab_item_t *dst = (htab_item_t*)instrukce->addr1;		// okazatel do tabulky na dest

	if(dst->type !=  IDENTIFIER)
	{
		return ERR_INTER_RUN;	//TODO
	}

	printf("READ LF@%s string\n", string_get(dst->k));

	return ERR_OK;
}

/**
 * Funkce pro zjištění déky řetězce
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// LEN_CALL | DST | STR | --- |
//				^	 ^
//  			|	 |
//				|	 | --- ukazatel do tabulky na řetězec, ze kterého se počítá délka
//				|--- ukazatel na do tabulky na levou stranu přiřazení
int gen_len_call_f(instr_t *instrukce, string_t* parrent)
{
	string_t *label = generate_label();
	string_t *type = generate_label();
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *string = (htab_item_t*)(instrukce->addr2);

	if(label == NULL || type == NULL || dst == NULL || string == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n",string_get(type));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(type));
	}


	printf("TYPE LF@%s LF@%s\n", string_get(type), string_get(string->it_data->str));
	printf("JUMPIFEQ %s LF@%s string@string\n", string_get(label), string_get(type));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label));
	printf("STRLEN LF@%s LF@%s\n", string_get(dst->it_data->str), string_get(string->it_data->str));

	return ERR_OK;
}

/**
 * Funkce pro zjištění ordinální hodnoty
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// ORD_CALL | DST | STR | EXPR |
//				^	 ^		^
//				|	 |		|
//				|	 |		| --- ukazatel do tabulky na vypočtený číselný výraz
//				| 	 |--- ukazatel do tabulky na zpracovávaný řetězec
//				| -- ukazatel do tabulky na L hodnotu
int gen_ord_call_f(instr_t *instrukce, string_t *parrent)
{
	string_t *label_string = generate_label();
	string_t *label_float = generate_label();
	string_t *label_int = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_end = generate_label();
	string_t *label_dolnimez = generate_label();
	string_t *label_hornimez = generate_label();

	string_t *typeS = generate_label();
	string_t *typeI = generate_label();
	string_t *len = generate_label();
	string_t *tmp = generate_label();
	string_t *boolean = generate_label();

	if(label_string == NULL || label_float == NULL || label_int == NULL ||
	   label_vypocet == NULL || label_vypocet == NULL || label_end == NULL ||
	   typeS == NULL || typeI == NULL || len == NULL || boolean == NULL || 
	   tmp == NULL || label_hornimez == NULL || label_dolnimez == NULL)
	{
		return ERR_INTER_RUN;
	}

	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *string = (htab_item_t*)(instrukce->addr2);
	htab_item_t *idx = (htab_item_t*)(instrukce->addr3);

	if(dst == NULL || string == NULL || idx == NULL)
	{
		return ERR_INTER_RUN;
	}


	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(typeS));
		printf("DEFVAR LF@%s\n", string_get(typeI));
		printf("DEFVAR LF@%s\n", string_get(len));
		printf("DEFVAR LF@%s\n", string_get(tmp));
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(typeS));
		printf("DEFVAR LF@%s\n", string_get(typeI));
		printf("DEFVAR LF@%s\n", string_get(len));
		printf("DEFVAR LF@%s\n", string_get(tmp));
		printf("DEFVAR LF@%s\n", string_get(boolean));
	}


	printf("TYPE LF@%s LF@%s\n", string_get(typeS), string_get(string->k));
	printf("JUMPIFEQ %s LF@%s string@string\n", string_get(label_string), string_get(typeS));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_string));

	printf("STRLEN LF@%s LF@%s\n", string_get(len), string_get(string->k));

	printf("TYPE LF@%s LF@%s\n", string_get(typeI), string_get(idx->k));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_int), string_get(typeI));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_float), string_get(typeI));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_float));
	printf("FLOAT2INT LF@%s LF@%s\n", string_get(tmp), string_get(idx->k));
	printf("JUMP %s\n", string_get(label_dolnimez));

	printf("LABEL %s\n", string_get(label_int));
	printf("MOVE LF@%s LF@%s\n", string_get(tmp), string_get(idx->k));

	printf("LABEL %s\n", string_get(label_dolnimez));

	printf("GT LF@%s LF@%s int@-1\n", string_get(boolean), string_get(tmp));
	printf("JUMPIFEQ %s LF@%s bool@true\n", string_get(label_hornimez), string_get(boolean));
	printf("MOVE LF@%s nil@nil\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_hornimez));

	printf("LT LF@%s LF@%s LF@%s\n", string_get(boolean), string_get(tmp), string_get(len));
	printf("JUMPIFEQ %s LF@%s bool@true\n", string_get(label_vypocet), string_get(boolean));
	printf("MOVE LF@%s nil@nil\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_vypocet));
	printf("STRI2INT LF@%s LF@%s LF@%s\n", string_get(dst->k), string_get(string->k), string_get(tmp));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Funkce pro součet
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// | ADD/SUB_COM | dst | levy oper | pravy oper  |
//					^		^			^
//					|		|			|--ukazatel do tabluky na zdroj
//					|		| -- ukazatel do tabulky na zdroj
//					| --- ukazatel do tabulky na nové umístění
int gen_add_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_test7 = generate_label();
	string_t *label_float1 = generate_label();
	string_t *label_vypocet = generate_label();

	string_t *label_end = generate_label();
	string_t *label_concat = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(typeA == NULL || typeB == NULL || tmpA == NULL ||
	   tmpB == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_test7 == NULL || label_float1 == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_concat == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeB), string_get(b->k));

	printf("JUMPIFNEQ %s LF@%s string@string\n", string_get(label_test2), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_concat), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("JUMPIFNEQ %s LF@%s string@string\n", string_get(label_test3), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test3));
	printf("JUMPIFNEQ %s LF@%s string@nil\n", string_get(label_test4), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test4));
	printf("JUMPIFNEQ %s LF@%s string@nil\n", string_get(label_test5), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFNEQ %s LF@%s string@bool\n", string_get(label_test6), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test6));
	printf("JUMPIFNEQ %s LF@%s string@bool\n", string_get(label_test7), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test7));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));

	printf("JUMPIFNEQ %s LF@%s string@float\n", string_get(label_float1), string_get(typeA));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));

	printf("JUMP %s\n", string_get(label_vypocet));
	printf("LABEL %s\n", string_get(label_float1));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("ADD LF@%s LF@%s LF@%s\n", string_get(dst->k), string_get(tmpA), string_get(tmpB));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_concat));
	printf("CONCAT LF@%s LF@%s LF@%s\n", string_get(dst->k), string_get(a->k), string_get(b->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Funkce pro rozdíl
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
//  SUB_COM | dst | levy oper | pravy oper  |
//				^		^			^
//				|		|			|--ukazatel do tabluky na zdroj
//				|		| -- ukazatel do tabulky na zdroj
//				| --- ukazatel do tabulky na nové umístění
int gen_sub_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_test7 = generate_label();
	string_t *label_float1 = generate_label();
	string_t *label_vypocet = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(typeA == NULL || typeB == NULL || tmpA == NULL ||
	   tmpB == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_test7 == NULL || label_float1 == NULL || label_vypocet == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeB), string_get(b->k));

	printf("JUMPIFNEQ %s LF@%s string@string\n", string_get(label_test2), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("JUMPIFNEQ %s LF@%s string@string\n", string_get(label_test3), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test3));
	printf("JUMPIFNEQ %s LF@%s string@nil\n", string_get(label_test4), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test4));
	printf("JUMPIFNEQ %s LF@%s string@nil\n", string_get(label_test5), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFNEQ %s LF@%s string@bool\n", string_get(label_test6), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test6));
	printf("JUMPIFNEQ %s LF@%s string@bool\n", string_get(label_test7), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test7));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));

	printf("JUMPIFNEQ %s LF@%s string@float\n", string_get(label_float1), string_get(typeA));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));

	printf("JUMP %s\n", string_get(label_vypocet));
	printf("LABEL %s\n", string_get(label_float1));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("SUB LF@%s LF@%s LF@%s\n", string_get(dst->k), string_get(tmpA), string_get(tmpB));

	return ERR_OK;

}

/**
 * Funkce pro součin
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
//  MUL | dst | levy oper | pravy oper  |
//			^		^			^
//			|		|			|--ukazatel do tabluky na zdroj
//			|		| -- ukazatel do tabulky na zdroj
//			| --- ukazatel do tabulky na nové umístění
int gen_mul_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_test7 = generate_label();
	string_t *label_float1 = generate_label();
	string_t *label_vypocet = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(typeA == NULL || typeB == NULL || tmpA == NULL ||
	   tmpB == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_test7 == NULL || label_float1 == NULL || label_vypocet == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeB), string_get(b->k));

	printf("JUMPIFNEQ %s LF@%s string@string\n", string_get(label_test2), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("JUMPIFNEQ %s LF@%s string@string\n", string_get(label_test3), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test3));
	printf("JUMPIFNEQ %s LF@%s string@nil\n", string_get(label_test4), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test4));
	printf("JUMPIFNEQ %s LF@%s string@nil\n", string_get(label_test5), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFNEQ %s LF@%s string@bool\n", string_get(label_test6), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test6));
	printf("JUMPIFNEQ %s LF@%s string@bool\n", string_get(label_test7), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test7));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));

	printf("JUMPIFNEQ %s LF@%s string@float\n", string_get(label_float1), string_get(typeA));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));

	printf("JUMP %s\n", string_get(label_vypocet));
	printf("LABEL %s\n", string_get(label_float1));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("MUL LF@%s LF@%s LF@%s\n", string_get(dst->k), string_get(tmpA), string_get(tmpB));

	return ERR_OK;

}

/**
 * Funkce pro podíl
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// DIV_COM | dst | levy oper | pravy oper  |
//				^		^			^
//				|		|			|--ukazatel do tabluky na zdroj
//				|		| -- ukazatel do tabulky na zdroj
//				| --- ukazatel do tabulky na nové umístění
int gen_div_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_test7 = generate_label();
	string_t *label_float1 = generate_label();
	string_t *label_testB = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_test_zero = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(typeA == NULL || typeB == NULL || tmpA == NULL || label_test_zero == NULL ||
	   tmpB == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_test7 == NULL || label_float1 == NULL || label_vypocet == NULL || label_testB == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));TOK
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeB), string_get(b->k));

	printf("JUMPIFNEQ %s LF@%s string@string\n", string_get(label_test2), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("JUMPIFNEQ %s LF@%s string@string\n", string_get(label_test3), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test3));
	printf("JUMPIFNEQ %s LF@%s string@nil\n", string_get(label_test4), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test4));
	printf("JUMPIFNEQ %s LF@%s string@nil\n", string_get(label_test5), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFNEQ %s LF@%s string@bool\n", string_get(label_test6), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test6));
	printf("JUMPIFNEQ %s LF@%s string@bool\n", string_get(label_test7), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test7));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_testB), string_get(typeA));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));

	printf("LABEL %s\n", string_get(label_testB));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_test_zero), string_get(typeB));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("LABEL %s\n", string_get(label_test_zero));
	printf("JUMPIFNEQ %s LF@%s float@0x0.0p+0\n", string_get(label_vypocet), string_get(tmpB));
	printf("EXIT int@9\n");

	printf("LABEL %s\n", string_get(label_vypocet));
	printf("DIV LF@%s LF@%s LF@%s\n", string_get(dst->k), string_get(tmpA), string_get(tmpB));

	return ERR_OK;
}

/**
 * Funkce pro celočíselné dělení
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// IDIV_COM | dst | levy oper | pravy oper  |
//				^		^			^
//				|		|			|--ukazatel do tabluky na zdroj
//				|		| -- ukazatel do tabulky na zdroj
//				| --- ukazatel do tabulky na nové umístění
int gen_idiv_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_test7 = generate_label();
	string_t *label_float1 = generate_label();
	string_t *label_testB = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_test_zero = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(typeA == NULL || typeB == NULL || tmpA == NULL || label_test_zero == NULL ||
	   tmpB == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_test7 == NULL || label_float1 == NULL || label_vypocet == NULL || label_testB == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeB), string_get(b->k));

	printf("JUMPIFNEQ %s LF@%s string@string\n", string_get(label_test2), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("JUMPIFNEQ %s LF@%s string@string\n", string_get(label_test3), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test3));
	printf("JUMPIFNEQ %s LF@%s string@nil\n", string_get(label_test4), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test4));
	printf("JUMPIFNEQ %s LF@%s string@nil\n", string_get(label_test5), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFNEQ %s LF@%s string@bool\n", string_get(label_test6), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test6));
	printf("JUMPIFNEQ %s LF@%s string@bool\n", string_get(label_test7), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test7));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_testB), string_get(typeA));
	printf("FLOAT2INT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));

	printf("LABEL %s\n", string_get(label_testB));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_test_zero), string_get(typeB));
	printf("FLOAT2INT LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("LABEL %s\n", string_get(label_test_zero));
	printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_vypocet), string_get(tmpB));
	printf("EXIT int@9\n");

	printf("LABEL %s\n", string_get(label_vypocet));
	printf("IDIV LF@%s LF@%s LF@%s\n", string_get(dst->k), string_get(tmpA), string_get(tmpB));

	return ERR_OK;
}
/**
 * Následuje soubor funkcí pro porovnání
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// | MUL/DIV_COM | dst | levy oper | pravy oper  |
//					^		^			^
//					|		|			|--ukazatel do tabluky na zdroj
//					|		| -- ukazatel do tabulky na zdroj
//					| --- ukazatel do tabulky na nové umístění
// | CMP_GEQ/GRE/EQ/LES/LEQ/NEQ/ | dst | levy oper | pravy oper  |
//									^		^			^
//									|		|			|--ukazatel do tabluky na zdroj
//									|		| -- ukazatel do tabulky na zdroj
//									| --- ukazatel do tabulky na nové umístění
int gen_les_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s LF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("EXIT int@4\n");
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_none));
	//printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("LT LF@%s LF@%s LF@%s\n", string_get(boolean), string_get(tmpA), string_get(tmpB));

	printf("JUMPIFEQ %s LF@%s bool@false\n", string_get(label_false), string_get(boolean));
	printf("MOVE LF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE LF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Popis funkce viz výše
 */
// | CMP_GEQ/GRE/EQ/LES/LEQ/NEQ/ | dst | levy oper | pravy oper  |
//									^		^			^
//									|		|			|--ukazatel do tabluky na zdroj
//									|		| -- ukazatel do tabulky na zdroj
//									| --- ukazatel do tabulky na nové umístění
int gen_gr_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s LF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("EXIT int@4\n");
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_none));
	//printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("GT LF@%s LF@%s LF@%s\n", string_get(boolean), string_get(tmpA), string_get(tmpB));

	printf("JUMPIFEQ %s LF@%s bool@false\n", string_get(label_false), string_get(boolean));
	printf("MOVE LF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE LF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Popis funkce viz výše
 */
// | CMP_GEQ/GRE/EQ/LES/LEQ/NEQ/ | dst | levy oper | pravy oper  |
//									^		^			^
//									|		|			|--ukazatel do tabluky na zdroj
//									|		| -- ukazatel do tabulky na zdroj
//									| --- ukazatel do tabulky na nové umístění
int gen_leq_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean1 = generate_label();
	string_t *boolean2 = generate_label();
	string_t *boolean3 = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean1 == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL || boolean2 == NULL || boolean3 == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(boolean1));
		printf("DEFVAR LF@%s\n", string_get(boolean2));
		printf("DEFVAR LF@%s\n", string_get(boolean3));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(boolean1));
		printf("DEFVAR LF@%s\n", string_get(boolean2));
		printf("DEFVAR LF@%s\n", string_get(boolean3));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s LF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("EXIT int@4\n");
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_none));
	//printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("LT LF@%s LF@%s LF@%s\n", string_get(boolean1), string_get(tmpA), string_get(tmpB));
	printf("EQ LF@%s LF@%s LF@%s\n", string_get(boolean2), string_get(tmpA), string_get(tmpB));
	printf("OR LF@%s LF@%s LF@%s\n", string_get(boolean3), string_get(boolean1), string_get(boolean2));


	printf("JUMPIFEQ %s LF@%s bool@false\n", string_get(label_false), string_get(boolean3));
	printf("MOVE LF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE LF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Popis funkce viz výše
 */
// | CMP_GEQ/GRE/EQ/LES/LEQ/NEQ/ | dst | levy oper | pravy oper  |
//									^		^			^
//									|		|			|--ukazatel do tabluky na zdroj
//									|		| -- ukazatel do tabulky na zdroj
//									| --- ukazatel do tabulky na nové umístění
int gen_geq_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean1 = generate_label();
	string_t *boolean2 = generate_label();
	string_t *boolean3 = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean1 == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL || boolean2 == NULL || boolean3 == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(boolean1));
		printf("DEFVAR LF@%s\n", string_get(boolean2));
		printf("DEFVAR LF@%s\n", string_get(boolean3));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(boolean1));
		printf("DEFVAR LF@%s\n", string_get(boolean2));
		printf("DEFVAR LF@%s\n", string_get(boolean3));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s LF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("EXIT int@4\n");
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_none));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("GT LF@%s LF@%s LF@%s\n", string_get(boolean1), string_get(tmpA), string_get(tmpB));
	printf("EQ LF@%s LF@%s LF@%s\n", string_get(boolean2), string_get(tmpA), string_get(tmpB));
	printf("OR LF@%s LF@%s LF@%s\n", string_get(boolean3), string_get(boolean1), string_get(boolean2));


	printf("JUMPIFEQ %s LF@%s bool@false\n", string_get(label_false), string_get(boolean3));
	printf("MOVE LF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE LF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Popis funkce viz výše
 */
int gen_eq_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s LF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE LF@%s int@0\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("MOVE LF@%s int@0\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("MOVE LF@%s int@0\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE LF@%s int@0\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_none));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE LF@%s int@0\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("EQ LF@%s LF@%s LF@%s\n", string_get(boolean), string_get(tmpA), string_get(tmpB));

	printf("JUMPIFEQ %s LF@%s bool@false\n", string_get(label_false), string_get(boolean));
	printf("MOVE LF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE LF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Popis funkce viz výše
 */
int gen_neq_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);
	htab_item_t *b = (htab_item_t*)(instrukce->addr3);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *typeB = generate_label();
	string_t *tmpA = generate_label();
	string_t *tmpB = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_pretypovat = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_false = generate_label();
	string_t *label_end = generate_label();

	if(dst == NULL || a == NULL || b == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || typeB == NULL ||
	   tmpA == NULL || tmpB == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_pretypovat == NULL || label_vypocet == NULL ||
	   label_end == NULL || label_false == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(typeB));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(tmpB));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeB), string_get(b->k));

	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("MOVE LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));

	printf("JUMPIFEQ %s LF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE LF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_int));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_vypocet), string_get(typeB));
	printf("MOVE LF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_float));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_pretypovat), string_get(typeB));
	printf("MOVE LF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_pretypovat));
	printf("INT2FLOAT LF@%s LF@%s\n", string_get(tmpB), string_get(b->k));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_bool));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE LF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_none));
	printf("JUMPIFEQ %s LF@%s LF@%s\n", string_get(label_vypocet), string_get(typeA), string_get(typeB));
	printf("MOVE LF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_vypocet));

	printf("EQ LF@%s LF@%s LF@%s\n", string_get(boolean), string_get(tmpA), string_get(tmpB));

	printf("JUMPIFNEQ %s LF@%s bool@false\n", string_get(label_false), string_get(boolean));
	printf("MOVE LF@%s int@1\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_false));
	printf("MOVE LF@%s int@0\n", string_get(dst->k));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

/**
 * Funkce pro získání písmena z číselné hodnoty
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// CHR_CALL | DST | EXPR | --- |
//				^	 ^		
//				|	 |		
//				|	 |	--- ukazatel do tabulky na číselný parametr funkce chr	
//				| -- zkazatel do tabulky na L hodnotu	
int gen_chr_call_f(instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);
	htab_item_t *a = (htab_item_t*)(instrukce->addr2);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *tmpA = generate_label();

	string_t *label_string = generate_label();
	string_t *label_test1 = generate_label();
	string_t *label_test2 = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_pretypovani = generate_label();

	if(dst == NULL || a == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || tmpA == NULL || label_string == NULL ||
	   label_test1 == NULL || label_test2 == NULL || label_vypocet == NULL || label_pretypovani == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(tmpA));
		printf("DEFVAR LF@%s\n", string_get(typeA));
	}

	printf("MOVE LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(a->k));

	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_test1), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_pretypovani), string_get(typeA));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_pretypovani));
	printf("FLOAT2INT LF@%s LF@%s\n", string_get(tmpA), string_get(a->k));

	printf("LABEL %s\n", string_get(label_test1));
	printf("LT LF@%s LF@%s int@0\n", string_get(boolean), string_get(a->k));
	printf("JUMPIFNEQ %s LF@%s bool@true\n", string_get(label_test2), string_get(boolean));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("GT LF@%s LF@%s int@255\n", string_get(boolean), string_get(a->k));
	printf("JUMPIFNEQ %s LF@%s bool@true\n", string_get(label_vypocet), string_get(boolean));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_vypocet));
	printf("INT2CHAR LF@%s LF@%s\n", string_get(dst->k), string_get(tmpA));

	return ERR_OK;

}

/**
 * Funkce generující strukturu if-else
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// IF_COM | expr | else(false) | konec
//           ^			^			^
//			 |			|			|
//			 |			|			|-- ukazatel do pásky na poslední příkaz elsu (poslední if-else)
//			 |			|-- ukazatel do pásky na poslední příkaz if části před else
//			 |-- ukazatel do tabulky na hodnotu řídicího výrazu
int gen_if_call_f(instr_t *instrukce, instr_tape_t *main_tape, string_t *parrent)
{
	htab_item_t *exp = (htab_item_t*)(instrukce->addr1);
	instr_t *el = (instr_t*)(instrukce->addr2);
	instr_t *en = (instr_t*)(instrukce->addr3);

	string_t *boolean = generate_label();
	string_t *typeA = generate_label();

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_calc = generate_label();
	string_t *label_else = generate_label();
	string_t *label_end = generate_label();

	if(exp == NULL || el == NULL || en == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_else == NULL || label_calc == NULL ||
	   label_end == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(typeA));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(exp->k));

	printf("JUMPIFEQ %s LF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("MOVE LF@%s bool@false\n", string_get(boolean));
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_int));
	printf("EQ LF@%s LF@%s int@0\n", string_get(boolean), string_get(exp->k)); 
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_float));
	printf("EQ LF@%s LF@%s float@0x0.0p+0\n", string_get(boolean), string_get(exp->k)); 
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_bool));
	printf("MOVE LF@%s LF@%s\n", string_get(boolean), string_get(exp->k));
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_none));
	printf("MOVE LF@%s int@0\n", string_get(boolean));

	printf("LABEL %s\n", string_get(label_calc));
	printf("JUMPIFEQ %s LF@%s bool@true\n",string_get(label_else), string_get(boolean));

	while(instrukce != (instr_t*)el)
	{

		instrukce=tape_read(main_tape);	
	    make_statements(instrukce, main_tape, parrent);	

	    if(!tape_is_active(main_tape) || en->next == tape_actual(main_tape))
	    {
	    	break;
	    }
	}
	printf("JUMP %s\n", string_get(label_end));
	printf("LABEL %s\n", string_get(label_else));
	while(instrukce != (instr_t*)en)
	{

		instrukce=tape_read(main_tape);	
	    make_statements(instrukce, main_tape, parrent);	

	    if(!tape_is_active(main_tape) || en->next == tape_actual(main_tape))
	    {
	    	break;
	    }
	}
	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;
}

/**
 * Funkce generující strukturu while
 * -----------------------------------------
 * instrukce - ukazatel na právě vykonávanou instrukci v pásce
 * parrent - detekující zanožení příkazu
 * 
 * Funkce vrací v případě úspěchu 0, v opačném případě kúod chyby.
 */
// WHILE_COM | expr | begin | end
//				^		^		^
//				|		|		|
//				|		|		|-- ukazatel do pásky na poslední příkaz whilu
//				|		|-- ukazatel do pásky na první příkaz před tělem whilu
//				|-- ukazatel do tabulky na řídící výraz
int gen_while_call_f(instr_t *instrukce, instr_tape_t *main_tape, string_t *parrent)
{
	htab_item_t *expr = (htab_item_t*)(instrukce->addr1);
	instr_t *begin = (instr_t*)(instrukce->addr2);
	instr_t *end = (instr_t*)(instrukce->addr3);


	string_t *boolean = generate_label();
	string_t *typeA = generate_label();
	string_t *A = generate_label();
	string_t *counter = generate_label();TOK

	string_t *label_string = generate_label();
	string_t *label_int = generate_label();
	string_t *label_float = generate_label();
	string_t *label_bool = generate_label();
	string_t *label_none = generate_label();
	string_t *label_calc = generate_label();

	string_t *label_begin = generate_label();
	string_t *label_end = generate_label();

	if(expr == NULL || begin == NULL || end == NULL)
	{
		return ERR_INTER_RUN;
	}

	if(boolean == NULL || typeA == NULL || label_string == NULL ||
	   label_int == NULL || label_float == NULL || label_bool == NULL ||
	   label_none == NULL || label_begin == NULL || label_calc == NULL ||
	   label_end == NULL || A == NULL || counter == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}

	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(A));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(counter));
		printf("MOVE LF@%s int@0\n", string_get(counter));TOK
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(A));
		printf("DEFVAR LF@%s\n", string_get(typeA));
		printf("DEFVAR LF@%s\n", string_get(counter));
		printf("MOVE LF@%s int@0\n", string_get(counter));TOK
	}

	printf("LABEL %s\n", string_get(label_begin));
	while(instrukce != begin)
	{
		instrukce=tape_read(main_tape);	
	    make_statements(instrukce, main_tape, counter);

	    if(!tape_is_active(main_tape) || end->next == tape_actual(main_tape))
	    {
	    	break;
	    }
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeA), string_get(expr->k));

	printf("JUMPIFEQ %s LF@%s string@string\n", string_get(label_string), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_int), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_float), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@bool\n", string_get(label_bool), string_get(typeA));
	printf("JUMPIFEQ %s LF@%s string@nil\n", string_get(label_none), string_get(typeA));

	printf("LABEL %s\n", string_get(label_string));
	printf("MOVE LF@%s bool@false\n", string_get(boolean));
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_int));
	printf("EQ LF@%s LF@%s int@0\n", string_get(boolean), string_get(expr->k)); 
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_float));
	printf("EQ LF@%s LF@%s float@0x0.0p+0\n", string_get(boolean), string_get(expr->k)); 
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_bool));
	printf("MOVE LF@%s LF@%s\n", string_get(boolean), string_get(expr->k));
	printf("JUMP %s\n", string_get(label_calc));

	printf("LABEL %s\n", string_get(label_none));
	printf("MOVE LF@%s int@0\n", string_get(boolean));

	printf("LABEL %s\n", string_get(label_calc));
	printf("JUMPIFEQ %s LF@%s bool@true\n",string_get(label_end), string_get(boolean));	// true a false je naopak

	while(instrukce != end)
	{
		instrukce=tape_read(main_tape);	
	    make_statements(instrukce, main_tape, counter);	

	    if(!tape_is_active(main_tape) || end->next == tape_actual(main_tape))
	    {
	    	break;
	    }
	}

	printf("ADD LF@%s LF@%s int@1\n", string_get(counter), string_get(counter));TOK
	printf("JUMP %s\n", string_get(label_begin));
	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;	
}

/**
 * Funkce kontroluje parametry a spouští vestavěnou funkci substr
 * instrukce - aktuální vykonávaná instrukce
 * main_tape - instrukční páska
 * parrent - pro kontrolu zanoření příkazu, 
 * 
 * Funkce vrací 0 v případě úspěchu, jinak kód chyby.
 */
 // SUBSTR_CALL | DST | --- | --- |
//				  ^
//  			  |
//				  |--- ukazatel na do tabulky na levou stranu přiřazení
int gen_substr_call_f(instr_tape_t *main_tape, instr_t *instrukce, string_t *parrent)
{
	htab_item_t *dst = (htab_item_t*)(instrukce->addr1);

	if(dst == NULL)
	{
		return ERR_INTER_RUN;
	}

	instrukce = tape_read(main_tape);
	if(instrukce == NULL)
	{
		return ERR_INTER_RUN;
	}
	if(instrukce->instr_code != SUBSTR_PAR)
	{
		return ERR_INTER_RUN;
	}

	htab_item_t *s = (htab_item_t*)(instrukce->addr1);
	htab_item_t *i = (htab_item_t*)(instrukce->addr2);
	htab_item_t *n = (htab_item_t*)(instrukce->addr3);

	if(s == NULL || i == NULL || n == NULL)
	{
		return ERR_INTER_RUN;
	}

	string_t *typeS = generate_label();
	string_t *typeI = generate_label();
	string_t *typeN = generate_label();
	string_t *strLen = generate_label();
	string_t *tmp = generate_label();
	string_t *tmp2 = generate_label();
	string_t *boolean = generate_label();
	string_t *ch = generate_label();
	string_t *toend = generate_label();

	string_t *label_test2 = generate_label();
	string_t *label_test3 = generate_label();
	string_t *label_test4 = generate_label();
	string_t *label_test5 = generate_label();
	string_t *label_test6 = generate_label();
	string_t *label_vypocet = generate_label();
	string_t *label_pretyp1 = generate_label();
	string_t *label_pretyp2 = generate_label();
	string_t *label_end = generate_label();


	if(typeS == NULL || typeI == NULL || typeN == NULL || strLen == NULL ||
	   tmp == NULL || tmp2 == NULL || boolean == NULL || ch == NULL ||
	   toend == NULL || label_test2 == NULL || label_test3 == NULL ||
	   label_test4 == NULL || label_test5 == NULL || label_test6 == NULL ||
	   label_vypocet == NULL || label_pretyp1 == NULL || label_pretyp2 == NULL || 
	   label_end == NULL)
	{
		return ERR_INTER_RUN;
	}


	string_t *label_non_def = generate_label();
	if(label_non_def == NULL)
	{
		return ERR_INTER_RUN;
	}
	// Pokud se jedná o příkaz v zanoření a potenciálně opakonatelný,
	// pak se tímto koden zamezí znovudefinice
	if(parrent != NULL)
	{
		printf("JUMPIFNEQ %s LF@%s int@0\n", string_get(label_non_def), string_get(parrent));
		printf("DEFVAR GF%s\n", string_get(typeS));
		printf("DEFVAR GF%s\n", string_get(typeI));
		printf("DEFVAR GF%s\n", string_get(typeN));
		printf("DEFVAR GF%s\n", string_get(strLen));
		printf("DEFVAR GF%s\n", string_get(tmp));
		printf("DEFVAR GF%s\n", string_get(tmp2));
		printf("DEFVAR GF%s\n", string_get(boolean));
		printf("DEFVAR GF%s\n", string_get(ch));
		printf("DEFVAR GF%s\n", string_get(toend));
		printf("LABEL %s\n", string_get(label_non_def));

	}
	else
	{
		printf("DEFVAR LF@%s\n", string_get(typeS));
		printf("DEFVAR LF@%s\n", string_get(typeI));
		printf("DEFVAR LF@%s\n", string_get(typeN));
		printf("DEFVAR LF@%s\n", string_get(strLen));
		printf("DEFVAR LF@%s\n", string_get(tmp));
		printf("DEFVAR LF@%s\n", string_get(tmp2));
		printf("DEFVAR LF@%s\n", string_get(boolean));
		printf("DEFVAR LF@%s\n", string_get(ch));
		printf("DEFVAR LF@%s\n", string_get(toend));
	}

	printf("TYPE LF@%s LF@%s\n", string_get(typeS), string_get(s->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeI), string_get(i->k));
	printf("TYPE LF@%s LF@%s\n", string_get(typeN), string_get(n->k));

	printf("JUMPIFEQ %s LF@%s string@string\n", string_get(label_test2), string_get(typeS));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_test2));
	printf("STRLEN LF@%s LF@%s\n", string_get(strLen), string_get(s->k));
	
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_test3), string_get(typeI));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_pretyp1), string_get(typeI));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_pretyp1));
	printf("FLOAT2INT LF@%s LF@%s\n", string_get(tmp), string_get(i->k));
	printf("MOVE LF@%s LF@%s\n", string_get(i->k), string_get(tmp));

	printf("LABEL %s\n", string_get(label_test3));
	printf("GT LF@%s LF@%s int@-1\n", string_get(boolean), string_get(i->k));
	printf("JUMPIFEQ %s LF@%s bool@true\n", string_get(label_test4), string_get(boolean));
	printf("MOVE LF@%s nil@nil\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_test4));
	printf("LT LF@%s LF@%s LF@%s\n", string_get(boolean), string_get(i->k), string_get(strLen));
	printf("JUMPIFEQ %s LF@%s bool@true\n", string_get(label_test5), string_get(boolean));
	printf("MOVE LF@%s nil@nil\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_test5));
	printf("JUMPIFEQ %s LF@%s string@int\n", string_get(label_test6), string_get(typeN));
	printf("JUMPIFEQ %s LF@%s string@float\n", string_get(label_pretyp2), string_get(typeN));
	printf("EXIT int@4\n");

	printf("LABEL %s\n", string_get(label_pretyp2));
	printf("FLOAT2INT LF@%s LF@%s\n", string_get(tmp), string_get(n->k));
	printf("MOVE LF@%s LF@%s\n", string_get(n->k), string_get(tmp));

	printf("LABEL %s\n", string_get(label_test6));
	printf("MOVE LF@%s LF@%s\n", string_get(tmp), string_get(i->k));
	printf("MOVE LF@%s int@0\n", string_get(tmp2));
	printf("GETCHAR LF@%s LF@%s LF@%s\n", string_get(dst->k), string_get(s->k), string_get(tmp));
	printf("ADD LF@%s LF@%s int@1\n", string_get(tmp), string_get(tmp));
	printf("ADD LF@%s LF@%s int@1\n", string_get(tmp2), string_get(tmp2));
	printf("MOVE LF@%s LF@%s\n", string_get(toend), string_get(strLen));
	printf("SUB LF@%s LF@%s LF@%s\n", string_get(toend), string_get(toend), string_get(i->k));

	printf("GT LF@%s LF@%s int@0\n", string_get(boolean), string_get(n->k));
	printf("JUMPIFEQ %s LF@%s bool@true\n", string_get(label_vypocet), string_get(boolean));
	printf("MOVE LF@%s nil@nil\n", string_get(dst->k));
	printf("JUMP %s\n", string_get(label_end));

	printf("LABEL %s\n", string_get(label_vypocet));
	printf("LT LF@%s LF@%s LF@%s\n", string_get(boolean), string_get(tmp2), string_get(n->k));
	printf("JUMPIFEQ %s LF@%s bool@false\n", string_get(label_end), string_get(boolean));
	printf("LT LF@%s LF@%s LF@%s\n", string_get(boolean), string_get(tmp2), string_get(toend));
	printf("JUMPIFEQ %s LF@%s bool@false\n", string_get(label_end), string_get(boolean));

	printf("GETCHAR LF@%s LF@%s LF@%s\n", string_get(ch), string_get(s->k), string_get(tmp));
	printf("CONCAT LF@%s LF@%s LF@%s\n", string_get(dst->k), string_get(dst->k), string_get(ch));
	printf("ADD LF@%s LF@%s int@1\n", string_get(tmp), string_get(tmp));
	printf("ADD LF@%s LF@%s int@1\n", string_get(tmp2), string_get(tmp2));
	printf("JUMP %s\n", string_get(label_vypocet));

	printf("LABEL %s\n", string_get(label_end));

	return ERR_OK;

}

//  CALL | *fce | id_last | --- |
//			^		^
//			|		|
//			|		|
//			|		|--- ukazatel do pásky na poslední operaci s parametrem
//			|--- ukazatel do tabulky na funkci,(ne jeji tabulku)
int gen_call_call_f(instr_tape_t *main_tape, instr_t *instrukce, string_t *parrent)
{
	htab_item_t *fce = (htab_item_t*)(instrukce->addr1);
	instr_t *end = (instr_t*)(instrukce->addr2);
	htab_item_t *param1 = NULL;

	if(fce == NULL || end == NULL)
	{
		return ERR_INTER_RUN;
	}

	printf("CREATEFRAME\n");

	param_t *tmp = fce->it_data->fce->h_table->params;
	while(instrukce != end)
	{
		printf("DEFVAR TF@%s\n", string_get(tmp->ptr->k));
		

		string_t *strin = NULL;

		instrukce=tape_read(main_tape);	
		if(instrukce->instr_code == PARAM)
		{
			// PARAM | *id | --- | --- |
			//			^
			//			|
			//			|--- parametr je ukazatelem na prvek do tabulky
			param1 = (htab_item_t*)(instrukce->addr1);
			if(param1 == NULL)
			{
				return ERR_INTER_RUN;
			}

							// Pokud je src NULL, pak dojde k nahrání hodnoty nill
			if(param1->type == NONE)
			{
				printf("MOVE TF@%s nil@nil\n", string_get(tmp->ptr->k));
			}
			else
			{
				// Rozhodování o různých způsobech nahrávání
				if(param1->type == INT)				printf("MOVE TF@%s int@%d\n",string_get(tmp->ptr->k),param1->it_data->i);
				else if(param1->type == FLOAT)		printf("MOVE TF@%s float@%a\n",string_get(tmp->ptr->k),param1->it_data->fl);
				else if(param1->type == STRING || param1->type == COMMENT)
				{
					strin = string_to_hex(param1->it_data->str);
					if(strin == NULL)
					{
						return ERR_INTER_RUN;
					}
						printf("MOVE TF@%s string@%s\n",string_get(tmp->ptr->k),string_get(strin));
				}
				else if(param1->type == IDENTIFIER)	printf("MOVE TF@%s LF@%s\n",string_get(tmp->ptr->k),string_get(param1->k));
			}

		//printf("MOVE TF@%s GF@%s\n", string_get(tmp->ptr->k), string_get(param1->k));
		}
		else
		{
		    make_statements_f(instrukce, main_tape, NULL);	
		}

	    if(!tape_is_active(main_tape) || end->next == tape_actual(main_tape))
	    {
	    	break;
	    }

	    tmp = tmp->next;
	}

	printf("CALL %s\n", string_get(fce->k));
}


/** 
 * Funkce rozdělující příkazy na písce jednotlivým funkcím podle jejich kódu
 * -------------------------------------------------------------------------
 * instrukce - aktuální vykonávaná instrukce
 * main_tape - instrukční páska
 * parrent - pro kontrolu zanoření příkazu, 
 * 
 * Funkce vrací 0 v případě úspěchu, jinak kód chyby.
 */
int make_statements_f(instr_t *instrukce, instr_tape_t *main_tape, string_t *parrent)
{

	switch(instrukce->instr_code)
	{
		case(IF_COM):				gen_if_call_f(instrukce, main_tape, parrent);		break; //OK
		case(WHILE_COM):			gen_while_call_f(instrukce, main_tape, parrent);	break; //OK
		case(PRINT_CALL): 			gen_print_call_f(instrukce);	break;	// OK
		case(INPUTS_CALL):			gen_inputs_call_f(instrukce); break;	// OK
		case(INPUTI_CALL):			gen_inputi_call_f(instrukce); break;	// OK
		case(INPUTF_CALL): 			gen_inputf_call_f(instrukce); break;	// OK
		case(LEN_CALL):				gen_len_call_f(instrukce, parrent);	break;
		case(SUBSTR_CALL):			gen_substr_call_f(main_tape, instrukce, parrent); break;
		case(ORD_CALL): 			gen_ord_call_f(instrukce, parrent); 	break; // OK
		case(CHR_CALL):				gen_chr_call_f(instrukce, parrent);	break; // OK

		case(DOK_COM):				gen_com(instrukce, parrent); break;

		case(CPY_COM): 				gen_cpy_com_f(instrukce); 	break; // OK

		case(CMP_NEQ):				gen_neq_call_f(instrukce, parrent);	break; // OK
		case(CMP_EQ):				gen_eq_call_f(instrukce, parrent);		break; // OK
		case(CMP_LES):				gen_les_call_f(instrukce, parrent);	break; // OK
		case(CMP_LEQ):				gen_leq_call_f(instrukce, parrent);	break; // OK
		case(CMP_GRE):				gen_gr_call_f(instrukce, parrent);		break; // OK
		case(CMP_GEQ):				gen_geq_call_f(instrukce, parrent);	break; // OK

		case(MUL_COM):				gen_mul_call_f(instrukce, parrent);	break; // OK
		case(DIV_COM): 				gen_div_call_f(instrukce, parrent);	break; // OK
		case(DIV_INT_COM):			gen_idiv_call_f(instrukce, parrent);	break; // OK
		case(ADD_COM): 				gen_add_call_f(instrukce, parrent); 	break; // OK
		case(SUB_COM): 				gen_sub_call_f(instrukce, parrent);	break; // OK
	
		case(PARAM):				break;
		case(CALL):					gen_call_call_f(main_tape, instrukce, parrent); break;

		case(DEF_VAR): 				gen_def_var_f(instrukce, parrent); 	break; //OK
	}

	return ERR_OK;
}
