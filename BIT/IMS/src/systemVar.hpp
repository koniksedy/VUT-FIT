/*
 * systemVar.hpp
 * Soubor s definicemi pro jednotlivé proměnné simulovaného systému.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>
 */
#ifndef _SYSTEMVAR_HPP_
#define _SYSTEMVAR_HPP_

#include "timing.hpp"

// Čas simulace ve dnech
#define SIM_IN_DAYS 365

// Zaměstnanci
#define WOOD_WORKERS 4
#define STEEL_WORKERS 5

// Auto
#define CAR_FREE_SPACE 20
#define CAR_ERR_RATE (5*YEAR)

// Příchozí surový materiál
#define RAW_WOOD_CNT 20
#define RAW_PLATE_L_CNT 1
#define RAW_PLATE_SM_CNT 20
#define RAW_ROD_CNT 16
#define RODS_IN_BUNCH 4

// Opracovávání dřeva
#define WOOD_PROCESS_MIN (30*MIN)
#define WOOD_PROCESS_MAX (HOUR)
#define WOOD_RUBBISH 0.03

// Lakování dřeva
#define PAINT_MIN (30*MIN)
#define PAINT_MAX (HOUR)
#define PAINT_DRYING (DAY)
#define PAINT_RUBBISH 0.05

// Řezání velkého plechu
#define CUTTER_PL_PLANE 205
#define CUTTER_PL_DUR 10
#define CUTTER_PL_EE 0.04
#define CUTTER_PL_HE 0.01
#define CUTTER_PL_EDUR 30
#define CUTTER_PL_HDUR (5*MIN)
#define CUTTER_PL_RUBBISH 0.1

// Řezání malých plátů
#define CUTTER_PSM_PLANE 5
#define CUTTER_PSM_DUR 30
#define CUTTER_PSM_EE 0.01
#define CUTTER_PSM_HE 0.01
#define CUTTER_PSM_EDUR (MIN)
#define CUTTER_PSM_HDUR (10*MIN)
#define CUTTER_PSM_RUBBISH 0.1

// Řezání prutů
#define CUTTER_ROD_PLANE 4
#define CUTTER_ROD_DUR 15
#define CUTTER_ROD_EE 0.03
#define CUTTER_ROD_HE 0
#define CUTTER_ROD_EDUR (MIN) 
#define CUTTER_ROD_HDUR 0
#define CUTTER_ROD_RUBBISH 0.1

// Lisování plechu
#define PRESS_PL_PLANE 1
#define PRESS_PL_DUR 20
#define PRESS_PL_EE 0
#define PRESS_PL_HE 0.05
#define PRESS_PL_EDUR 0
#define PRESS_PL_HDUR (5*MIN)
#define PRESS_PL_RUBBISH 0.1

// Lisování malých plátů
#define PRESS_PSM_PLANE 1
#define PRESS_PSM_DUR 15
#define PRESS_PSM_EE 0
#define PRESS_PSM_HE 0.05
#define PRESS_PSM_EDUR 0
#define PRESS_PSM_HDUR (5*MIN)
#define PRESS_PSM_RUBBISH 0.15

// Lisování prutů
#define PRESS_ROD_PLANE 1
#define PRESS_ROD_DUR 20
#define PRESS_ROD_EE 0
#define PRESS_ROD_HE 0.04
#define PRESS_ROD_EDUR 0
#define PRESS_ROD_HDUR (5*MIN)
#define PRESS_ROD_RUBBISH 0.1

// Svařování
#define WELD_RUBBISH 0.03

// Sestavování lavičky různými zaměstnanci
#define WW_ASSEMBLING_MIN (1.5*HOUR)
#define WW_ASSEMBLING_MAX (2*HOUR)
#define WW_ASSEMBLING_RUBBISH 0.02
#define SW_ASSEMBLING_MIN (HOUR)
#define SW_ASSEMBLING_MAX (1.5*HOUR)
#define SW_ASSEMBLING_RUBBISH 0.018


#endif
