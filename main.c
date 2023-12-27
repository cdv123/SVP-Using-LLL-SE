#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "gram_schmidt.h"
#include "vector_math_functions.h"

// #include "vector_math_functions.c"

double get_search_area(double ** basis, int N){

    double gamma_n = (double)(N + 1)/4;
    gamma_n = sqrt(gamma_n);

    double vol_l = 1;
    for (int i = 0; i< N; i++){
        vol_l = vol_l * sqrt(dot_product(basis[i], basis[i], N));
    }

    vol_l = pow(vol_l, 1/N);
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
    // double r_squared = get_search_area(gs_basis, N);
    double r_squared = dot_product(basis[0], basis[0], N);

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


int main() {
    // printf("%d\n", argc);
    // for (int i = 1; i < argc; i++){
    //     printf("%s\n", argv[i]);
    // }
    int N = 10;
    // double init_basis[10][10] = {{14800322531141549294, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {17796564942578567201, 0, 1, 0, 0, 0, 0, 0, 0, 0}, {7301743052718098431, 0, 0, 1, 0, 0, 0, 0, 0, 0}, {5792622719448160275, 0, 0, 0, 1, 0, 0, 0, 0, 0}, {13825670105792957044, 0, 0, 0, 0, 1, 0, 0, 0, 0}, {7338474235679864976, 0, 0, 0, 0, 0, 1, 0, 0, 0}, {6466088670324559620, 0, 0, 0, 0, 0, 0, 1, 0, 0}, {5315957138953616152, 0, 0, 0, 0, 0, 0, 0, 1, 0}, {7038974009087367626, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {13662771198155157402, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    double init_basis[10][10] = {{3462528728, 444363041, 953498107, 3019162, 293072558, 454157756, 1950819278, 2174448309, 2956307025, 2050440410}, {1676991435, 2190045933, 3360605833, 3782131535, 1076691369, 1286267085, 3124954673, 1356441824, 1675223571, 3617515077}, {3467837865, 2727897666, 2512785299, 3013662465, 108820214, 2883385007, 1068246967, 798890469, 2874893319, 279879480}, {3036791821, 956443849, 199347373, 1643597530, 1622084324, 471638746, 1055553070, 2020155791, 1772672903, 179014643}, {3726309520, 3416039174, 816778328, 4178510478, 1052560161, 2365858231, 3810589638, 1478643591, 3537405930, 1196123656}, {2101281032, 731853286, 35290828, 1463099681, 62123405, 1905949073, 3166373256, 2899371508, 674265594, 3891491225}, {2924313520, 4018395377, 1444789986, 2465339761, 1288445553, 3964167277, 3710424026, 2819673876, 103363586, 3597674584}, {3237227071, 578279073, 119471827, 325475776, 1510452858, 210663454, 2633248087, 2140229824, 153673582, 2811110910}, {1566590271, 1264942241, 750770120, 1308334483, 1808072791, 1483343336, 4170849932, 3261787946, 1391377726, 2865208703}, {1950175842, 47002310, 377036287, 1445470496, 1706137745, 2166273829, 1896680823, 2831856618, 2825133614, 3610138421}};
    // double init_basis[2][2] = {{2709894480, 2051880200}, {3409234876, 724643694}};
    // double init_basis[3][3] = {{1,-1,3}, {1,0,5}, {1,2,6}};
    // double init_basis[2][2] = {{18704, 43309}, {64726, 54865}};
    double ** basis = malloc(sizeof(double)*N);
    double ans;
    for (int i = 0; i < N; i++){
        basis[i] = malloc(sizeof(double)*N);
        for (int j = 0; j < N; j++){
            basis[i][j] = init_basis[i][j];
        }
    }

    ans = svp(basis, N);
    // LLL(basis, N);
    // print_2d_arr(basis, N);
    // for (int i = 0; i < N; i++){ 
    //     printf("%f\n", sqrt(dot_product(basis[i], basis[i], N)));
    // }
    // for (int i = 0; i < N; i++){
    //     for (int j = 0; j < N; j++){
    //         printf("%f ", basis[i][j]);
    //     }
    //     printf("\n");
    // }
    printf("Value of answer: %f\n", ans);
}