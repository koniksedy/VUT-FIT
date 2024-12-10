/*
 * woodPaint.hpp
 * Hlavičkový soubor modulu pro malování dřeva.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "woodPaint.hpp"

// Zaměstnanci
extern Store woodWorkers;

// Kontroler pro uvědomnění 
extern AssemblyController *assemblyController;

// Fronta dřeva pro namalování
Queue woodToBePainted("Fronta dřeva čekajícího na natření");

// Počet hotovách dřev
extern int woodReadyCnt;

/*
 * Proces malování dřeva.
 */
void woodPaint::Behavior()
{
	while(1)
	{
		if(!woodWorkers.Full())
		{

			Enter(woodWorkers, 1);

			Wait(Uniform(PAINT_MIN, PAINT_MAX));
			
			Leave(woodWorkers, 1);
        	GetNextWorkWoodWorker();
			
			// Schnutí dřeva
        	Wait(PAINT_DRYING);

        	if(Random() <= PAINT_RUBBISH)
			{
				// Oprava špatně nalakovaného dřeva
        		(new woodRepair)->Activate();
        	}
			else
			{
				// Hotové dřevo do sestavení
        		woodReadyCnt++;
				assemblyController->Activate();
        	}
			break;
		}
		else
		{
			Into(woodToBePainted);
			Passivate();
		}
	}
}

