/*
 * getGoodsFromTruck.hpp
 * Hlavičkový soubor s definicemi tříd procesů obsluhujících
 * vyskládavání surového materiálu z korby dodavatele.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _GETGOODSFROMTRUCK_HPP_
#define _GETGOODSFROMTRUCK_HPP_

#include "../lib/simlib.h"
#include "jobSelector.hpp"  // zaměstnanci
#include "woodPrepare.hpp"  // zpracovávání dřeva
#include "timing.hpp"       // časové konstanty
#include "machine.hpp"      // modulární stroj
#include "systemVar.hpp"            // hodnoty vlastvostí systému

/*
 * Nošení surového dřeva do provozu.
 */ 
class WoodFromTruck: public Process
{
    void Behavior();
};

/*
 * Nošení surových svazků prutů do provozu.
 */ 
class SteelRodFromTruck: public Process
{
    void Behavior();
};

/*
 * Nošení surových velkých plechů do provozu.
 */
class SteelPlateFromTruck: public Process
{
    void Behavior();
};

/*
 * Nošení surových malých kovových plátů do porovozu.
 */
class SteelThingFromTruck: public Process
{
    void Behavior();
};

#endif