/**
 * Modul pro parser - syntaktickou analýzu
 * Napsal: Michal Šedý <xsedym02>
 * Přeloženo: gcc 7.4.0
 * Standard: C99
 * Poslední změna: 05.12.19
 */

#ifndef  __PARSE_H__
#define  __PARSE_H__

#include <stdbool.h>
#include "symtable.h"
#include "math_expr.h"
#include "instr_tape.h"
#include "scanner.h"
#include "param.h"
#include "string.h"
#include "error.h"

#define PLACE_HOLDER 100

// Makro pro porovnání typu tokenu s typen konkrétním
#define CMP_TOKEN_CONST(t,num) (t->type == num)

// Makro zařídí dačtení dalšího tokenu a v případě chyby (interní, nedo lexikální) ji obslouží.
// Nepříjmá tokeny GAP, ty zahodí a hledá další
#define GET_NEXT_TOKEN(t) do {if(!token_get_next(t) || t->type == UNDEFINED){error_msg("CHYBA při získávání tokenu\n");\
																token_dtor(&t);\
																exit(ERR_LEX);}} while(t->type == GAP)
// Kontroluje zda návratová hodnota volané funkce byka OK
#define CHACK_RET(r, name) do {if(r > ERR_OK){return r; string_dtor(&name);}} while(0)

// Makro zařídí kontrolu aktuálního typu tokenu a typem kontrétním a načtení tokenu dalšího.
// V případě neúspěšného načítání chybu obslouží.
#define CHACK_TOKEN_AND_GET_NEXT(t, cmp, rul)	do {if(!CMP_TOKEN_CONST(t, cmp)){error_msg("%s. Pravidlo %s\n", MSG_SYN, rul);\
																			string_dtor(&name);\
																			return ERR_SYN;}} while(0); \
																			GET_NEXT_TOKEN(t)

// Makro zahazuje veškeré tokeny až po hledaný.
#define DELETE_TO(end) do{GET_NEXT_TOKEN(token);}while(!CMP_TOKEN_CONST(token, end) && !CMP_TOKEN_CONST(token, EOL_t) && !CMP_TOKEN_CONST(token, EOF_t));

// Makro zkoumá, zda je je typ tokenu operátor.
#define IS_OPERATOR(t) (t->type <= OPERATOR_NEROVNOST && t->type >= OPERATOR_PLUS)

// Makro vloží na konec instrukční pásky novou instrukci a v případě chyby ji obslouží.
#define MAKE_INSTR(ins_tap, c, a1, a2, a3) do{tape_append(ins_tap, c, a1, a2, a3); if((*ins_tap) == NULL) {\
																					error_msg("CHYBA appendu pasky\n%s", MSG_INTER_RUN);\
																					return ERR_INTER_RUN;}} while(0);

// Enumerace typu příkazů
// vkádaných do instrukční pásky.
enum {
	IF_COM,
	WHILE_COM,
	DEF_COM,
	PRINT_CALL,
	INPUTS_CALL,
	INPUTI_CALL,
	INPUTF_CALL,
	LEN_CALL,
	SUBSTR_PAR,
	SUBSTR_CALL,
	ORD_CALL,
	CHR_CALL,
	DOK_COM,
	CPY_COM,
	CMP_NEQ,
	CMP_EQ,
	CMP_LES,
	CMP_LEQ,
	CMP_GRE,
	CMP_GEQ,
	MUL_COM,
	DIV_COM,
	DIV_INT_COM,
	ADD_COM,
	SUB_COM,
	PARAM,
	CALL,
	DEF_VAR
};

/**
 * Funkce syntaktické anatýzy.
 * ----------------------------
 * global_table - globální tabulka
 * main_tape - instrukční páska
 * 
 * V případě úspěchu vrací funkce 0 v jiném kód chyby.
 */
int parse(htab_t *global_table, instr_tape_t **main_tape);

#endif
