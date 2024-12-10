/*
 * Definice funkcí pro získání informací o systému.
 * --------------------------
 * Soubor: getData.c
 * Projekt: ISA - rozšíření SNMP agenta.
 * Autor: Michal Šedý <xsedym02>
 * Datum úpravy: 30.10.2020
 */
#include "getData.h"


/*
 * Funkce vrací booleovskou hodnotu, zda obsahuje řetězec daný prefix.
 * -------------------------------------------------------------------
 * string - testovaný textový řetězec
 * prefix
 */
bool starts_with(const char *string, const char *prefix)
{
	int string_len = strlen(string);
	int prefix_len = strlen(prefix);

	if (prefix_len > string_len)
	{
		return false;
	}
	
	return memcmp(string, prefix, prefix_len) == 0;
}


/*
 * Z daného souboru získá první řádek začínající prefixem.
 * Získaný řádek nahraje do dst.
 * ------------------------------------------------------
 * dst - ukazatel na proměnnou, kam se nahraje získaný řádek
 * prefix - hledaný prefix
 * fiel_name - prohledávaný soubor
 */
void get_line_starts_with(char *dst, const char *prefix, const char *file_name)
{
	FILE *fp = NULL;
	fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "ERROR: Nepodařilo se otevřít soubor %s.\n", file_name);
		return;
	}

	do 
	{
		if(fgets(dst, 256, fp) == NULL)
		{
			fclose(fp);
			return;
		}
	} while(!starts_with(dst, prefix));
	fclose(fp);
}


/*
 * Získá hodnotu z textu ve formátu: "VAR_NAME  :  12345 KB"
 * Vrací všechny hodnoty za dvojtečkou: "12345 KB".
 * ---------------------------------------------------------
 * line - parsovaný řádek
 * dst - ukazatel kam má být zkopírována získaný hodnota.
 */
void get_value_from_line(char line[], char *dst)
{
	short state = 0;
	unsigned short j = 0;
	for(int i = 0; line[i] != '\0'; i++)
	{
		switch(state)
		{	
			case 0:		// Zahazuje znaky do dvojtečky.
				if(line[i] == ':')
				{
					state = 1;
				}
				break;

			case 1:		// Za dvojtečkou zahazuje mezery.
				if(line[i] != ' ')	// Nejedná o bílý znak, začne zaznamenávat.
				{
					state = 2;
					j++;
				}
				break;
			case 2:		// Zaznamenává do konce řádku.
				if(line[i] == '\n')
				{
					state = -1;
				}
				else
				{
					j++;
				}
				break;
		}

		// Uložení výsledku
		if (state == -1)
		{	
			// Smaže odřádkování a výsledek od zjištěné pozice uloží.
			line[i] = '\0';
			strcpy(dst, line + (i - j));
			break;
		}
	}
}


/*
 * Funkce pro získání hodnoty proměnné nacházející se v daném souboru.
 * -------------------------------------------------------------------
 * fiel_name - jméno prohledávaného souboru
 * var_name  - jméno proměnné
 * dst - ukazatel kam se má výsledek uložit
 * is_int - informace o typu očekávaného výsledku.
 *        - TRUE  -> int
 *        - FALSE -> char[]
 */
void get_data(const char *file_name, const char *var_name, void *dst, bool is_int)
{
	char line[256] = {0,};
	char value[256] = {0,};

	// Nalezne řádek obsahující hledanou proměnnou
	get_line_starts_with(line, var_name, file_name);
	if(line[0] == '\0')
	{
		fprintf(stderr, "CHYBA: Nebyla načtena proměnná %s ze souboru %s.\n", var_name, file_name);
		return;
	}

	// Získá data z daného řádku, už bez jména, pouze hodnotu.
	get_value_from_line(line, value);
	
	// Uložení získaných hodnot do proměnné, na kterou odkazuje *dst.
	// Pokud se jedná o číselnou hodnotu, převede ji na int, jinak ponechá char.
	if(is_int)
	{	
		for(int i = 0; value[i] != '\0'; i++)
		{
			if(!isdigit(value[i]))
			{
				value[i] = '\0';
				break;
			}
		}
		*(int*)dst = strtol(value, NULL, 10);
	}
	else
	{
		strcpy((char*)dst, value);
	}
}
