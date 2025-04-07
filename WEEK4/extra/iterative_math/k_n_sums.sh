#! /bin/sh 

if [ $# -eq 0 ];
then
	echo "Insert N: "
	read N 

	if [ $N -le 0 ];
	then 
		echo "ERROR N must be a positive integer"
		exit 1
	fi 

	echo "Insert $N values: "
	values=()

	for ((i=0; i<N; i++)); do
		read val 
		if [ $val -le 0 ];
		then 
			echo "ERROR, value must be positive integer"
			exit 1
		fi 
		values+=("$val")
	done 

	>  output.txt


	sum=0

	for val in "${values[@]}";
	do 
		sum=$((sum + val))
	done 

	echo "Sum: $sum"
	echo "$sum" >> output.txt
else 
	N=$1
	shift 

	if [ $N -le 0 ];
	then 
		echo "ERROR: N must be positive integer"
		exit 1
	fi 

	> output.txt 

	while [ $# -gt 0 ];
	do 
		sum=0
		count=0

		for ((i=0; i<N && $#>0; i++));
		do
			if [ $1 -le 0 ];
			then 
				echo "ERROR: value must be an positive integer number"
				exit 1
			fi 
			sum=$((sum+$1))
			shift 
			count=$((count+1))
		done 
		if [ $count -gt 0 ];
		then 
			echo "Group Sum: $sum"
			echo "$sum" >> output.txt
		fi 
	done 
fi 
