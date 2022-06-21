/*Hlavičkový soubor k modulu: instrukční páska
  Vypracoval: Ondřej Pavlacký (xpavla15) 
  Přeloženo gcc 7.4.0, standard C99
  Poslední úprava 11.12.2019, 2.BIT 2019/20  */

#ifndef __INSTR_TAPE_H__
#define __INSTR_TAPE_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct t_instr_t {
	unsigned int instr_code; //Kodove oznaceni instrukce #define
	void *addr1;
	void *addr2;
	void *addr3;	//Tri adresy operandu
	struct t_instr_t *next;
} instr_t;

typedef struct {
	instr_t *first;	//Ukazatel na prvni prikaz pasky
	instr_t *act;	//Ukazatel na prave vykonavany prikaz
	instr_t *last;	//Ukazatel na posledni, pro append
} instr_tape_t;

instr_tape_t *tape_ctor(void); //Vytvoreni instrukcni pasky, nastaveni na NULL

void tape_append(instr_tape_t **tape, unsigned instr_code, void *addr1, void *addr2, void *addr3);  //Vlozeni prvku na konec instrukcni pasky, vhodne nastaveno 
																									//pri vkladani prvniho co dalsich parametru
instr_t* tape_last(instr_tape_t *tape); //Vraci ukazatel na posledni prvek pasky

void tape_next(instr_tape_t *tape); //Nastavuje aktivni prvek na dalsi, probehne kontrola posledniho

instr_t *tape_read(instr_tape_t *tape); //Nastaveje aktivini na dalsi, vraci ukazatel na predchozi aktivni

instr_t *tape_actual(instr_tape_t *tape); //Vraci uakzatel na naktivni

bool tape_is_active(instr_tape_t *tape); //Vraci true v pripade ze paska je aktivni, jinak false

bool tape_activate(instr_tape_t *tape); //Aktivuje pasku a vraci true, že se aktivace povedla, jinak false

void tape_dtor(instr_tape_t **tape); //Dealokuje pásku a nastavuje ji na NULL

bool instr_change(instr_t *dest, unsigned instr_code, void *addr1, void *addr2, void *addr3); //Prepisuje triadresny kod instrukce, vraci true

#endif //__INSTR_TAPE_H__