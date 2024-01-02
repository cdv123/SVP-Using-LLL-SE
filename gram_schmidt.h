typedef struct return_schmidt {
    double ** mu;
    double ** gs_basis;
} gs_info;

gs_info gram_schmidt(double ** basis, int N);