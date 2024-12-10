/*
 * assemblyController.hpp
 * Hlavičkový soubor modulu kontrolujícího správné množství
 * materiálnů pro zahájení sestavování lavičky.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _ASSEMBLYCONTROLLER_HPP_
#define _ASSEMBLYCONTROLLER_HPP_

#include "../lib/simlib.h"
#include "assembling.hpp"   // proces tvorby laviček

/*
 * Třída kontrolleru správného množství materiálů
 * pro zahájení sestavování lavičky.
 */
class AssemblyController: public Event
{
    void Behavior();
};

#endif