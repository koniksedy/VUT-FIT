/*
 * assemblyController.cpp
 * Hlavičkový soubor modulu kontrolujícího správné množství
 * materiálnů pro zahájení sestavování lavičky.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "assemblyController.hpp"

// Počty materiálů dostupných pro stavění lavičky
int standReadyCnt = 0;
int woodReadyCnt = 0;

// Existuje pouze jeden kontroller
AssemblyController *assemblyController = new AssemblyController;

/*
 * Kontrola, zda je dostupné požadované množství materiálu
 * pro tvorbu lavičky.
 */
void AssemblyController::Behavior(){

	if(standReadyCnt >= 2 && woodReadyCnt >= 1)
	{
		standReadyCnt -= 2;
		woodReadyCnt--;
		(new Assembling)->Activate();
	}
}
