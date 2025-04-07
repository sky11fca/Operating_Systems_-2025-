#! /bin/sh

set -x

if [ $# -ne 3 ];
then 
    echo "USAGE: $0 <Source> <Destination> <Targetfile>" >&2
    exit 1;
fi

source="$1"
destination="$2" 
files_extensions="$3"

if [! -d "$source"];
then
    echo "ERROR: $source not a directory or doesn't exist" >&2
    exit 1;
fi

if [! -d "$destination"];
then
    echo "ERROR: $destination not a directory or doesn't exist" >&2
    exit 1;
fi

if [! -f "$files_extensions"];
then
    echo "ERROR: $files_extensions not a directory or doesn't exist" >&2
    exit 1;
fi

sourcedir=$(realpath "$source")
destdir=$(realpath "$destination")

if [ "$sourcedir" = "$destdir" ];
then 
    echo "These are the same source destination folders" >&2
    exit 1
fi 


if [ "$sourcedir" == "$destdir" ];
then 
    echo "$source is the parrent of $destination" >&2
    exit 1
fi 

extensions=()
while IFS= read -r line || [[ -n "$line" ]]; do 
    [ -z "$line" ] && continue 
    extensions+=("$line")
done < "$files_extensions"
