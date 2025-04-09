#! /bin/sh


if [ $# -ne 2 ];
then 
    echo "USAGE: $0 <inst_nr> <program_variant>" >&2
    echo "1: No locks, 2: Locks" >&2
    exit 1
fi 



N=$1
VAR=$2 



if [ ! -d "programs" ];
then 
    echo "ERROR: Dir doesn't exist" >&2
    exit 2
fi


if [ ! -x "programs/Me_CritSect" ] || [ ! -x "programs/Me_CritSectLock" ];
then 
    gcc Me_CritSect.c -o programs/Me_CritSect
    gcc Me_CritSectLock.c -o programs/Me_CritSectLock

    if [ $? -ne 0 ] || [ ! -x "programs/Me_CritSect" ] || [ ! -x "programs/Me_CritSectLock" ];
    then 
        echo "COMPILING ERROR" >&2
        exit 2
    fi 

fi 


if [ ! -d "instruct" ];
then 
    echo "ERROR: Dir doesn't exist" >&2
    exit 3
fi 

for ((i=1; i<=$N; i++));
do 
    if [ ! -f "instruct/instr$i.txt" ] || [ ! -r "instruct/instr$i.txt" ]; 
    then
        echo "ERROR: File not found" >&2
        exit 3
    fi
done 

if [ ! -d "database" ];
then 
    echo "ERROR: Dir doesn't exist" >&2
    exit 4
fi 



if [ ! -f "database/stonks.bin" ];
then 
    echo "Creating database..." >&2
    touch database/stonks.bin

    if [ $? -ne 0 ];
    then
        echo "ERROR creating the database" >&2
        exit 4
    fi 


    echo "add some initial product: code, stock (CTRL+D TO CANCELL): "
    while read code stock;
    do 
        printf "%d\0\0\0\0%f" $code $stock >> database/stonks.bin
    done 
fi 


if [ ! -r "database/stonks.bin" ] || [ ! -w "database/stonks.bin" ]; 
then
    echo "ERROR Database has no read and write rights" >&2
    exit 4
fi

echo "RUNNING $N INSTANCES OF A PROGRAM..."
for ((i=1; i<=$N; i++));
do 
    if [ $VAR -eq 1 ];
    then 
        programs/Me_CritSect database/stonks.bin instruct/instr$i.txt
    else 
        programs/Me_CritSectLock database/stonks.bin instruct/instr$i.txt
    fi 
done 

wait 
echo "Success"