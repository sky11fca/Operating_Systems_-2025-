#! /bin/sh 

#1

sort -t : -k 3 -nr /etc/group

#2

ps -o pid,ppid,euser,ruser,cmd -U root -u root 

#3

find ~/Documents -perm -g=x,g=r -printf "%p %M\n"

#4



#5
#6
#7
#8
