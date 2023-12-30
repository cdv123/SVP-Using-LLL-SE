#!/bin/bash 
echo  >> tests.txt
latticegen -randseed $1 u $2 20 | tr -d '\n' >> tests.txt