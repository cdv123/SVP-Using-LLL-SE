#include <stdio.h>
#include <math.h>
#include <stdlib.h>



double dot_product(double * x, double * y, int N){
    double res = 0;

    for (int i = 0; i < N; i++){
        res+= x[i] * y[i];
    }

    return res;
}

struct return_schmidt {
    double ** mu;
    double ** gs_basis;
};

struct return_schmidt gram_schmidt(double ** basis, int N){

    double ** gs_basis = malloc(sizeof(double*)*N);
    double ** mu = malloc(sizeof(double)*N);
    
    for (int i = 0; i < N; i++){
        mu[i] = malloc(sizeof(double)*N);
        gs_basis[i] = malloc(sizeof(double)*N);
    }


    for (int i = 0; i < N; i++){

        for (int j = 0; j < N; j++){
            mu[i][j] = 0;
            gs_basis[i][j] = basis[i][j];
        }

        for (int k = 0; k < i; k++){
            mu[i][k] = dot_product(basis[i], gs_basis[k], N) / dot_product(gs_basis[k], gs_basis[k], N);
            for (int j = 0; j < N; j++){
                gs_basis[i][k] -= mu[i][k] * gs_basis[k][j];
            }
        }

    }

    for (int i = 0; i < N; i++){
        mu[i][i] = 1;
    }

    struct return_schmidt return_val = {mu, gs_basis};

    return return_val;
}

double* scalar_product(double *x, double scalar, int N){

    double *y = malloc(sizeof(double)*N);

    for (int i = 0; i < N; i++){
        y[i] = x[i] * scalar;
    }

    return y;
}

double get_search_area(double ** basis, int N){

    double gamma_n = (N + 1)/4;
    gamma_n = sqrt(gamma_n);

    double vol_l = 1;
    for (int i = 0; i< N; i++){
        vol_l = vol_l * sqrt(dot_product(basis[i], basis[i], N));
    }

    return square(gamma_n*vol_l);

}

double square(double x){
    return x * x;
}

double svp(double ** basis, int N){

    struct return_schmidt gram_schmidt_info;

    gram_schmidt_info = gram_schmidt(basis, N); 


    double ** gs_basis = gram_schmidt_info.gs_basis;
    double ** mu = gram_schmidt_info.mu;
    double r_squared = get_search_area(gs_basis, N);

    double *p = calloc(sizeof(double), N+1);
    double *v = calloc(sizeof(double), N);
    double *c = calloc(sizeof(double), N);
    double *w = calloc(sizeof(double), N);
    v[0] = 1;
    int k = 0;
    int last_nonzero = 0;

    while (1){

        p[k] = p[k+1] + square((v[k]-c[k]))/dot_product(gs_basis[k], gs_basis[k], N);

        if (p[k] < r_squared){
            if (k == 1){
                r_squared = p[k];
            }
            else {
                k-=1;
                for (int i = 0; i<k+1; i++){
                    c[k]-=mu[i][k]*v[i];
                }
                w[k] = 1;
                v[k] = round(c[k]);
            }
        }
        else{
            k+=1;
            if (k == N){
                return sqrt(r_squared);
            }
            else{
                if (k >= last_nonzero)  {
                    last_nonzero = k;
                    v[k]+=1;
                }
            }

        }


    }
}

int main() {
    double basis[3][3] = {{2,0,1}, {0,2,0}, {1,0,2}};
    double ans;
    ans = svp(basis, 3);
    printf("Value of answer: %f\n", ans);
}