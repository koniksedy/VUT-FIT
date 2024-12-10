/*
 * getGoodsFromTruck.cpp
 * Hlavičkový soubor s implementací tříd procesů obsluhujících
 * vyskládavání surového materiálu z korby dodavatele.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "getGoodsFromTruck.hpp"

// TODO zmazat
// int woodStock = 0;
// int steelThingStock = 0;
// int steelPlateStock = 0;
// int steelRodStock = 0;

double goodsUnloadingFinish = 0;

// Fronty surových materiálů čekajících k vyložení
Queue woodOnTruck("Nevyložené dřevo");
Queue steelThingOnTruck("Nevyložené tlusté pláty");
Queue steelPlateOnTruck("Nevyložené plech");
Queue steelRodOnTruck("Nevyložené tyčky");

// Pracovnící - Dřevaři, Zámečníci
extern Store woodWorkers;
extern Store steelWorkers;

// Linty (stroje) - jednotlivých řezaček kového materiálů
extern Facility plateLCutter;
extern Facility plateSMCutter;
extern Facility rodCutter;

// Fronty připraveného materiálu u stroje čekajícího na pracovníka
extern Queue plateLCutterWaitingForWorker;
extern Queue plateSMCutterWaitingForWorker;
extern Queue rodCutterWaitingForWorker;


/*
 * Nošení surového dřeva do provozu.
 * Nosit dřevo mohou jak Dřevaři, tak Zámečníci.
 */ 
void WoodFromTruck::Behavior()
{
    while(1)
    {
        if(!woodWorkers.Full())
        {
            // Nosí Dřevař
            Enter(woodWorkers, 1);
            Wait(MIN);

            Leave(woodWorkers, 1);
            (new woodPrepare) -> Activate();           
            GetNextWorkAllWorker();
            break;
        }
        else if(!steelWorkers.Full())
        {
            // Nosí Zámačník
            Enter(steelWorkers, 1);
            Wait(MIN);

            Leave(steelWorkers, 1);
            (new woodPrepare) -> Activate();
            GetNextWorkSteelWorker();
            goodsUnloadingFinish = (goodsUnloadingFinish > Time) ? goodsUnloadingFinish : Time;
            break;
        }
        else
        {
            // Dřevo nemá kdo odnést, čeká na korbě.
            Into(woodOnTruck);
            Passivate();
        }
    }
}


/*
 * Nošení surových svazků prutů do provozu.
 * Nosí pouze Zámečníci. Dřevaři mají moc práce.
 * Pruty se nosí ve svazcích po N.
 */ 
void SteelRodFromTruck::Behavior()
{
    while(1)
    {
        if(!steelWorkers.Full())
        {
            // Zámečníci nosí pruty
            Enter(steelWorkers, 1);
            Wait(MIN);

            // Pruty se nosí ve svazcích po N
            for (int i = 0; i < RODS_IN_BUNCH; i++)
            {
                // Zaregistrování budoucí práce s vyloženým materiálem na daném stroji.
                // Nastavení parametrů stroje, chybovost, zmetkovitost, doba obsluhy ...
                (new Machine(CUTTER_ROD_EDUR, CUTTER_ROD_HDUR, CUTTER_ROD_EE, CUTTER_ROD_HE,
                             CUTTER_ROD_DUR, CUTTER_ROD_RUBBISH, CUTTER_ROD_PLANE, ROD_CUTTER,
                             &rodCutter, &rodCutterWaitingForWorker))->Activate();
            }
            Leave(steelWorkers,1);
            GetNextWorkSteelWorker();
            goodsUnloadingFinish = (goodsUnloadingFinish > Time) ? goodsUnloadingFinish : Time;
            break;
        }
        else
        {
            Into(steelRodOnTruck);
            Passivate();
        }
    }
}

/*
 * Nošení surových velkých plechů do provozu.
 * Pláty nosí pouze Zámečníci.
 */
void SteelPlateFromTruck::Behavior()
{
    while(1)
    {
        if(!steelWorkers.Full())
        {
            // Zámečník nese plech
            Enter(steelWorkers, 1);
            Wait(MIN);

            // Zaregistrování budoucí práce s výrobkem na daném stroji.
            // Nastavení parametrů stroje, chybovost, zmetkovitost, doba obsluhy ...
            (new Machine(CUTTER_PL_EDUR, CUTTER_PL_HDUR, CUTTER_PL_EE, CUTTER_PL_HE,
                         CUTTER_PL_DUR, CUTTER_PL_RUBBISH, CUTTER_PL_PLANE, PLATE_L_CUTTER,
                         &plateLCutter, &plateLCutterWaitingForWorker))->Activate();

            Leave(steelWorkers, 1);
            GetNextWorkSteelWorker();
            goodsUnloadingFinish = (goodsUnloadingFinish > Time) ? goodsUnloadingFinish : Time;
            break;
        }
        else
        {
            Into(steelPlateOnTruck);
            Passivate();
        }
    }
}

/*
 * Nošení surových malých kovových plátů do porovozu.
 * Pláty nosí pouze Zámečníci.
 */
void SteelThingFromTruck::Behavior()
{
    while(1)
    {
        if(!steelWorkers.Full())
        {
            // Zámečník nese plát
            Enter(steelWorkers, 1);
            Wait(MIN);

            // Zaregistrování budoucí práce s výrobkem na daném stroji.
            // Nastavení parametrů stroje, chybovost, zmetkovitost, doba obsluhy ...
            (new Machine(CUTTER_PSM_DUR, CUTTER_PSM_HDUR, CUTTER_PSM_EE, CUTTER_PSM_HE,
                         CUTTER_PSM_DUR, CUTTER_PSM_RUBBISH, CUTTER_PSM_PLANE, PLATE_SM_CUTTER,
                         &plateSMCutter, &plateSMCutterWaitingForWorker))->Activate();

            Leave(steelWorkers, 1);
            GetNextWorkSteelWorker();
            goodsUnloadingFinish = (goodsUnloadingFinish > Time) ? goodsUnloadingFinish : Time;
            break;
        }
        else
        {
            Into(steelThingOnTruck);
            Passivate();
        }
    }
}

