#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "svp.h"
#include "vector_math_functions.h"
#define square(a) ((a)*(a))

/*
 *              Function: get_search_area
 * ---------------------------------------------------
 *
 * Inspired from [add citation]
 *
 * Computes the bound for the shortest norm using Minkowski's second theorem
 *
 * double * norm - the norm of the gram schmidt basis
 * int N - the dimension of the basis
 *
 * return - the bound for the shortest norm squared
 *
 */

double get_search_area(double * norm, int N) {
    // use known bound for gamma_n
    double gamma_n = ((double)N)/4.0 + 1.0;
    gamma_n = sqrt(gamma_n);

    // get volume by calculating the product of the norms
    // of the gram schmidt basis
    double vol_l = 1;
    for (int i = 0; i< N; i++) {
        vol_l = vol_l * sqrt(norm[i]);
    }

    vol_l = pow(vol_l, (double)1/(double)N);
    double temp = gamma_n*vol_l;

    // return norm squared to ease computation later
    return square(gamma_n*vol_l);
}

/*
 *              Function: size_reduction
 * ---------------------------------------------------
 *
 * Inspired from https://eprint.iacr.org/2023/261.pdf
 *
 *
 * Performs the size reduction procedure within LLL
 *
 * double ** basis - the basis of the lattice
 * double ** mu - the gram schmidt coefficients of the basis
 * int k - the index of the basis to reduce
 * int N - the dimension of the basis
 * double * temp - a placeholder array to store the 
 * dot product of one of the basis
 *
 * return - void as function is done in place
 *
 */

void size_reduce(double ** basis, double ** mu, int k, int N, double * temp) {
    // define place holder value to store the old value of mu
    double old_mu;

    for (int j = k-1; j >= 0; j --) {
        // if mu[i][j] > 0.5, size reduce by changing the basis,
        // update mu as necessary
        if (fabs(mu[k][j]) > 0.5) {
            // store scalar product in placeholder array
            scalar_product(basis[j], round(mu[k][j]), N, temp);
            vector_sub(basis[k], temp, N);
            old_mu = mu[k][j];
            mu[k][j] = mu[k][j] - round(mu[k][j]);

            for (int i = 0; i < j; i++) {
                mu[k][i] = mu[k][i] - (round(old_mu)*mu[j][i]);
            }
        }
    }
}

/*
 *              Function: LLL
 * ---------------------------------------------------
 *
 * Inspired from https://eprint.iacr.org/2023/261.pdf
 *
 * Reduces the basis using the standard LLL algorithm with delta = 0.99
 *
 * l1, l2 - the arrays to swap
 * N - the number of dimensions of l1 and l2
 *
 * return - void as function is done in place
 *
 * note that l1 and l2 should be of the same size
 *
 */

int LLL(double ** basis, int N, gs_info gram_schmidt_info) {
    gram_schmidt(basis, N, gram_schmidt_info);

    // unpack gram schmidt info from gram schmidt structure
    double ** mu = gram_schmidt_info.mu;
    double ** gs_basis = gram_schmidt_info.gs_basis;
    double * norm = gram_schmidt_info.norm;

    // check if vectors are linearly indpendent, if not,
    // then the input was invalid
    for (int i = 0; i < N; i++) {
        if (norm[i] < 1e-7 && norm[i] > -1e-7) {
            return 0;
        }
    }

    // allocate placeholder array to store scalar product in size reduction
    double * temp = malloc(sizeof(double)*N);

    // value of 0.99 used for delta to increase number of reductions
    double delta = 0.99;
    int k = 1;

    while (k < N) {
        size_reduce(basis, mu, k, N, temp);

        // if Lovasz Condition satisfied, increment k to go to next basis
        if ((norm[k]) > ((delta - square(mu[k][k-1]))*(norm[k-1]))) {
            k+=1;
        } else {
        // if Lovasz Condition not satisfied,
        // swap basis and update gram schmidt info
            swap_arr(basis[k], basis[k-1], N);
            gram_schmidt(basis, N, gram_schmidt_info);

            // unpack gram schmidt info from gram schmidt structure
            mu = gram_schmidt_info.mu;
            gs_basis = gram_schmidt_info.gs_basis;
            norm = gram_schmidt_info.norm;

            if (k > 2) {
                k -= 1;
            } else {
                k = 1;
            }
        }
    }
    free(temp);
    return 1;
}

/*
 *              Function: svp
 * ---------------------------------------------------
 *
 * Used pseudocode from
 * A Survey of Solving SVP Algorithms and Recent Strategies for Solving the SVPChallenge
 * Masaya Yasuda - 2020
 *
 * Solves the shortest vector problem using LLL and a version of the Schnorr Euchnerr algorithm
 *
 * double ** basis - an array of pointers to arrays, corresponding to the basis of the lattice
 * int N - the dimension of the basis.
 *
 * return - the shortest norm of a lattice
 *
 * Note that the basis must be of full rank
 *
 */

double svp(double ** basis, int N) {
    gs_info gram_schmidt_info = {
        malloc(sizeof(double)*N),
        malloc(sizeof(double)*N),
        malloc(sizeof(double)*N)
    };

    // define mu, gs_basis to have N arrays of size N and allocate memory
    for (int i = 0; i < N; i++) {
        gram_schmidt_info.mu[i] = malloc(sizeof(double)*N);
        gram_schmidt_info.gs_basis[i] = malloc(sizeof(double)*N);
    }

    int is_linearly_independent = LLL(basis, N, gram_schmidt_info);

    // inputs should be linearly_independent
    if (is_linearly_independent == 0) {
        return 0;
    }

    // unpack gram schmidt info from gram schmidt structure
    double ** mu = gram_schmidt_info.mu;
    double ** gs_basis = gram_schmidt_info.gs_basis;
    double * norm = gram_schmidt_info.norm;

    // get search area (upper bound where solution can be found)
    double r_squared = get_search_area(norm, N);

    // initialise variables with 0s and allocate necessary memory
    double *p = calloc(sizeof(double), N+1);
    int *v = calloc(sizeof(int), N);
    double *c = calloc(sizeof(double), N);
    double *w = calloc(sizeof(double), N);
    v[0] = 1;
    int k = 0;

    // last_nonzero represents the greatest index k for which v[k] != 0
    int last_nonzero = 0;

    while (1) {
        // get projection of current solution
        p[k] = p[k+1] + square((v[k]-c[k])) * norm[k];

        if (p[k] < r_squared) {
            // if k == 0, better solution has been found, hence update r_squared
            if (k == 0) {
                r_squared = p[k];
            } else {
                k -= 1;
                c[k] = 0;

                for (int i = k+1; i < N; i++) {
                    c[k]-= (mu[i][k]*v[i]);
                }

                w[k] = 1;
                v[k] = round(c[k]);
            }
        } else {
            // increment k to go to next level
            k+=1;

            // if tree has been traversed, return solution
            if (k == N) {
                // free everything before exiting
                free(p);
                free(v);
                free(c);
                free(w);
                free(norm);

                // free 2D arrays
                for (int i = 0; i < N; i++) {
                    free(mu[i]);
                    free(gs_basis[i]);
                    free(basis[i]);
                }

                free(mu);
                free(gs_basis);
                free(basis);

                // return the shortest norm
                return sqrt(r_squared);
            } else {
                if (k >= last_nonzero)  {
                    last_nonzero = k;
                    v[k]+=1;
                } else {
                    if (v[k] > c[k]) {
                        v[k] -= w[k];
                    } else {
                        v[k] += w[k];
                    }
                    w[k]+=1;
                }
            }
        }
    }
}

/*
 *              Function: swap
 * ---------------------------------------------------
 *
 * Swaps two double values using pointers
 *
 * a, b - the pointers to the double values
 *
 * return - void as function is done in place
 *
 */

void swap(double * a, double * b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

/*
 *              Function: swap_array
 * ---------------------------------------------------
 *
 * Swaps two arrays of doubles
 *
 * l1, l2 - the arrays to swap
 * N - the number of dimensions of l1 and l2
 *
 * return - void as function is done in place
 *
 * note that l1 and l2 should be of the same size
 * 
 */

void swap_arr(double * l1, double * l2, int N) {
    for (int i = 0; i < N; i++) {
        swap(&l1[i], &l2[i]);
    }
}
