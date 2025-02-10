#include "matrix.h"


typedef struct {
    double *data;
    size_t length;
} array;

typedef struct {
    void *data;
    size_t elemSize;
    size_t length;
} varray;

array explicitEulerMethod(size_t order, double *iv, double(*F[])(double, double*),
      double t0, double T, double h);

array midpointMethod(size_t order, double *iv, double(*F[])(double, double*),
      double t0, double T, double h);
