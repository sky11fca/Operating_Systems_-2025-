#! /bin/sh


if [ $# -ne 1 ];
then 
    exit 2
fi 

DIRECTORY="$1"

if [ ! -d "$DIRECTORY" ] || [ ! -x "$DIRECTORY" ] || [ ! -r "$DIRECTORY" ];
then 
    exit 3
fi 

if [ ! -x "validators/default.sh" ];
then 
    echo "Default.sh: No such script exist" >&2
    exit 1
fi 

for entry in "$DIRECTORY"/*;
do 
    if [ -d "$entry" ];
    then 
        "$0" "$entry";
    elif [ -f "$entry" ];
    then 
        ./validators/default.sh "$entry" >> results.txt 2>> results.txt
    fi 
done 