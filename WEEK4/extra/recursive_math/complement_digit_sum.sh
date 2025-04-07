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

nr=0

function complement_number(){
    local n=$1
    local nr=$2


    if [ $n -eq 0 ];
    then 
        echo "Complement_number = $nr"
        exit 0
    else
        complement_number $((n/10)) $(( nr*10 + 9 - n%10))
    fi 
}


complement_number $n $nr