#!/bin/bash
from=$1
send_to=$2
echo "" > $send_to
while read -r line
do
    hyperfine --runs 10 "./noLLL $line" >> $send_to
    hyperfine --runs 10 "./runme $line" >> $send_to
done < $from