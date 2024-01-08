CC = gcc -O3
CFLAGS = -I.
# replace DEPS so that it gets all the header files needed
DEPS = vector_math_functions.h gram_schmidt.h svp.h

all: main.o gram_schmidt.o vector_math_functions.o svp.o
	$(CC) -o runme main.o gram_schmidt.o vector_math_functions.o svp.o -lm $(CFLAGS)

lllOnly: main.o gram_schmidt.o vector_math_functions.o svp_only_LLL.o
	$(CC) -o lll main.o gram_schmidt.o vector_math_functions.o svp_only_LLL.o -lm $(CFLAGS)

noLLL: main.o gram_schmidt.o vector_math_functions.o enum_only.o
	$(CC) -o noLLL main.o gram_schmidt.o vector_math_functions.o enum_only.o -lm $(CFLAGS)

test: test.o svp.o vector_math_functions.o gram_schmidt.o
	$(CC) -o test test.o svp.o gram_schmidt.o vector_math_functions.o -lm $(CFLAGS)

# test: test.o svp.o vector_math_functions.o gram_schmidt.o
# 	$(CC) -o test test.o svp.o gram_schmidt.o vector_math_functions.o -lm $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< -lm $(CFLAGS) 

clean:
	rm -rf test runme noLLL lll *.o