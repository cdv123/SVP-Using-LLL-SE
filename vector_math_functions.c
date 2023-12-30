#include "vector_math_functions.h"
#include <stdlib.h>
#include <math.h>

// function which takes in two arrays of long doubles and outputs the dot product between them
long double dot_product(long double * x, long double * y, int N){

    long double res = 0;

    for (int i = 0; i < N; i++){
        res+= x[i] * y[i];
    }

    return res;
}

// function to return array multiplied by scalar product, can be in place or not
long double* scalar_product(long double *x, long double scalar, int N, int in_place){

    if (in_place){
        for (int i = 0; i < N; i++){
            x[i] = x[i] * scalar;
        }

        return x;
    }

    else{
        long double *y = malloc(sizeof(long double)*N);
        for (int i = 0; i < N; i++){
            y[i] = x[i] * scalar;
        }

        return y;
    }

}

long double square(long double x){
    return (x * x);
}

// subtract v2 from v1 in place
void vector_sub(long double * v1, long double * v2, int N){
    for (int i = 0; i < N; i++){
        v1[i] = v1[i] - v2[i];
    }
}