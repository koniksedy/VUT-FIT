/**
 * Modul pro generování řetězců string_t
 * Napsal: Michal Šedý <xsedym02>
 * Přeloženo: gcc 7.4.0
 * Standard: C99
 * Poslední úprava: 05.12.19
 */

#include "string_generator.h"

#define INFO ;//fprintf(stderr, "---------------------------------\nmodul-----> %s\nfunkce ------> %s\nline-----> %d\n---------------------------------\n", __FILE__, __func__, __LINE__);


/**
 * Funkce pro vytvoření jména pomocné proměnné
 * -------------------------------------------
 * - Funkce vytvoří pokaždé různé jméno s konstantním počátečním znakem $
 */
string_t *string_rand(void)
{
	INFO;
	// Inicializace počítadla a proměnné pro převod čísla na char*
	static unsigned int i = 0;
	char num[MAX_ORDER] = {'\0',};

	// Kontrola, zda se po sprintf vejde do num
	if(i > 999999) return NULL;
	sprintf(num, "%d", i);

	// Alokace string_t
	string_t *str = string_ctor();
	if(str == NULL)	return NULL;

	// Vložení nezaměnitelného znaku
	string_put_char(&str, '$');
	if(str == NULL)	return NULL;

	// Vložení čísla do stringu číslici po číslici
	int num_len = strlen(num);
	for(int j = 0; j < num_len; j++)
	{
		string_put_char(&str, num[j]);
		if(str == NULL)	return NULL;
	}
	
	i++;
	return str;
}

/**
 * Funkce pro vytvoření jména návěští
 * -------------------------------------------
 * - Funkce vytvoří pokaždé různé jméno s konstantním počátečním znakem %
 */
string_t *generate_label(void)
{
	INFO;
	// Inicializace počítadla a proměnné pro převod čísla na char*
	static unsigned int l = 0;
	char num[MAX_ORDER] = {'\0',};

	// Kontrola, zda se po sprintf vejde do num
	if(l > 999999) return NULL;
	sprintf(num, "%d", l);

	// Alokace string_t
	string_t *str = string_ctor();
	if(str == NULL)	return NULL;

	// Vložení nezaměnitelného znaku
	string_put_char(&str, '%');
	if(str == NULL)	return NULL;

	// Vložení čísla do stringu číslici po číslici
	int num_len = strlen(num);
	for(int j = 0; j < num_len; j++)
	{
		string_put_char(&str, num[j]);
		if(str == NULL)	return NULL;
	}
	
	l++;
	return str;
}

/**
 * Funkce pro přepis textového řetězce na string_t
 * -----------------------------------------------
 * str - řetězec pro přepis
 *
 * Funkce vzací ukazatel na takto vzniklý řetězec, pokud se vytváření
 * nepovede, vrátí NULL.
 */
string_t *string_make(const char *str)
{
	INFO;

	// Alokace string_t
	string_t *string = string_ctor();
	if(str == NULL) return NULL;

	// Kopírováíní znak po znaku
	int str_len = strlen(str);
	for(int i = 0; i < str_len; i++)
	{
		string_put_char(&string, str[i]);
		if(string == NULL) return NULL;
	}

	return string;
}