CC = gcc -O3
CFLAGS = -I.
DEPS = vector_math_functions.h gram_schmidt.h svp.h

all: main.o gram_schmidt.o vector_math_functions.o svp.o
	$(CC) -o runme main.o gram_schmidt.o vector_math_functions.o svp.o -lm $(CFLAGS)

lllOnly: main.o gram_schmidt.o vector_math_functions.o svp_only_LLL.o
	$(CC) -o lll main.o gram_schmidt.o vector_math_functions.o svp_only_LLL.o -lm $(CFLAGS)

badBound: main.o gram_schmidt.o vector_math_functions.o svp_with_worse_bound.o
	$(CC) -o badBound main.o gram_schmidt.o vector_math_functions.o svp_with_worse_bound.o -lm $(CFLAGS)

noLLL: main.o gram_schmidt.o vector_math_functions.o svp_without_LLL.o
	$(CC) -o noLLL main.o gram_schmidt.o vector_math_functions.o svp_without_LLL.o -lm $(CFLAGS)

test: test.o svp_only_LLL.o vector_math_functions.o gram_schmidt.o
	$(CC) -o test test.o svp_only_LLL.o gram_schmidt.o vector_math_functions.o -lm $(CFLAGS)

# test: test.o svp.o vector_math_functions.o gram_schmidt.o
# 	$(CC) -o test test.o svp.o gram_schmidt.o vector_math_functions.o -lm $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< -lm $(CFLAGS) 

clean:
	rm -rf test runme noLLL lll *.o