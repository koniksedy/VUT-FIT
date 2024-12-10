/*
 * welding.cpp
 * Hlavičkový soubor modulu definujícího přídu procesu svařování kovových dílů.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "welding.hpp"

// Materiál čekající na pracovníka
Queue materialWaitingForWelding;

// Pracovníci
extern Store steelWorkers;

// Kontroler, který bude informován o vzniku nové stojky,
// aby rozhodl, zda již je dostupný potřebný počt pro kompletaci lavičky.
extern AssemblyController *assemblyController;

// Počítadlo dostupných stojek.
extern int standReadyCnt;


/*
 * Proces svařování.
 */
void Welding::Behavior()
{
    while(1)
    {
        if(!steelWorkers.Full())
        {
            // Zabrání pracovníka
            Enter(steelWorkers);
            Wait(Uniform(20 * MIN, 35 * MIN));

            // Kontrola zmetků
            // Zmetek se musí převařit
            // Znovu svařit kovy může jiný pravocník
            if(Random() <= WELD_RUBBISH)
            {
                Wait(Uniform(35 * MIN, 40 * MIN));
                Leave(steelWorkers);
                GetNextWorkSteelWorker();
                (new Welding)->Activate();
            }
            else
            {
                Leave(steelWorkers);
                GetNextWorkSteelWorker();
                standReadyCnt++;
                // Informování kontroleru o vytvoření stojky
                assemblyController->Activate();
            }
            break;
        }
        else
        {
            Into(materialWaitingForWelding);
            Passivate();
        }
    }
}