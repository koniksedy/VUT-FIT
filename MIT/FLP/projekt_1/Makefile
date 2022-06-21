# Makefile for FLA Project 1.
# Procect: SIMPLIFY-BKG (FLP)
# Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
# Last change: 24.03.2022

OUT = flp21-fun
FLAGS = -Wall
SRC = src
DEP= src/*.hs
ZIP = flp-fun-xsedym02.zip

.PHONY: test


$(OUT): $(DEP)
	ghc $(FLAGS) $(SRC)/*.hs -o $@

# Remove build files. Will not clean the test folder.
clean:
	rm -f $(OUT)
	rm -f $(SRC)/*.hi $(SRC)/*.o
	rm -f $(ZIP)

# Generate and run tests.
test: $(OUT) tests-generate
	./test/utils/testing.sh test $(OUT)

# Run allready generated.
test-quick: $(OUT)
	./test/utils/testing.sh test $(OUT)

# Gemerate tests.
tests-generate:
	cd test/utils; ./testsGen.sh ..

# Remove tests.
tests-clean:
	rm -f test/*.cfg test/*.out

# Zip project.
zip:
	zip -r $(ZIP) *

