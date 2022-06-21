# TCP a UDP skener síťových portů

## O programu
Program ipk-scan je jednoduchý TCP a UDP skener síťových portů napsaný
v jazyce C standardu gnu11. Umožňuje oskenovat zadané porty zadané IP
adresy pomocí zvolené metody.


## Sestavení
Pro sestavení skeneru portů jsou zapotřebí následující soubory:

    - arg_parser.c      // zpracování parametrů spuštění
    - arg_parser.h
    - interface_ip.c    // nažte interface a jeho IP adresu
    - interface_ip.h    
    - port_queue.c      // implementace FIFO
    - port_queue.h
    - sniffer.c         // vytvoří ACK-SYN/RST/ICMP3 sniffer
    - sniffer.h
    - tcp_scanner.c     // TCP skenování portů
    - tcp_scanner.h
    - udp_scanner.c     // UDP skenování portů
    - udp_scanner.h
    - error.h           // chybové kódy, nápověda
    - ipk-scan.c        // hlavní program řídicí skenování
    - ipk-scan.h

K úspěšné kompilaci musí být nainstalovaná knihovna lpcap.Program dále
využívá vlákna, tudíž je potřebná také knihlvna lpthread. Program je napsán
ve standartu gnu11. Sestavení provedete příkazem make (nutný Makefile).


## Spuštění
Pro spušnění je potřeba mít root oprávnění.
```
./ipk-scan {-i <interface>} --pu <port-ranges> --pt <port-ranges> {<option>} [<domain-name> | <IP-address>] 
    -i          Přepínač umožňující manuální nastavení interface, kde argument
                představuje identifikátor tohoto rozhraní. Pokud není přepínač
                použit, je systémem vybráno první neloopbackové interface.
                Pokud je scanovaný localhost, je standartně vybráno loopbackové 
                interface.

    -u, --pu    Je možno použít zkrácenou variantu -u. Argumentem jsou skenované
                UDP porty. Rezervonavý port 0 je povoleno skenovat. Rozsah
                skenovaných portů může být zadán například následovně: 1,5,9,7
                nebo 1-3600 nebo 1-10,22,36-75. Hranice intervalů jsou odněleny
                pomlčkou. Jednotlivá čísla a intervaly jsou odděleny čárkou.
                Nepoužívají se mezery. Za posledním číslem se čárka neuvádí.

    -t, --pt    Je možno použít zkrácenou variantu -t. Argumentem jsou skenované
                TCP porty. Rezervonavý port 0 je povoleno skenovat. Rozsah
                skenovaných portů může být zadán například následovně: 1,5,9,7
                nebo 1-3600 nebo 1-10,22,36-75. Hranice intervalů jsou odněleny
                pomlčkou. Jednotlivá čísla a intervaly jsou odděleny čárkou.
                Nepoužívají se mezery. Za posledním číslem se čárka neuvádí.

    --only-open 
                Ve výsledcích budou zobrazeny pouze otevřené porty. Nelze kombinovat
                s ostatními přepínači filtrujícími výsledky.

    --only-closed
                Ve výsledcích budou zobrazeny pouze zavřené porty. Nelze kombinovat
                s ostatními přepínači filtujícími výsledky.

    --only-unclosed
                Ve výsledcích budou zobrazeny pouze otevřené a filtrované porty.
                Nelze kombinovat s ostatními přepínači filtujícími výsledky.

    -h, --help  Zobrazení nápovědy programu ips-scan.

    Pořadí přepínačů lze zaměňovat, ale skenovaná adresa/jméno domény musí být 
    vždy jako posledním argumentem spuštění. Vždy musí být zadán alespoň 
    jeden port pro skenování.
```

## Příklad spuštění
Spouštěno s oprávněním root.

1) TCP skenování portů 1,5,6,7,8,9,10 na localhost, vypíšou se puze otevřené porty.
    ./ipk-scan --pt 1,5-10 --only-open localhost

2) TCP a UDP skenování všech portů scanme.nmap.org bez omezení na výpis výsledků na interface wlp4s0.
    ./ipk-scan -i wlp4s0 -t 1-65535 -u 1-65535 scanme.nmap.org


## TCP skenování
TCP skenování nevyúžívá 3-way-handshake, ale pouze odešle SYN packet, na skenovaný
port a pokud příjme ACK odpověď, je port označen jako otevřený, jestliže příjme
RST odpověď, je port označen jako zavřený. Pokud z portu nepříjde odpověď,
je na něj znovu odeslán dotaz, a pokud nepříjde ani podruhé v daném čase odpověď,
je port označen jako filtrovaný.


## UDP skenování
UDP skenování odešle na skenovaný port UDP packet, a pokud příjde zpráva ICMP typu 3
(port unreachable) je port označen jako uzavřený, v opačném případě je port otevřený.


## IPv4 a IPv6
ipk-scan skenuje porty na zadané adrese, a to jak IPv4, tak IPv6.
Při výběru interface je zvoleno to, které umožňuje komunikovat
pomocí dané verze internetového protocolu. Pokud takové neexistuje,
nebo uživatel explicitně vybral interface, které nepodporuje
danou verzi bude vypsána chybové hlášní a program se ukončí s chybou.
Skenner IPv6 byl testován pouze na localhostu.


## Návratové kódy
0  - úspěšné provedení
1  - chyba argumentů spuštění
2  - chyba vyhledávání interface
3  - chyba skenované ip/jména (nenalezeno, špatný formát, ...)
4  - chyba během vytváření sniffru
5  - chyba vytvoření vlákna programu
6  - chyba socketu
99 - vnitřní chyba (problém s alokací paměti, ...)

## Hodnocení
Hodnocení: 20/20
