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

// function to read the input and get the number of dimensions
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

// given the number of dimensions, this function puts the input into a 2d array called basis
void read_input(double ** basis, int argc, char* argv[], int N){
    int index = 0;
    int extra = 0;
    for (int i = 0; i < N; i++){

        // allocate space ith basis vector
        basis[i] = malloc(sizeof(double)*N);
        for (int j = 0; j<N; j++){

            // if [ present, add 1 to index with strtod, to not include [ within strtod
            if (argv[index][0] == '['){
                extra = 1;
            }
            else{
                extra = 0;
            }
            // convert input to double
            basis[i][j] = strtod(argv[index]+extra, NULL);

            // go to next element within basis vector
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

    // get the numebr of dimensions
    int N = get_n(argc-1, argv+1);

    // allocate required memory for 2d array
    double ** basis = malloc(N*sizeof(double));

    // put values from input into 2d array
    read_input(basis, argc-1, argv+1, N);

    double ans;
    ans = svp(basis, N);

    // open file in write mode
    FILE *file = fopen("result.txt", "w");

    if (file == NULL){
        perror("Error opening file");
        return -1;
    }

    // output result to file
    fprintf(file, "%f", ans);

    // close file
    fclose(file);
    
    return 0;
}