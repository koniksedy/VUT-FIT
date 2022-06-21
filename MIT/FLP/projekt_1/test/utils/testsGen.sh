#!/bin/bash

# testsGen.sh
# Generates 3000 grammars and their reduced versions.
# USAGE:
# 	./testsGen.sh output_folder
#
#	output_folder - The folder, where the tests will be generated.
#                 - The grammars with colliding names will be overwritten.
#
# Project: SIMPLIFY-BKG (FLP)
# Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
# Last change: 24.03.2022


function carriage_return(){
	for i in `seq 10`; do
		printf "\b"
	done
}

function show_cursor() {
    tput cnorm
}

function hidecursor() {
    tput civis
}

# Show cursor after aborting.
trap show_cursor EXIT

hidecursor

# 5x5x6x4x5 = 3000 grammars = 9000 tests
TERMS_CNTS="1 2 4 8 16"
NONTERMS_CNTS="1 2 4 8 16"
RULES_PER_T="0.1 0.2 0.5 1 2 4"
MAX_RIGHT="1 2 4 8"
EPSILON="0.01 0.1 0.2 0.4 0.8"
OUTPUT_FOLDER=$1

TOTAL=3000
CNT=1

echo "GENERATING TESTS."
for n in $NONTERMS_CNTS; do
	for t in $TERMS_CNTS; do
		for r in $RULES_PER_T; do
			for m in $MAX_RIGHT; do
				for e in $EPSILON; do
					carriage_return
					printf "%04d/%d" "$CNT" "$TOTAL"
					CNT=$(($CNT+1))
					python3 gramGen.py $n $t $r $m $e ${1}/${n}-${t}-${r}-${m}-${e}
					if [ $? != 0 ]; then
						echo ""
						echo "ERROR: aborting."
						exit 1
					fi
				done
			done
		done
	done
done

echo ""
echo "--------------- DONE ---------------"
show_cursor
