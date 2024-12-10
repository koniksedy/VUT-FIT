/*
 * materialGenerator.hpp
 * Hlavičkový soubor s definicemi tríd generujícich surový materiál.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "materialGenerator.hpp"

double goodsUnloadingStart;

// Počet vstupního materiálu
extern int platesLBeforeCutting;
extern int platesSMBeforeCutting;
extern int rodsBeforeCutting;

int woodIncomming = 0;
int platesLIncomming = 0;
int platesSMIncomming = 0;
int rodsIncomming = 0;

/*
 * Generátor surového dřeva.
 */
void WoodGenerator::Behavior()
{
    for(int i = 0; i < RAW_WOOD_CNT; i ++)
    {
        (new WoodFromTruck)->Activate();
    }
    Activate(Time + DAY);
    woodIncomming += RAW_WOOD_CNT;
    goodsUnloadingStart = Time;
}

/*
 * Generátor svazků surových kovových prutů
 */
void SteelRodGenerator::Behavior()
{
    if((RAW_ROD_CNT * (int)(Time/DAY)) - rodsBeforeCutting < RAW_ROD_CNT)
    {
        for(int i = 0; i < (RAW_ROD_CNT/RODS_IN_BUNCH); i ++)
        {
            (new SteelRodFromTruck)->Activate();
        }
    rodsIncomming += RAW_ROD_CNT;
    }
    Activate(Time + DAY);
}

/*
 * Generátor velkého surového plechu.
 */
void SteelPlateGenerator::Behavior()
{
    if((RAW_PLATE_L_CNT * (int)(Time/DAY)) - platesLBeforeCutting < RAW_PLATE_L_CNT)
    {
        for(int i = 0; i < RAW_PLATE_L_CNT; i ++)
        {
            (new SteelPlateFromTruck)->Activate();
        }
    platesLIncomming += RAW_PLATE_L_CNT;
    }
    Activate(Time + DAY);
}


/*
 * Generátor malých surových kovových plátů.
 */
void SteelThingGenerator::Behavior()
{
    if((RAW_PLATE_SM_CNT * (int)(Time/DAY)) - platesSMBeforeCutting < RAW_PLATE_SM_CNT)
    {
        for(int i = 0; i < RAW_PLATE_SM_CNT; i ++)
        {
            (new SteelThingFromTruck)->Activate();
        }
    platesSMIncomming += RAW_PLATE_SM_CNT;
    }
    Activate(Time + DAY);
}
