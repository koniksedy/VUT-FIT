"""var_frame.py
Modul obsahující soubor všech rámců (GF, LF, TF) a taky jednotlivých
operací nad nimi.
Autor: Michal Šedy <xsedym02>
Spuštění: python3.8
Poslední změna: 18.3.2020
"""


class RedefError(Exception):
    """ Vyjímka při redefinici proměnné."""
    pass


class ValueMissError(Exception):
    """ Vyjímka při chybějící hodnotě proměnné nebo na zásobníku"""
    pass


class FrameError(Exception):
    """ Vyjímka pro přístup k neexistujícímu rámci."""
    pass


class Variable(str):
    """ Třída udávající pouze typ jménu proměnné uložené v rámcích.
    Pomáhá rozlišit proměnnou od řetězce."""
    pass


class Frame(dict):
    """ Třída představující jednotlivé paměťové rámce (GF, LF, TF) IPPcode20.

    Exceptions
    ----------
    RedefError
    KeyError
    """

    def define(self, name):
        """ Metoda pro definice proměnné daného rámce. Nepovoluje redefinice.

        Params
        ------
        name - jméno proměnné pro definicik

        Exceptions
        ----------
        RedefError - V případě redefinice již existující prověnné vyvolá vyjímku.
        """
        if(name in self.keys()):
            raise RedefError("Redefinice proměnné {0}.".format(name))
        self[name] = list()

    def get_value(self, name):
        """ Metoda pro získání proměnné daného jména z rámce.

        Params
        ------
        name - jméno získávané proměnné

        Excetions
        ---------
        KeyError - Pokud dotazovaná proměnná neexistuje (ale rámec ano),
                   je vyvolána vyjímka KeyError
        ValueMissError - Hodnota proměnné je nedefinovaná.

        Return
        ------
        Funkce vrátí hodnotu proměnné name uložené v daném rámci.
        """
        if(type(self[name]) is list):
            raise ValueMissError("Proměnná nemá specifikovanou hodnotu.")
        return self[name]

    def set_value(self, name, value):
        """ Metoda pro přiřazení hodnody od proměnné.

        Params
        ------
        name - jméno proměnné k nastavení
        value - hodnota pro uložení do proměnné name

        Exceptions
        ----------
        KeyError - Pokud proměnná neexistuje (ale rámec ano), je vyvolána vyjímka.
        """
        if name not in self.keys():
            raise KeyError()

        self[name] = value


class FrameSpace:
    """ Třída obsahující všechny rámce programu (GF, LF, TF).

    Exceptions
    ----------
    FrameError
    RedefError
    KeyError
    ValueMissError
    """

    def __init__(self):
        """ Metoda pro inicializaci rámců.

        Inicializuje se pouze GF, ostatní jsou nedefinované.
        """
        self.__gf = Frame()
        self.__lf_stack = list()    # Zásobník lokálních rámců
        self.__tf = None

    def __repr__(self):
        return "FrameSpace({0}, {1}, {2})".format(self.__gf, self.__lf_stack, self.__tf)

    def __str__(self):
        return repr(self)

    def init_cnt(self):
        """ Vrátí počet inicializovaných proměnných ve všech rámcích."""
        cnt = len([v for k, v in self.__gf.items() if type(v) is not list])
        if self.__tf is not None:
            cnt += len([v for k, v in self.__tf.items() if type(v) is not list])
        if self.__lf_stack is not None:
            cnt += len([v for s in self.__lf_stack for k, v in s.items() if type(v) is not list])

        return cnt

    def createframe(self):
        """ Metoda pro vytvoření nového TF.

        Stavý rámec je přepsám.
        """
        self.__tf = Frame()

    def pushframe(self):
        """ Metoda pro přesun TF na vrchol LF.

        Po provedení bude TF nedefinovaný.

        Exceptions
        ----------
        FrameError - V případě přesunu nedefinovaného TF je vyvolána tato vyjímka.
        """
        # Kontrola existence TF
        if self.__tf is None:
            raise FrameError("PUSHFRAME na neinicializovaný TF.")
        self.__lf_stack.append(self.__tf)
        self.__tf = None

    def popframe(self):
        """ Metoda pro přesun rámce z vrcholu LF do TF.

        Exceptions
        ----------
        FrameError - V případě prázdného, nebo neexistujícího LF je vyvolána tato vyjímka.
        """
        # Zachycení vyjímky IndexError a převedení na FrameError.
        try:
            self.__tf = self.__lf_stack.pop()
        except IndexError:
            raise FrameError("Provádějí POPFRAME na prázdný LF.")

    def defvar(self, name):
        """ Metoda pro definici proměnné v konkrétním rámci na záklacě předpony ve jméně.

        Proměnná musí již obsahovat známý druh rámce(GF, LF, TF).

        Params
        ------
        name - Jméno proměnné s předponou označující rámes GF#var (glovální rámec)

        Exceptions
        ----------
        FrameError - Pokus o přístup k nedefinovanému rámci vyvolá vyjímku TypeError,
                     která je převedana na FrameError.
        RedefError - Vyjímka vyvolána v případě redefinice proměnné.
        """
        try:
            if name[:2] == "GF":
                self.__gf.define(name[3:])
            elif name[:2] == "LF":
                self.__lf_stack[-1].define(name[3:])
            elif name[:2] == "TF":
                self.__tf.define(name[3:])
            else:
                raise FrameError("Neznámý typ rámce {0}.".format(name[:2]))
        except (TypeError, IndexError, AttributeError):
            raise FrameError("Přístup k nedefinovanému rámci vyvolán operací DEFVAR.")

    def setvar(self, name, value):
        """ Metoda pro přižazení hodnoty do proměnné v rámsi specifikovaným její předponou.

        Params
        ------
        name - Jméno proměnné s předponou označující rámes GF#var (glovální rámec)
        value - hodnota k přířazení

        Exceptions
        ----------
        KeyError - Pokud proměnná neexistuje (ale rámec ano).
        FrameError - Pokus o přístup k nedefinovanému rámci vyvolá vyjímku TypeError,
                     která je převedana na FrameError.
        """
        try:
            if name[:2] == "GF":
                self.__gf.set_value(name[3:], value)
            elif name[:2] == "LF":
                self.__lf_stack[-1].set_value(name[3:], value)
            elif name[:2] == "TF":
                self.__tf.set_value(name[3:], value)
            else:
                raise FrameError("Neznámý typ rámce {0}.".format(name[:2]))
        except (TypeError, IndexError, AttributeError):
            raise FrameError("Přístup k nedefinovanámu rámci.")

    def getvar(self, name):
        """ Metoda pro získání hodnoty proměnné z rámse spedcifikovaného její předponou.

        Params
        ------
        name - Jméno proměnné s předponou označující rámes GF#var (glovální rámec)

        Return
        ------
        Funkce vrací číselnou hodnotu získanou z proměnné v daném rámci.

        Exceptions
        ----------
        KeyError - Pokud proměnná neexistuje (ale rámec ano).
        FrameError - Přístup k nedefinovanému rámci při čtení hodnoty vyvolá vyjímku
                     TypeError, která je převedena na FrameError
        ValueMissError - Hodnota proměnné je nedefinovaná.
        """
        try:
            if(name[:2] == "GF"):
                return self.__gf.get_value(name[3:])
            elif(name[:2] == "LF"):
                return self.__lf_stack[-1].get_value(name[3:])
            elif(name[:2] == "TF"):
                return self.__tf.get_value(name[3:])
            else:
                raise FrameError("Neznámý typ rámce {0}.".format(name[:2]))
        except (TypeError, IndexError, AttributeError):
            raise FrameError("Přístup k nedefinovanámu rámci.")
