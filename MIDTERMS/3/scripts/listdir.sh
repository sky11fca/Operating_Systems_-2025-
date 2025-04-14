#! /bin/sh

if [ $# -ne 1 ];
then 
    echo "NO PROVIDED ARGS" >&2
    exit 2
fi 

ARG="$1"


if [ ! -x "programs/file_page" ];
then 
    gcc programs/file_page.c -o programs/file_page
    exit_code=$?

    if [ $exit_code -ne 0 ];
    then 
        echo "listdir.sh: COMPILER ERROR" >&2
        exit 3
    fi 
fi 

if [ -f "$ARG" ];
then 
    ./programs/file_page "$ARG"
elif [ -d "$ARG" ];
then
    for entry in "$ARG"/*;
    do 
        "$0" "$entry"
    done 
else 
    echo "NOT VALID" >&2
    exit 2
fi 