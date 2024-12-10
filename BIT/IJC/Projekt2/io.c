/**
 * io.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 * Popis: Modul pro zajištění čtení slova ze souboru.
 */

#include <stdio.h>
#include <ctype.h>
#include "io.h"

/**
 * Funkce čte jednoslovo ze souboru f do zadaného pole znaků.
 * Vrací délku slova (z delších slov načte prvních max-1 znaků a zbytek přeskočí a vrátí max),
 * vrací EOF, pokud je konec souboru.
 * V případě fatální chyby vrací -2;
 */
int get_word(char *s, int max, FILE *f)
{
	// Kontrola parametnů
	if(s == NULL || max < 1 || f == NULL)
	{
		return -2;
	}

	int chr;
	int i = 0;
	while(i < max)
	{
		chr = fgetc(f);
		if(chr == EOF)
		{
			s[i] = '\0';	// projistoru
			return EOF;
		}
		
		// jedná se pouze o bílý znak načítáme znovu
		if(isspace(chr) && i == 0)
		{
			continue;
		}
		// slovo bylo ukončeno bílým znakem
		if(isspace(chr) && i > 0)
		{
			break;
		}

		s[i] = chr;
		i++;
	}

	if(i == max)
	{
		// Přeskočení na konec slova
		while(!isspace(fgetc(f)))
			;
		s[i-1] = '\0';
	}
	else
	{
		s[i] = '\0';
	}

	return i; 
}
