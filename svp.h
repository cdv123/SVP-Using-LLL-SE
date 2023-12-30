#include "gram_schmidt.h"

long double get_search_area(long double ** basis, int N);

void size_reduction(long double ** basis, long double ** gs_basis, long double ** mu, int k, int N);

gs_info LLL(long double ** basis, int N);

long double svp(long double ** basis, int N);

void swap(long double * a, long double * b);

void swap_arr(long double * l1, long double * l2, int N);