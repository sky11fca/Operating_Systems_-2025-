#! /bin/sh

set -x

CURRENT_MONTH=$(date +"%b")
CURRENT_YEAR=$(date +"%Y")

read -p "Insert username: " username

id "$username"
exit_code=$#

if [ $exit_code -ne 0 ];
then 
    echo "ERROR: $username doesn't exist"
fi 

logins=$(last -F $username | grep "$CURRENT_MONTH" | grep $CURRENT_YEAR | head -n 3)


if [[ -z "$logins" ]];
then 
    echo "The user $username never connected"
else 
    echo "The last 3 connections of user $username in this month: "
    echo $logins
fi 