<?php
/* test.php
 * Skript pro automatickou kontrolu funkčnosti parse.php a interpret.py.
 * Autor: Michal Šedý <xsedym02>
 * Verze php: 7.4
 * Poslední úprava: 20.3.2020 
 */

# Návratové kódy
const OK = 0;
const ERR_ARG = 10;     # Chybějící parametry skriptu nebo zakázaný kompinace.
const ERR_IN = 11;      # Chyba při otevírání vstupních souborů.
const ERR_INTER = 99;   # Interní chyba.

/*
 * Funkce kontrolující validitu zadaných parametrů skriptu.
 *
 * Return - Funkce nic nevrací, ale při nalezení chybného parametru vypíše
 *          chybu a ukončí skript s příslušným návratovým kódem.
 */
function check_arg_validity($array_check, $array_valid)
{
	foreach($array_check as $arg) 
	{
		if(!array_key_exists(str_replace("--", "", $arg), $array_valid))
		{
			if(preg_match('/^\-\-(help|recursive|parse\-only|int\-only).+/u', $arg) ||
			   !array_key_exists(preg_replace('/^--([^=]*).*/u', '${1}', $arg), $array_valid))
			{
				fwrite(STDERR, "Nepodporovaný přepínač {$arg}.\n");
				exit(ERR_ARG);
			}
		}
	}
}

/* Funkce se pokusí najít zadaný soubor, pokud soubor neexistuje, vytvoří jej.
 *
 * Return - Funkce nic nevrací, ale v případě neuspěchného vytvoření chybějícího
 *          souboru ukončí skript s příslušným návratovým kódem.
 */
function is_file_or_make($name, $str=NULL)
{
	# Kontrola, zda soubor již existuje.
	if(!is_file($name))
	{
		# Vytvoření souboru a kontrola její uspěšnosti.
		$fh = fopen($name, "w");
		if(!$fh)
		{
			fwrite(STDERR, "Nepodařilo se vytvořit chybějící soubor {$name}\n");
			exit(ERR_IN);
		}
		if($str != NULL)
		{
			fwrite($fh, $str);
		}
		fclose($fh);
	}
}

/* Funkce pro vytvoření řádku ve výsledné HTML5 tabulce obsahující informace
 * o úspěchnosti testu spuštěného nad daným souborem.
 * 
 * $name - jméno souboru (testu), MUSÍ EXISTOVAT
 * $exit_code - návratový kód testu
 * $exit_code_expected - očekávaný návratový kód
 * $diff_result - výsledek operace diff, nebo JExamXML, nad výstupními daty.
 *              - true - bez rozdílu
 *              - false - rozdíl výstupů
 *              - NULL - není třeba testovat na rozdíl výstupů
 *
 * Return - Funkce nic nevrací.
 */
function make_table_row($name, $exit_code, $exit_code_expected, $diff_result)
{	
	# Výpočet barev výstupů v tabulce. (rozdíl | neúspěch)->red, (nebylo třeba dál testovat| stejné) ->black
	if($diff_result === NULL)
	{
		$cell_colour = ($exit_code == $exit_code_expected) ? ("green") : ("red");
		$diff_text = "NAN";
	}
	else
	{
		$cell_colour = ($exit_code == $exit_code_expected && $diff_result == true) ? ("green") : ("red");
		$diff_text = ($diff_result == true) ? ("OK") : ('<span style="color:red;font-weight: bold">FAIL</span>');
	}
	$exit_text = ($exit_code == $exit_code_expected) ? ("OK") : ('<span style="color:red;font-weight: bold">FAIL</span>');

	# Vytvoření řádku tabulky.
	echo'<tr>'."\n";
	echo'    <td style="background-color:'.$cell_colour.';font-weight: bold">'.$name.'</td>'."\n";
	echo'    <td>'.$exit_code.' expected '.$exit_code_expected.'   ==> '.$exit_text.'</td>'."\n";
	echo'    <td>'.$diff_text.'</td>'."\n";
	echo'</tr>'."\n";
}

# Nastavení chybových výpisů a varování na stderr.
ini_set('display_errors', 'stderr');

# Prasování parametrů spuštění skriptu
$shortopts = "";
$longopts = array(
	"help",
	"directory:",
	"recursive",
	"parse-script:",
	"int-script:",
	"parse-only",
	"int-only",
	"jexamxml:",
	"testlist:",
	"match:",
);
$options = getopt($shortopts, $longopts);

# Kontrola validity parametrů skriptu
check_arg_validity(array_slice($argv, 1), $options);

# Kontrola nepovolených kombinací parametrů.
if((array_key_exists("int-only", $options) && array_key_exists("parse-only", $options)) ||
   (array_key_exists("int-script", $options) && array_key_exists("parse-only", $options)) ||
   (array_key_exists("parse-script", $options) && array_key_exists("int-only", $options)) ||
   (array_key_exists("testlist", $options) && array_key_exists("directory", $options)))
{
	fwrite(STDERR, "Nepodporovaná kombinace parametrů.\n");
	exit(ERR_ARG);
}

# --help 
if(array_key_exists("help", $options) && count($options) != 1)
{
	fwrite(STDERR, "Nepodporovaná kombinace parametrů.\n");
	exit(ERR_ARG);
}
if(array_key_exists("help", $options))
{
	echo "test.php je skript pro testování parse.php a interpret.py.\n";
	echo "Použití: php7.4 test.php [PARAMETRY]...\n\n";
	echo "   --help                - Nápověda.\n\n";
	echo "   --directory=path      - Testy bude hledat v zadaném adresáři.\n\n";
	echo "   --recursive           - Testy bude hledat nejen v zadaném adresáří,\n";
	echo "                           ale i rekurzivně ve všech jeho podadresářích.\n\n";
	echo "   --parse-script=file   - Soubor se kriptrem v PHP 7.4 pro analýzu zdrojového\n";
	echo "                           kódu v IPPcode20.\n\n";
	echo "   --int-script=file     - Soubor se kriptem v Python3.8 pro interpret XML \n";
	echo "                           reprezentace kdóu v IPPcode20.\n\n";
	echo "   --parse-only          - Bude testován pouze skript pro analýzu zdrojového \n";
	echo "                           kódu v IPPcode20. Tento parametr se nesmí kombinovat\n";
	echo "                           s parametry --int-only a --int-script.\n\n";
	echo "   --int-only            - Bude testován pouze skript pro interpret XML \n";
	echo "                           reprezentace kódu v IPPcoee20. Tento parametr se \n";
	echo "                           nesmí kombinovat s parametry --parse-only \n";
	echo "                           a --parse-script. Vstupní program je reprezentován\n";
	echo "                           pomocí XML s příponou src.\n\n";
	echo "   --jexamxml=file       - Soubor s JAR balíčkem s nástrojem A7Soft JExamXML.\n";
	echo "                           Je-li parametr vynechán, uvažuje se umístění\n";
	echo "                           /pub/courses/ipp/jexamxml.jar na serveru Merlin.\n\n";
	echo "   --testlist=file       - Slouží pro explicitní zadání seznamu adresárů,\n";
	echo "                           případně i souborů s testy formou zadání externího\n";
	echo "                           souboru file. Nekombinovat s parametrem --directory.\n\n";
	echo "   --match=regexp        - Výběr testů, jejichž jméno bez přípony (ne cesta)\n";
	echo "                           odpovídá zadanému regulárnímu výrazu dle PCRE.\n";
	exit(OK);
}

# --directory
$tests_dir = array(realpath("."));
if(array_key_exists("directory", $options))
{
    # Nesmí být zadáno více parametrů --directory
    if(is_array($options["directory"]))
    {
        fwrite(STDERR, "Více parametrů --directory.\n");
        exit(ERR_ARG);
    }

	$tests_dir = array(realpath($options["directory"]));

	# Kontrola, zda zadána složka existuje
	if(!is_dir($tests_dir[0]))
	{
		fwrite(STDERR, "Neexistující adresář s testy.\n");
		exit(ERR_IN);
	}
}
# --parse-script=file
if(!array_key_exists("int-only", $options))
{
	if(array_key_exists("parse-script", $options))
	{

        # Nesmí být zadáno více parametrů --parse-script
        if(is_array($options["parse-script"]))
        {
            fwrite(STDERR, "Více parametrů --parse-script.\n");
            exit(ERR_ARG);
        }
		$parse_path = realpath($options["parse-script"]);
	}
	else
	{
		$parse_path = realpath("./parse.php");
	}

	# Kontrola zadané cesty k parse.php
	if(!is_file($parse_path))
	{
		fwrite(STDERR, "Neexistující cesta k parse.php.\n");
		exit(ERR_IN);
	}
}

# --int-scipt=file
if(!array_key_exists("parse-only", $options))
{
	if(array_key_exists("int-script", $options))
	{
        # Nesmí být zadáno více parametrů --int-script
        if(is_array($options["int-script"]))
        {
            fwrite(STDERR, "Více parametrů --int-script.\n");
            exit(ERR_ARG);
        }
		$interpret_path = realpath($options["int-script"]);
	}
	else
	{
		$interpret_path = realpath("./interpret.py");
	}

	# Kontrola zadané cesty k interpret.py
	if(!is_file($interpret_path))
	{
		fwrite(STDERR, "Neexistující cesta k interpret.py\n");
		exit(ERR_IN);
	}
}

# --jexamxml=file
$jexamxml_path = "/pub/courses/ipp/jexamxml/jexamxml.jar";
if(array_key_exists("jexamxml", $options))
{
    # Nesmí být zadáno více parametrů --jexamxml
    if(is_array($options["jexamxml"]))
    {
        fwrite(STDERR, "Více parametrů --jexamxml.\n");
        exit(ERR_ARG);
    }
	$jexamxml_path = realpath($options["jexamxml"]);
    if(!is_file($jexamxml_path))
    {
        fwrite(STDERR, "Neexistující cesta k jexamxml.jar.\n");
        exit(ERR_IN);
    }
}


$tests_name = [];
# --testlist
# V souboru jsou jména testovacích souborů a složek s nimi
if(array_key_exists("testlist", $options))
{
    # Nesmí být zadáno spolu s --directory
    if(array_key_exists("directory", $options))
    {
        fwrite(STDERR, "Nepovolená kombinace --directory a --testlist.\n");
        exit(ERR_ARG);
    }
    # Nesmí být zadáno více parametrů --testlist
    if(is_array($options["testlist"]))
    {
        fwrite(STDERR, "Více parametrů --testlist.\n");
        exit(ERR_ARG);
    }

    $tests_dir = array();
    $testlist = fopen($options["testlist"], "r");
    if(!$testlist)
    {
        fwrite(STDERR, "Nepodařilo se otevřít soubor specifikovaný parametrem --test-file.\n");
        exit(ERR_IN);
    }

    while(($line = fgets($testlist)) != false)
    {
        $line = trim($line);
        if(is_file($line))
        {
            $tests_name[] = substr(realpath($line), 0, strlen(realpath($line))-4);
        }
        elseif(is_dir($line))
        {
            $tests_dir[] = realpath($line);
        }
        else
        {
            fwrite(STDERR, "Neplatný soubor nebo složka {$line} v seznamu určeném přepínačem --test-file.\n");
            exit(ERR_IN);
        }
    }

    fclose($testlist);
}

# Zprasuje soubory ve složkách určených polem $tests_dir.
# V případě nepoužití --testfile bude obsahuvat pouze jednu hodnotu.
foreach($tests_dir as $folder)
{
    # --recursive
    if(array_key_exists("recursive", $options))
    {
    	# Rekurzívní získávání souborů s příponou src
    	$Directory = new RecursiveDirectoryIterator($folder);
    	$Iterator = new RecursiveIteratorIterator($Directory);
    	$Regex = new RegexIterator($Iterator, '/^.+\.src$/i');

    	foreach($Regex as $src) 
    	{
    		$tests_name[] = substr($src, 0, strlen($src)-4);
    	}

    }
    else
    {	
    	# NErekurzivní získávání souborů s příponou src
    	foreach(glob($folder . "/*.src") as $src)
    	{
    		$tests_name[] = substr($src, 0, strlen($src) - 4);
    	}
    }
}

# -match=regexp
# Kontrola, zda jména testů vyhovují regulárníu výrazu.
# Pokud návez nevyhovuje, je test odstraněn.
if(array_key_exists("match", $options))
{
    # Nesmí být zadáno více parametrů --match
    if(is_array($options["match"]))
    {
        fwrite(STDERR, "Více parametrů --match.\n");
        exit(ERR_ARG);
    }
    # Kontrola správnosti regulárního výrazu.
    if(@preg_match('/'.$options["match"].'/', NULL) === false)
    {
        fwrite(STDERR, "Chybný regexp u volby --match.\n");
        exit(ERR_IN);
    }

    $tests_name_cnt = count($tests_name);
    for($i = 0; $i < $tests_name_cnt; $i++)
    {
        if(!preg_match('/'.$options["match"].'/', preg_replace('/^(.*\/)([^\/]*)$/u', '${2}', $tests_name[$i])))
        {
            unset($tests_name[$i]);
        }
    }
}


# Kontrola existence souborů in, out a rc, se souboru src.
# Pokud neexistují, budou vytvořeny.
foreach($tests_name as $name)
{
	is_file_or_make($name . ".in");
	is_file_or_make($name . ".out");
	is_file_or_make($name . ".rc", "0");
}

# Vytvoření jména dočasného soubor pro výstup kontrolovaného programu
# Pokud se soubor s tímto jménem již existuje, vytvoří se nové 100 pokusů.
$tmp_out = "";
$time = time();
for($i = 0; $i < 100; $i++)
{
	
	if(!file_exists("./".($time+$i*7)."test.tmp"))
	{
		$tmp_out = "./".($time+$i*7)."test.tmp";
		break;
	}
}
if(empty($tmp_out))
{
	fwrite(STDERR, "Nepodařilo se vytvořit dočasné soubory potřebné pro běh aplikace, zkuste to znovu.\n");
	exit(ERR_INTER);
}

# Generování hlavičky HTML5 výstupu testů
echo'<!DOCTYPE html>'."\n";
echo'<html>'."\n";
echo'<title>Testy</title>'."\n";
echo'<head>'."\n";
echo'<meta charset="UTF-8">'."\n";
echo'<style>'."\n";
echo'table, th, td {'."\n";
echo'  margin: 60px;'."\n";
echo'  border: 1px solid black;'."\n";
echo'  border-collapse: collapse;'."\n";
echo'}'."\n";
echo'th, td {'."\n";
echo'  padding: 5px;'."\n";
echo'}'."\n";
echo'table tr:nth-child(even) {'."\n";
echo'  background-color: #eee;'."\n";
echo'}'."\n";
echo'table tr:nth-child(odd) {'."\n";
echo' background-color: #fff;'."\n";
echo'}'."\n";
echo'</style>'."\n";
echo'</head>'."\n";
echo'<body>'."\n";

$passed = 0;
$failed = 0;
$actual_folder = "";  # Proměnná pro detekri změny složky s testy
foreach($tests_name as $name)
{
	# Kontrola, zda nejsou testy čteny z jiné složky
	if(strcmp(preg_replace('/^(.*)\/[^\/]*$/u', '${1}', $name), $actual_folder) != 0)
	{
		if(!empty($actual_folder))
		{
			echo'</table>'."\n";
		}

		# Nastavení nového jména složky s právě vykonávanými testy
		$actual_folder = preg_replace('/^(.*)\/[^\/]*$/u', '${1}', $name);
		
		# Vytvoření nové hlavičky tabulky.
		echo'<table style="width:80%"; align="center">'."\n";
		echo'  <tr>'."\n";
		echo'    <th colspan="4" >'.$actual_folder.'/</th>'."\n";
		echo'  </tr>'."\n";
		echo'  <tr>'."\n";
		echo'    <th>Test name</th>'."\n";
		echo'    <th>Exit code</th>'."\n";
		echo'    <th>Diff</th>'."\n";
		echo'  </tr>'."\n";
	}

	# Načtení referenčního návratového kódu
	$fh = fopen($name . ".rc", "r");
	if(!$fh)
	{
		fwrite(STDERR, "Nepodařilo se otevřít soubor pro kontrolu návratového kódu.\n");
		exit(ERR_IN);
	}
	$line = fgets($fh);
	if($line === false)
	{
		fwrite(STDERR, "Soubor {$name}.rc je prázdný.\n");
		exit(ERR_IN);
	}
	fclose($fh);

	# TESTOVÁNÍ
	if(array_key_exists("parse-only", $options))
	{
		exec('php7.4 '.$parse_path.' <'.$name.'.src >'.$tmp_out, $trash, $return_code);
	}
	elseif(array_key_exists("int-only", $options))
	{
        exec('python3.8 '.$interpret_path.' --source='.$name.'.src --input='.$name.'.in >'.$tmp_out, $trash, $return_code);
	}
	else    # Komplexní testování
	{
        exec('php7.4 '.$parse_path.' <'.$name.'.src >'.$tmp_out, $trash, $return_code);
        if($return_code == 0)
        {
            exec('php7.4 '.$parse_path.' <'.$name.'.src | '.
             'python3.8 '.$interpret_path.' --input='.$name.'.in >'.$tmp_out, $trash, $return_code);
        }
	}

	# Kontrola exit kódů, v případě 0 je provedem diff, nebo jexamxml
	if(trim($line) == $return_code && $return_code == 0)
	{
		if(array_key_exists("parse-only", $options))
		{
			exec('java -jar '. $jexamxml_path . ' ' . $name . '.out '.$tmp_out, $trash, $return_code);

		}
		else
		{
			exec('diff '.$name.'.out '.$tmp_out, $trash, $return_code);			
		}

		# Kontrola výsledku porovnání výstupů a vytvoření příslušného řádků
		# do výsledné tabulky testů
		if($return_code == 0)
		{
			$passed++;
			make_table_row(preg_replace('/^(.*\/)([^\/]*)$/u', '${2}', $name), 0, 0, true);
		}
		else
		{
			$failed++;
			make_table_row(preg_replace('/^(.*\/)([^\/]*)$/u', '${2}', $name), 0, 0, false);
		}
	}
	elseif(trim($line) == $return_code)   # Kódy byly různé od 0 a nebylo třeba porovnávat výstupy.
	{
		$passed++;
		make_table_row(preg_replace('/^(.*\/)([^\/]*)$/u', '${2}', $name), $return_code, trim($line), NULL);
	}
	else
	{	
		$failed++;
		make_table_row(preg_replace('/^(.*\/)([^\/]*)$/u', '${2}', $name), $return_code, trim($line), NULL);
	}
}

# Pokud nebyl nalezen žádný test, není třeba ukončovat tabulku.
if(!empty($actual_folder))
{
	echo'</table>'."\n";
}

# HTML5 souhrn testů a ukončení HTML5 dokumentu
echo'<p><span style="font-weight: bold">PASSED = <span style="color:green;">'.$passed.'</span></p>'."\n";
echo'<p><span style="font-weight: bold">FAILED = <span style="color:red;">'.$failed.'</span></p>'."\n";
echo'</body>'."\n";
echo'</html>'."\n";

# Pokud byl vytvořen dočasný soubor, odstraní se.
if(file_exists($tmp_out))
{
	unlink($tmp_out);
}
?>