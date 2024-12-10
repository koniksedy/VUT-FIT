/*
 * assembling.cpp
 * Modulu implementující sestavování laviček.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "assembling.hpp"

// Pracovníci
// Lavičky mohou skládat všichni pracovníci,
// každý typ s jinou šikovností.
extern Store steelWorkers;
extern Store woodWorkers;

// Fronta laviček ke složení čekajících na pracovníka
Queue benchToBeAssembled("Fronta laviček čekajících na složení.");

// Kontroler, který bude uvědomen v případě uvolnění stojek
// z důvodu poškození lavičky během montáže.
extern AssemblyController *assemblyController;

// Pro vrácení stojky v případě zničení dřeva
extern int standReadyCnt;

// Počet smontovaných laviček za den
int benchDone = 0;
extern int woodThrowOut;

/*
 * Proces skládání lavičky.
 * Lavičky mohou skládat všichni pracovníci,
 * každy typ pracovníku ale s jinou šikovností.
 */
void Assembling::Behavior()
{
	while(1)
	{
	    if(!steelWorkers.Full())
		{
			// Zabrání Dřevaře
	    	Enter(steelWorkers);
			
			// Pravděpodobnost zničení lavičky během montáže
	    	if(Random() <= SW_ASSEMBLING_RUBBISH)
			{
				// Pracovník bude lavičku rozdělávat stejně dlouho, jako ji skládat
				// do poškození. K době rozdělávání se připočítává i doba odepisovábní.
	    		double workTime = Uniform(0, SW_ASSEMBLING_MAX);
	    		Wait(2*workTime + 10*MIN);

				// Informuje assemblyControler o uvolnení dvou stojek
	    		standReadyCnt += 2;	
				woodThrowOut++;
				assemblyController->Activate();
	    	}
			else
			{
				// Úspěšná montáž lavičky
	    		Wait(Uniform(SW_ASSEMBLING_MIN, SW_ASSEMBLING_MAX));
	    		(new FinishedBench)->Activate();
				benchDone++;
	    	}

			// Uvolnění Dřevaře
	    	Leave(steelWorkers);	  
	    	GetNextWorkSteelWorker();
	    	break;
	    }
	    else if(!woodWorkers.Full())
		{
			// Zabrání zámečníka
	    	Enter(woodWorkers);

			// Pravděpodobnost zničení lavičky během montáže
	    	if(Random() <= WW_ASSEMBLING_RUBBISH)
			{
				// Pracovník bude lavičku rozdělávat stejně dlouho, jako ji skládat
				// do poškození. K době rozdělávání se připočítává i doba odepisovábní.
	    		double workTime = Uniform(0, WW_ASSEMBLING_MAX);
	    		Wait(2*workTime + 10*MIN);

				// Informuje assemblyControler o uvolnení dvou stojek
	    		standReadyCnt += 2;
				woodThrowOut++;
				assemblyController->Activate();
	    	}
			else
			{
				// Úspěšná montáž lavičky
	    		Wait(Uniform(WW_ASSEMBLING_MIN, WW_ASSEMBLING_MAX));
	    		(new FinishedBench)->Activate();
				benchDone++;
	    	}

			// Uvolnění zámečníka
	    	Leave(woodWorkers); 
	    	GetNextWorkWoodWorker();
	    	break;
	    }
	    else
		{
			// Materiál čeká na pracovníka
			Into(benchToBeAssembled);
			Passivate();
	    }
	}
}
