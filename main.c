#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "svp.h"
#include "vector_math_functions.h"



void print_1d_arr(double * arr, int N){
    for (int i = 0; i < N; i++){
        printf("%f ", arr[i]);
    }
    printf("\n");
}

int get_n(int argc, char* argv[]){
    int char_num = 0;
    int arg_num = 0;
    int N = 1;
    for (int i = 0; i < argc; i++){
        // printf("%d\n", i);
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
void print_2d_arr(double ** arr, int N){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            printf("%f ", arr[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int N = get_n(argc-1, argv+1);
    double ** basis = malloc(N*sizeof(double));
    read_input(basis, argc-1, argv+1, N);
    // print_2d_arr(basis, N);
    double ans;

    ans = svp(basis, N);
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