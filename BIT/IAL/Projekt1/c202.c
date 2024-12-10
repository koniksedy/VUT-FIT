
/* ******************************* c202.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c202 - Zásobník znaků v poli                                        */
/*  Referenční implementace: Petr Přikryl, 1994                               */
/*  Vytvořil: Václav Topinka, září 2005                                       */
/*  Úpravy: Kamil Jeřábek, září 2019                                          */
/* ************************************************************************** */
/*
** Implementujte datový typ zásobník znaků ve statickém poli. ADT zásobník je
** reprezentován záznamem typu tStack. Statické pole 'arr' je indexováno
** do maximální hodnoty STACK_SIZE. Položka záznamu 'top' ukazuje na prvek
** na vrcholu zásobníku. Prázdnému zásobníku odpovídá hodnota top == -1,
** zásobníku s jedním prvkem hodnota 0, atd. Přesnou definici typů neleznete
** v hlavičkovém souboru c202.h.
**
** Implementujte následující funkce:
**
**    stackInit .... inicializace zásobníku
**    stackEmpty ... test na prázdnost zásobníku
**    stackFull .... test na zaplněnost zásobníku
**    stackTop ..... přečte hodnotu z vrcholu zásobníku
**    stackPop ..... odstraní prvek z vrcholu zásobníku
**    stackPush .... vloží prvku do zásobníku
**
** Své řešení účelně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

/**
 * Projekt vypracovat: Michal Šedý (xsedym02)
 */

#include "c202.h"

int solved;
int STACK_SIZE = MAX_STACK;

void stackError ( int error_code ){
/*   ----------
** Vytiskne upozornění, že došlo k chybě při práci se zásobníkem.
**
** TUTO FUNKCI, PROSÍME, NEUPRAVUJTE!
*/
	static const char* SERR_STRINGS[MAX_SERR+1] = {"Unknown error","Stack error: INIT","Stack error: PUSH","Stack error: TOP"};
	if ( error_code <= 0 || error_code > MAX_SERR )
		error_code = 0;
	printf ( "%s\n", SERR_STRINGS[error_code] );
	err_flag = 1;
}

void stackInit ( tStack* s ) {
/*   ---------
** Provede inicializaci zásobníku - nastaví vrchol zásobníku.
**
** V případě, že funkce dostane jako parametr s == NULL,
** volá funkci stackError(SERR_INIT).
*/
	if (s == NULL)
	{
		stackError(SERR_INIT);
		return;
	}

	s->top = -1;
}

int stackEmpty ( const tStack* s ) {
/*  ----------
** Vrací nenulovou hodnotu, pokud je zásobník prázdný, jinak vrací hodnotu 0.
*/
	return (s->top == -1) ? 1 : 0;
	  
}

int stackFull ( const tStack* s ) {
/*  ---------
** Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
*/
	return (s->top < STACK_SIZE-1) ? 0 : 1;
	 
}

void stackTop ( const tStack* s, char* c ) {
/*   --------
** Vrací znak z vrcholu zásobníku prostřednictvím parametru c.
** Tato operace ale prvek z vrcholu zásobníku neodstraňuje.
** Volání operace Top při prázdném zásobníku je nekorektní
** a vznikne volání funkce stackError(SERR_TOP). 
*/
	if (stackEmpty(s))
	{
		stackError(SERR_TOP);
		return;
	}

	*c = s->arr[s->top];
}


void stackPop ( tStack* s ) {
/*   --------
** Odstraní prvek z vrcholu zásobníku.
*/
	if(!stackEmpty(s))
	{
		s->top--;
	}
}


void stackPush ( tStack* s, char c ) {
/*   ---------
** Vloží znak na vrchol zásobníku. Pokus o vložení prvku do plného zásobníku
** je nekorektní a vytvoí volání procedury stackError(SERR_PUSH).
*/
	if (stackFull(s))
	{
		stackError(SERR_PUSH);
		return;
	}

	s->top++;
	s->arr[s->top] = c;	
}

/* Konec c202.c */
