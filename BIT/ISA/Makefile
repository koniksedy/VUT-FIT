# Makefile pro dynamicky načítatelný modul isaAgent
# ---------------------------
# Autor: Michal Šedý <xsedym02>
# Projekt: ISA - rozšíření SNMP agenta.
# Autor: Michal Šedý <xsedym02>
# Datum úpravy: 30.10.2020
# ---------------------------
# make - sestaví projekt
# make snmpd - spustí snmpd
# make bind - dynamicky nahraje rozšíření
# make addMib - přidá MIB tabulku do složky ~/.snmp/mibs
# make clean - odstraní soubory vzniklé během překladu

CC=gcc
CFLAGS=-std=c99 -I. `net-snmp-config --cflags`
DLFLAGS=-fPIC -shared
VD=${CURDIR}

all: isaAgent.so

isaAgent.so: isaAgent.o getData.o	
	$(CC) $(CFLAGS) $(DLFLAGS) -o $@ $^

isaAgent.o: isaAgent.c
	$(CC) $(CFLAGS) $(DLFLAGS) -c $<

getData.o: getData.c
	$(CC) $(CFLAGS) $(DLFLAGS) -c $<

addMib:
	cp ISA-MIB.txt ~/.snmp/mibs/

bind: isaAgent.so
	snmpset localhost UCD-DLMOD-MIB::dlmodStatus.1 i create
	snmptable localhost UCD-DLMOD-MIB::dlmodTable 
	snmpset localhost UCD-DLMOD-MIB::dlmodName.1 s "isaAgent" UCD-DLMOD-MIB::dlmodPath.1 s "$(VD)/isaAgent.so" 
	snmptable localhost UCD-DLMOD-MIB::dlmodTable
	snmpset localhost UCD-DLMOD-MIB::dlmodStatus.1 i load

snmpd:
	sudo snmpd -f -L -DnstAgentPluginObject,dlmod

clean:
	rm isaAgent.so isaAgent.o getData.o