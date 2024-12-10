/*
 * woodPaint.hpp
 * Hlavičkový soubor modulu pro malování dřeva.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _WOOD_PAINT_
#define _WOOD_PAINT_

#include <stdio.h>          // ladící výpis
#include "../lib/simlib.h"
#include "timing.hpp"       // časové konstanty
#include "jobSelector.hpp"  // zaměstnanci
#include "woodRepair.hpp"   // oprava špatně zaschlého laku
#include "systemVar.hpp"    // hodnoty vlastvostí systému
#include "assemblyController.hpp"   // informování controlleru o nevě vzniklém dřevě

/*
 * Proces malování dřeva.
 */
class woodPaint: public Process
{
    void Behavior();
};


#endif