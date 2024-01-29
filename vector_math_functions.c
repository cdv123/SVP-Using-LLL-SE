#include "vector_math_functions.h"
#include <stdlib.h>
#include <math.h>
#define square(a) ((a)*(a))

/*
 *              Function: dot_product
 * ---------------------------------------------------
 * 
 * Computes the dot product between two vectors
 * 
 * x, y - vectors to compute the dot product of
 * N - the number of dimensions of x and y
 * 
 * return - void as function is done in place (in v1)
 * 
 * note that x and y should be of the same size
 * 
 */

double dot_product(double * x, double * y, int N) {
    double res = 0;

    for (int i = 0; i < N; i++) {
        res+= x[i] * y[i];
    }

    return res;
}

/*
 *              Function: scalar_product
 * ---------------------------------------------------
 * 
 * Multiplies a vector by a scalar multiple
 * 
 * x - vector to multiply
 * scalar - scalar multiple to multiply by
 * N - number of dimensions of x and save_to
 * save_to - array to save to 
 * 
 * Note that x and save_to should be of the same size
 * 
 */

void scalar_product(double *x, double scalar, int N, double * save_to) {
    for (int i = 0; i < N; i++) {
        save_to[i] = x[i] * scalar;
    }
}

/*
 *              Function: vector_sub
 * ---------------------------------------------------
 * 
 * Subtracts two vectors
 * 
 * v1 - vector to subtract from
 * v2 - values to subtract from v1
 * N - number of dimensions of v1 and v2
 * 
 * Note that v1 and v2 should be of the same dimension
 * 
 * return - void as function is done in place (in v1)
 * 
 */

void vector_sub(double * v1, double * v2, int N) {
    for (int i = 0; i < N; i++) {
        v1[i] = v1[i] - v2[i];
    }
}
