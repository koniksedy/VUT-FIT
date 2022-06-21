/**
 * Modul syntaktické analýzy
 * Napsal: Michal Šedý <xsedym02>
 * Přeloženo: gcc 7.4.0
 * Standard: C99
 * Poslední změna: 05.12.19
 */

#include "parse.h"

// Prototypy funkcí
/****************************************************************************************************************/
int prog(htab_t *global_table, instr_tape_t **main_tape, token_t *token);
int free_line(htab_t *global_table, instr_tape_t **main_tape, token_t *token);
int prog_n(htab_t *global_table, instr_tape_t **main_tape, token_t *token);
int stat_list(htab_t *global_table, instr_tape_t **main_tape, token_t *token);
int stat_list_n(htab_t *global_table, instr_tape_t **main_tape, token_t *token);
int stat(htab_t *global_table, instr_tape_t **main_tape, token_t *token);
int stat_const(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token);
int assg_op_fce(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token, string_t *id);
int operator(token_t *token);
int params(htab_t *global_table, instr_tape_t **main_tape, token_t *token, string_t *key, htab_t **func_def);
int param_n(htab_t *global_table, instr_tape_t **main_tape, token_t *token);
int params_print(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token);
int param_print_n(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token);
int stat_list_f(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token);
int stat_list_f_n(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token);
int stat_if_while(htab_t *global_table, instr_tape_t **main_tape, token_t *token);
int stat_list_if_while(htab_t *global_table, instr_tape_t **main_tape, token_t *token);
int stat_list_n_if_while(htab_t *global_table, instr_tape_t **main_tape, token_t *token);
int stat_f(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token);
int assign(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token, htab_item_t **result);
int expr_fce(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token, htab_item_t **result);
int params_call(htab_t *global_table, htab_t *func_table, string_t *func_name, instr_tape_t **main_tape, token_t *token);
int params_n_call(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token, unsigned short *param_count);
/*****************************************************************************************************************/

/**
 * Funkce pravidla <eol_eof>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int eol_eof(htab_t *global_table, instr_tape_t **main_tape, token_t *token)
{
	int ret_val = ERR_OK;
	string_t *name = NULL;

	if(CMP_TOKEN_CONST(token, EOL_t))
	{
		// ##09## <eol_eof> --> EOL
		GET_NEXT_TOKEN(token);
	}

	// ##10## <eol_eof> --> e

	return ret_val;
}

/**
 * Funkce pravidla <dedent_eof>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int dedent_eof(htab_t *global_table, instr_tape_t **main_tape, token_t *token)
{
	
	int ret_val= ERR_OK; 
	string_t *name = NULL;

	if(!CMP_TOKEN_CONST(token, EOF_t))
	{
		// ##17## <dedent_eof> --> DEDENT
		
		CHACK_TOKEN_AND_GET_NEXT(token, DEDENT, "<dedent_eof>");
	}
	

	// ##18## <dedent_eof> --> e

	return ret_val;
}

/**
 * Funkce pravidla <prog>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int prog(htab_t *global_table, instr_tape_t **main_tape, token_t *token)
{
	int ret_val = ERR_OK;
	string_t *name = NULL;	// pro usnadnění dealokace

	// ##01## <prog> --> <free-line><prog-N>							
	ret_val = free_line(global_table, main_tape, token);
	CHACK_RET(ret_val, name);
	ret_val = prog_n(global_table, main_tape, token);

	return ret_val;
}

/**
 * Funkce pravidla <free_line>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int free_line(htab_t *global_table, instr_tape_t **main_tape, token_t *token)
{
	int ret_val = ERR_OK;

	if(CMP_TOKEN_CONST(token, EOL_t))
	{
		// ##02## <free-line> --> EOL <free-line>												
		GET_NEXT_TOKEN(token);
		ret_val = free_line(global_table, main_tape, token);
	}

		// ##03## <free-line> --> e 																

	return ret_val;
}

/**
 * Funkce pravidla <prog_n>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int prog_n(htab_t *global_table, instr_tape_t **main_tape, token_t *token)
{
	int ret_val = ERR_OK;
	string_t *name = NULL;	// pro usnadnění dealokace

	if(!CMP_TOKEN_CONST(token, EOF_t))
	{
		// ##04## <prog-N> --> <stat><eol_eof><free-line><stat-list>									
		ret_val = stat(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		eol_eof(global_table, main_tape, token);

		ret_val = free_line(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		ret_val = stat_list_n(global_table, main_tape, token);
	}

		// ##05## <prog-N> --> e 																	

	return ret_val;
}

/**
 * Funkce pravidla <stat_list>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int stat_list(htab_t *global_table, instr_tape_t **main_tape, token_t *token)
{
	int ret_val = ERR_OK;
	string_t *name = NULL;	// pro usnadnění dealokace
	
	// ##06## <stat-list> --> <free-line><stat><eol_eof><free-line><stat-list-N>						
	ret_val = free_line(global_table, main_tape, token);
	CHACK_RET(ret_val, name);

	ret_val = stat(global_table, main_tape, token);
	CHACK_RET(ret_val, name);
	eol_eof(global_table, main_tape, token);

	ret_val = free_line(global_table, main_tape, token);
	CHACK_RET(ret_val, name);

	ret_val = stat_list_n(global_table, main_tape, token);

	return ret_val;
}

/**
 * Funkce pravidla <stat-list-N>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int stat_list_n(htab_t *global_table, instr_tape_t **main_tape, token_t *token)
{
	int ret_val = ERR_OK;
	string_t *name = NULL;	// pro usnadnění dealokace

	if(!CMP_TOKEN_CONST(token, EOF_t) && !CMP_TOKEN_CONST(token, DEDENT))
	{
		// ##07## <stat-list-N> --> <free-line><stat><eol_eof><free-line><stat-list-N>					
		ret_val = free_line(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		ret_val = stat(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		eol_eof(global_table, main_tape, token);

		ret_val = free_line(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		ret_val = stat_list_n(global_table, main_tape, token);
	}

	// ##08## <stat-list-N> --> e 																

	return ret_val;
}

/**
 * Funkce pravidla <stat_list_if_while>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int stat_list_if_while(htab_t *global_table, instr_tape_t **main_tape, token_t *token)
{
	int ret_val = ERR_OK;
	string_t *name = NULL;	// pro usnadnění dealokace
	
	// ##11## <stat-list> --> <free-line><stat_if_while><eol_eof><free-line><stat-list-N>
	ret_val = free_line(global_table, main_tape, token);
	CHACK_RET(ret_val, name);

	ret_val = stat_if_while(global_table, main_tape, token);
	CHACK_RET(ret_val, name);

	eol_eof(global_table, main_tape, token);
	
	ret_val = free_line(global_table, main_tape, token);
	CHACK_RET(ret_val, name);

	ret_val = stat_list_n_if_while(global_table, main_tape, token);

	return ret_val;
}

/**
 * Funkce pravidla <stat_list_n_if_while>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int stat_list_n_if_while(htab_t *global_table, instr_tape_t **main_tape, token_t *token)
{
	int ret_val = ERR_OK;
	string_t *name = NULL;	// pro usnadnění dealokace

	if(!CMP_TOKEN_CONST(token, EOF_t) && !CMP_TOKEN_CONST(token, DEDENT))
	{
		// ##12## <stat_list_n_if_while> --> <free-line><stat_if_while><free-line><stat_list_n_if_while>					
		ret_val = free_line(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		ret_val = stat_if_while(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		eol_eof(global_table, main_tape, token);
		
		ret_val = free_line(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		ret_val = stat_list_n_if_while(global_table, main_tape, token);
	}

	// ##13## <stat-stat_list_n_if_while> --> e 																	

	return ret_val;
}

/**
 * Funkce pravidla <stat_if_while>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int stat_if_while(htab_t *global_table, instr_tape_t **main_tape, token_t *token)
{
int ret_val = ERR_OK;

	instr_t *instr_holder = NULL;

	void *addr1 = NULL;
	void *addr2 = NULL;
	void *addr3 = NULL;

	string_t *name = string_ctor();	// TODO - proč alokuji ?
	if(name  == NULL)
	{
		error_msg("CHYBA při alokaci name <stat>\n%s", MSG_INTER_RUN);
		return ERR_INTER_RUN;
	}


	if(CMP_TOKEN_CONST(token, IF))
	{
		// ##14## <stat_if_while> --> if<expr>:EOL INDENT<stat_list_if_while>DEDENT else: EOL INDENT <stat_list_if_while><dedent_eof> 
		// IF_COM | expr | else(false) | konec
		//           ^			^			^
		//			 |			|			|
		//			 |			|			|-- ukazatel do pásky na poslední příkaz elsu (poslední if-else)
		//			 |			|-- ukazatel do pásky na poslední příkaz if části před else
		//			 |-- ukazatel do tabulky na hodnotu řídicího výrazu

											
		GET_NEXT_TOKEN(token);
		ret_val = expr(global_table, NULL, main_tape, token, (htab_item_t**)(&addr1), NULL, false);	//addr1

		//PLACE_HOLDER
		MAKE_INSTR(main_tape, PLACE_HOLDER, NULL, NULL, NULL);					
		instr_holder = tape_last(*main_tape);

		CHACK_RET(ret_val, name);
		CHACK_TOKEN_AND_GET_NEXT(token, COLON, "<stat>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, EOL_t, "<stat>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, INDENT, "<stat>-->if");

		ret_val = stat_list_if_while(global_table, main_tape, token);
		CHACK_RET(ret_val, name);
		CHACK_TOKEN_AND_GET_NEXT(token, DEDENT, "<stat>-->if");
		addr2 = (void*)tape_last(*main_tape);								//addr2
		CHACK_TOKEN_AND_GET_NEXT(token, ELSE, "<stat>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, COLON, "<stat>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, EOL_t, "<stat>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, INDENT, "<stat>-->if");

		ret_val = stat_list_if_while(global_table, main_tape, token);
		CHACK_RET(ret_val, name);
		addr3 = (void*)tape_last(*main_tape);								//addr3

		dedent_eof(global_table, main_tape, token);

		// výměna PLACE_HOLDER
		if(instr_change(instr_holder, IF_COM, addr1, addr2, addr3) == false)
		{
			error_msg("Chyba pri place holder if.\n%s", MSG_INTER_RUN);
			
			return ERR_INTER_RUN;
		}
	}
	else if(CMP_TOKEN_CONST(token, WHILE))
	{
		// ##15## <stat_if_while> --> while<expr>: EOL INDENT <stat_list_if_while><dedent_eof>									
		// WHILE_COM | expr | begin | end
		//				^		^		^
		//				|		|		|
		//				|		|		|-- ukazatel do pásky na poslední příkaz whilu
		//				|		|-- ukazatel do pásky na první příkaz před tělěm whilu
		//				|-- ukazatel do pásky na poslední isnstrukci podmínky

		//PLACE_HOLDER
		MAKE_INSTR(main_tape, PLACE_HOLDER, NULL, NULL, NULL);					
		instr_holder = tape_last(*main_tape);									

		
		GET_NEXT_TOKEN(token);

		ret_val = expr(global_table, NULL, main_tape, token, (htab_item_t**)(&addr1), NULL, false);	
		CHACK_RET(ret_val, name);

		addr2 = (void*)tape_last(*main_tape);						//addr2

		CHACK_TOKEN_AND_GET_NEXT(token, COLON, "<stat>-->while");
		CHACK_TOKEN_AND_GET_NEXT(token, EOL_t, "<stat>-->while");
		CHACK_TOKEN_AND_GET_NEXT(token, INDENT, "<stat>-->while");

		ret_val = stat_list_if_while(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		dedent_eof(global_table, main_tape, token);
		addr3 = (void*)tape_last(*main_tape);						//addr3

		// Výměna place_hoder
		if(!instr_change(instr_holder, WHILE_COM, addr1, addr2, addr3))
		{
			error_msg("Chyba pri place holder while.\n%s", MSG_INTER_RUN);
			
			return ERR_INTER_RUN;
		}
	}
	else
	{	
		// ##16## <stat_if_while> --> <stat-const>
		ret_val = stat_const(global_table, NULL, main_tape, token);
	}

	
	return ret_val;
}

/**
 * Funkce pravidla <stat>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int stat(htab_t *global_table, instr_tape_t **main_tape, token_t *token)
{
	int ret_val = ERR_OK;

	// Pomocné proměnné
	instr_t *instr_holder = NULL;
	void *addr1 = NULL;
	void *addr2 = NULL;
	void *addr3 = NULL;
	htab_item_t *result = NULL;

	string_t *name = string_ctor();		//  TODO - proč alokuji ?
	if(name  == NULL)
	{
		error_msg("CHYBA při alokaci name <stat>\n%s", MSG_INTER_RUN);
		return ERR_INTER_RUN;
	}


	if(CMP_TOKEN_CONST(token, IF))
	{
		// ##19## <stat> --> if<expr>:EOL INDENT<stat_list_if_while>DEDENT else: EOL INDENT <stat_list_if_while><dedent_eof> 	
		// IF_COM | expr | else(false) | konec
		//           ^			^			^
		//			 |			|			|
		//			 |			|			|-- ukazatel do pásky na poslední příkaz elsu (poslední if-else)
		//			 |			|-- ukazatel do pásky na poslední příkaz if části před else
		//			 |-- ukazatel do tabulky na hodnotu řídicího výrazu

											
		GET_NEXT_TOKEN(token);
		ret_val = expr(global_table, NULL, main_tape, token, (htab_item_t**)(&addr1), NULL, false);	//addr1

		// PLACE_HOLDER
		MAKE_INSTR(main_tape, PLACE_HOLDER, NULL, NULL, NULL);					
		instr_holder = tape_last(*main_tape);

		CHACK_RET(ret_val, name);
		CHACK_TOKEN_AND_GET_NEXT(token, COLON, "<stat>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, EOL_t, "<stat>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, INDENT, "<stat>-->if");

		ret_val = stat_list_if_while(global_table, main_tape, token);
		CHACK_RET(ret_val, name);
		CHACK_TOKEN_AND_GET_NEXT(token, DEDENT, "<stat>-->if");
		addr2 = (void*)tape_last(*main_tape);							//addr2

		CHACK_TOKEN_AND_GET_NEXT(token, ELSE, "<stat>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, COLON, "<stat>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, EOL_t, "<stat>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, INDENT, "<stat>-->if");

		ret_val = stat_list_if_while(global_table, main_tape, token);
		CHACK_RET(ret_val, name);
		addr3 = (void*)tape_last(*main_tape);							//addr3

		dedent_eof(global_table, main_tape, token);

		// Výměna PLACE_HOLDER
		if(instr_change(instr_holder, IF_COM, addr1, addr2, addr3) == false)
		{
			error_msg("Chyba pri place holder if.\n%s", MSG_INTER_RUN);
			
			return ERR_INTER_RUN;
		}
	}
	else if(CMP_TOKEN_CONST(token, WHILE))
	{
		// ##20## <stat> --> while<expr>: EOL INDENT <stat_list_if_while><dedent_eof>							
		// WHILE_COM | expr | begin | end
		//				^		^		^
		//				|		|		|
		//				|		|		|-- ukazatel do pásky na poslední příkaz whilu
		//				|		|-- ukazatel do pásky na první příkaz před tělěm whileu 
		//				|-- ukazatel do globální tabulky na řídicí výraz, bude se buset přepočítávat

		//PLACE_HOLDER
		MAKE_INSTR(main_tape, PLACE_HOLDER, NULL, NULL, NULL);					
		instr_holder = tape_last(*main_tape);									


		GET_NEXT_TOKEN(token);

		ret_val = expr(global_table, NULL, main_tape, token, (htab_item_t**)(&addr1), NULL, false);	//addr1
		CHACK_RET(ret_val, name);

		addr2 = (void*)tape_last(*main_tape);							//addr2
		CHACK_TOKEN_AND_GET_NEXT(token, COLON, "<stat>-->while");
		CHACK_TOKEN_AND_GET_NEXT(token, EOL_t, "<stat>-->while");
		CHACK_TOKEN_AND_GET_NEXT(token, INDENT, "<stat>-->while");

		ret_val = stat_list_if_while(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		dedent_eof(global_table, main_tape, token);
		addr3 = (void*)tape_last(*main_tape);							//addr3

		// Výměna PLACE_HOLDER
		if(!instr_change(instr_holder, WHILE_COM, addr1, addr2, addr3))
		{
			error_msg("Chyba pri place holder while.\n%s", MSG_INTER_RUN);
			
			return ERR_INTER_RUN;
		}
	}
	else if(CMP_TOKEN_CONST(token, DEF))
	{
		// ##21## <stat> --> def id(<params>): EOL INDENT <stat-list-F><dedent_eof> 						
		// jméno se uloží do tabulky
		// DEF_COM | params | end | ----- |
		//				^		^
		//				|		|
		//				|		|-- ukazatel do pásky na poslední příkaz v definici
		//				|-- ukazatel do tabulky funkci

		//PLACE_HOLDER
		MAKE_INSTR(main_tape, PLACE_HOLDER, NULL, NULL, NULL);					
		instr_holder = tape_last(*main_tape);										

		GET_NEXT_TOKEN(token);
		if(!CMP_TOKEN_CONST(token, IDENTIFIER)) 
		{
			error_msg("Pravidlo <stat>->def \n%s", MSG_SYN);
			
			return ERR_SYN;
		}

		// Získání stringu identifikátorů
		if(!string_cpy(name, token->data->str))
		{
			error_msg("CHYBA při kopírování jména identifikátorů\n%s", MSG_SYN);
			
			return ERR_INTER_RUN;
		}

		GET_NEXT_TOKEN(token);
		CHACK_TOKEN_AND_GET_NEXT(token, LEFT_PAR, "<stat> -->def");

		
		ret_val =  params(global_table, main_tape, token, name, (htab_t**)(&addr1));		//addr1
		CHACK_RET(ret_val, name);
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<stat> -->def");
		CHACK_TOKEN_AND_GET_NEXT(token, COLON, "<stat> -->def");
		CHACK_TOKEN_AND_GET_NEXT(token, EOL_t, "<stat> -->def");
		CHACK_TOKEN_AND_GET_NEXT(token, INDENT, "<stat> -->def");

		addr1 = (void*)tape_last(*main_tape);

		ret_val = stat_list_f(global_table, name, main_tape, token);	
		CHACK_RET(ret_val, name);

		dedent_eof(global_table, main_tape, token);
		addr2 = (void*)tape_last(*main_tape);							//addr2

		// výměna place_hoder
		if(!instr_change(instr_holder, DEF_COM, addr1, addr2, (void*)name))
		{
			error_msg("Chyba pri place holder def.\n%s", MSG_INTER_RUN);
			
			return ERR_INTER_RUN;
		}
	}
	else
	{	
		// ##22## <stat> --> <stat-const>
		ret_val = stat_const(global_table, NULL, main_tape, token);
	}


	return ret_val;
}

/**
 * Funkce pravidla <stat-const>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int stat_const(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token)
{
	// Pomocné proměnné
	int ret_val = ERR_OK;
	param_t *params_def = NULL;
	param_t *params_set = NULL;

	// Alokace stringu pro budoucí použití a kontrola
	string_t *name = string_ctor();	
	if(name  == NULL)
	{
		error_msg("CHYBA při alokaci name <stat>\n%s", MSG_INTER_RUN);
		return ERR_INTER_RUN;
	}

	if(CMP_TOKEN_CONST(token, PASS))
	{
		// ##23## <stat_const> --> PASS 																
		GET_NEXT_TOKEN(token);
	}
	else if(CMP_TOKEN_CONST(token, LEFT_PAR))
	{
		// ##24## <stat_const> --> (<expr>)								

		// Pouhá kontrola, jinak mrtvý kód
		ret_val = expr(global_table, func_name, NULL, token, NULL, NULL, true);
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<stat> -->(");
	}
	else if(CMP_TOKEN_CONST(token, IDENTIFIER))
	{
		// ##25## <stat_const>--> id <assg-op-fce>														
		
		// získání identifikátorů pro assg_og_fce
		if(!string_cpy(name, token->data->str))
		{
			error_msg("CHYBA při kopírování jména identifikátorů\n%s", MSG_SYN);
			
			return ERR_INTER_RUN;
		}

		GET_NEXT_TOKEN(token);

		ret_val = assg_op_fce(global_table, func_name, main_tape, token, name);		
	}
	else if(CMP_TOKEN_CONST(token, COMMENT))
	{
		// ##26## <stat_const> --> STR-DOKUMENT													
		// DOK_COM |  str | ---- | ----|
		//				^	
		//				|
		//				|-- ukazatel do tabulky na dokumentační řetězec

		// Projistotu vymázání a zvytvoření jména pomocné proměnné
		
		name = string_rand();
		if(name == NULL)
		{
			error_msg("CHYBA při alokaci name <stat>\n%s", MSG_INTER_RUN);
			return ERR_INTER_RUN;
		}

		// Vloření řetězce do tabulky
		if(!htab_insert(global_table, name, token))
		{
			error_msg("CHYBA při vkládání do tabulky\n%s", MSG_SYN);
			
			return ERR_INTER_RUN;
		}

		MAKE_INSTR(main_tape, DOK_COM, (void*)htab_get_item(global_table, name), NULL, NULL);

		GET_NEXT_TOKEN(token);
	}
	else if(CMP_TOKEN_CONST(token, INPUTS))
	{
		// ##27## <stat_const> --> INPUTS() -- mrtvý kód											

		ret_val = check_type(global_table, func_name, token);		
	}
	else if(CMP_TOKEN_CONST(token, INPUTI))
	{
		// ##28## <stat_const> --> INPUTI() -- mrtvý kód												

		ret_val = check_type(global_table, func_name, token);		
	}
	else if(CMP_TOKEN_CONST(token, INPUTF))
	{
		// ##29## <stat_const> --> INPUTF()  -- mrtvy kod												

		ret_val = check_type(global_table, func_name, token);	
	}
	else if(CMP_TOKEN_CONST(token, PRINT))
	{
		// ##30## <stat_const> --> PRINT(<params-print>)	

		GET_NEXT_TOKEN(token);
		CHACK_TOKEN_AND_GET_NEXT(token, LEFT_PAR, "<stat> -->PRINT");

		ret_val = params_print(global_table, func_name, main_tape, token);
		CHACK_RET(ret_val, name);
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<stat> -->PRINT");
	}
	else if(CMP_TOKEN_CONST(token, LEN))
	{
		// ##31## <stat_const> --> LEN(STR)  -- mrtvy kod													

		ret_val = check_type(global_table, func_name, token);		
	}
	else if(CMP_TOKEN_CONST(token, SUBSTR))
	{
		// ##32## <stat_const> --> SUBSTR(STR, NUM, NUM)  -- mrtvy kod						

		ret_val = check_type(global_table, func_name, token);		
	}
	else if(CMP_TOKEN_CONST(token, ORD))
	{
		// ##33## <stat_const> --> ORD(STR, NUM)  -- mrtvy kod											

		ret_val = check_type(global_table, func_name, token);		
	}
	else if(CMP_TOKEN_CONST(token, CHR))
	{
		// ##34## <stat_const> --> CHR(NUM)  -- mrtvy kod										

		ret_val = check_type(global_table, func_name, token);		
	}
	else if(CMP_TOKEN_CONST(token, STRING))
	{
		// ##35## <stat_const> --> STRING<expr2>   -- mrtvy kod												

		ret_val = expr(global_table, func_name, NULL, token, NULL, NULL, true);		
	}
	else if(CMP_TOKEN_CONST(token, NONE))
	{
		// ##36## <stat_const> --> NONE<expr2>   -- mrtvy kod												

		ret_val = expr(global_table, func_name, NULL, token, NULL, NULL, true);	
	}
	else if(CMP_TOKEN_CONST(token, INT))
	{
		// ##37## <stat_const> --> INT<expr2>   -- mrtvy kod														

		ret_val = expr(global_table, func_name, NULL, token, NULL, NULL, true);	
	}
	else if(CMP_TOKEN_CONST(token, FLOAT))
	{
		// ##38## <stat_const> --> FLOAT<expr2>  -- mrtvy kod													

		ret_val = expr(global_table, func_name, NULL, token, NULL, NULL, true);		
	}
	else
	{
		// Token nikam nepatři >> syntaktická chyba
		error_msg("CHYBA: <stat_const>\n%s", MSG_SYN);
		ret_val = ERR_SYN;
	}

	
	return ret_val;
}

// <assg-op-fce>
int assg_op_fce(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token, string_t *id)
{
	// TODO zkontrolovat výpočty hodnot
	int ret_val = ERR_OK;
	htab_item_t *result = NULL;
	htab_item_t *tmp = NULL;
	string_t *name = NULL; 		// pro lepří dealokaci
	instr_t *instr_holder = NULL;
	void *addr1 = NULL;
	void *addr2 = NULL;
	void *addr3 = NULL;
	token_t *touken = NULL;


	if(CMP_TOKEN_CONST(token, ASSIGN))
	{
		// ##28## <assg-op-fce> --> = <assign>														

		if(((tmp = htab_get_item(global_table, id)) != NULL) && tmp->type == FUNCTION)
		{
			// kontrola, zda se nepokoušíme o vytvoření proměné stejného jména jako funkce.
			return ERR_SEM_DEF;
		}

		if((result = htab_get_item(global_table, id)) == NULL){
			if((touken = token_ctor()) == NULL) return ERR_INTER_RUN;
			touken->type = IDENTIFIER;
			if((touken->data = malloc(sizeof(token_data_t))) == NULL) return ERR_INTER_RUN;
			touken->data->str=id; //tady

			htab_insert(global_table, id, touken);
			if((result = htab_get_item(global_table, id)) == NULL) return ERR_INTER_RUN;
		} //return ERR_INTER_RUN;
		
		GET_NEXT_TOKEN(token);
		ret_val = assign(global_table, func_name, main_tape, token, &result);	

	}
	else if(CMP_TOKEN_CONST(token, LEFT_PAR))
	{
		// ##29## <assg-op-fce> --> (<params-call>)													
		//  CALL | *fce | id_last | --- |
		//			^		^
		//			|		|
		//			|		|
		//			|		|--- ukazatel do pásky na poslední operaci s parametrem
		//			|--- ukazatel do tabulky na funkci,(ne jeji tabulku)
		MAKE_INSTR(main_tape, PLACE_HOLDER, NULL, NULL, NULL);					
		instr_holder = tape_last(*main_tape);										//PLACE_HOLDER
		if((addr1 = (void*)htab_get_item(global_table, id)) == NULL) return ERR_SEM_DEF;	//addr1

		GET_NEXT_TOKEN(token);
		tmp = (htab_item_t*)addr1;
		ret_val = params_call(global_table, tmp->it_data->fce->h_table, func_name, main_tape, token);	//TODO
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<assg_op_fce> -->call");

		addr2 = tape_last(*main_tape);		// addr2

		// výměna place_hoder
		if(!instr_change(instr_holder, CALL, addr1, addr2, addr3))
		{
			error_msg("Chyba pri place holder while.\n%s", MSG_INTER_RUN);
			
			return ERR_INTER_RUN;
		}
	}
	else if(IS_OPERATOR(token))
	{
		// ##30## <assg-op-fce> --> <operator> <expr>											
		// mrtvý kod
		ret_val = operator(token);
		CHACK_RET(ret_val, name);
		ret_val = expr(global_table, func_name, NULL, token, NULL, NULL, true);		// TODO
	}
	else if(CMP_TOKEN_CONST(token, EOL_t) || CMP_TOKEN_CONST(token, EOF_t))
	{	//GET_NEXT_TOKEN(token);
		
		ret_val = ERR_OK;
	}
	else
	{
		error_msg("CHYBA při assg_op_fce\n");
		ret_val = ERR_SYN;
	}

	return ret_val;
}

/**
 * Funkce pravidla <assign>
 * -------------------------
 * global_table - globální tabulka
 * func_name - jméno funkce, v jejimž těle se nachází výpočet,
 *			 - pokud je výpočet v globální části je nastaveno na NULL
 * main_tape - instrukční páska
 * token - aktuální načtený token
 * result - ukazatel do tabulky na výsledek případného přiřazení
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int assign(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token, htab_item_t **result)
{

	int ret_val = ERR_OK;
	string_t *name = NULL; 		// pro lepří dealokaci
	htab_item_t *par1 = NULL;
	htab_item_t *par2 = NULL;
	htab_item_t *par3 = NULL;


	if(CMP_TOKEN_CONST(token, IDENTIFIER)) 
	{
		// ##39## <assing> --> = identifier <expr_fce>	
		//do tabulky na položku, která se bude definovat		

		// Pokud nebyla proměnná levé strany jeěšte definována, učiní se tak
		// DEF_COM | *name | ---- | ----- |
		//				^
		//				|
		//				|--- ukazatel do tabulky na jméno proměnné, která se bude definovat.
		if((*result)->defined == false) MAKE_INSTR(main_tape, DEF_VAR, (void*)(*result), NULL, NULL);
		ret_val = expr_fce(global_table, func_name, main_tape, token, result);
		(*result)->defined = true;
	}
	else if(CMP_TOKEN_CONST(token, INT) || CMP_TOKEN_CONST(token, FLOAT) || CMP_TOKEN_CONST(token, STRING) ||
		    CMP_TOKEN_CONST(token, COMMENT) || CMP_TOKEN_CONST(token, LEFT_PAR) || CMP_TOKEN_CONST(token, NONE))
	{
		// ##40##  <assign> --> <expr>

		// Pokud nebyla proměnná levé strany jeěšte definována, učiní se tak
		// DEF_COM | *name | ---- | ----- |
		//				^
		//				|
		//				|--- ukazatel do tabulky na jméno proměnné, která se bude definovat.

		// Pokud nebyla proměnná levé strany jeěšte definována, učiní se tak
		if((*result)->defined == false) MAKE_INSTR(main_tape, DEF_VAR, (void*)(*result), NULL, NULL);
		ret_val = expr(global_table, func_name, main_tape, token, result, NULL, false);
		(*result)->defined = true;
	}
	else if(CMP_TOKEN_CONST(token, INPUTI))
	{
		// ##41## <assing> --> INPUTI()	
		// INPUTI_CALL | DST | --- | --- |
		//				  ^
		//  			  |
		//				  |--- ukazatel na do tabulky na levou stranu přiřazení

		// Pokud nebyla proměnná levé strany jeěšte definována, učiní se tak
		if((*result)->defined == false) MAKE_INSTR(main_tape, DEF_VAR, (void*)(*result), NULL, NULL);
		GET_NEXT_TOKEN(token);
		CHACK_TOKEN_AND_GET_NEXT(token, LEFT_PAR, "<assg> -->INPUTI");
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<assg> -->INPUTI");
		MAKE_INSTR(main_tape, INPUTI_CALL, (void*)(*result), NULL, NULL);
		(*result)->defined = true;
	}
	else if(CMP_TOKEN_CONST(token, INPUTF))
	{
		// ##42## <assing> --> INPUTF()
		// INPUTF_CALL | DST | --- | --- |
		//				  ^
		//  			  |
		//				  |--- ukazatel na do tabulky na levou stranu přiřazení

		// Pokud nebyla proměnná levé strany jeěšte definována, učiní se tak
		if((*result)->defined == false) MAKE_INSTR(main_tape, DEF_VAR, (void*)(*result), NULL, NULL);
		GET_NEXT_TOKEN(token);
		CHACK_TOKEN_AND_GET_NEXT(token, LEFT_PAR, "<assg> -->INPUTF");
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<assg> -->INPUTF");
		MAKE_INSTR(main_tape, INPUTF_CALL, (void*)(*result), NULL, NULL);
		(*result)->defined = true;
	}
	else if(CMP_TOKEN_CONST(token, INPUTS))
	{
		// ##43## <assing> --> INPUTS()
		// INPUTS_CALL | DST | --- | --- |
		//				  ^
		//  			  |
		//				  |--- ukazatel na do tabulky na levou stranu přiřazení

		// Pokud nebyla proměnná levé strany jeěšte definována, učiní se tak
		if((*result)->defined == false) MAKE_INSTR(main_tape, DEF_VAR, (void*)(*result), NULL, NULL);
		GET_NEXT_TOKEN(token);
		CHACK_TOKEN_AND_GET_NEXT(token, LEFT_PAR, "<assg> -->INPUTS");
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<assg> -->INPUTS");
		MAKE_INSTR(main_tape, INPUTS_CALL,(void*)(*result), NULL, NULL);
		(*result)->defined = true;
	}
	else if(CMP_TOKEN_CONST(token, LEN))
	{
		// ##44## <assing> --> LEN(STR)
		// LEN_CALL | DST | STR | --- |
		//				^	 ^
		//  			|	 |
		//				|	 | --- ukazatel do tabulky na řetězec, ze kterého se počítá délka
		//				|--- ukazatel na do tabulky na levou stranu přiřazení

		// Pokud nebyla proměnná levé strany jeěšte definována, učiní se tak
		if((*result)->defined == false) MAKE_INSTR(main_tape, DEF_VAR, (void*)(*result), NULL, NULL);
		GET_NEXT_TOKEN(token);
		CHACK_TOKEN_AND_GET_NEXT(token, LEFT_PAR, "<assg> -->LEN");
		ret_val = expr(global_table, func_name, main_tape, token, &par1, NULL, false);
		CHACK_RET(ret_val, name); 
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<assg> -->LEN");
		MAKE_INSTR(main_tape, LEN_CALL, (void*)(*result), (void*)par1, NULL);
		(*result)->defined = true;
	}
	else if(CMP_TOKEN_CONST(token, SUBSTR))
	{
		// ##45## <assing> --> SUBSTR(STR, <expr>, <expr>)
		// SUBSTR_CALL | DST | --- | --- |
		//				  ^
		//  			  |
		//				  |--- ukazatel na do tabulky na levou stranu přiřazení

		// Pokud nebyla proměnná levé strany jeěšte definována, učiní se tak
		if((*result)->defined == false) MAKE_INSTR(main_tape, DEF_VAR, (void*)(*result), NULL, NULL);
		GET_NEXT_TOKEN(token);
		CHACK_TOKEN_AND_GET_NEXT(token, LEFT_PAR, "<assg> -->SUBSTR");

		ret_val = expr(global_table, func_name, main_tape, token, &par1, NULL, false);
		CHACK_RET(ret_val, name); 
		CHACK_TOKEN_AND_GET_NEXT(token, COMMA, "<assg> -->SUBSTR");

		ret_val = expr(global_table, func_name, main_tape, token, &par2, NULL, false);
		CHACK_RET(ret_val, name); 
		CHACK_TOKEN_AND_GET_NEXT(token, COMMA, "<assg> -->SUBSTR");

		ret_val = expr(global_table, func_name, main_tape, token, &par3, NULL, false);
		CHACK_RET(ret_val, name); 

		// SUBSTR_PAR | par1 | par2 | par3 |
		//				  ^		^		^
		//  			  |		|		|
		//  			  |		|		|-- ukazatel do tabulky na třetí parametr
		//  			  |		|-- ukazatel do tabulky na druhý parametr
		//				  |--- ukazatel do tabulky na první parametr
		MAKE_INSTR(main_tape, SUBSTR_CALL, (void*)(*result), NULL, NULL);
		MAKE_INSTR(main_tape, SUBSTR_PAR, (void*)par1, (void*)par2, (void*)par3);
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<assg> -->SUBSTR");

		
		(*result)->defined = true;

	}
	else if(CMP_TOKEN_CONST(token, ORD))
	{	
		// ##45## <assing> --> ORD(STR, <expr>)
		// ORD_CALL | DST | STR | EXPR |
		//				^	 ^		^
		//				|	 |		|
		//				|	 |		| --- ukazatel do tabulky na vypočtený číselný výraz
		//				| 	 |--- ukazatel do tabulky na zpracovávaný řetězec
		//				| -- ukazatel do tabulky na L hodnotu

		// Pokud nebyla proměnná levé strany jeěšte definována, učiní se tak
		if((*result)->defined == false) MAKE_INSTR(main_tape, DEF_VAR, (void*)(*result), NULL, NULL);
		GET_NEXT_TOKEN(token);
		CHACK_TOKEN_AND_GET_NEXT(token, LEFT_PAR, "<assg> -->ORD");

		ret_val = expr(global_table, func_name, main_tape, token, &par1, NULL, false);
		CHACK_RET(ret_val, name); 
		CHACK_TOKEN_AND_GET_NEXT(token, COMMA, "<assg> -->ORD");

		ret_val = expr(global_table, func_name, main_tape, token, &par2, NULL, false);
		CHACK_RET(ret_val, name); 
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<assg> -->ORD");

		MAKE_INSTR(main_tape, ORD_CALL, (void*)(*result), (void*)par1, (void*)par2);
		(*result)->defined = true;
	}
	else if(CMP_TOKEN_CONST(token, CHR))
	{
		// ##46## <assing> --> CHR(<expr>)
		// CHR_CALL | DST | EXPR | --- |
		//				^	 ^		
		//				|	 |		
		//				|	 |	--- ukazatel do tabulky na číselný parametr funkce chr	
		//				| -- zkazatel do tabulky na L hodnotu
		
		// Pokud nebyla proměnná levé strany jeěšte definována, učiní se tak
		if((*result)->defined == false) MAKE_INSTR(main_tape, DEF_VAR, (void*)(*result), NULL, NULL);
		GET_NEXT_TOKEN(token);
		CHACK_TOKEN_AND_GET_NEXT(token, LEFT_PAR, "<assg> -->CHR");

		ret_val = expr(global_table, func_name, main_tape, token, &par1, NULL, false);
		CHACK_RET(ret_val, name); 
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<assg> -->CHR");

		MAKE_INSTR(main_tape, CHR_CALL, (void*)(*result), (void*)par1, NULL);
		(*result)->defined = true;
	}
	else if(CMP_TOKEN_CONST(token, PRINT))
	{
		// ##47## <assign> --> PRINT(<params-print>)	
		// Print uloží do L hodnoty Nan

		// Pokud nebyla proměnná levé strany jeěšte definována, učiní se tak									
		if((*result)->defined == false) MAKE_INSTR(main_tape, DEF_VAR, (void*)(*result), NULL, NULL);
		GET_NEXT_TOKEN(token);
		CHACK_TOKEN_AND_GET_NEXT(token, LEFT_PAR, "<assg> -->PRINT");

		ret_val = params_print(global_table, func_name, main_tape, token);
		CHACK_RET(ret_val, name);
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<assg> -->PRINT");

		// zkopírování hodnoty NONE
		// CPY_COM | dst | --- | --- |
		//			  ^
		//			  |
		//			  | -- ukazatel do tabulky na L hodnotu
		if((*result)->defined == false) MAKE_INSTR(main_tape, DEF_VAR, (void*)(*result), NULL, NULL);
		MAKE_INSTR(main_tape, CPY_COM, (void*)(*result), NULL, NULL);
		(*result)->defined = true;
	}
	else 
	{	
		// Ani jedna předchozí možnost nebyhovovala >> chyba syntaktické analýzy
		ret_val = ERR_SYN;
	}

	return ret_val;

}

/**
 * Funkce pravidla <expr_fce>
 * -------------------------
 * global_table - globální tabulka
 * func_name - jméno funkce, v jejimž těle se nachází výpočet,
 *			 - pokud je výpočet v globální části je nastaveno na NULL
 * main_tape - instrukční páska
 * token - aktuální načtený token
 * result - ukazatel do tabulky na výsledek případného přiřazení
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int expr_fce(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token, htab_item_t **result)
{
	int ret_val = ERR_OK;
	string_t *name = NULL; 		// pro lepří dealokaci
	instr_t *instr_holder = NULL;
	void *addr1 = NULL;
	void *addr2 = NULL;
	void *addr3 = NULL;
	htab_item_t *tmp;

	// Alokace pomocného tokenu a překopírování dat aktuálního do něj
	token_t *token_identif = token_ctor();
	if(token_identif == NULL) return ERR_INTER_RUN;
	if((token_identif->data = (token_data_t*)malloc(sizeof(token_data_t))) == NULL)
	{
		free(token_identif);
		return ERR_INTER_RUN;
	}
	if((token_identif->data->str = string_ctor()) == NULL)
	{
		free(token_identif->data);
		free(token_identif);
		return ERR_INTER_RUN;
	}
	if(!string_cpy(token_identif->data->str, token->data->str))
	{
		free(token_identif->data->str);
		free(token_identif->data);
		free(token_identif);
		return ERR_INTER_RUN;
	}
	token_identif->type= IDENTIFIER;

	GET_NEXT_TOKEN(token);

	if(CMP_TOKEN_CONST(token, LEFT_PAR))
	{
		//  ##48## <expr_fce> -> (<params_call)
		//  CALL | *fce | id_last | --- |
		//			^		^
		//			|		|
		//			|		|
		//			|		|--- ukazatel do pásky na poslední operaci s parametrem
		//			|--- ukazatel do tabulky na funkci,(ne jeji tabulku)

		//PLACE_HOLDER
		MAKE_INSTR(main_tape, PLACE_HOLDER, NULL, NULL, NULL);					
		instr_holder = tape_last(*main_tape);										
		if((addr1 = (void*)htab_get_item(global_table, token_identif->data->str)) == NULL) return ERR_SEM_DEF;	//addr1

		GET_NEXT_TOKEN(token);
		tmp = (htab_item_t*)addr1;

		ret_val = params_call(global_table, tmp->it_data->fce->h_table, func_name, main_tape, token);	
		CHACK_TOKEN_AND_GET_NEXT(token, RIGHT_PAR, "<assg_op_fce> -->call");
		addr2 = tape_last(*main_tape);		// addr2

		// Výměna PLACE_HOLDER
		if(!instr_change(instr_holder, CALL, addr1, addr2, addr3))
		{
			error_msg("Chyba pri place holder while.\n%s", MSG_INTER_RUN);
			
			return ERR_INTER_RUN;
		}
		
	}
	else if(IS_OPERATOR(token) || CMP_TOKEN_CONST(token, EOL_t) || CMP_TOKEN_CONST(token, EOF_t))
		{
			// ##49## <expr_fce> -> <operator><expr2>
			ret_val = expr(global_table, func_name, main_tape, token, result, token_identif, false);

		}


	return ret_val;

}

/**
 * Funkce pravidla <params_call>
 * -------------------------
 * global_table - globální tabulka
 * func_table - func_table - Ukazatel na tabulku funkce, která se volá
 * func_name - jméno funkce, v jejimž těle se nachází výpočet,
 *			 - pokud je výpočet v globální části je nastaveno na NULL
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int params_call(htab_t *global_table, htab_t *func_table, string_t *func_name, instr_tape_t **main_tape, token_t *token)
{
	int ret_val = ERR_OK;
	string_t *name = NULL; 

	// Počítadlo počtu přiřazených parametrů
	unsigned short param_count = 0;

	// Pomocné proměnné
	htab_item_t *item = NULL;
	htab_item_t *func = NULL;
	token_t *token_new = NULL;

	if(CMP_TOKEN_CONST(token, IDENTIFIER))
	{
		// ##50##  <params_call> --> id<params_n_call>
		// Kontrola, zda se jedná při volání o lokální, nebo globální proměnnou
		if(func_name != NULL)
		{
			if((func = htab_get_item(global_table, func_name)) == NULL) return ERR_SYN;	
			if((item = htab_get_item(func->it_data->fce->h_table, token->data->str)) == NULL)
			{
				if((item = htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
			}
		}
		else
		{
			if((item = htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
		}
		if(item->type == FUNCTION) return ERR_SYN;

		// PARAM | *id | --- | --- |
		//			^
		//			|
		//			|--- parametr je ukazatelem na prvek do tabulky
		MAKE_INSTR(main_tape, PARAM, (void*)item, NULL, NULL);
		param_count++;
		GET_NEXT_TOKEN(token);

		ret_val = params_n_call(global_table, func_name, main_tape, token, &param_count);

	}
	if(CMP_TOKEN_CONST(token, INT) || CMP_TOKEN_CONST(token, FLOAT) || 
	   CMP_TOKEN_CONST(token, STRING) || CMP_TOKEN_CONST(token, NONE) ||
	   CMP_TOKEN_CONST(token, COMMENT))
	{
		// Alokace tokenu pro pomocnou proměnnou a překopírování dat z tokenu hlavního
		if((token_new = token_ctor()) == NULL) return ERR_INTER_RUN;
		if((token_new->data = (token_data_t*)malloc(sizeof(token_data_t))) == NULL)
		{
			free(token_new);
			return ERR_INTER_RUN;
		}
		if((token_new->data->str = string_rand()) == NULL)
		{
			free(token_new->data);
			free(token);
			return ERR_INTER_RUN;
		}
		token_new->type = token->type;

		if(!htab_insert(global_table, token_new->data->str, token))
		{
			
			free(token_new->data);
			free(token);
			return ERR_INTER_RUN;
		}

		// Znovuzískání ukazatele vložené pomocné proměnné
		item = htab_get_item(global_table, token_new->data->str);
		
		token_dtor(&token_new);

		// PARAM | *id | --- | --- |
		//			^
		//			|
		//			|--- ukazatel ja parametrem v tabule
		MAKE_INSTR(main_tape, PARAM, (void*)item, NULL, NULL);
		param_count++;
		GET_NEXT_TOKEN(token);

		ret_val = params_n_call(global_table, func_name, main_tape, token, &param_count);	
	}

	//  ##51## <params_call> --> e

	// Kontrola, zda byl správný počet parametrů při volání funkce
	if(param_count != func_table->params_cnt)
	{
		return ERR_PARAMS_COUNT;
	}
	else
	{
		return ret_val;
	}
}

/**
 * Funkce pravidla <params_n_call>
 * -------------------------
 * global_table - globální tabulka
 * func_name - jméno funkce, v jejimž těle se nachází výpočet,
 *			 - pokud je výpočet v globální části je nastaveno na NULL
 * main_tape - instrukční páska
 * token - aktuální načtený token
 * param_count - počet dopoud zpracovaných parametrů
 *
 * Funkce inkrementuje počítadlo zpracovaných parametrů při volání. Vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int params_n_call(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token, unsigned short *param_count)
{
		int ret_val = ERR_OK;
	string_t *name = NULL; 

	htab_item_t *item = NULL;
	htab_item_t *func = NULL;
	token_t *token_new = NULL;

	if(CMP_TOKEN_CONST(token, COMMA))
	{
		GET_NEXT_TOKEN(token);
		if(CMP_TOKEN_CONST(token, IDENTIFIER))
		{
			// ##52##  <params_n_call> --> ,id<params_n_call>

			// Nejdíve se zkoumá, zda je proměnná lokální, nebo globální
			if(func_name != NULL)
			{
				if((func = htab_get_item(global_table, func_name)) == NULL) return ERR_SEM_DEF;	
				if((item = htab_get_item(func->it_data->fce->h_table, token->data->str)) == NULL)
				{
					if((item = htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
				}
			}
			else
			{
				if((item = htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
			}
			if(item->type == FUNCTION) return ERR_SYN;

			// PARAM | *id | --- | --- |
			//			^
			//			|
			//			|--- ukazatel na parametr
			MAKE_INSTR(main_tape, PARAM, (void*)item, NULL, NULL);
			// Zvýšení počítadla parametrů
			(*param_count)++;
			GET_NEXT_TOKEN(token);
			ret_val = params_n_call(global_table, func_name, main_tape, token, param_count);	

		}
		if(CMP_TOKEN_CONST(token, INT) || CMP_TOKEN_CONST(token, FLOAT) ||
		   CMP_TOKEN_CONST(token, STRING) || CMP_TOKEN_CONST(token, NONE) ||
		   CMP_TOKEN_CONST(token, COMMENT))
		{
			// ##52-57## --> DATA<params_n_call>
			// Alokace tokenu pro pomocnou proměnnou a překopírování dat z tokenu hlavního
			if((token_new = token_ctor()) == NULL) return ERR_INTER_RUN;
			if((token_new->data = (token_data_t*)malloc(sizeof(token_data_t))) == NULL)
			{
				free(token_new);
				return ERR_INTER_RUN;
			}
			if((token_new->data->str = string_rand()) == NULL)
			{
				free(token_new->data);
				free(token);
				return ERR_INTER_RUN;
			}
			token_new->type = token->type;

			// Znovuzískání ukazatele vložené pomocné proměnné
			if(!htab_insert(global_table, token_new->data->str, token_new))
			{
				
				free(token_new->data);
				free(token);
				return ERR_INTER_RUN;
			}

			item = htab_get_item(global_table, token_new->data->str);
			token_dtor(&token_new);

			// PARAM | *id | --- | --- |
			//			^
			//			|
			//			|--- ukazatel na parametr do tabulky
			MAKE_INSTR(main_tape, PARAM, (void*)item, NULL, NULL);
			(*param_count)++;
			GET_NEXT_TOKEN(token);
			ret_val = params_n_call(global_table, func_name, main_tape, token, param_count);	//TODO
		}
	}

	//  ##58## <params_call> --> e

	return ret_val;
}

/**
 * Funkce pravidla <operator>
 * -------------------------
 * token - token pro rozhodnutí, zda se jedná o operátor
 *
 * V případě rozpoznání operátoru vrací funkce 0, v opačném případě kód chyby
 */
int operator(token_t *token)
{
	int ret_val = ERR_OK;

	if(!IS_OPERATOR(token))
	{
		error_msg("CHYBA: <operator>\n%s", MSG_SYN);
		ret_val = ERR_SYN;
	}

	// ##30-40## <operator> --> všechny operatory											
	GET_NEXT_TOKEN(token);

	return ret_val;
}

/**
 * Funkce pravidla <params>
 * -------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * token - aktuální načtený token
 * key - jméno funkce
 * func_def - vlastní tabulka funkce
 *
 */
int params(htab_t *global_table, instr_tape_t **main_tape, token_t *token, string_t *key, htab_t **func_def)
{
	string_t* name = NULL;
	int ret_val = ERR_OK;
	htab_item_t *tmp = NULL;

	// Kontrola, zda neprovádíme redefinici
	if(htab_get_item(global_table, key) != NULL)
	{
		error_msg("CHYBA funkce, nebo proměnná stejného jména existuje.\n%s", MSG_SEM_DEF);
		return ERR_SEM_DEF;
	}
	// Vytvoření funkce
	if(!htable_make_func(global_table, key))
	{
		error_msg("CHYBA nepodařilo se vytvořit funkci\n");
		return ERR_INTER_RUN;
	}
	// Znovuzískání ukazatele na fukci
	if((tmp = htab_get_item(global_table, key)) == NULL)
	{
		error_msg("CHYBA nepodařilo se znovu získat funkci z tabulky.\n");
		return ERR_INTER_RUN;
	}

	*func_def = tmp->it_data->fce->h_table;

	if(CMP_TOKEN_CONST(token, IDENTIFIER))
	{
		// ##60## <params> --> id <param-N>
		ret_val = params_append(*func_def, token);
		CHACK_RET(ret_val, name);
		type_defined(*func_def, token->data->str);
		MAKE_INSTR(main_tape, DEF_VAR, (void*)(htab_get_item(*func_def, token->data->str)), NULL, NULL);
		GET_NEXT_TOKEN(token);
		ret_val = param_n(*func_def, main_tape, token);
	}

	// ##61## <params> --> e 																	

	return ret_val;
}

/**
 * Funkce pravidla <params>
 * -------------------------
 * table - vlastní tabulka funkce
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 *Vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int param_n(htab_t *table, instr_tape_t **main_tape, token_t *token)
{
	int ret_val = ERR_OK;
	string_t *name = NULL;

	if(CMP_TOKEN_CONST(token, COMMA))
	{
		// ##45## <param-N> --> ,id <param-N>
		GET_NEXT_TOKEN(token);
		if(CMP_TOKEN_CONST(token, IDENTIFIER))
		{
			ret_val = params_append(table, token);
			CHACK_RET(ret_val, name);		

			MAKE_INSTR(main_tape, DEF_VAR, (void*)(htab_get_item(table, token->data->str)), NULL, NULL);
			GET_NEXT_TOKEN(token);
			ret_val = param_n(table, main_tape, token);
		}
		else
		{
			error_msg("CHYBA v definici funkce, hybí praametr za čárkou.%s", MSG_SYN);
			ret_val = ERR_SYN;
		}
	}

	// ##61## <param-N> --> e 																		

	return ret_val;
}

/**
 * Funkce pravidla <params_print>
 * -------------------------
 * global_table -  globálníé tabulka
 * func_name - jméno funkce ve které se volá, pokud se volá v globále je NULL
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int params_print(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token)
{
	string_t* name = NULL;
	int ret_val = ERR_OK;
	htab_item_t *result = NULL;

	
	if(!CMP_TOKEN_CONST(token, RIGHT_PAR))
	{
		// ##62## <params_print> --> <expr><params_print_n>										
		// PRINT_CALL | PARAM | --- | --- |
		//				^
		//				|
		//				| -- ukazatel do tabulky na první parametr k vytisknutí


		ret_val = expr(global_table, func_name, main_tape, token, &result, NULL, false);
		MAKE_INSTR(main_tape, PRINT_CALL, (void*)result, NULL, NULL);
		CHACK_RET(ret_val, name);
		ret_val = param_print_n(global_table, func_name, main_tape, token);
	}

	// ##63## <params_print> --> e 																	

	// PRINT_CALL | ---- | ---- | ----|
	// Odřádkuje
	MAKE_INSTR(main_tape, PRINT_CALL, NULL, NULL, NULL);
	return ret_val;
}

/**
 * Funkce pravidla <params_print_n>
 * -------------------------
 * global_table -  globálníé tabulka
 * func_name - jméno funkce ve které se volá, pokud se volá v globále je NULL
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int param_print_n(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token)
{
	string_t* name = NULL;
	int ret_val = ERR_OK;
	htab_item_t *result = NULL;

	if(CMP_TOKEN_CONST(token, COMMA))
	{
		// ##64## <param-print-N> --> ,<expr><param_print_n>

		// PRINT_CALL | param | ---- | ---- |
		//				^
		//				|
		//				| -- ukazatel do tabulky na další parametr k vytisknutí
		GET_NEXT_TOKEN(token);
		ret_val = expr(global_table, func_name, main_tape, token, &result, NULL, false);
		MAKE_INSTR(main_tape, PRINT_CALL, NULL, (void*)result, NULL);
		CHACK_RET(ret_val, name);
		ret_val = param_print_n(global_table, func_name, main_tape, token);
	}

	// ##65## <param-print-N> --> e 																	

	return ret_val;
}

/**
 * Funkce pravidla <stat_list_F>
 * -------------------------
 * global_table - globální tabulka
 * func_name - jméno funkce, ve které se vykonávají příkazy
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int stat_list_f(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token)
{
	string_t* name = NULL;
	int ret_val = ERR_OK;

	// ##66## <stat-list> --> <free-line><stat-F><free-line><stat-list-F-N>	

	ret_val = free_line(global_table, main_tape, token);
	CHACK_RET(ret_val, name);

	ret_val = stat_f(global_table, func_name, main_tape, token);
	CHACK_RET(ret_val, name);

	eol_eof(global_table, main_tape, token);

	ret_val = free_line(global_table, main_tape, token);
	CHACK_RET(ret_val, name);

	ret_val = stat_list_f_n(global_table, func_name, main_tape, token);

	return ret_val;
}

/**
 * Funkce pravidla <stat_list_F_N>
 * -------------------------
 * global_table - globální tabulka
 * func_name - jméno funkce, ve které se vykonávají příkazy
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int stat_list_f_n(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token)
{
	string_t* name = NULL;
	int ret_val = ERR_OK;

	if(!CMP_TOKEN_CONST(token, EOF_t) && !CMP_TOKEN_CONST(token, DEDENT))
	{
		// ##67## <stat-list-N> --> <free-line><stat-F>EOL<free-line><stat-list-F-N>	

		ret_val = free_line(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		ret_val = stat_f(global_table, func_name, main_tape, token);
		CHACK_RET(ret_val, name);

		eol_eof(global_table, main_tape, token);
	
		ret_val = free_line(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		ret_val = stat_list_f_n(global_table, func_name, main_tape, token);
	}

	// ##68## <stat-list-N> --> e 																	

	return ret_val;
}

/**
 * Funkce pravidla <stat_f>
 * -------------------------
 * global_table - globální tabulka
 * func_name - jméno funkce, ve které se vykonávají příkazy
 * main_tape - instrukční páska
 * token - aktuální načtený token
 *
 * Funkce vrací v případě úspěšné analýzy 0, jinak kód chyby.
 */
int stat_f(htab_t *global_table, string_t *func_name, instr_tape_t **main_tape, token_t *token)
{
	string_t* name = NULL;
	int ret_val = ERR_OK;
	token_t *token_new = NULL;
	htab_item_t *result = NULL;
	htab_item_t *addr1 = NULL;
	htab_item_t *addr2 = NULL;
	htab_item_t *addr3 = NULL;
	htab_item_t *tmp = NULL;
	htab_t *fce_table = NULL;
	instr_t* instr_holder = NULL;

	if(CMP_TOKEN_CONST(token, IF))
	{
		// ##54## <stat-F> --> if<expr>:EOL INDENT<stat-list-F>DEDENT else: EOL INDENT <stat-list-F>DEDENT 	

		// IF_COM | expr | else(false) | konec
		//           ^			^			^
		//			 |			|			|
		//			 |			|			|-- ukazatel do pásky na poslední příkaz elsu (poslední if-else)
		//			 |			|-- ukazatel do pásky na poslední příkaz if části před else
		//			 |-- ukazatel do tabulky na hodnotu řídicího výrazu

		

		GET_NEXT_TOKEN(token);
		ret_val = expr(global_table, func_name, main_tape, token, (htab_item_t**)(&addr1), NULL, false);	//addr1

		//PLACE_HOLDER
		MAKE_INSTR(main_tape, PLACE_HOLDER, NULL, NULL, NULL);					
		instr_holder = tape_last(*main_tape);									

		CHACK_RET(ret_val, name);
		CHACK_TOKEN_AND_GET_NEXT(token, COLON, "<stat-F>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, EOL_t, "<stat-F>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, INDENT, "<stat-F>-->if");

		ret_val = stat_list_f(global_table, func_name, main_tape, token);
		CHACK_RET(ret_val, name);
		CHACK_TOKEN_AND_GET_NEXT(token, DEDENT, "<stat-F>-->if");
		addr2 = (void*)tape_last(*main_tape);										//addr2

		CHACK_TOKEN_AND_GET_NEXT(token, ELSE, "<stat-F>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, COLON, "<stat-F>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, EOL_t, "<stat-F>-->if");
		CHACK_TOKEN_AND_GET_NEXT(token, INDENT, "<stat-F>-->if");

		ret_val = stat_list_f(global_table, func_name, main_tape, token);
		CHACK_RET(ret_val, name);

		addr3 = (void*)tape_last(*main_tape);									//addr3
		dedent_eof(global_table, main_tape, token);

		// výměna place_hoder
		if(!instr_change(instr_holder, IF_COM, addr1, addr2, addr3))
		{
			error_msg("Chyba pri place holder <stat-F-if>.\n%s", MSG_INTER_RUN);
			
			return ERR_INTER_RUN;
		}
	}
	else if(CMP_TOKEN_CONST(token, WHILE))
	{
		// ##69## <stat-F> --> while<expr>: EOL INDENT <stat-list-F> DEDENT	
		// WHILE_COM | expr | begin | end
		//				^		^		^
		//				|		|		|
		//				|		|		|-- ukazatel do pásky na poslední příkaz whilu
		//				|		|-- ukazatel do pásky na první příkaz před tělem whilu
		//				|-- ukazatel do globální tabulky na řídicí výraz, bude se buset přepočítávat

		//PLACE_HOLDER
		MAKE_INSTR(main_tape, PLACE_HOLDER, NULL, NULL, NULL);					
		instr_holder = tape_last(*main_tape);									
		

		GET_NEXT_TOKEN(token);
		ret_val = expr(global_table, func_name, main_tape, token, (htab_item_t**)(&addr1), NULL, false);	//addr1
		CHACK_RET(ret_val, name);

		addr2 = (void*)tape_last(*main_tape);									//addr2

		CHACK_TOKEN_AND_GET_NEXT(token, COLON, "<stat-F>-->while");
		CHACK_TOKEN_AND_GET_NEXT(token, EOL_t, "<stat-F>-->while");
		CHACK_TOKEN_AND_GET_NEXT(token, INDENT, "<stat-F>-->while");

		ret_val = stat_list(global_table, main_tape, token);
		CHACK_RET(ret_val, name);

		dedent_eof(global_table, main_tape, token);
		addr3 = (void*)tape_last(*main_tape);									//addr3

		// výměna place_hoder
		if(!instr_change(instr_holder, WHILE_COM, addr1, addr2, addr3))
		{
			error_msg("Chyba pri place holder <stat-F>-->while.\n%s", MSG_INTER_RUN);
			
			return ERR_INTER_RUN;
		}
	}
	else if(CMP_TOKEN_CONST(token, RETURN))
	{
		// ##70## <stat-F> --> return <expr>														
		GET_NEXT_TOKEN(token);

		if((token->type >= INT && token->type <= IDENTIFIER) || token->type == NONE)
		{
			ret_val = expr(global_table, func_name, main_tape, token, &addr1, NULL, false);	// addr1
		}

		if((token_new = token_ctor()) == NULL)
		{ 
			return ERR_INTER_RUN;
			
		}
		if((token_new->data = (token_data_t*)malloc(sizeof(token_data_t))) == NULL)
		{
			free(token_new);
			return ERR_INTER_RUN;
		}
		if(((token_new->data->str = string_make("$return")) == NULL))
		{
			free(token_new->data);
			free(token);
			return ERR_INTER_RUN;
		}
		token_new->type = token->type;
		tmp = htab_get_item(global_table, func_name);
		fce_table = tmp->it_data->fce->h_table;
		
		token_dtor(&token_new);

		// CPY_COM | dst | src | --- |
		MAKE_INSTR(main_tape, CPY_COM, (void*)result, (void*)addr1, NULL);	

	}
	else
	{
		// ##71## <stat-F> --> <stat-const>														
		ret_val = stat_const(global_table, func_name, main_tape, token);
	}

	return ret_val;
}

/**
 * Funkce syntaktické anatýzy.
 * ----------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * 
 * V případě úspěchu vrací funkce 0 v jiném kód chyby.
 */
int parse(htab_t *global_table, instr_tape_t **main_tape)
{

	// Alokace tokenu a kontrola
	token_t *token = token_ctor();
	if(token == NULL)
	{
		error_msg("CHYBA: Nepodařil se alokovat token\n");
		return ERR_INTER_RUN;
	}

	// Načtení prvního tokenu
	GET_NEXT_TOKEN(token);

	// Provedení syntaktické analýzy
	int ret_val = prog(global_table, main_tape, token);
	
	// Kontrola průběhu syntaktické analýza
	if(ret_val != ERR_OK) 
	{
		error_msg("Chyba během syntaktické analýzy\n");
	}

	return ret_val;
}