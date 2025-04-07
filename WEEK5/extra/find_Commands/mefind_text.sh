#! /bin/sh

if [ $# -lt 1 ];
then 
    read -p "Insert an existing directory: " dir
else
    dir="$1"
fi


if [ ! -d "$dir" ];
then 
    echo "The path: $dir is not a directory"
    exit 1
fi 

function me_find(){
    local dir="$1"

    for entry in $dir/*;
    do
        if [ -d "$entry" ];
        then 
            me_find "$entry"
        fi 

        if [ -f "$entry" ];
        then 
            if [[ $(file -b "$entry") == *"ASCII text"* ]];
            then 
                wc -wL "$entry" 
            fi 
        fi 
    done

}



me_find "$dir"