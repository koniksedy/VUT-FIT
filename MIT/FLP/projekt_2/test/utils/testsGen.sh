#!/bin/bash

# testsGen.sh
# Generates 1539 turing machines.
# USAGE:
#   ./testsGen.sh output_folder max_proc
#
#   output_folder - The folder, where the tests will be generated.
#                 - Files with colliding names will be overwritten.
#   max_proc      - The number of parallel tests. High number can result in fork error.
#
# Project: Turing-machine (FLP project 2)
# Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz
# Last change: 24.04.2022


function carriage_return() {
	for _ in `seq 10`; do
		printf "\b"
	done
}

function show_cursor() {
    tput cnorm
}

function hidecursor() {
    tput civis
}

RUNNING=()
MAX_PROC=$2
# Get a free slot for parallel testing.
# If there is no free slot, then complete running tests and free all slots.
function get_resources() {
    while true; do
        if [ ${#RUNNING[@]} -lt $MAX_PROC ]; then
            break
        else
            for pid in ${RUNNING[*]}; do
                wait $pid
            done
            RUNNING=()
            break
        fi
    done
}

# Show cursor after aborting.
trap show_cursor EXIT

hidecursor

FOLDER=$1
TOTAL=1539
STATES="5 10 25"
ALPHABET="5 10 25"
INPUT_LEN="10 20 100"
LEFT_P="0 0.2 0.5"
CHANGE_P="0.2 0.8 1"
STEPS="10 20 40 110"

if [ ! -d $FOLDER ]; then
    mkdir $FOLDER
fi

CNT=1

echo "GENERATING TESTS."
for i in `seq 1 3`; do
    for states in $STATES; do
        for alphabet in $ALPHABET; do
            for input_len in $INPUT_LEN; do
                for left_p in $LEFT_P; do
                    for change_p in $CHANGE_P; do
                        for steps in $STEPS; do
                            if [ $steps -gt 10 ] && [ $states -eq 5 ]; then
                                continue
                            fi
                            if [ $steps -ge 40 ] && [ $input_len -le 20 ]; then
                                continue
                            fi
                            # if [ $steps -lt $input_len ]; then
                            #     continue
                            # fi
                            get_resources $CNT
                            carriage_return
					        printf "%04d/%d" "$CNT" "$TOTAL"
                            CNT=$(($CNT+1))
                            python3 ntmGen.py ${alphabet} ${states} ${input_len} ${left_p} ${change_p} ${steps} >${FOLDER}/${alphabet}-${states}-${input_len}-${left_p}-${change_p}-${steps}-${i}.in &
                            RUNNING[$(($CNT % $MAX_PROC))]=$!
                        done
                    done
                done
            done
        done
    done
done

echo ""
echo "--------------- DONE ---------------"
show_cursor
