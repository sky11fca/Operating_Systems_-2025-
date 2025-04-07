#! /bin/sh 

if [ $# -eq 2 ];
then 
	n=$1
	k=$2
else 
	echo "USAGE: $0 <n> <k>" >&2
	exit 0;
fi 


if [ $n -le 0 ] && [ $k -le 0 ];
then 
	echo "ERROR: n and k must be positive natural numbers" >&2
	exit 0
fi 

if [ $n -lt $k ];
then 
	echo "ERROR: n must be greater than k" >&2
	exit 0
fi


function factorial(){
	number=$1
	if [ $number -eq 0 ] || [ $number -eq 1 ];
	then 
		echo 1
		return
	fi 
	iterator=1
	product=1

	while [ $iterator -le $number ];
	do 
		product=$((product*iterator))
		iterator=$((iterator+1))
	done 

	echo $product
}


echo "A($n, $k) = $(($(factorial $n)/$(factorial $((n-k)))))"
