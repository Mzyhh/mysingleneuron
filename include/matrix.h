#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define WRONG_SHAPE 1052
#define OUT_OF_BOUNDARIES 1069
#define ALLOCATION_ERROR 666

#define PROPAGATE_ERROR(return_value) if (materrorno != EXIT_SUCCESS) return return_value;

typedef double elem;

typedef struct {
    elem *matrix;
    size_t rows;
    size_t columns;
} matrix;

elem geti(const matrix m, size_t i, size_t j);
void seti(matrix m, const elem value, size_t i, size_t j);

void set_err(int code);
int get_err();

matrix matcreate(const size_t rows, const size_t columns);
void matremove(matrix m);
matrix matcopy(const matrix source);
matrix matmul(const matrix m1, const matrix m2);
matrix matadd(const matrix m1, const matrix m2);
int matprint(const matrix m);

void rowswap(matrix m, const size_t row1, const size_t row2);
elem determinant(const matrix m);

#endif //_MATRIX_H
