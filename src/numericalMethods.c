#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "numericalMethods.h"


array explicitEulerMethod(CauchyProblem ivp, double T, double h) {
    size_t dots = (size_t)ceil((T - ivp.t0)/h);
    size_t N = (ivp.order + 1)*(dots+1);
    double* solution = (double*)malloc(sizeof(double)*N); 
    for (size_t i = 1; i < ivp.order + 1; ++i)
        solution[i] = ivp.initial_values[i-1];
    
    size_t curr, next;
    double t = ivp.t0;
    for (size_t i = 0; i < dots; ++i, t+=h) {
        curr = (ivp.order+1)*i;
        next = (ivp.order+1)*(i+1);
        solution[curr] = t;
        for (size_t j = 1; j < ivp.order + 1; ++j) {
            solution[next+j] = solution[curr+j] + h*(*ivp.F[j-1])(t, solution+curr+1);
        }
    }
    solution[next] = t;

    array result = {solution, N}; 
    return result;
}

array midpointMethod(CauchyProblem ivp, double T, double h) {
    size_t dots = (size_t)ceil((T - ivp.t0)/h);
    size_t N = (ivp.order + 1)*(dots+1);
    double* solution = (double*)malloc(sizeof(double)*N); 
    for (size_t i = 1; i < ivp.order + 1; ++i)
        solution[i] = ivp.initial_values[i-1];
    
    size_t curr, next;
    double t = ivp.t0;
    double *quasimiddle = (double*)malloc(sizeof(double)*ivp.order);
    for (size_t i = 0; i < dots; ++i, t+=h) {
        curr = (ivp.order+1)*i;
        next = (ivp.order+1)*(i+1);
        solution[curr] = t;
        for (size_t k = 0; k < ivp.order; ++k)
            quasimiddle[k] = solution[curr+k+1] + 0.5*h*(*ivp.F[k])(t, solution+curr+1); 
        for (size_t j = 1; j < ivp.order + 1; ++j) {
            solution[next+j] = solution[curr+j] + h*(*ivp.F[j-1])(t+0.5*h, quasimiddle);
        }
    }
    free(quasimiddle);
    solution[next] = t;

    array result = {solution, N}; 
    return result;
}

array rungeKuttaMethod(CauchyProblem ivp, double T, double h) {
    size_t dots = (size_t)ceil((T - ivp.t0)/h);
    size_t N = (ivp.order + 1)*(dots+1);
    double* solution = (double*)malloc(sizeof(double)*N); 
    for (size_t i = 1; i < ivp.order + 1; ++i)
        solution[i] = ivp.initial_values[i-1];

    double *temp1 = (double*)malloc(sizeof(double)*ivp.order*2);
    double *temp2 = temp1 + ivp.order;
    double t = ivp.t0;
    for(size_t i = 0, curr, next; i < dots; ++i, t+=h) {
        curr = (ivp.order+1)*i;
        next = (ivp.order+1)*(i+1);
        solution[curr] = t;
        for (size_t j = 0; j < ivp.order; ++j)
            solution[next+1+j] = solution[curr+1+j];
        for (size_t j = 0; j < ivp.order; ++j) {
            temp1[j] = (*ivp.F[j])(t, solution+curr+1);
            solution[next+j+1] += h/6 * temp1[j]; 
            temp1[j] = solution[curr+j+1] + h/2 * temp1[j];
        }
        for (size_t j = 0; j < ivp.order; ++j) {
            temp2[j] = (*ivp.F[j])(t + h/2, temp1);
            solution[next+j+1] += h/3 * temp2[j];
            temp2[j] = solution[curr+j+1] + h/2 * temp2[j];
        }
        for (size_t j = 0; j < ivp.order; ++j) {
            temp1[j] = (*ivp.F[j])(t + h/2, temp2);
            solution[next+j+1] += h/3 * temp1[j];
            temp1[j] = solution[curr+j+1] + h*temp1[j];
        }
        for (size_t j = 0; j < ivp.order; ++j) {
            solution[next+j+1] += h/6 * (*ivp.F[j])(t + h, temp1);
        }
    }
    free(temp1);
    array result = {solution, N}; 
    return result;
}
