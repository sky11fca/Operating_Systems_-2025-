#! /bin/sh 

#1)
cut -d : -f 1,7 /etc/passwd | grep "/sbin/nologin" | grep -E "*[m-z]:" | head -n15 > max15.txt

#2)
ps -axo user,cmd,pid,tty | tr -s " " ":"| grep -e ":pts/[0,2,3,5]" | sort -t: -nk3 | grep ":bash:" > output.txt 

#3)

who | tr -s " " | sort -k3, 4 -t" " | head -n8 | cut -d" " -f5 > filtruIPs.txt

#4)

ps -eo user,comm,pid --no-headers | tr -s " " | cut -d" " -f1,2,3 --output-delimiter=: | sort -k2 -t:
