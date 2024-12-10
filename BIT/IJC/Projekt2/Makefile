# Makefile
# Autor: Michal Šedý, FIT
# Řešení: IJC-DU2 16.4.2019
# Poznámka: Makefile sestaví všechny spustitelné verve programů a knihovny.
# Popis: make vytvoří včechny spustitelní verze programů a knihovny
#		 clean smaže soubory nzniklé při překladu
CC=gcc
CXX=g++
AR=ar
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic 
CFLAGS=-std=c99 -Wall -pedantic 
MDLSOURCES=htab_hash_function.c htab_init.c htab_size.c htab_bucket_count.c htab_lookup_add.c htab_begin.c htab_end.c htab_iterator_next.c htab_iterator_get_key.c htab_iterator_get_value.c htab_iterator_set_value.c htab_clear.c htab_free.c	htab_move.c htab_extern_inline.c
MDLOBJECTS=$(MDLSOURCES:.c=.o)

all:tail tail2 wordcount wordcount-dynamic

tail:tail.c
	$(CC) $(CFLAGS) $^ -o $@

tail2:tail2.cc
	$(CXX) $(CXXFLAGS) $^ -o $@

wordcount-dynamic: wordcount.o io.o libhtab.so
	$(CC) $(CFLAGS) -fPIC $^ -o $@ -Wl,-rpath=.

wordcount: wordcount.o io.o libhtab.a
	$(CC) $(CFLAGS) $^ -o $@

wordcount.o:wordcount.c io.h htab.h
	$(CC) $(CFLAGS) -c $< -o $@

io.o: io.c io.h htab.h privatedata.h
	$(CC) $(CFLAGS) -c $<

libhtab.a: $(MDLOBJECTS)
	$(AR) rcs $@ $(MDLOBJECTS)

libhtab.so: $(MDLOBJECTS)
	$(CC) -shared -fPIC $(MDLOBJECTS) -o $@ 

.c.o: htab.h privatedata.h
	$(CC) $(CFLAGS) -fPIC -c $<

clean:
	rm tail tail2 wordcount wordcount-dynamic libhtab.a libhtab.so *.o
