#!/bin/bash 
echo >> uniform_test_cases.txt
latticegen -randseed $1 u $2 32 | tr -d '\n' >> uniform_test_cases.txt
