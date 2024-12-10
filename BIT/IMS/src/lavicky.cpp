/*
 * lavicky.cpp
 * Hlavní část programu do předměnu IMS.
 * Program byl vytvořen na základě zadání č.8 Simulace výrobního procesu.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#include "lavicky.hpp"
#include "time.h"

/*******************************************************
 *                     STATS                           *
 *******************************************************/

// Doba vykládání surového materiálu
extern Stat goodsUnloading;

// Stroje - řezačky
extern Facility plateLCutter;
extern Facility plateSMCutter;
extern Facility rodCutter;

// Stroje - lisy
extern Facility platePress;
extern Facility rodPress;

// Fronty materiálů na řezačkách čekajících na pracovníky
extern Queue plateLCutterWaitingForWorker;
extern Queue plateSMCutterWaitingForWorker;
extern Queue rodCutterWaitingForWorker;

// Fronty materálů na lisech čekajících na pracovníky
extern Queue plateLPressWaitingForWorker;
extern Queue plateSMPressWaitingForWorker;
extern Queue rodPressWaitingForWorker;

// Časy na strojích
extern Stat plateLCutterWorkingTime;
extern Stat plateSMCutterWorkingTime;
extern Stat roodCutterWorkingTime;
extern Stat platePressWorkingTime;
extern Stat roodPressWorkingTime;

// Fronta laviček
extern Queue benchesWaitingForDeparture;

// Fronty dřeva při zpracovávání
extern Queue woodToBePrepared;
extern Queue woodToBePainted;
extern Queue woodToBeRepaired;

// Fronta laviček ke složení čekajících na pracovníka
extern Queue benchToBeAssembled;

// Statistika denně vyrobených laviček
extern Stat bechDoneForDay;

// Počty vyrobených nezničených kovů
extern int platesLAfetrCutting;
extern int platesSMAfterCutting;
extern int rodsAfterCutting;
extern int platesLAfterPressing;
extern int platesSMAfterPressing;
extern int rodsAfterPressing;

// Počet vstupního materiálu
extern int platesLBeforeCutting;
extern int platesSMBeforeCutting;
extern int rodsBeforeCutting;
extern int platesLBeforePressing;
extern int platesSMBeforePressing;
extern int rodsBeforePressing;

// Statistiky surového materiálu
extern int woodIncomming;
extern int platesLIncomming;
extern int platesSMIncomming;
extern int rodsIncomming;

// Statictiky dřeva
extern int woodThrowOut;
extern int woodProcessed;

// Celkový počet laviček
extern int benchTotalDone;
extern int woodReadyCnt;
/*******************************************************
 *                 Program Init                        *
 *******************************************************/
int main()
{
    const int simulationLenDay = SIM_IN_DAYS;
    RandomSeed(time(NULL));
    Init(0, simulationLenDay * DAY-1);

    (new WoodGenerator)->Activate();
    (new SteelThingGenerator)->Activate();
    (new SteelPlateGenerator)->Activate();
    (new SteelRodGenerator)->Activate();
    (new CarMalfunction)->Activate(CAR_ERR_RATE);
    (new WorkingHours)->Activate();
    Run();

    // Čas vyskládávání materiálu
    goodsUnloading.Output();

    // Fronty materiálů na řezačkách čekajících na pracovníky
    plateLCutterWaitingForWorker.Output();
    plateLCutter.Output();
    plateSMCutterWaitingForWorker.Output();
    plateSMCutter.Output();
    rodCutterWaitingForWorker.Output();
    rodCutter.Output();

    // Fronty materálů na lisech čekajících na pracovníky
    plateLPressWaitingForWorker.Output();
    plateSMPressWaitingForWorker.Output();
    platePress.Output();
    rodPressWaitingForWorker.Output();
    rodPress.Output();

    // Časy na strojích
    plateLCutterWorkingTime.Output();
    plateSMCutterWorkingTime.Output();
    roodCutterWorkingTime.Output();
    platePressWorkingTime.Output();
    roodPressWorkingTime.Output();

    // Fronty dřeva při zpracovávání
    woodToBePrepared.Output();
    woodToBePainted.Output();
    woodToBeRepaired.Output();

    // Fronta laviček ke složení čekajících na pracovníka
    benchToBeAssembled.Output();

    // Fronta hotových laviček
    benchesWaitingForDeparture.Output();

    // statistiky denně vyrobených laviček
    bechDoneForDay.Output();

    printf("\nCELKEM VYROBENO LAVIČEK: %d\n\n", benchTotalDone);

    // Celkový počet vyrobených materiálů
    int incomming = platesLIncomming;
    int afterCutting = platesLAfetrCutting;
    int beforeCutting = platesLBeforeCutting;
    int afterPressing = platesLAfterPressing;
    int beforePress = platesLBeforePressing;
    int cutterPlane = CUTTER_PL_PLANE;
    int pressPlane = PRESS_PL_PLANE;

    printf("#####################################################\n");
    printf("ZMETKOVITOST PŘI VÝROBĚ Z VELKÉHO PLECHU.\n");
    printf("Celkem přivezeno: %d\n", incomming);
    printf("Zůstalo surového: %d\n", incomming - beforeCutting);
    printf("Celkem vyřezáno dílů: %d (%02.2f%%)\n", afterCutting, 100 * (float)afterCutting/(beforeCutting*cutterPlane));
    printf("Zůstalo nařezáno: %d\n", afterCutting - beforePress);
    printf("Celkem vylisováno dílů: %d (%02.2f%%)\n", afterPressing, 100 * (float)afterPressing/(beforePress*pressPlane));
    printf("Očekávaný počet produktů lisu: %d\n", beforePress*pressPlane);
    printf("Celková ztráta: %d \n\n", (beforeCutting*cutterPlane*pressPlane-(afterCutting - beforePress))-afterPressing);

    incomming = platesSMIncomming;
    afterCutting = platesSMAfterCutting;
    afterPressing = platesSMAfterPressing;
    beforeCutting = platesSMBeforeCutting;
    beforePress = platesSMBeforePressing;
    cutterPlane = CUTTER_PSM_PLANE;
    pressPlane = PRESS_PSM_PLANE;


    printf("#####################################################\n");
    printf("ZMETKOVITOST PŘI VÝROBĚ Z TLUSTÝCH PLÁTŮ.\n");
    printf("Celkem přivezeno: %d\n", incomming);
    printf("Zůstalo surového: %d\n", incomming - beforeCutting);
    printf("Celkem vyřezáno dílů: %d (%02.2f%%)\n", afterCutting, 100 * (float)afterCutting/(beforeCutting*cutterPlane));
    printf("Zůstalo nařezáno: %d\n", afterCutting - beforePress);
    printf("Celkem vylisováno dílů: %d (%02.2f%%)\n", afterPressing, 100 * (float)afterPressing/(beforePress*pressPlane));
    printf("Očekávaný počet produktů lisu: %d\n", beforePress*pressPlane);
    printf("Celková ztráta: %d \n\n", (beforeCutting*cutterPlane*pressPlane-(afterCutting - beforePress))-afterPressing);

    incomming = rodsIncomming;
    afterCutting = rodsAfterCutting;
    afterPressing = rodsAfterPressing;
    beforePress = rodsBeforePressing;
    beforeCutting = rodsBeforeCutting;
    cutterPlane = CUTTER_ROD_PLANE;
    pressPlane = PRESS_ROD_PLANE;

    printf("#####################################################\n");
    printf("ZMETKOVITOST PŘI VÝROBĚ Z PRUTŮ.\n");
    printf("Celkem přivezeno: %d\n", incomming);
    printf("Zůstalo surového: %d\n", incomming - beforeCutting);
    printf("Celkem vyřezáno dílů: %d (%02.2f%%)\n", afterCutting, 100 * (float)afterCutting/(beforeCutting*cutterPlane));
    printf("Zůstalo nařezáno: %d\n", afterCutting - beforePress);
    printf("Celkem vylisováno dílů: %d (%02.2f%%)\n", afterPressing, 100 * (float)afterPressing/(beforePress*pressPlane));
    printf("Očekávaný počet produktů lisu: %d\n", beforePress*pressPlane);
    printf("Celková ztráta: %d \n\n", (beforeCutting*cutterPlane*pressPlane-(afterCutting - beforePress))-afterPressing);
    
    printf("###################################################\n");
    printf("ZMETKOVITOST DŘEVA\n");
    printf("Celkem přivezeno: %d\n", woodIncomming);
    printf("Celkem zpotřebovaného dřeva: %d\n", woodProcessed);
    printf("Celková ztráta: %d\n", woodThrowOut);

    
    printf("Čeká na montáž: %d\n", woodReadyCnt);

    return 0;
}
/*******************************************************/