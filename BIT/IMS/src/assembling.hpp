/*
 * assembling.hpp
 * Hlavičkový soubor modulu pro sestavování laviček.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _ASSEMBLING_HPP_
#define _ASSEMBLING_HPP_

#include <stdio.h>          // ladící výpisy
#include "../lib/simlib.h"
#include "timing.hpp"       // časové konstanty
#include "jobSelector.hpp"  // pracovníci
#include "deliveryCar.hpp"  // odvor vyrobených laviček
#include "assemblyController.hpp"   // uvolnění stojek z poškozené lavičky
#include "systemVar.hpp"            // hodnoty vlastvostí systému


/*
 * Proces sestavování lavičky
 */
class Assembling: public Process
{
    void Behavior();
};

#endif
