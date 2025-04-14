#! /bin/sh 

if [ $# -ne 1 ];
then 
    echo "LISTDIR.SH: no DIRECTORY provided" >&2
    exit 1
fi 

DIR="$1"

if [ ! -d "$DIR" ];
then 
    echo "LISTDIR.SH: Not a directory" >&2
    exit 1
fi 


if [ ! -x "programs/build_scattered_numbers" ];
then 
    echo "LISTDIR.SH: build_scattered_numbers doesn't exist" >&2
    exit 2
fi 


for entry in "$DIR"/*;
do 
    if [ -d "$entry" ];
    then 
        "$0" "$entry"
    elif [ -f "$entry" ] && [[ "$entry" == "*.txt" ]];
    then 
        ./programs/build_scattered_numbers "$entry"

        $exit_code=$?

        if [ $exit_code -ne 0 ];
        then 
            echo "Something went wrong for file: EXIT CODE: $exit_code, File: $ENTRY" >&2
        fi 
    fi 

done 