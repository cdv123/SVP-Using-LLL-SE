#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "svp.h"
#include "vector_math_functions.h"


// function to get a lower bound on the norm of the shortest vector
long double get_search_area(long double ** basis, int N){
    long double gamma_n = ((long double)N)/4.0 + 1.0;
    gamma_n = sqrt(gamma_n);
    long double vol_l = 1;
    for (int i = 0; i< N; i++){
        vol_l = vol_l * sqrt(dot_product(basis[i], basis[i], N));
    }
    vol_l = pow(vol_l, (long double)1/(long double)N);
    long double temp = gamma_n*vol_l;
    return square(gamma_n*vol_l);

}

// size reduce for LLL
void size_reduction(long double ** basis, long double ** gs_basis, long double ** mu, int k, int N) {
    for (int j = k-1; j >= 0; j --){
        if (fabs(mu[k][j]) > 0.5){
            long double * temp = scalar_product(basis[j], round(mu[k][j]), N, 0);
            vector_sub(basis[k], temp, N);
            long double old_mu = mu[k][j];
            mu[k][j] = mu[k][j] - round(mu[k][j]);  
            for (int i = 0; i < j; i++){
                mu[k][i] = mu[k][i] - (round(old_mu)*mu[j][i]);
            }
            free(temp);
            // for (int i = 0; i < N; i++){
            //     free(mu[i]);
            //     free(gs_basis[i]);
            // }
            // free(mu);
            // free(gs_basis);
            // gs_info gram_schmidt_info = gram_schmidt(basis, N); 
            // // unpack gram schmidt info from gram schmidt structure
            // mu = gram_schmidt_info.mu;
            // gs_basis = gram_schmidt_info.gs_basis;
            
            // print_1d_arr(basis, N);
        }
    }
    // print_2d_arr(mu, N);
    // print_2d_arr(basis, N);
}

gs_info LLL(long double ** basis, int N) {

    gs_info gram_schmidt_info;

    gram_schmidt_info = gram_schmidt(basis, N); 

    // unpack gram schmidt info from gram schmidt structure
    long double ** mu = gram_schmidt_info.mu;
    long double ** gs_basis = gram_schmidt_info.gs_basis;

    long double delta = 0.75;
    int k = 1;
    while (k < N) {
        size_reduction(basis, gs_basis, mu, k, N);
        // printf("%Lf ", dot_product(gs_basis[k], gs_basis[k], N));
        // printf("%Lf \n", (delta - square(mu[k][k-1])) * (dot_product(gs_basis[k-1], gs_basis[k-1], N)));
        if (dot_product(gs_basis[k], gs_basis[k], N) > ((delta - square(mu[k][k-1]))*(dot_product(gs_basis[k-1], gs_basis[k-1], N)))){
            k+=1;
        }
        else{
            swap_arr(basis[k], basis[k-1], N);
            for (int i = 0; i < N; i++){
                free(mu[i]);
                free(gs_basis[i]);
            }
            free(mu);
            free(gs_basis);
            gram_schmidt_info = gram_schmidt(basis, N); 
            // unpack gram schmidt info from gram schmidt structure
            mu = gram_schmidt_info.mu;
            gs_basis = gram_schmidt_info.gs_basis;
            if (k-1 > 1){
                k-=1;
            }
            else {
                k = 1;
            }
        }
    }

    return gram_schmidt_info;
}

// function to solve svp given the basis vectors
long double svp(long double ** basis, int N){

    gs_info gram_schmidt_info = gram_schmidt(basis, N);
    // gs_info gram_schmidt_info = LLL(basis, N);
    // unpack gram schmidt info from gram schmidt structure
    long double ** mu = gram_schmidt_info.mu;
    long double ** gs_basis = gram_schmidt_info.gs_basis;

    // if (N < 5){
    //     long double ans = sqrt(dot_product(basis[0], basis[0], N));
    //     long double temp;
    //     for (int i = 0; i < N; i++){
    //         temp = sqrt(dot_product(basis[i], basis[i], N));
    //         if (temp < ans){
    //             ans = temp;
    //         }
    //     }
    //     for (int i = 0; i < N; i++){
    //         free(mu[i]);
    //         free(gs_basis[i]);
    //         free(basis[i]);
    //         }
    //     free(mu);
    //     free(gs_basis);                
    //     free(basis);
    //     return ans;
    // }


    // get search area (upper bound where solution can be found)
    long double r_squared = get_search_area(gs_basis, N);
    // long double r_squared = dot_product(basis[0], basis[0], N);

    // initialise variables with 0s and allocate necessary memory
    long double *p = calloc(sizeof(long double), N+1);
    long double *v = calloc(sizeof(long double), N);
    long double *c = calloc(sizeof(long double), N);
    long double *w = calloc(sizeof(long double), N);
    v[0] = 1;
    int k = 0;

    // last_nonzero represents the greatest index k for which v[k] != 0
    int last_nonzero = 0;

    while (1){
        
        // get projection of current solution
        p[k] = p[k+1] + square((v[k]-c[k])) * dot_product(gs_basis[k], gs_basis[k], N);

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

                // free 2D arrays
                for (int i = 0; i < N; i++){
                    free(mu[i]);
                    free(gs_basis[i]);
                    free(basis[i]);
                }
                free(mu);
                free(gs_basis);
                free(basis);
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

void swap(long double * a, long double * b){
    long double temp = *a;
    *a = *b;
    *b = temp;
}

void swap_arr(long double * l1, long double * l2, int N){
    for (int i = 0; i < N; i++){
        swap(&l1[i], &l2[i]);
    }
}