CC=mpic++
PREFIX=--prefix /usr/local/share/OpenMPI
PROC=2
NUM=8

.PHONY: clen run numbers test

all: parkmeans

parkmeans: parkmeans.cc
	@$(CC) $(PREFIX) $^ -o $@

numbers:
	@./numbersGen.sh $(NUM)

run: parkmeans
	@mpirun -np $(PROC) ./parkmeans

test: test.sh
	./test.sh

clean:
	@rm -f parkmeans oems numbers

