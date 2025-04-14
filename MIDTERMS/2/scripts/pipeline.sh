#! /bin/sh

if [ $# -ne 1 ];
then
    echo "PIPELINE.SH: No argument provided" >&2
    exit 1
fi

scripts/listdir.sh "$1" | sort -t ":" -k2,2nr | head -n 5 | cut -d ':' -f1