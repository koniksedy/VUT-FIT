/*
 * woodRepair.cpp
 * Modul implementující opravu špatně nalakované dřeva.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "woodRepair.hpp"

// Pracovníci
extern Store woodWorkers;

// Fronta špatně nalakovaného dřeva čekajícího na opravu (na zaměstnance)
Queue woodToBeRepaired("Fronta dřeva čekajícího na opravu laku");

/*
 * Proces opravy špatně nalakovaného dřeva
 */
void woodRepair::Behavior()
{
	while(1)
	{
		if(!woodWorkers.Full())
		{
			// Oprava dřeva
			Enter(woodWorkers, 1);
			Wait(Uniform(1*HOUR,1.5*HOUR));
			Leave(woodWorkers, 1);
		    GetNextWorkWoodWorker();
			// Znovu lakování
		    (new woodPaint)->Activate();
		    break;
		}
		else
		{
			Into(woodToBeRepaired);
			Passivate();
		}
	}
	
}
