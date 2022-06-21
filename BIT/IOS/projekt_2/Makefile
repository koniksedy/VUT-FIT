# Makefile
# Autor: Michal Šedý <xsedym02>, FIT
# Řešení: IOS - projekt 2
# Datum: 22.4.2019
# Přikazy: make - vytvoří spustitelnou verzi programu
#		   clean - smaže veškeré soubory vytvořené přikazem make

CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic
LDFLAGS=-lrt -pthread

all: proj2

proj2: proj2.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm proj2
