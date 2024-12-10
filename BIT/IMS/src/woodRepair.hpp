/*
 * woodRepair.hpp
 * Hlavičkový soubor pro modul opravující špatně nalakované dřeva.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _WOOD_REPAIR_
#define _WOOD_REPAIR_

#include "../lib/simlib.h"
#include "timing.hpp"       // časové konstanty
#include <stdio.h>          // ladící výstupy
#include "jobSelector.hpp"  // zaměstnanci
#include "woodPaint.hpp"    // znovunalakování

/*
 * Proces opravy špatně nalakovaného dřeva
 */
class woodRepair: public Process
{
    void Behavior();
};


#endif