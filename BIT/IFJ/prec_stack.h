#ifndef __PREC_STACK_H__
#define __PREC_STACK_H__

#include <stdbool.h>
#include "string_dynamic.h"
#include "scanner.h"
#include "error.h"
#include "symtable.h"
#include "param.h"

typedef struct prec_stack_item {
	htab_item_t *data;
	struct prec_stack_item *prew;
	bool terminal;
	bool pseudo;
} prec_stack_item_t;

typedef struct {
	prec_stack_item_t *top;
	prec_stack_item_t *buttom;
} prec_stack_t;

prec_stack_t *prec_stack_ctor(void);

bool prec_stack_push(prec_stack_t *stack, htab_item_t *item, bool term, bool pseudo_type);

htab_item_t *prec_stack_top(prec_stack_t *stack);

htab_item_t *prec_stack_top_terminal(prec_stack_t *stack);

bool prec_stack_empty(prec_stack_t *stack);

bool prec_stack_pop(prec_stack_t *stack);

void prec_stack_dtor(prec_stack_t *stack);


#endif