// function to compute the dot product between two vectors
double dot_product(double * x, double * y, int N);

// function to multiply vector by a scalar multiple, saves to a destination array
void scalar_product(double * x, double scalar, int N, double * save_to);

// function to subtract two vectors inplace
void vector_sub(double * v1, double * v2, int N);