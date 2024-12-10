/*
 * jobSelector.cpp
 * Implementace modulu přidělující práci zaměstnancům a kontrolu pracovní doby.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "jobSelector.hpp"

// Zaměstnanci
Store woodWorkers("Dřevaři", WOOD_WORKERS);
Store steelWorkers("Zámečníci", STEEL_WORKERS);

// Práce - vyskládávaní surového materiálu.
extern Queue woodOnTruck;
extern Queue steelThingOnTruck;
extern Queue steelPlateOnTruck;
extern Queue steelRodOnTruck;

// Práce - zpracovávání dřeva
extern Queue woodToBeRepaired;
extern Queue woodToBePainted;
extern Queue woodToBePrepared;

// Práce na strojích
extern Queue plateLCutterWaitingForWorker;
extern Queue plateSMCutterWaitingForWorker;
extern Queue rodCutterWaitingForWorker;
extern Queue plateLPressWaitingForWorker;
extern Queue plateSMPressWaitingForWorker;
extern Queue rodPressWaitingForWorker;

// Práce - svařování
extern Queue materialWaitingForWelding;

// Práce - sestavování lavičky
extern Queue benchToBeAssembled;

// Příznak pracovní doby
bool isWorkTime = false;

// Statistické údaje
extern double goodsUnloadingStart;
extern double goodsUnloadingFinish;
Stat goodsUnloading("Doba vykládání surového materiálu");
extern int benchDone;
Stat bechDoneForDay("Lavičky vytvořené za jeden den");

// Celkový počet laviček
int benchTotalDone = 0;

/*
 * Funkce pro zadání další práce Dřevařovi
 * Přideluje práci v tomto pořadí:
 *  1 - skládání surového dřeva
 *  2 - natírání dřeva
 *  3 - opracování dřeva
 *  4 - sestavování lavičky
 *  5 - hoblování špatného nátěru (oprava) 
 */
void GetNextWorkWoodWorker()
{
    // Pokud skončila pracovní doba, nic se nepřiděluje
    if(!isWorkTime) {
        return;
    }

    
    if(!benchToBeAssembled.Empty()) {
        benchToBeAssembled.GetFirst()->Activate();
    }
    else if(!woodToBePainted.Empty()) {
        woodToBePainted.GetFirst()->Activate();
    }
    else if(!woodToBePrepared.Empty()) {
        woodToBePrepared.GetFirst()->Activate();
    }
    else if(!woodToBeRepaired.Empty()) {
        woodToBeRepaired.GetFirst()->Activate();
    }
    else if(!woodOnTruck.Empty()) {
        woodOnTruck.GetFirst()->Activate();
    }
}


/*
 * Funkce zadá Zámečníkům další práci.
 * Vrací booleovskou hodnotu, zda byla práce, kterou by mohl přidělit.
 */
bool GetNextWorkSteelWorker()
{
    // Pokud je konec pracovní doby, nic neděláme.
    if(!isWorkTime) {
        return false;
    }


    if(!benchToBeAssembled.Empty()) {
        benchToBeAssembled.GetFirst()->Activate();
    }  
    else if(!materialWaitingForWelding.Empty()) {
        materialWaitingForWelding.GetFirst()->Activate();
    }   
    else if(!plateLCutterWaitingForWorker.Empty()) {
        plateLCutterWaitingForWorker.GetFirst()->Activate();
    }
    else if(!plateSMCutterWaitingForWorker.Empty()) {
        plateSMCutterWaitingForWorker.GetFirst()->Activate();
    }
    else if(!rodCutterWaitingForWorker.Empty()) {
        rodCutterWaitingForWorker.GetFirst()->Activate();
    }
    else if(!plateLPressWaitingForWorker.Empty()) {
        plateLPressWaitingForWorker.GetFirst()->Activate();
    }
    else if(!plateSMPressWaitingForWorker.Empty()) {
        plateSMPressWaitingForWorker.GetFirst()->Activate();
    }
    else if(!rodPressWaitingForWorker.Empty()) {
        rodPressWaitingForWorker.GetFirst()->Activate();
    }
    else if(!woodOnTruck.Empty()) {
        woodOnTruck.GetFirst()->Activate();
    }
    else if(!steelPlateOnTruck.Empty()) {
        steelPlateOnTruck.GetFirst()->Activate();
    }
    else if(!steelRodOnTruck.Empty()) {
        steelRodOnTruck.GetFirst()->Activate();
    }
    else if(!steelThingOnTruck.Empty()) {
        steelThingOnTruck.GetFirst()->Activate();
    }
    else {
        return false;
    }
    

    return true;
}

/*
 * Funkce přidělí práci buď Děvařovi, nebo Zámečníkovi
 */
void GetNextWorkAllWorker()
{
    if(!woodOnTruck.Empty()) {
        GetNextWorkWoodWorker();
    }
    else {
        GetNextWorkSteelWorker();
    }
    
    // Návrh na optimalizaci
    /*
    if(!GetNextWorkSteelWorker())
    {
        GetNextWorkWoodWorker();
    }
    */
}


/*
 * Metoda implementující pracovní hodiny
 */
void WorkingHours::Behavior()
{
    if(!isWorkTime)
    {
        isWorkTime = true;
        // Pošle do práce N Dřevařů a M Zámečníků
        for(int i = 0; i < STEEL_WORKERS; i++)
        {
            GetNextWorkSteelWorker();
        }
        for(int i = 0; i < WOOD_WORKERS; i++)
        {
            GetNextWorkWoodWorker();
        }
        // Nastavení trvání prcovní doby
        Activate(Time + 8 * HOUR);
    }
    else
    {
        isWorkTime = false;
        goodsUnloading(goodsUnloadingFinish - goodsUnloadingStart);
        bechDoneForDay(benchDone);
        benchTotalDone += benchDone;
        benchDone = 0;
        Activate(Time + 16 * HOUR);
    }
}