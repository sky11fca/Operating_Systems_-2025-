#! /bin/sh

set -x

dir="$1"
total_lines=0;

function procedure(){
    local dir="$1"

    for entry in "$dir"/*;
    do 
        if [ -d "$entry" ];
        then 
            procedure "$entry"
        fi 

        if [ -f "$entry" ];
        then 
            if [[ $(file -b "$entry") == *"C source"* ]];
            then

                wc -l "$entry"
                nr_lines=$(wc -l "$entry" | tr -s " " ":" | cut -d: -f1)
                let total_lines+=nr_lines

            else 

                echo "$entry  $(head -n 5 $entry)" >&2

            fi 
        fi 

    done

}


procedure "$dir"


if [ $total_lines -eq 0 ];
then 
    echo "Nr lines=0"
    exit 1;
else
    echo "Nr lines=$total_lines"
    exit 0;
fi