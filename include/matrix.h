#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdlib.h>
#include <stdio.h>

#define WRONG_SHAPE 52


typedef struct {
    float **matrix;
    size_t rows;
    size_t columns;
} matrix;

int matcreate(const size_t rows, const size_t columns, matrix *const result);
int matmul(const matrix m1, const matrix m2, matrix *const result);
int matadd(const matrix m1, const matrix m2, matrix *const result);
int matprint(const matrix m);

#endif //_MATRIX_H
