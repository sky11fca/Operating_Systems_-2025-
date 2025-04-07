#! /bin/sh

if [ $# -ne 1 ];
then 
	echo "USAGE: $0 <n>"
	exit 1
fi 

n=$1

if [ $n -le 0 ];
then 
	echo "ERROR: N must be a positive natural number" >&2
	exit 1
fi 

if [ $n -eq 1 ];
then
    echo 1
    exit 0
elif [ $n -eq 2 ];
then 
    echo 2
    exit 0
fi

a=1
b=2

iteration=3

function sequence1(){
    local a=$1
    local b=$2
    local iteration=$3


    if [ $iteration -eq $n ];
    then 
        echo "for iteration $iteration the value is = $b"
        exit 0
    else
        sequence1 $b $(($a+$b*4+iteration)) $((iteration+1)) $n
    fi 
}


sequence1 $a $b $iteration