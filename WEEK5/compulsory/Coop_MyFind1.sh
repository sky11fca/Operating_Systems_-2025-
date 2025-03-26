#! /bin/sh 


if [ $# -ne 1 ]
then 
	echo "USAGE: $0 <dir>"
	exit 1
fi 

dir="$1"

if [ ! -d "$dir" ]
then 
	echo "ERROR: Given Directory '$dir' doesn't exist" >&2
	exit 1 
fi 

if [ ! -r "$dir" ] || [ ! -x "$dir" ] 
then 
	echo "ERROR: Given directory '$dir' doesn't have rights to read or traverse" >&2 
	exit 2
fi 

second_script="$(pwd)/subfolder/Coop_MyFind2.sh"

if [ ! -f "$second_script" ]
then 
	echo "ERROR: The second script does not exist" >&2 
	exit 1 
fi 

"$second_script" "$dir"
exit_code=$?

echo "Second Script exited with code: $exit_code"
exit 0
