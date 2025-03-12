#! /bin/sh 

#1
stat ~/programe/program1.c ~/programe/program2.c ~/programe/program2.h 

chmod u-r ~/programe/program1.c 
chmod ug+x ~/programe/program2.c 
chmod 440 ~/programe/program2.h 

#2

wc -lL /etc/mtab 

#3

find ~ -maxdepth 1 -type f -ctime -13 -printf "%M %p %t\n"

#4

find ~/Documents -type f -writable -executable -printf "%p %t %k bytes\n" 

#5

ps axo user,cmd,pid


