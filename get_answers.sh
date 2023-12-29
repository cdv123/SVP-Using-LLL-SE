#!/bin/bash
file_name=$1
while read -r line
do 
    echo "$line" | fplll -a svp
done < $file_name