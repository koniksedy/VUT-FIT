/*
 * woodPrepare.cpp
 * Modul implementující opracovávání (hoblování) břeva.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "woodPrepare.hpp"

// Zaměstnanci
extern Store woodWorkers;

// Fronta dřeva čekajícího na zpracování
Queue woodToBePrepared("Fronta dřeva čekajícího na opracování");

int woodThrowOut = 0;
int woodProcessed = 0;

/*
 * Proces opracovávání dřeva
 */ 
void woodPrepare::Behavior()
{
	while(1)
	{
		if(!woodWorkers.Full())
		{
			woodProcessed++;
			// Pracovník zabrán
			Enter(woodWorkers, 1);
			// Pokud je dřevo z vadou, nebo jej zaměstnanec
			// poškodí, pak se čeká přiměřenou dobu, která simuluje
			// objejvení chyby.
			if(Random() <= WOOD_RUBBISH)
			{
				Wait(Uniform(0, WOOD_PROCESS_MAX));
				Leave(woodWorkers, 1);
            	GetNextWorkWoodWorker();
				woodThrowOut++;
            	break;
			}
			else
			{	
				Wait(Uniform(WOOD_PROCESS_MIN, WOOD_PROCESS_MAX));
            	Leave(woodWorkers, 1);
            	GetNextWorkWoodWorker();
				(new woodPaint) -> Activate();
				break;
			}
		}
		else
		{
			Into(woodToBePrepared);
			Passivate();
		}
	}
}




