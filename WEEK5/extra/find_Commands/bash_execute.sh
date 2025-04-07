#! /bin/sh

if [ $# -ne 1 ];
then 
    read -p "Insert a number: " n
else 
    n=$1
fi


find_cmd="find $HOME -maxdepth $n -type f"
file_list=$($find_cmd)
exit_code=$?



if [ $exit_code -ne 0 ];
then
    echo "Find command failed" >&2
    exit 2
fi


IFS=$'\n'
for entry in $file_list;
do 
    if [ -z "$entry" ];
    then
        continue 
    fi 


    if [[ $(file -b "$entry") == *"Bourne-Again shell script"* ]];
    then
        chmod +x "$entry"
        permisions=$(stat -c "%A" $entry)
        echo "$permisions $entry" 
    else 
        permisions=$(stat -c "%a" $entry)
        echo "$permisions $entry"
    fi
done

unset IFS