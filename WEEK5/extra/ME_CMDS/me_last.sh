#! /bin/sh

if [ $# -ne 1 ];
then 
    read -p "Insert an username: " user
else
    user="$1"
fi


useless_buffer="$(last $user)"
exit_code=$?

if [ $exit_code -eq 0 ];
then 
    last $user -3
else
    echo "user: $user never connected to the server"
fi 