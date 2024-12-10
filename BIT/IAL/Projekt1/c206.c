
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

/**
 * Ukol vypracolal: Michal Šedý (xsedym02)
 */


#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    
    // V prázdném seznamu budou všechny prvky NULL
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

	// Odstanujeme první prvek, dokud není seznam prázdný	
	while(L->First != NULL)
	{
		DLDeleteFirst(L);
	}
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	// Alokace paměti a její kontrola
	tDLElemPtr new = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if (new == NULL)			
	{
		DLError();
		return;
	}

	// Nastavení hodnot nového prvku
	new->data = val;
	new->lptr = NULL;
	new->rptr = L->First;

	// Vložení prvku do seznamu, dva přístupy, pro prázdný a neprázdný seznam.
	if (L->First != NULL)			
	{
		L->First->lptr = new;
	}	
	else							
	{
		L->Last = new;
	}

	L->First = new;	
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	
	// Alokace paměti a její kontrola
	tDLElemPtr new = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if (new == NULL)
	{
		DLError();
		return;
	}

	// Nastavení hodnot nového prvku
	new->data = val;
	new->rptr = NULL;
	new->lptr = L->Last;

	// Vložení prvku do seznamu, dva přístupy, pro prázdný a neprázdný seznam.
	if (L->First != NULL)
	{
		L->Last->rptr = new;
	}	
	else
	{
		L->First = new;
	}

	L->Last = new;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;	
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	// Kontrola obsazení seznamu
	if (L->First == NULL)
	{
		DLError();
		return;
	}

	*val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	
	// Kontrola obsazení seznamu
	if (L->First == NULL)
	{
		DLError();
		return;
	}
	
	*val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

	// Ukončíme funkci, pokud je seznam prázdný.
	if (L->First == NULL)
	{
		return;
	}

	// Pokud byl první prvek aktivní, ztrácí seznam aktivitu.
	if (L->Act == L->First)
	{
		L->Act = NULL;
	}

	// Odpojení prvku ve seznamu. Dva postupy: pro seznam s jedním a více prvky.
	tDLElemPtr tmpPtr = L->First;
	if(L->First == L->Last)			
	{
		L->First = NULL;
		L->Last = NULL;
	}
	else							
	{
		L->First = L->First->rptr;
		L->First->lptr = NULL;
	}

	// Dealokace
	free(tmpPtr);	
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 

	// Ukončíme funkci, pokud je seznam prázdný.
	if (L->First == NULL)
	{
		return;
	}

	// Pokud byl poslední prvek aktivní, ztrácí seznam aktivitu.
	if (L->Last == L->Act)
	{
		L->Act = NULL;
	}

	// Odpojení prvku ve seznamu. Dva postupy: pro seznam s jedním a více prvky.
	tDLElemPtr tmpPtr = L->Last;
	if (L->First == L->Last)
	{
		L->First = NULL;
		L->Last = NULL;
	}
	else
	{
		L->Last = L->Last->lptr;
		L->Last->rptr = NULL;
	}

	// Dealokace
	free(tmpPtr);
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

	if (L->Act == L->Last || !DLActive(L))
	{
		return;
	}

	// Odpojení prvku ze seznamu. Dva postupy: pro prvek uvnitř seznamu a na kraji.
	tDLElemPtr tmpPtr = L->Act->rptr;
	if (tmpPtr != L->Last)
	{
		L->Act->rptr = tmpPtr->rptr;
		tmpPtr->rptr->lptr = L->Act;
	}
	else
	{
		L->Act->rptr = NULL;
		L->Last = L->Act;
	}
	
	// Dealokace
	free(tmpPtr);
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	
	if (L->Act == L->First || !DLActive(L))
	{
		return;
	}

	// Odpojení prvku ze seznamu. Dva postupy: pro prvek uvnitř seznamu a na kraji.
	tDLElemPtr tmpPtr = L->Act->lptr;
	if (tmpPtr != L->First)
	{
		L->Act->lptr = tmpPtr->lptr;
		tmpPtr->lptr->rptr = L->Act;
	}
	else
	{
		L->Act->lptr = NULL;
		L->First = L->Act;
	}

	// Dealokace
	free(tmpPtr);
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	// Kontrola, zda existuje aktivní prvek
	if (!DLActive(L))
	{
		return;
	}

	// Alokace paměti a její kontrola
	tDLElemPtr new = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if (new == NULL)
	{
		DLError();
		return;
	}

	// Nastavení hodnoty nového prvku
	new->data = val;

	// Zařazení prvku do seznamu. Dva přístupy pro připojení prvku:
	// dovnitř seznamu, nebo na okraj.
	if (L->Act != L->Last)
	{
		new->lptr = L->Act;
		new->rptr = L->Act->rptr;
		L->Act->rptr->lptr = new;
		L->Act->rptr = new;
	}
	else
	{
		new->rptr = NULL;
		new->lptr = L->Act;
		L->Last = new;
		L->Act->rptr = new;
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	// Kontrola, zda existuje aktivní prvek
	if (!DLActive(L))
	{
		return;
	}

	// Alokace paměti a její kontrola
	tDLElemPtr new = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if (new == NULL)
	{
		DLError();
		return;
	}

	// Nastavení hodnoty nového prvku
	new->data = val;

	// Zařazení prvku do seznamu. Dva přístupy pro připojení prvku:
	// dovnitř seznamu, nebo na okraj.
	if (L->Act != L->First)
	{
		new->lptr = L->Act->lptr;
		new->rptr = L->Act;
		L->Act->lptr->rptr = new;
		L->Act->lptr = new;
	}
	else
	{
		new->lptr = NULL;
		new->rptr = L->Act;
		L->First = new;
		L->Act->lptr = new;
	}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

	if (!DLActive(L))
	{
		DLError();
		return;
	}

	*val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	
	if (DLActive(L))
	{
		L->Act->data = val;
	}
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	
	if (DLActive(L))
	{
		L->Act = L->Act->rptr;
	}
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if (DLActive(L))
	{
		L->Act = L->Act->lptr;
	}
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return (L->Act == NULL) ? 0 : 1;
}

/* Konec c206.c*/
