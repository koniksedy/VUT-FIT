/**
 * tail2.cc
 * Řešení IJC-DU2, příklad ab), 29.3.2019
 * Autor: Michal Šedý, FIT
 * Přeloženo: g++ 5.5.0
 * Popis: Zdrojový soubor programu tail2, napsaného v jazyce c++,
 *		  který ze zadaného vstupního souboru vytiskne posledních 10 řádků.
 *		  Není-li zadán vstupní soubor, čte ze stdin. Je-li programu zadán
 *		  parametr -n číslo, bude se tisknout tolik posledních řádků,
 *		  kolik je zadáno parametrem 'číslo' > 0.
 *		  Je-li programu zadán parametr -n +číslo, bude se tisknout vše
 *		  od řádku 'číslo' > 0.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <limits>

using namespace std;

/**
 * FCE pro detekcei znaku +
 * Vstup - chr - znak pro otestování
 * Výstup - fce vrací true pokud byl detekován znak + a nastaví chr na 0, jinak vrací false
 */
bool is_plus(char *chr)
{
	if(*chr == '+')
	{
		*chr = '0';
		return true;
	}
	else
		return false;
}

/**
 * Fce převede řetězec na číslo a kontroluje, zda je větší než 1
 * Vstup - str - řetězec pro převod
 * Výstup - přvedená hodnota, v případě chyby vrací hodnotu 0
 */
unsigned parse_stringtoint(char *str)
{
	int n;
	stringstream parse(str);
	parse >> n;
	if(parse.good() || n <= 0)
	{
		throw string("chybný počet řádků: ") + string(str);
		return 0;
	}

	return unsigned(n);
}

int main(int argc, char *argv[])
{
	std::ios::sync_with_stdio(false);
	unsigned n = 10;
	bool plus = false;
	istream *input = &cin;
	ifstream file;
	string tmp_line;
	queue<string> lines;

	// Zpracovávání argumentů 
	try
	{
		// Kontrola počtu argumentů
		if(argc > 4)
		{
			throw string("príliš mnoho argumentů.");
		}

		//NAČÍTÁNÍ ČÍSLA při PŘEPÍNAČI -n
		//argc = 3 || 4 -> .tail -n číslo ...
		if(argc == 3 || argc == 4)
		{
			// Kontrola, zda je zadán přepínač -n
			if(string(argv[1]) != "-n")
			{
				throw string("neznámý přepínač: ") + string(argv[1]);
			}

			//Kontrola parametru + např. +10
			plus = is_plus(&argv[2][0]);

			// Převod stringu na číslo a kontrola
			n = parse_stringtoint(argv[2]);
		}

		// NAČÍTÁNÍ SOUBORU
		// argc = 2 tail soubor
		// argc = 4 -> tail -n cislo soubor
		if(argc == 2 || argc == 4)
		{
			file.open(argv[argc-1], ios::in);
			if(!file)
			{
				throw string(argv[argc-1]) + string(" nelze otevřít pro čtení: Adresář, nebo soubor neexistuje");
			}
			input = &file;
		}
	}
	// Kontrola, zda nedošlo k chybě
	catch (string errmsg)
	{
		cerr << "tail: " << errmsg  << endl;
		return 1;
	}
	
	// A) VÝPIS POSLEDNÍCH N ŘÁDKŮ
	//----------------------------
	if(!plus)
	{
		// Inicializace lines
		for(unsigned i = 0; i < n; i++)
		{
			lines.push("");
		}

		// Načítání
		while(getline(*input, tmp_line))
		{
			tmp_line += "\n";
			lines.push(tmp_line);
			lines.pop();
		}

		// Výpis
		while(!lines.empty())
		{
			cout << lines.front();
			lines.pop();
		}
	}
	// B) VÝPIS OD ŘÁDKU N
	//--------------------
	else
	{
		// Odřádkování
		for(unsigned i = 0; i < n-1; i++)
		{
			input->ignore(numeric_limits<streamsize>::max(),'\n');
		}

		// Výpis 
		while(getline(*input, tmp_line))
		{
			cout << tmp_line << endl;
		}
	}

	// Pokud bylo čteno ze souboru uzavřít
	if(file.is_open())
	{
		file.close();
	}

	return 0;
}
