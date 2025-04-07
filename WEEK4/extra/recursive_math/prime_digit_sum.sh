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

sum=0


function is_prime(){
	x=$1
	div=0

	for i in $(seq 1 $x);
	do 
		if [ $((x%i)) -eq 0 ];
		then
			div=$((div+1))
		fi 
	done 


	if [ $div -eq 2 ];
	then 
		echo 1
	else 
		echo 0
	fi
}


function prime_sum() {
	local n=$1
	local sum=$2


	if [ $n -eq 0 ];
	then 
		echo "Prime_sum=$sum"
		exit 0
	fi 


	if [ $(is_prime $((n%10))) -eq 0 ];
	then 
		prime_sum $((n/10)) $sum 
	else 
		prime_sum $((n/10)) $((sum + n%10))
	fi 
	
}



prime_sum $n $sum
