#include "include/matrix.h"

static int materrorno = EXIT_SUCCESS;
extern int errno;

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

elem geti(const matrix m, size_t i, size_t j)
{
    if (i < 0 || i >= m.rows || j < 0 || j >= m.columns) {
        set_err(OUT_OF_BOUNDARIES);
    }
    return m.matrix[i*m.columns + j];
}

void seti(matrix m, const elem value, size_t i, size_t j)
{
    if (i < 0 || i >= m.rows || j < 0 || j >= m.columns) {
        set_err(OUT_OF_BOUNDARIES);
    }
    m.matrix[i*m.columns + j] = value;
}

void rowswap(matrix m, const size_t row1, const size_t row2)
{
    if (row1 == row2) {
        return;
    }
    elem* buffer = (elem*)calloc(m.columns, sizeof(elem));
    if (!buffer) {
        set_err(ROW_SWAP_ERROR);
        return;
    }
    elem* ptr1 = m.matrix + row1*m.columns, *ptr2 = m.matrix + row2*m.columns;
    const size_t size = m.columns*sizeof(elem);
    memcpy(buffer, ptr1, size);
    memcpy(ptr1, ptr2, size);
    memcpy(ptr2, buffer, size);
}

/**
 * Create new matrix with shape (rows, columns)
 * @param rows The number of rows
 * @param columns The number of columns
 * If allocation failed result matrix has rows, columns, matrix field are equal to 0. 
 */
matrix matcreate(const size_t rows, const size_t columns)
{
    if (rows <= 0 || columns <= 0) {
        set_err(WRONG_SHAPE);
    }
    matrix result;

    result.rows = rows;
    result.columns = columns;
    result.matrix = (elem*)calloc(rows*columns, sizeof(elem*));
    if (!result.matrix) {
        set_err(ALLOCATION_ERROR);
        result.columns = 0;
        result.rows = 0;
    }
    return result;
}

void matremove(matrix m)
{
    free(m.matrix);
    m.matrix = NULL;
    m.rows = 0;
    m.columns = 0;
}

matrix matcopy(const matrix source)
{
    matrix dest = matcreate(source.rows, source.columns);
    PROPAGATE_ERROR(dest);
    memmove(dest.matrix, source.matrix, sizeof(elem)*source.rows*source.columns);
    return dest;
}

/**
 * This function multiples matrices m1 and m2.
 * Matrices should be of shapes (m, k) and (k, n);
 * Resulting matrix has shape (m, n).
 */
matrix matmul(const matrix m1, const matrix m2)
{
    matrix result;
    result.matrix = NULL;
    if (m1.columns != m2.rows) {
        set_err(WRONG_SHAPE);
        return result;
    }

    result = matcreate(m1.rows, m2.columns);
    PROPAGATE_ERROR(result);
    for (size_t i = 0; i < m1.rows; ++i) {
        for (size_t j = 0; j < m2.columns; ++j) {
            seti(result, 0, i, j);
            for (size_t k = 0; k < m1.rows; ++k) {
                const int c = geti(m1, i, k)*geti(m2, k, j);
                seti(result, geti(result, i, j) + c, i, j);
            }
        }
    }
    return result;
}

/**
 * This function adds matrices m1 and m2.
 * Matrices m1 and m2 should be of the same shape.
 * @param m1 left addition operand.
 * @param m2 right addition operand.
 */
matrix matadd(const matrix m1, const matrix m2)
{
    matrix result;
    result.matrix = NULL;
    if (m1.rows != m2.rows || m1.columns != m2.columns) {
        set_err(WRONG_SHAPE);
        return result;
    }
    
    result = matcopy(m1);
    PROPAGATE_ERROR(result);
    mataddassign(result, m2);

    return result;
}

/**
 * This function adds matrices m1 and m2 and put result into m1.
 * Matrices m1 and m2 should be of the same shape.
 * @param m1 left addition operand and destination.
 * @param m2 right addition operand.
 */
void mataddassign(matrix m1, const matrix m2)
{
    for (size_t i = 0; i < m1.rows; ++i) {
        for (size_t j = 0; j < m1.columns; ++j) {
            seti(m1, geti(m1, i, j) + geti(m2, i, j), i, j);
        }
    }
}

matrix matmap(const matrix m, elem (*f)(elem))
{
    matrix result = matcopy(m);
    PROPAGATE_ERROR(result);
    for (size_t i = 0; i < result.rows; ++i) {
        for (size_t j = 0; j < result.columns; ++j) {
            seti(result, f(geti(result, i, j)), i, j);
        }
    }
    return result;
}

matrix matmulnum(const matrix m, const double factor)
{
    matrix result = matcopy(m);
    PROPAGATE_ERROR(result);
    for (size_t i = 0; i < result.rows; ++i) {
        for (size_t j = 0; j < result.columns; ++j) {
            seti(result, factor*geti(result, i, j), i, j);
        }
    }
    return result;
}

elem determinant(const matrix m)
{
    if (m.rows != m.columns) {
        set_err(WRONG_SHAPE);
        return 0.0;
    }

    matrix tmp = matcopy(m);
    PROPAGATE_ERROR(0.0);
    elem det = 1.0;
    for (size_t j = 0; j < m.columns-1; ++j) {
        size_t i = j;
        elem pivot = geti(tmp, i, j);
        for (;pivot == 0.0 && i < m.rows;) {
            pivot = geti(tmp, ++i, j);
        }

        if (pivot == 0.0) {
            return 0.0;
        }
        if (i != j) {
            rowswap(tmp, i, j);
            det *= -1;
        }

        for (size_t k = j+1; k < tmp.rows; ++k) {
            elem aim = geti(tmp, k, j);
            elem factor = aim / pivot;
            for (size_t l = j; l < tmp.columns; ++l) {
                elem curr = geti(tmp, k, l);
                seti(tmp, curr - factor*geti(tmp, j, l), k, l); 
            }
        }
        det *= pivot;
    }
    det  *= geti(tmp, tmp.rows - 1, tmp.columns - 1);
    matremove(tmp);
    return det;
}

int matprint(const matrix m) 
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.columns; ++j) {
            printf("%f\t", geti(m, i, j));
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}



int main(void) 
{
//    matrix *a = matcreate(2, 2); 
//    set_index(a, 3, 0, 0);
//    set_index(a, 6, 0, 1);
//    set_index(a, 4, 1, 0);
//    set_index(a, 12, 1, 1);
//    matprint(a); 
//    printf("\n");
    
    matrix b = matcreate(3, 3); 
    seti(b, 1, 1, 0);
    seti(b, 2, 0, 1);
    seti(b, 3, 2, 2);
    matprint(b); 
    printf("\n");
    printf("DET=%f\n", determinant(b));
    matrix c = matmulnum(b, 2);
    matprint(c); 
    printf("\n");

    mataddassign(b, c);
    matprint(b); 
    printf("\n");

//    matrix *c = matadd(a, b);
//    matprint(c); 
//    printf("\n");

    return materrorno;
}
