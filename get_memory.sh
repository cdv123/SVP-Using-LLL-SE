#!/bin/bash
echo "" > memory_res.txt
file_name=$1
while read -r line
do
    echo "$line" | memusage -T ./runme "$line" >> memory_res.txt
    echo "$line" | memusage -T ./lll "$line" >> memory_res.txt
    echo "$line" | memusage -T ./noLLL "$line" >> memory_res.txt
done < $file_name