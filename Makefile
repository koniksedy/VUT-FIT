# Makefile
# A Makefil for a KKO project (Huffman coder/decoder)
# Author: Micha Šedý <xsedym02@vutbr.cz>
# Last change: 04.05.2023

# USAGE:
# make  - make executable file "huff_codec"
# test  - test "huff_codec" using automatic tests from test folder
# clean - clean tests and build files

.PHONY: test clean

all: huff_codec

huff_codec:
	@cd src;\
	make;\
	cd ..;\
	cp src/huff_codec .

test: huff_codec
	@cd test;\
	./test.sh ../src/huff_codec;\
	cd ..

clean:
	rm -f huff_codec
	@cd src;\
	make clean;\
	cd ..
	rm -rf test/ERRORS
	rm -f test/*.log test/*.raw test/*.coded test/*.decoded
