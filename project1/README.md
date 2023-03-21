## Implementace algoritmu PARALLEL SPLITTING

**Obsah**
- [Hodnocení](#hodnocení)
- [Zadání](#zadání)
    - [Vstup](#vstup)
        - [Soubor numbers](#soubor-numbers)
    - [Překlad a spuštění](#skript-pro-překlad-a-spuštění)
    - [Výstup](#výstup)
    - [Postup](#postup)
    
## Hodnocení

TBD

## Zadání 

Pomocí knihovny Open MPI implementujte v jazyce C++ úlohu Přiřazení pořadí preorder vrcholům tak, jak byla uvedena na přednášce PRL. Pouze v těch částech, kdy algoritmus předpokládá využití sdílené paměti, použijte vhodně prostředky MPI (vizte níže v textu)

### Vstup

Vstupem je posloupnost osmi náhodných čísel uložených v souboru.

#### Soubor numbers

Soubor numbers obsahující čísla velikosti 1 byte, která jdou bez mezery za sebou. Pro příklad vytvoření tohoto souboru uvádíme kód níže. ve kterém je ukázáno vytvoření takovéto posloupnosti náhodných čísel a její uložení do souboru pomocí utility dd. V případě tohoto projektu předpokládejte rozsah "numbers" od 8 do 64. Tato utilita generuje náhodná čísla v rozsahu určeném velikostí bloku. Při bloku 1B jsou hodnoty v rozsahu 0-255. Vygenerovaná čísla jsou pak přesměrována do souboru. Vznikne tedy další soubor s náhodnými znaky jdoucími bez mezery za sebou. Po otevření v libovolném textovém editoru se hodnoty tváří jako náhodné ascii znaky, které by však měly být chápany jako celá čísla. Soubor je v tomto případě chápan jako binární.

### Skript pro překlad a spuštění

V ukázkovém souboru je předpokládaný název souboru parsplit, který prosím použijte pro vaše řešení (tedy odevzdáte kód parsplit.c)

```bash
#!/bin/bash

#pocet cisel bud zadam nebo 10 :)
if [ $# -lt 1 ];then 
    numbers=10;
else
    numbers=$1;
fi;

#preklad cpp zdrojaku
mpic++ --prefix /usr/local/share/OpenMPI -o parsplit parsplit.c


#vyrobeni souboru s random cisly
dd if=/dev/random bs=1 count=$numbers of=numbers

#spusteni
mpirun --prefix /usr/local/share/OpenMPI -np $numbers parsplit

#uklid
rm -f oems numbers
```

### Výstup

Výsledné rozdělení vstupní posloupnosti bude vytištěno na obrazovku terminálu.

### Postup

Kořenový root proces (číslo 0 v hlavním komunikátoru) načte všechna čísla ze souboru. Dále vybere medián v polovině posloupnosti (v případě sudého počtu prvků bude volen blíže počátku posloupnosti), rozešle jej ostatním procesům a také jim přidělí jejich část posloupnosti. Rozdělení na části je rovnoměrné. Předpokládejte, že počet prvků v posloupnosti je vždy dělitelný beze zbytku počtem procesů, které jsou spuštěny. K rozeslání použijte kolektivní operaci MPI_Scatter. 

Po provedení patřičné části práce procesy použitím kolektivní operace MPI_Gatherv odevzdají výsledky kořenovému procesu, který na závěr celkový výsledek vytiskne na obrazovku.

Tedy pro sestavení výsledků není možné použít sdílenou paměť. Namísto toho použijte, jak již bylo uvedeno, kolektivní operac MPI_Gatherv. Pro pochopení správné implementace doporučuji prostudovat příklad z následujícího odkazu: https://stackoverflow.com/questions/31890523/how-to-use-mpi-gatherv-for-collecting-strings-of-diiferent-length-from-different

Poznámka: pro spuštění aplikace s více než dvanácti procesy je nutné použít následující:

`mpirun -oversubscribe -np 16 hello`