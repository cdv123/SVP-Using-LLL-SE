#!/bin/bash
echo "" > out
file_name=$1
while read -r line
do
    hyperfine --runs 5 "./runme $line" "./lll $line" "./noLLL $line" "./badBound $line" >> out
done < $file_name