/**
 * IZP - Projekt 2 - Iterační výpočty
 * ##################################
 *
 * Program vypočítává přirozený logaritmus a exponenciální funkci s obecným základem pouze pomocí operací +,-,*,/.
 * Michal Šedý
 * Zahájení 17.11.2018
 * Poslední úprava 25.11.2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define PI 3.141592653589793
#define ACCURACY 0.000000001	// Aby nedošlo k zaodkouhlení, tak hlídám i 9 platné číslo
enum err{ERR_FAIL=(-1), SUCCESS, ERR_ARG, ERR_CNT, ERR_ISNUM, ERR_ITERATION, ERR_VAL} ERRN;

void error(int e);
int load(int argc, char *argv[], double a[]);
bool chack_log(double *x);
bool  chack_pow(double *x, double y);	
double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
double taylor_pow(double x, double y, unsigned int n);
double taylorcf_pow(double x, double y, unsigned int n);
bool acc(double x, double y);
double mylog (double x);
double mypow(double x, double y);
double sinx(double x);
double sign(double result, double y, double x);

int main(int argc, char *argv[])
{
	double tmpvar[3] = {0.0};	
	double x, y;
	unsigned int n;			
	double result_taylor;
	double result_cfrac;
	double result_my;

	// NAČÍTÁNÍ PROMĚNNÝCH A KONTROLA ARGUMENŮ
	ERRN = load(argc, argv, tmpvar); 
	if(ERRN)
	{
		error(ERRN);
		return ERR_FAIL;
	}
	
	// VÝPOČET LOGARITMU
	if(strcmp(argv[1], "--log") == 0 && argc == 4)
	{
		x = tmpvar[0];
		n = (unsigned int)(tmpvar[1]);
		result_cfrac = cfrac_log(x, n);
		result_taylor = taylor_log(x, n);
		printf("       log(%g) = %.12g\n", x, log(x));
		printf(" cfrac_log(%g) = %.12g\n", x, result_cfrac);
		printf("taylor_log(%g) = %.12g\n", x, result_taylor);
	}
	// VÝOIČET EXPONENCIÁLNÍ FCE
	else if(strcmp(argv[1], "--pow") == 0 && argc == 5)
	{
		x = tmpvar[0];
		y = tmpvar[1];
		n = (unsigned int)(tmpvar[2]);
		if(x < 0)
		{
			error(ERR_VAL);
			return ERR_FAIL;
		}
		result_taylor = taylor_pow(x, y, n);
		result_cfrac = taylorcf_pow(x, y, n);
		printf("         pow(%g,%g) = %.12g\n", x, y, pow(x, y));
		printf("  taylor_pow(%g,%g) = %.12g\n", x, y, result_taylor);
		printf("taylorcf_pow(%g,%g) = %.12g\n", x, y, result_cfrac);
	}
	// VÝPOČET EXP. FCE (V CMPLX ČÍSLECH)
	else if(strcmp(argv[1], "--mypow") == 0 && argc == 4)
	{
		x = tmpvar[0];
		y = tmpvar[1];
		result_my = mypow(x, y);
		if(y == (int)(y) || x >= 0 || (isinf(x) == 1 && isinf(y) == -1))
		{
			printf("  pow(%g,%g) = %.8g\n", x, y, pow(x, y));
			printf("mypow(%g,%g) = %.8g\n", x, y, sign(result_my, y, x));
		}
		// Výsledek v cmplx
		else
		{	
			printf("  pow(%g,%g) = %.8g\n", x, y, pow(x, y));
			printf("mypow(%g,%g) = %.8g %+.8gi\n", x, y, (sign(result_my, y, x)*sinx(y*PI-(PI/2))),(result_my*sinx(y*PI)));
		}
		printf("Vypočteno pomocí zřetězeného zlomku.\n");
	}
	// VÝPOČET LOGARITMU (V CMPLX ČÍSLECH)
	else if(strcmp(argv[1], "--mylog") == 0 && argc == 3)
	{
		x = tmpvar[0];
		result_my = mylog(x);
		printf("  log(%.8g) = %.8g\n", x, log(x));
		if(x < 0)
		{
			printf("mylog(%g) = %.8g + %.8gi\n", x, result_my, PI);
		}
		// Výsledek v cmplx
		else
		{
			printf("mylog(%g) = %.8g\n", x, result_my);  
		}
		printf("Vypočteno pomocí zřetězeného zlomku.\n");
	}
	else
	{
		error(ERR_ARG);
		return ERR_FAIL;
	}

	return SUCCESS;
}

/**
 * FCE PRO VÝPIS CHYBOVÝCH HLÁŠEK 
 * ##############################
 */
void error(int e)
{
	switch(e)
	{
		case 1:
			fprintf(stderr, "CHYBA: Nesprávné argumeny.\n");
			break;
		case 2:
			fprintf(stderr, "CHYBA: Nesprávný počet argumentů.\n");
			break;
		case 3:
			fprintf(stderr, "CHYBA: Číselné argumenty nesmí obsahobat jiné znaky než číselné.\n");
			break;
		case 4:
			fprintf(stderr, "CHYBA: Počet iterací musí být celé nezáporné číslo menší než 4 294 967 295.\n");
			break;		
		case 5:
			fprintf(stderr, "CHYBA: Pro výpočet exponenciální funkce musí být zálkad nezáporný.\n");
			break;
	}
	fprintf(stderr, "-----------------------------PARAMETRY-PRO-SPUŠTENÍ------------------------------\n"
	      		    "proj2.c --log [X] [N]                  X - logaritmus z čísla X (X > 0)\n"
 	  		 	    "                                       N - počet iterací (0 < N > 4 294 967 295, celé číslo)\n"
 	   	            "\n"
 			  	    "proj2.c --pow [X] [Y] [N]              X - základ mocniny (X > 0)\n"
 			 	    "                                       Y - exponent\n"
			 	    "                                       N - počet iterací  (0 < N > 4 294 967 295, celé číslo)\n"
				    "\n"
				    "proj2.c --mylog [X]                    X - logaritmus z čísla X\n"
				    "\n"
				    "proj2.c --mypow [X] [Y]                X - základ mocniny\n"
				    "                                       Y - exponent\n"
			  	    "Program neakceptuje destionou čárku. Pouze desetinou tečku.\n");
}

/**
 * FCE KONTROLY ARGUMENŮ A NAČÍTÁNÍ ČÍSELNYCH HODNOT Z NICH
 * ########################################################
 * VSTUP:
 *		argc - počet arg.
 *		*argv[] - jednotlivé arg.
 *		a[] - pro uločení číselné hodnoty z arg.
 * VÝSTUP:
 *		Fce vrací v případě uspěchu 0(SUCCESS), v opačném případě 1(ERR_ARG)
 */
int load(int argc, char *argv[], double a[])
{
	char *res;	// Zde se uloží případný zbytek po strtod

	// Kontrola počtů arg
	if(argc < 3 || argc > 5)
		return ERR_CNT;

	// Načítání číselných hodnot
	for(int i = 0; i < argc-2; i++)
	{
		a[i] = strtod(argv[i+2], &res);
		// Kontrola, zda se jedná pouze o čísla
		if(strcmp(res, "\0"))
			return ERR_ISNUM;
	}
	if(strcmp(argv[1], "--mylog") == 0 || strcmp(argv[1],"--mypow") == 0)
		return SUCCESS;
	// Kontrola, zad-li je počet iterací selé číslo
	if((a[argc-3]) != (unsigned int)a[argc-3])
		return ERR_ITERATION;

	return SUCCESS;
}

/**
 * FCE KONTROLY SPECIÁLNÍCH HODNOT PRO LOG
 * #######################################
 */
bool chack_log(double *x)
{
	if(isnan(*x))
		*x = NAN;
	else if(*x == 0.0)
		*x = (-INFINITY);
	else if(isinf(*x) == 1)
		*x = INFINITY;
	else
		return true;	// x se ponechá, prošlo kontrolou

	return false;
}

/**
 * FCE KONTROLY SPECIÁLNÍCH HODNOT PRO POW
 * #######################################
 */
bool chack_pow(double *x, double y)
{
	if(y == 0 || (*x == 1 && (fabs(isinf(y))==1 || isnan(y))))
		*x = 1;
	else if(y == 1)
		return false;	// x se ponechá
	else if((*x == 0 && y > 0) || (isinf(*x) == 1 && y < 0))
		*x = 0;
	else if((fabs(isinf(*x)) == 1 && y > 0) || (*x == 0 && y < 0))
		*x = INFINITY;
	else if(isnan(*x) || isnan(y))
		*x = NAN;
	else if(*x < 0 && isinf(*x) != -1)
		*x = -NAN;
	else 
		return true;	// x se ponechá prošlo kontrolou
	
	return false;
}

/** FCE ROZHODUJE O ZNAMÉNKU PŘI EXP. FCI
 * ######################################
 * VSTUP:
 * 		result - výsledek u kterého chceme rozhodnout o znaménku
 * 		y - exponent
 * 		x - základ
 * VÝSTUP:
 * 		Fce vrací výsledek s přispůsobeným výsledkem
 */
double sign(double result, double y, double x)
{
	double tmp = fabs(y/2);
	tmp = tmp - (int)tmp;
	if(fabs(tmp) <= 1.5 && fabs(tmp) !=0  && x < 0 )
		return -result;
	else
		return result;
}

/**
 * FCE PRO VÝPOČET LOGARITMU POMOCÍ ZŘETĚZENÉHO ZLOMKU
 * ###################################################
 * VSTUP:
 *		x - logaritmus u čísla x
 *		n - počet iterací
 * VÝSTUP:
 * 		Fce vrací výsledek logaritmu, v opačném případě
 */
double cfrac_log(double x, unsigned int n)
{
	// Mezní případy
	if(!chack_log(&x))
		return x; 	// x je v tuto chvíli pozměněné
	if(x < 0.0)
		return NAN;
	if(n == 0)
		return 0;

	double z = (x - 1.0)/(x + 1.0);
	double z2 = z * z;
	double a = (2*(n)-1);
	for(int i = n-1; i > 0; i--)   // 1, protože první iterace je konstantní
	{
		a = (2.0*i - 1.0) - ((i*i*z2)/a);
	}
	return (2.0*z)/a;
}

/**
 * FCE PRO VÝPOČET LOGARITMU POMOCÍ TAYLOROVA POLYNOMU
 * ###################################################
 * VSTUP:
 * 		x - logaritmus z čísla x
 * 		n - počet iterací
 * VÝSTUP:
 * 		Fce vrací výsledek logaritmu.
 */
double taylor_log(double x, unsigned int n)
{
	// Mezní případy
	if(!chack_log(&x))
		return x; 	// x je pozměněné
	if(x < 0.0)
		return NAN;

	double q = 0.0;
	double s = 0.0;
	double y = 0.0;
	double tmp = 1.0;
	if(x < 1.0)
	{
		y = 1.0 - x;
		q = y;
		for(unsigned int i = 0; i < n; i++)
		{
			s = s - (q)/(i+1.0);
			q = q * y;
		}
	}
	else
	{
		q = (x-1.0)/x;
		for(unsigned int i = 0; i < n; i++)
		{
			tmp = tmp * q;
			s = s + (tmp/(i+1.0));
		}
	}
	return s;
}

/**
 * FCE PRO VÝPOČET EXPONENCIÁLNÍ FUNKCE ZA POUŽÍTÍ TAYLOROVA POLYNOMU
 * ##################################################################
 * VSTUP:
 *		x - základ exponenciální fce
 * 		y - exponent
 *		n - počet iterací
 * VÝSTUP:
 *		Fce vrací výsledek exponenciální fce. 
 */
double taylor_pow(double x, double y, unsigned int n)
{
	if(!chack_pow(&x, y) || y == 1)
		return x; 	// x je pozměněné
	if(n == 0)
		return 0;
	double auxil = x;
	double s = 1;
	double q;
	double tmp = 1;
	if(x < 1)
		x = 1/x;
	q = fabs(y) * taylor_log(x, n);
	for(unsigned int i = 0; i < (n-1); i++)	// -1, protože první iterace je konstatní(1)
	{
		tmp = tmp*(q/(i+1));
		s = s + tmp;
	}
	if(isnan(s))
		return 0;
	if(y < 0 || auxil < 1)
		return 1/s;
	return s;
}

/**
 * FCE PRO VÝPOČET EXPONENCIÁLNÍ FUNKCE ZA POUŽÍTÍ ZŘETĚZENÉHO ZLOMKU
 * ##################################################################
 * VSTUP:
 *		x - základ exponenciální fce
 * 		y - exponent
 *		n - počet iterací
 * VÝSTUP:
 *		Fce vrací výsledek exponenciální fce. 
 */
double taylorcf_pow(double x, double y, unsigned int n)
{
	if(!chack_pow(&x, y) || y == 1)
		return x;	// x je pozměněno
	if(n == 0)
		return 0;
	double auxil = x;
	double s = 1;
	double q;
	double tmp = 1;
	if(x < 1)
		x = 1/x;
	q = fabs(y) * cfrac_log(x, n);
	for(unsigned int i = 0; i < (n-1); i++) 	// -1, protože první iterace je konstatní
	{
		tmp = tmp*(q/(i+1));
		s = s + tmp;
	}
	if(isnan(s))
		return 0;
	if(y < 0 || auxil < 1)
		return 1/s;
	return s;
}

/*
 * FCE KONTROLUJÍCÍ PŘESNOST NA 8 PLATNÝCH ČÍSLIC
 * ##############################################
 *
 * VSTUP:
 * 		x - výsledek aktuální iterace 
 * 		y - výsledek předchozí iterace
 * VÝSTUP:
 * 		True pokud je rozdíl zanedbatelný, jinak false
 */
bool acc(double x, double y)
{
	if(y == 0.0)
		return true;
	if(fabs(x-y) >= fabs(x*ACCURACY))
	{
		return false;
	}
	else
		return true;
}

/**
 * FCE PRO VÝPOČET LOGARITMU S PŘESNOSTÍ NA 8 PLATNÝCH ČÍSEL
 * #########################################################
 * VSTUP:
 *		x - logaritmus 	Z čísla x
 * VÝSTUP:
 * 		Fce vrací výsledek logaritmu
 * ---------------------------------------------------------------
 *  !!POZNÁMKA!!  - Při extrémních výpočtech může dojít k odchylce
 *  		 	    viz mypow.
 */
double mylog(double x)
{
	if(!chack_log(&x))
		return x;  // Je pozměněné

	double tmp = fabs(x);
	double s1 = tmp;
	double s2 = tmp;
	double z = (tmp - 1.0)/(tmp + 1.0);
	double z2 = z * z;
	double a;
	unsigned int n = 1;
	do{
		s2 = s1;
		a = 1.0;
		for(unsigned int i = (n-1); i > 0; i--)   // 1, protože první iterace je konstantní
		{
			a = (2.0*i - 1.0) - ((i*i*z2)/a);
		}
		s1 = (2.0*z)/a;
		n++;
	}while(!acc(s1, s2));
	return s1;

}

/**
 * FCE PRO VÝPOČET FCE SINUS
 * #########################
 * VSTUP:
 * 		x - hodnota pro sin
 * VÝSTUP:
 * 		Výsledek
 */
double sinx(double x)
{
	double i = 1;
	double eps = 10e-9; // Projistotu 9, stačilo by 8
	double s, tmp;
	if(fabs(isinf(x)) == 1)
		return INFINITY;
	if(x > 2*PI)
	{
		tmp = x/(2*PI);
		x = (tmp - (int)(tmp)) * 2*PI;
	}
	tmp = x;
	s = tmp;
	while(fabs(tmp) > eps)
	{
		i = i + 2;
		tmp = tmp * (-(x*x)/(i*(i-1)));
		s = s + tmp;
	}
	return s;
}

/**
 * FCE PRO VÝPOČET EXPONENCIÁLNÍ FUNKCE S PŘESNOSTÍ NA 8 PLATNÝCH ČÍSEL
 * ####################################################################
 * VSTUP:
 *		x - základ exponenciální fce
 * 		y - exponent
 * VÝSTUP:
 *		Fcevrací výsledek exponenciální fce. 
 *-----------------------------------------------------------------------------------
 *							!!!POZNÁMKA!!!
 * Oproti funkci mylog se průběžné výsledky ve funkci mypow mění velice pomalu,
 * a tak může nastat, že se dva po sobě jdoucí výsledky budou shodovat na
 * x-té platné číslici, ale nebudou odpovídat správnému výsledku.
 * V důsledku toho jsem zavedl proměnnou ABORT, díky niž se výpočet ukončí až
 * tehdy, když se bude 10 po sobě jdoucích výsledků shodovat na x-tém platmén
 * čísle. Hodnota ABORT = 10 stačí pro jednoduché výpočty. Pro velké by se ale
 * musela zvýšit, což by ale bylo pro triviální výpočty zpomalující.
 * (Je možné, že podobný problém se může objevit i ve funkci mylog, ale podle doposud
 * získaných dat by tato chyba neměla nastat.)
 */
double mypow(double x, double y)
{
	// Mezní přpady
	if(y == 0 || (x == 1 && (fabs(isinf(y))==1 || isnan(y))))
		return 1;
	else if(isinf(x) == 1 && isinf(y) == -1)
		return 0;
	else if(y == 1 && fabs(isinf(y)) != 0)
		return x;
	else if((x == 0 && y > 0) || (isinf(x) == 1 && y < 0))
		return 0;
	else if((isinf(x) == 1 && y > 0) || (x == 0 && y < 0))
		return INFINITY;
	else if(isnan(x) || isnan(y))
		return NAN;

	double auxil = x;
	double s1 = 1;
	double s2 = s1;
	double q, tmp;
	unsigned int n = 2;
	int abort = 0;
	if(x <1)
		x = 1/x;
	q = fabs(y) * mylog(x);
	do{
		tmp = 1;
		s2 = s1;
		s1 = tmp;
		for(unsigned int i = 0; i < (n-1); i++) 	//první iterace ej konstantní, proto (n-1)
		{
			tmp = tmp*(q/(i+1));
			s1 = s1 + tmp;
		}
		if(acc(s1, s2))
			abort++;
		n++;
	}while(abort < 10);
	if(y < 0)
		s1 = 1/s1;
	if(auxil < 1)
		return 1/s1;
	return s1;

}
