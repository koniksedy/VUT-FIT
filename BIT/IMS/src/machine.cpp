/*
 * machine.cpp
 * Modul implementující třídu pro modulární stroj,
 * který 
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "machine.hpp"

// Zaměstnanci obsluhující stroje
extern Store steelWorkers;

// Stroje - řezačky
Facility plateLCutter("Řezčka velkého plechu.");
Facility plateSMCutter("Řezačka malých tlustých plátů.");
Facility rodCutter("Řezačka prutů.");

// Stroje - lisy
Facility platePress("Společný list nařezaných plechů.");
Facility rodPress("List nařezaných prutů");

// Fronty materiálů na řezačkách čekajících na pracovníky
Queue plateLCutterWaitingForWorker("Fronta materiálu čakajícího na řezačce velkého plechu");
Queue plateSMCutterWaitingForWorker("Fronta materiálu čakajícího na řezačce tlustých plátů");
Queue rodCutterWaitingForWorker("Fronta materálu čakajícího na řezačce řezačkou prutů");

// Fronty materálů na lisech čekajících na pracovníky
Queue plateLPressWaitingForWorker("Fronta materiálu čakajícího na lisu kusů velkého plechu");
Queue plateSMPressWaitingForWorker("Fronta materiálu čakajícího na lisu kusů tlustých plátů");
Queue rodPressWaitingForWorker("Fronta materuálu čakajícího na lisu lisem prutů");

// Časy na strojích
Stat plateLCutterWorkingTime("Pracovní čas na řezačce velkého plechu");
Stat plateSMCutterWorkingTime("Pracovní čas na řezačce tlustých plátů");
Stat roodCutterWorkingTime("Pracovní čas na řezačce prutů");
Stat platePressWorkingTime("Pracovní čas na lisu libovolných plátů");
Stat roodPressWorkingTime("Pracovní čas na lisu prutů");

// Produkty lisů
int platesLToWelding = 0;
int platesSMToWelding = 0;
int rodsToWelding = 0;

// Čítač produktů
int platesLAfetrCutting = 0;
int platesSMAfterCutting = 0;
int rodsAfterCutting = 0;
int platesLAfterPressing = 0;
int platesSMAfterPressing = 0;
int rodsAfterPressing = 0;

// Čítače vstupů
int platesLBeforeCutting = 0;
int platesSMBeforeCutting = 0;
int rodsBeforeCutting = 0;
int platesLBeforePressing = 0;
int platesSMBeforePressing = 0;
int rodsBeforePressing = 0;


/*
 * Metoda vytvářenící materiál pro linku.
 * ed - doba opravy lehké chyby
 * hD - doba opravy vážné chyby
 * eP - pravděpodobnost lehké chyby
 * hP - pravděpodobnost vážné chyby
 * rP - zmetkovitost
 * wD  - doba obsluhy pro jeden výsledný produkt
 * Plane - plán výsledných produktů (vstup 1---> vystup N)
 * Machine - identifikace konkrétního stroje
 * f - ukazatel na linku stroje
 * w - ukazatel na frontu materiálu čekajícího na zaměstnance
 */ 
Machine::Machine(int eD, int hD,
                 double eP, double hP,
                 int wD, double rP, int plane,
                 short machine, Facility *f, Queue *w) : Process()
{
    this->easyRepairDuration = eD;
    this->hardRepairDuration = hD;
    this->easyErrorProbability = eP;
    this->hardErrorProbability = hP;
    this->totalErrorProbability = eP + hP;
    this->elementaryWorkDuration = wD;
    this->rubbishProbability = rP;
    this->workingPlane = plane;
    this->currentMachine = machine;
    this->facility = f;
    this->waitingForWokerQueue = w;
}

/*
 * Obsluha stroje
 */
void Machine::Behavior()
{   
    Seize(*(this->facility));
    while(1)
    {
        if(!steelWorkers.Full())
        {
            // Vyplnit počítadla vstupního materiálu
            switch (this->currentMachine)
            {
                case PLATE_L_CUTTER:
                    platesLBeforeCutting++;
                    break;
                
                case PLATE_SM_CUTTER:
                    platesSMBeforeCutting++;;
                    break;

                case ROD_CUTTER:
                    rodsBeforeCutting++;
                    break;
                
                case PLATE_L_PRESS:
                    platesLBeforePressing++;
                    break;

                case PLATE_SM_PRESS:
                    platesSMBeforePressing++;
                    break;
                
                case ROD_PRESS:
                    rodsBeforePressing++;
                    break;  
            }
            // Zámečník pracuje
            this->startTime = Time;
            Enter(steelWorkers, 1);
            break;
        }
        else
        {
            Into(*(this->waitingForWokerQueue));
            Passivate();
        }
    }

    // Stroj pracuje vygeneruje N výrobků
    for(int i = 0; i < this->workingPlane; i++)
    {
        
        Wait(this->elementaryWorkDuration);

        // Porucha
        double error = Random();
        if(error <= this->totalErrorProbability)
        {
            // Oprava poruchy
            Repairing(error <= this->easyErrorProbability);
            // U lisu je i přez poruchu vygenerovaný produkt.
            if(this->currentMachine == PLATE_L_PRESS || this->currentMachine == PLATE_SM_PRESS || this->currentMachine == ROD_PRESS)
            {
                // Kontrola zmetku
                if(Random() > this->rubbishProbability)
                {
                    GenerateNewProduct();
                }
                
                
            }
        }
        else
        {
            // Kontrola zmetku
            if(Random() > this->rubbishProbability)
            {
                GenerateNewProduct();
            }
        }  
    }

    // Poznamemání času stráveného prací na konkrétním stroji.
    switch (this->currentMachine)
    {
        case PLATE_L_CUTTER:
            plateLCutterWorkingTime(Time - this->startTime);
            break;
        
        case PLATE_SM_CUTTER:
            plateSMCutterWorkingTime(Time - this->startTime);
            break;

        case ROD_CUTTER:
            roodCutterWorkingTime(Time - this->startTime);
            break;
        
        case PLATE_L_PRESS:
            platePressWorkingTime(Time - this->startTime);
            break;

        case PLATE_SM_PRESS:
            platePressWorkingTime(Time - this->startTime);
            break;
        
        case ROD_PRESS:
            roodPressWorkingTime(Time - this->startTime);
            break;  
    }

    // Uvolnění pracovníka a linky
    Leave(steelWorkers, 1);
    GetNextWorkSteelWorker();
    Release(*(this->facility));
}


/*
 * Oprava stroje.
 * isEasy - booleovská hodnota, zda se jedná o jednoduchou chybu a snadnou poruchu.
 */
void Machine::Repairing(bool isEasy)
{
    if(isEasy)
    {
        Wait(this->easyRepairDuration);
    }
    else
    {
        Wait(this->hardRepairDuration);
    }
}


/*
 * Vygenerování produktu v závislosti na typu stroje.
 */
void Machine::GenerateNewProduct()
{
    switch (this->currentMachine)
    {
        case PLATE_L_CUTTER:
            // Zaregistrování materiálu pro zpracování na lisu.
            (new Machine(PRESS_PL_EDUR, PRESS_PL_HDUR, PRESS_PL_EE, PRESS_PL_HE,
                         PRESS_PL_DUR, PRESS_PL_RUBBISH, PRESS_PL_PLANE, PLATE_L_PRESS,
                         &platePress, &plateLPressWaitingForWorker))->Activate();
            platesLAfetrCutting++;
            break;
        
        case PLATE_SM_CUTTER:
            // Zaregistrování materiálu pro zpracování na lisu.
            (new Machine(PRESS_PSM_EDUR, PRESS_PSM_HDUR, PRESS_PSM_EE, PRESS_PSM_HE,
                         PRESS_PSM_DUR, PRESS_PSM_RUBBISH, PRESS_PSM_PLANE, PLATE_SM_PRESS,
                         &platePress, &plateSMPressWaitingForWorker))->Activate();
            platesSMAfterCutting++;
            break;

        case ROD_CUTTER:
            // Zaregistrování materiálu pro zpracování na lisu.
            (new Machine(PRESS_ROD_EDUR, PRESS_ROD_HDUR, PRESS_ROD_EE, PRESS_ROD_HE,
                         PRESS_ROD_DUR, PRESS_ROD_RUBBISH, PRESS_ROD_PLANE, ROD_PRESS,
                         &rodPress, &rodPressWaitingForWorker))->Activate();
            rodsAfterCutting++;
            break;
        
        case PLATE_L_PRESS:
            platesLToWelding++;
            TryToWeld();
            platesLAfterPressing++;
            break;

        case PLATE_SM_PRESS:
            platesSMToWelding++;
            TryToWeld();
            platesSMAfterPressing++;
            break;
        
        case ROD_PRESS:
            rodsToWelding++;
            TryToWeld();
            rodsAfterPressing++;
            break;

        default:
            fprintf(stderr, "ERR: Pozor stroj dostal určitě chybně zadaný typ.\n");    
    }
}


/*
 * Při každém vytvoření materiálu vhodného pro svařování je provedena
 * kontrola, zda již není možné použít N potřebných kusů na svaření.
 */
void Machine::TryToWeld()
{
    if(platesSMToWelding >= 2 && platesLToWelding >= 3 && rodsToWelding >= 1)
    {
        platesSMToWelding -= 2;
        platesLToWelding -= 3;
        rodsToWelding -= 1;
        (new Welding)->Activate();
    }
}