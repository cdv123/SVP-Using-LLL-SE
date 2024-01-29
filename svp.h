#include "gram_schmidt.h"

double get_search_area(double * norm, int N);

void size_reduce(double ** basis, double ** mu, int k, int N, double * temp);

int LLL(double ** basis, int N, gs_info gram_schmidt_info);

double svp(double ** basis, int N);

void swap(double * a, double * b);

void swap_arr(double * l1, double * l2, int N);