#include "include/matrix.h"

static int error = EXIT_SUCCESS;

void set_err(int code)
{
    error = code;
}

int get_err()
{
    int tmp = error;
    error = EXIT_SUCCESS;
    return tmp;
}

double get_index(const matrix *m, size_t i, size_t j)
{
    if (i < 0 || i >= m->rows || j < 0 || j >= m->columns) {
        set_err(OUT_OF_BOUNDARIES);
    }
    return m->matrix[i*m->rows + j];
}

void set_index(matrix *m, const double value, size_t i, size_t j)
{
    if (i < 0 || i >= m->rows || j < 0 || j >= m->columns) {
        set_err(OUT_OF_BOUNDARIES);
    }
    m->matrix[i*m->rows + j] = value;
}

matrix* matcreate(const size_t rows, const size_t columns)
{
    if (rows <= 0 || columns <= 0) {
        set_err(WRONG_SHAPE);
    }
    matrix* result = (matrix*)malloc(sizeof(matrix));
    if (!result) {
        set_err(ALLOCATION_ERROR);
        return result;
    }

    result->rows = rows;
    result->columns = columns;
    result->matrix = (double*)calloc(rows*columns, sizeof(double*));
    return result;
}

/**
 * This function multiples matrices m1 and m2 and put resulting matrix into result.
 * Matrices should be of shapes (m, k) and (k, n);
 * Resulting matrix has shape (m, n).
 * @param result should be pointer on existing matrix with no allocated memory under field matrix.
 */
matrix* matmul(const matrix *m1, const matrix *m2)
{
    if (m1->columns != m2->rows) {
        set_err(WRONG_SHAPE);
        return NULL;
    }

    matrix *result = matcreate(m1->rows, m2->columns); 
    if (result) {
        for (size_t i = 0; i < m1->rows; ++i) {
            for (size_t j = 0; j < m2->columns; ++j) {
                set_index(result, 0, i, j);
                for (size_t k = 0; k < m1->rows; ++k) {
                    const int c = get_index(m1, i, k)*get_index(m2, k, j);
                    set_index(result, get_index(result, i, j) + c, i, j);
                }
            }
        }
    }
    return result;
}

/**
 * This function adds matrices m1 and m2 and put resulting matrix into result.
 * Matrices m1 and m2 should be of the same shape.
 * @param result should be pointer on existing matrix with no allocated memory under field matrix.
 */
matrix* matadd(const matrix *m1, const matrix *m2)
{
    if (m1->rows != m2->rows || m1->columns != m2->columns) {
        set_err(WRONG_SHAPE);
        return NULL;
    }
    
    matrix *result = matcreate(m1->rows, m1->columns); 
    if (result) {
        for (size_t i = 0; i < result->rows; ++i) {
            for (size_t j = 0; j < result->columns; ++j) {
                set_index(result, get_index(m1, i, j) + get_index(m2, i, j), i, j);
            }
        }
    }

    return result;
}

int matprint(const matrix *m) 
{
    for (size_t i = 0; i < m->rows; ++i) {
        for (size_t j = 0; j < m->columns; ++j) {
            printf("%f\t", get_index(m, i, j));
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}

int main(void) 
{
    matrix *a = matcreate(2, 2); 
    set_index(a, 1, 0, 0);
    set_index(a, 2, 0, 1);
    set_index(a, 3, 1, 0);
    set_index(a, 4, 1, 1);
    matprint(a); 
    printf("\n");

    matrix *b = matcreate(2, 2); 
    set_index(b, -1, 0, 0);
    set_index(b, 5, 0, 1);
    set_index(b, -3, 1, 0);
    set_index(b, 0, 1, 1);
    matprint(b); 
    printf("\n");

//    matrix *c = matadd(a, b);
//    matprint(c); 
//    printf("\n");

    matrix *d = matmul(a, b);
    matprint(d); 
    printf("\n");


    return error;
}
