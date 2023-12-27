#include gram_schmidt.h

// structure with Gram-Schmidt information
struct return_schmidt {
    double ** mu;
    double ** gs_basis;
};

// Function to get Gram Schmidt information from set of basis vectors
struct return_schmidt gram_schmidt(double ** basis, int N){

    // initialise new basis and allocate necessary memory
    double ** gs_basis = malloc(sizeof(double*)*N);

    // initialise the gram schmidt coefficients and allocate necessary memory
    double ** mu = malloc(sizeof(double)*N);
    
    // define mu and gs_basis to have N arrays of size N and allocate necessary memory
    for (int i = 0; i < N; i++){
        mu[i] = malloc(sizeof(double)*N);
        gs_basis[i] = malloc(sizeof(double)*N);
    }

    // main loop for gram schmidt
    for (int i = 0; i < N; i++){

        // initialise mu to 0 and gs_basis to original basis
        for (int j = 0; j < N; j++){
            mu[i][j] = 0;
            gs_basis[i][j] = basis[i][j];
        }

        for (int k = 0; k < i; k++){

            // get gram schmidt coefficient
            mu[i][k] = dot_product(basis[i], gs_basis[k], N) / dot_product(gs_basis[k], gs_basis[k], N);

            // use gram schmidt coefficient to get projection
            for (int j = 0; j < N; j++){
                gs_basis[i][j] -= (mu[i][k] * gs_basis[k][j]);
            }
        }

    }

    // set diagonal to 1s so that B = B* x mu
    for (int i = 0; i < N; i++){
        mu[i][i] = 1;
    }


    // define structure to return both structures
    struct return_schmidt return_val = {mu, gs_basis};

    return return_val;
}