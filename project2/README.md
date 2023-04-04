# Implementace paralelního algoritmu K-MEANS

Pomocí knihovny Open MPI implementujte v jazyce C++ algoritmus K-Means pracující se skalárními (jednorozměrnými) hodnotami. Algoritmus paralelizujte a navrhněte tak, aby měl co nejmenší časovou složitost. Pro posloupnost N hodnot budete mít k dispozici N procesorů. Dále předpokládajte, že algoritmus vytváří 4 shluky (tedy konkrétně se bude jednat o 4-means) a vstupní data budou obsahovat minimálně 4 a maximálně 32 hodnot. V dokumentaci představte vaše řešení a analyticky uveďte jeho časovou složitost.

**Obsah**
- [Hodnocení](#hodnocení)
- [Algoritmus](#algoritmus)
- [Implementace](#implementace)
- [Vstup](#vstup)
- [Výstup](#výstup)
- [Další informace](#další-informace-poznámky)
- [Dokumentace](#dokumentace)

## Hodnocení
TBD

## Algoritmus
```
1) Zvolte 4 středy (průmery) shluků 1 – 4
2) Jednotlivé prvky přiřaďte do shluku, který reprezentuje střed nejbližší danému prvku. Vzdálenost od středu počítejte jak absolutní hodnotu rozdílu hodnoty prvku a hodnoty středu.
3) Z přiřazených hodnot jednotlivym shlukům spočítejte nový střed jako průměr těchto hodnot. Pokud k některému ze zhluků nebyl přiřazen žádný prvek, jeho střed zůstává stejný.
4) Pokud se hodnota středu některého ze shluků změnila, vraťte se na bod 2.
5) Vypište vhodným způsobem jednotlivé shluky na obrazovku, tento krok do analýzy časové složitosti neuvažujte
```

## Implementace

K dispozici máte:
- C++ (přeložitelné pomocí mpic++ kvůli jednotnému skriptu test.sh, ne nutně objektově)
- Open MPI
- nic jiného

Název souboru nyní bude parkmaens.c / parkmeans.cc

## Vstup

Vstupem je posloupnost čtyř až dvaceti dvou náhodných čísel uložených v souboru. Vytvořte skript, který bude obdobný tomu, který jste vytvářeli v prtním projektu

## Výstup

Jako výstup proběhne vytištění jednotlových shluků vhodným způsobem, například:
 ```
[0.33]  -1, 0, 2
[5.5] 5, 6
[10] 8, 10, 12
[15] 15
```

## Další informace, poznámky

Počáteční hodnoty středů shluků volte z prvních čtyřech prvků posloupnosti.

Redukovat v MPI můžete i pole hodnot, dejme tomu o čtyřech prvcích, to by mohlo být užitečné.

Pokud po spuštění počet procesů N se nerovná velikosti vstupní posloupnosti, pak v případě že je na vstupu hodnot vice, pracujte s prvními N hodnotami. Pokud je hodnot méně, výpočet neprovádějte a nechte aplikaci toto oznámit na obrazovku.



## Dokumentace

Součástí řešení je dokumentace, která bude o rozsahu maximálně 3 strany (rozumné a odůvodněné překročení limitu stran není důvod k bodové srážce) funkčního textu.

### Do dokumentace nedávejte:

Úvodní stranu, obsah, popis zadání.

### V dokumentaci popište:

Rozbor a analýzu algoritmu, odvoďte jeho teoretickou složitost (časovou a prostorovou složitost, celkovou cenu). Uvedené vzorce slovně popište, včetně jejich proměnných.
Komunikační protokol, jak si "procesy" zasílají zprávy. Pro vizualizaci použijte sekvenční diagram (http://en.wikipedia.org/wiki/Sequence_diagram). Nezapomeňte, že protokol musí být obecný, tedy pro nprocesů.
Závěr, ve kterém zhodnotíte dosažené výsledky.
