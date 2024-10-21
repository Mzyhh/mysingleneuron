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
 * This function multiple matrices m1 and m2 and put resulting matrix into result.
 * Matrices should be of shapes (m, k) and (k, n);
 * Resulting matrix has shape (m, n).
 */
int matmul(const matrix m1, const matrix m2, matrix *const result) {
    if (m1.columns != m2.rows) {
        return WRONG_SHAPE;
    }
     
    for (int i = 0; i < m1.rows; ++i) {
        for (int j = 0; j < m2.columns; ++j) {
             
        }
    }
    return EXIT_SUCCESS;
}

int matprint(const matrix m) {
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.columns; ++j) {
            printf("%f\t", m.matrix[i][j]);
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}

int main(void) {
    matrix m;
    matcreate(2, 2, &m); 
    m.matrix[0][0] = 1; m.matrix[0][1] = 2;
    m.matrix[1][0] = 3; m.matrix[1][1] = 4;
    matprint(m); 
}
