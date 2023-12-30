#!/bin/bash
file_name=$1
echo "" > test_results.txt
while read -r line
do 
    echo "$line" | fplll -a svp >> test_results.txt
done < $file_name