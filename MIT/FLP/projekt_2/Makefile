# Makefile for FLP Project 2.
# Procect: Turing-machine (FLP)
# Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
# Last change: 24.04.2022

PL=swipl
FLAGS=-q
OUTPUT=flp21-log
DEP=src/*.pl
MAIN=main
TESTS=test/auto/*.in test/auto/*.out test/auto/*.diff test/manual/accept/*.diff test/manual/accept/*.out
ZIP=flp-log-xsedym02.zip
MAXPROC=16   # Number of parallel generators/tests. High number can cause fork error.
TIMEOUT=5    # Prolog timeout (sec).

.PHONY: test

all: $(OUTPUT)

$(OUTPUT): $(DEP)
	$(PL) $(FLAGS) -g $(MAIN) -o $@ -c $^

zip:
	zip -r $(ZIP) *

test: tests-generate $(OUTPUT)
	$(MAKE) tests-quick

tests-quick:
	@echo "TESTING auto"
	cd test/utils; ./testing.sh ../auto ../../$(OUTPUT) $(MAXPROC) $(TIMEOUT)
	@echo "TESTING manual/accept"
	cd test/utils; ./testing.sh ../manual/accept ../../$(OUTPUT) 1 2

tests-generate: tests-clean
	cd test/utils; ./testsGen.sh ../auto $(MAXPROC)

tests-clean:
	rm -f $(TESTS)

clean:
	rm -f $(OUTPUT) $(ZIP) 
