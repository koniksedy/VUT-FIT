/*Modul: symtable
  Vypracoval: Ondřej Pavlacký (xpavla15) 
  Přeloženo gcc 7.4.0, standard C99
  Poslední úprava 11.12.2019, 2.BIT 2019/20  */

#include "symtable.h"

//Funkce pro vyhledávání v tabulce
unsigned int htab_hash_function(char *str){
	
	uint32_t h=0;
	const unsigned char *p;
	for(p=(const unsigned char*)str; *p!='\0'; p++){
		h= 65599*h+*p; //Číslo je tam kvůli efektivnímu výpočtu adresy
	}
	return (h % HASH_SIZE);
}

//Vytvoří novou tabulku	
htab_t *htab_ctor(void){
	
	htab_t *t = malloc(sizeof(htab_t)+ sizeof(htab_item_t*) * (HASH_SIZE)); //Alokace místa
	t->params = NULL;
	t->params_cnt = 0;
	int i = 0;				//Nastavuje všude na NULL/0
	while(i <HASH_SIZE){
	t->ptr[i]= NULL;
	i++;
	}
	return (t == NULL)? NULL : t;  //Vrací ukazatel na tabulku
}

//Vkládá prvek do tabulky
bool htab_insert(htab_t *table, string_t *key, token_t *token){

	char *tmp_key = string_get(key); //Vygenerujeme si string pro uložení do tabulky 
	htab_item_t *zaloha = table->ptr[htab_hash_function(tmp_key)];
	htab_item_t *new = NULL;
	htab_item_t *ukazatel = htab_get_item(table, key);
	if(ukazatel != NULL){
		return false; //Uvidime jestli je budeme menit nebo ne
	}
	else{
		new = table->ptr[htab_hash_function(tmp_key)];
		if((new = malloc(sizeof(htab_item_t))) == NULL) return false; 
		
		if(token->type == INT){ //Int
			new->it_data=malloc(sizeof(data_item_t));

			new->it_data->i = token->data->i;
			new->type = INT;
		}
		if(token->type == NONE){ //None
			new->it_data= NULL;
			new->type = NONE;
		}
		else if(token->type == FLOAT){ //Double
			new->it_data=malloc(sizeof(data_item_t));
			new->it_data->fl = token->data->fl;
			new->type = FLOAT;
		}
		else if(token->type == STRING || token->type == IDENTIFIER || token->type == COMMENT){ //String / Identif
			new->it_data=malloc(sizeof(data_item_t));
			new->it_data->str = string_ctor();
			string_cpy(new->it_data->str, token->data->str);
			new->type = token->type;
		}
		new->k=string_ctor();
		string_cpy(new->k, (token->type == IDENTIFIER) ? token->data->str : key);
		new->defined = false;
		new->next = zaloha;
		table->ptr[htab_hash_function(tmp_key)] = new;
	
		return true;
	}
}

//Měníme defined na true
void type_defined(htab_t *table,string_t *key){
	
	if(htab_get_item(table, key) != NULL){
		table->ptr[htab_hash_function(string_get(key))]->defined = true;
	}

}

//Hledá prvku v tabulce
htab_item_t *htab_get_item(htab_t *table, string_t *key){

	htab_item_t *ukazatel = table->ptr[htab_hash_function(string_get(key))];
	while(ukazatel != NULL){ //Dokud nejsme na konci prohledavej
 		if(string_cmp_string(ukazatel->k, key)){ //Pokud se budou klice rovnat, tak vratim ten hledany prvek
 			return ukazatel;
 		} //Posunu se dal v hledani, na dalsi misto
 		ukazatel=ukazatel->next;
 	}
	return NULL;
}

//Vytváření tabulky pro funkci uvnitř globální tabulky
bool htable_make_func(htab_t *table, string_t *key){

	htab_item_t *new = NULL;
	htab_item_t *zaloha = table->ptr[htab_hash_function(string_get(key))];

	new = table->ptr[htab_hash_function(string_get(key))];  //Místo pro nový prvek
	if((new = malloc(sizeof(htab_item_t))) == NULL) return false; 

	htab_t *new_table = htab_ctor(); //Vytvoření nové tabulky
	char *tmp_key = string_get(key); //Klíč předělaný ze string na char
	new->it_data = malloc(sizeof(data_item_t)); //Alkoace místa
	new->it_data->fce = malloc(sizeof(fce_t)); //Alokace místa
	if(new->it_data->fce == NULL) return false; 
	new->it_data->fce->h_table = new_table; //Přiřazení tabulky
	new->type = FUNCTION; //Nastavíme typ na funkci
	new->k=string_ctor(); //Vytvoření string pro key
	string_cpy(new->k, key);
	new->defined = false; //Ještě není děfinováno
	new->next = zaloha; //Další je minulý předchozí
	table->ptr[htab_hash_function(string_get(key))] = new;
	return true;
}

//Funkce na mazání tabulky a tabulek funkcí
void htab_dtor(htab_t *table){

	htab_item_t *tmp = NULL ;
	htab_item_t *to_del = NULL ;
	int i = 0;
	for(; i < HASH_SIZE; i++){ //Procházíme celou tabulkou a mažeme 
		tmp = table->ptr[i];	
		while(tmp != NULL){
			
			if(tmp->type == FUNCTION){ //Pokud je to funkce tak rekurzivně voláme a mažene tabulku v tabulce 
				htab_dtor(tmp->it_data->fce->h_table);
				free(tmp->it_data->fce);
			}
			to_del = tmp;
			tmp = tmp->next; //Posuneme na další prvek
			if(table->ptr[i]->type == STRING) string_dtor(&(table->ptr[i]->it_data->str)); //Když je to string, děalokujeme i jej
			string_dtor(&(table->ptr[i]->k));
			free(to_del->it_data);
			free(to_del);
		}
	}
	free(table); //Nakonec uvolnění samotné tabulky
}




