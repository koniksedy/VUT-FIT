<?php
/* parse.php
 * Skript pro kontrolu programu v IPPcode20 a přepis do XML
 * Autor: Michal Šedý <xsedym02>
 * Verze php: 7.4
 * Poslední úprava: 1.3.2020 
 */

include_once './parse_lib/error.php';
include './parse_lib/synt_analys.php';


$stats = false;  # Povolení výpisu statistik
$stats_file_name = NULL;   # Soubor pro výpis statistik

# Prasování parametrů spuštění skriptu
# getopts pouze pro kontrolu použití help
$shortopts = "";
$longopts = array(
    "help",
    "stats:",
    "loc",
    "comments",
    "labels",
    "jumps",
);
$options = getopt($shortopts, $longopts);

foreach ($argv as $k) 
{
    # První argument přeskočíme
    if(strcmp($k, $argv[0]) == 0)
    {
        continue;
    }

    if(strcmp($k, "--help") == 0)
    {
        if(count($options) != 1)
        {
            fwrite(STDERR, "Nepodporovaná kombinace parametrů.\n");
            exit(ERR_ARG);
        }
    	# Výpis nápovědy
    	echo "phrse.php je skript, pro převod, syntaktickou a lexikální kontrolu programu v IPPcode20 do XML reprezentace.\n";
        echo "Použití: php7.4 parse.php [PRĚPÍNAČ]...\n\n";
        echo "      --help             Nápověda\n\n";
        echo "      --stats=file       Umožňuje výpis statistik specifikovaných statictickými\n";
        echo "                         přepínači. Výpis je prováděn pouze do souboru.\n\n";
        echo "      --loc              Statistický přepínači vypisující počet řádků kódu\n";
        echo "                         IPPcode20, bez komentářů a hlavičky.\n\n";
        echo "      --comments         Statistický přepínači vypisující počet komentářů.\n\n";
        echo "      --labels           Statistický přepínači vypisující počet různých návěští.\n\n";
        echo "      --jumps            Statistický přepínači vypisující počet podmíněných\n";
        echo "                         i nepodmíněných skoků, volání a návratů z funkcí.\n";
        exit(OK);
    }
    elseif(strcmp($k, "--loc") == 0 || strcmp($k, "--comments") == 0 ||
           strcmp($k, "--labels") == 0 || strcmp($k, "--jumps") == 0)
    {
        $stats = true;
    }
    elseif(preg_match("/--stats=.*/", $k))
    {
        # Nesmí být zadány dva soubory
        if(!is_null($stats_file_name))
        {
            fwrite(STDERR, "Pozor --stats=file můžete zadat pouze jednou.\n");
            exit(ERR_ARG);
        }

        $stats = true;
        $stats_file_name = substr($k, 8);

        # Musí být zadáno jméno souboru pro výpis statistik
        if(empty($stats_file_name))
        {
            fwrite(STDERR, "Nebyl zadán výstupní soubor pro statistiky.\n");
            exit(ERR_ARG);
        }
    }
    else
    {
        fwrite(STDERR, "Chybný argument \"" . $k . "\" spuštění.\n");
        exit(ERR_ARG);
    }
}

# Kontrola parametrů pro STATS, nemůžou být požadovány statistiky,
# když není zadán soubor, do kterého by se vypisovaly.
if($stats && $stats_file_name === NULL)
{
    fwrite(STDERR, "Chyba použití rozšiření STATS.\n");
    fwrite(STDERR, "Nebyl zadán parametr --stats=file\n");
    exit(ERR_ARG);
}

# Provedení lexikální a syntaktické kontroly a přepis IPPcode20 do XML.
# Získání statistických dat.
list($loc, $comments, $labels, $jumps) = parse();

# Výpis STATISTIK
if($stats)
{
    # Otevření souboru pro zápis statistik.
    if(($stats_file = fopen($stats_file_name, "w")) == false)
    {
        fwrite(STDERR, "Soubor". $stats_file_name ."pro výpis tatistik se nepovedlo otevřít/vytvořit.\n");
        exit(ERR_OUT);
    }

    # Procházení všech statistických parametrů a výpis hodnot do souboru.
    foreach ($argv as $key)
    {
        if(strcmp($key, "--loc") == 0)
        {
            fwrite($stats_file, $loc . "\n");
        }
        elseif(strcmp($key, "--comments") == 0)
        {
            fwrite($stats_file, $comments . "\n");
        }
        elseif(strcmp($key, "--labels") == 0)
        {
            fwrite($stats_file, $labels . "\n");
        }
        elseif(strcmp($key, "--jumps") == 0)
        {
            fwrite($stats_file, $jumps . "\n");
        }
    }
    
    fclose($stats_file);
}
?>