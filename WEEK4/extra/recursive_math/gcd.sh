#! /bin/sh

if [ $# -eq 0 ];
then 
    read -p "Insert a:" a
    read -p "Insert b:" b
elif [ $# -eq 2 ];
then 
    a=$1
    b=$2
else 
    echo "USAGE: $0 <a> <b>">&2
    exit 1
fi

function gcd_comp(){
    local a=$1
    local b=$2

    if [ $a -eq $b ];
    then 
        echo $b 
        exit 0
    elif [ $a -gt $b ];
    then 
        gcd_comp $((a-b)) $b 
    else
        gcd_comp $a $((b-a))
    fi
}



gcd_comp $a $b