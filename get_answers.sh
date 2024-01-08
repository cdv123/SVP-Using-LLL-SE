#!/bin/bash
file_name=$1
var=0
echo "" > hard_vector_results.txt
while read -r line
do 
    var=$((var+1))
    echo $var
    echo "$line" | fplll -a svp >> hard_vector_results.txt
done < $file_name