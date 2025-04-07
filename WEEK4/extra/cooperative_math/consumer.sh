#! /bin/sh 

sum=0
count=0


if [ -z "$secventa1001" ]; then 
	echo "Consumer: computes sum is NULL"
	exit 1 
fi 


for num in $secventa1001; do 
	sum=$((sum+num))
	count=$((count+1))
done 


if [ $count -eq 0 ];
then 
	echo "Consumer: computes sum is NULL"
	exit 1 
else 
	echo "ConsumerL computes sum is $sum"
	exit 0
fi
