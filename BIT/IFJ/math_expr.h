/*Hlavičkový soubor k modulu: math_expr
  Vypracoval: Ondřej Pavlacký (xpavla15) 
  Přeloženo gcc 7.4.0, standard C99
  Poslední úprava 11.12.2019, 2.BIT 2019/20  */

#include "instr_tape.h"
#include "scanner.h"
#include "string_dynamic.h"
#include "string_generator.h"
#include "prec_stack.h"
#include "parse.h"

//Kontrola tokenu na konec radku/souboru
int check_type(htab_t* global_table, string_t* func_name, token_t* token); 

bool is_token_dollar(token_t *token);

//Probiha kontrola podle precedencni tabulky a nasledne tvoreni instrukci a vypoctu
int expr(htab_t *global_table, string_t *func_name, instr_tape_t **tape, token_t *token, htab_item_t **result,token_t* token_first, bool only_check);