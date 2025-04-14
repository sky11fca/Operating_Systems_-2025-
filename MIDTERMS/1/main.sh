#! /bin/sh

if [ $# -ne 1 ];
then
    echo "USAGE: $0 <a directory>">&2
    exit 1
fi

DIRECTORY="$1";

if [ ! -d "xprocesator" ] || [ ! -x "xprocesator/procesator.sh" ];
then 
    echo "No such file xprocesator/procesator.sh exist, create directory or file" >&2
    exit 1
fi

if [ ! -d "validators" ] || [ ! -r "validators/validatorASCII.c" ];
then 
    echo "No such file validator/validatorASCII.c exists" >&2
    exit 2
fi

if [ ! -d "$DIRECTORY" ] || [ ! -r "$DIRECTORY" ];
then 
    echo "Directory $DIRECTORY doesn't exist" >&2
    exit 3
fi 

./xprocesator/procesator.sh "$DIRECTORY"
exit_code=$?

echo "Program exited with code: $exit_code"
exit $exit_code
