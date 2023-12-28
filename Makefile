CC = gcc
CFLAGS = -I. -lm
DEPS = vector_math_functions.h gram_schmidt.h main.h

all: main.o gram_schmidt.o vector_math_functions.o
	$(CC) -o runme main.o gram_schmidt.o vector_math_functions.o $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< -lm $(CFLAGS) 

clean:
	rm -rf runme *.o