#! /bin/sh  

if [ $# -ge 1 ]
then 
	n=$1 
else 
	read -p "Insert a number: " n 
fi 

if [ $n -lt 0 ]
then 
	echo "ERROR: number n must be a positive integer " >&2
	exit 1
fi 

find_cmd="find $HOME -maxdepth $n -type f -mtime -28"
files_list=$($find_cmd)
exit_code=$?

if [ $exit_code -ne 0 ]
then 
	echo "ERROR: find command failed " >&2
	exit 1
fi

IFS=$'\n' #IFS: internal field separator, apparently, Should iterate through file list normaly even though end with newline

for entry in $files_list
do 
	if [ -z "$entry" ]
	then 
		continue 
	fi 

	f_type=$(file -b "$entry")

	if [[ "$f_type" == *"C source"* ]]
	then 
		dir=$(dirname "$entry")
		base=$(basename "$entry")
		out="${dir}/${base%.*}"

		if gcc "$entry" -o "$out"
		then 
			echo "$entry -> Compiled OK!"
		else 
			echo "$entry -> Compile ERROR" >&2 
		fi
	else 
		f_date=$(stat -c "%y" "$entry")
		echo "$f_date $entry"
	fi 
done 
unset IFS

exit 0
