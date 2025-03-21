#! /bin/sh 

# Second excercise: Producer script

if [ $# -eq 0 ];
then 
	echo "Insert some numbers: "
	read -a numbers 
fi 

read -p "Insert p: " p

for num in $@;
do
	if [ $num -le $p ]
	then 
		echo -n "$num "
	else 
		break
	fi
done 

