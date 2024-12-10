"""program.py
Modul vykonávající instrukce uložené v instrukční pásce.
Autor: Michal Šedy <xsedym02>
Spuštění: python3.8
Poslední změna: 18.3.2020
"""
import sys
import re
import interpret_lib.var_frame as fr


class StrError(Exception):
    """ Operace nad řetězcem mimo index, nebo neplatná hodnota znaku"""
    pass


class Stack(list):
    """ Třída implemntující zásobníl.

    Exceptions:
    -----------
    fr.ValueMissError
    """

    def push(self, data):
        """ Vloží hodnotu na vrchol zásobníku.

        Params:
        -------
        data - hodnota k vložení na zásobník
        """
        super().append(data)

    def pop(self):
        """ Vyjme hodnotu z vrcholu zásobníku.

        Exceptions:
        -----------
        fr.ValueMissError - Pop na prázdný zásobník
        """
        try:
            to_ret = super().pop()
        except IndexError:
            raise fr.ValueMissError("Provádějí popu na prázdný zásobník.")
        return to_ret


class Program:
    """ Třída implementující operace IPPcode20

    Exceptions:
    -----------
    ValueMissError
    KeyError
    FrameError
    RedefError
    TypeError
    LabelError
    ZeroDivisionError
    ValueError
    StrError
    """

    def __init__(self, tape):
        """ Inicializuje Program se zadanou instrukční páskou.

        Params:
        -------
        tape - páska k nahrání do programu
        """
        self.__tape = tape
        self.__frames = fr.FrameSpace()
        self.__call_st = Stack()
        self.__stack = Stack()

    def __repr__(self):
        return "Program({0} {1} {2} {3})".format(self.__tape, self.__frames,
                                                 self.__call_st, self.__stack)

    def __str__(self):
        return "{0} {1} {2} {3}".format(self.__tape, self.__frames,
                                        self.__call_st, self.__stack)

    def get_v(self, var):
        """ Vrátí hodnotu proměnné var z příslušného rámce.
        Pokud se jedná o konstantu a ne jméno proměnné, ihned ji vrátí.

        Params:
        -------
        var - jméno proměnné, nebo konstanta

        Return:
        -------
        Funkce vrací získanou hodnotu.

        Exceptions:
        -----------
        KeyError - neexistující proměnná (rámec existuje)
        fr.ValueMissError - neinicializovaná proměnná
        """
        #if "#" in str(var):
        if type(var) is fr.Variable:
            return self.__frames.getvar(var)
        else:
            return var

    def arit(self, var_1, var_2, fce):
        """ Provede aritmetickou (dle fce) operaci nad zadanými hodnotami (kontroluje typy).

        Params:
        --------
        var_1 - první operand
        var_2 - druhý operand
        fce - lambda funkce specifikující operaci

        Return:
        -------
        Funkce vrací výsledek aritmetické operace.

        Exceptions:
        -----------
        ZeroDivisionError - dělení nulou
        TypeError - špatný typ operandů (musí být int)
        """
        if (type(var_1) is not float and type(var_1) is not int) or type(var_1) is not type(var_2):
            raise TypeError("Špatný typ operandů aritmetické instrukce.")
        return fce(var_1, var_2)

    def rel(self, var_1, var_2, fce, allow_nil=False):
        """ Provede relační (dle fce) operaci nad zadanými hodnotami (kontroluje typy).

        Params:
        -------
        var_1 - první operand
        var_2 - druhý operand
        fce - lambda funkce specifikující operaci
        allow_nil - povolení operace EQ s hodotou nil

        Return:
        -------
        Funkce vrací výsledek relační operace

        Exceptions:
        -----------
        TypeError - Špatný typ operandů
        """

        if type(var_1) is int and type(var_2) is int:
            pass
        elif type(var_1) is bool and type(var_2) is bool:
            pass
        elif type(var_1) is str and type(var_2) is str:
            pass
        elif type(var_1) is float and type(var_2) is float:
            pass
        else:
            if allow_nil is True and (var_1 is None or var_2 is None):
                    return var_1 is var_2
            else:
                raise TypeError("Špatný typ operandů relační instrukce.")
        return fce(var_1, var_2)

    def logic(self, var_1, var_2=None, fce=None, is_not=False):
        """ Provede logickou operaci (dle fce) nad zadanými hodnotami (kontroluje typy).

        Params:
        var_1 - první operand
        var_2 - druhý operand
        fce - lambda funkce specifikujíící operaci
        is_not - umožňujísí negaci pouze nad prvním operandem

        Return:
        -------
        Funkce vrací výsledek logické operace.

        Exceptions:
        -----------
        TypeError - špatný typ operandů
        """
        if type(var_1) is not bool:
            raise TypeError("Špatný typ operandů logické instrukce.")

        if is_not is True:
            return not var_1

        if type(var_2) is not bool:
            raise TypeError("Špatný typ operandů logické instrukce.")
        return fce(var_1, var_2)

    def get_vars_jump(self, var_1, var_2):
        """ Získá hodnoty pro provedení podmíněného skoku (kontroluje typy).

        Params:
        -------
        var_1 - první operand
        var_2 - druhý operand

        Return:
        -------
        Funkce vrací dvojici získaných hodnot.

        Exceptions:
        -----------
        KeyError - neexistující proměnná (rámec existuje)
        fr.ValueMissError - neinicializovaná proměnná
        TypeError - Špatný typ operandů.
        """
        v_1 = self.get_v(var_1)
        v_2 = self.get_v(var_2)

        # todo nemělo by být type(neco) is None
        if type(v_1) is not type(v_2) and (v_1 is not None and v_2 is not None):
            raise TypeError("Špatný typ operandů instrukce JUMPIF(N)EQ.")

        return v_1, v_2

    def status(self):
        """ Vypíše aktuální stav rámců, zásobniku volání, datového zásobníku
        a pozici na pásce. Vhodné pro ladící výpisy."""
        print("cnt = {0}\n".format(self.__tape.actual() - 1),
              "frames = {0}\n".format(self.__frames),
              "data_s = {0}\n".format(self.__stack),
              "call_s = {0}".format(self.__call_st), sep="", file=sys.stderr)

    def run_program(self, stat):
        """ Na základě IPPcod20 porovná operační kód získá potřebné operandy,
        zkontroluje je a zahájí provádění instrukce.

        Exceptions:
        -----------
        KeyError - neexistující proměnná (rámec existuje)
        FrameError - rámec neexistuje
        RedefError - redefinice proměnné
        fr.ValueMissError - neinicializovaná proměnná, nebo pop neprázdný zásobník
        TypeError - Špatný typ operandů.
        LabelError - Nedefinované návěští
        ZeroDivisionError - dělení nulou
        ValueError - chyba rozmezí u operace EXIT
        StrError - chyba při řetězcových operacích
                 - (index mimo řetězec nebo neplatná hodnota znaku)
        """
        # Aktivace pásky pro čtení
        self.__tape.activate()
        instr_cnt = 0
        max_init = 0
        while True:
            # Načtení instrukce a kontrola, zda není páska na konci
            instr = self.__tape.read()

            #self.status()  # LADICI VYPIS

            if instr is None:
                break

            if stat is True:
                instr_cnt += 1
                init_val_cnt = self.__frames.init_cnt()
                if init_val_cnt > max_init:
                    max_init = init_val_cnt

            #print("instr = {0}\n".format(instr), file=sys.stderr)   # LADICI VYPIS

            if instr.opcode() == "CREATEFRAME":
                self.__frames.createframe()

            elif instr.opcode() == "PUSHFRAME":
                self.__frames.pushframe()

            elif instr.opcode() == "POPFRAME":
                self.__frames.popframe()

            elif instr.opcode() == "DEFVAR":
                self.__frames.defvar(instr.arg_1())

            elif instr.opcode() == "CALL":
                # Uloží následující pozici v pásce do zásobníku volání.
                self.__call_st.push(self.__tape.actual())
                self.__tape.jump(instr.arg_1())

            elif instr.opcode() == "RETURN":
                self.__tape.ret(self.__call_st.pop())

            elif instr.opcode() == "JUMP":
                self.__tape.jump(instr.arg_1())

            elif instr.opcode() == "LABEL":
                pass

            elif instr.opcode() == "MOVE":
                self.__frames.setvar(instr.arg_1(), self.get_v(instr.arg_2()))

            elif instr.opcode() == "PUSHS":
                self.__stack.push(self.get_v(instr.arg_1()))

            elif instr.opcode() == "POPS":
                self.__frames.setvar(instr.arg_1(), self.__stack.pop())

            elif instr.opcode() == "ADD":
                self.__frames.setvar(instr.arg_1(),
                                     self.arit(self.get_v(instr.arg_2()), self.get_v(instr.arg_3()),
                                               lambda a, b: a + b))

            elif instr.opcode() == "SUB":
                self.__frames.setvar(instr.arg_1(),
                                     self.arit(self.get_v(instr.arg_2()), self.get_v(instr.arg_3()),
                                               lambda a, b: a - b))

            elif instr.opcode() == "MUL":
                self.__frames.setvar(instr.arg_1(),
                                     self.arit(self.get_v(instr.arg_2()), self.get_v(instr.arg_3()),
                                               lambda a, b: a * b))

            elif instr.opcode() == "IDIV":
                if type(instr.arg_2()) is float or type(instr.arg_3()) is float:
                    raise TypeError("Špatný typ operandů aritmetické instrukce.")
                self.__frames.setvar(instr.arg_1(),
                                     self.arit(self.get_v(instr.arg_2()), self.get_v(instr.arg_3()),
                                               lambda a, b: a // b))

            elif instr.opcode() == "DIV":
                if type(instr.arg_2()) is int or type(instr.arg_3()) is int:
                    raise TypeError("Špatný typ operandů aritmetické instrukce.")
                self.__frames.setvar(instr.arg_1(),
                                     self.arit(self.get_v(instr.arg_2()), self.get_v(instr.arg_3()),
                                               lambda a, b: a / b))

            elif instr.opcode() == "LT":
                self.__frames.setvar(instr.arg_1(),
                                     self.rel(self.get_v(instr.arg_2()), self.get_v(instr.arg_3()),
                                              lambda a, b: a < b))

            elif instr.opcode() == "GT":
                self.__frames.setvar(instr.arg_1(),
                                     self.rel(self.get_v(instr.arg_2()), self.get_v(instr.arg_3()),
                                              lambda a, b: a > b))

            elif instr.opcode() == "EQ":
                # Podporuje porovnávání na nil
                self.__frames.setvar(instr.arg_1(),
                                     self.rel(self.get_v(instr.arg_2()), self.get_v(instr.arg_3()),
                                              lambda a, b: a == b, allow_nil=True))

            elif instr.opcode() == "AND":
                self.__frames.setvar(instr.arg_1(),
                                     self.logic(self.get_v(instr.arg_2()), self.get_v(instr.arg_3()),
                                                lambda a, b: a and b))

            elif instr.opcode() == "OR":
                self.__frames.setvar(instr.arg_1(),
                                     self.logic(self.get_v(instr.arg_2()), self.get_v(instr.arg_3()),
                                                lambda a, b: a or b))

            elif instr.opcode() == "NOT":
                self.__frames.setvar(instr.arg_1(),
                                     self.logic(self.get_v(instr.arg_2()), is_not=True))

            elif instr.opcode() == "JUMPIFEQ":
                v_1, v_2 = self.get_vars_jump(instr.arg_2(), instr.arg_3())

                if v_1 is None or v_2 is None:
                    if v_1 is v_2:
                        self.__tape.jump(instr.arg_1())
                else:
                    if v_1 == v_2:
                        self.__tape.jump(instr.arg_1())

            elif instr.opcode() == "JUMPIFNEQ":
                v_1, v_2 = self.get_vars_jump(instr.arg_2(), instr.arg_3())

                if v_1 is None or v_2 is None:
                    if v_1 is not v_2:
                        self.__tape.jump(instr.arg_1())
                else:
                    if v_1 != v_2:
                        self.__tape.jump(instr.arg_1())

            elif instr.opcode() == "EXIT":
                v_1 = self.get_v(instr.arg_1())

                if type(v_1) is not int:
                    raise TypeError("Špatný typ operandu instrukce EXIT.")
                if v_1 < 0 or v_1 > 49:
                    raise ValueError("Neplatná hodnota EXIT.")
                else:
                    sys.exit(v_1)

            elif instr.opcode() == "BREAK":
                print("Instrukční páska: {0}\n{1}".format(self.__tape, "#" * 50), file=sys.stderr)
                print("Rámce (GF, LF, TF): {0}\n{1}".format(self.__frames, "#" * 50), file=sys.stderr)

            elif instr.opcode() == "INT2CHAR":
                v_1 = self.get_v(instr.arg_2())

                if type(v_1) is not int:
                    raise TypeError("Špatný typ operandu při instrukci INT2CHAR.")
                try:
                    self.__frames.setvar(instr.arg_1(), chr(v_1))
                except ValueError:
                    raise StrError("Nevalidní hodnoty znaku Unicode při instrukci INT2CHAR.")

            elif instr.opcode() == "STRI2INT":
                v_1 = self.get_v(instr.arg_2())
                v_2 = self.get_v(instr.arg_3())

                if type(v_1) is not str or type(v_2) is not int:
                    raise TypeError("Špatný typ operandu při instrukci STRI2INT.")
                if v_2 < 0:
                    raise StrError("Index mimo řetězec při instrukci STRI2INT.")

                try:
                    self.__frames.setvar(instr.arg_1(), ord(v_1[v_2]))
                except IndexError:
                    raise StrError("Index mimo řetězec při instrukci STRI2INT.")

            elif instr.opcode() == "INT2FLOAT":
                v_2 = self.get_v(instr.arg_2())

                if type(v_2) is not int:
                    raise TypeError("Špatný typ operandu při instrukci INT2FLOAT.")

                self.__frames.setvar(instr.arg_1(), float(v_2))

            elif instr.opcode() == "FLOAT2INT":
                v_2 = self.get_v(instr.arg_2())

                if type(v_2) is not float:
                    raise TypeError("Špatný typ operandu při instrukci FLOAT2INT.")

                self.__frames.setvar(instr.arg_1(), int(v_2))

            elif instr.opcode() == "CONCAT":
                v_1 = self.get_v(instr.arg_2())
                v_2 = self.get_v(instr.arg_3())

                if type(v_1) is not str or type(v_2) is not str:
                    raise TypeError("Špatný typ operandů pří instrukci CONCAT.")
                self.__frames.setvar(instr.arg_1(), v_1 + v_2)

            elif instr.opcode() == "STRLEN":
                v_1 = self.get_v(instr.arg_2())
                if type(v_1) is not str:
                    raise TypeError("Špatný typ oprandu při instrukci STRLEN.")
                self.__frames.setvar(instr.arg_1(), len(v_1))

            elif instr.opcode() == "GETCHAR":
                v_1 = self.get_v(instr.arg_2())
                v_2 = self.get_v(instr.arg_3())

                if type(v_1) is not str or type(v_2) is not int:
                    raise TypeError("Špatný typ operandů při instrukci GETCHAR.")
                if v_2 < 0 or v_2 > (len(v_1) - 1) or not v_1:
                    raise StrError("Indexace mino řetězec při instrukci GETCHAR.")

                self.__frames.setvar(instr.arg_1(), v_1[v_2])

            elif instr.opcode() == "SETCHAR":
                v_1 = self.get_v(instr.arg_1())
                v_2 = self.get_v(instr.arg_2())
                v_3 = self.get_v(instr.arg_3())

                if type(v_1) is not str or type(v_2) is not int or type(v_3) is not str:
                    raise TypeError("Špatný typ operandů při instrukci SETCHAR.")
                if v_2 < 0 or v_2 > (len(v_1) - 1) or not v_3:
                    raise StrError("Indexace mino řetězec při instrukci SETCHAR.")
                self.__frames.setvar(instr.arg_1(), v_1[:v_2] + v_3[0] + v_1[v_2+1:])

            elif instr.opcode() == "TYPE":
                try:
                    v_1 = self.get_v(instr.arg_2())

                    if type(v_1) is int:
                        self.__frames.setvar(instr.arg_1(), "int")
                    elif type(v_1) is bool:
                        self.__frames.setvar(instr.arg_1(), "bool")
                    elif type(v_1) is str:
                        self.__frames.setvar(instr.arg_1(), "string")
                    elif type(v_1) is float:
                        self.__frames.setvar(instr.arg_1(), "float")
                    else:
                        self.__frames.setvar(instr.arg_1(), "nil")
                except fr.ValueMissError:
                    self.__frames.setvar(instr.arg_1(), "")

            elif instr.opcode() == "WRITE":
                v_1 = self.get_v(instr.arg_1())
                if type(v_1) is bool:
                    print(str(v_1).lower(), end="")
                elif v_1 is None:
                    print("", end="")
                elif type(v_1) is float:
                    print(float.hex(v_1), end="")
                else:
                    print(v_1, end="")

            elif instr.opcode() == "READ":
                # Čte zadaný typ, pokud se nepovede uloží do výsledku nil.
                try:
                    in_val = input()
                    to_save = None
                    if instr.arg_2() == "int":
                        if re.match(r"^(\+|\-)?([1-9][0-9]*$|0$)", in_val):
                            to_save = int(in_val)
                    elif instr.arg_2() == "bool":
                        if in_val.lower() == "true":
                            to_save = True
                        else:
                            to_save = False
                    elif instr.arg_2() == "float":
                        try:
                            to_save = float.fromhex(in_val)
                        except ValueError:
                            to_save = None
                    else:
                        to_save = in_val
                except EOFError:
                    to_save = None

                self.__frames.setvar(instr.arg_1(), to_save)

            elif instr.opcode() == "DPRINT":
                v_1 = self.get_v(instr.arg_1())
                if type(v_1) is bool:
                    print(str(v_1).lower(), end="", file=sys.stderr)
                elif v_1 is None:
                    print("", end="", file=sys.stderr)
                else:
                    print(v_1, end="", file=sys.stderr)

            elif instr.opcode() == "CLEARS":
                self.__stack.clear()

            elif instr.opcode() == "ADDS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                self.__stack.push(self.arit(v_1, v_2, lambda a, b: a + b))

            elif instr.opcode() == "SUBS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                self.__stack.push(self.arit(v_1, v_2, lambda a, b: a - b))

            elif instr.opcode() == "MULS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                self.__stack.push(self.arit(v_1, v_2, lambda a, b: a * b))

            elif instr.opcode() == "IDIVS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                if type(v_1) is float or type(v_2) is float:
                    raise TypeError("Špatný typ operandů aritmetické instrukce.")
                self.__stack.push(self.arit(v_1, v_2, lambda a, b: a // b))

            elif instr.opcode() == 'DIVS':
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                if type(v_1) is int or type(v_2) is int:
                    raise TypeError("Špatný typ operandů aritmetické instrukce.")
                self.__stack.push(self.arit(v_1, v_2, lambda a, b: a / b))

            elif instr.opcode() == "LTS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                self.__stack.push(self.rel(v_1, v_2, lambda a, b: a < b))

            elif instr.opcode() == "GTS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                self.__stack.push(self.rel(v_1, v_2, lambda a, b: a > b))

            elif instr.opcode() == "EQS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                self.__stack.push(self.rel(v_1, v_2, lambda a, b: a == b,
                                           allow_nil=True))

            elif instr.opcode() == "ANDS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                self.__stack.push(self.logic(v_1, v_2, lambda a, b: a and b))

            elif instr.opcode() == "ORS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                self.__stack.push(self.logic(v_1, v_2, lambda a, b: a or b))

            elif instr.opcode() == "NOTS":
                v_1 = self.__stack.pop()
                self.__stack.push(self.logic(v_1, is_not=True))

            elif instr.opcode() == "INT2CHARS":
                v_1 = self.__stack.pop()
                if type(v_1) is not int:
                    raise TypeError("Špatný typ operandu při instrukci INT2CHAR.")
                try:
                    self.__stack.push(chr(v_1))
                except ValueError:
                    raise StrError("Nevalidní hodnoty znaku Unicode při instrukci INT2CHAR.")

            elif instr.opcode() == "STRI2INTS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                if type(v_1) is not str or type(v_2) is not int:
                    raise TypeError("Špatný typ operandu při instrukci STRI2INT.")
                if v_2 < 0:
                    raise StrError("Index mimo řetězec při instrukci STRI2INT.")
                try:
                    self.__stack.push(ord(v_1[v_2]))
                except IndexError:
                    raise StrError("Index mimo řetězec při instrukci STRI2INT.")

            elif instr.opcode() == "INT2FLOATS":
                v_1 = self.__stack.pop()

                if type(v_1) is not int:
                    raise TypeError("Špatný typ operandu při instrukci INT2FLOATS.")

                self.__stack.push(float(v_1))

            elif instr.opcode() == "FLOAT2INTS":
                v_1 = self.__stack.pop()

                if type(v_1) is not float:
                    raise TypeError("Špatný typ operandu při instrukci FLOAT2INTS.")

                self.__stack.push(int(v_1))

            elif instr.opcode() == "JUMPIFEQS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                if v_1 is None or v_2 is None:
                    if v_1 is v_2:
                        self.__tape.jump(instr.arg_1())
                else:
                    if type(v_1) is not type(v_2):
                        raise TypeError("Špatný typ operandů instrukce JUMPIFNEQS.")
                    if v_1 == v_2:
                        self.__tape.jump(instr.arg_1())

            elif instr.opcode() == "JUMPIFNEQS":
                v_2 = self.__stack.pop()
                v_1 = self.__stack.pop()
                if v_1 is None or v_2 is None:
                    if v_1 is not v_2:
                        self.__tape.jump(instr.arg_1())
                else:
                    if type(v_1) is not type(v_2):
                        raise TypeError("Špatný typ operandů instrukce JUMPIFNEQS.")
                    if v_1 != v_2:
                        self.__tape.jump(instr.arg_1())

            else:
                print("Neznámý opcode {0} při vykonávání programu.", file=sys.stderr)
                sys.exit(99)

        return instr_cnt, max_init
