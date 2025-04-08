#! /bin/sh

if [ $# -ne 1 ];
then 
    echo "USAGE: $0 <dir>" >&2
    exit 1
fi

dir="$1"

if [ ! -d "$dir" ];
then
    echo "ERROR: the given item is not a directory"
    exit 1
fi

for txt_file in "$dir"/*.txt;
do 
    if [ -f "$txt_file" ];
    then 
        gzip -k "$txt_file" # -k keeps the original file
    else 
        break 
    fi 
done


for sh_file in "$dir"/*.sh;
do 
    if [ -f "$sh_file" ];
    then
        base_name=$(basename "$sh_file" .sh)
        zipname="$dir/$base_name.zip"

        zip -j "$zip_name" "$sh_file" # -j just the file, not the entire directory path  
    else 
        break
    fi 
done 