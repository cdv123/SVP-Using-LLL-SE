#include "vector_math_functions.h"
#include <stdlib.h>
#include <math.h>
#define square(a) ((a)*(a))

// function which takes in two arrays of doubles and outputs the dot product between them
double dot_product(double * x, double * y, int N){

    double res = 0;

    for (int i = 0; i < N; i++){
        res+= x[i] * y[i];
    }

    return res;
}

// function to return array multiplied by scalar product, can be in place or not
void scalar_product(double *x, double scalar, int N, double * save_to){
    for (int i = 0; i < N; i++){
        save_to[i] = x[i] * scalar;
    }
}

// subtract v2 from v1 in place
void vector_sub(double * v1, double * v2, int N){
    for (int i = 0; i < N; i++){
        v1[i] = v1[i] - v2[i];
    }
}