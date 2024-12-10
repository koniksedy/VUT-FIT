/**
 * tail.c
 * Řečení IJC-DU2, příklad aa), 28.3.2019
 * Autor: Michal Šedý, FIT
 * Přeloženo: gcc 7.4.0
 * Popis: Zdrojový soubor programu tail, naspsného v jazyce c,
 *		  který ze zadanéhoh vstupního souboru vytiskne posledních 10 řádků.
 *		  Není-li zadán vstupní soubor, čte ze stdin. Je-li programu zadán
 *		  parametr -n číslo, bude se tisknout tolik posledních řádků,
 *		  kolik je zadáno parametrem 'číslo' > 0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define LIM 1023		// Limit maximální délky řádku

/**
 * FCE pro výpis chyby a ukončení programu
 */
void error_exit(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "tail: ");
	vfprintf(stderr, fmt, args);
	exit(1);
}

/**
 * FCE pro detekci znau +
 * vstup - chr - znak pro otestování
 * Výstup - fce vrasí true pokud bylo detekováno plus a + přenastaví na 0, jinak vrací false
 */
bool is_plus(char *chr)
{
	if(*chr == '+')
	{
		*chr = '0';
		return true;
	}
	else
		return false;
}

/**
 * FCE pro převod řetězce na číslo a kontrola, zda je větší než 0
 * Vstup - str - řetězec na převod
 * Výstup - fce vrací převedené číslo, v případě chyby volá error_exit
 */
unsigned parse_stringtoint(char *str)
{
	char *end = NULL;
	long n = strtol(str, &end, 10);

	if(*end || n < 1)
		error_exit("chybný počet řádků: \"%s\"\n", str);

	return (unsigned)n;
}

/**
 * FCE pro přechod n-krát na nový řádek.
 * Vstup - stream
 *		 - n - počet odřídkování
 */
void new_line(FILE* stream, unsigned n)
{
	int chr;

	for(unsigned i = 0; i < n; i++)
	{
		while((chr=fgetc(stream)) != '\n' && chr != EOF)
		;
	}
}

int main(int argc, char *argv[])
{
	FILE *stream = stdin;
	unsigned n = 10;			// Standartní počet vypisovaných řádků
	unsigned iter = 0;			// Iterátor
	int chr;					// Pro výpis v případě -n +cislo
	bool plus = false;			// Pro detekci -n +cislo	
	bool fail_lenght = false;	// Pro detekci dlouhých řádků
	char *lines;				// Cyklický seznam

	// Kontrola počtu argumentů
	if(argc > 4)
	{
		error_exit("příliš mnoho argumentů.\n");
	}
	
	// NAČÍTÁNÍ ČÍSLA při PŘEPÍNAČI -n
	// argc == 3 || 4 -> /tail -n cislo ...
	if(argc == 3 || argc == 4)
	{
		// Kontrola zda je zadán přepínač -n
		if(strcmp(argv[1], "-n"))
		{
			error_exit("neplatný přepínač \"%s\"\n", argv[1]);
		}

		// Kontrola parametru + např. +10
		plus = is_plus(&argv[2][0]);

		// Převod stringu na číslo 
		n = parse_stringtoint(argv[2]);
	}
	
	// NAČÍTÁNÍ SOUBORU
	// argc == 2 -> ./taili soubor
	// argc == 4 -> ./tail -n cislo soubor
	if(argc == 2 || argc == 4)
	{	
		stream = fopen(argv[argc-1], "r");
		if(stream == NULL)
		{
			error_exit("\'%s\' nelze otevřít pro čtení: Adresář nebo soubor neexistuje\n", argv[argc-1]);
		}
	}

	// A) VÝPIS POSLEDNÍCH N ŘÁDKŮ
	// ---------------------------
	if(!plus)
	{
		lines = calloc(1, n*(LIM+2));

		if(lines == NULL)
		{
			fclose(stream);
			error_exit("nepodařila se alokace paměti.\n");
		}
	
		// Načítání
		while(fgets(&lines[(iter%n)*(LIM+2)], LIM+2, stream) != NULL)
		{
			// Kontorola překročení limitu
			if(strlen(&lines[(iter%n)*(LIM+2)]) > LIM && lines[(iter%n)*(LIM+2)+LIM] != '\n')
			{
				lines[(iter%n)*(LIM+2)+LIM] = '\n';
				new_line(stream, 1);

				if(!fail_lenght)
				{
					fail_lenght = true;
					fprintf(stderr,"tail: Řádek je delší než %d znaků. Pokračuji dále se zkrácenými.\n", LIM);
				}
			}
			iter++;
		}

		// Výpis
		for(unsigned i = 0; i < n ; i++, iter++)
		{
			printf("%s", &lines[(iter%n)*(LIM+2)]);
		}

		free(lines);
	}
	// B) VÝPIS OD ŘÁDKU N
	// -------------------
	else
	{
		// Odřádkování
		new_line(stream, n-1);

		// Výpis
		while((chr = fgetc(stream)) != EOF)
		{
			putchar(chr);
		}
	}

	fclose(stream);
	return 0;
}
