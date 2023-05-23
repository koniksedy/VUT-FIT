# Makefile
# A Makefil for a KKO project (Huffman coder/decoder)
# Author: Micha Šedý <xsedym02@vutbr.cz>
# Last change: 04.05.2023

CC=g++
CFLAGS=-Wall -Wextra -O2
PROJ=huff_codec
OBJECTS=bitdata.o coder.o decoder.o huff_codec.o huffman.o linearizer.o model.o optparser.o splitter.o tree.o

all: $(PROJ)

$(PROJ): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c

clean:
	rm -f $(PROJ) $(OBJECTS) numbers.raw numbers.out
