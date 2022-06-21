/*Hlavičkový soubor k modulu: error
  Vypracoval: Ondřej Pavlacký (xpavla15) 
  Přeloženo gcc 7.4.0, standard C99
  Poslední úprava 11.12.2019, 2.BIT 2019/20  */

#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdio.h>
#include <stdarg.h>

//Definice typů errorů
enum ERR_TYPE { 
	ERR_OK,
	ERR_LEX,
	ERR_SYN,
	ERR_SEM_DEF,
	ERR_TYPE_MISMATCH,
	ERR_PARAMS_COUNT,
	ERR_SEM_ELSE,
	ERR_ZERO_DIV = 9,
	ERR_INTER_RUN = 99
};

//Definice chybových hlášek
#define MSG_LEX "Chyba v programu v ramci lexikalni analyzy\n"
#define MSG_SYN "Chyba v programu v ramci syntakticke analyzy\n"
#define MSG_SEM_DEF "Sematicka chyba v programu - promenna/funkce\n"
#define MSG_TYPE_MISMATCH "Sematicka/behova chyba typove kompatibility\n"
#define MSG_PARAMS_COUNT "Sematicka chyba v programu - spatny pocet paramatetru\n"
#define MSG_SEM_ELSE "Ostatni sematicke chyby\n"
#define MSG_ZERO_DIV "Behova chyba deleni nulou\n"
#define MSG_INTER_RUN "Interni chyba prekladace tj. neovlivnena vstupnim programem\n"

//Funkce na výpis erroru na stderr
void error_msg(const char *fmt, ...);

#endif //__ERROR_H__