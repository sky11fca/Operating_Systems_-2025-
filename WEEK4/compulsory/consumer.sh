#! /bin/sh  

# Second script: Consumer script

read nums

if [ -z "$nums" ]
then 
	echo "Consumer: computed sum is NULL";
	exit 1
fi 

sum=0 
for num in $nums
do 
	cube=$((num*num*num))
	sum=$((sum+cube))
done 

echo "Consumer: computed sum is $sum."
exit 0
