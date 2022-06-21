#!/bin/bash

# testing.sh
# Runs the tested program on every Turing machine in the test folder and validates the results.
# USAGE:
#   ./testing.sh test_folder program max_proc timeout
#
#   test_folder - The folder with turing machines created by the tool testsGen.sh
#   program     - Prolog program for testing.
#   max_proc    - The number of parallel tests. High number can result in fork error.
#   timeout     - Prolog program will be stopped after a given amount of time (sec). 
#               - Prevents from looping.
#               - Small timeout can lead to less accurate test results.
#
# Project: Turing-machine (FLP project 2)
# Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz
# Last change: 24.04.2022


# Terminal colours.
RED="\033[1;31m"
GREEN="\033[1;32m"
RESET="\033[0m"
YELLOW="\033[1;33m"
BOLD="\033[1;0m"


function carriage_return(){
	for i in `seq 10`; do
		printf "\b"
	done
}

function show_cursor() {
    tput cnorm
}

function show_cursor_sigint() {
    tput cnorm
}

function hidecursor() {
    tput civis
}


RUNNING_PL=()
RUNNING_PY=()
MAX_PROC=$3
# Get a free slot for parallel testing.
# If there is no free slot, then complete running tests and free all slots.
function get_resources() {
    while true; do
        if [ ${#RUNNING_PL[@]} -lt $MAX_PROC ]; then
            break
        else
            for pid in ${RUNNING_PL[*]}; do
                wait $pid
            done
            for pid in ${RUNNING_PY[*]}; do
                wait $pid
            done
            RUNNING_PL=()
            RUNNING_PY=()
            break
        fi
    done
}


PROGRAM=$2
TIMEOUT=$4
# Parallel testing section.
function parallel() {

    # Run prolog
    timeout $TIMEOUT $PROGRAM <${1}.in >${1}.out
    # sleep 0.01
    return_code=$?

    # Return if the parent is allready dead.
    if ! kill -0 $4 >/dev/null 2>&1; then
       return 
    fi

    # Prolog timeouted
    if [ $return_code -eq 124 ]; then
        carriage_return
        echo -e "${YELLOW}WARNING:${RESET} prolog, on ${1}.in, does not ended in ${TIMEOUT}s."
        echo "2" >> $3
        return
    # Fork error, kill parrend and abort testing.
    elif [ $return_code -eq 125 ]; then
        echo -e "${RED}TEST ERROR:${RESET} Too many forks. Decrease MAXPROC in Makefile."
        kill -SIGINT $4
        return
    # Prolog rejected the Turing machine of the tape.
    elif [ $return_code -ne 0 ]; then
        carriage_return
        echo -e "${RED}DIFF:${RESET} unexpected prolog rejection on ${1}.in"
        echo "0" >> $3
        return
    fi

    # Test prolog output
    python3 validator.py ${1}.out ${1}.in 2>${1}.diff &

    RUNNING_PY[$2]=$!

    wait $!

    if [ $? -ne 0 ]; then
        # Return if the parent is allready dead.
        if ! kill -0 $4 >/dev/null 2>&1; then
           return 
        fi
        carriage_return
        echo -e "${RED}DIFF:${RESET} in ${1}.diff"
        echo "0" >> $3
    else
        echo "1" >> $3
        rm -f ${1}.diff
    fi
}

# Show cursor after aborting.
trap show_cursor EXIT
trap show_cursor_sigint SIGINT

hidecursor

FOLDER=$1
FILES=${FOLDER}/*.in
TOTAL=$(echo $FILES | wc -w)

# Crete temp. files for each parallel process.
for j in `seq 0 $MAX_PROC`; do
    TEMP_FILE[$j]=$(mktemp)
done

# Run testing.
CNT=1
echo "TESTING."
for file in $FILES; do
    file_name=${file::-3}
    get_resources $CNT
    carriage_return
    printf "%04d/%d" "$CNT" "$TOTAL"
    parallel $file_name $(($CNT % $MAX_PROC)) ${TEMP_FILE[$(($CNT % $MAX_PROC))]} $$ &
    CNT=$(($CNT+1))
    RUNNING_PL[$(($CNT % $MAX_PROC))]=$!
done

# Wait for last results.
echo ""
echo "Waiting for $(($TIMEOUT))s"
sleep $TIMEOUT

# Collect results from temp. files.
MAIN_TEMP_FILE=$(mktemp)
for file in ${TEMP_FILE[*]}; do
    cat $file >> $MAIN_TEMP_FILE
    rm $file
done
FAILED=$(grep "0" $MAIN_TEMP_FILE  -o | wc -l)
PASSED=$(grep "1" $MAIN_TEMP_FILE  -o | wc -l)
TIMEOUTED=$(grep "2" $MAIN_TEMP_FILE  -o | wc -l)
rm -f $MAIN_TEMP_FILE

# Print results.
echo ""
echo "--------------- DONE ---------------"

ACC=$(echo "print(round(($(($TOTAL-$TIMEOUTED))*100)/${TOTAL}, 2))" | python3)

echo -e "Test accuracy is $ACC%. (Increase timeout for higher accuracy.)"
echo -e "TOTAL : ${TOTAL}"
echo -e "${GREEN}PASSED:${RESET} $PASSED"
echo -e "${RED}FAILED:${RESET} ${FAILED}"
echo -e "${YELLOW}TIMEOUTED:${RESET} ${TIMEOUTED}"
show_cursor
