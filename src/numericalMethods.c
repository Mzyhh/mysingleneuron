#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "numericalMethods.h"


array explicitEulerMethod(size_t order, double *iv, double(*F[])(double, double*),
      double t0, double T, double h) {
    size_t dots = (size_t)ceil((T - t0)/h);
    size_t N = (order + 1)*(dots+1);
    double* solution = (double*)malloc(sizeof(double)*N); 
    for (size_t i = 1; i < order + 1; ++i)
        solution[i] = iv[i-1];
    
    size_t curr, next;
    double t = t0;
    for (size_t i = 0; i < dots; ++i, t+=h) {
        curr = (order+1)*i;
        next = (order+1)*(i+1);
        solution[curr] = t;
        for (size_t j = 1; j < order + 1; ++j) {
            solution[next+j] = solution[curr+j] + h*(*F[j-1])(t, solution+curr+1);
        }
    }
    solution[next] = t;

    array result = {solution, N}; 
    return result;
}

array midpointMethod(size_t order, double *iv, double(*F[])(double, double*),
      double t0, double T, double h) { 
    size_t dots = (size_t)ceil((T - t0)/h);
    size_t N = (order + 1)*(dots+1);
    double* solution = (double*)malloc(sizeof(double)*N); 
    for (size_t i = 1; i < order + 1; ++i)
        solution[i] = iv[i-1];
    
    size_t curr, next;
    double t = t0;
    double *temp = (double*)malloc(sizeof(double)*order);
    for (size_t i = 0; i < dots; ++i, t+=h) {
        curr = (order+1)*i;
        next = (order+1)*(i+1);
        solution[curr] = t;
        for (size_t k = 0; k < order; ++k)
            temp[k] = solution[curr+k+1] + 0.5*h*(*F[k])(t, solution+curr+1); 
        for (size_t j = 1; j < order + 1; ++j) {
            solution[next+j] = solution[curr+j] + h*(*F[j-1])(t+0.5*h, temp);
        }
    }
    free(temp);
    solution[next] = t;

    array result = {solution, N}; 
    return result;
}

// TODO: here might be optimization, because F(t_i, X_i) is calculated twice
array trapezoidMethod(size_t order, double *iv, double(*F[])(double, double*),
      double t0, double T, double h) { 
    size_t dots = (size_t)ceil((T - t0)/h);
    size_t N = (order + 1)*(dots+1);
    double* solution = (double*)malloc(sizeof(double)*N); 
    for (size_t i = 1; i < order + 1; ++i)
        solution[i] = iv[i-1];
    
    size_t curr, next;
    double t = t0, delta = 0;
    double *prenext = (double*)malloc(sizeof(double)*order);
    for (size_t i = 0; i < dots; ++i, t+=h) {
        curr = (order+1)*i;
        next = (order+1)*(i+1);
        solution[curr] = t;
        for (size_t k = 0; k < order; ++k)
            prenext[k] = solution[curr+k+1] + h*(*F[k])(t, solution+curr+1); 
        for (size_t j = 1; j < order + 1; ++j) {
            delta = h/2*((*F[j-1])(t, solution+curr+1) + (*F[j-1])(t+h, prenext));
            solution[next+j] = solution[curr+j] + delta;
        }
    }
    free(prenext);
    solution[next] = t;
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
    double step = 0.1;
    F[0] = &f1;
    F[1] = &f2;
    double iv[2] = {1, 1};
    array euler_sol = explicitEulerMethod(order, iv, F, 0, 1.5, step);
    array midpoint_sol = midpointMethod(order, iv, F, 0, 1.5, step);
    array trapezoid_sol = trapezoidMethod(order, iv, F, 0, 1.5, step);

    FILE *gp = popen("gnuplot -persist", "w");
    FILE *euler_x = fopen("euler_x.dat", "w");
    //FILE *output_y = fopen("y.dat", "w");
    FILE *real_x = fopen("real_x.dat", "w");
    //FILE *real_y = fopen("real_y.dat", "w");
    FILE *midpoint_x = fopen("midpoint_x.dat", "w");
    FILE *trapezoid_x = fopen("trapezoid_x.dat", "w");
    for (int i = 0; i < midpoint_sol.length; i += 3) {
        double t = midpoint_sol.data[i];
        //printf("%f %f\n", t, midpoint_sol.data[i+1]);
        fprintf(euler_x, "%f %f\n", t, euler_sol.data[i+1]);
        //fprintf(output_y, "%f %f\n", t, solution.data[i+2]);
        fprintf(real_x, "%f %f\n", t, x(t)); 
        //fprintf(real_y, "%f %f\n", t, y(t)); 
        fprintf(midpoint_x, "%f %f\n", t, midpoint_sol.data[i+1]);
        fprintf(trapezoid_x, "%f %f\n", t, trapezoid_sol.data[i+1]);
    }
    fprintf(gp, "plot 'midpoint_x.dat' with lines, 'real_x.dat' with lines,\
                 'trapezoid_x.dat' with lines\n");
    fflush(gp);
    free(euler_sol.data);
    free(midpoint_sol.data);
    return EXIT_SUCCESS;
}
