/*
 * materialGenerator.hpp
 * Hlavičkový soubor s definicemi tríd generujícich surový materiál.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _MATERIAL_GENERATOR_
#define _MATERIAL_GENERATOR_

#include <stdio.h>
#include "../lib/simlib.h"
#include "timing.hpp"               // časové konstanty
#include "getGoodsFromTruck.hpp"    // procesy skládání zboží z korby
#include "systemVar.hpp"            // hodnoty vlastvostí systému

/*
 * Generátor surového dřeva.
 */
class WoodGenerator: public Event
{
    virtual void Behavior();
};

/*
 * Generátor svazků surových kovových prutů
 */
class SteelRodGenerator: public Event
{
    virtual void Behavior();
};

/*
 * Generátor velkého surového plechu.
 */
class SteelPlateGenerator: public Event
{
    virtual void Behavior();
};

/*
 * Generátor malých surových kovových plátů.
 */
class SteelThingGenerator: public Event
{
    virtual void Behavior();
};

#endif