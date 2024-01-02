#include "gram_schmidt.h"
#include "vector_math_functions.h"
#include <stdlib.h>
#include <math.h>

// Function to get Gram Schmidt information from set of basis vectors
void gram_schmidt(double ** basis, int N, gs_info gram_schmidt_info){
    
    double ** gs_basis = gram_schmidt_info.gs_basis;
    double ** mu = gram_schmidt_info.mu;

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

}