CC=g++
CFLAGS=-Wall -Wextra -O2
PROJ=huff_code
OBJECTS=huff_codec.o optparser.o coder.o decoder.o model.o

all: $(PROJ)

$(PROJ): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c

clean:
	rm -f $(PROJ) $(OBJECTS)

