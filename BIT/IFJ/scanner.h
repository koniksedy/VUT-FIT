/* Autor: Radek Sedlar (xsedlaij)
 * Spoluautori:  Ondrej Pavlacky (xpavla15), Michal Sedy (xsedym02)
 * Rocnik: 2.BIT 2019/2020
 */

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include<stdlib.h>
#include<string.h>
#include<string.h>
#include<stdbool.h>
#include "string_dynamic.h"
#include "stack.h"
#include "error.h"
#include <stdio.h>
stack_t* GLOBALNI_STACK;
long int fileCharLenght;

typedef enum {
    UNDEFINED,      // neni prirazeno
    EOF_t,            // konec vstupu
    INT,            // jednoduchy int
    FLOAT,          // float
    COMMENT,        // komentar
    STRING,         // string
    IDENTIFIER,     // identifikator
    OPERATOR_PLUS,          // +
    OPERATOR_MINUS,         // -
    OPERATOR_KRAT,          // *
    OPERATOR_DELENO_INT,    // //
    OPERATOR_DELENO,        // /
    OPERATOR_VETSI,         // >
    OPERATOR_MENSI,         // <
    OPERATOR_VETSI_ROVNO,   // >=
    OPERATOR_MENSI_ROVNO,   // <=
    OPERATOR_ROVNOST,       // ==
    OPERATOR_NEROVNOST,     // !=
    ASSIGN,         // =
    DEF,            // klicove slovo
    ELSE,           // klicove slovo
    IF,             // klicove slovo
    NONE,           // klicove slovo
    PASS,           // klicove slovo
    RETURN,         // klicove slovo
    WHILE,          // klicove slovo

    EOL_t,            // konec radku '\n'
    INDENT,         // vetsi odsazeni nez predtim
    DEDENT,         // mensi odsazeni nez predtim
    COMMA,          // carka ','
    RIGHT_PAR,      // prava zavorka ')'
    LEFT_PAR,       // leva zavorka '('
    COLON,          // dvojtecka ':'
    GAP,            // mezera ' '
    INPUTS,
    INPUTI,
    INPUTF,
    PRINT,
    LEN,
    SUBSTR,
    ORD,
    CHR,
    FUNCTION,
    DOLLAR
}typTokenu;

typedef union{
    int i;
    double fl;
    char ch;
    string_t *str;
}token_data_t;

typedef struct{
    unsigned char type;
    token_data_t *data;
}token_t;

token_t *token_ctor();

void token_dtor(token_t**  token);

/**
 *@brief vycisti vsechnu pamet kterou ma token zarezervovanou
 *@param token     token ktery ma byt vycisten
 *@return          vraci VOID
 */
void token_clean(token_t* token);

bool token_get_next(token_t* token);

bool append_array(char* arr, const char character);

#endif // __SCANNER_H__
