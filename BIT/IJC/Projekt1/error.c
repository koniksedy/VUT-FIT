/**
 * error.c
 * Řešení IJC-DU1, příklad a), b), 13.3.2019
 * Autor: Michal Šedý, FIT
 * Přeloženo: gcc 5.4.0
 * Popis: Definice funkcí pro výpis chybových hlážení
 *        a funkce error_exit ukončuje program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"

void warning_msg(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "CHYBA: ");
	vfprintf(stderr, fmt, args);
	va_end(args);
}

void error_exit(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "CHYBA: ");
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
}
