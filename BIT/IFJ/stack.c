/*Modul: stack
  Vypracoval: Ondřej Pavlacký (xpavla15) 
  Přeloženo gcc 7.4.0, standard C99
  Poslední úprava 11.12.2019, 2.BIT 2019/20  */

#include "stack.h"

//Vytvoření zásobníku
stack_t *stack_ctor(void){

	stack_t* zasobnik = (stack_t*)malloc(sizeof(stack_t)); //Alokace místa
	if(zasobnik == NULL){
		return NULL;
	}
	unsigned short *field = malloc(MAX_SIZE*(sizeof(char))); //Alokace místa
	zasobnik->expand = MAX_SIZE;
	zasobnik->arr = field;			//Přiřazení hodnot
	zasobnik->top = -1;
	return zasobnik;
}

//Vložení prvku na zásobník
void stack_push(stack_t *stack,unsigned short val){

    unsigned short *tmp_ptr;
    	if(stack->top >= MAX_SIZE -1){  //Rozšíření zásobníku
			stack->expand+=5;
			tmp_ptr= realloc(stack->arr, stack->expand);
            if(tmp_ptr == NULL) free(stack->arr);   // tím se vynutí další if
            else stack->arr = tmp_ptr;
	}
	if(stack->arr == NULL){
		free(stack);
		stack=NULL;
	}
	stack->top++; //Zvýšení hodnoty počtu prvků
	stack->arr[stack->top]=val; //Přiřažení hodnoty na vrchol zásobníku
}

//Vrací hodnoty na vrchu zásobníku
unsigned short stack_top(stack_t *stack){ 

	return (stack->top == -1)? 0 : stack->arr[stack->top];
}

//Odebrání hodnoty z vrcholu zásobníku
bool stack_pop(stack_t *stack){

	return (stack->top == -1)? false : (stack->top--, true);
}

//Uvolnění zásobníku
void stack_dtor(stack_t **stack){

	free((*stack)->arr);
	free(*stack);
	*stack = NULL;
}
