Soubor README k projektu 1 (SIMPLIFY-BKG) z předmětu FLP
Autor: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Poslední úprava: 24.03.2022


Struktura adresáře
==================
.
├── doc
│    └── README.txt
├── Makefile
├── src
│    ├── Main.hs
│    ├── Minimization.hs
│    ├── ParseInput.hs
│    └── Types.hs
└── test
    └── utils
        ├── gramGen.py
        ├── testing.sh
        └── testsGen.sh


PŘEKLAD
=======
Program se překládá pomocí Makefile zadáním příkazu 'make'.
Tímto se v kořenové složce vytvoří spustitelný soubor 'flp21-fun'.


SPOUŠTĚNÍ
=========
./flp21-fun [input] <-i|-1|-2>

    input       Jméno vstupního souboru s gramatikou. (Volitelný parametr).
                Pokud není zadán je gramatika čtena ze standardního vstupu

    -i          Načtená gramatika bude beze změn vypsána na standardní výstup.

    -1          Z gramatiky budou odstraněny negenerující neterminály a jim
                odpovídající pravidla. Výsledná gramatika bude vypsána na
                standardní výstup.

    -2          Z gramatiky budou odstraněny negenerující neterminály,
                nedostupné symboly a jim odpovídající pravidla. Výsledná
                gramatika bude vypsána na standardní výstup.

    -h, --help  Vypíše příklad spuštění.


TVAR VSTUPNÍ GRAMATIKY
======================
(neterminály)
(terminály)
(počáteční neterminál)
(pravidlo)
(pravidlo)
(...)
-----------------------
A,B,Z
a,b
Z
Z->#
Z->AB
A->a
B->b
----------------------
- # značí speciální znak pro epsilon.
- neterminály jsou znaky [A-Z]
- terminály jsou znaky [a-z] (seznam terminálů může být prázdný)
- počáteční neterminál musí být obsažen v seznamu neterminálů
- pravidla nesmí využívat nedefinované terminály, nebo neterminály,
  nesmí se vyskytovat duplicitní pravidle, seznam pravidel může být prázdn

Pokud je na vstupu programu gramatika v nesprávném tvaru, bude program
ukončen s chybovou hláškou.


TESTOVÁNÍ
=========
Za účelem testování správnosti naimplementovaného algoritmu, byly vytvořeny automatické testy.
Automatické testování se spustí z kořene složky zadáním 'make test'.

Složka test/utils obsahuje:
    testGen.sh
    ----------
        ./testsGen.sh <tests_folder>

            Vygeneruje 3000 gramatik a jejich verze (-i, -1, -2) do složky tests_folder

    gramGen.py
    ------------
        python3 testGen.py <nts_cnt> <ts_cnt> <rels_per_nt> <max_righ_side> <epsilon_prob> <output>

            Program generuje tři gramatiky se strukturálními vlastnostmi na základě parametrů.
            Vygegeruje:
                *-i.cfg     obsahuje nezměněnou gramatiku
                *-1.cfg     obsahuje gramatiku bez negenerující neterminálů
                *-2.cfg     obsahuje gramatiku bez zbytečných symbolů

            nts_cnt         Počet (int) neterminálů generované gramatiky.

            s_cnt           Počet (int) terminálů generované gramatiky.

            rels_per_nt     Počet (float) pravidel pro jeden neterminál.

            max_right_side  Maximální počet (float) terminálů a neterminálů vyskytujících
                            se na pravé straně jednoto pravidla. Konkrétní počet
                            symbolů pravé strany je určen rovnoměrným rozdělením <1,max>.

            epsilon_prog    Pravděpodobnost (0,1), že pravidlo bude epsilon pravidlem.

            output          Prefix jména výstupních gramatik.

    testing.sh
    ----------
        ./testing.sh <test_folder> <program>

            Otestuje všechny gramatiky a jejich verze ze složky test_folder vůči programu
            a porovná výstupy od očekávaných. Nakonec vypíše počet úspěšných a selhaných testů.
