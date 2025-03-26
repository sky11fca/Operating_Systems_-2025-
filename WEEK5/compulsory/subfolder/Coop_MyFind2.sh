#! /bin/sh
 

if [ $# -ne 1 ]
then 
	echo "USAGE: $0 <Given directory from script 1>" >&2
	exit 1 
fi 

dir="$1"
bash_script_counter=0

function process_file(){
	local file="$1"

	file_type=$(file -b "$file")

	if [[ "$file_type" == *"Bourne-Again shell script"* ]]
	then 
		sha_sum=$(sha256sum "$file" | tr -s " " | cut -d " " -f1)
		echo "$sha_sum -> $file"
		((bash_script_counter++))
	else 
		newline_counter=$(grep -c $ "$file" || echo 0)
		echo "$file $newline_counter" >&2 
	fi
}


function traverse(){
	local current_dir="$1"

	for item in "$current_dir"/*
	do 
		if [ -d "$item" ]
		then 
			traverse "$item"
		elif [ -f "$item" ]
		then 
			process_file "$item"
		fi 
	done 

}

traverse "$dir"

echo "Total bash script found: $bash_script_counter"

if [ $bash_script_counter -gt 0 ]
then 
	exit 0
else 
	exit -1 
fi 
