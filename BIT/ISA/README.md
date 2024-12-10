# README soubor rozšíření net-snmp agenta.
Projekt: ISA - rozšíření SNMP agenta.

Autor: Michal Šedý <xsedym02>

isaAgent je dynamicky načítatelné rozšíření SNMP agenta, které
bylo vypracováno jakožto projekt do předmětu ISA.


## MIB a její obsah
MIB (ISA-MIB.txt) se registruje pod OID .1.3.6.3.1 s vlastním číslem 22 (isaAgentMib).
V celé MIB je pro skalární objekty použit nulová sub-identifikátor .0

**Obsahuje**:
```
	.1.3.6.1.3.22.1.0 (isaLogin)      - read-only string s loginem "xsedym02"
	.1.3.6.1.3.22.2.0 (isaTime)       - read-only string s aktuálním časem ve formátu RFC 3339.
	.1.3.6.1.3.22.3.0 (isaInteger)    - read-write Int32
	.1.3.6.1.3.22.4   (isaSystemInfo) - read-only objekt seskupující informace o systému viz. schéma
```

**Schéma**:
```
	+--isaAgentMib(22)
	   +-- -R-- String    isaLogin(1)
	   |        Size: 0..255
	   +-- -R-- String    isaTime(2)
	   |        Size: 0..255
	   +-- -RW- Integer32 isaInteger(3)
	   |
	   +--isaSystemInfo(4)
	      |
	      +-- -R-- String    isaSystemName(1)
	      |        Size: 0..255
	      +-- -R-- String    isaSystemRelease(2)
	      |        Size: 0..255
	      +-- -R-- String    isaSystemVersion(3)
	      |        Size: 0..255
	      +-- -R-- String    isaMachineArch(4)
	      |        Size: 0..255
	      +-- -R-- Integer32 isaMemTotal(5)
	      +-- -R-- Integer32 isaMemFree(6)
	      +-- -R-- Integer32 isaSwapTotal(7)
	      +-- -R-- Integer32 isaSwapFree(8)
	      +-- -R-- String    isaCpuModelName(9)
	      |        Size: 0..255
	      +-- -R-- Integer32 isaCpuCores(10)
	      +-- -R-- String    isaAddressSize(11)
	               Size: 0..255
```

## Dynamické načtení
Pro úspěšné nahráni definované MIB (ISA-MIB.txt) je potřeba, aby
se tento soubor nacházel ve složce '~/.snmp/mibs'. Zkopírování MIB
je možné provést příkazem 'make addMib'.

Do snmp agenta se dynamicky načítá modu isaAgent.so, který je
získán jakožto produkt překladu, který je možné spustit příkazem
'make'.

Spuštění SNMPd daemona se provede příkazem 'make snmpd'.
Je vyžadováno oprávnění sudo.

Dynamické načtení binárního souboru isaAgent.so je provedeno příkazem
'make bind'.


## Příklad SET, GET
Před použitím snmpget, nebo snmpset je potřeba mít spuštěného daemona
a načtené rozšíření SNMP agenta.

```
$ snmpget localhost ISA-MIB::isaLogin.0
ISA-MIB::isaLogin.0 = STRING: "xsedym02"
```
```
$ snmpget localhost .1.3.6.1.3.22.2.0
SNMPv2-SMI::experimental.22.2.0 = STRING: "2020-10-30T15:32:32-04:00"
```
```
$ snmpset localhost .1.3.6.1.3.22.3.0 i 42
SNMPv2-SMI::experimental.22.3.0 = INTEGER: 42
```
```
$ snmpget localhost ISA-MIB::isaSystemInfo.isaMemTotal.0
ISA-MIB::isaMemTotal.0 = INTEGER: 8008720 kB
```
```
$ snmpwalk -v1 -c public localhost ISA-MIB::isaSystemInfo
ISA-MIB::isaSystemName.0 = STRING: "Linux"
ISA-MIB::isaSystemRelease.0 = STRING: "3.10.0-1127.19.1.el7.x86_64"
ISA-MIB::isaSystemVersion.0 = STRING: "#1 SMP Tue Aug 25 17:23:54 UTC 2020"
ISA-MIB::isaMachineArch.0 = STRING: "x86_64"
ISA-MIB::isaMemTotal.0 = INTEGER: 8008720 kB
ISA-MIB::isaMemFree.0 = INTEGER: 5610784 kB
ISA-MIB::isaSwapTotal.0 = INTEGER: 839676 kB
ISA-MIB::isaSwapFree.0 = INTEGER: 839676 kB
ISA-MIB::isaCpuModelName.0 = STRING: "Intel(R) Core(TM) i7-2600 CPU @ 3.40GHz"
ISA-MIB::isaCpuCores.0 = INTEGER: 4
ISA-MIB::isaAddressSize.0 = STRING: "36 bits physical, 48 bits virtual"
```

## Využité knihovny
- net-snmp/net-snmp-config.h
- net-snmp/net-snmp-includes.h
- net-snmp/agent/net-snmp-agent-includes.h
- stdio.h
- stdlib.h
- string.h
- stdbool.h
- ctype.h
- time.h
- sys/utsname.h


## Odevzdané soubory
- isaAgent.c     - hlavní část modulu (inicializace, registrace, ...)
- isaAgent.h     - hlavičkový soubor pro modul isaAgent.c
- getData.c      - pomocný modul pro získání informací o systému
- getData.h      - hlavičkový soubor pro modul getData.c
- ISA-MIB.txt    - definice MIB
- Makefile
- README
- manual.pdf

## Hodnocení
17/20
