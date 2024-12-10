/*
 * woodPrepare.hpp
 * Hlavičkový soubor pro modul implementující opracovávání (hoblování) břeva.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _WOOD_PREPARE_
#define _WOOD_PREPARE_

#include <stdio.h>
#include "../lib/simlib.h"
#include "timing.hpp"       // časové konstanty
#include "woodPaint.hpp"    // proces malování dřeva
#include "jobSelector.hpp"  // zaměstnanci
#include "systemVar.hpp"    // hodnoty vlastvostí systému

/*
 * Proces opracovávání dřeva
 */ 
class woodPrepare: public Process
{
    void Behavior();
};


#endif