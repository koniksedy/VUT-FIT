# Simulátor jízdy autobusů

Aplikace ja napsána v jazyce Java verze 8.

## Překlad
Před přeložením musí být staženy potřebné závislosti pomocí skriptu get-libs.sh umístěného v lib/.
Pro automatizaci překladu byl vytvořen soubor build.xml pro sestavovací program ANT.
Překlad a sestavení aplikace se provede zadáním příkazu: ant compile
Do složky build/ se vygenerují pomocné struktůry a .class soubory.
Výsledná spustitelný aplikace je z dispozici ve složce dest/.
Soubory vzniklé překladem se možné odstranit příkazem: ant clean.

## Spuštění
Aplikace se spoučtí pomocí programu ANT příkazem: ant run.
V případě manuálního spuštění aplikace (kliknutí na ikonu) již nebudou fungonat cesty k testovacím
souborům (reprezentujících mapu), tyto soubory jsou umístěny ve složse data/. Počítá se se spuštěním
pomocí programu ANT z kořenového adresáře projektu.

## Dokumentace
Dokumentace vygenerována nástrojem javadoc je dostupná ve složce doc/.

## Závislosti
Ke stažení pořtebných knihovem slouží skript get-libs.sh umístěný ve složce lib/.
Externí knihovny pro serializaci a deserializaci využívané aplikaci jsou po stažení uloženy v adresáři lib/.
Jedná se o následující knihovny:
    - jackson-annotations-2.10.3
    - jackson-core-2.10.3
    - jackson-databind-2.10.3
    - jackson-dataformat-yaml-2.10.3
    - jackson-datatype-jdk8-2.10.3
    - snakeyaml-1.26

## Zdrojové soubory
Soubory obsahující zdrojové kódy se nacházejí v adresáři scr/ija/.
Soubory .fxml definující grafický vzhled aplikace jsou umístěny ve složce src/resource/.

## Spolupracovali
- Michal Šedý
- Ondřej Pavlacký

## Hodnocení
Hodnocení: 80/80

