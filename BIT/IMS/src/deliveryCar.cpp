/*
 * deliveryCar.hpp
 * Hlavičkový soubor monulu auta odvážejícího hotové lavičky.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "deliveryCar.hpp"

// Linka auta odvážejícího hotové lavičky.
Facility pickup("Dodávka pro odvoz laviček");

// Fronta hotových laviček čekajících na naložení
Queue benchesWaitingForDeparture("Hotové lavičky čekající na odvezení");

// Proces poruchy auta a informace o servisování
CarMalfunction *malfunction;
bool isServiced = false;

// Proces autoa s volnou kapacitou
Car *departure = new Car;
int capacity = CAR_FREE_SPACE; 

/*
 * Definice procesu auta jedoucího s hotovými lavičkami.
 * Auto se může během používání pokazit.
 * V případě poruchy se opravuje a je nedostupné.
 * Porucha při odvážení nemá na odvoz laviček vliv.
 */
void Car::Behavior() 
{
  while (1) 
  {
    // Jízda auta
    Seize(pickup);
    Wait(2*HOUR);
    
    // Pokud je auto pokažené, bude se opravovat
    if(isServiced)
    {
        Wait(5*DAY);
        isServiced = false;
        // Započetí odpočtu k nové poruše
        malfunction->Activate(Time+Exponential(CAR_ERR_RATE));
    }
    // Vyprázdnění auta
    capacity = CAR_FREE_SPACE;
    Release(pickup);
    
    // Zahájíme nakládáni laviček
    if(benchesWaitingForDeparture.Length() > 0)
    {
        benchesWaitingForDeparture.GetFirst()->Activate();
    }  
    Passivate();
  }
}

/*
 * Proces nakládáni hotové lavičky.
 * Pokud není místo na korbě, vloží se lavička do fronty.
 */
void FinishedBench::Behavior() 
{
    while(1)
    {
        // Snaha o vložení lavičky na korbu
        if(capacity > 0)
        {
            Seize(pickup);
            Wait(30);
            capacity--;
            Release(pickup);
            
            if(capacity == 0)
            {
                // Auto odjíždí
                departure->Activate();
            }
            else 
            {
                // Pokračujeme v nakládání čekajících laviček
                if(benchesWaitingForDeparture.Length() > 0)
                {
                    benchesWaitingForDeparture.GetFirst()->Activate();
                }
            }
            break; 
        }
        else
        {
            Into(benchesWaitingForDeparture);
            Passivate();
        }
    }
}


/*
 * Definice poruchy auta
 */
void CarMalfunction::Behavior() {
   
    isServiced = true;
    malfunction = this;   
}
