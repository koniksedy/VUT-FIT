/*Modul: error
  Vypracoval: Ondřej Pavlacký (xpavla15) 
  Přeloženo gcc 7.4.0, standard C99
  Poslední úprava 11.12.2019, 2.BIT 2019/20  */

#include "error.h"

//Funkce na výpis erroru
void error_msg(const char *fmt, ...){ 

	va_list args; //Funkce přijme více parametrů
	va_start(args, fmt);
	fprintf(stderr, "ERROR: "); //Výpis na strerr
	vfprintf(stderr, fmt, args);
	va_end(args); //Ukončení přijímání
}