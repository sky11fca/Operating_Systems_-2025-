#! /bin/bash

if [ $# -ne 2 ];
then 
    echo "USAGE: $0 <p=1/2>"
    exit 1
fi

p=$1

if [ $p -ne 1 ] && [ $p -ne 2 ];
then 
    echo "Given integer must be 1 or 2"
    exit 2
fi 

if [ $p -eq 1];
then 
    ./workers/worker2 &
    sleep 2
    ./manager/supervisor ./input_data.txt &
elif [ $p -eq 2 ];
then 

    ./manager/supervisor ./input_data.txt &
    sleep 3
    ./workers/worker2 &
fi 

wait 

rm -f /dev/shm/shmem1
rm -f socket

exit 0