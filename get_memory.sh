#!/bin/bash
echo "" > memory_res.txt
file_name=$1
while read -r line
do
    echo "$line" | memusage -T ./runme "$line" |& grep -E -o 'heap total: [0-9]+' >> memory_res.txt
    echo "$line" | memusage -T ./runme "$line" |& grep -E -o 'heap total: [0-9]+' >> memory_res.txt
done < $file_name