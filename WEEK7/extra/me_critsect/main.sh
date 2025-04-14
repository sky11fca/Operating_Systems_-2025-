#! /bin/sh

if [ $# -lt 2 ];
then 
    echo "USAGE: $0 <Nr Inst> <Program var>" >&2
    echo "Where: 1->without locks and 2->with locks" >&2
    exit 1
fi 

N=$1
VARIANT=$2


if [ "$VARIANT" -ne 1 ] || [ "$VARIANT" -ne 2 ];
then
    echo "SPECIFY: 1 ot 2" >&2;
    exit 2;
fi 


DIR_PROGRAMS="program"

if [ ! -d "DIR_PROGRAMS" ];
then
    echo "ERROR: Executable directory doesn't exist";
    exit 2;
fi 

if [ "$VARIANT" -eq 1 ];
then 
    EXECUTABLE="$DIR_PROGRAMS/mesect"
else 
    EXECUTABLE="$DIR_PROGRAMS/mesectL"
fi 


if [! ]