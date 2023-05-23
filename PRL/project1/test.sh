#!/bin/bash

# Compilation
mpic++ --prefix /usr/local/share/OpenMPI -o parsplit parsplit.cpp

ERROR=0
TOTAL=500
CNT=0
for proc in `seq 10`; do
    for n in `seq 50`; do
        numbers_tot=$(($proc * $n))

        # Generate numbers
        dd if=/dev/random bs=1 count=$numbers_tot of=numbers &>/dev/null

        # Run test
        mpirun -np $proc ./parsplit | python3 comparator.py numbers &> out.txt
        if [ $? -ne 0 ]; then
            echo "ERROR"
            mkdir -p ERRORS
            ERROR=$(($ERROR + 1))
            mv out.txt ERRORS/${CNT}-out.txt
            mv numbers ERRORS/${CNT}-numbers
        fi

        CNT=$(($CNT + 1))
        echo "${CNT}/${TOTAL}"
    done
done

# Clean temp files
rm -f oems numbers out.txt

echo "##############################"
echo "ERRORS: $ERROR"
