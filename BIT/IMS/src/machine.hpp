/*
 * machine.hpp
 * Hlavičkový soubor definicí třídu pro modulární stroj,
 * který 
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _MACHINE_HPP_
#define _MACHINE_HPP_

#include <stdio.h>          // ladící výpisy
#include "../lib/simlib.h"
#include "timing.hpp"       // časové konstatnty
#include "welding.hpp"      // svařování kovů
#include "systemVar.hpp"            // hodnoty vlastvostí systému


// Definice maker pro odličení jednotlivých strojů
#define PLATE_SM_CUTTER 0
#define PLATE_L_CUTTER 1
#define ROD_CUTTER 2
#define PLATE_SM_PRESS 3
#define PLATE_L_PRESS 4
#define ROD_PRESS 5

/*
 * Třída modulárního stroje, u kterého se mohou specifikovat nejrůznější vlastnosti.
 * Poruchovost, doba opravy, zmektovitost, počet výstupních kusu, doba obsluhy ...
 */ 
class Machine: public Process
{
    private:
        int easyRepairDuration;         // doba opravy lehké chyby
        int hardRepairDuration;         // doba opravy vážné chyby
        double easyErrorProbability;    // pravděpodobnost lehké chyby
        double hardErrorProbability;    // pravděpodobnost vážné chyby
        double rubbishProbability;      // zmetkovitost
        double totalErrorProbability;   // celková poruchovost stroje
        int elementaryWorkDuration;     // doba obsluhy pro jeden výsledný produkt
        int workingPlane;               // plán výsledných produktů (vstup 1---> vystup N)
        short currentMachine;           // identifikace konkrétního stroje
        Facility *facility;             // ukazatel na linku stroje
        Queue *waitingForWokerQueue;    // ukazatel na frontu materiálu čekajícího na zaměstnance
        double startTime;               // začátek práce na stroji

        void Behavior();
        void Repairing(bool isEasy);    // Oprava pokaženého stroje
        void GenerateNewProduct();      // Vygenerování výsledkého prduktu
        void TryToWeld();               // Kontrola, zda jemožné výsledné produkty svařit.
    
    public:
        /*
         * Metoda vytvářenící materiál pro linku.
         * ed - doba opravy lehké chyby
         * hD - doba opravy vážné chyby
         * eP - pravděpodobnost lehké chyby
         * hP - pravděpodobnost vážné chyby
         * rP - zmetkovitost
         * wD  - doba obsluhy pro jeden výsledný produkt
         * Plane - plán výsledných produktů (vstup 1---> vystup N)
         * Machine - identifikace konkrétního stroje
         * f - ukazatel na linku stroje
         * w - ukazatel na frontu materiálu čekajícího na zaměstnance
         */ 
        Machine(int eD, int hD,
               double eP, double hP,
               int wD, double rP, int plane,
               short machine, Facility *f, Queue *w);
};

#endif
