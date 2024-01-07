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
    for (int j = k-1; j >= 0; j --){
        if (fabs(mu[k][j]) > 0.5){
            scalar_product(basis[j], round(mu[k][j]), N, temp);
            vector_sub(basis[k], temp, N);
            double old_mu = mu[k][j];
            mu[k][j] = mu[k][j] - round(mu[k][j]);  
            for (int i = 0; i < j; i++){
                mu[k][i] = mu[k][i] - (round(old_mu)*mu[j][i]);
            }
        }
    }
    // print_2d_arr(mu, N);
    // print_2d_arr(basis, N);
}

void LLL(double ** basis, int N, gs_info gram_schmidt_info) {

    gram_schmidt(basis, N, gram_schmidt_info); 
    double * temp = malloc(sizeof(double)*N);
    // unpack gram schmidt info from gram schmidt structure
    double ** mu = gram_schmidt_info.mu;
    double ** gs_basis = gram_schmidt_info.gs_basis;

    double delta = 0.99;
    int k = 1;
    while (k < N) {
        size_reduction(basis, mu, k, N, temp);
        // printf("%Lf ", dot_product(gs_basis[k], gs_basis[k], N));
        // printf("%Lf \n", (delta - square(mu[k][k-1])) * (dot_product(gs_basis[k-1], gs_basis[k-1], N)));
        if (dot_product(gs_basis[k], gs_basis[k], N) > ((delta - square(mu[k][k-1]))*(dot_product(gs_basis[k-1], gs_basis[k-1], N)))){
            k+=1;
        }
        else{
            swap_arr(basis[k], basis[k-1], N);
            gram_schmidt(basis, N, gram_schmidt_info); 
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

    free(temp);

}

// function to solve svp given the basis vectors
double svp(double ** basis, int N){

    gs_info gram_schmidt_info = {malloc(sizeof(double)*N), malloc(sizeof(double)*N)};

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

    if (N < 5){
        double ans = sqrt(dot_product(basis[0], basis[0], N));
        double temp;
        for (int i = 0; i < N; i++){
            temp = sqrt(dot_product(basis[i], basis[i], N));
            if (temp < ans){
                ans = temp;
            }
        }
        for (int i = 0; i < N; i++){
            free(mu[i]);
            free(gs_basis[i]);
            free(basis[i]);
            }
        free(mu);
        free(gs_basis);                
        free(basis);
        return ans;
    }


    // get search area (upper bound where solution can be found)
    double r_squared = get_search_area(gs_basis, N);

    double A = dot_product(gs_basis[0], gs_basis[0], N);
    // sysconf(_SC_NPROCESSORS_ONLN);
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