/**
 * Modul pro práci s parametry funkcí
 * Napsal: Michal Šedý <xsedym02>
 * Přeloženo: gcc 7.4.0
 * Standard: C99
 * Poslední změna: 05.12.19
 */

#include "param.h"

/**
 * Funkce alokuje místo v tabulke pro parametry.
 * ---------------------------------------------
 * table - tabulka, ve které vytvoříme parametry
 *
 * V případě úspěchu vrací true, jinak false.
 */
bool params_ctor(htab_t *table)
{
	// Kontrola, zda tabulka existuje
	if(table == NULL)
	{
		return false;
	}

	// Alokace zdrojů a kontrola
	param_t *new = (param_t*)malloc(sizeof(param_t));
	if(new == NULL)
	{
		return false;
	}

	new->next = NULL;
	new->ptr = NULL;

	table->params = new;

	return true;
}

/**
 * Funkce pro přidání parametru do tabulky na konec jejich seznamu
 * ---------------------------------------------------------------
 * table - tabulka pro vložení parametru
 * token - token obsahující jmého parametru
 *
 * V případě úspěchu vrací funkce 0, jinak patřičný chybový kód
 */
int params_append(htab_t *table, token_t *token)
{

	// Pokud nenajde tabulka prvek z tokenu, pak chyba
	if(!htab_insert(table, token->data->str, token)) return false;

	// Bkládání prvního parametru
	if(table->params == NULL)
	{
		if(!params_ctor(table))
		{
			return ERR_INTER_RUN;
		}

		table->params->ptr = htab_get_item(table, token->data->str);
	}

	// Vkládání parametru až na konec seznamu
	param_t *tmp = table->params;
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
	}

	// Alokace parametru a kontrola
	tmp = (param_t*)malloc(sizeof(param_t));
	if(tmp == NULL)
	{
		return ERR_INTER_RUN;
	}
	// Přiřazení hodnot
	tmp->next = NULL;
	tmp->ptr = htab_get_item(table, token->data->str);
	tmp->ptr->defined = true;
	// Zvýšení počítadla parametrů
	(table->params_cnt)++;

	return ERR_OK;
}

/**
 * Funkce vrací ukazatel na parametry 
 * ----------------------------------
 * table - tabulky, ze které jsou parametry čteny
 * 
 * Funkce vrací ukazatel na tento seznam parametrů.
 */
param_t *params_get(htab_t *table)
{
	return table->params;
}

/**
 * Funkce dealokuje parametry
 * --------------------------
 * table - tabulka, ze které budou odstraněny parametry
 */
void params_dtor(htab_t *table)
{
	param_t *tmp = table->params;
	param_t *to_del = tmp;

	// Postupně se mažou všechnyn prvky seznamu parametrů
	while(tmp != NULL)
	{
		to_del = tmp;
		tmp = tmp->next;
		free(to_del);
	}

	// Nakonec se dealokuje sanotný seznam
	free(table->params);
	table->params = NULL;
}