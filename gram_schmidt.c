#include "gram_schmidt.h"
#include "vector_math_functions.h"
#include <stdlib.h>
#include <math.h>

// Function to get Gram Schmidt information from set of basis vectors
gs_info gram_schmidt(long double ** basis, int N){

    // initialise new basis and allocate necessary memory
    long double ** gs_basis = malloc(sizeof(long double*)*N);

    // initialise the gram schmidt coefficients and allocate necessary memory
    long double ** mu = malloc(sizeof(long double)*N);
    
    // define mu and gs_basis to have N arrays of size N and allocate necessary memory
    for (int i = 0; i < N; i++){
        mu[i] = malloc(sizeof(long double)*N);
        gs_basis[i] = malloc(sizeof(long double)*N);
    }

    // main loop for gram schmidt
    for (int i = 0; i < N; i++){

        // initialise mu to 0 and gs_basis to original basis
        for (int j = 0; j < N; j++){
            mu[i][j] = 0;
            gs_basis[i][j] = basis[i][j];
        }

        for (int k = 0; k < i; k++){

            // get gram schmidt coefficient
            mu[i][k] = dot_product(basis[i], gs_basis[k], N) / dot_product(gs_basis[k], gs_basis[k], N);

            // use gram schmidt coefficient to get projection
            for (int j = 0; j < N; j++){
                gs_basis[i][j] -= (mu[i][k] * gs_basis[k][j]);
            }
        }

    }

    // set diagonal to 1s so that B = B* x mu
    for (int i = 0; i < N; i++){
        mu[i][i] = 1;
    }


    // define structure to return both structures
    gs_info return_val = {mu, gs_basis};

    return return_val;
}