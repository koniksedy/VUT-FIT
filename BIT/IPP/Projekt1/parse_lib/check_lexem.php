<?php
/* check_lexem.php
 * Modul pro kontrolu lexému v IPPcode20
 * Autor: Michal Šedý <xsedym02>
 * Verze php: 7.4
 * Poslední úprava: 3.3.2020 
 */

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém <bool> v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém <bool> v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_bool_ippcode20($str) : bool 
{
	if(strcmp($str, "bool@true") == 0 || strcmp($str, "bool@false") == 0)
	{
		return true;
	}

	return false;

}

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém <nil> v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém <nil> v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_nil_ippcode20($str) : bool
{
	if(strcmp($str, "nil@nil") == 0)
	{
		return true;
	}

	return false;
}

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém <int> v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém <int> v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_int_ippcode20($str) : bool
{
	if(preg_match('/^int@(\+|\-)?\d+$/', $str))
	{
		return true;
	}

	return false;
}

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém <string> v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém <string> v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_string_ippcode20($str) : bool
{
	if(preg_match('/^string@.*$/u', $str))
	{
        if(preg_match('/\\\\(\d{0,2}$|\d{0,2}[^0-9])|\s/u', $str))
        {
            return false;
        }
		return true;
	}

	return false;
}

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém <label> v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém <label> v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_label_ippcode20($str) : bool
{
	if(preg_match('/^(\-|\$|&|%|\*|\!|\?|[a-zA-Z_])(\-|\$|&|%|\*|\!|\?|\w)*$/u', $str))
	{
		return true;
	}

	return false;
}

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém <var> v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém <var> v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_var_ippcode20($str) : bool
{
	if(preg_match('/^(G|L|T)F@(\-|\$|&|%|\*|\!|\?|[a-zA-Z_])(\-|\$|&|%|\*|\!|\?|\w)*$/u', $str))
	{
		return true;
	}

	return false;
}

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém <symb> v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém <symb> v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_symb_ippcode20($str) : bool
{
	if(is_string_ippcode20($str) || is_int_ippcode20($str) ||
	   is_bool_ippcode20($str) || is_nil_ippcode20($str) ||
	   is_var_ippcode20($str))
	{
		return true;
	}

	return false;
}

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém <type> v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém <type> v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_type_ippcode20($str) : bool
{
	if(strcmp($str, "int") == 0 || strcmp($str, "bool") == 0 || strcmp($str, "string") == 0)
	{
		return true;
	}

	return false;
}

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém <comment> v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém <commet> v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_comment_ippcode20($str) : bool
{
	if(preg_match('/^\#.*/u', $str))
	{
		return true;
	}

	return false;
}

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém <head> v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém <head> v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_head_ippcode20($str) : bool
{
	if(preg_match('/^\.IPPcode20$/i', $str))
	{
		return true;
	}

	return false;
}

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém <jump> (libovolný skok) v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém <jump> v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_jump_ippcode20($str) : bool
{
	$str_up = strtoupper($str);
	if(strcmp($str_up, "CALL") == 0 || strcmp($str_up, "RETURN") == 0 ||
	   strcmp($str_up, "JUMP") == 0 || strcmp($str_up, "JUMPIFEQ") == 0 ||
	   strcmp($str_up, "JUMPIFNEQ") == 0)
	{
		return true;
	}

	return false;
}

/* Funkce vrací boolovskou hodnotu, zda je zadaný lexém lexémem v IPPcode20.
 *
 * $str - lexém ke kontrole
 *
 * Return - Pokud je lexém lexémem v jazyce IPPcode20, vrací funkce true,
 *          v opačném případě false.
 */
function is_lexem_ippcode20($str) : bool
{
	# Pole všech podporovaných operačních kódů v IPPcode20.
	$opcodes = array(
		"MOVE",
	    "CREATEFRAME",
	    "PUSHFRAME",
	    "POPFRAME",
	    "DEFVAR",
	    "CALL",
	    "RETURN",
	    "PUSHS",
	    "POPS",
	    "ADD",
	    "SUB",
	    "MUL",
	    "IDIV",
	    "LT",
	    "GT",
	    "EQ",
	    "AND",
	    "OR",
	    "NOT",
	    "INT2CHAR",
	    "STRI2INT",
	    "READ",
	    "WRITE",
	    "CONCAT",
	    "STRLEN",
	    "GETCHAR",
	    "SETCHAR",
	    "TYPE",
	    "LABEL",
	    "JUMP",
	    "JUMPIFEQ",
	    "JUMPIFNEQ",
	    "EXIT",
	    "DPRINT",
	    "BREAK",
	);

	# Kontrola na všechny ostatní lexémy IPPcode20.
	if(in_array(strtoupper($str), $opcodes))
	{
		return true;
	}
	elseif(is_head_ippcode20($str) || is_comment_ippcode20($str) || is_symb_ippcode20($str) ||
	        is_var_ippcode20($str) || is_label_ippcode20($str) || is_type_ippcode20($str))
	{
		return true;
	}
	elseif(is_string_ippcode20($str) || is_int_ippcode20($str) ||
	       is_nil_ippcode20($str) || is_bool_ippcode20($str))
	{
		return true;
	}

	return false;
}

?>