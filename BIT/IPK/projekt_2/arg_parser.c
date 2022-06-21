/**
 * Modu zpracovávající argumenty spuštění programu.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#include "arg_parser.h"


/**
 * Zpracuje vstupní argumenty programu.
 * ----------------------------------------
 * př: {-i <interface>} --pu(-u) <port-ranges> --pt(-t) <port-ranges> 
 *     { --only-open |--only-closed | --only-unclosed} [<domain-name> | <IP-address>]
 * ----------------------------------------
 * tcp    - ukazatel který bude odkazovat na případně vatvořenou frontu,
 *          pro naplnění porty pro TCP skenování
 * udp    - ukazatel který bude odkazovat na případně vatvořenou frontu,
 *          fronta, pro naplnění porty pro UDP skenování
 * ip_dst - ukazatel na textový řetězec cílové ip pro návrat získaného ip
 * inter  - ukazatel na textový řetězec jména interface pro návrat získaného jména
 * argc   - počet argumetů spuštění programu
 * argv   - argumenty spuštění programu
 * ---------------------------------------
 * V případě úspěchu nahraje funkce do parametrů předaných odkazem
 * příslušné získané hodnoty a vrátí mod výpislu dat ze skenování portů (ALL/OPEN/CLOSED/UNCLOSED).
 * Pokud během parsování narazí na chybu parametrů, ukončí program
 * s návratovým kódem 1.
 */
short int parse_arg(port_queue_t **tcp, port_queue_t **udp, char **ip_dst, char **inter, const int argc, char *argv[])
{
    // Standartně se vypisují veškeré výsledky.
    short int run_type = GET_ALL; 

    // Kontrola na počet parametrů.
    if(argc > 9 || argc < 2)
    {
        fprintf(stderr, "[ERROR]: Špatný počet argumentů spuštění.\n");
        PRINT_HELP;
        exit(ERR_ARG);
    }

    // Postupný průchod parametry
    int i;
    for(i = 1; i < argc-1; i++)
    {
        if(strcmp(argv[i], "--pt") == 0 || strcmp(argv[i], "-t") == 0)
        {   
            // Víse parametrů pt/t nesmí být zadáno
            if(*tcp == NULL)
            {   
                // Vytvoří frontu portů pro TCP skenování.
                *tcp = port_queue_ctor();
                if(*tcp == NULL)
                {
                    fprintf(stderr, "[ERROR]: Chyba alokace port_queue pro tcp.\n");
                    exit(ERR_INTER);
                }
                // Nahraje hodnoty portů do fronty pro TCP skenování.
                // Ihned za přpínašem se nachází parsované hodnoty.
                if(!load_range(*tcp, argv[++i]))
                {
                    fprintf(stderr, "[ERROR]: Chyba v rozmezí portů tcp skenování.\n");
                    PRINT_HELP;
                    exit(ERR_ARG);
                }
            }
            else
            {
                fprintf(stderr, "[ERROR]: Více přepínačů pro tcp skenování (-t, --pt).\n");
                PRINT_HELP;
                exit(ERR_ARG);
            }

        }
        else if(strcmp(argv[i], "--pu") == 0 || strcmp(argv[i], "-u") == 0)
        {
            // Více parametrů pu/u je zakázáno.
            if(*udp == NULL)
            {
                // Vytvoří frontu portů pro UDP skenování.
                *udp = port_queue_ctor();
                if(*udp == NULL)
                {
                    fprintf(stderr, "[ERROR]: Chyba alokace port_queue pro udp.\n");
                    exit(ERR_INTER);
                }
                // Nahraje hodnoty portů do fronty pro UDP skenování.
                // Ihned za přpínašem se nachází parsované hodnoty.
                if(!load_range(*udp, argv[++i]))
                {
                    fprintf(stderr, "[ERROR]: Chyba v rozmezí portů udp skenování.\n");
                    PRINT_HELP;
                    exit(ERR_ARG);
                }
            }
            else
            {
                fprintf(stderr, "[ERROR]: Více přepínačů pro udp skenování (-u, --pu).\n");
                PRINT_HELP;
                exit(ERR_ARG);
            }
        }
        else if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            // Výpis návovědy, ukončí provádění programu s návratovým kódem 0.
            PRINT_HELP;
            exit(ERR_OK);
        }
        else if(strcmp(argv[i], "-i") == 0)
        {
            // Parametr i (interface) může být zadám pouze jeden.
            // Ihned za přpínašem se nachází jméno interface.
            if(*inter == NULL)
            {
                *inter = argv[++i];
            }
            else
            {
                fprintf(stderr, "[ERROR]: Přípiš mnoho interface.\n");
                PRINT_HELP;
                exit(ERR_ARG);
            }
        }
        else if(strcmp(argv[i], "--only-open") == 0)
        {   
            // Přepínač upřesňující mod výpisu výsledků může být jen jeden.
            if(run_type != GET_ALL)
            {
                fprintf(stderr, "[ERROR]: Více parametrů --only.\n");
                PRINT_HELP;
                exit(ERR_ARG);
            }
            run_type = GET_OPEN;
        }
        else if(strcmp(argv[i], "--only-closed") == 0)
        {
            // Přepínač upřesňující mod výpisu výsledků může být jen jeden.
            if(run_type != GET_ALL)
            {
                fprintf(stderr, "[ERROR]: Více parametrů --only.\n");
                PRINT_HELP;
                exit(ERR_ARG);
            }
            run_type = GET_CLOSED;
        }
        else if(strcmp(argv[i], "--only-unclosed") == 0)
        {
            // Přepínač upřesňující mod výpisu výsledků může být jen jeden.
            if(run_type != GET_ALL)
            {
                fprintf(stderr, "[ERROR]: Více parametrů --only.\n");
                PRINT_HELP;
                exit(ERR_ARG);
            }
            run_type = GET_UNCLOSED;
        }
        else
        {
            fprintf(stderr, "[ERROR]: Neznámý parametr %s\n", argv[i]);
            PRINT_HELP;
            exit(ERR_ARG);
        }
    }

    // Pokud již nezůstal žádný parametr, jedná se o chybu.
    // Jako poslední měla zůstat ip adresa, nebo jméno cíle.
    if(i == argc)
    {
        fprintf(stderr, "[ERROR]: Chybějící IP, nebo špatné parametry spuštění.\n");
        PRINT_HELP;
        exit(ERR_ARG);
    }
    else
    {
        // Jako poslední a také správný parametr, může být help
        if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            PRINT_HELP;
            exit(ERR_OK);
        }
        else
        {
            // Předání ip/jméno cíle. Poslední argument.
            *ip_dst = argv[i];
        }
    }

    return run_type;
}

/**
 * Z textového řetězce ve formátu naplní frontu číselnými hodnotami,
 * a to i v zadaných rozsazích. např. 1,9-210,987.
 * Ke zpracování je použit automat.
 * ----------------------------------------
 * queue - fronta pro naplnění
 * str   - text obsahující číselné hodnoty ke zpracování
 * ----------------------------------------
 * V případě úspěšného zpracování bude fronta queue naplněna
 * získanýmí hodnotami z řetězce str a vrátí true. Pokud ale
 * vstupní řetězec str nebude ve zprávném formátu (např. A,4 nebo 1,,34)
 * vrátí funkce false a queue obsahovat hodnoty uložené před detekcí.
 */
bool load_range(port_queue_t *queue, const char str[])
{
    int state = 0;  // Počáteční stav autmatu
    int chr;        // Příjatý znak
    int string_len = strlen(str);

    // port_1 - nahrávaji se znaky čísla portu stojícího samostatně 22,
    //          nebo první číslo intervalu 22-100 (22)
    char port_1[6] = {0,};
    // index nové pozice k zaplnění v port_1
    short port_1_idx = 0;
    // port_2 - nahrávají se znaky čísla portu konce intervatu 22-100 (100)
    char port_2[6] = {0,};
    // index nové pozice k zaplnění v port_2
    short port_2_idx = 0;

    // Cyklus bude inkrementova i do rovnosti délky parsovaného řetězce.
    // I po překonání indexové hranice str se provede ještě poslední cyklus.
    for(int i = 0; i <= string_len; i++)
    {
        // Detekce konce parsovaného řetězce
        chr = (i < string_len) ? str[i] : EOF;
        
        // Samotný autmat
        switch(state)
        {
            case 0:     // DETEKCE PRVNÍ ČÍSLICE ČISLAa --> stav 1
                if(chr >= '0' && chr <= '9')
                {
                    // Číslo nespí být delší než 5 znaků (index 4)
                    if(port_1_idx > 4)
                    {
                        return false;
                    }
                    port_1[port_1_idx++] = chr;
                    state = 1;
                }
                else
                {
                    return false;
                }
                break;

            case 1:     // DETEKCE DRUHÉ ČÍSLICE ČÍSLAa --> stav 1
                        // DETEKCE INTERVALU --> stav 2
                        // DETEKCE KONCE ČÍSLAa --> zpracování a stav 0
                if(chr >= '0' && chr <= '9')
                {
                    // Číslo nespí být delší než 5 znaků (index 4)
                    if(port_1_idx > 4)
                    {
                        return false;
                    }
                    port_1[port_1_idx++] = chr;
                }
                else if(chr == '-')
                {
                    // Detekce intervalu, potřeba získat horní mez.
                    state = 2;
                }
                else if(chr == ',' || chr == EOF)
                {
                    // Pokec samostatného čísla.
                    // Kontrola na maximum a uložení do queue
                    if(atoi(port_1) > 65535)
                    {
                        return false;
                    }
                    put(queue, atoi(port_1));
                    // Reset počítadel
                    memset(port_1, 0, 5);
                    port_1_idx = 0;
                    state = 0;
                }
                else
                {
                    return false;
                }
                break;

            case 2:     // DETEKCE PRVNÍ ČÍSLICE ČISLAb --> stav 3
                if(chr >= '0' && chr <= '9')
                {
                    // Číslo nesmí být delší než 5 znaků (index 4)
                    if(port_2_idx > 4)
                    {
                        return false;
                    }
                    port_2[port_2_idx++] = chr;
                    state = 3;
                }
                else
                {
                    return false;
                }
                break;

            case 3:     // DETEKCE DRUHÉ ČÍSLICE ČÍSLAb --> stav 3
                        // DETEKCE KONCE INTERVALU --> zpracování a stav 0
                if(chr >= '0' && chr <= '9')
                {
                    if(port_2_idx > 4)
                    {
                        return false;
                    }
                    // Číslo nesmí být delší než 5 znaků (index 4)
                    port_2[port_2_idx++] = chr;
                }
                else if(chr == ',' || chr == EOF)
                {
                    // Detekce konce intervalu.
                    // Kontrola a uložení rozgerenovaného intervalu do queue.
                    if(atoi(port_1) > 65535 || atoi(port_2) > 65535)
                    {
                        return false;
                    }

                    // Kontrola hodnot hranic intervalu.
                    if(atoi(port_2) < atoi(port_1))
                    {
                        fprintf(stderr, "[INFO]: Vypadá to, že hranice %s a %s jsou prohozene. Interval přeskakuji.\n", port_1, port_2);
                    }

                    // Prohozené hranice intervalu jsou přeskočeny.
                    // Započítá se i horní mez intervalu
                    for(int j = atoi(port_1); j <= atoi(port_2); j++)
                    {
                        put(queue, (unsigned short)(j));
                    }

                    // Reset obou počítadel
                    memset(port_1, 0, 5);
                    port_1_idx = 0;
                    memset(port_2, 0, 5);
                    port_2_idx = 0;
                    state = 0;
                }
                else
                {
                    return false;
                }
                break;
        }
    }

    return true;
}