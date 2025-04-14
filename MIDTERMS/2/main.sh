#! /bin/sh

if [ $# -ne 1 ];
then 
    echo "USAGE: $0 <ARG>" >&2
    exit 1
fi 

ARG="$1"

if [ ! -d "scripts" ] || [ ! -f "scripts/listdir.sh" ] || [ ! -f "scripts/pipeline.sh" ];
then 
    echo "SCRIPT: script directory doesn't exist or specified script don't exist" >&2
    exit 2
fi 


if [ ! -x "scripts/listdir.sh" ];
then 
    chmod +x scripts/listdir.sh
fi 

if [ ! -x "scripts/pipeline.sh" ];
then 
    chmod +x scripts/pipeline.sh
fi 


if [ ! -d "programs" ] || [ ! -f "programs/build_scattered_numbers.c" ];
then 
    echo "PROGRAMS: No such directory or file" >&2
    exit 3
fi 

if [ ! -x "programs/build_scattered_numbers" ];
then 
    gcc programs/build_scattered_numbers.c -o programs/build_scattered_numbers
    exit_code=$?
    if [ $exit_code -ne 0 ];
    then 
        echo "PROGRAMS: Compile error" >&2
        exit 4
    fi 
fi 


./scripts/pipeline.sh "$ARG"
exit_code2=$?

echo "MAIN: script executed with exit code: $exit_code2"

exit $exit_code2