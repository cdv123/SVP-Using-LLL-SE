#!/bin/bash
echo "" > out
file_name=$1
while read -r line
do
    hyperfine "./lll $line" >> out
    hyperfine "./noLLL $line" >> out
    hyperfine "./badBound $line" >> out
    hyperfine "./runme $line" >> out
done < $file_name