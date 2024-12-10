/*
 * jobSelector.hpp
 * Hlavičkový soubor pro modul přidělující práci zaměstnancům.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _JOBSELECTOR_HPP_
#define _JOBSELECTOR_HPP_

#include "../lib/simlib.h"
#include <stdio.h>
#include "timing.hpp"        // časové konstanty
#include "systemVar.hpp"     // hodnoty vlastvostí systému

// Funkce zadá další práci Dřevaři
void GetNextWorkWoodWorker();

// Funkce zadá další práci Zámečníkovi
bool GetNextWorkSteelWorker();

// Funkce zadá další práci Dřevaři, nebo Zámečníkovi
void GetNextWorkAllWorker();

/*
 * Událost implementující pracovní hodiny zaměstnanců
 */
class WorkingHours: public Event
{
    void Behavior();
};

#endif
