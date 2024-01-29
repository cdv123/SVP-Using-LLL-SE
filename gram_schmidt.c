#include "gram_schmidt.h"
#include "vector_math_functions.h"
#include <stdlib.h>
#include <math.h>

/*
 *              Function: gram_schmidt
 * ---------------------------------------------------
 * 
 * Inspired from 
 * Efficient Implementations of Sieving and Enumeration Algorithms for Lattice-Based Cryptography 
 * But modified
 * 
 * Computes the Gram-Schmidt information of a basis
 * Modified version of Gram-Schmidt to improve accuracy
 * 
 * double ** basis - the basis of the lattice
 * int N - the dimension of the basis
 * gram_schmidt_info - contains malloced arrays for Gram-Schmidt basis,
 * Gram-Schmidt coefficients
 *  
 * Note, this does not normalize the basis
 * 
 * return - void as the information is saved in gram_schmidt_info
 * 
 */

void gram_schmidt(double ** basis, int N, gs_info gram_schmidt_info) {
    // extract gram schmidt info
    double ** gs_basis = gram_schmidt_info.gs_basis;
    double ** mu = gram_schmidt_info.mu;
    double * norm = gram_schmidt_info.norm;

    // main loop for gram schmidt
    for (int i = 0; i < N; i++) {
        // initialise mu to 0 and gs_basis to original basis
        for (int j = 0; j < N; j++) {
            mu[i][j] = 0;
            gs_basis[i][j] = basis[i][j];
        }

        for (int k = 0; k < i; k++) {
            // get gram schmidt coefficient
            mu[i][k] = dot_product(basis[i], gs_basis[k], N) / norm[k];

            // use gram schmidt coefficient to get projection
            for (int j = 0; j < N; j++) {
                gs_basis[i][j] -= (mu[i][k] * gs_basis[k][j]);
            }
        }

        // memoize norm for future processing
        norm[i] = dot_product(gs_basis[i], gs_basis[i], N);
        if (norm[i] == 0) {
            return;
        }
        // central diagonal should be 1 to ensure B = muB*
        mu[i][i] = 1;
    }
}
