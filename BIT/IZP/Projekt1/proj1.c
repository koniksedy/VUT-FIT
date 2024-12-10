/**
 * IZP projekt 1
 * TEXTOVÝ EDITOR
 * ##############
 * Michal Šedý
 * zahájení 23.10.2018
 * poslední úprava 3.11.2018 
 * -----------------------------------------------------------------------------------------------------------
 * Textový editor podporuje následujísí sadu příkazu:
 * iCONTENT (insert) - před aktuální řádek vypíše nový s obsahem CONTENT
 * bCONTENT (before) - na začátek aktuálního řádku připojí obsah CONTENT
 * aCONTENT (after) - na konec aktuálního rádku připojí obsah CONTENT
 * dN (deleteN) - smaže N řádků (N > 0), pokud se vyskytuje samotný příkaz d, pak smaže aktuální řádek
 * r (remove EOL) - smaže na aktuálním řádku znak konce řádku
 * s:PATTERN:REPLACEMENT (substitute) - na aktuálním řádku provede nahrazení části PATTERN za REPLACEMENT,
 * 										PATTERN musí být neprázdný. Pro oddělovač se může použít livobolný
 * 										tisknutelný znak, nebo mezera. Část REPLACEMENT nesmí obsahonat 
 * 										daný oddělovač (došlo by k chybě při nahrazování).
 * S:PATTERN:REPLACEMENT (substituteAll) - funkce podobná předcházejícímu příkazu, zde jsou ale nahrazeny
 * 										   všechny výskyty PATTERN. Pro oddělovač se nesmí použít mezera.
 * nN (next) - vypíše N řádků (N > 0), pokud se vyskytuje samotný příkaz n, pak se vypíše pouze aktuální řádek
 * q (quit) - ukončí editaci, další řádky se již nezpracovávají
 * gN (goto) - příkaz přeskočí na X-tý řádek s editečnám příkazem (1 <= X >= počet řádku s příkazy)
 * ------------------------------------------------------------------------------------------------------------
 * Program poskutuje ošetření všech vstupů. Detekuje zacyklení(které může nastat) při použí píkazu GOTO.
 * V případě chyby program vypíše příslušnou chybovou hlášku na standartní chybový výstup.
 * Aktuálně program podporuje 200 editačních příkazů a řádky o 2000 znacích.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define RADKY 200
#define ZNAKY 2000 

void err(int e, int i);
int kontDelky(int *d, char *retezec);
int kontPrikazu(int d, char prikaz);
bool kontZacykleni(char *prikaz, int radek, int n);
int before(int *dr, char *retezec, char *postprik, int dp);
int after(int *dr, char *retezec, char *postpr, int dp);
int NextAndDelete(char typ, char *retezec, int *dr, char *postprik);
int goToN(int *radek, char *prikaz, char *postprik, int p_radku);
int substitute(int *dr, char *retezec, int dp, char *postprik);
int substituteAll(int *dr, char *retezec, int dp, char *postprik);

int main(int argc, char *argv[])
{
	int i = 0;							/* Index příkazu */
	int pocet_radku;					/* Celkový počet řádků s příkazy */
	int pomocny;						/* Pomocná proměná pro výsledek fgetc */
	char prikaz[RADKY+1]; 				/* První znaky příkazů (převod z pomocny)*/
	char postprik[RADKY+1][ZNAKY+2]; 	/* Příkazy bez prvního znaku */
	char retezec[ZNAKY+2];				/* Zpracovávaný textový řetězec */ 
	int delka_postprik[RADKY+1];		/* Délky příkazů bez prvního znaku */
	int delka_retezec;					/* Délka zpracovávaného textového řetězce */
	int errN = 0;						/* Pro ukládání chybových hodnot */
	bool pokracovat = true;

	/* KONTORLA SPRÁVNOSTI ARGUMENTŮ */
	if(argc != 2)
	{
		err(1, i+1);
		return 1;
	}

	/* OTEVŘENÍ SOUBORU a jeho následná KONTROLA */
	FILE *soubor = fopen(argv[1], "r");
	if(soubor == NULL)
	{
		err(2, i+1);
		return 1;
	}

	/* NAČÍTÁNÍ PŘÍKAZŮ DO PAMĚTI a současná KONTROLA 
	 * ----------------------------------------------
	 * i = RADKY, je signalizací, že se v souboru nachází více řádků, naž je podporováno.
	 */
	while(i < RADKY+1 && (pomocny = fgetc(soubor)) != EOF)
	{
		prikaz[i] = pomocny;
		fgets(postprik[i], ZNAKY+2, soubor);
		/* KONTROLA, zda nebyl překročen podporovaný počet znaků (v příkazu).*/
		if((errN = kontDelky(&delka_postprik[i], postprik[i])))
		{
			break;
		}
		/* KONTROLA, zda se jedná o podporovaný příkaz */
		if((errN = kontPrikazu(delka_postprik[i], prikaz[i])))
		{
			break;
		}
		i++;
	}
	if(errN)
	{
		err(errN, i+1);
		fclose(soubor);
		return 1;
	}
	/* KONTROLA, zda nebyl překročen podporovaný počet příkazů */
	if(i > RADKY)
	{
		err(5, i+1);
		fclose(soubor);
		return 1;
	}
	/* ZAVIRÁM SOUBOR. (Všechny příkazy jsou již načteny v paměti.) */
	fclose(soubor);
	pocet_radku = i;
	i = 0;	/* Přesun na prví příkaz */

	/* NAČÍTÁNÍ ŘÁTKŮ TEXTU A APLIKACE PŘÍKAZŮ */
	while(fgets(retezec, ZNAKY+2, stdin) != NULL)
	{
		/* TEST, zda nebyl překročen podporovaný počet znaků (v textu). */
		if(kontDelky(&delka_retezec, retezec))
		{
			err(6, i+1);
			return 1;
		}
		/* ROZLIŠENÍ PŘÍKAZŮ */
		while(i < pocet_radku && pokracovat)
		{
			switch(prikaz[i])
			{
				case 'i':
					printf("%s", postprik[i]);
					break;

				case 'b':
					errN = before(&delka_retezec, retezec, postprik[i], delka_postprik[i]);
					break;

				case 'a':
					errN = after(&delka_retezec, retezec, postprik[i], delka_postprik[i]);
					break;

				case 'n':
				case 'd':
					errN = NextAndDelete(prikaz[i], retezec, &delka_retezec, postprik[i]);
					if(errN == (-1))
					{
						errN = 0;
						pokracovat = false;
					}
					break;

				case 'r':
					retezec[delka_retezec] = '\0';
					break;

				case 'q':
					pokracovat = false;
					break;

				case 'g':
					errN = goToN(&i, prikaz, postprik[i], pocet_radku);
					break;
				
				case 'S':
					errN = substituteAll(&delka_retezec, retezec, delka_postprik[i], postprik[i]);
					break;

				case 's':
					if((errN = substitute(&delka_retezec, retezec, delka_postprik[i], postprik[i])) < 0)
					{
						errN = 0;
					}
					break;
			}
			/* KONTROLA, zda nedošlo během aplikace příkazů k chybě */
			if(errN)
			{
				err(errN, i+1);
				return 1;
			}
			i++;
		}
		/* KONTORLA, zda už nejsme na konci textu */
		if(!pokracovat)
		{
			break;
		}
		printf("%s", retezec);
	}
	if(prikaz[i-1] == 'q')
	{
		return 0;
	}
	/* KONTROLA, zda je poslední řádek ukončen \n */
	if(retezec[delka_retezec] != '\n')
	{
		printf("\n");
	}
	return 0;
}

/* FCE pro výpis chybových hlášení
 * ###############################
 * e - hodnota chyby
 * i - číslo řádu (začíná 1)
 */
void err(int e, int i)
{
	switch(e)
	{
		case 1:
			fprintf(stderr, "CHYBA: Zadány nesprávné argumenty.\n"
							"----------------------------------\n"
							"Paramentry pro spušténí jsou: ./proj1 [soubor s příkazy] .\n");
			break;
		case 2:
			fprintf(stderr, "CHYBA: Soubor se napodařilo otevřít, zkontrolujte cestu k souboru.\n");
			break;
		case 3:
			fprintf(stderr, "CHYBA: V souboru s příkazy se na řádku [%d] nachází příkaz s obsahem,"
							" který přesahuje podlorovanou délku [%d] znaků.\n", i, ZNAKY);
			break;
		case 4:
			fprintf(stderr, "CHYBA: V souboru s příkazy se na řádku [%d] nachází nepodporovaný/neplatný příkaz.\n", i);
			break;
		case 5:
			fprintf(stderr, "CHYBA: Byl překročen podporovaný počet [%d] příkazů.\n", RADKY);
			break;
		case 6:
			fprintf(stderr, "CHYBA: Upravovaný text přesáhl podporovanou délku [%d] znaků.\n", ZNAKY);
			break;
		case 7:
			fprintf(stderr, "CHYBA: V souboru s příkazy se na řádku [%d] nenachází druhý oddělovač, který je zapotřebí pro příkaz [s/S].\n"
							"-----------------------------------------------------------------------------------------------------------\n"
							"Správný syntaxe příkazu: s:PATTERN:REPLACEMENT / S:PATTERN:REPLACEMENT  (: symbolizuje oddělovač)\n", i);
			break;
		case 8:
			fprintf(stderr, "CHYBA: V příkazu [s/S] musí být pattern nenulový.\n"
							"-------------------------------------------------\n"
							"Správná syntaxe příkazu: s:PATTERN:REPLACEMENT / S:PATTERN:REPLACEMENT  (: symbolizuje oddělovač)\n");
			break;
		case 9:
			fprintf(stderr, "CHYBA: V příkazu [S] nelze použít mezeru jako oddělovač."
							" Zkontrolujte v souboru s příkazy řádek [%d].\n", i);
			break;
		case 10:
			fprintf(stderr, "CHYBA: Riziko zacyklení vrámci příkazu goto. V souboru s příkazy se mezi příkazem goto"
							" a odkazovaným řádkem musí nacházet příkaz next, nebo delete.\n");
			break;
	}
}

/* FCE pro KONTROLU a určení DÉLKY řetězce
 * #######################################
 * d - ukazatel na déku zkoumaného řetězce
 * retezec - zkoumaný řetězec
 * ---------------------------------------------------------------------------------------
 * NÁVRATOVÁ H. - Pokud délka řetězce nepřesáhne podporovanou délku programen fce vrací 0, 
 * 				  v opačném případě hodnotu chyby.
 */
int kontDelky(int *d, char *retezec)
{
	if((*d = strlen(retezec)-1) == ZNAKY && retezec[ZNAKY] != '\n')
	{
		return 3;
	}
	return 0;
}

/* FCE pro KONTROLU správnosti PŘÍKAZU
 * ###################################
 * d - délka postpříkazu
 * prikaz - první(referenční) znak z řádku s příkazem
 * ------------------------------------------------------------------------------------
 * NÁVRATOVÁ H. - Pokud je příkaz pogporován a zapsán se správnou syntaxí, fce vraci 0, 
 * 				  v opačném případě hodnotu chyby.
 */
int kontPrikazu(int d, char prikaz)
{
	/* Příkazy, na které už nenavazuje postpříkaz */
	char prik_samost[4] = {'r','q', 'd', 'n'};
	/* Příkazy, na které navazuje postpříkaz */
	char znprik[8] = {'i', 'b', 'a', 's', 'S', 'g', 'd', 'n'};	
	int kontpocet = 0;
	/* KONTROLA samostatných příkazů */
	if(d == 0)
	{	
		for(int i = 0; i < 4; i++)
		{
			if(prikaz != prik_samost[i])
			{
				kontpocet++;
			}
		}
		if(kontpocet != 3)
		{
			return 4;
		}
	}
	/* KONTROLA ostatních příkazů */
	else
	{
		kontpocet = 0;
		for(int i = 0; i < 8; i++)
		{
			if(prikaz != znprik[i])
			{
				kontpocet++;
			}
		}
		if(kontpocet != 7)
		{
			return 4;
		}
	}
	return 0;
}

/* FCE zabraňuje ZACYKLENÍ při použití příkazu GOTO
 * ################################################
 * prikaz - pole příkazů(všech)
 * radek - index konkrétního řádku, ze kterého jsme provedli skok
 * n - index řádku na který skok provádíme
 * ------------------------------------------------------------------------------------------------
 * NÁVRATOVA H. - Pokud bude mezi řádkem s intexem [n] a [radek], nalezen příkaz next, nebo delete,
 * 				  fce vrátí false(k zacyklení nedojde). V opačném případě vrátí true.
 */
bool kontZacykleni(char prikaz[], int radek, int n)
{
	if(n > radek)
	{
		return false;
	}
	/* Příkaz goto může skočit sám na sebe, proto testujeme pro <= . */
	for(int i = n; i <= radek; i++)
	{
		if(prikaz[i] == 'd' || prikaz[i] == 'n')
		{
			return false;
		}
	}
	return true;
}

/* FCE provádí příkaz BEFORE
 * #########################
 * dr - ukazatel na délku zpracovávaného řetězce, aktualizuje se o délku přidané části
 * retezec - zpracovávaný textový řetězec
 * postprik - postpříkaz
 * dp - délka postpříkazu
 * ----------------------------------------------------------------------------------------------------------
 * NÁVRATOVÁ H. - Fce vrací 0. Pokud je překročena podporovaná délka řetězce, vrací příslušnou hodnotu chyby.
 */
int before(int *dr, char *retezec, char *postprik, int dp)
{
	if((*dr + dp) > ZNAKY)
	{
		return 6;
	}
	/* Odstranění \n v připojovaném řetězci */
	postprik[dp] = '\0';
	strcpy(retezec, strcat(postprik, retezec));
	*dr += dp; 
	return 0;
}

/* FCE provádí příkaz AFTER
 * ########################
 * dr - ukazatel na délku zpracovávaného řetězce, aktualizuje se o délku přidané části
 * retezec - zpracovávaný textový řetězec
 * postprik - postpříkaz
 * dp - délka postpříkazu
 * ----------------------------------------------------------------------------------------------------------
 * NÁVRATOVÁ H. - Fce vrací 0. Pokud je překročena podporovaná délka řetězce, vrací příslušnou hodnotu chyby.
 */
int after(int *dr, char *retezec, char *postprik, int dp)
{
	if((*dr + dp) > ZNAKY)
	{
		return 6;
	}
	/* Odstranění \n v hlavním řetězci */
	retezec[*dr] = '\0';
	strcat(retezec, postprik);
	*dr += dp;
	return 0;
}

/* FCE provádí příkaz NEXT i DELETE
 * ################################
 * typ - určuje zda se má provést příkaz next, či delete (Vzhledem k tomu, že příkazy next a delete si jsou
 * 		 velice podobné, byly sloučeny do jedné fce.)
 * retezec - zpracovávaný textový řetězec. Z fce se vrací aktualizovaný(načte se další řádek na úpravu).
 * dr - ukazatel na délku řetězce. Z fce se vrací také aktualizovaný.
 * postprik - postpříkaz
 * -----------------------------------------------------------------------------------------------------------------
 * NÁVRATOVÁ H. - Fce vrací 0(Pokud byly načteny další řádky pro úpravu.), -1(Pokud se dospělo ke textu pro úpravu),
 * 				  v ostatních případech vrací fce hodnotu chyby.
 */
int NextAndDelete(char typ, char *retezec, int *dr, char *postprik)
{
	char *zbytek;
 	int n = strtol(postprik, &zbytek, 10);
	/* KONTROLA, zda se za příkazem  next/delete nachází číslo */
 	if((zbytek != NULL && zbytek[0] != '\n') || (n == 0 && zbytek[0] == 0))
	{
		return 4;
	} 
	/* Pokud se za příkazem next/delete nenachází žádná číselna hodnota bere se to jako jedna. */
 	if(n == 0 && postprik[0] == '\n')
	{
		n = 1;
	}
	for(int i = 0; i < n ; i++)
	{	
		/* Jediný rozdíl mezi příkazy next a delete, je ten že next ještě navíc řetězec vypisuje. */
		if(typ == 'n')
		{
			printf("%s", retezec);
		}
		/* KONTROLA, zda už nejsme na konci upravovaného textu */
		if(fgets(retezec, ZNAKY+2, stdin) == NULL)
			{
				return (-1);
			}
		/* KONTROLA délky řetězce */
   		if(kontDelky(dr, retezec))
		{
	 		return 6;
		}
	 }
	 return 0;
}

/* FCE provádí příkaz GOTO
 * #######################
 * radek - ukazatel k číslo řáku(v souboru s příkazy). Z fce se vrátí aktualizovaný(podle potřeby skoku).
 * postprik - postpříkaz
 * p_radku - celkový počet řádků v souboru s příkazy
 * ------------------------------------------------------------------------------------------------------
 *  NÁVRATOVÁ H. - Fce vrací 0. V opačném případě hodnotu chyby.
 */
int goToN(int *radek, char *prikaz, char *postprik, int p_radku)
{
	char *zbytek;
 	int n = strtol(postprik, &zbytek, 10);
 	if((zbytek != NULL && zbytek[0] != '\n') || n > p_radku || n == 0)
	{
		return 4;
	}
	/* KONTROLA, zda může dojít k ZACYKLENÍ */
	if(kontZacykleni(prikaz, *radek, n-1))
	{
		return 10;
	}
	*radek = (n - 2); /* -1 z důvodu indexování(od 0) a další -1 z důvodu i++ na konci while v main */
	return 0;
}

/* FCE provádí příkaz SUBSTITUTE
 * #############################
 * dr - ukazatel na délku řetězce. Z fce se vrátí aktualizovaný.
 * retezec - zpracovávaný textový řetězec. Z fce se vrátí aktualizovaný.
 * dp - délka postpříkazu
 * postprik - postpříkaz
 * --------------------------------------------------------------------------------------------------------------
 * NÁVRATOVÁ H. - Fce vrací 0(v případě úspěšného nahrazení), -1(v případě, chybějící nahrazované částí řetězce),
 *  			   v ostatních případech vrací fce hodnotu chyby.
 */
int substitute(int *dr, char *retezec, int dp, char *postprik)
{
	int d_docasna;				/* Délka aktuálního řetězce */
	int d_hled;					/* Délka hledané části řetězce */
	char hledany[ZNAKY+1] = {0}; /* Hledaná část řetězce */
	char nahrada[ZNAKY+1] = {0}; /* Náhradní řetězec */
	char sek[ZNAKY+2] = {0}; 	/* Sekundární řetězec (Pokračování hlavního řetězce za nahrazovanou částí) */
	int zac_sek; 				/* Index začátku sekundárního řetězce */ 
	int d_prim;					/* Délka primárního řetězce (Před nahrazovanou částí) */
	/* NAČTENÍ HLEDANÉ ČÁSTI z postpříkazu. postprik[0] - oddělovač */
	for(d_hled = 0; postprik[1+d_hled] != postprik[0]; d_hled++)
	{
		hledany[d_hled] = postprik[1+d_hled];
		/* KONTROLA, zda obsahuje oba oddělovače */
		if(d_hled+2 == dp)	/* +1 přeskočí první oddělovač, další +1 nahrazuje intrementaci na konci ciklu  */
		{
			return 7;
		}
	}
	if(!d_hled)
	{
		return 8;
	}
	if(strstr(retezec, hledany) == NULL)
	{
		return (-1);
	}
	d_docasna = *dr;
	/* KONTROLA DÉLKY */
	if((*dr = (*dr+dp-d_hled-d_hled-2)) > ZNAKY) /* -2 kompenzuje dva oddělovače */
	{
		return 6;
	}
	/* NAČÍTÁNÍ NÁHRADNÍ části z postpříkazu +2 slouží k přeskočení obou oddělovačů
	 * -2 reprezentuje oba obbělovače, \n se neukládá */
	memcpy(nahrada, &postprik[d_hled+2], dp-d_hled-2);
	d_prim = strstr(retezec, hledany) - &retezec[0];
	zac_sek = d_prim + d_hled;
	/* NAČÍTÁNÍ SEKUNDÁRNÍHO řetězce, +2 z důvodu indexování od 0 a potřebě uložit \n */
	memcpy(sek, &retezec[zac_sek], d_docasna-zac_sek+2);
	retezec[d_prim] = '\0';
	strcat(retezec, nahrada);
	strcat(retezec, sek);
	return 0;
}

/* FCE provádí příkaz SUBSTITUTEALL
 * #############################
 * dr - ukazatel na délku řetězce. Z fce se vrátí aktualizovaný.
 * retezec - zpracovávaný textový řetězec. Z fce se vrátí aktualizovaný.
 * dp - délka postpříkazu
 * postprik - postpříkaz
 * ---------------------------------------------------------------------
 * NÁVRATOVÁ H. - Fce vrací 0, nebo příslušnou hodnotu chyby.
 */
int substituteAll(int *dr, char *retezec, int dp, char *postprik)
{
	int navrat = 0;
	/* Oddělovačem nemůže být mezera */
	if(postprik[0] == ' ')
	{	
		return 9;
	}
	while((navrat = substitute(dr, retezec, dp, postprik)) == 0);
	if(navrat == (-1))
	{
		return 0;
	}
	return navrat;
}
