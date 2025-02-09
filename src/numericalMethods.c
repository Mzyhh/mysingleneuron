#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "numericalMethods.h"


array explicitEulerMethod(size_t order, double h, double t0,
        double T, double *iv, double(*F[])(double*))
{
    size_t dots = (size_t)ceil((T - t0)/h);
    double* solution = (double*)malloc((order + 1)*dots); 
    for (size_t i = 1; i < order + 1; ++i)
        solution[i] = iv[i-1];
    
    size_t curr, next;
    double t = t0;
    for (size_t i = 0; i < dots - 1; ++i, t+=h) {
        curr = (order+1)*i;
        next = (order+1)*(i+1);
        solution[curr] = t;
        for (size_t j = 1; j < order + 1; ++j) {
            solution[next+j] = solution[curr+j] + h*(*F[j-1])(solution+curr);
        }
    }

    array result = {solution,(order + 1)*dots}; 
    return result;
}

double f1(double* args) {
    return 2*args[1] + args[2];
}

double f2(double* args) {
    return 3*args[1] + 4*args[2];
}

int main() {
    double(*F[2])(double*);
    F[0] = &f1;
    F[1] = &f2;
    double iv[2] = {1, 1};
    array solution = explicitEulerMethod(2, 0.1, 0, 1, iv, F);
    for (int i = 0; i < solution.length; ++i) {
        printf("%f\t", solution.data[i]);
    }
    return EXIT_SUCCESS;
}
