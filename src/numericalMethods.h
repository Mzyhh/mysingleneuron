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

typedef struct {
    size_t order;
    double *initial_values;
    double t0;
    double(**F)(double, double*);
} CauchyProblem;

array explicitEulerMethod(CauchyProblem ivp, double T, double h);

array midpointMethod(CauchyProblem ivp, double T, double h);

array rungeKuttaMethod(CauchyProblem ivp, double T, double h);

