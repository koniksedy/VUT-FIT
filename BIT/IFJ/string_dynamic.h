/* Autor: Radek Sedláø (xsedlaij)
 * Spoluaktori:  Ondrej Pavlacky (xpavla15), Michal Sedy (xsedym02)
 * Rocnik: 2.BIT 2019/2020
 *
 *
 *
 */

#ifndef __STRING_DYNAMIC_H__
#define __STRING_DYNAMIC_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string.h>
#include<stdbool.h>


#define BUDGET 3

typedef struct{
    int len;
    int free_spac;
    char* data;
}string_t;

string_t* string_ctor();

void string_put_char(string_t** stringPtr, char pridavanyChar);

bool string_realoc(string_t* stringPtr);

void string_clean(string_t** stringPtr );

unsigned int string_len(string_t* stringPtr);

bool string_cmp_string(string_t* stringOnePtr, string_t* stringTwoPtr);

bool string_cmp_constant_string(string_t* stringPtr, char* stringTwoPtr);

char* string_get(string_t* stringPtr);

bool string_cpy(string_t* destStringPtr, string_t* stringTwoPtr);

bool string_cat(string_t* destStringPtr, string_t* stringTwoPtr);

void string_dtor(string_t** stringPtr);

#endif // __STRING_DYNAMIC_H__
