typedef struct return_schmidt {
    double ** mu;
    double ** gs_basis;
} gs_info;

void gram_schmidt(double ** basis, int N, gs_info gram_schmidt_info);