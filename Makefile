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
	rm -r test/*.log test/*.raw test/*.coded test/*.decoded
