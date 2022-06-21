Soubor README k projektu 2 (Turing-machine) z předmětu FLP
Autor: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Poslední úprava: 25.04.2022

Struktůra adresáře
==================
.
├── doc
│   └── README.txt
├── Makefile
├── src
│   ├── io.pl
│   ├── ntm.pl
│   └── sim.pl
└── test
    ├── auto
    ├── manual
    │   ├── accept
    │   │   ├── t1-nonempty.in
    │   │   ├── t2-right.in
    │   │   ├── t3-rightLeft.in
    │   │   ├── t4-catchableLoop.in
    │   │   ├── t5-noblank.in
    │   │   ├── t6-anbn.in
    │   │   └── t7-original.in
    │   ├── error
    │   │   ├── t1-emptyFile.in
    │   │   ├── t2-onlyRules.in
    │   │   ├── t3-onlyTape.in
    │   │   ├── t4-badState.in
    │   │   ├── t5-noS.in
    │   │   ├── t6-badLetter.in
    │   │   ├── t7-uncatchableLoop.in
    │   │   └── t8-moreSpaces.in
    │   └── reject
    │       ├── t1-catchableLoop.in
    │       ├── t2-noF.in
    │       └── t3-easyReject.in
    └── utils
        ├── ntmGen.py
        ├── testing.sh
        ├── testsGen.sh
        └── validator.py


IMPLEMENTACE
============
Ze standardního vstupu je načten Turingův stroj a je provedena
kontrola formátu pravidel a vstupní pásky. V případě chybného vstupu
je program ukončen s návratovým kódem 2.

Pokud neobsahuje stroj koncový stav, je automaticky zamítnuta
vstupní páska a program je ukončen s návratovým kódem 1, jinak
je simulován běh Turingova stroje na vstupní pásce. Pokud Turingův
stroj pásku akceptuje, je program ukončen s návratovým kódem 0, jinak 1.

Pravidla Turingova stroje jsou implementována za pomocí dynamických
predikátů. Simulace detekuje jednoduché případy zacyklení za pomocí
uchovávání všech konfigurací stroje, ve kterých se nacházel.


MODULY
======
sim.pl   Hlavní modul programu.
ntm.pl   Modul Turingova stroje a jeho přechodů.
io.pl    Vstupně výstupní operace (převzato z WIS).


NÁVRATOVÉ KÓDY
==============
0 - Turingův stroj akceptuje vstupní pásku.
1 - Turingův stroj neakceptuje vstupní pásku.
2 - Chybný formát vstupu.


PŘEKLAD
=======
Program se překládá pomocí Makefile zadáním příkazu 'make'.
Tímto se v kořenové složce vytvoří spustitelný soubor 'flp21-log'.


SPOUŠTĚNÍ
=========
./flp21-fun <input >output

    Program čte Turingův stoj ze standardního vstupu a na standardní výstup
    vypisuje modifikace konfigurací, které vedly z počátečního stavu (S) do
    akceptujícího stav (F).

    -h, --help  Vypíše příklad spuštění.

    Návratové kódy
    --------------
        0 - Turingův stroj akceptuje vstupní pásku.
        1 - Turingův stroj odmítl vstupní pásku.
        2 - Chybný formát Turingova stroje.


TVAR TURINGOVA STROJE
=====================
(pravidlo_1)
(pravidlo_2)
...
(pravidlo_N)
(páska)
--------------------
Formát pravidel:
(stav) (symb) (stav) (modifikace)
--------------------
Formát pásky:
(symb_1)(symb_2)...(symb_N)
-------PŘÍKLAD------
S # A R
A a B b
B b C L
C # F #
#ab
--------------------
- symboly jsou [a-z] a speciální symbol blank (#)
- modifikace je buďto symbol, nebo posun hlavy vpravo (R), nebo vlevo (L)
- Zápis stroje se skládá ze seznamu pravidel a na posledním řádku je obsah vstupní pásky.
- Mezi pravidly ani páskou se nesmí vyskytovat žádné prázdné řádky.
- Jednotlivé části pravidla jsou odděleny jednou mezerou.
- Musí být zadaný počáteční stav S.


TESTOVÁNÍ
=========
Za účelem testování správnosti naimplementovaného algoritmu, byly vytvořeny automatické testy
('test/auto). Automatické testování se spustí z kořene složky zadáním 'make test'. Testy jsou 
generovány a spouštěny paralelně. Toto chování je ovládáno hodnotou proměnné 'MAXPROC' v souboru 
Makefile. V některých případech může příliš vysoká hodnota vést k selhání při použití fork.
(Řešení: snížit hodnotu.). Dále jsou simulace Turingových strojů limitované na dobu běhu 5-ti sekund.
Pokud do této doby test neskončí, vynechá se (ovlivňuje přesnost testu). Toto chování je ovládáno
hodnotou proměnné 'TIMEOUT' v souboru Makefile (je možné změnit). Automatické testování na serveru
Merlin při zmíněném nastavení trvá 40s.

Soubor manuálních testů, terý obsahuje turingovy stroje, které akceptují, zamítací i stroje
ve špatném formátu jsou umístěny do složky 'test/manual'.

Složka test/utils obsahuje:
    testGen.sh
    ----------
        ./testsGen.sh <tests_folder> <max_proc>

            Vygeneruje 1539 nedeterministických Turingových strojů různé charakteristiky 
            (délka vstupní pásky, velikost abecedy, počet stavů, počet kroků, ...) do složky
            'tests_folder'. Testy jsou generovány paralelně v maximálně 'max_proc' paralelních 
            procesech.

            tests_folder      Složka, kam budou vygenerované testy uloženy.

            max_proc          Počet paralelně běžících generátorů testů.


    testing.sh
    ----------
        ./testing.sh <test_folder> <program> <max_proc> <timeout>

            Otestuje všechny vstupy ze složky 'test_folder' na programu. Testování
            je prováděno paralelně v 'max_proc' procesech. Pokud jednotlivý test
            neskončí do času 'timeout', je zastaven automaticky a odebrán z výsledků
            (snižuje přesnost testů). Výsledek programu je poté zkontrolován programem
            'validator.py'.

            test_folder       Složka s testy.

            program           Testovaný program.

            max_proc          Počet paralelně běžících generátorů testů.

            timeout           Maximální povolená doba běhu jednoho testu.


    ntmGen.py
    ---------
        ./gen.py <alphabet_size> <states> <input_len> <left_prob> <change_prob> <steps> {--verbose}

            Vygeneruje na standardní výstup Turingův stroj na základě zadaných parametrů.

            alphabet_size     Velikost abecedy.

            states            Počet stavů.

            input_len         Velikost vstupní pásky.

            left_prob         Pravděpodobnost posunu čtecí/zapisovací hlavy vlevo.

            change_prob       Pravděpodobnost zápisu nového znaku.

            steps             Počet kroků k dosažení akceptujícího stavu.

            --verbose         Na standardní chybový výstup tiskne postupné
                              modifikace konfigurací.


    validator.py
    ------------
        ./validator.py <input> <rules>

            Zkontroluje, jestli se k postupných modifikacím konfigurací,
            zaznamenaných v souboru 'input', Turingova stroje dospělo za
            použití korektní a existujících pravidel ze souboru 'rules'.

            input             Záznam modifikací konfigurací (pásky).

            rules             Pravidla přechodů Turingova stroje.


Složka test/manual obsahuje:
    error
    -----
        Soubor Turingových strojů, které mají chybnou strukturu, nebo obsahují nekonečný cyklus,
        který vede k přetečení zásobníku volání.

        t1-emptyFile.in              Prázný soubor.
        t2-onlyRules.in              Není zadána vstupní páska.
        t3-onlyTape.in               Nejsou zadány pravidla.
        t4-badState.in               Chybný formát stavu stroje.
        t5-noS.in                    Neobsahují povinný počáteční stav S.
        t6-badLetter.in              Chybný formát symbolu.
        t7-uncatchableLoop.in        Nekonečná smyčka (neustálý posun vpravo) vede na
                                     přetečení zásobníku.
        t8-moreSpaces.in             Více než jedna mezera v kódování pravidla stroje

    reject
    ------
        Soubor Turingových strojů s korektní strukturou, které ale neakceptují svou vstupní pásku.

        t1-catchableLoop.in          Obsahuje detekovatelnou smyčku.
        t2-noF.in                    Neobsahuje koncový stav.
        t3-easyReject.in             Páska není akceptována.

    accept
    ------
        Soubor Turingových strojů s korektní strukturou, které akceptují svou vstupní pásku.

        t1-nonempty.in               Test na neprázdnost. (0,013s)
        t2-right.in                  Obsahuje posun vpravo. (0,011s)
        t3-rightLeft.in              Obsahuje posun vpravo a vlevo. (0,014s)
        t4-catchableLoop.in          Obsahuje nedeterministickou smyčku. (0,014s)
        t5-noblank.in                Neobsahuje blank na první pozici pásky. (0,014s)
        t6-anbn.in                   Akceptuje a^nb^n. (0,014s)
        t7-original.in               Turingův stroj ze zadání. (0,014s)
