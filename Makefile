CC = gcc
CFLAGS = -I.
DEPS = vector_math_functions.h gram_schmidt.h svp.h

all: main.o gram_schmidt.o vector_math_functions.o svp.o
	$(CC) -o runme main.o gram_schmidt.o vector_math_functions.o svp.o -lm $(CFLAGS)

test: test.o svp.o vector_math_functions.o gram_schmidt.o
	$(CC) -o test test.o svp.o gram_schmidt.o vector_math_functions.o -lm $(CFLAGS)
	./test

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< -lm $(CFLAGS) 

clean:
	rm -rf test runme *.o