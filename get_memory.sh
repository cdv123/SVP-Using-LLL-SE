#!/bin/bash
echo "" > memory_res.txt
file_name=$1
while read -r line
do
    memusage -T ./runme $line |&grep -E -o 'heap total: [0-9]+' >> out 2>&1
    memusage -T ./noLLL $line |&grep -E -o 'heap total: [0-9]+' >> out 2>&1
done < $file_name