# UPA projekt

Projekt do předmětu UPA.

Autoři:
* Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
* Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
* Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>


## Zadání 1. části

1) Prozkoumejte způsob [poskytování dat vlakových jízdních řádů veřejné dopravy](https://mdcr.cz/Dokumenty/Verejna-doprava/Jizdni-rady,-kalendare-pro-jizdni-rady,-metodi-%281%29/Jizdni-rady-verejne-dopravy), konkrétně [formát datových souborů](https://portal.cisjr.cz/pub/draha/celostatni/szdc/Popis%20DJ%C5%98_CIS_v1_09.pdf) a [jejich distribuci](https://portal.cisjr.cz/pub/draha/celostatni/szdc/). Analyzujte dílčí poskytnuté datové sady z daného zdroje, jejich strukturu (schéma), typy datových položek, identifikátory, možnosti propojení datových sad (společné entity) či napojení na externí data (jiné zdroje, entity reálného světa), změnu dat v čase (aktualizace), a jiné.


2) Na základě analýzy z předchozího kroku navrhněte a v dokumentaci projektu popište způsob úsporného a rychlého uložení a hledání dat v NoSQL databázi, které splňuje následující požadavky:

*  předpokládané využití databáze je pro aplikaci hledající přímá spojení mezi zadanými stanicemi -- podstatné zejména elementy `CZPTTInformation` a `CZPTTLocation`, které by se uplatnily při hledání tak, jak je uvedeno v popisu aplikace níže;

* ukládají se všechna data ze vstupních datových sad, tedy i ta, kterým zatím nerozumíme, ale později je možná budeme potřebovat -- např. `NetworkSpecificParameters` (je možno je ukládat nerozparsovaná);

* datové položky v úložišti (NoSQL hodnoty) musí být vhodně zvoleny a všechny vhodně identifikovány (NoSQL klíče příslušných hodnot) tak, aby bylo možné potřebné položky optimálně nalézt (na základě klíče) či neplatné položky přepsat bez nutnosti vyprázdnění a opětovného uložení celé databáze -- věnujte pozornost elementům Identifiers a způsobu zušení či dočasného nahrazení spojů elementy `/CZCanceledPTTMessage/PlannedTransportIdentifiers` a `/CZPTTCISMessage/Identifiers/RelatedPlannedTransportIdentifiers`;

* při hledání se využívají pomocné struktury databáze tak, aby bylo nalezení potřebné datové položky rychlé (tedy aplikace se dostane v databázi k hledané datové položce krok za krokem přes několik dílčích upřesňujících dotazů, aniž by se vyžadovalo procházení nepotřebných datových položek, ať už v databázi nebo v klientské aplikaci) -- např. pomocné databáze klíč-hodnota pro klíče s názvy hledaných stanic, hledanými daty odjezdu a příjezdu atp. (např. i v podobě pomocných tabulek wide-column databází či kolekcí dokumentových databází); pomocné indexy či materializované pohledy (u wide-column databází); vazby mezi souvisejícími datovými položkami (u grafových databází); pomocné časové řady záznamů odjezdů a příjezdů (u databází časových řad) a jiné.


3) Na základě návrhu zvolte vhodnou NoSQL databázi a tuto dle návrhu zprovozněte a připravte pro uložení a hledání datových položek dle zadání (pokud nechcete instalovat databáze, lze jako základ využít obraz virtuálního stroje s databázemi ze cvičení).

* získat z distribučního portálu příslušné datové soubory, tyto případně předzpracovat, a poté nahrát do NoSQL databáze připravené v předchozích krocích vč. konstrukce a nahrání obsahu pomocných datových struktur a aniž by musel být obsah databáze předtím smazán (nahráváme tedy dílčí data pomocí vkládání či aktualizace vhodně identifikovaných jednotlivých záznamů, tzv. UPSERT, nebo záznamy mažeme, což bude zejména v případě pomocných struktur a dat);

* vyhledávat spojení z pojmenovaných stanic `odkud` a `kam` v zadané `datum_a_cas` dle následujícího algoritmu, který pracuje nad původními zdrojovými XML daty (bude potřeba upravit na použitou NoSQL databázi): 

    * najdeme platné datové položky (tj. nenahrazené či nezrušené elementy `CZCanceledPTTMessage` a `RelatedPlannedTransportIdentifiers`, vizte výše), které mají neprázdný průnik uzavřeného intervalu `/CZPTTCISMessage/CZPTTInformation/PlannedCalendar/ValidityPeriod` se zadaným `datum_a_cas`, přičemž bereme v potaz pouze dny daného intervalu (ignorujeme časovou hodnout) vybrané bitovou maskou `/CZPTTCISMessage/CZPTTInformation/PlannedCalendar/BitmapDays` (vlak jede v daný den),

    * a zároveň mají u některého elementu `/CZPTTCISMessage/CZPTTInformation/CZPTTLocation` hodnotu jeho podelementu `Location/PrimaryLocationName` shodnou s odkud a zároveň tam existuje i podelement `TrainActivity/TrainActivityType` s hodnotu `0001` (vlak zastavuje ve výchozí stanici),

    * a zároveň mají u některého elementu `/CZPTTCISMessage/CZPTTInformation/CZPTTLocation`, který následuje po elementu s nalezenou hodnotou odkud z předchozího kroku, hodnotu jeho podelementu `Location/PrimaryLocationName` shodnou s kam a zároveň tam existuje i podelement `TrainActivity/TrainActivityType` s hodnotu `0001` (vlak zastavuje v cílové stanici),

    * a vypíšeme pro výše nalezené datové položky celou trasu vlaku s časovými údaji, tj. posloupnost elementů `/CZPTTCISMessage/CZPTTInformation/CZPTTLocation` s jejich hodnotami podelementů `TimingAtLocation/Timing` a `Location/PrimaryLocationName`.


5) Implementovanou aplikaci otestujte postupným nahrávání zdrojových souborů a dotazování dle zadání výše. Změřte a zdokumentujte časovou náročnost načtení dostupných zdrojových souborů z celého období prosinec 2021 až listopad 2022 včetně a časovou náročnost vyhledání spoje mezi stanicemi v daný den a čas.


6) V dokumentaci projektu popište:

* postup a výsledky analýzy a návrhu NoSQL včetně výchozích informací a úvah vedoucích k návrhovým rozhodnutím;
* typ zvolené NoSQL databáze se zdůvodnění na základě provedené analýzy a strukturu navržené NoSQL databáze včetně pomocných struktur a dat zvolenou na základě návrhu;
* způsob použití implementované aplikace (či aplikací) k zadaným úlohám (nahrání dat do prázdné i již naplněné databáze a vyhledávání spoje dle zadání);
* průběh vykonávání zadaných úloh v aplikaci (kdy se co a jak dotazuje);
* způsob otestování a výsledné časy k testovacím úlohám.
