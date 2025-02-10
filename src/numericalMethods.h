#include "matrix.h"


typedef struct {
    double *data;
    size_t length;
} array;

array explicitEulerMethod(size_t order, double h, double t0,
        double T, double *iv, double(*F[])(double, double*));
