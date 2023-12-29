#include "gram_schmidt.h"

double get_search_area(double ** basis, int N);

void size_reduction(double ** basis, double ** mu, int k, int N);

gs_info LLL(double ** basis, int N);

double svp(double ** basis, int N);

void swap(double * a, double * b);

void swap_arr(double * l1, double * l2, int N);