#include "include/matrix.h"

int matcreate(const size_t rows, const size_t columns, matrix *const result)
{
    if (rows <= 0 || columns <= 0) {
        return WRONG_SHAPE;
    }
    result->rows = rows;
    result->columns = columns;
    result->matrix = (float**)calloc(rows, sizeof(float*));
    for (size_t i = 0; i < rows; ++i) {
        result->matrix[i] = (float*)calloc(columns, sizeof(float));
    }
    return EXIT_SUCCESS;
}

/**
 * This function multiples matrices m1 and m2 and put resulting matrix into result.
 * Matrices should be of shapes (m, k) and (k, n);
 * Resulting matrix has shape (m, n).
 * @param result should be pointer on existing matrix with no allocated memory under field matrix.
 */
int matmul(const matrix m1, const matrix m2, matrix *const result)
{
    if (m1.columns != m2.rows) {
        return WRONG_SHAPE;
    }

    int status = matcreate(m1.rows, m2.columns, result); 
    if (status != EXIT_SUCCESS) {
        return status;
    }

    for (size_t i = 0; i < m1.rows; ++i) {
        for (size_t j = 0; j < m2.columns; ++j) {
            result->matrix[i][j] = 0.0f;
            for (size_t k = 0; k < m1.rows; ++k) {
                result->matrix[i][j] += m1.matrix[i][k] * m2.matrix[k][j];
            }
        }
    }
    return EXIT_SUCCESS;
}

/**
 * This function adds matrices m1 and m2 and put resulting matrix into result.
 * Matrices m1 and m2 should be of the same shape.
 * @param result should be pointer on existing matrix with no allocated memory under field matrix.
 */
int matadd(const matrix m1, const matrix m2, matrix *const result)
{
    if (m1.rows != m2.rows || m1.columns != m2.columns) {
        return WRONG_SHAPE;
    }
    
    int status = matcreate(m1.rows, m1.columns, result);
    if (status != EXIT_SUCCESS) {
        return status;
    }

    for (size_t i = 0; i < result->rows; ++i) {
        for (size_t j = 0; j < result->columns; ++j) {
            result->matrix[i][j] = m1.matrix[i][j] + m2.matrix[i][j];
        }
    }

    return EXIT_SUCCESS;
}

int matprint(const matrix m) 
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.columns; ++j) {
            printf("%f\t", m.matrix[i][j]);
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}

int main(void) 
{
    matrix a;
    matcreate(2, 2, &a); 
    a.matrix[0][0] = 1; a.matrix[0][1] = 2;
    a.matrix[1][0] = 3; a.matrix[1][1] = 4;
    matprint(a); 
    printf("\n");

    matrix b;
    matcreate(2, 2, &b); 
    b.matrix[0][0] = -1; b.matrix[0][1] = 2;
    b.matrix[1][0] = 5; b.matrix[1][1] = 4;
    matprint(b); 
    printf("\n");

    matrix c;
    matmul(a, b, &c);
    matprint(c);
    printf("\n");

    matrix d;
    int ret = matadd(a, b, &d);
    if (ret != EXIT_SUCCESS) {
        printf("Error code is %d\n", ret);
    } else {
        matprint(d);
        printf("\n");
    }
}
