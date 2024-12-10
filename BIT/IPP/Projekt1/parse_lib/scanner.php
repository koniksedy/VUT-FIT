<?php
/* scanner.php
 * Skript pro kontrolu lexikální stránky IPPcode20.
 * Autor: Michal Šedý <xsedym02>
 * Verze php: 7.4
 * Poslední úprava: 9.2.2020 
 */

include_once 'error.php';
include_once 'check_lexem.php';

/* Funkce načte ze standartního vstupu jeden řádek(max 1024 znaků)
 * a rozloří jej do pole lexémů, zkontroluje jejich lexikální správnost
 * vůči IPPcode20, vrátí toto pole lexémů. V opačném případě ukončí skript
 * s příslušným návratovým kódem.
 *
 * Return - Funkce vrací pole lexémů odpovídajících IPPcode20,
 *        - nebo prázdné pole pokud načetla EOF, nebo se načítání nepovedlo.
 *        - Pokud se vyskytla leximální chyba u hlavičky nebo opcodu vrací false.
 */
function get_lexems()
{
    $lexem_out_arr = [];
    $lexem_arr = [];

    # Načílá řadáky, dokud jsou prázdné, nebo není detekován EOF.
    while(empty($lexem_arr))
    {
    	# Kontrola EOF
    	$line_in = fgets(STDIN);
    	if($line_in == false)
    	{
    		return $lexem_out_arr;
    	}

    	# Každý zank '#' rozčíří o mezeru na ' #', tím se obejde detekce komentářů
    	# těsně připojenýh ke kódu IPPcode20.
    	$line = str_replace("#", " #", $line_in);

    	# Rozdělení vstupního řádku na pole lexémů podle bílých znaků.
    	$lexem_arr = preg_split('/\s+/', $line, 0, PREG_SPLIT_NO_EMPTY);
    }

    # Každý lexém je zkontrolován, zda patří do IPPcode20.
    # Lexémy se kopírují do výstupního pole.
    $lexem_arr_count = count($lexem_arr);
    for($i = 0; $i < $lexem_arr_count; $i++)
    {
    	if(is_comment_ippcode20($lexem_arr[$i]))
    	{
    		# Pokud je detekován komentář přesune se na výstupní pole pouze první slovo,
    		# nebo znak komentáře, vše ostatní se zahodí (jedná se stejně pouze o koment.).
    		$lexem_out_arr[] = $lexem_arr[$i];
    		break;
    	}
    	elseif(is_head_ippcode20($lexem_arr[$i]))
    	{
    		$lexem_out_arr[] = $lexem_arr[$i];
    	}
    	elseif(is_lexem_ippcode20($lexem_arr[$i]))
    	{
    		$lexem_out_arr[] = $lexem_arr[$i];
    	}
    	else
    	{
    		# Lexikální chyba mohla nastat na úrovni OPCODu (1) nebo OPERANDů (>=2).
    		# Je potřeba tyto chyba odlišit.
    		if($i == 0)
    		{
    			return false;
    		}
    		else
    		{
    			fwrite(STDERR, "Cybně zapsaný operand {$lexem_arr[$i]} nacházející se za operačním kódem.\n");
    			exit(ERR_OTHER);
    		}
    	}
    }

    return $lexem_out_arr;
}
?>