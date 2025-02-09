#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "globals.h"

#define MAT_WRONG_SHAPE 1052
#define MAT_OUT_OF_BOUNDARIES 1069
#define MAT_ALLOCATION_ERROR 666
#define MAT_ROWSWAP_ERROR 777

static int materrorno = EXIT_SUCCESS;

#define PROPAGATE_ERROR(return_value) if (materrorno != EXIT_SUCCESS) return return_value;



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
matrix matidentity(const size_t size);
void matremove(matrix *m);
matrix matcopy(const matrix source);
matrix matmul(const matrix m1, const matrix m2);
matrix matadd(const matrix m1, const matrix m2);
void mataddassign(matrix m1, const matrix m2);
int matprint(const matrix m);
matrix matmap(const matrix m, elem (*f)(elem, size_t, size_t));
void matmapassign(matrix m, elem (*f)(elem, size_t, size_t));
matrix matmulnum(const matrix m, const double factor);
void matmulnumassign(matrix m, const double factor);
matrix matadamar(const matrix a, const matrix b);
matrix mattranspose(const matrix m);

matrix mathorjoin(const matrix left, const matrix right);
matrix matverjoin(const matrix up, const matrix down);
void matinsert(const matrix source, matrix destination, size_t row, size_t col);

int mateq(const matrix a, const matrix b);

void rowswap(matrix m, const size_t row1, const size_t row2);
elem matdet(const matrix m);

#endif //_MATRIX_H
