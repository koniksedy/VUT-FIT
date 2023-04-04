#!/bin/bash

# Deside numbers count.
numbers_cnt=10
if [ $# -eq 2 ]; then
    numbers_cnt=$1
fi

# Generate numbers into "numbers" file.
dd if=/dev/random bs=1 count=$numbers_cnt of=numbers &>/dev/null
