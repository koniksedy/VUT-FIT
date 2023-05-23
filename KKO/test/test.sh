#!/bin/bash

# A script with automaton tests for Huffman coder/decoder.
# Author: Michal Šedý <xsedym02@vutbr.cz>
# Last change: 04.05.2023

#######################################################
#                     TEST CASES                      #
#=====================================================#
#       RANDOM        |       HARD       |    DATA    #
# -r {1,2,4,8,32,256} |   -r / no flag   |  no flag   #
#=====================================================#
# aligned / unaligned W / unaligned H / unaligned W&H #
#=====================================================#
#             no flag / -a / -m / -a -m               #
#######################################################


PROGRAM=$1
BLOCK_SIZE=64
PASS=0
FAIL=0
TOTAL=$((6*4*4 + 2*4*4 + 4*`ls data/*.raw | wc -l`))

function log() {
    name=$1
    width=$2
    height=$3
    flags=$4
    codec_size=$5
    time=$6

    printf "%-28s %-7s %-8s %-10s %-14s %s sec\n" "$1" "$2" "$3" "$4" "$5" "$6"
}

function err_log() {
    printf "%-28s %-7s %-8s %-10s %s\n" "$1" "$2" "$3" "$4" "$5"
}

function test() {
    name=$1
    width=$2
    height=$3
    raw_size=$(du -b ${name}.raw | sed -e 's/\s.*//')
    for flag in " " "-a" "-m" "-a -m"; do
        start_time=$(($(date +%s%N)/1000000))
        log_file=${name}$(echo $flag | sed 's/ //g').log
        ./$PROGRAM -c -i ${name}.raw -w $width $flag -o ${name}.coded &>$log_file
        if [ $? -ne 0 ]; then
            err_log "${name}.raw" "$width" "$height" "$flag" "ERROR: CODER FAILED"
            mkdir -p ERRORS
            echo $flags >> ERRORS/${name}.flags
            cp ${name}.raw $log_file ERRORS/ 2>/dev/null
            FAIL=$(($FAIL+1))
            continue
        fi

        end_time=$(($(date +%s%N)/1000000))
        coding_time=$(($end_time - $start_time))
        coding_time=$(echo "scale=3; ${coding_time}/1000" | bc)
        coded_size=$(du -b ${name}.coded | sed -e 's/\s.*//')
        efficiency=$(echo "scale=2; ($coded_size*8)/$raw_size" | bc)

        ./$PROGRAM -d -i ${name}.coded -o ${name}.decoded &>$log_file
        if [ $? -ne 0 ]; then
            err_log "${name}.coded" "$width" "$height" "$flag" "ERROR: DECODER FAILED"
            mkdir -p ERRORS
            echo $flags >> ERRORS/${name}.flags
            cp ${name}.raw ${name}.coded $log_file ERRORS/ 2>/dev/null
            FAIL=$(($FAIL+1))
            continue
        fi

        diff ${name}.raw ${name}.decoded &>/dev/null
        if [ $? -ne 0 ]; then
            err_log "${name}.raw" "$width" "$height" "$flag" "ERROR: DIFFERENT OUTPUT"
            mkdir -p ERRORS
            echo $flags >> ERRORS/${name}.flags
            cp ${name}.raw ${name}.coded ${name}.decoded ERRORS/ 2>/dev/null
            FAIL=$(($FAIL+1))
            continue
        fi
        PASS=$(($PASS+1))
        log $name $width $height "$flag" $efficiency "$coding_time"
    done
}


###############
# PREPARATION #
###############
aligned=$(($BLOCK_SIZE*3))
unaligned=$(($BLOCK_SIZE*3-5))

echo "################################################################################"
echo "NAME                         WIDTH   HEIGHT   FLAGS      EFFICIENCY     TIME"
echo "--------------------------------------------------------------------------------"

##################
# TESTING RANDOM #
##################
for r in 1 2 4 8 32 256; do
    for a in "$aligned $aligned" "$unaligned $aligned" "$aligned $unaligned" "$unaligned $unaligned"; do
        width=$(echo $a | cut -d " " -f1)
        height=$(echo $a | cut -d " " -f2)
        name="random_w${width}_h${height}_r${r}"
        python3 randomgen.py --width $width --height $height -r $r -o ${name}.raw
        test $name $width $height
        rm -f ${name}.raw ${name}.coded ${name}.decoded *.log
    done
done

################
# TESTING HARD #
################
for random_flag in "" "-r"; do
    for a in "$aligned $aligned" "$unaligned $aligned" "$aligned $unaligned" "$unaligned $unaligned"; do
        width=$(echo $a | cut -d " " -f1)
        height=$(echo $a | cut -d " " -f2)
        name="hard_w${width}_h${height}${random_flag}"
        python3 hardgen.py --width $width --height $height $random_flag -o ${name}.raw
        test $name $width $height
        rm -f ${name}.raw ${name}.coded ${name}.decoded *.log
    done
done

###########################
# TESTING PICTUREC (data) #
###########################
for file in data/*.raw; do
    name=$(echo $file | rev | cut -d "/" -f1 | rev | cut -d "." -f1)
    cp $file .
    width=512
    height=512
    test $name $width $height
    rm -f ${name}.raw ${name}.coded ${name}.decoded *.log
done


##################
# RECAPITULATION #
##################
echo "--------------------------------------------------------------------------------"
echo "################################################################################"
echo "TOTAL: $TOTAL"
echo "PASS:  $PASS"
echo "FAIL:  $FAIL"
