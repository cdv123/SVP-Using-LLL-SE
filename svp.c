#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "svp.h"
#include "vector_math_functions.h"
#define square(a) ((a)*(a))

// function to get a lower bound on the norm of the shortest vector
double get_search_area(double ** basis, int N){
    double gamma_n = ((double)N)/4.0 + 1.0;
    gamma_n = sqrt(gamma_n);
    double vol_l = 1;
    for (int i = 0; i< N; i++){
        vol_l = vol_l * sqrt(dot_product(basis[i], basis[i], N));
    }
    vol_l = pow(vol_l, (double)1/(double)N);
    double temp = gamma_n*vol_l;
    return square(gamma_n*vol_l);
}

// size reduce for LLL
void size_reduction(double ** basis, double ** mu, int k, int N, double * temp) {
    double old_mu;
    for (int j = k-1; j >= 0; j --){
        if (fabs(mu[k][j]) > 0.5){
            scalar_product(basis[j], round(mu[k][j]), N, temp);
            vector_sub(basis[k], temp, N);
            old_mu = mu[k][j];
            mu[k][j] = mu[k][j] - round(mu[k][j]);  
            for (int i = 0; i < j; i++){
                mu[k][i] = mu[k][i] - (round(old_mu)*mu[j][i]);
            }
        }
    }
}

void LLL(double ** basis, int N, gs_info gram_schmidt_info) {

    gram_schmidt(basis, N, gram_schmidt_info); 
    double * temp = malloc(sizeof(double)*N);

    // unpack gram schmidt info from gram schmidt structure
    double ** mu = gram_schmidt_info.mu;
    double ** gs_basis = gram_schmidt_info.gs_basis;
    double * norm = gram_schmidt_info.norm;

    double delta = 0.99;
    int k = 1;
    while (k < N) {
        size_reduction(basis, mu, k, N, temp);
        if ((norm[k]) > ((delta - square(mu[k][k-1]))*(norm[k-1]))){
            k+=1;
        }
        else{
            swap_arr(basis[k], basis[k-1], N);
            gram_schmidt(basis, N, gram_schmidt_info); 

            // unpack gram schmidt info from gram schmidt structure
            mu = gram_schmidt_info.mu;
            gs_basis = gram_schmidt_info.gs_basis;
            norm = gram_schmidt_info.norm;
            if (k > 2){
                k-=1;
            }
            else {
                k = 1;
            }
        }
    }

    free(temp);

}

// function to solve svp given the basis vectors
double svp(double ** basis, int N){

    gs_info gram_schmidt_info = {malloc(sizeof(double)*N), malloc(sizeof(double)*N), malloc(sizeof(double)*N)};

    // define mu and gs_basis to have N arrays of size N and allocate necessary memory
    for (int i = 0; i < N; i++){
        gram_schmidt_info.mu[i] = malloc(sizeof(double)*N);
        gram_schmidt_info.gs_basis[i] = malloc(sizeof(double)*N);
    }
    // gs_info gram_schmidt_info = gram_schmidt(basis, N);
    LLL(basis, N, gram_schmidt_info);
    // unpack gram schmidt info from gram schmidt structure
    double ** mu = gram_schmidt_info.mu;
    double ** gs_basis = gram_schmidt_info.gs_basis;
    double * norm = gram_schmidt_info.norm;

    // get search area (upper bound where solution can be found)
    double r_squared = get_search_area(gs_basis, N);

    // initialise variables with 0s and allocate necessary memory
    double *p = calloc(sizeof(double), N+1);
    int *v = calloc(sizeof(int), N);
    double *c = calloc(sizeof(double), N);
    double *w = calloc(sizeof(double), N);
    v[0] = 1;
    int k = 0;

    // last_nonzero represents the greatest index k for which v[k] != 0
    int last_nonzero = 0;

    while (1){
        
        // get projection of current solution
        p[k] = p[k+1] + square((v[k]-c[k])) * norm[k];

        if (p[k] < r_squared){

            if (k == 0){
                r_squared = p[k];
            }
            else {
                k-=1;
                c[k] = 0;
                for (int i = k+1; i<N; i++){
                    c[k]-= (mu[i][k]*v[i]);
                }
                w[k] = 1;
                v[k] = round(c[k]);
            }
        }
        else{
            // increment k to go to next level
            k+=1;
            // if tree has been traversed, return solution
            if (k == N){

                // free everything before exiting
                free(p);
                free(v);
                free(c);
                free(w);
                free(norm);

                // free 2D arrays
                for (int i = 0; i < N; i++){
                    free(mu[i]);
                    free(gs_basis[i]);
                    free(basis[i]);
                }
                free(mu);
                free(gs_basis);
                free(basis);

                // return the shortest norm
                return sqrt(r_squared);
            }
            else {
                if (k >= last_nonzero)  {
                    last_nonzero = k;
                    v[k]+=1;
                }
                else{
                    if (v[k] > c[k]){
                        v[k]-=w[k];
                    }
                    else{
                        v[k]+=w[k];
                    }
                    w[k]+=1;
                }
            }

        }


    }
}

void swap(double * a, double * b){
    double temp = *a;
    *a = *b;
    *b = temp;
}

void swap_arr(double * l1, double * l2, int N){
    for (int i = 0; i < N; i++){
        swap(&l1[i], &l2[i]);
    }
}