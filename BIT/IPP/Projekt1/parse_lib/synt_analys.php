<?php
/* synt_analys.php
 * Modul pro skript parse.php. Přepisuje a kontroluje IPPcode20 do XML.
 * Autor: Michal Šedý <xsedym02>
 * Verze php: 7.4
 * Poslední úprava: 3.3.2020 
 */

include 'scanner.php';
include_once 'error.php';
include_once 'check_lexem.php';

# Globální proměnné pro sbírání statistik.
# Jsou používány pouze uvnitř modulu.
# Ostatním modulům jsou předávány jako parametry,
# nebo návratové hodnoty.
$comments = 0;
$loc = 0;
$labels = array();
$jumps = 0;

/* Vyhledává hlavičku .IPPcode20, a počítá případné komentáře,
 * výskyt čehokoliv jiného vede na chybu.
 *
 * Return - Funkce vrací počet komentářů před hlavičkou
 *        - Pokud hlavičku nenajde vypíše chybu a ukončí skript.
 *
 * Side effect - počítá komentáře --> inkrementuje glob. prom. $comments
 */
function find_head()
{
	global $comments;

    $lexem_arr = [];

    # Vyhledává hlavičku, dokud ji nenajde, nebo nedetekuje chybu.
    while(true)
	{
		# Získání proudu lexému
		$lexem_arr = get_lexems();
        # Kontrola správnosti hlavičky
        if($lexem_arr === false)
        {
            fwrite(STDERR, "Chybná hlavička IPPcode20.\n");
            exit(ERR_HEAD);
        }
        # Pokud je proud prázdný --> EOF
		if(empty($lexem_arr))
		{
			fwrite(STDERR, "Hlavička .IPPcode20 nenalezana.\n");
			fwrite(STDERR, "Vstup je prázdný, nebo obsahuje pouze komentáře.\n");
			exit(ERR_HEAD);
		}

		# Kontrola lexému na hlavičku a komentář.
		if(is_comment_ippcode20($lexem_arr[0]))
		{
			$comments++;
		}
		elseif(is_head_ippcode20($lexem_arr[0]))
		{
			if(count($lexem_arr) > 1)
			{
				# I za hlavíčkou se může nacházet komentář, ale už nic jiného.
				if(is_comment_ippcode20($lexem_arr[1]))
				{
					$comments++;
				}
				else
				{
					fwrite(STDERR, "Na stejném řádku s hlavičkou se může vyskytovat pouze komentář.\n");
					exit(ERR_OTHER);
				}
			}
			break;
		}
    	else
    	{
    		fwrite(STDERR, "Před dosažením hlavičky byl nalezen nepovolený lexém.\n");
    		exit(ERR_HEAD);
    	}
    }
}

/* Zkontroluje operandy(argumenty) příslušného operačního kódu.
 * v případě chyby ukončí skript s příslušným návratovým kódem.
 * 
 * $array - pole obsahující instrukci
 * $arg_1 - typ prvního operand (volitelné)
 * $arg_2 - typ druhého operandu(volitelné)
 * $arg_3 - typ třetího operandu (volitelné)
 *
 * Return - Funkce nevrací hodnotu.
 *
 * Side effect - počítá komentáře a névěští
 *             - $comments, $labels
 */
function check_arguments($array, $arg_1 = NULL, $arg_2 = NULL, $arg_3 = NULL)
{
	global $comments;
	global $labels;

	# Získéní standartní délky kontrolované instrukce
	$func_argv = func_get_args();
	$func_argc = count($func_argv);

	# Kontrola délky, délka může být větší o 1, pouze v případě existence
	# komentáře nainstrukcí, jinak chyba.
	if(count($array) != $func_argc && count($array) != ($func_argc + 1))
	{
		fwrite(STDERR, "Špatný počet operandů.\n");
		exit(ERR_OTHER);
	}

	# Detekce komentáře za instrukcí.
	if(count($array) == ($func_argc + 1))
	{
		if(!is_comment_ippcode20($array[$func_argc]))
		{
			fwrite(STDERR, "Špatný operand.\n");
			exit(ERR_OTHER);
		}

		$comments++;
	}


	# Kontrola shody operandů v $array, a počadovaných v $arg_1 atd.
	# Pokud není shody potvrzena, je vyvolána chyba a ukončen skript.
	for($i = 1; $i < $func_argc; $i++)
	{
		switch($func_argv[$i])
		{
			case "var":
				if(!is_var_ippcode20($array[$i]))
				{
					fwrite(STDERR, "Špatný <var> operand č. {$i} operace {$array[$i]}.\n");
					exit(ERR_OTHER);
				}
				break;

			case "symb":
				if(!is_symb_ippcode20($array[$i]))
				{
					fwrite(STDERR, "Špatný <symb> operand č. {$i} operace {$array[$i]}.\n");
					exit(ERR_OTHER);
				}
				break;

			case "label":
				if(!is_label_ippcode20($array[$i]))
				{
					fwrite(STDERR, "Špatný <label> operand č. {$i} operace {$array[$i]}.\n");
					exit(ERR_OTHER);
				}
                # Počítání unikátních návěští
                if(!in_array($array[$i], $labels))
                {
                    $labels[] = $array[$i];
                }
				break;

			case "type":
				if(!is_type_ippcode20($array[$i]))
				{
					fwrite(STDERR, "Špatný <type> operand č. {$i} operace {$array[$i]}.\n");
					exit(ERR_OTHER);
				}
				break;
		}
	}
}

/* Funkce převání již ověřené instrukce do XML reprezentace.
 *
 * &$xml - reference na výstupní xml
 * $array - pole obsahující instrukci pro převod
 * $array - pole obsahující instrukci
 * $arg_1 - typ prvního operand (volitelné)
 * $arg_2 - typ druhého operandu(volitelné)
 * $arg_3 - typ třetího operandu (volitelné)
 *
 * Return - Funkce nic nevrací
 *        - Ve funkci již nemůže vzniknout chyba, funkce se vždy úspěšně provede.
 */
function ippcode20_to_xml(&$xml, $array, $arg_1 = NULL, $arg_2 = NULL, $arg3 = NULL)
{
	# Počítadlo instrukcí
	static $order = 1;

	# Získání délky instrukce
	$func_argv = func_get_args();
	$arg_count = count($func_argv) - 1;

	# Započetí novéhoh xml elementu
	$xml->startElement("instruction");
	$xml->writeAttribute("order", $order++);
	$xml->writeAttribute("opcode", strtoupper($array[0]));

	# Pro každý operand se vyplní jeho povinné XML elementy a atributy.
	# Na základě $arg_1 atd. jsou určeny jejich typy.
	for($i = 1; $i < $arg_count; $i++)
	{
		switch($func_argv[$i+1])
		{
			case "var":
				$xml->startElement("arg".$i);
				$xml->writeAttribute("type", "var");
				$xml->text($array[$i]);
				$xml->endElement();
				break;

			case "symb":
				# Určenít, zda se jedné o proměnnou, nebo literál.
				if(is_var_ippcode20($array[$i]))
				{
					$xml->startElement("arg".$i);
					$xml->writeAttribute("type", "var");
					$xml->text($array[$i]);
					$xml->endElement();
				}
				else
				{
					$parts = explode("@", $array[$i], 2);
					$xml->startElement("arg".$i);
					$xml->writeAttribute("type", $parts[0]);
					$xml->text($parts[1]);
					$xml->endElement();
				}
				break;

			case "label":
				$xml->startElement("arg".$i);
				$xml->writeAttribute("type", "label");
				$xml->text($array[$i]);
				$xml->endElement();
				break;

			case "type":
				$xml->startElement("arg".$i);
				$xml->writeAttribute("type", "type");
				$xml->text($array[$i]);
				$xml->endElement();
				break;
		}
	}

	# Ukončení zapisovaného elementu(IPPcode20 instrukce).
	$xml->endElement();
}

/* Provádí syntakrickou analýzu lexémů získazých od scanneru,
 * volá funkcí pro přepis IPPcode20 do XML.
 *
 * Return - Funkce vrací pole statictických hodnot: řádky kódu (loc),
 *          počet komentářů (comments), počet návěšní (labels) a počet skoků (jumps).
 *        - V případě chyby ukončí skript s příslušným chybovám kódem.
 *
 * Side effect - funkce počítá statiskty a inkrementuje globální počítadla
 *             - $comments, $labels, $jumps, $loc
 */
function parse()
{

	# Počítadla statistických hodnot
	global $comments;
	global $labels;
	global $jumps;
	global $loc;

    # Vyhledá hlavičky .IPPcode20 a počítá komentáře před ní, popřípadě hned za ní.
    find_head();

    # Otevře xml pro výstup
    $xml = new XMLWriter();
	$xml->openUri('php://stdout');
	$xml->startDocument('1.0', 'UTF-8');
	$xml->setIndent(true);
	$xml->startElement("program");
	$xml->writeAttribute("language", "IPPcode20");

	# Zpracovává každý proud lexému získaný od skeneru,
	# dokud nejsou zpracované všechny ze vstupu, nebo není detekované chyba.
    while(true)
    {
    	# Získání proudu lexému
        # Kontrola na nepodporovaný opcode
    	$lexem_arr = get_lexems();
        if($lexem_arr === false)
        {
            fwrite(STDERR, "Chybný operační kód jazyka IPPcode20.\n");
            exit(ERR_OPCODE);
        }
        # Pokud je proud prázdný --> EOF
    	if(empty($lexem_arr))
    	{
    		break;
    	}

    	# Získávání statistik, kromě 'labels'
    	if(is_comment_ippcode20($lexem_arr[0]))
    	{
    		$comments++;
    		continue;
    	}
    	elseif(is_jump_ippcode20($lexem_arr[0]))
    	{
    		$jumps++;
    	}
    	$loc++;

    	# Volání Syntaktická kontrola pro jednotlivé skupiny lexémů 
    	# a volání funkce pro přepis do XML.
    	# Pokud je nalzen neznámý operační kód, je skript ukončen s příslušným kódem.
    	switch(strtoupper($lexem_arr[0]))
    	{
    		# OPCODE
    		case "CREATEFRAME":
    		case "PUSHFRAME":
    		case "POPFRAME":
    		case "RETURN":
    		case "BREAK":
    			check_arguments($lexem_arr);
    			ippcode20_to_xml($xml, $lexem_arr);
    			break;

    		# OPCODE <var>
    		case "DEFVAR":
    		case "POPS":
    			check_arguments($lexem_arr, "var");
    			ippcode20_to_xml($xml, $lexem_arr, "var");
    			break;

    		# OPCODE <label>
    		case "CALL":
    		case "LABEL":
    		case "JUMP":
    			check_arguments($lexem_arr, "label");
    			ippcode20_to_xml($xml, $lexem_arr, "label");
    			break;

    		# OPCODE <symb>
    		case "PUSHS":
    		case "WRITE":
    		case "EXIT":
    		case "DPRINT":
    			check_arguments($lexem_arr, "symb");
    			ippcode20_to_xml($xml, $lexem_arr, "symb");
    			break;

    		# OPCODE <var> <symb>
    		case "MOVE":
    		case "STRLEN":
    		case "INT2CHAR":
    		case "TYPE":
    		case "NOT":
    			check_arguments($lexem_arr, "var", "symb");
    			ippcode20_to_xml($xml, $lexem_arr, "var", "symb");
    			break;

    		# OPCODE <var> <type>
    		case "READ":
    			check_arguments($lexem_arr, "var", "type");
    			ippcode20_to_xml($xml, $lexem_arr, "var", "type");
    			break;

    		# OPCODE <var> <symb> <symb>
    		case "ADD":
    		case "SUB":
    		case "MUL":
    		case "IDIV":
    		case "LT":
    		case "GT":
    		case "EQ":
    		case "AND":
    		case "OR":
    		case "STRI2INT":
    		case "CONCAT":
    		case "GETCHAR":
    		case "SETCHAR":
    			check_arguments($lexem_arr, "var", "symb", "symb");
    			ippcode20_to_xml($xml, $lexem_arr, "var", "symb", "symb");
    			break;

    		# OPCODE <label> <symb> <symb>
    		case "JUMPIFEQ":
    		case "JUMPIFNEQ":
    			check_arguments($lexem_arr, "label", "symb", "symb");
    			ippcode20_to_xml($xml, $lexem_arr, "label", "symb", "symb");
    			break;

    		# Ostatní chybná OPCODE
    		default:
    			fwrite(STDERR, "Neznámý operační kód {$lexem_arr[0]}.\n");
    			exit(ERR_OPCODE);
    	}
    }

    # Uzavčení výstupního elementu 'program' a vypsání získaného XML.
    $xml->endElement();
	$xml->endDocument();
	$xml->flush();

	# Návrat statistik.
    return array($loc, $comments, count($labels), $jumps);
}
?>