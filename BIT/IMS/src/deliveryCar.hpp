/* 
 * deliveryCar.hpp
 * Hlavičkový soubor monulu auta odvážejícího hotové lavičky.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _DELIVERYCAR_HPP_
#define _DELIVERYCAR_HPP_

#include <stdio.h>          // ladící hláčky
#include "../lib/simlib.h"
#include "timing.hpp"       // časové konstanty
#include "systemVar.hpp"    // hodnoty vlastvostí systému


/*
 * Proces auta jedoucího k odběrateli.
 */
class Car: public Process {

    void Behavior();
};


/*
 * Proces poruchy auta, která mu zabrání v novém odvozu laviček.
 */
class CarMalfunction: public Event {
	
    void Behavior();
};


/*
 * Proces nakládání hotových laviček
 */
class FinishedBench : public Process {

	void Behavior();
};

#endif
