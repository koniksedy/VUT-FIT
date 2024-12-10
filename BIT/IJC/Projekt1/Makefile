# Makefile
# Řešení IJC-DU1, příklad a), b), 16.03.2019
# Autor: Michal Šedý, FIT
# Popis: Příkaz make vytvoří všechny spustitelné soubory pro DU1.
#		 Příkaz make run spustí všechny varianky programů.
#		 Příkaz make clena smaže všechny spustitelné verze a objektové soubory.

CC=gcc
CFLAGS=-O2 -std=c11 -Wall -pedantic
TARGETS=primes primes-i steg-decode

all: $(TARGETS)

# Primes - využití maker
primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) $^ -lm -o $@

primes.o: primes.c eratosthenes.h bit_array.h error.h
	$(CC) $(CFLAGS) $< -c

eratosthenes.o: eratosthenes.c eratosthenes.h bit_array.h error.h
	$(CC) $(CFLAGS) $< -c

# Primes-i - využití inline funkcí
primes-i: primes-i.o eratosthenes-i.o error.o
	$(CC) $(CFLAGS) $^ -lm -o $@

primes-i.o: primes.c eratosthenes.h bit_array.h error.h
	$(CC) $(CFLAGS) -DUSE_INLINE $< -c -o $@

eratosthenes-i.o: eratosthenes.c eratosthenes.h bit_array.h error.h
	$(CC) $(CFLAGS) -DUSE_INLINE $< -c -o $@

# Steg-decode 
steg-decode: steg-decode.o ppm.o eratosthenes.o error.o
	$(CC) $(CFLAGS) $^ -lm -o $@

steg-decode.o: steg-decode.c ppm.h eratosthenes.h error.h
	$(CC) $(CFLAGS) $< -c

ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) $< -c

error.o: error.c error.h
	$(CC) $(CFLAGS) $< -c

# Příkazy
run: primes primes-i steg-decode
	time ./primes
	time ./primes-i
	./steg-decode du1-obrazek.ppm

clean:
	rm $(TARGETS) *.o 
