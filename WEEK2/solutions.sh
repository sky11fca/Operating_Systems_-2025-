#! /bin/sh 

#1
stat program1.c 
stat program2.c 
stat program2.h 

chmod u-r programe/program1.c 
chmod ug+x programe/program2.c 
chmod 440 program2.h 

#2

wc -lL /etc/mtab 

#3

find ~ -type f -ctime 13 -printf "%M, %p, %t\n"

#4

find ~/Documents -type f -writable -executable -printf "%p %t %k\n" 

#5

ps -o user,cmd,pid


