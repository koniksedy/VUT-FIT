/**
 * ppm.h
 * Řešení IJC-DU1, příklad b), 16.3.2019
 * Autor: Michal Šedý, FIT
 * Přeloženo: gcc 5.4.0
 * Popis: Deklarace funkcí pro prási s PPM souborem.
 */

#ifndef PPM_H
#define PPM_H

struct ppm {
	unsigned xsize;
	unsigned ysize;
	char data[];		// RGB bajty, celkaem 3*xsize*ysize
};

/* FCE načte obsah PPM souboru do touto funkcí dynamicky alokované struktury.
   Při chybě formátu použije funkci warning_msg a vrátí NULL. */
struct ppm * ppm_read(const char * filename);

/* FCE uvolní paměť dynamicky alokaovanou v ppm_read */
void ppm_free(struct ppm *p);

#endif
