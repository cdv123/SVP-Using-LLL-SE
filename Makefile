CC = gcc -O3
CFLAGS = -lm -I.
DEPS = ls | grep "\.h"
TEST_FILE = test_cases.txt

all: main.o gram_schmidt.o vector_math_functions.o svp.o
	$(CC) -o runme main.o gram_schmidt.o vector_math_functions.o svp.o $(CFLAGS)

test: test.o svp.o vector_math_functions.o gram_schmidt.o
	$(CC) -o test test.o svp.o gram_schmidt.o vector_math_functions.o $(CFLAGS)
	wc -l < $(TEST_FILE) | xargs ./test

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

clean:
	rm -rf test runme noLLL lll *.o