# Makefile pro projekt 2 do předmětu IPK (skenner portů).
# Autor: Michal Šedý <xsedym02>
# verze gcc: 7.5.0
# Poslední úprava: 27.4.2020
##########################################################
# make        - sestaví projekt pod jménem ipk-scan.
# make clean  - vyčistí složku od sestavených souborů.
##########################################################
CC=gcc
CFLAGS=-std=gnu11
CLIBS=-lpcap -lpthread
OBJ=arg_parser.o interface_ip.o ipk-scan.o port_queue.o sniffer.o tcp_scanner.o udp_scanner.o

all: ipk-scan

ipk-scan: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(CLIBS)

arg_parser.o: arg_parser.c arg_parser.h error.h
	$(CC) $(CFLAGS) $< -c

interface_ip.o: interface_ip.c interface_ip.h error.h
	$(CC) $(CFLAGS) $< -c

port_queue.o: port_queue.c port_queue.h 
	$(CC) $(CFLAGS) $< -c

sniffer.o: sniffer.c sniffer.h
	$(CC) $(CFLAGS) $< -c -lpcap

tcp_scanner.o: tcp_scanner.c tcp_scanner.h error.h
	$(CC) $(CFLAGS) $< -c $(CLIBS)

udp_scanner.o: udp_scanner.c udp_scanner.h error.h
	$(CC) $(CFLAGS) $< -c $(CLIBS)

ipk-scan.o: ipk-scan.c ipk-scan.h error.h
	$(CC) $(CFLAGS) $< -c

clean:
	rm *.o ipk-scan