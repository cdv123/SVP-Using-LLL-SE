typedef struct return_schmidt {
    long double ** mu;
    long double ** gs_basis;
} gs_info;

gs_info gram_schmidt(long double ** basis, int N);