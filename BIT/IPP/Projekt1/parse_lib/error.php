<?php
/* error.php
 * Skript obsahující pomocné chybové proměnné pro parse.php
 * Autor: Michal Šedý
 * Verze php: 7.4
 * Poslední úprava: 9.2.2020 
 */

# Návratové kóy sdílené v modulech a skriptu parse.php
const OK = 0;           
const ERR_ARG = 10;       # Chybějísí parametr skriptu, nebo zakázaná kombinace
const ERR_IN = 11;        # Chyba při otvírání vstupních souborů
const ERR_OUT = 12;       # Chyba při otvírání výstupních souborů
const ERR_HEAD = 21;      # Chybná ,nebo chybějící hlavička, IPPcode20.
const ERR_OPCODE = 22;    # Neznámý, nebo chybný operační kód IPPcode20.
const ERR_OTHER = 23;     # Jiná lexikální chyba.
const ERR_INTER = 99;     # Vnitřní chyba skriptu.
?>