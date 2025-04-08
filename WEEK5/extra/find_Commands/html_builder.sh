#! /bin/sh

if [ $# -ne 1 ];
then 
    dir="$HOME"
else 
    dir="$1"
fi

rm -rf ~/html

mkdir ~/html
mkdir ~/html/muzica

file_html=~/html/index.html

touch $file_html

find "$dir" -type f -name "*.mp3" -exec ln {} ~/html/muzica \;


echo "<!DOCTYPE html>" > "$file_html"
echo "<html><head></head><body>" >> "$file_html"

echo "<h3>Playlist file</h3>" >> "$file_html"
echo "<ul>" >> "$file_html"


find ~/html/muzica -type l -name "*.mp3" -printf "<li><a href=\"muzica/%P\">%p</a></li>\n" >> $file_html

echo "</ul></body></html>" >> $file_html


chmod +x ~/html
chmod +x ~/html/muzica
chmod +r $file_html

echo "File created"

brave $file_html
