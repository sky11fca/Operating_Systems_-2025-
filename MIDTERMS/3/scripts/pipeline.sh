#! /bin/sh

if [ $# -ne 1 ];
then 
    exit 2
fi 

DIR="$1"

if [ ! -x "scripts/listdir.sh" ];
then 
    echo "PIPELINE.SH: NO SUCH SCRIPT EXISTS" >&2
    exit 1
fi 

total_pages=0

./scripts/listdir.sh "$DIR" | while read line;
do 
    mod_line=$(echo "$line" | sed 's/+++/ /')


    file=$(echo "$mod_line" | cut -d " " -f 2)
    page=$(echo "$mod_line" | cut -d " " -f 3)

    echo "$file ---> $page"

    total_pages=$((total_pages+pages))
done 

echo "TOTAL NUMBER OF PAGES: $total_pages">&2
