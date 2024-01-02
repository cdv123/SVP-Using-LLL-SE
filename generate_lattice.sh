#!/bin/bash 
echo  >> new_tests.txt
latticegen -randseed $1 u $2 30 | tr -d '\n' >> new_tests.txt