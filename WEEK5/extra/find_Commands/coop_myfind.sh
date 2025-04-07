#! /bin/sh

if [ $# -ne 1 ];
then 
    echo "USAGE: $0 <dir>" >&2
    exit 1
fi

dir="$1"


if [ ! -d "$dir" ];
then 
    echo "The path: $dir is not a directory" >&2
    exit 1
fi 


if [ ! -r "$dir" ];
then 
    echo "Given directory: $dir doesn't have reading permission" >&2
    exit 2
fi 


./subdir_sh/coop_myfind2.sh "$dir" 

exit_code=$?

echo "Second script executed with exit code $exit_code"