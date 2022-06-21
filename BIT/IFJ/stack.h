/*Hlavičkový soubor k modulu: stack
  Vypracoval: Ondřej Pavlacký (xpavla15) 
  Přeloženo gcc 7.4.0, standard C99
  Poslední úprava 11.12.2019, 2.BIT 2019/20  */

#ifndef __STACK_H__
#define __STACK_H__

#include<stdlib.h>
#include <stdio.h>
#include<stdbool.h>

#define MAX_SIZE 10

typedef struct stack {
	int top; //Vrchol zásovníku
	unsigned short *arr; //Zásobník
	int expand; //Kapacity zásobníku
} stack_t;

stack_t *stack_ctor(void); //vytvoření zásobníku a nastavení na NULL

void stack_push(stack_t *stack, unsigned short val); //Vložení prvku do zásobníku

unsigned short stack_top(stack_t *stack); //Vrací prvek na vrcholu zásobníku, jinak 0

bool stack_pop(stack_t *stack); //Uvolní ze zásobníku vrchní prvek

void stack_dtor(stack_t **stack); //Uvolňuje celý zásobník

#endif //__STACK_H__
