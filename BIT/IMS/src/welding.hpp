/*
 * welding.hpp
 * Hlavičkový soubor modulu definujícího přídu procesu svařování kovových dílů.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _WELDING_HPP_
#define _WELDING_HPP_

#include "../lib/simlib.h"
#include "timing.hpp"       // časové konstanty
#include "jobSelector.hpp"  // zaměstnanci
#include "assembling.hpp"   // kompletace laviček
#include "assemblyController.hpp"   // čítač svařených stojek
#include "systemVar.hpp"            // hodnoty vlastvostí systému

/*
 * Třída procesu svařování kovových stojek
 */
class Welding: public Process
{
    void Behavior();
};

#endif

