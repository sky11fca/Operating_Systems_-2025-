#! /bin/bash

#THis script will do the following:
# 1. Will receive a number as an argument, the number must be either 1 or 2. If that is not the case a message will be displayed and the script will exit.
# 2. If the number is 1, then will execute the worker1 program in background firstand after a pause of 2 seconds will execute worker 2 program
# 3. If the number is 2 then it will execute worker 2 in background, will sleep for 3 seconds and then execute worker 1
# 4. No matter the case worker 1 will be executed with input_data.txt as an argument
# 5. And then after the 3 programs are done executiong and done displaying the contents of the named non-persistent mapping file and then will destroy the shared memory file and the fifo file

if [ $# -ne 1 ]; then 
    echo "USAGE: $0 <p> (1/2)"
    exit 1
fi

p=$1

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)

if [ "$p" != "1" ] && [ "$p" != "2" ]; 
then 
    echo "P must be either 1 or 2"
    exit 1
fi 

# cleaning mess:

rm -f "$SCRIPT_DIR/share_mem_file"
rm -f "$SCRIPT_DIR/a_fifo"

mkfifo "$SCRIPT_DIR/a_fifo"

if [ "$p" == "1" ];
then 
    "$SCRIPT_DIR/subordinates/worker1" "$SCRIPT_DIR/input_data.txt" &
    sleep 2
    "$SCRIPT_DIR/subordinates/worker2" &
else
    "$SCRIPT_DIR/subordinates/worker2" & 
    sleep 3
    "$SCRIPT_DIR/subordinates/worker1" "$SCRIPT_DIR/input_data.txt" &
fi 

"$SCRIPT_DIR/coordonator/supervisor" "$SCRIPT_DIR/input_data.txt"

wait 

rm -f "$SCRIPT_DIR/share_mem_file"
rm -f "$SCRIPT_DIR/a_fifo"

echo "ALL DONE"

exit 0;