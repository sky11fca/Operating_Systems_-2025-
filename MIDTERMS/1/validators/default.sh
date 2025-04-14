#! /bin/sh

if [ $# -ne 1 ];
then 
    echo "Default.sh: must specify argument" >&2
    exit 2
fi

FILE="$1"

if [ ! -x "validators/validatorASCII" ];
then 
    gcc validators/validatorsASCII.c -o validators/validatorsASCII
    exit_code=$?
    if [ $exit_code -ne 0 ]
    then 
        echo "Compilation error" >&2
        exit 3
    fi 
fi 


./validators/validatorASCII "$FILE" | tr ":" ">"