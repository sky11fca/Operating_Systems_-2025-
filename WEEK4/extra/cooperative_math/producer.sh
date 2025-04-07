#! /bin/sh 

if [ $# -gt 0 ];
then 
	numbers=("$@")
else 
	echo "Insert some numbers: "
	read -a numbers 
fi 


secventa1001=""

for num in "${numbers[@]}";
do
	if [ "$num" -gt 1000 ];
	then 
		secventa1001+="$num "
	fi 
done 


secventa1001=${secventa1001% }


export secventa1001
./consumer.sh
exit_code=$?

echo "Producer: Consumer exit code: $exit_code"
exit 0
