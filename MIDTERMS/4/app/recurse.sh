#! /bin/sh

if [ $# -ne 1 ];
then 
    exit 2
fi 

if [ ! -x "app/source/first_and_last_byte" ];
then 
    gcc app/source/first_and_last_byte.c -o app/source/first_and_last_byte 

    if [ $? -ne 0 ];
    then 
        echo "COMPILER ERROR" >&2
        exit 3
    fi 

fi 

function process_function()
{
    local ELEMENT="$1"

    if [ -f "$ELEMENT" ];
    then 
        ./app/source/first_and_last_byte "$ELEMENT"
    elif [ -d "$ELEMENT" ];
    then 
        for entry in "$ELEMENT"/*;
        do 
            process_function "$entry"
        done
    fi 
}

process_function "$1"