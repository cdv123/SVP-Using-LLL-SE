#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "svp.h"
#include "vector_math_functions.h"
#include <ctype.h>


/*
 *              Function: read_input
 * ---------------------------------------------------
 *
 * Converts the string arguments (argv) into a basis
 * 
 * double ** basis - empty array of arrays to store the basis in
 * int argc - the number of arguments when calling executable 
 * char* argv[] - the arguments when calling the executable 
 * int N - the dimension of the basis
 *
 * return - void as basis saved onto the argument double ** basis
 *
 */

void read_input(double ** basis, int argc, char* argv[], int N) {
    int index = 0;
    int extra = 0;
    for (int i = 0; i < N; i++) {
        // allocate space ith basis vector
        basis[i] = malloc(sizeof(double)*N);
        for (int j = 0; j < N; j++) {
            // if [ present, add 1 to index with strtod,
            // to not include [ when calling strtod
            if (argv[index][0] == '[') {
                extra = 1;
            } else {
                extra = 0;
            }

            // convert input to double
            basis[i][j] = strtod(argv[index]+extra, NULL);

            // go to next element within basis vector
            index+=1;
        }
    }
}

/*
 *              Function: is_numeric
 * ---------------------------------------------------
 *
 * Checks if a character is a number, - or .
 *
 * char c - character to check
 *
 * return - 1 if numeric, 0 otherwise
 *
 *
 */
int is_numeric(char c) {
    if (!isdigit(c) && c != '.' && c != '-') {
        return 0;
    }

    return 1;
}

/*
 *              Function: check_valid
 * ---------------------------------------------------
 *
 * Checks whether input is in the correct format
 *
 * int argc - the number of arguments when calling the executable
 * char *argv[] - the arguments when calling the executable
 *
 * return - 0 if invalid, otherwise returns the dimension of the basis
 *
 */

const char* check_valid(int argc, char *argv[]) {
    int n1 = 0;
    int n2 = 0;
    int next_valid = 0;
    int N = sqrt(argc);
    int dim_counter = 0;
    static char buffer[81];

    // iterate through all characters and ensure correct format
    for (int i = 0; i < argc; i++) {
        dim_counter++;
        for (int j = 0; j < strlen(argv[i]); j++) {
            switch (argv[i][j]) {
            // squared brackets must be at end or beginning of vector
            case '[':
                n1++;
                if (j == strlen(argv[i])-1 || (!is_numeric(argv[i][j+1]))) {
                    snprintf(buffer, sizeof(buffer), "[ character in "
                    "wrong location");
                    return buffer;
                }
                break;
            case ']':
                n2++;
                if (j == 0 || (!is_numeric(argv[i][j-1]))) {
                    snprintf(buffer, sizeof(buffer), "] character "
                    "in wrong location");
                    return buffer;
                } else if (dim_counter != N) {
                    snprintf(buffer, sizeof(buffer), "Number of "
                    "dimensions does not match number of vectors");
                    return buffer;
                }
                dim_counter = 0;
                break;
            // decimal points should come before a number
            case '.':
                if (j == strlen(argv[i])-1 || !isdigit(argv[i][j+1])) {
                    snprintf(buffer, sizeof(buffer), ". sign in wrong"
                    " location");
                    return buffer;
                }
                break;
            // minus sign should come before a number
            case '-':
                next_valid = (!isdigit(argv[i][j+1]) && argv[i][j+1] != '-');
                if (j == strlen(argv[i])-1 || next_valid) {
                    snprintf(buffer, sizeof(buffer), "- sign in"
                    " wrong location");
                    return buffer;
                }
                break;
            default:
                if (!is_numeric(argv[i][j])) {
                    snprintf(buffer, sizeof(buffer), "Invalid "
                    "character used");
                    return buffer;
                }
            }

            if (n1 > n2+1 || n2 > n1) {
                snprintf(buffer, sizeof(buffer), "Number of "
                "dimensions not equal to number of vectors,"
                " or invalid squared brackets");
                return buffer;
            }
        }
    }

    // there should be the same number of [ and ]
    // basis should be a square basis
    if (n1 != n2 || n1*n1 != argc) {
        snprintf(buffer, sizeof(buffer), "Invalid number "
        "of dimensions or squared brackets");
        return buffer;
    }

    snprintf(buffer, sizeof(buffer), "Passed");
    return buffer;
}

int main(int argc, char *argv[]) {
    // get the numebr of dimensions
    const char *message = check_valid(argc-1, argv+1);

    if (strcmp("Passed", message) != 0) {
        fprintf(stderr, "%s\n", message);
        return 1;
    }

    int N = sqrt(argc-1);

    // allocate required memory for 2d array
    double ** basis = malloc(N*sizeof(double));

    // put values from input into 2d array
    read_input(basis, argc-1, argv+1, N);

    double ans;

    // solve SVP
    ans = svp(basis, N);

    // SVP does not accept 0 as a solution
    if (ans == 0) {
        fprintf(stderr, "Input should be linearly "
        "independent / of full rank\n");
    }

    // open file in write mode
    FILE *file = fopen("result.txt", "w");

    if (file == NULL) {
        fprintf(stderr, "Error opening file");
        return 1;
    }

    // output result to file
    fprintf(file, "%f", ans);

    // close file
    fclose(file);

    return 0;
}
