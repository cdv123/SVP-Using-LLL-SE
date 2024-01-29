#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "svp.h"

/*
 *              Function: get_dim
 * ---------------------------------------------------
 * 
 * Gets the number of dimensions from string representation of basis
 * 
 * char * string - string representation of basis
 * 
 * return - int N, number of dimensions of the basis
 * 
 */

int get_dim(char * string) {
    int length = strlen(string);
    int N = 0;
    for (int i = 0; i < length; i++) {
        // use number of square brackets to determine the number of dimensions
        if (string[i] == '[') {
            N++;
        }
    }

    return N;
}

/*
 *              Function: decode_input
 * ---------------------------------------------------
 * 
 * Converts string representation of basis into an array of arrays of doubles
 * 
 * char * string - string representation of basis
 * int N - number of dimensions of basis
 * 
 * return - double ** basis, an array of arrays of doubles storing the basis
 * 
 */

double ** decode_input(char * string, int N) {
    int length = strlen(string);
    double ** basis = malloc(sizeof(double)*N);
    int i = 0;
    int index = 0;
    int j = 0;

    while (i < length) {
        if (string[i] == '[') {
            basis[index] = malloc(sizeof(double)*N);
            basis[index][j] = strtod(string+i+1, NULL);
            j++;
            while (i < length && (string[i] != ' ' && string[i] != ']')) {
                i++;
            }
            while (i < length && string[i] != ']') {
                if (string[i] == ' ') {
                    i++;
                }
                basis[index][j] = strtod(string+i, NULL);
                j++;
                while (i < length && (string[i] != ' ' && string[i] != ']')) {
                    i++;
                }
            }
            index++;
            j = 0;
        }
        i++;
    }

    return basis;
}

/*
 *              Function: get_ans
 * ---------------------------------------------------
 * 
 * Computes euclidean norm of vector obtained from string representation of vector
 * 
 * char * string - string representation of vector
 * 
 * return - double - euclidean norm of vector
 * 
 */

double get_ans(char * string) {
    // get number of dimensions
    int length = strlen(string);
    int N = 1;

    for (int i = 0; i < length; i++) {
        if (string[i] == ' ') {
            N++;
        }
    }
    double * vector = malloc(sizeof(double)*N);

    int i = 1;
    int index = 0;
    vector[index] = strtod(string+i, NULL);
    index++;

    while (i < length- 1) {
        while (i < length-1 && string[i] != ' ' && string[i] != ']') {
            i++;
        }

        if (index == N) {
            break;
        }

        // convert to double
        vector[index] = strtod(string+i, NULL);
        index++;
        i++;
    }

    // compute dot product
    double ans = 0;

    for (int i = 0; i < N; i++) {
        ans+= vector[i]*vector[i];
    }

    free(vector);

    // return norm
    return sqrt(ans);
}

int main(int argc, char* argv[]) {
    // open files with test cases
    FILE * fptr = fopen("test_cases.txt", "r");
    FILE * fptr2 = fopen("test_vector_results.txt", "r");

    // get length of file
    int file_length = atoi(argv[1]);
    double * results = malloc(sizeof(double)*file_length);

    /* allocate large enough size to read each line 
    (tested with 32bit inputs up to 40 dimensions) */
    char myString[30000];
    int i = 0;

    while (fgets(myString, 30000, fptr)) {
        // get number of dimensions from line
        int N = get_dim(myString);

        // store into basis
        double ** basis = decode_input(myString, N);

        // copmute solution
        results[i] = svp(basis, N);
        i++;
    }

    int index = 0;
    if (fptr2 == NULL) {
        printf("Error opening file");
    }

    while (fgets(myString, 30000, fptr2)) {
        // compute norm of vector from answer file
        double ans = get_ans(myString);

        // get percent difference with computed solution
        double percen_diff = fabs(ans-results[index])/results[index];
        results[index] = percen_diff;
        index++;
    }

    FILE *file = fopen("test_accuracy_result.txt", "w");

    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    double mean = 0;
    for (int j = 0; j < file_length; j++) {
        // keep track of accuracy
        mean+= results[j];

        // write accuracy to file
        fprintf(file, "%f\n", results[j]);
    }

    printf("Mean percent diff to fplll's answers: %f%%\n", mean/file_length);
    free(results);

    // close files before exiting
    fclose(file);
    fclose(fptr);
    fclose(fptr2);
}
