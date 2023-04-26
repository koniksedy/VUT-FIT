CC=g++
CFLAGS=-Wall -Wextra -g
PROJ=huff_code
OBJECTS=bitdata.o coder.o decoder.o huff_codec.o huffman.o linearizer.o model.o optparser.o spliter.o tree.o

all: $(PROJ)

$(PROJ): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c

clean:
	rm -f $(PROJ) $(OBJECTS)

