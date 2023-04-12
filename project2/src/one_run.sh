#!/bin/bash

make clean
make numbers NUM=$1
make run PROC=$1
