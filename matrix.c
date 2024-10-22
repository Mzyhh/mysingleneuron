#include "include/matrix.h"

static int materrorno = EXIT_SUCCESS;

void set_err(int code)
{
    materrorno = code;
}

int get_err()
{
    int tmp = materrorno;
    materrorno = EXIT_SUCCESS;
    return tmp;
}

elem get_index(const matrix *m, size_t i, size_t j)
{
    if (i < 0 || i >= m->rows || j < 0 || j >= m->columns) {
        set_err(OUT_OF_BOUNDARIES);
    }
    return m->matrix[i*m->columns + j];
}

void set_index(matrix *m, const elem value, size_t i, size_t j)
{
    if (i < 0 || i >= m->rows || j < 0 || j >= m->columns) {
        set_err(OUT_OF_BOUNDARIES);
    }
    m->matrix[i*m->columns + j] = value;
}

void rowswap(matrix *m, const size_t row1, const size_t row2)
{
    elem* buffer = (elem*)calloc(m->columns, sizeof(elem));
    elem* ptr1 = m->matrix + row1*m->columns, *ptr2 = m->matrix + row2*m->columns;
    const size_t size = m->columns*sizeof(elem);
    memcpy(buffer, ptr1, size);
    memcpy(ptr1, ptr2, size);
    memcpy(ptr2, buffer, size);
}
/**
 * Create new matrix with shape (rows, columns)
 * @param rows The number of rows
 * @param columns The number of columns
 */
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
    result->matrix = (elem*)calloc(rows*columns, sizeof(elem*));
    return result;
}

void matremove(matrix *m)
{
    free(m->matrix);
    free(m);
}

matrix* matcopy(const matrix *source)
{
    matrix *dest = matcreate(source->rows, source->columns);
    memcpy(dest->matrix, source->matrix, sizeof(elem)*source->rows*source->columns);
    return dest;
}

/**
 * This function multiples matrices m1 and m2.
 * Matrices should be of shapes (m, k) and (k, n);
 * Resulting matrix has shape (m, n).
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
 * This function adds matrices m1 and m2.
 * Matrices m1 and m2 should be of the same shape.
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

//elem determinant(const matrix *m)
//{
//    if (m->rows != m->columns) {
//        set_err(WRONG_SHAPE);
//        return 0.0;
//    }
//
//    return 0.0;
//}

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
//    matprint(a); 
//    printf("\n");

    matrix *b = matcreate(4, 3); 
    set_index(b, -1, 0, 0);
    set_index(b, 5, 0, 1);
    set_index(b, -3, 0, 2);
    set_index(b, 0, 1, 0);
    set_index(b, 10, 1, 1);
    set_index(b, 110, 1, 2);
    matprint(b); 
    printf("\n");

//    matrix *c = matadd(a, b);
//    matprint(c); 
//    printf("\n");

    return materrorno;
}
