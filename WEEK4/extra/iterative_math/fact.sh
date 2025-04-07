#! /bin/sh 

if [ $# -ne 1 ];
then 
	echo "USAGE: $0 <n>" >&2
	exit 1
fi 

n=$1

if [ $n -lt 0 ];
then 
	echo "ERROR: N must be an positive number" >&2
	exit 1
elif [ $n -eq 0 ] || [ $n -eq 1 ];
then 
	echo "fact($n)=1"
	exit 0
else 
	iterator=1
	product=1

	while [ $iterator -le $n ];
	do 
		product=$((product*iterator))
		iterator=$((iterator+1))
	done 

	echo "fact($n)=$product"
	exit 0
fi 
	
