#!/bin/bash

CXX=clang++

SRC_FILE=main.cpp
OUT_FILE=main

$CXX -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include -L/usr/local/opt/libomp/lib -lomp $SRC_FILE -o $OUT_FILE 
./$OUT_FILE
