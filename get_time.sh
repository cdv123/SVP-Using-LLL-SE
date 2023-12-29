#!/bin/bash
file_name=$1
while read -r line
do
    hyperfine "./runme $line" --export-markdown results.md 
done < $file_name