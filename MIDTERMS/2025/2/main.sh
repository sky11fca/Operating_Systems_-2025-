#! /bin/bash

read -p "Insert a number of instances" NI

if [ $NI -le 0  ];
then 
    echo "NI must be a positive integer"
    exit 1
fi 

$BINFILE="freq.bin"

> "$BINFILE"

if [ ! -x "sum" ];
then 
    gcc sum.c -o sum
    if [ $? -ne 0 ];
    then 
        echo "ERROR COMPILING"
        exit 2
    fi 
fi 

for (( INST=1; INST<=NI; INST++ ));
do 
    ./sum "$BINFILE" "word$INST.txt" &
done 

./sum --words "$BINFILE"