#! /bin/sh 
set -x
if [ $# -ne 1 ];
then 
	echo "USAGE: $0 <n>" >&2;
	exit 1;
fi 

n=$1

if [ $n -lt 2 ];
then 
	echo "ERROR: n must be an integer greater than 2" >&2
	exit 1;
fi 


a=1
b=1
it=2

while [ $it -lt $n ];
do 
	aux=$((a+b))
	a=$b 
	b=$aux 
	it=$((it+1))
done 

echo "Fib($n)=$b"

echo "Sucessfully executed"
