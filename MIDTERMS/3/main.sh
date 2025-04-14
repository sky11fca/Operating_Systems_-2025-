#! /bin/sh

if [ $# -ne 1 ];
then 
    echo "USAGE: $0 <arg>"
    exit 1
fi 

ARG="$1"

if [ ! -d "scripts" ] || [ ! -x "scripts/pipeline.sh" ]
then 
    echo "SCRIPTS: THERE IS NO SUCH DIRECTORY OR SCRIPT">&2
    exit 1
fi 

if [ ! -d "programs" ] || [ ! -r "programs/file_page.c" ]
then 
    echo "PROGRAMS: THERE IS NO SUCH DIRECTORY OR FILE">&2
    exit 2
fi

if [ ! -d "$ARG" ];
then 
    echo "MAIN: Provided argument is not a directory" >&2
    exit 3
fi 

./scripts/pipeline.sh "$ARG"
EXIT_CODE=$?

echo "Program existed with exit code: $EXIT_CODE"