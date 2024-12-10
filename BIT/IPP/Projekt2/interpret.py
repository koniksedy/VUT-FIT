"""interpret.py
Skript interpretující imperativní jazyk IPPcode20 uložený v XML formátu.
Autor: Michal Šedy <xsedym02>
Spuštění: python3.8
Poslední změna: 21.3.2020
"""
import sys
import getopt
from interpret_lib.exit_code import *
from interpret_lib.xml_parser import ippcode20_xml_parser
from interpret_lib.program import Program
from interpret_lib.program import StrError
from interpret_lib.instr_tape import LabelError
from interpret_lib.var_frame import ValueMissError
from interpret_lib.var_frame import FrameError
from interpret_lib.var_frame import RedefError


def count_item(arr, key):
    """Funkce počítá počet výskytů hledaného výrazu v poli.

    Params:
    -------
    arr - prohledávané pole
    key - hledaný klíč

    Reutrn:
    -------
    Počet výskytů key v arr.
    """
    c = 0
    for item in arr:
        if item[0] == key:
            c += 1
    return c


def is_in(arr, key):
    """Funkce zkontroluje výskyt hledaného výrazu v poli.

    Params:
    -------
    arr - prohledávané pole
    key - hledaný klíč

    Reutrn:
    -------
    Funkce vazí boolovskou hodnotu určující existenci key v arr.
    """
    for item in arr:
        if item[0] == key:
            return True
    return False


def get_item(arr, key):
    """Funkce vrátí hodnotu prvního výskytu key v arr.

    Params:
    -------
    arr - prohledávané pole
    key - hledaný klíč

    Reutrn:
    -------
    Hodnota key v arr, pokud není nalezena, vrací None.
    """
    for item in arr:
        if item[0] == key:
            return item[1]
    return None


# Získání parametrů spuštění a jejich kontrola.
try:
    opts, other = getopt.getopt(sys.argv[1:], '', ['source=', 'input=', 'stats=', 'insts', 'vars', 'help'])
    if len(other) != 0:
        print("Chybný parametr spuštění.", file=sys.stderr)
        exit(ERR_ARG)
except getopt.GetoptError:
    print("Chybný parametr spuštění.", file=sys.stderr)
    exit(ERR_ARG)

source = sys.stdin
input_code = sys.stdin

if count_item(opts, "--source") > 1 or count_item(opts, "--input") > 1 or count_item(opts, "--stats") > 1:
    print("Přípiš mnoho stejných parametrů.", file=sys.stderr)
    exit(ERR_ARG)

# --help
if is_in(opts, "--help"):
    if count_item(opts, "--help") != len(opts):
        print("Nepovolená kombinace s help.", file=sys.stderr)
        exit(ERR_ARG)
    print("Spuštění interpretu: ./interpret [--input=file | --source=file] ...\n\n"
          "--input=file      Soubor pro simulaci načítání z klávesnice.\n"
          "                  Na každém řádku musí být pouze jeden vstup.\n"
          "                  Při spuštení musí být upřesněn alespoň --input, nebo --source.\n\n"
          "--source=file     Soubor s XML reprezentací programu v IPPcode20.\n"
          "                  Při spuštení musí být upřesněn alespoň --source, nebo --input.\n\n"
          "--stats=file      Spoubor pro výpis agregovaných statistik specifikovaných\n"
          "                  statistickými přepínači (--insts, --vars).\n\n"
          "--insts           Statistický přepínač vypisující počet vykonaných instrukcí.\n"
          "                  Při použití musí být zadán --stats=file.\n\n"
          "--vars            Statistický přepínač vypisující maximální počet\n"
          "                  inicializovaných proměnných během interpretace IPPcode20,\n"
          "                  přítomných ve všech rámcích.\n"
          "                  Při použití musí být zadán --stats=file.", file=sys.stderr)
    exit(0)


# Kontrola požadavku na sběr statistik
stats = True if is_in(opts, "--stats") else False
if stats is False and (is_in(opts, "--insts") or is_in(opts, "--vars")):
    print("Pro sbírání statistik nebyl zadám soubor --stats=file.", file=sys.stderr)
    exit(ERR_ARG)

# Nastavení --source a --input
source = get_item(opts, "--source")
input_code = get_item(opts, "--input")

if source is None and input_code is None:
    print("Nebyl uveden ani source ani input.", file=sys.stderr)
    exit(ERR_ARG)

try:
    if source is None:
        source = sys.stdin
    else:
        source = open(source, 'r')

    if input_code is None:
        input_code = sys.stdin
    else:
        input_code = open(input_code, 'r')
except Exception:
    print("Soubor nenalezen.", file=sys.stderr)
    exit(ERR_IN)

# Hlavní spuštění interpretace a zachytávání vyjímek
ret_code = 0
try:
    prog = Program(ippcode20_xml_parser(source))    # Parsovíní XML
    sys.stdin = input_code
    instr_cnt, max_init_cnt = prog.run_program(stats)    # Interpretace
except ZeroDivisionError:    # Dělení nulou.
    print("Dělení nulou.", file=sys.stderr)
    ret_code = ERR_RANGE
except ValueError as err:    # Špatná hodnota operandu.
    print(err, file=sys.stderr)
    ret_code = ERR_RANGE
except ValueMissError as err:    # Chybějící hodnota v proměnné, nebo na zásobníku.
    print(err, file=sys.stderr)
    ret_code = ERR_VAL_MISS
except TypeError as err:    # Špatný typ operandů.
    print(err, file=sys.stderr)
    ret_code = ERR_TYPE
except StrError as err:    # Chyba při řetězcových operacích.
    print(err, file=sys.stderr)
    ret_code = ERR_STR
except (RedefError, LabelError) as err:   # Redefinice proměnné/návěští, nebo nedefinovan návěští.
    print(err, file=sys.stderr)
    ret_code = ERR_DEFIN
except FrameError as err:    # Přístup k nedefinovanému rámci.
    print(err, file=sys.stderr)
    ret_code = ERR_FRAME
except KeyError:    # Neexistující proměnné, rámec existuje.
    print("Přístup k nedefinované proměnné, rámec existuje.", file=sys.stderr)
    ret_code = ERR_UNDEF

if source is not sys.stdin:
    source.close()
if input_code is not sys.stdin:
    input_code.close()

# Výpis statistik, pokud vše proběhlo bez problému
if ret_code == 0 and stats:
    try:
        out_stats = open(get_item(opts, "--stats"), 'w')
    except Exception:
        print("Nepovařilo se otevřít soubor pro zásis statistik.", file=sys.stderr)
        exit(ERR_OUT)

    for op in sys.argv[1:]:
        if op == "--insts":
            print(instr_cnt, file=out_stats)
        elif op == "--vars":
            print(max_init_cnt, file=out_stats)

    out_stats.close()

exit(ret_code)
