"""instr_tape.py
Modul obsahující instrukční pásku a jednotlivé instrukce s operandy
a metodani definovanými nad nimi.
Autor: Michal Šedy <xsedym02>
Spuštění: python3.8
Poslední změna: 17.2.2020
"""


class LabelError(Exception):
    """ Vyjímka skoku na nedefinované návěští, nebo redefinice návěští."""
    pass


class Instruction:
    """ Třída obsahující instrukci programu.
    __opcode - operační kód
    __arg_1 - první opedand
    __arg_2 - druhý operand
    __arg_3 - třetí operand

    Exceptions
    ----------
    LabelError
    """

    def __init__(self, opcode, arg_1=None, arg_2=None, arg_3=None):
        """ Metoda inicializace instrukce

        Metoda nastaví opcode. A operandy (standartně NULL), pokud jsou zadány.

        Params
        ------
        opcode - operační kód IPPcode20 (string)
        arg_1 - první operand (volitelné)
        arg_2 - druhý operand (volitelné)
        arg_3 - třetí operand (volitelné)
        """
        self.__opcode = opcode
        self.__arg_1 = arg_1
        self.__arg_2 = arg_2
        self.__arg_3 = arg_3

    def __repr__(self):
        return "Instruction({0}, {1}, {2}, {3})".format(self.__opcode, self.__arg_1,
                                                        self.__arg_2, self.__arg_3)

    def __str__(self):
        return "({0}, {1}, {2}, {3})".format(self.__opcode, self.__arg_1, self.__arg_2,
                                             self.__arg_3)

    def opcode(self):
        """ Metoda pro získání hodnoty operandu instrukce."""
        return self.__opcode

    def arg_1(self):
        """ Metoda pro získání hodnoty prvního operandu instrukce."""
        return self.__arg_1

    def arg_2(self):
        """ Metoda pro získání hodnoty druhého operandu instrukce."""
        return self.__arg_2

    def arg_3(self):
        """ Metoda pro získání hodnoty třetího operandu instrukce."""
        return self.__arg_3


class InstrTape:
    """ Třída představující instrukční pásku programu v IPPcode20.
    __tape - data instrukcí na pásce
    __labels - jména návěší v pásce s pozicemi odpovídajících instrukcí
    __act - aktuální čtený prvek pásky, neaktivní páíska --> -1
    __size - velikost pásky (počet instrukcí v pásce)
    """

    def __init__(self):
        """ Metoda pro inicializaci instrukční pásky."""
        self.__tape = list()
        self.__labels = dict()
        self.__act = -1
        self.__size = 0

    def __repr__(self):
        return "InstrTape({0}, {1}, {2}, {3})".format(self.__tape, self.__act,
                                                      self.__size, self.__labels)

    def __str__(self):
        return "({0}, {1}, {2}, {3})".format(self.__tape, self.__act,
                                             self.__size, self.__labels)

    def append(self, instr):
        """ Metoda pro přidání nové instrukce na konec instrukční pásky.

        Pokud bude přidávána instrukce label, přidá se zároveň do tabulky návěští.
        Metoda inkrementuje velikost instrukční pásky (počet instrukcí).

        Exceptions
        ----------
        LabelError - Pokud ale takové návěští již existuje, bude vyvolana vyjímka.

        Params
        ------
        instr - instrukce k přidání do pásky
        """
        if instr.opcode() == 'LABEL':
            self.set_label(instr.arg_1(), self.__size)
        self.__tape.append(instr)
        self.__size += 1

    def activate(self):
        """ Metoda aktivuje instrukční pásku pro vykonávání instrukcí.

        Pokud je páska prázdná, k akrivování nedojde.
        """
        self.__act = -1 if (self.__size == 0) else 0

    def set_label(self, label_name, pos):
        """ Metoda přidá jméno návěští do tabulky návěští a jeho pozicí v kódu.

        Exceptions
        ----------
        LabelError - Pokud již návěští stejného jména existuje je zachycena
                     vyjímka KeyError a vyvoláná vyjímika LabelError.

        Params
        ------
        label_name - jméno návěští
        pos - pozice v kódu
        """
        if self.__labels.get(label_name) is not None:
            raise LabelError("Redefinice návěští.")
        self.__labels[label_name] = pos

    def jump(self, label_name):
        """ Metoda provede skok v pásce na příslušné návěští.

        Exceptions
        ---------
        LabelError - Pokud návěští zadaného jména neexistuje je zachycena
                     vyjímka KeyError a předána jako vyjímka LabelError.

        Params
        ------
        label_name - jméno vávěští pro skok
        """
        try:
            self.__act = self.__labels[label_name]
        except KeyError:
            raise LabelError("Návěští neexistuje.")

    def ret(self, dst):
        """ Metoda pro návrat z volání na pozici v pásce definovanou v dst.

        Params
        ------
        dst - Pozice v pásce, kam se má vrátit řízení programu.
        """
        self.__act = dst

    def read(self):
        """ Metoda pro získání aktuálně prováděné instrukce z pásky.

            Po přečtení se aktivita posune na další instrukci, pokud
            se aktivita posune za hranici pásky, pak se aktivita ztrácí
            a páska se stává znovu neaktivní __act == -1.

            Return
            ------
            V případě úspěchu metoda vrátí aktuální instrukci z pásky.
            Pokud je páska neaktivní, vrací None
        """
        if self.__act == -1 or self.__size == 0:
            return None
        else:
            to_return = self.__tape[self.__act]

            if self.__act == self.__size - 1:
                self.__act = -1
            else:
                self.__act += 1

            return to_return

    def actual(self):
        """ Metoda pro získání pozice následující instrukce."""
        return self.__act

    def size(self):
        """ Metoda pro získání velikosti instrukční pásky (počet instrukcí)."""
        return self.__size
