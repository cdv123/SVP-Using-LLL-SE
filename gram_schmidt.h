/*
 *              Struct: return_schmidt
 * ---------------------------------------------------
 * 
 * Stores the Gram-Schmidt information of a basis
 * 
 * double ** mu - the Gram-Schimdt coefficients of the basis
 * double ** gs_basis - the non-normalized Gram-Schmidt basis
 * double * norm - the euclidean norm of gs_basis
 *  
 * 
 */
typedef struct return_schmidt {
    double ** mu;
    double ** gs_basis;
    double * norm;
} gs_info;

void gram_schmidt(double ** basis, int N, gs_info gram_schmidt_info);