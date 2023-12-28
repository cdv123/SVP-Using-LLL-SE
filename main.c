#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "gram_schmidt.h"
#include "main.h"
#include "vector_math_functions.h"

// function to get a lower bound on the norm of the shortest vector
double get_search_area(double ** basis, int N){

    double gamma_n = (double)(N + 1)/4;
    gamma_n = sqrt(gamma_n);

    double vol_l = 1;
    for (int i = 0; i< N; i++){
        vol_l = vol_l * sqrt(dot_product(basis[i], basis[i], N));
    }

    vol_l = pow(vol_l, (double)1/N);
    return square(gamma_n*vol_l);

}

// function to solve svp given the basis vectors
double svp(double ** basis, int N){

    gs_info gram_schmidt_info;

    gram_schmidt_info = gram_schmidt(basis, N); 

    // unpack gram schmidt info from gram schmidt structure
    double ** mu = gram_schmidt_info.mu;
    double ** gs_basis = gram_schmidt_info.gs_basis;

    // get search area (upper bound where solution can be found)
    double r_squared = get_search_area(gs_basis, N);
    // double r_squared = dot_product(basis[0], basis[0], N);

    // initialise variables with 0s and allocate necessary memory
    double *p = calloc(sizeof(double), N+1);
    double *v = calloc(sizeof(double), N);
    double *c = calloc(sizeof(double), N);
    double *w = calloc(sizeof(double), N);
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
                free(basis);
                free(gs_basis);
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

void print_2d_arr(double ** arr, int N){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            printf("%f ", arr[i][j]);
        }
        printf("\n");
    }
}

void print_1d_arr(double * arr, int N){
    for (int i = 0; i < N; i++){
        printf("%f ", arr[i]);
    }
    printf("\n");
}

// size reduce for LLL
void size_reduction(double ** basis, double ** mu, int k, int N) {
    for (int j = k-1; j >= 0; j --){
        if (abs(mu[k][j]) > 0.5){
            vector_sub(basis[k], scalar_product(basis[j], round(mu[k][j]), N, 0), N);
            mu[k][j] = mu[k][j] - round(mu[k][j]);  

            for (int i = 0; i < j; i++){
                mu[k][i] = mu[k][i] - (round(mu[k][j])*mu[j][i]);
            }
            // print_1d_arr(basis, N);
        }
    }
}


void LLL(double ** basis, int N) {

    gs_info gram_schmidt_info;

    gram_schmidt_info = gram_schmidt(basis, N); 

    // unpack gram schmidt info from gram schmidt structure
    double ** mu = gram_schmidt_info.mu;
    double ** gs_basis = gram_schmidt_info.gs_basis;

    double delta = 0.75;
    int k = 1;
    while (k < N) {
        size_reduction(basis, mu, k, N);
        // printf("%f ", dot_product(gs_basis[k], gs_basis[k], N));
        // printf("%f \n", (delta - square(mu[k][k-1])) * (dot_product(gs_basis[k-1], gs_basis[k-1], N)));
        if (dot_product(gs_basis[k], gs_basis[k], N) > ((delta - square(mu[k][k-1]))*(dot_product(gs_basis[k-1], gs_basis[k-1], N)))){
            k+=1;
        }
        else{
            swap_arr(basis[k], basis[k-1], N);
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
}
int get_n(int argc, char* argv[]){
    int char_num = 0;
    int arg_num = 0;
    int N = 1;
    for (int i = 0; i < argc; i++){
        if (argv[i][strlen(argv[i])-1] != ']'){
            N+=1;
        }
        else {
            break;
        }
    }
    return N;
}
void read_input(double ** basis, int argc, char* argv[], int N){
    int index = 0;
    int extra = 0;
    for (int i = 0; i < N; i++){
        basis[i] = malloc(sizeof(double)*N);
        for (int j = 0; j<N; j++){
            if (argv[index][0] == '['){
                extra = 1;
            }
            else{
                extra = 0;
            }
            basis[i][j] = strtod(argv[index]+extra, NULL);
            index+=1;
        }
    }

}

int main(int argc, char *argv[]) {
    int N = get_n(argc-1, argv+1);
    double ** basis = malloc(N*sizeof(double));
    read_input(basis, argc-1, argv+1, N);
    print_2d_arr(basis, N);
    double ans;

    ans = svp(basis, N);
    // LLL(basis, N);
    // print_2d_arr(basis, N);
    // for (int i = 0; i < N; i++){ 
    //     printf("%f\n", sqrt(dot_product(basis[i], basis[i], N)));
    // }
    FILE *file = fopen("result.txt", "w");

    if (file == NULL){
        perror("Error opening file");
        return -1;
    }

    fprintf(file, "%f", ans);
    fclose(file);
    
    return 0;
}