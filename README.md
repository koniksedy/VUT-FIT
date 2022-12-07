# Porovnání algoritmů hledání cyklů v grafech

Tato práce byla vytvořena v rámci projektu č. 13 do předmětu Grafové Algoritmy (GAL) na Fakultě Informačních Technologií VUT. Cílem projektu je porovnat čtyři algoritmy pro výčet všech cyklů v orientovaném grafu. Je porovnáván [algoritmus implementovaný knihovnou Networkx](https://epubs.siam.org/doi/10.1137/0204007), [brute-force algoritmus](https://www.edutechlearners.com/download/Graphtheory.pdf#page=302), [algoritmus Hongbo Liu a Jiaxin Wang](https://ieeexplore.ieee.org/document/1602189) a [Herbert Weinblattův algoritmus](https://doi.org/10.1145/321679.321684).

### Autoři
- Bc. Jan Bíl <xbilja00@stud.fit.vutbr.cz>
- Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>

## Obsah
- [Struktura projektu](#struktura-projektu)
- [Instalace závislostí](#instalace-závislostí)
- [Spuštění programu](#spuštění-programu)
- [Formát vstupu](#formát-vstupu)
- [Formát výstupu](#formát-výstupu)
- [Automatické testy](#automatické-testy)

## Struktura projektu

Na kořenové úrovni projektu se nachází tři složky. Složka `/src` obsahuje modul `gal` a spustitelný program `enum_cycles.py`. Příklady grafů podporovaných programem se nachází ve složce `/graphs`. Tato dokumentace je umístěná ve složce `/doc`. Na kořenové úrovni projektu se také nachází soubor `requirements.txt` obsahující využívané knihovny a jejich verze. Skript s automatickými testy pro modul `gal` je umístěn ve složce `/tests`.

## Instalace závislostí

Potřebné knihovny využívané programem lze instalovat pomocí nástroje pip3 do virtuálního prostředí `gal-env` spuštěním skriptu `build.sh`. Spuštění virtuálního prostředí lze provést příkazem `source ./gal-env/bin/activate` v kořenové úrovni projektu.

## Spuštění programu

Při spuštění programu lze přepínači zvolit algoritmus, který bude použit pro výčet všech cyklů v zadaném grafu. Lze vybírat mezi algoritmem z knihovny Networkx, brute-force algoritmem, algoritmem Hongbo Liu a Jiaxin Wanga, nebo algoritmem Herberta Weinblatta. Musí být zadán právě jeden přepínač určující algoritmus.

Dále lze zpracovávat graf zadaný vstupním souborem, vyheverovaným úplným, multi-cyklickým, nebo nested grafem. Právě jeden graf musí být zadán.

Program `enum_cycles.py` umístěný ve složce `/src` lze po úspěšné instalaci závislostí spustit ve virtuálním prostředí `gal-env` s využitím interpretu python3 (verze 3.8+) následovně:

```
použití: enum_cycles.py [-h] (--nx | --bf | --hj | --wein) [-c N] [-m N M] [-n N] [input]

Poziční argumenty:
  input                 Vstupní soubor s grafem.

přepínače:
  -h, --help            Zobrazí tuto nápovědu.
  --nx                  Použije knihovnu Networkx.
  --bf                  Použije algoritmus brute-force.
  --hj                  Použije algoritmus Hongbo Liu a Jiaxin Wanga.
  --wein                Použije algoritmus Herbert Weinblatta.
  -c N, --complete N    Provede výčet nad úplným grafem s N uzly.
  -m N M, --multicycle N M
                        Provede výčet nad multi-cyklickým grafem s N nodes X hranami, kde X se blíží N+M.
  -n N, --nested N      Provede výčet nad nested grafem s N vrcholy.
```

## Formát vstupu

Vstupní soubor s příponou `.grph` obsahuje orientovaný graf. Každý řádek souboru reprezentuje jeden přechod. Uzel z kterého hrana vystupuje a uzel, do kterého vede jsou na řádku odděleny mezerou. Jména uzlu mohou sestávat z alfanumerických znaků. V tomto formátu jsou brány v úvahu pouze ty uzly grafu, pro které existuje hrana. Podpora pro izolované uzly bez hran není implementována (tyto uzly nikdy nemohou být součástí cyklu).

```
v0 v0
v0 v1
v1 v2
v2 v0
```

## Formát výstupu

Všechny nalezené cykly jsou vypsány na `STDOUT`. Na každém řádku se nachází právě jeden cyklus. Cyklus je reprezentován posloupností vrcholu oddělených mezerou. Poslední uzel cyklu, který je shodný s počátečním uzlem není uváděn (po vzoru knihovny Networkx).

Následující příklad výstupu je výsledkem prohledávání grafu uvedeného v [předchozí sekci](#formát-vstupu).

```
v0
v0 v1 v2
```

## Příklady spuštění

```bash
# Networkx + vstupní soubor
python3 enum_cycles.py --nx g.grph
```

```bash
# Brute-force + úplný graf
python3 enum_cycles.py --bf -c 6
```

```bash
# Hongbo Liu a Jiaxin Wang + multi-cyklický graf
python3 enum_cycles.py --hj -m 10 5
```

```bash
# Herberta Weinblatt + nested graf
python3 enum_cycles.py --bf -n 9
```

## Automatické testy

Při vývoji modulu `gal` byl použit skript `run_tests.py` s automatickými testy, který je umístěn ve složce `/tests`. Testy jsou prováděny na úplných grafech pro $|V| \in \langle 1, 6 \rangle$, multy-cyklických grafech pro kombinace $|V| \in \langle 1, 20 \rangle$ a $|E| \in \langle |V|, |V| + 20 \rangle$ a na nested grafech pro $|V| \in \langle 1, 100 \rangle$. Za referenční výsledky jsou zvoleny cykly, které jsou objeveny pomocí knihovny Networkx.

Za účelem vyžší přehlednosti je během testování tištěna informace o počtu provedených testů pomocí knihovny [Tqdm](https://tqdm.github.io) (verze 4.64.1). Tato knihovna se souběžně instaluje se závislostmi pro program `enum_cycles.py`.
