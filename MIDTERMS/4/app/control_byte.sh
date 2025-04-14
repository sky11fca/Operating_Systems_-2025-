#! /bin/sh

if [ $# -ne 1 ];
then 
    exit 2
fi 

if [ ! -x "app/recurse.sh" ];
then 
    echo "GENERIC ERROR"
    exit 1
fi 


./app/recurse.sh "$1" | tr "0123456789" "1234567890" | sort -t '=' -k2n

exit $?