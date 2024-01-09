#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "svp.h"

// get nummber of dimensions from input
int get_dim(char * string){

    int length = strlen(string);
    int N = 0;
    for (int i = 0; i < length; i++){
        if (string[i] == '['){
            N++;
        }
    }
    
    return N;
}

// transform string from text file to double
double ** decode_input(char * string, int N){

    int length = strlen(string);
    double ** basis = malloc(sizeof(double)*N);
    int i = 0;
    int index = 0;
    int j = 0;

    while (i < length){
        if (string[i] == '['){
            basis[index] = malloc(sizeof(double)*N); 
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
            j = 0;
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
    double * vector = malloc(sizeof(double)*N);

    int i = 1;
    int index = 0;
    vector[index] = strtod(string+i, NULL);
    index++;
    while (i < length- 1){
        while (i < length-1 && string[i] != ' ' && string[i] != ']'){
            i++;
        }
        if (index == N){
            break;
        }
        vector[index] = strtod(string+i, NULL);
        index++;
        i++;
    }
    double ans = 0;
    for (int j = 0; j < N; j++){
        ans+= (vector[j]*vector[j]);
    }
    free(vector);
    return sqrt(ans);

}

int main(int argc, char* argv[]){
    
    FILE * fptr = fopen("test_cases.txt", "r");
    FILE * fptr2 = fopen("test_vector_results.txt", "r");  

    int file_length = atoi(argv[1]);
    double * results = malloc(sizeof(double)*file_length);
    char myString[30000];
    int i = 0;
    while (fgets(myString, 30000, fptr)){
        if (strlen(myString) > 1){
            int N = get_dim(myString);
            double ** basis = decode_input(myString, N);
            results[i] = svp(basis, N);
            i++;
        }
    }
    int index = 0;
    if (fptr2 == NULL){
        printf("Error opening file");
    }
    while (fgets(myString, 30000, fptr2)){
        double ans = get_ans(myString);
        if (index < file_length){
            double ans = get_ans(myString);
            double percen_diff = fabs(ans-results[index])/results[index];
            results[index] = percen_diff;
            index++;
        }
    }

    FILE *file = fopen("test_accuracy_result.txt", "w");

    if (file == NULL){
        perror("Error opening file");
        return -1;

    }
    double mean = 0;
    for (int j = 0; j < file_length; j++){
        mean+= results[j];
        fprintf(file, "%f", results[j]);
        fprintf(file, "\n");
    }
    printf("The average percentage different to results given by fplll was %f%%\n", mean/file_length);
    free(results);
    fclose(file);
    
    // FILE * fptr = fopen("new_test2.txt", "r");
    // FILE * file = fopen("dimensions2.txt", "w");

    // int file_length = atoi(argv[1]);
    // int * dimensions = malloc(sizeof(int)*file_length);

    // char myString[100000];
    // int i = 0;

    // while (fgets(myString, 100000, fptr)){
    //     // int N = get_dim(myString);
    //     dimensions[i] = get_dim(myString);
    //     // printf("%d\n", N);
    //     // double ** basis = decode_input(myString, N);
    //     // results[i] = svp(basis, N);
    //     // results[i] = 0;
    //     i++;
    // }


    fclose(fptr);
    fclose(fptr2);
}