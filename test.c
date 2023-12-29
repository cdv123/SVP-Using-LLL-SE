#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "svp.h"

// get nummber of dimensions from input
int get_dim(char * string){
    int length = strlen(string);
    int N = -1;
    for (int i = 0; i < length; i++){
        if (string[i] == '['){
            N++;
        }
    }
    return N;
}

// void print_2d_arr(double ** arr, int N){
//     for (int i = 0; i < N; i++){
//         for (int j = 0; j < N; j++){
//             printf("%f ", arr[i][j]);
//         }
//         printf("\n");
//     }
// }

double ** decode_input(char * string, int N){

    int length = strlen(string);
    double ** basis = malloc(sizeof(double)*N);
    int i = 1;
    int index = 0;
    while (i < length){
        if (string[i] == '['){
            basis[index] = malloc(sizeof(double)*N); 
            int j = 0;
            basis[index][j] = strtod(string+i+1, NULL);
            j++;
            while (i < length && (string[i] != ' ' && string[i] != ']')){
                i++;
            }
            while (i < length && string[i] != ']'){
                if (string[i] == ' '){
                    i++;
                }
                basis[index][j] = strtod(string+i, NULL);
                j++;
                while (i < length && (string[i] != ' ' && string[i] != ']')){
                    i++;
                }
            }
            index++;
        }
        i++;
    }

    // print_2d_arr(basis, N);
    return basis;
}

double get_ans(char * string){

    int length = strlen(string);
    int N = 1;

    for (int i = 0; i < length; i++){
        if (string[i] == ' '){
            N++;
        }
    }

    double * vector = malloc(N*sizeof(double));

    int i = 1;
    int index = 0;
    vector[0] = strtod(string+1, NULL);
    index++;
    while (i < length -1){
        while (i < length-1 && string[i] != ' ' && string[i] != '['){
            i++;
        }
        vector[index] = strtod(string+i+1, NULL);
        index++;
        i++;
    }

    double ans = 0;
    for (int i = 0; i < N; i++){
        // printf("%f\n", vector[i]);
        ans+= (vector[i]*vector[i]);
    }
    // printf("%f\n", ans);
    return sqrt(ans);

}

int main(){
    
    FILE * fptr = fopen("tests.txt", "r");
    FILE * fptr2 = fopen("test_results.txt", "r");  
    double results[7];
    char myString[10000];
    int i = 0;

    while (fgets(myString, 10000, fptr)){
        int N = get_dim(myString);
        if (N > 0){
            double ** basis = decode_input(myString, N);
            results[i] = svp(basis, N);
            i++;
            // free(basis);
        }
    }


    int index = 0;
    // if (fptr2 == NULL){
    //     printf("h");
    // }
    while (fgets(myString, 10000, fptr2)){
        if (index < i){
            double ans = get_ans(myString);
            double percen_diff = fabs(ans-results[index])/results[index];
            printf("%f %f\n", ans, results[index]);
            printf("%f\n", percen_diff);
            index++;
        }

    }

    fclose(fptr);
    fclose(fptr2);
}