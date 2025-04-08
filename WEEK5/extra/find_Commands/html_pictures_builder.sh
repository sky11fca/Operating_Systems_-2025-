#! /bin/sh

if [ $# -ne 1 ];
then 
    dir="$HOME"
else 
    dir="$1"
fi

rm -rf ~/html

mkdir ~/html
mkdir ~/html/photos

file_html=~/html/index.html

touch $file_html

find "$dir" -type f \( -name "*.png" -o -name "*.gif" -o -name "*.jpg" \) -exec ln {} ~/html/photos \;


echo "<!DOCTYPE html>" > "$file_html"
echo "<html><head></head><body>" >> "$file_html"

echo "<h3>Images</h3>" >> "$file_html"
echo "<ul>" >> "$file_html"


find ~/html/photos -type f \( -name "*.png" -o -name "*.gif" -o -name "*.jpg" \) -printf "<li><a href=\"photos/%P\"> %p %t </a></li>\n" | sort -nk4>> $file_html

echo "</ul></body></html>" >> $file_html


chmod +x ~/html
chmod +x ~/html/photos
chmod +r $file_html

echo "File created"

brave $file_html #change to firefox

#I still hadn't figgured that out with the time stuff
