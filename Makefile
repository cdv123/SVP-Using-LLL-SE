CC = gcc
CFLAGS = -I.
DEPS = vector_math_functions.h gram_schmidt.h

all: main.o gram_schmidt.o vector_math_functions.o
	$(CC) -o runme main.o gram_schmidt.o vector_math_functions.o $(CFLAGS) -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< -lm $(CFLAGS) 

clean:
	rm -rf runme *.o