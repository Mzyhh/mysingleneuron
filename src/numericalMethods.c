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


double f1(double t, double* args) {
    return 2*args[0] + args[1];
}

double f2(double t, double* args) {
    return 3*args[0] + 4*args[1];
}

double x(double t) {
    return 0.5*exp(t) + 0.5*exp(5*t);
}

double y(double t) {
    return -0.5*exp(t) + 1.5*exp(5*t);
}

int main() {
    double(*F[2])(double, double*);
    int order = 2;
    double step = 0.05;
    F[0] = &f1;
    F[1] = &f2;
    double iv[2] = {1, 1};
    CauchyProblem ivp1 = {.order = 2, .initial_values = iv, .F = F, .t0 = 0};
    array euler_sol = explicitEulerMethod(ivp1, 1.5, step);
    array midpoint_sol = midpointMethod(ivp1, 1.5, step);
    array rk_sol = rungeKuttaMethod(ivp1, 1.5, step);

    FILE *gp = popen("gnuplot -persist", "w");
    FILE *euler_x = fopen("euler_x.dat", "w");
    //FILE *output_y = fopen("y.dat", "w");
    FILE *real_x = fopen("real_x.dat", "w");
    //FILE *real_y = fopen("real_y.dat", "w");
    FILE *midpoint_x = fopen("midpoint_x.dat", "w");
    FILE *RK_x = fopen("rk_x.dat", "w");
    for (int i = 0; i < midpoint_sol.length; i += 3) {
        double t = midpoint_sol.data[i];
        //printf("%f %f\n", t, midpoint_sol.data[i+1]);
        fprintf(euler_x, "%f %f\n", t, euler_sol.data[i+1]);
        //fprintf(output_y, "%f %f\n", t, solution.data[i+2]);
        fprintf(real_x, "%f %f\n", t, x(t)); 
        //fprintf(real_y, "%f %f\n", t, y(t)); 
        fprintf(midpoint_x, "%f %f\n", t, midpoint_sol.data[i+1]);
        fprintf(RK_x, "%f %f\n", t, rk_sol.data[i+1]);
    }
    fprintf(gp, "plot 'midpoint_x.dat' with lines, 'real_x.dat' with lines,\
                 'rk_x.dat' with lines, 'euler_x.dat' with lines\n");
    fflush(gp);
    free(euler_sol.data);
    free(midpoint_sol.data);
    free(rk_sol.data);
    return EXIT_SUCCESS;
}
