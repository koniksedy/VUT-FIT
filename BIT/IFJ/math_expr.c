/*Hlavičkový soubor k modulu: math_expr
  Vypracoval: Ondřej Pavlacký (xpavla15) 
  Přeloženo gcc 7.4.0, standard C99
  Poslední úprava 11.12.2019, 2.BIT 2019/20  */

#include "math_expr.h"

//Kotrola typu tokenu na EOL nebo EOF
int check_type(htab_t* global_table, string_t* func_name, token_t* token){
	
	while(token->type != EOL_t && token->type != EOF_t){
		GET_NEXT_TOKEN(token); //Pokud to neni jeden z nic bereme dalsi token
	}
	return ERR_OK;
}

//Kontrola zda je token dollar, tzn podle pravidel precedenční tabulky
bool is_token_dollar(token_t *token) 
{
	return (token->type == COMMA || token->type == EOL_t || token->type == EOF_t ||token->type == COLON || token->type == RIGHT_PAR);
}

//Probiha kontrola podle precedencni tabulky a nasledne tvoreni instrukci a vypoctu
int expr(htab_t *global_table, string_t *func_name, instr_tape_t **tape, token_t *token, htab_item_t **result, token_t* token_first, bool only_check)
{
	bool is_expr_empty = true;
	unsigned char tmp_type = 0;
	htab_item_t* ukazatel = NULL;
	htab_item_t *tmp1 = NULL; 
	htab_item_t *tmp2 = NULL; //Nastavení všech hodnot na NULL/0
	htab_item_t *tmp3 = NULL; 
	htab_item_t *tmp4 = NULL;
	string_t* name = NULL;
	token_t* touken = NULL;
	int ret_val = -1; //Aby jsme věděli že se nic nepokazilo, protože -1 se nikde nepoužívá
	bool empty = true;
	prec_stack_t *stack = prec_stack_ctor(); //Vytvoříme si zásobník na precedenční tabulku
	
	if(stack == NULL) //Pokud se nepodaří vytvoření, vracíme interní chybu
	{
		
		return ERR_INTER_RUN;
	}
	
	htab_item_t *top_terminal = NULL;
	if(token_first != NULL){ //Token není NULL
		

		if(token_first->type != IDENTIFIER){  //Pokud to není identifikátor, error
			return ERR_SYN;
		}
		if(func_name == NULL){ //Pokud nejsme ve funkci	
			
			if((ukazatel = htab_get_item(global_table, token_first->data->str)) == NULL) return ERR_SEM_DEF;
			if((ukazatel->defined) == false) return ERR_SEM_DEF;
			prec_stack_push(stack, ukazatel, true, false);  //Na zásobník dáme item z tabulky
		}
		else{  //Jsme ve funkci

			if((ukazatel = htab_get_item(global_table, func_name)) == NULL) return ERR_INTER_RUN;
			//Tady je v lokální tabulce
			if((ukazatel= htab_get_item(ukazatel->it_data->fce->h_table, token_first->data->str)) == NULL) {
				if((ukazatel= htab_get_item(global_table, token_first->data->str)) == NULL) return ERR_SEM_DEF;
				//Hledany  je v globalu
			}
			//Tady uz nekde je
			if((ukazatel->defined) == false) return ERR_SEM_DEF;
			prec_stack_push(stack, ukazatel, true, false);

		}
	}
	

	while(ret_val == -1) //Dokud se to nepokazí, iterujeme
	{
		
		top_terminal = prec_stack_top_terminal(stack); //Načteme si ze zásobníku terminál
		if(top_terminal == NULL)
		{
			
			prec_stack_dtor(stack); //Pokud nastane chyba, dealokujeme a hlásíme chybu
			return ERR_INTER_RUN;
		}
		switch(top_terminal->type) //Tady rozhodujeme jaký je to typ
		{
			case DOLLAR: //Je typ  dollar
			
				if((token->type >= INT && token->type <= OPERATOR_NEROVNOST) || token->type == LEFT_PAR  ||
					token->type == NONE)
				{
					// <
					if((token->type >= INT && token->type <= STRING) || token->type == NONE){
						
						name = string_rand();
						htab_insert(global_table, name, token); //Vložíme prvek do tabulky
						ukazatel = htab_get_item(global_table, name);
						ukazatel->defined = true;  //Řekneme že je definovaný
						if(!only_check)
						{
							MAKE_INSTR(tape, DEF_VAR, (void*)ukazatel, NULL, NULL); //Voláme funkci na vytvoření instrukce
						}
						prec_stack_push(stack, ukazatel, true, false); //Vložíme na stack
						string_dtor(&name);
					}
					else if(token->type == IDENTIFIER){  //Je to identifikátor
						
						if(func_name == NULL){ //Nejsme ve funkci

							if((ukazatel = htab_get_item(global_table, token->data->str)) == NULL){ return ERR_SEM_DEF;} //Vezmeme si položku z tabulky
							if((ukazatel->defined) == false) {return ERR_SEM_DEF;}
							prec_stack_push(stack, ukazatel, true, false); //Vložíme jej na zásobník

						}
						else{ //Jsme ve funkci

							if((ukazatel = htab_get_item(global_table, func_name)) == NULL) return ERR_INTER_RUN;
							//Tady je v lokální tabulce 
							if((ukazatel= htab_get_item(ukazatel->it_data->fce->h_table, token->data->str)) == NULL) {
								if((ukazatel= htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
								//Hledany  je v globalu
							}
							//Tady uz nekde je
							if((ukazatel->defined) == false) return ERR_SEM_DEF;
							prec_stack_push(stack, ukazatel, true, false);
						}

					}
					else{ //Pokud je to něco jiného
					htab_item_t* item = malloc(sizeof(htab_item_t));
					item->k = NULL;
					item->type = token->type;
					item->defined = true;
					item->it_data = NULL;
					item->next = NULL;
					prec_stack_push(stack, item, true, true); //Vložíme prvek do tabulky
					}
				}
				else if(is_token_dollar(token))
				{

					ret_val = ERR_OK;
					if(token->type != EOF_t && token->type != EOL_t)
					{
						// Pokud jsme sem spadli, nejspíše s čárkou a result je NULL, pak je chyba
						if(result == NULL)
						{
							return ERR_SYN;
						}
						*result = prec_stack_top(stack);
					}
					else
					{
						if(!only_check)
						{
						//
							MAKE_INSTR(tape, CPY_COM, ((void*)(*result)), prec_stack_top(stack), NULL); //Vytvoření instrukce
						}
					}
					prec_stack_pop(stack); //Vyprázdníme stack
					if(prec_stack_empty(stack) == false){
						error_msg("Chyba radek +-99, neprazdny stack na konci\n");
						return ERR_SYN;
					}
					if(is_expr_empty) return ERR_SYN;
					else return ret_val;
				}
				else{
					ret_val = ERR_SYN;
				}

				GET_NEXT_TOKEN(token); //Bereme si další token

				break;

			case RIGHT_PAR: //V případě pravé závorky
			
				if((token->type >= OPERATOR_PLUS && token->type <= OPERATOR_NEROVNOST) ||
				   token->type == RIGHT_PAR || is_token_dollar(token))
				{
					//>
					//E->(E)
					is_expr_empty = false;
					tmp1 = prec_stack_top(stack); //Vezmeme si prvek z vrcholu zásobníku, musí být pravá závorka
					if(tmp1->type != RIGHT_PAR) return ERR_SYN; //Zkontrolujeme o jaký prvek se jedná
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN;
					tmp2 = prec_stack_top(stack); //Vezmeme si další prvek
					if(tmp2->type != IDENTIFIER && tmp2->type != STRING && tmp2->type != FLOAT && 
					   tmp2->type != INT && tmp2->type != NONE) return ERR_SYN; //Tenhle prvek musí být E
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN;
					tmp3 = prec_stack_top(stack); //Další prvek
					if(tmp3->type != LEFT_PAR) return ERR_SYN; //Musí být levá závorka
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN;

					prec_stack_push(stack, tmp2, false, false); //Vrátíme zpátky E
				}
				else
				{
					ret_val = ERR_SYN;
				}
				break;

			case LEFT_PAR: //Levá závorka
			
				if((token->type >= INT && token->type <= OPERATOR_NEROVNOST) || token->type == LEFT_PAR || token->type == NONE)
				{
					// <
					if((token->type >= INT && token->type <= STRING) || token->type == NONE){
						name = string_rand();
						htab_insert(global_table, name, token);
						ukazatel = htab_get_item(global_table, name);
						ukazatel->defined = true; //Řekneme že je defined
						if(!only_check)
						{
							MAKE_INSTR(tape, DEF_VAR, (void*)ukazatel, NULL, NULL); //Uděláme instrukci
						}
						prec_stack_push(stack, ukazatel, true, false); //Vložíme jej na zásobník
						string_dtor(&name);
					}
					else if(token->type == IDENTIFIER){ //Když je to identifikátor
						htab_item_t* ukazatel = NULL;
						if(func_name == NULL){ //Nejsme ve funkci
							if((ukazatel = htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
							if((ukazatel->defined) == false) return ERR_SEM_DEF; //Nastavíme defined na fasle
							prec_stack_push(stack, ukazatel, true, false); //Vložíme na zásobník
						}
						else{

							if((ukazatel = htab_get_item(global_table, func_name)) == NULL) return ERR_INTER_RUN;
							//Tady je v lokální tabulce
							if((ukazatel= htab_get_item(ukazatel->it_data->fce->h_table, token->data->str)) == NULL) {
								if((ukazatel= htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
								//Hledany  je v globalu
							}
							//Tady uz nekde je
							if((ukazatel->defined) == false) return ERR_SEM_DEF;
							prec_stack_push(stack, ukazatel, true, false); //Vložíme na zásobník
						}
					}
					else{ //Jinak vložíme na zásobník
					htab_item_t* item = malloc(sizeof(htab_item_t));
					item->k = NULL;
					item->type = token->type;
					item->defined = true;
					item->it_data = NULL;
					item->next = NULL;
					prec_stack_push(stack, item, true, true);
					}
				}
				else if(token->type == RIGHT_PAR) //Když je to pravá závorka
				{
					// =
					htab_item_t* item = malloc(sizeof(htab_item_t));
					item->k = NULL;
					item->type = token->type;
					item->defined = true;
					item->it_data = NULL;
					item->next = NULL;
					prec_stack_push(stack, item, true, true); //Jen vložíme na zásobník
				}
				else
				{
					ret_val = ERR_SYN;
				}

				GET_NEXT_TOKEN(token); //Získáme si další token


				break;

			case IDENTIFIER:
			case INT:
			case STRING:
			case COMMENT: //Pro tyto tokeny je tabulka stejná
			case FLOAT:
			case NONE:
				if((token->type >= OPERATOR_PLUS && token->type <= OPERATOR_NEROVNOST) ||
				   token->type == RIGHT_PAR || is_token_dollar(token))
				{
					// >
					is_expr_empty = false;
					tmp1 = prec_stack_top(stack); //Vezmeme si prvek z vrcholu zásobníku
					if(tmp1->type < INT || (tmp1->type > IDENTIFIER && tmp1->type != NONE)) return ERR_SYN;
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN;
					if((touken = token_ctor()) == NULL) return ERR_INTER_RUN;
					touken->type = IDENTIFIER; //Nastavíme typ na identifikátor
					if((touken->data = malloc(sizeof(token_data_t))) == NULL) return ERR_INTER_RUN;
					if((touken->data->str = string_rand()) == NULL) return ERR_INTER_RUN;
					htab_insert(global_table, touken->data->str,touken);
					if((tmp2 = htab_get_item(global_table, touken->data->str)) == NULL) return ERR_INTER_RUN; //Vložíme prvek do tabulky
					tmp2->defined = true; //Nastavíme ho na definovaný
					if(!only_check)
					{
						MAKE_INSTR(tape, DEF_VAR, (void*)tmp2, NULL, NULL);  //Vytvoříme instrukci
					}
					prec_stack_push(stack, tmp2, false, false); //Vložíme na zásobník
					token_dtor(&touken);

					// | CPY_COM | dst | sour | ---- |
					//				^		^
					//				|		|
					//				|		| -- ukazatel do tabulky na zdroj
					//				| --- ukazatel do tabulky na nnové umístění
					//if(tmp1->type != IDENTIFIER && tmp1->type != INT && tmp1->type != STRING && tmp1->type != FLOAT ) return ERR_SYN; //TODO NECO PRIDANO  //WAT
					if(!only_check)
					{
						if(tmp1->type != NONE)
						{
							MAKE_INSTR(tape, CPY_COM, (void*)tmp2, (void*)tmp1, NULL); //Vytvoříme instrukci
						}
						else
						{
							MAKE_INSTR(tape, CPY_COM, (void*)tmp2, NULL, NULL); //Vytvoříme instrukci pro none
						}
					}
				}
				else
				{
					// err
					ret_val = ERR_SYN;
				}
				break;

			case OPERATOR_VETSI:  
			case OPERATOR_MENSI:
			case OPERATOR_VETSI_ROVNO:  //Pro tyto operátory jsou pravidla v tabulce stejná
			case OPERATOR_MENSI_ROVNO:
			case OPERATOR_NEROVNOST:
			case OPERATOR_ROVNOST:
			
				if((token->type >= OPERATOR_PLUS && token->type <= OPERATOR_DELENO) ||
				   token->type == RIGHT_PAR || is_token_dollar(token))
				{
					// >
					is_expr_empty = false;
					tmp1 = prec_stack_top(stack); //E
					if(tmp1->type != IDENTIFIER) return ERR_SYN;
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN; 
					tmp2 = prec_stack_top(stack); // operátor
					if(tmp2->type < OPERATOR_VETSI || tmp2->type > OPERATOR_NEROVNOST) return ERR_INTER_RUN;
					tmp_type = tmp2->type;
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN;
					tmp3 = prec_stack_top(stack); //E
					if(tmp3->type != IDENTIFIER) return ERR_SYN;
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN;
					if((touken = token_ctor()) == NULL) return ERR_INTER_RUN;
					touken->type = IDENTIFIER; //Nastavíme na identifikátor
					if((touken->data = malloc(sizeof(token_data_t))) == NULL) return ERR_INTER_RUN;
					if((touken->data->str = string_rand()) == NULL) return ERR_INTER_RUN;
					htab_insert(global_table, touken->data->str,touken);  //Vložíme do tabulky
					if((tmp4 = htab_get_item(global_table, touken->data->str)) == NULL) return ERR_INTER_RUN; //Mezivysledek
					tmp4->defined = true; //nastavíme že je definovaný
					if(!only_check)
					{
						MAKE_INSTR(tape, DEF_VAR, (void*)tmp4, NULL, NULL); //Vytvoříme instrukci
					}
					prec_stack_push(stack, tmp4, false, false); //Vratim zpatky na zasobnik
					token_dtor(&touken);
					
					// | CMP_GEQ/GRE/EQ/LES/LEQ/NEQ/ | dst | levy oper | pravy oper  |
					//					^		^			^
					//					|		|			|--ukazatel do tabluky na zdroj
					//					|		| -- ukazatel do tabulky na zdroj
					//					| --- ukazatel do tabulky na nové umístění
					if(tmp_type == OPERATOR_VETSI && !only_check){
						MAKE_INSTR(tape, CMP_GRE, (void*)tmp4, (void*)tmp3, (void*)tmp1);
					}
					else if(tmp_type == OPERATOR_VETSI_ROVNO && !only_check){
						MAKE_INSTR(tape, CMP_GEQ, (void*)tmp4, (void*)tmp3, (void*)tmp1);
					}
					else if(tmp_type == OPERATOR_MENSI && !only_check){
						MAKE_INSTR(tape, CMP_LES, (void*)tmp4, (void*)tmp3, (void*)tmp1);
					}
					else if(tmp_type == OPERATOR_MENSI_ROVNO && !only_check){
						MAKE_INSTR(tape, CMP_LEQ, (void*)tmp4, (void*)tmp3, (void*)tmp1);
					}
					else if(tmp_type == OPERATOR_ROVNOST && !only_check){
						MAKE_INSTR(tape, CMP_EQ, (void*)tmp4, (void*)tmp3, (void*)tmp1);
					}
					else if(tmp_type == OPERATOR_NEROVNOST && !only_check){
						MAKE_INSTR(tape, CMP_NEQ, (void*)tmp4, (void*)tmp3, (void*)tmp1);
					}
				}
				else if((token->type >= INT && token->type <= IDENTIFIER) || token->type == LEFT_PAR || token->type == NONE)
				{
					// <
					if((token->type >= INT && token->type <= STRING) || token->type == NONE){
						name = string_rand();
						htab_insert(global_table, name, token); //Vložení prvku do tabulky
						ukazatel = htab_get_item(global_table, name);
						ukazatel->defined = true; //Nastavíme že je definovaný
						if(!only_check)
						{
							MAKE_INSTR(tape, DEF_VAR, (void*)ukazatel, NULL, NULL); //Vytvoříme instrukci
						}
						prec_stack_push(stack, ukazatel, true, false); //Vložíme na zásobník
						string_dtor(&name);
					}
					else if(token->type == IDENTIFIER){ //Když je to identifikátor
						htab_item_t* ukazatel = NULL;
						if(func_name == NULL){ //Nejsme ve funkci
							if((ukazatel = htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
							if((ukazatel->defined) == false) return ERR_SEM_DEF;
							prec_stack_push(stack, ukazatel, true, false);
						}
						else{
							if((ukazatel = htab_get_item(global_table, func_name)) == NULL) return ERR_INTER_RUN;
							//Tady je v lokalni tabulce
							if((ukazatel= htab_get_item(ukazatel->it_data->fce->h_table, token->data->str)) == NULL) {
								if((ukazatel= htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
								//Hledany  je v globalu
							}
							//Tady uz nekde je
							if((ukazatel->defined) == false) return ERR_SEM_DEF;
							prec_stack_push(stack, ukazatel, true, false);
						}
					}
					else{ //Jinak vložíme na zásobník
					htab_item_t* item = malloc(sizeof(htab_item_t));
					item->k = NULL;
					item->type = token->type;
					item->defined = true;
					item->it_data = NULL;
					item->next = NULL;
					prec_stack_push(stack, item, true, true);
					}


					GET_NEXT_TOKEN(token); //Vezmeme si další token
				}
				else 
				{
					ret_val = ERR_SYN;
				}
				break;

			case OPERATOR_PLUS:  //Pro tyto dva jsou pravidla stejná
			case OPERATOR_MINUS:
			
				if((token->type >= INT && token->type <= IDENTIFIER) ||
					(token->type >= OPERATOR_KRAT && token->type <= OPERATOR_DELENO) ||
					 token->type == LEFT_PAR || token->type == NONE)
				{
					// <
					if((token->type >= INT && token->type <= STRING) || token->type == NONE){
						name = string_rand();
						htab_insert(global_table, name, token); //Vložíme do tabulky
						ukazatel = htab_get_item(global_table, name);
						ukazatel->defined = true; //Nastavíme jako definovaný
						if(!only_check)
						{
							MAKE_INSTR(tape, DEF_VAR, (void*)ukazatel, NULL, NULL);  //Vytvoříme instrukci
						}
						prec_stack_push(stack, ukazatel, true, false); //Vložíme na zásobník
						string_dtor(&name);
					}
					else if(token->type == IDENTIFIER){ //Pokud je to identifikátor
						htab_item_t* ukazatel = NULL;
						if(func_name == NULL){ //Nejsme ve funkci
							if((ukazatel = htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
							if((ukazatel->defined) == false) return ERR_SEM_DEF;

							prec_stack_push(stack, ukazatel, true, false); //Vložíme na zásobník
						}
						else{ //Když jsme ve funkci
							if((ukazatel = htab_get_item(global_table, func_name)) == NULL) return ERR_INTER_RUN;
							//Tady je v lokální tabulce
							if((ukazatel= htab_get_item(ukazatel->it_data->fce->h_table, token->data->str)) == NULL) {
								if((ukazatel= htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
								//Hledany  je v globalu
							}
							//Tady uz nekde je
							if((ukazatel->defined) == false) return ERR_SEM_DEF;
							prec_stack_push(stack, ukazatel, true, false);
						}
					}
					else{ //Jinak 
					htab_item_t* item = malloc(sizeof(htab_item_t));
					item->k = NULL;
					item->type = token->type;
					item->defined = true;
					item->it_data = NULL;
					item->next = NULL;
					prec_stack_push(stack, item, true, true); //Vložíme na zásobník
					}

					GET_NEXT_TOKEN(token); //Bereme si další token
				
				}
				else if((token->type >= OPERATOR_VETSI && token->type <= OPERATOR_NEROVNOST) ||
						token->type == OPERATOR_PLUS || token->type == OPERATOR_MINUS ||
						token->type == RIGHT_PAR || is_token_dollar(token))
				{
					// >
					is_expr_empty = false;
					tmp1 = prec_stack_top(stack); //E
					if(tmp1->type != IDENTIFIER) return ERR_SYN;
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN; 
					tmp2 = prec_stack_top(stack); // Operátor
					if(tmp2->type != OPERATOR_PLUS && tmp2->type != OPERATOR_MINUS) return ERR_SYN;
					tmp_type = tmp2->type;
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN;
					tmp3 = prec_stack_top(stack); //E
					if(tmp3->type != IDENTIFIER) return ERR_SYN;
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN;
					if((touken = token_ctor()) == NULL) return ERR_INTER_RUN;
					touken->type = IDENTIFIER; //Nastavíme jej jako identifikátor
					if((touken->data = malloc(sizeof(token_data_t))) == NULL) return ERR_INTER_RUN;
					if((touken->data->str = string_rand()) == NULL) return ERR_INTER_RUN;
					htab_insert(global_table, touken->data->str,touken); //Vložíme do tabulky
					if((tmp4 = htab_get_item(global_table, touken->data->str)) == NULL) return ERR_INTER_RUN; //Mezivysledek
					tmp4->defined = true; //Nastavíme jej na definovaný
					if(!only_check)
					{
						MAKE_INSTR(tape, DEF_VAR, (void*)tmp4, NULL, NULL); //Vytvoříme instrukci
					}
					prec_stack_push(stack, tmp4, false, false); //Vratim zpatky na zasobnik
					token_dtor(&touken);
					
					// | ADD/SUB_COM | dst | levy oper | pravy oper  |
					//					^		^			^
					//					|		|			|--ukazatel do tabluky na zdroj
					//					|		| -- ukazatel do tabulky na zdroj
					//					| --- ukazatel do tabulky na nové umístění
					if(tmp_type == OPERATOR_PLUS && !only_check){
						MAKE_INSTR(tape, ADD_COM, (void*)tmp4, (void*)tmp3, (void*)tmp1); //Instrukci pro plus
					}
					else if(tmp_type == OPERATOR_MINUS && !only_check){
						MAKE_INSTR(tape, SUB_COM, (void*)tmp4, (void*)tmp3, (void*)tmp1); //Instrukce pro minus
					}
				}
				else
				{
					ret_val = ERR_SYN;
				}
				break;


			case OPERATOR_KRAT:
			case OPERATOR_DELENO:  //Tyhle tokeny mají v tabulce stejná pravidla
			case OPERATOR_DELENO_INT:
			
				if((token->type >= OPERATOR_PLUS && token->type <= OPERATOR_NEROVNOST) ||
				   token->type == RIGHT_PAR || is_token_dollar(token))
				{
					// >
					is_expr_empty = false;
					tmp1 = prec_stack_top(stack); //E
					if(tmp1->type != IDENTIFIER) return ERR_SYN;
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN; 
					tmp2 = prec_stack_top(stack); // operátor
					if(tmp2->type != OPERATOR_KRAT && tmp2->type != OPERATOR_DELENO & tmp2->type != OPERATOR_DELENO_INT) return ERR_SYN;
					tmp_type = tmp2->type;
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN;
					tmp3 = prec_stack_top(stack); //E
					if(tmp3->type != IDENTIFIER) return ERR_SYN;
					if((prec_stack_pop(stack)) == false) return ERR_INTER_RUN;

					if((touken = token_ctor()) == NULL) return ERR_INTER_RUN;
					touken->type = IDENTIFIER;  //Nastavíme jako identifikátor
					if((touken->data = malloc(sizeof(token_data_t))) == NULL) return ERR_INTER_RUN;
					if((touken->data->str = string_rand()) == NULL) return ERR_INTER_RUN;
					htab_insert(global_table, touken->data->str,touken); //Vložíme do tabulky
					if((tmp4 = htab_get_item(global_table, touken->data->str)) == NULL) return ERR_INTER_RUN; //Mezivysledek
					tmp4->defined = true; //nastavíme defined na true
					if(!only_check)
					{
						MAKE_INSTR(tape, DEF_VAR, (void*)tmp4, NULL, NULL); //Vytvoříme instrukci
					}
					prec_stack_push(stack, tmp4, false, false); //Vratim zpatky na zasobnik
					token_dtor(&touken);
					
					// | MUL/DIV_COM | dst | levy oper | pravy oper  |
					//					^		^			^
					//					|		|			|--ukazatel do tabluky na zdroj
					//					|		| -- ukazatel do tabulky na zdroj
					//					| --- ukazatel do tabulky na nové umístění
					if(tmp_type == OPERATOR_KRAT && !only_check){
						MAKE_INSTR(tape, MUL_COM, (void*)tmp4, (void*)tmp3, (void*)tmp1); //Instrukce pro násobení
					}
					else if(tmp_type == OPERATOR_DELENO && !only_check){
						MAKE_INSTR(tape, DIV_COM, (void*)tmp4, (void*)tmp3, (void*)tmp1); //Pro dělení
					}
					else if(tmp_type == OPERATOR_DELENO_INT && !only_check){
						MAKE_INSTR(tape, DIV_INT_COM, (void*)tmp4, (void*)tmp3, (void*)tmp1); //Pro děleno int
					}
				}
				else if((token->type >= INT && token->type <= IDENTIFIER) || token->type == LEFT_PAR || token->type == NONE)
				{
					// <
					if((token->type >= INT && token->type <= STRING) || token->type == NONE){
						name = string_rand();
						htab_insert(global_table, name, token); //Vložíme do tabulky
						ukazatel = htab_get_item(global_table, name);
						ukazatel->defined = true; //nastavíme defined na true
						prec_stack_push(stack, ukazatel, true, false);
						if(!only_check)
						{
							MAKE_INSTR(tape, DEF_VAR, (void*)ukazatel, NULL, NULL);  //Vytvoříme instrukci
						}
						string_dtor(&name);
					}
					else if(token->type == IDENTIFIER){ //Pokud je to identifikátror
						htab_item_t* ukazatel = NULL;
						if(func_name == NULL){
							if((ukazatel = htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
							if((ukazatel->defined) == false) return ERR_SEM_DEF;
							prec_stack_push(stack, ukazatel, true, false); //Vložím na zásobník
						}
						else{
							if((ukazatel = htab_get_item(global_table, func_name)) == NULL) return ERR_INTER_RUN;
							//Tady je v lokalni tabulce
							if((ukazatel= htab_get_item(ukazatel->it_data->fce->h_table, token->data->str)) == NULL) {
								if((ukazatel= htab_get_item(global_table, token->data->str)) == NULL) return ERR_SEM_DEF;
								//Hledany  je v globalu
							}
							//Tady uz nekde je
							if((ukazatel->defined) == false) return ERR_SEM_DEF;
							prec_stack_push(stack, ukazatel, true, false);
						}
					}
					else{ //Jinak vložím zpátky na zásobník
					htab_item_t* item = malloc(sizeof(htab_item_t));
					item->k = NULL;
					item->type = token->type;
					item->defined = true;
					item->it_data = NULL;
					item->next = NULL;
					prec_stack_push(stack, item, true, true);
					}

					GET_NEXT_TOKEN(token); //Vezmu si další token
				}
				else
				{
					ret_val = ERR_SYN;
				}
				break;
		}
	}	
		if(is_expr_empty) return ERR_SYN; 
		else return ret_val;
	}

