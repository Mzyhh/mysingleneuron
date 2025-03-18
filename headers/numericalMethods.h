#include "matrix.h"

#ifndef __NUMERICAL_METHODS_H__
#define __NUMERICAL_METHODS_H__


typedef struct {
    double *data;
    size_t length;
} array;

typedef struct {
    size_t order;
    double *initial_values;
    double t0;
    double(**F)(double, double*);
} CauchyProblem;

array explicitEulerMethod(CauchyProblem ivp, double T, double h);

array midpointMethod(CauchyProblem ivp, double T, double h);

array rungeKuttaMethod(CauchyProblem ivp, double T, double h);

#endif // __NUMERICAL_METHODS_H__
