#!/bin/sh
if [ $# -lt 1 ]; 
then
    read -p "Dati n: " n
else
    n=$1
fi

if [ $n -eq 0 ] || [ $n -eq 1 ];
then 
    echo "Fact($n)=1"
    exit 0
fi

prod=1
for k in $(seq 1 $n);
do
    let prod*=k
done

echo "Fact($n)=$prod"
exit 0

# Script calls /bin/nologin
# Illegal usage of if statement, lack of spaces
# If statement not properly ended, no fi
# No special cases when n is 0 or 1
# prod variable initialized with 0
# Seq command not properly substituted
# illegal operation prod*=k: use let
# will output Fact(number)=prod whenever the program is executed (no $ in prog)