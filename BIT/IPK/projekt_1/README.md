# HTTP resolver doménových jmen  


Program napsán v jazyce python3.6 implementuje server, který zajišťuje překlad doménových jmen prostřednictvím HTTP komunikace.

## Podporované operace
### Get
Provádí překlad jednoto dotazu, který je obsažen v URL požadavku zaslaném serveru.

tvar požadavku: GET /resolve?name=seznam.cz&type=A HTTP/1.1
- name = doménové jméno, nebo IP adresa
- type = typ odpovědi A/PTR

tvar odpovědi: DOTAZ:TYP=OPDOVED
 - např: www.seznam.cz=77.75.75.172

Pokud je odpověď nalezena, je navrácen výsledek *200 OK*. Pokud není nalezena, vrátí *404 Not Found*. Ostatní výsledky jsou uvedeny v sekci Chyby.
### POST
Provádí překlad více dotazů, které jsou obsaženy v těle požadavku zaslaného na server. Každý dotaz je na vlastním řádku.

tvar požadavku: POST /dns-query HTTP/1.1  
formát řádku s dotazem: DOTAZ:TYP  
- DOTAZ - doménové jméno, nebo IP adresa
- TYP - typ odpovědi A/PTR  
- např: www.seznam.cz:A

tvar odpovědi: DOTAZ:TYP=OPDOVED
 - např: www.seznam.cz:A=77.75.75.172

Pokud bude překlad nalezen, zahrne se do odpovědi, v opačném případě nebude v odpovědi dotaz a jeho odpověď uvedena. Navrácený výsledek bude *200 OK*. Vyjímku tvoří chybné případy uvedené v sekci Chyby.
### Chyby
- nesprávné URL (jiné než /resolve, nebo /dns-query) - *400 Bad Request*.
- nesprávné parametry GET -  *400 Bad Request*.
- nesprávný formát POST - *400 Bad Request*.
- nenalezení odpovědi na žádný dotaz POST - *404 Not Found*.
- nepodporovaná operace (jiná než GET a POST) - *405 Method Not Allowed*.

## Spuštění
Pro spuštění serveru byl vytvořen Makefile. Server se spustí příkazem *make run* s volitelným parametrem *PORT=port_n*, který specifikuje post, na kterém se server spustí. Poku parametr *PORT* není uveden je vybrán port 8888. Server se ukončí přijením signálu SIGINT.

## Implementace
Po spuštění se vytvoří socket, který se pokusí připojit lokálně na port určený při spuštění. Pokud není určen, je použit 8888. Poté je socket nastaven na příjem dat. V nekonečném cyklu čeká na požadavky různých klientů. Klienty obsluhuje postupně. Přijatý požadavek zkontroluje na jeho správnou formu. Pokud není požadavek GET, nebo POST, je vyvolána chyba a jako odpověď navráceno *405 Method Not Alowed*. V případě detekce chybného formátu požadavku u operace GET je navráceno *400 Bad Request*. Pokud bude v operaci POST chybný požadavek, bude přeskočen. Jakmile získá odpověď pro jednoho klienta, zašle mu ji zpět. Odpojí se od něj a naváže spojení s novým klientem. Server se ukončí přijetím signálu SIGINT (cltr+C).

## Hodnocení
18/20

