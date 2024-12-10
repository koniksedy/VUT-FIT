/**
 * ppm.c
 * Řešení IJC-DU1, příklad b), 16.3.2019
 * Autor: Michal Šedý
 * Přeloženo: gcc 5.4.0
 * Popis: Modul pro načtení PPM souboru do vytvořené struktury
 * 	      a dealokaci struktury a daty. Pokud by během načítání došlo k jakékoliv chybě
 * 		  (soubor se nepodařilo otevřít, chyby formátování souboru, chyba alokace...),
 *		  zavolá se funkce warning_msg, která vypíše příslušnou chybovou hlášku.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm.h"
#include "error.h"

struct ppm * ppm_read(const char * filename)
{
	FILE *img;
	struct ppm *p = NULL;
	unsigned xsize, ysize, color;
	char p6[3] = {0,};

	// Otevření a kontrola
	img = fopen(filename, "r");
	if(img == NULL)
	{
		warning_msg("Nepodařilo se otevřít soubor %s.\n", filename);
		return NULL;
	}

	// Kontrola zda-li se jedná o P6
	if(fscanf(img, "%2s\n", p6) != 1 || strcmp(p6, "P6"))
	{
		warning_msg("Soubor %s není ve formátu P6.\n", filename);
		fclose(img);
		return NULL;
	}

	// Načtení a kontrola velikosti
	if(fscanf(img, "%u %u\n", &xsize, &ysize) != 2)
	{
		warning_msg("Chybné rozměry v souboru %s.\n", filename);
		fclose(img);
		return NULL;
	}

	// Kontrola počtu barev
	if(fscanf(img, "%u\n", &color) != 1 || color != 255)
	{
		warning_msg("V souboru %s je obsaženo nesprávné množství barev.\n", filename, color);
		fclose(img);
		return NULL;
	}

	// Alokace a kontrola paměti pro strukturu obsahujici PPM
	p = malloc(sizeof(struct ppm) + (3*xsize*ysize));
	if(p == NULL)
	{
		warning_msg("Nepodařila se alokace paměti pro PPM strukturu.\n");
		fclose(img);
		return NULL;
	}

	// Načtení dat do PPM struktuky
	p->xsize = xsize;
	p->ysize = ysize;
	if(fread(&p->data, 1, 3*xsize*ysize, img) != 3*xsize*ysize)
	{
		warning_msg("Soubor %s neobsahuje potřebné množství dat.\n", filename);
		fclose(img);
		ppm_free(p);
		return NULL;
	}
	// Kontrola zakončení EOF
	if(fgetc(img), fgetc(img) != EOF)
	{
		warning_msg("Soubor %s více dat, než je očekávano.\n", filename);
		fclose(img);
		ppm_free(p);
		return NULL;
	}

	fclose(img);
	return p;

}

/* Uvolní paměť dynamicky alokovanou v ppm_read. */
void ppm_free(struct ppm *p)
{
	free(p);
}
