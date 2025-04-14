#! /bin/sh

if [ $# -ne 1 ];
then 
    echo "USAGE: $0 <ARG>" >&2
    exit 1
fi 

if [ ! -d "app" ] || [ ! -x "app/control_byte.sh" ];
then 
    echo "GENERIC ERROR" >&2
    exit 1
fi

if [ ! -d "app" ] || [ ! -d "app/source" ] || [ ! -r "app/source/first_and_last_byte.c" ];
then 
    echo "GENERIC ERROR" >&2
    exit 2
fi

ARG="$1"

if [ ! -d "$ARG" ] && [ ! -f "$ARG" ];
then 
    echo "GENERIC ERROR" >&2
    exit 3
fi 


./app/control_byte.sh "$ARG"

exit_code=$?

echo "EXIT CODE: $exit_code"