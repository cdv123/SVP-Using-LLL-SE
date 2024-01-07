#!/bin/bash
file_name=$1
echo "" > uniform_vector_results.txt
while read -r line
do 
    echo "$line" | fplll -a svp >> uniform_vector_results.txt
done < $file_name