#! /bin/sh 

# First Excercise

if [ $# -ne 1 ]
then
	read -p "Insert a number n: " n
else 
	n=$1
fi


function pow() {
	power=$1
	n=$2
	echo "$power " 2> error_out.txt

	if [ $n -eq 0 ]
	then
		return
	fi

	pow $((power*2)) $((n-1)) 
}


pow 1 $n
