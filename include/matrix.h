#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdlib.h>
#include <stdio.h>

#define WRONG_SHAPE 52
#define OUT_OF_BOUNDARIES 69
#define ALLOCATION_ERROR 11


typedef struct {
    double *matrix;
    size_t rows;
    size_t columns;
} matrix;

double get_index(const matrix *m, size_t i, size_t j);
void set_index(matrix *const m, const double value, size_t i, size_t j);
void set_err(int code);
int get_err();
matrix* matcreate(const size_t rows, const size_t columns);
matrix* matmul(const matrix *m1, const matrix *m2);
matrix* matadd(const matrix *m1, const matrix *m2);
int matprint(const matrix *m);

#endif //_MATRIX_H
