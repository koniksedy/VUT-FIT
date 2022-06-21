#!/bin/bash

# testing.sh
# Runs tested program or every grammar in the test folder and compares the results.
# USAGE:
# 	./testing.sh test_folder program
#
#	test_folder - The folder with grammars created by the tool testsGrn.sh
# 	program - Testing program.
#
# Project: SIMPLIFY-BKG (FLP)
# Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz
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

# Show cursor after aborting
trap show_cursor EXIT

hidecursor

# Remove old results
rm -f ${1}/*-output.cfg

TEST_FOLDER=$1
PROGRAM=$2
GRAMMARS=${1}/*-i.cfg
OPTS="-i -1 -2"

# Terminal colours.
RED="\033[1;31m"
GREEN="\033[1;32m"
RESET="\033[0m"


TOTAL=$(($(echo $GRAMMARS | wc -w)*3))
CNT=1

PASS=0
FAILED=0

echo "TESTING"
for grammar in $GRAMMARS; do
	purename=${grammar%-*}
	for opt in $OPTS; do

		# Log
		carriage_return
		printf "%04d/%d" "$CNT" "$TOTAL"
		CNT=$(($CNT+1))

		correct_result=${purename}${opt}.cfg
		output=${purename}${opt}.out
		./$PROGRAM $opt $grammar > $output
		# Test program return code.
		if [ $? != 0 ]; then
			carriage_return
			echo -e "${RED}ERROR:${RESET} $grammar $opt to $output"
			FAILED=$(($FAILED+1))
		else
			# Compare outputs.
			diff $correct_result $output > /dev/null
			if [ $? != 0 ]; then
				carriage_return
				echo -e "${RED}DIFF:${RESET} $correct_result $opt in $output"
				FAILED=$(($FAILED+1))
			else
				PASS=$(($PASS+1))
			fi
		fi
	done
done

echo ""
echo "--------------- DONE ---------------"
echo -e "${GREEN}PASSED:${RESET} ${PASS}"
echo -e "${RED}FAILED:${RESET} ${FAILED}"
show_cursor
