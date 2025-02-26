#! /bin/sh 

#1)
mkdir ~/programe
mkdir ~/programe/tema{1,2}
mkdir ~/programe/tema1/sub-temaA
mv programe/tema1/sub-temaA programe/tema2/tema2_sub-temaA

#2)

touch ~/programe/program{1,2}.c ~/programe/program2.h
touch ~/programe/tema1/tema1-{1,2}.c 
touch ~/programe/tema2/tema2_sub-temaA/sub-temaA1.{c,cpp}
ln ~/programe/tema2/tema2_sub-temaA/sub-temaA1.c ~/programe/tema2/tema2_sub-temaA/tema2_sub-temaA1.c ; ln -s ~/programe/tema2/tema2_sub-temaA/sub-temaA1.cpp ~/programe/tema2/tema2_sub-temaA/tema2_sub-temaA1.cpp
cp ~/programe/tema1/tema1-1.c ~/programe/tema2/tema2-1.c 
mv ~/programe/tema1/tema1-2.c ~/programe/tema2/tema2-2.c 
rm ~/programe/tema2/tema2_sub-temaA/tema2_sub-temaA1.c 

#3)

cut -d : --output-delimiter=" - " -f 1,3 /etc/passwd

#4)

grep ":x:[0-9]:0:" /etc/passwd 

#5)

stat /bin/sh --printf="Nr of links = %h\n"
