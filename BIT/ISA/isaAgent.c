/*
 * Definice MIB struktur a jejich registrace do existujícího stromu.
 * Vytvoření obslužných handlerů.
 * --------------------------
 * Soubor: isaAgent.c
 * Projekt: ISA - rozšíření SNMP agenta.
 * Autor: Michal Šedý <xsedym02>
 * Datum úpravy: 30.10.2020
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "isaAgent.h"
#include "getData.h"


// ######################################################################
// ################# DEFINICE INSTANCÍ A OID ############################
// ######################################################################

// Proměnná obsahující login. READ-ONLY
static char isaLogin[] = "xsedym02";
static oid  isaLogin_oid[] = {1, 3, 6, 1, 3, 22, 1, 0};

// Proměnná obsahující čas ve formátu RFC 3339. READ-ONLY
static char isaTime[] = "YYYY-MM-DDTHH:MM:SS-ZZ:ZZ";
static oid  isaTime_oid[] = {1, 3, 6, 1, 3, 22, 2, 0};

// Proměnná typu Int32. READ-WRITE
// Z důvodu implementace musí zde být typ long.
// Nic to nemění na faktu, že ukládané číslo je Int32.
static long isaInteger = 42;
static oid  isaInteger_oid[] = {1, 3, 6, 1, 3, 22, 3, 0};

// Následuje definice objektu obsahujícího různé informace o systému.

// Název systému. READ-ONLY
// Název je po čas běhu systému konstantní.
static char isaSystemName[256] = {0,};
static oid  isaSystemName_oid[] = {1, 3, 6, 1, 3, 22, 4, 1, 0};

// Vydání. READ-ONLY
// Po čas běhu zůstává konstantní.
static char isaSystemRelease[256] = {0,};	
static oid  isaSystemRelease_oid[] = {1, 3, 6, 1, 3, 22, 4, 2, 0};

// Verze systému. READ-ONLY
// Po čas běhu zůstává konstantní.
static char isaSystemVersion[256] = {0,};
static oid  isaSystemVersion_oid[] = {1, 3, 6, 1, 3, 22, 4, 3, 0};

// Architektura. READ-ONLY
// Po čas běhu zůstává konstantní.
static char isaMachineArch[256] = {0,};
static oid  isaMachineArch_oid[] = {1, 3, 6, 1, 3, 22, 4, 4, 0};

// Celková velikost paměti. READ-ONLY
static long isaMemTotal = 0;
static oid  isaMemTotal_oid[] = {1, 3, 6, 1, 3, 22, 4, 5, 0};

// Množství volné paměti. READ-ONLY
static long isaMemFree = 0;
static oid  isaMemFree_oid[] = {1, 3, 6, 1, 3, 22, 4, 6, 0};

// Celková velikost swap. READ-ONLY
static long isaSwapTotal = 0;
static oid  isaSwapTotal_oid[] = {1, 3, 6, 1, 3, 22, 4, 7, 0};

// Množství volného prostoru ve swap. READ-ONLY
static long isaSwapFree = 0;
static oid  isaSwapFree_oid[] = {1, 3, 6, 1, 3, 22, 4, 8, 0};

// Název procesoru. READ-ONLY
// Po čas běhu systému zůstává konstantní.
static char isaCpuModelName[256] = {0,};
static oid  isaCpuModelName_oid[] = {1, 3, 6, 1, 3, 22, 4, 9, 0};

// Počet jader procesoru. READ-ONLY
static long isaCpuCores = 0;
static oid  isaCpuCores_oid[] = {1, 3, 6, 1, 3, 22, 4, 10, 0};

// Adresování. READ-ONLY
// Po čas běhu systému zůstává konstantní.
static char isaAddressSize[256] = {0,};
static oid  isaAddressSize_oid[] = {1, 3, 6, 1, 3, 22, 4, 11, 0};


/*
 * Nahraje do registrovaných instancí základní data.
 */
void load_default_data(void)
{
	// Získání dat z <sys/utsname.h>
	static struct utsname uname_ptr;
	uname(&uname_ptr);

	strcpy(isaSystemName, uname_ptr.sysname);
	strcpy(isaSystemRelease, uname_ptr.release);
	strcpy(isaSystemVersion, uname_ptr.version);
	strcpy(isaMachineArch, uname_ptr.machine);

	// Získá data ze souboru /proc/meminfo
	get_data("/proc/meminfo", "MemTotal", &isaMemTotal, true);
	get_data("/proc/meminfo", "MemFree", &isaMemFree, true);
	get_data("/proc/meminfo", "SwapTotal", &isaSwapTotal, true);
	get_data("/proc/meminfo", "SwapFree", &isaSwapFree, true);

	// Získá data ze souboru /proc/cpuinfo
	get_data("/proc/cpuinfo", "model name", isaCpuModelName, false);
	get_data("/proc/cpuinfo", "cpu cores", &isaCpuCores, true);
	get_data("/proc/cpuinfo", "address sizes", isaAddressSize, false);
}


// ######################################################################
// ####################### REGISTRACE INSTANCÍ ##########################
// ######################################################################

/*
 * Registrace READ-ONLY instance isaLogin typu string.
 */
void register_instance_isaLogin(void)
{
    netsnmp_handler_registration *reginfo;
    static netsnmp_watcher_info watcher_info;

    int watcher_flags;
    reginfo = netsnmp_create_handler_registration("isaLogin", NULL, isaLogin_oid,
                                                  OID_LENGTH(isaLogin_oid), HANDLER_CAN_RONLY);
    // Vytvoří watcher.
    watcher_flags = WATCHER_MAX_SIZE;
    netsnmp_init_watcher_info6(&watcher_info, isaLogin, strlen(isaLogin), ASN_OCTET_STR,
                               watcher_flags,sizeof(isaLogin), NULL);
    // Registrace proměnné
    netsnmp_register_watched_instance(reginfo, &watcher_info);
}


/*
 * Registrace READ-ONLY instance isaTime typu string.
 */
void register_instance_isaTime(void)
{
    netsnmp_handler_registration *reginfo;
    static netsnmp_watcher_info watcher_info;

    int watcher_flags;
    reginfo = netsnmp_create_handler_registration("isaTime", &isaTime_handler, isaTime_oid,
                                                  OID_LENGTH(isaTime_oid), HANDLER_CAN_RONLY);
    // Vytvoří watcher.
    watcher_flags = WATCHER_MAX_SIZE;
    netsnmp_init_watcher_info6(&watcher_info, isaTime, strlen(isaTime), ASN_OCTET_STR,
                               watcher_flags,sizeof(isaTime), NULL);
    // Registrace proměnné
    netsnmp_register_watched_instance(reginfo, &watcher_info);
}


/*
 * Registrace READ-ONLY instance isaSystemName typu string.
 */
void register_instance_isaSystemName(void)
{
    netsnmp_handler_registration *reginfo;
    static netsnmp_watcher_info watcher_info;

    int watcher_flags;
    reginfo = netsnmp_create_handler_registration("isaSystemName", NULL, isaSystemName_oid,
                                                  OID_LENGTH(isaSystemName_oid), HANDLER_CAN_RONLY);
     // Vytvoří watcher.
    watcher_flags = WATCHER_MAX_SIZE;
    netsnmp_init_watcher_info6(&watcher_info, isaSystemName, strlen(isaSystemName), ASN_OCTET_STR,
                               watcher_flags,sizeof(isaSystemName), NULL);
    // Registrace proměnné
    netsnmp_register_watched_instance(reginfo, &watcher_info);
}


/*
 * Registrace READ-ONLY instance isaSystemRelease typu string.
 */
void register_instance_isaSystemRelease(void)
{
    netsnmp_handler_registration *reginfo;
    static netsnmp_watcher_info watcher_info;
    int watcher_flags;
    reginfo = netsnmp_create_handler_registration("isaSystemRelease", NULL, isaSystemRelease_oid,
                                                  OID_LENGTH(isaSystemRelease_oid), HANDLER_CAN_RONLY);
    // Vytvoří watcher.
    watcher_flags = WATCHER_MAX_SIZE;
    netsnmp_init_watcher_info6(&watcher_info, isaSystemRelease, strlen(isaSystemRelease), ASN_OCTET_STR,
                               watcher_flags,sizeof(isaSystemRelease), NULL);
    // Registrace proměnné
    netsnmp_register_watched_instance(reginfo, &watcher_info);
}


/*
 * Registrace READ-ONLY instance isaSystemVersion typu string.
 */
void register_instance_isaSystemVersion(void)
{
    netsnmp_handler_registration *reginfo;
    static netsnmp_watcher_info watcher_info;
    int watcher_flags;
    reginfo = netsnmp_create_handler_registration("isaSystemVersion", NULL, isaSystemVersion_oid,
                                                  OID_LENGTH(isaSystemVersion_oid), HANDLER_CAN_RONLY);
    // Vytvoří watcher.
    watcher_flags = WATCHER_MAX_SIZE;
    netsnmp_init_watcher_info6(&watcher_info, isaSystemVersion, strlen(isaSystemVersion), ASN_OCTET_STR,
                               watcher_flags,sizeof(isaSystemVersion), NULL);
    // Registrace proměnné
    netsnmp_register_watched_instance(reginfo, &watcher_info);
}


/*
 * Registrace READ-ONLY instance isaMachineArch typu string.
 */
void register_instance_isaMachineArch(void)
{
    netsnmp_handler_registration *reginfo;
    static netsnmp_watcher_info watcher_info;
    int watcher_flags;
    reginfo = netsnmp_create_handler_registration("isaMachineArch", NULL, isaMachineArch_oid,
                                                  OID_LENGTH(isaMachineArch_oid), HANDLER_CAN_RONLY);
    // Vytvoří watcher.
    watcher_flags = WATCHER_MAX_SIZE;
    netsnmp_init_watcher_info6(&watcher_info, isaMachineArch, strlen(isaMachineArch), ASN_OCTET_STR,
                               watcher_flags,sizeof(isaMachineArch), NULL);
    // Registrace proměnné
    netsnmp_register_watched_instance(reginfo, &watcher_info);
}


/*
 * Registrace READ-ONLY instance isaCpuModelName typu string.
 */
void register_instance_isaCpuModelName(void)
{
    netsnmp_handler_registration *reginfo;
    static netsnmp_watcher_info watcher_info;
    int watcher_flags;
    reginfo = netsnmp_create_handler_registration("isaCpuModelName", NULL, isaCpuModelName_oid,
                                                  OID_LENGTH(isaCpuModelName_oid), HANDLER_CAN_RONLY);
    // Vytvoří watcher.
    watcher_flags = WATCHER_MAX_SIZE;
    netsnmp_init_watcher_info6(&watcher_info, isaCpuModelName, strlen(isaCpuModelName), ASN_OCTET_STR,
                               watcher_flags,sizeof(isaCpuModelName), NULL);
    // Registrace proměnné
    netsnmp_register_watched_instance(reginfo, &watcher_info);
}


/*
 * Registrace READ-ONLY instance isaAddressSize typu string.
 */
void register_instance_isaAddressSize(void)
{
    netsnmp_handler_registration *reginfo;
    static netsnmp_watcher_info watcher_info;
    int watcher_flags;
    reginfo = netsnmp_create_handler_registration("isaAddressSize", NULL, isaAddressSize_oid,
                                                  OID_LENGTH(isaAddressSize_oid), HANDLER_CAN_RONLY);
    // Vytvoří vatcher.
    watcher_flags = WATCHER_MAX_SIZE;
    netsnmp_init_watcher_info6(&watcher_info, isaAddressSize, strlen(isaAddressSize), ASN_OCTET_STR,
                               watcher_flags,sizeof(isaAddressSize), NULL);
    // Registrace proměnné
    netsnmp_register_watched_instance(reginfo, &watcher_info);
}


/*
 * Registruje všechny instance které jsme definovali v ISA-MIB.
 */
void register_all_instances(void)
{
	register_instance_isaLogin();
	register_instance_isaTime();
	netsnmp_register_long_instance("isaInteger", isaInteger_oid, OID_LENGTH(isaInteger_oid), &isaInteger, NULL);
	register_instance_isaSystemName();
	register_instance_isaSystemRelease();
	register_instance_isaSystemVersion();
	register_instance_isaMachineArch();
	netsnmp_register_read_only_long_instance("isaMemTotal", isaMemTotal_oid, OID_LENGTH(isaMemTotal_oid), &isaMemTotal, &isaMemTotal_handler);
	netsnmp_register_read_only_long_instance("isaMemFree", isaMemFree_oid, OID_LENGTH(isaMemFree_oid), &isaMemFree, &isaMemFree_handler);
	netsnmp_register_read_only_long_instance("isaSwapTotal", isaSwapTotal_oid, OID_LENGTH(isaSwapTotal_oid), &isaSwapTotal, &isaSwapTotal_handler);
	netsnmp_register_read_only_long_instance("isaSwapFree", isaSwapFree_oid, OID_LENGTH(isaSwapFree_oid), &isaSwapFree, &isaSwapFree_handler);
	register_instance_isaCpuModelName();
	netsnmp_register_read_only_long_instance("isaCpuCores", isaCpuCores_oid, OID_LENGTH(isaCpuCores_oid), &isaCpuCores, NULL);
	register_instance_isaAddressSize();
}


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
                    netsnmp_request_info *requests)
{
	time_t now = time(NULL);
	struct tm *time = localtime(&now);

	if(time == NULL)
	{
		fprintf(stderr, "Nepodařil se načíst systémový čas.\n");
		return SNMP_ERR_GENERR;
	}

	sprintf(isaTime, "%d-%02d-%02dT%02d:%02d:%02d%+03d:%02d", 
		    1900 + time->tm_year, time->tm_mon + 1, time->tm_mday,
		    time->tm_hour, time->tm_min, time->tm_sec,
		    (int)time->tm_gmtoff / 3600, (ABS((int)time->tm_gmtoff) % 3600) / 60);

	strcpy((char*)requests->requestvb_start->val.string, isaTime);

    return SNMP_ERR_NOERROR;
}


/*
 * Handler proměnné isaMemTotal.
 * Získá ze systému informaci o maximální možné velikosti paměti.
 */
int isaMemTotal_handler(netsnmp_mib_handler *handler,
                        netsnmp_handler_registration *reginfo,
                        netsnmp_agent_request_info *reqinfo,
                        netsnmp_request_info *requests)
{
	get_data("/proc/meminfo", "MemTotal", &isaMemTotal, true);
	*(requests->requestvb_start->val.integer) = isaMemTotal;

	return SNMP_ERR_NOERROR;

}


/*
 * Handler proměnné isaMemFree.
 * Získá ze systému informaci o maximální možné volné paměti.
 */
int isaMemFree_handler(netsnmp_mib_handler *handler,
                       netsnmp_handler_registration *reginfo,
                       netsnmp_agent_request_info *reqinfo,
                       netsnmp_request_info *requests)
{
	get_data("/proc/meminfo", "MemFree", &isaMemFree, true);
	*(requests->requestvb_start->val.integer) = isaMemFree;

	return SNMP_ERR_NOERROR;
}


/*
 * Handler proměnné isaSwapTotal.
 * Získá ze systému informaci o maximální možné velikosti swapu.
 */
int isaSwapTotal_handler(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *reqinfo,
                         netsnmp_request_info *requests)
{
	get_data("/proc/meminfo", "SwapTotal", &isaSwapTotal, true);
	*(requests->requestvb_start->val.integer) = isaSwapTotal;

	return SNMP_ERR_NOERROR;
}


/*
 * Handler proměnné isaSwapFree.
 * Získá ze systému informaci o volné velikosti swapu.
 */
int isaSwapFree_handler(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *reqinfo,
                         netsnmp_request_info *requests)
{
	get_data("/proc/meminfo", "SwapFree", &isaSwapFree, true);
	*(requests->requestvb_start->val.integer) = isaSwapFree;

	return SNMP_ERR_NOERROR;
}


// ######################################################################
// ##################### INICIALZACE, DEINICIALIZACE ####################
// ######################################################################

/*
 * Inicializační funkce modulu.
 * Automaticky se volá při zavádějí agenta.
 * Je potřeba, aby se jmenovala "init_<jméno souboru>".
 */
void init_isaAgent(void)
{
	// Nahrání základních hodnot
	load_default_data();
	
	// Registrace všech instancí
	register_all_instances();
}


/*
 * Uvolnění všech zdrojů, při odpojení modulu.
 * Je potřeba, aby se jmenovala "deinit_<jméno souboru>".
 */
void deinit_isaAgent(void)
{
	unregister_mib(isaLogin_oid, OID_LENGTH(isaLogin_oid));
	unregister_mib(isaTime_oid, OID_LENGTH(isaTime_oid));
	unregister_mib(isaInteger_oid, OID_LENGTH(isaInteger_oid));
	unregister_mib(isaSystemName_oid, OID_LENGTH(isaSystemName_oid));
	unregister_mib(isaSystemRelease_oid, OID_LENGTH(isaSystemRelease_oid));
	unregister_mib(isaSystemVersion_oid, OID_LENGTH(isaSystemVersion_oid));
	unregister_mib(isaMachineArch_oid, OID_LENGTH(isaMachineArch_oid));
	unregister_mib(isaMemTotal_oid, OID_LENGTH(isaMemTotal_oid));
	unregister_mib(isaMemFree_oid, OID_LENGTH(isaMemFree_oid));
	unregister_mib(isaSwapTotal_oid, OID_LENGTH(isaSwapTotal_oid));
	unregister_mib(isaSwapFree_oid, OID_LENGTH(isaSwapFree_oid));
	unregister_mib(isaCpuModelName_oid, OID_LENGTH(isaCpuModelName_oid));
	unregister_mib(isaCpuCores_oid, OID_LENGTH(isaCpuCores_oid));
	unregister_mib(isaAddressSize_oid, OID_LENGTH(isaAddressSize_oid));
}