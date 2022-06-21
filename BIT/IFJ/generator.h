#include "parse.h"
#include "symtable.h"
#include "math_expr.h"
#include "string_dynamic.h"
#include "string_generator.h"
#include "prec_stack.h"
#include "instr_tape.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Funkce pro překlad celé instrukční pásky do IFJcode19
 * -----------------------------------------------------
 * main_tape - páska pro přeložení
 *
 * Funkce vrací v případě úspěchu 0, v opačném případě kód chyby.
 */
int compile(instr_tape_t* main_tape);
