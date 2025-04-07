#!/bin/sh

set -x

A=1; 
B=0;
for a in $(ls /usr/share)
do
  [ -d "/usr/share/$a" ] && let B++
  
done
echo $B, $A

# The script was set to run /bin/noshel (Not a thing in Arch Linux BTW)
# B is commented out
# When a iterates through the result of ls at /usr/share, only gets the items, not the full path name. to make it work, the full path must be specified.