/*
 * Hlavičkový soubor modulu isaAgent.c
 * --------------------------
 * Soubor: isaAgent.h
 * Projekt: ISA - rozšíření SNMP agenta.
 * Autor: Michal Šedý <xsedym02>
 * Datum úpravy: 30.10.2020
 */

#ifndef _ISA_AGENT_H_
#define _ISA_AGENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <sys/utsname.h>

#define ABS(x) ((x < 0) ? -x : x)


/*
 * Nahraje do registrovaných instancí základní data.
 */
void load_default_data(void);


// ######################################################################
// ####################### REGISTRACE INSTANCI ##########################
// ######################################################################

// Funkce pro registraci jednotlivých instanci typu STRING
void register_instance_isaLogin(void);
void register_instance_isaTime(void);
void register_instance_isaSystemName(void);
void register_instance_isaSystemRelease(void);
void register_instance_isaSystemVersion(void);
void register_instance_isaMachineArch(void);
void register_instance_isaCpuModelName(void);
void register_instance_isaAddressSize(void);

/*
 * Registruje všechny instance které jsme definovali v ISA-MIB.
 */
void register_all_instances(void);


// ######################################################################
// ########################## HANDLERY ##################################
// ######################################################################

/*
 * Handler proměnné isaTime.
 * Získává systémový čas ve formátu RFC 3339.
 * Pokud se čas nepodaří načíst vrací SNMP_ERR_GENERR.
 */
int isaTime_handler(netsnmp_mib_handler *handler,
                    netsnmp_handler_registration *reginfo,
                    netsnmp_agent_request_info *reqinfo,
                    netsnmp_request_info *requests);

/*
 * Handler proměnné isaMemTotal.
 * Získá ze systému informaci o maximální možné velikosti paměti.
 */
int isaMemTotal_handler(netsnmp_mib_handler *handler,
                        netsnmp_handler_registration *reginfo,
                        netsnmp_agent_request_info *reqinfo,
                        netsnmp_request_info *requests);

/*
 * Handler proměnné isaMemFree.
 * Získá ze systému informaci o maximální možné volné paměti.
 */
int isaMemFree_handler(netsnmp_mib_handler *handler,
                       netsnmp_handler_registration *reginfo,
                       netsnmp_agent_request_info *reqinfo,
                       netsnmp_request_info *requests);

/*
 * Handler proměnné isaSwapTotal.
 * Získá ze systému informaci o maximální možné velikosti swapu.
 */
int isaSwapTotal_handler(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *reqinfo,
                         netsnmp_request_info *requests);

/*
 * Handler proměnné isaSwapFree.
 * Získá ze systému informaci o volné velikosti swapu.
 */
int isaSwapFree_handler(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *reqinfo,
                         netsnmp_request_info *requests);


// ######################################################################
// ###################### INICIALIZACE, DEINICIALIZACE ##################
// ######################################################################

/*
 * Inicializační funkce modulu.
 * Automaticky se volá agentem. Je potřeba, aby se jmenovat "init_<jméno souboru>".
 */
void init_isaAgent(void);

/*
 * Uvolnění všech zdrojů.
 */
void deinit_isaAgent(void);


#endif  // _ISA_AGENT_H_