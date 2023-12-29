#!/bin/bash
file_name=$1
while read -r line
do
    hyperfine "./runme $line" >> out
    cat out
done < $file_name