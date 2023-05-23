#!/bin/bash

# Trap for Ctrl+C
function handler {
    make clean
    exit 1
}
trap handler SIGINT

MAX_PROC=32
if [ $# -eq 1 ]; then
    MAX_PROC=$1
fi
TOTAL=$(($(($MAX_PROC-3))*100))
CNT=0
FAILED=0

# Build project
make

for n in `seq 4 $MAX_PROC`; do
    for rep in `seq 100`; do

        # Generate numbers
        ./numbersGen.sh $n &>/dev/null

        # Run and test clustering.
        mpirun -np $n ./parkmeans | python3 comparator.py &>/dev/null
        if [ $? -ne 0 ]; then
            mkdir -p ERRORS
            mv numbers ERRORS/numbers-${n}-${rep}
            echo "ERROR"
            FAILED=$(($FAILED+1))
        fi

        CNT=$(($CNT+1))
        echo "${CNT}/${TOTAL}"
    done
done

echo "FAILED = $FAILED"

# Clean after testing
make clean
