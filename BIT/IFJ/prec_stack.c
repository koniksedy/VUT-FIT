#include "prec_stack.h"
#define INFO ;// fprintf(stderr, "---------------------------------\nmodul-----> %s\nfunkce ------> %s\nline-----> %d\n---------------------------------\n", __FILE__, __func__, __LINE__);
prec_stack_t *prec_stack_ctor(void)
{
	INFO;
	prec_stack_t *new = (prec_stack_t*)malloc(sizeof(prec_stack_t));
	if(new == NULL)
	{
		error_msg("CHYBA při alokaci prec_stack.\n");
		return NULL;
	}

	new->top = (prec_stack_item_t*)malloc(sizeof(prec_stack_item_t));
	if(new->top == NULL)
	{
		free(new);
		error_msg("CHYBA při alokaci itemu prec_stack.\n");
		return false;
	}

	htab_item_t *buttom_item = (htab_item_t*)malloc(sizeof(htab_item_t));
	if(buttom_item == NULL)
	{
		free(new->top);
		free(new);
		error_msg("CHYBA při alokaci buttom v prec_stack.\n");
		return NULL;
	}
	buttom_item->type = DOLLAR;

	new->top->pseudo = true;
	new->top->terminal = true;
	new->top->prew = NULL;
	new->top->data = buttom_item;
	new->buttom = new->top;
	return new;
}


bool prec_stack_push(prec_stack_t *stack, htab_item_t *item, bool term, bool pseudo_type)
{INFO;
	if(stack == NULL || item == NULL)
	{
		error_msg("CHYBA v prec_stack_push byly předány NULL hodnoty.\n");
		return false;
	}

	prec_stack_item_t* new = (prec_stack_item_t*)malloc(sizeof(prec_stack_item_t));
	if(new == NULL)
	{
		error_msg("CHYBA při alokaci itemu prec_stack.\n");
		return false;
	}

	new->prew = stack->top;
	new->data = item;
	new->terminal = term;
	new->pseudo = pseudo_type;

	stack->top = new;

	return true;
}

bool is_terminal(prec_stack_item_t *item)
{INFO;
	return item->terminal;
}

htab_item_t *prec_stack_top(prec_stack_t *stack)
{INFO;
	return stack->top->data;
}

htab_item_t *prec_stack_top_terminal(prec_stack_t *stack)
{INFO;
	prec_stack_item_t *terminal = stack->top;
	while(!is_terminal(terminal))
	{
		terminal = terminal->prew;
	}

	return terminal->data;	
}

bool prec_stack_empty(prec_stack_t *stack)
{INFO;
	return (stack->top == stack->buttom);
}

bool prec_stack_pop(prec_stack_t *stack)
{INFO;
	if(stack->top == stack->buttom)
	{INFO;
		return false;
	}

	prec_stack_item_t *tmp = stack->top;
	stack->top = stack->top->prew;
	if(tmp->pseudo)
	{
		free(tmp->data);
		free(tmp);
	}

	return true;
}

void prec_stack_dtor(prec_stack_t *stack)
{INFO;
	if(stack == NULL)
	{
		error_msg("POZOR, chceš udělat free neexistujícího prec_stack.\n");
		return;
	}

	prec_stack_item_t *tmp = stack->top;
	prec_stack_item_t *to_del = tmp;

	while(stack->top != stack->buttom)
	{
		tmp = tmp->prew;
		if(to_del->pseudo)
		{
			free(to_del->data);
			free(to_del);
		}
	}
	free(tmp->data);
	free(tmp);
	free(stack);
}