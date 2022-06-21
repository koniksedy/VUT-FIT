/*Modul: instrukční páska
  Vypracoval: Ondřej Pavlacký (xpavla15) 
  Přeloženo gcc 7.4.0, standard C99
  Poslední úprava 11.12.2019, 2.BIT 2019/20  */

#include "instr_tape.h"

//Vytvoření pásky
instr_tape_t* tape_ctor(void) { 

	instr_tape_t* struktura = (instr_tape_t*)malloc(sizeof(instr_tape_t)); //Alokace místa
	if(struktura== NULL){
		return NULL; //Pokud se nepovede alokace, vracím NULL
	}
	struktura->first=NULL;
	struktura->act=NULL;  //Nastavím vše na NULL
	struktura->last=NULL;
	return struktura; //Vracím ukazatel na nově vytvořenoui pásku
}

//Přidání prvku do instrukční pásky
void tape_append(instr_tape_t **tape, unsigned instr_code, void *addr1, void *addr2, void *addr3){ 
	
	instr_t *p= (instr_t*)malloc(sizeof(instr_t)); //Alokace místka pro prvek
	if(p == NULL){ 
			tape_dtor(tape); //Dealokace pásky
	}
	else{
		p->instr_code=instr_code;
		p->addr1=addr1;
		p->addr2=addr2;			//Přiřazení hodnot
		p->addr3=addr3;
		p->next=NULL;
		if((*tape)->first == NULL){ //Pokud je prvek první, tak nastaví aktivní a první
			(*tape)->first=p;
			(*tape)->act=(*tape)->first;
		}
		else (*tape)->last->next=p;
		
		(*tape)->last=p; //Každopádně přiřadí nový prvek na poslední místo
	}
}

//Čte pásku a nastavuje aktivní na další, vrací ukazatel na předchozí aktivní
instr_t* tape_read(instr_tape_t *tape){

	instr_t* odkaz_na_act= tape->act;
	if(tape->act != NULL){
		tape->act= tape->act->next;
	}
	return odkaz_na_act;
}

//Vrací ukazatel na aktivní
instr_t *tape_actual(instr_tape_t *tape)
{

	return tape->act;
}

//Vrací ukazatel na poslední
instr_t* tape_last(instr_tape_t *tape)
{  
    return tape->last;
}

//Posouvá se v pásce dál, bez toho, aby něco vracel
void tape_next(instr_tape_t *tape)
{  
    if(tape->act != NULL){
        tape->act= tape->act->next;
    }
}

//Vrací true pokud je páska aktivní, jinak false
bool tape_is_active(instr_tape_t *tape){

	return (tape->act != NULL)? true : false; 
}

//Vrací true pokud první něco je, jinak false
bool tape_activate(instr_tape_t *tape){

	return (tape->first != NULL)? (tape->act=tape->first, true) : false;
}

//Funkce na dealokci pásky
void tape_dtor(instr_tape_t **tape){

	(*tape)->act=(*tape)->first; //Nastavím aktivní na první
	if((*tape)->act != NULL) {	 //Postupně iteruji a posouvám se dál a uvolňuji členy
		instr_t *p=(*tape)->act->next;
		while((*tape)->act != (*tape)->last){
			free((*tape)->act);
			(*tape)->act=p;
			p=p->next;
		}
		free((*tape)->act);
	}
	free(*tape); //Nakonec uvolním pásku samotnou a nastavím ji na NULL
	*tape=NULL;
}

//Měním hodnotu tříadresného kódu a vracím true, jinak false
bool instr_change(instr_t *dest, unsigned instr_code, void *addr1, void *addr2, void *addr3){

	if(dest == NULL){
		return false;
	}
	else {
		dest->instr_code = instr_code;
		dest->addr1 = addr1;
		dest->addr2 = addr2;
		dest->addr3 = addr3;
		return true;
	}
}