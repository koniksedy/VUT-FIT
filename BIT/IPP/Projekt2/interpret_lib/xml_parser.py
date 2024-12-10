"""xml_parser.py
Modul pro získání a zkontrolování IPPcode20 programu z XML
jeho syntaktickou a lexikální kontrolu a přepis do instrukční pásky.
Autor: Michal Šedy <xsedym02>
Spuštění: python3.8
Poslední změna: 18.3.2020
"""
import re
import sys
import xml.etree.ElementTree as ET
import interpret_lib.var_frame as fr
from interpret_lib.instr_tape import LabelError, InstrTape, Instruction
from interpret_lib.exit_code import *


def parse_instruction(instr, xml_elem, type_1=None, type_2=None, type_3=None):
    """ Funkce kontroluje počet, typ a syntaktickou správnost jednotlivých operandů instrukce.

    Po úspěšné kontrole vloží funkce instrukci na konec instrukční pásky.
    type_* € {'var', 'symb', 'type', 'label'}

    Params
    ------
    instr - instrukční páska
    xml_elem - XML element obsahující operand ke kontrole
    type_1 - požadovaný typ prvního operandu (volitelné)
    type_2 - požadovaný typ druhého operandu (volitelné)
    type_3 - požadovaný typ třetího oparendu (volitelné)

    Return
    ------
    Funkce vrací v případě úspěšné kontroly True, jinak False

    Exceptions
    ----------
    KeyError - Pokud se v XML nebude nacházet požadovaný typ, bude vyvolana
               vyjímka KeyError.
    LabelError - Při nalezení opakované definice návěští se stejným
                 jménem bude vyvolana vyjímka instr_tape.LabelError
    """

    # Kontrola každého elementu XML obsahujícího operandy
    for t, i in zip((type_1, type_2, type_3), range(3)):
        # Pokud je operand None, zapíše se do instrukce
        if t is None:
            instr.append(None)
            continue

        # Elementy musí představovat pouze arg1 atd...
        if xml_elem[i].tag != ('arg' + str(i + 1)):
            return False

        if t == 'var':
            if xml_elem[i].attrib['type'] != 'var':
                return False
            if re.match(r"^(G|L|T)F@(\w|\-|\$|&|%|\*|\!|\?)+$", xml_elem[i].text) is not None:
                instr.append(fr.Variable((xml_elem[i].text).replace("@", "#", 1)))
            else:
                return False

        elif t == 'symb':
            if xml_elem[i].attrib['type'] == 'var':
                if re.match(r"^(G|L|T)F@(\w|\-|\$|&|%|\*|\!|\?)+$", xml_elem[i].text) is not None:
                    instr.append(fr.Variable((xml_elem[i].text).replace("@", "#", 1)))
                else:
                    return False
            elif xml_elem[i].attrib['type'] == 'nil':
                if xml_elem[i].text == 'nil':
                    instr.append(None)
                else:
                    return False
            elif xml_elem[i].attrib['type'] == 'int':
                if re.match(r"^(\+|\-)?([1-9][0-9]*$|0$)", xml_elem[i].text) is not None:
                    instr.append(int(xml_elem[i].text))
                else:
                    return False
            elif xml_elem[i].attrib['type'] == 'float':
                try:
                    instr.append(float(float.fromhex(xml_elem[i].text)))
                except ValueError:
                    return False
            elif xml_elem[i].attrib['type'] == 'bool':
                if xml_elem[i].text == 'false' or xml_elem[i].text == 'true':
                    instr.append(False if (xml_elem[i].text == 'false') else True)
                else:
                    return False
            elif xml_elem[i].attrib['type'] == 'string':
                if xml_elem[i].text is not None:
                    if re.match(r"\\(\d{0,2}$|\d{0,2}[^0-9])|\s", xml_elem[i].text) is not None:
                        return False
                    else:
                        # Nahradí všechny ASCII escape sekvence za přískušné znaky
                        to_save = xml_elem[i].text
                        while True:
                            id = to_save.find('\\')
                            if id == -1:
                                break
                            to_save = str(chr(int(to_save[id+1:id+1+3]))).join((to_save[:id], to_save[id+1+3:]))
                        instr.append(str(to_save))
                else:
                    instr.append(str())
            else:
                return False

        elif t == 'type':
            if xml_elem[i].attrib['type'] == 'type':
                if(xml_elem[i].text == "int" or xml_elem[i].text == "bool" or
                   xml_elem[i].text == "string" or xml_elem[i].text == "float"):
                    instr.append(str(xml_elem[i].text))
                else:
                    return False

        elif t == 'label':
            if xml_elem[i].attrib['type'] == 'label':
                if re.match(r"^(\w|\-|\$|&|%|\*|\!|\?)*$", xml_elem[i].text) is not None:
                    instr.append(str(xml_elem[i].text))
                else:
                    return False
            else:
                return False

    return True


def ippcode20_xml_parser(xml_in):
    """ Funkce pro přepis programu jazyka IPPcode20 uloženého v XML do instrukční pásky.

    Při přepisu kontroluje lexikální a syntaktickou správnost.
    Také kontroluje redefinice jmen návěští.

    Params
    ------
    xml_in - xml soubor (vstup) pro parsování

    Return
    ------
    Funkce vrací v případě úspěšné kontroly naplněnou instrukční pásku.
    V případě chyby ukončí provádění skriptu, vypíše hlášku chyby a vrátí příslušný kód.
    """
    try:
        tree = ET.parse(xml_in)
        root = tree.getroot()
    except ET.ParseError:
        print("Chybný formát XML, XML není dobře naformátováno.", file=sys.stderr)
        sys.exit(ERR_XML_FORMAT)

    # Kontrola existence elementů a jejich atributů
    # <program language="IPPcode20" name="Stack test">
    try:
        if root.tag != 'program' or root.attrib['language'] != 'IPPcode20':
            print("Nebyl nalezen element program, nebo atribut IPPcode20.", file=sys.stderr)
            sys.exit(ERR_XML_DATA)
    except KeyError:
        print("Chybná hlavička XML.", file=sys.stderr)
        sys.exit(ERR_XML_DATA)

    # Inicializace instrukční pásky
    tape = InstrTape()
    instr_all = dict()  # Pomocná struktura pro řazení instrukcí vzestupně podle ORDER
    # Parsování každéhé instrukce ukryté v XML
    try:
        for elem in root:

            # Kontrola hlavičky každé instrukce
            if elem.tag != "instruction":
                print("Chybně zapsaný element instruction.", file=sys.stderr)
                sys.exit(ERR_XML_DATA)

            # Kontrola operačního kódu a jeho operandů
            # Vytváření instrukce
            opcode = elem.attrib["opcode"].upper()
            instruction = [opcode]

            pars_res = False
            # Kontrola jednotlivých syntaxí IPPcode20
            if(opcode == 'CREATEFRAME' or opcode == 'PUSHFRAME' or opcode == 'POPFRAME' or
               opcode == 'RETURN' or opcode == 'BREAK' or opcode == "ADDS" or opcode == "CLEARS" or
               opcode == "SUBS" or opcode == "MULS" or opcode == "IDIVS" or opcode == "LTS" or
               opcode == "GTS" or opcode == "EQS" or opcode == "ANDS" or opcode == "ORS" or
               opcode == "NOTS" or opcode == "INT2CHARS" or opcode == "STRI2INTS" or
               opcode == "DIVS" or opcode == "INT2FLOATS" or opcode == "FLOAT2INTS"):
                pars_res = parse_instruction(instruction, elem)

            elif(opcode == 'DEFVAR' or opcode == 'POPS'):
                pars_res = parse_instruction(instruction, elem, 'var')

            elif(opcode == 'CALL' or opcode == 'LABEL' or opcode == 'JUMP' or
                 opcode == "JUMPIFEQS" or opcode == "JUMPIFNEQS"):
                pars_res = parse_instruction(instruction, elem, 'label')

            elif(opcode == 'PUSHS' or opcode == 'WRITE' or opcode == 'EXIT' or
                 opcode == 'DPRINT'):
                pars_res = parse_instruction(instruction, elem, 'symb')

            elif(opcode == 'MOVE' or opcode == 'STRLEN' or opcode == 'INT2CHAR' or
                 opcode == 'TYPE' or opcode == 'NOT' or opcode == 'INT2FLOAT' or opcode == 'FLOAT2INT'):
                pars_res = parse_instruction(instruction, elem, 'var', 'symb')

            elif(opcode == 'READ'):
                pars_res = parse_instruction(instruction, elem, 'var', 'type')

            elif(opcode == 'ADD' or opcode == 'SUB' or opcode == 'MUL' or opcode == 'IDIV' or
                 opcode == 'LT' or opcode == 'GT' or opcode == 'EQ' or opcode == 'AND' or
                 opcode == 'OR' or opcode == 'STRI2INT' or opcode == 'CONCAT' or
                 opcode == 'GETCHAR' or opcode == 'SETCHAR' or opcode == 'DIV'):
                pars_res = parse_instruction(instruction, elem, 'var', 'symb', 'symb')

            elif(opcode == 'JUMPIFEQ' or opcode == 'JUMPIFNEQ'):
                pars_res = parse_instruction(instruction, elem, 'label', 'symb', 'symb')

            else:
                print("Neznámý opcode {0}.".format(opcode), file=sys.stderr)
                sys.exit(ERR_XML_DATA)

            # Kontrola úspěšnosti parsování operandů
            if pars_res is False:
                print("Chyba při parsování operandů instrukce {0} IPPcode20.".format(opcode))
                sys.exit(ERR_XML_DATA)

            # Kontrola order a uložení instrukce
            order = int(elem.attrib["order"])
            if instr_all.get(order) is not None or order < 1:
                print("Chybný order.", file=sys.stderr)
                sys.exit(ERR_XML_DATA)

            instr_all[order] = Instruction(*instruction)

    except KeyError:
        print("Neznámý atribut XML.", file=sys.stderr)
        sys.exit(ERR_XML_DATA)
    except LabelError as err:    # Redefinice jména návěští
        print(err, file=sys.stderr)
        sys.exit(ERR_LABEL)

    # Seřazení instrukcí a uložení do instrukční pásky
    for k in sorted(instr_all.keys()):
        tape.append(instr_all[k])

    return tape
