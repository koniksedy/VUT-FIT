CC=mpic++
MPIFLAGS=--prefix /usr/local/share/OpenMPI

.PHONY: clean run test numbers

all: parsplit

parsplit: parsplit.cc
	$(CC) $(MPIFLAGS) $^ -o $@

numbers:
	dd if=/dev/random bs=1 count=$(NUM) of=numbers

run: parsplit
	mpirun -np $(PROC) ./parsplit

test: test.sh
	./test.sh

clean:
	rm -f parsplit oems numbers out.txt
