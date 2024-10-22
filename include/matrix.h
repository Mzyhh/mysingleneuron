#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define WRONG_SHAPE 52
#define OUT_OF_BOUNDARIES 69
#define ALLOCATION_ERROR 11
typedef double elem;

typedef struct {
    elem *matrix;
    size_t rows;
    size_t columns;
} matrix;

elem get_index(const matrix *m, size_t i, size_t j);
void set_index(matrix *const m, const elem value, size_t i, size_t j);

void set_err(int code);
int get_err();

matrix* matcreate(const size_t rows, const size_t columns);
void matremove(matrix *m);
matrix* matcopy(const matrix *source);
matrix* matmul(const matrix *m1, const matrix *m2);
matrix* matadd(const matrix *m1, const matrix *m2);
int matprint(const matrix *m);

void rowswap(matrix *m, const size_t row1, const size_t row2);

#endif //_MATRIX_H
