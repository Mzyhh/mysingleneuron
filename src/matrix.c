#include "matrix.h"

extern int materrorno;
extern int errno;

/**
 * Function `set_err` sets globally error code variable of module `matrix`
 * to value `code`.
 */
void set_err(int code)
{
    materrorno = code;
}

/**
 * Function `get_err` return error code of module `matrix` and set
 * it globally to `EXIT_SUCCESS`
 */
int get_err()
{
    int tmp = materrorno;
    materrorno = EXIT_SUCCESS;
    return tmp;
}

/**Function `geti` return element of matrix that is located
 * on row `i` and column `j`
 */
elem geti(const matrix m, size_t i, size_t j)
{
    if (i < 0 || i >= m.rows || j < 0 || j >= m.columns) {
        set_err(MAT_OUT_OF_BOUNDARIES);
    }
    return m.matrix[i*m.columns + j];
}

/**
 * Function `seti` sets `value` to matrix element on row `i` and column `j`.
 */
void seti(matrix m, const elem value, size_t i, size_t j)
{
    if (i < 0 || i >= m.rows || j < 0 || j >= m.columns) {
        set_err(MAT_OUT_OF_BOUNDARIES);
    }
    m.matrix[i*m.columns + j] = value;
}

/**
 * Function `rowswap` transposes rows with indices `row1` and `row2`
 */
void rowswap(matrix m, const size_t row1, const size_t row2)
{
    if (row1 == row2) {
        return;
    }
    elem* buffer = (elem*)calloc(m.columns, sizeof(elem));
    if (!buffer) {
        set_err(MAT_ROWSWAP_ERROR);
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
    matrix result;
    if (rows <= 0 || columns <= 0) {
        set_err(MAT_WRONG_SHAPE);
        result.matrix = NULL;
        result.columns = 0;
        result.rows = 0;
        return result;
    }

    result.rows = rows;
    result.columns = columns;
    result.matrix = (elem*)calloc(rows*columns, sizeof(elem*));
    if (!result.matrix) {
        set_err(MAT_ALLOCATION_ERROR);
        result.columns = 0;
        result.rows = 0;
    }
    return result;
}

/** This function make memory allocated for m->matrix free and set all matrix fields to 0.
 */
void matremove(matrix *m)
{
    free(m->matrix);
    m->matrix = NULL;
    m->rows = 0;
    m->columns = 0;
}

/**Create new matrix that is equal to source matrix
 */
matrix matcopy(const matrix source)
{
    matrix dest = matcreate(source.rows, source.columns);
    PROPAGATE_ERROR(dest);
    memmove(dest.matrix, source.matrix, sizeof(elem)*source.rows*source.columns);
    return dest;
}

/**Analogue of == for matrices.
 */
int mateq(const matrix a, const matrix b)
{
    if (a.rows != b.rows || a.columns != b.columns) {
        return 0; 
    }

    for (size_t i = 0; i < a.rows; ++i) {
        for (size_t j = 0; j < a.rows; ++j) {
            if (geti(a, i, j) != geti(b, i, j)) {
                return 0;
            }
        }
    }

    return 1;
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
        set_err(MAT_WRONG_SHAPE);
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
        set_err(MAT_WRONG_SHAPE);
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

/** This function map function f to each element of matrix m.
 * @param f Function: elem -> elem
 * @param m source matrix, it is not changed
 */
matrix matmap(const matrix m, elem (*f)(elem, size_t, size_t))
{
    matrix result = matcopy(m);
    PROPAGATE_ERROR(result);
    matmapassign(result, f);
    return result;
}

/** This function map function f to each element of matrix m.
 * @param f Function: elem -> elem
 * @param m Source and resulting matrix 
 */
void matmapassign(matrix m, elem (*f)(elem, size_t, size_t))
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.columns; ++j) {
            seti(m, f(geti(m, i, j), i, j), i, j);
        }
    }
}

/**Return matrix that is matrix m with every element multiplyed by factor.
 */
matrix matmulnum(const matrix m, const double factor)
{
    matrix result = matcopy(m);
    PROPAGATE_ERROR(result);
    matmulnumassign(result, factor);
    return result;
}

/**Multiply every matrix m's element by factor.
 */
void matmulnumassign(matrix m, const double factor)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.columns; ++j) {
            seti(m, factor*geti(m, i, j), i, j);
        }
    }
}

/**Function `matdet` calculates determinant of matrix m,
 * using triangular view of matrix.
 */
elem matdet(const matrix m)
{
    if (m.rows != m.columns) {
        set_err(MAT_WRONG_SHAPE);
        return 0.0;
    }

    matrix tmp = matcopy(m);
    PROPAGATE_ERROR(0.0);
    elem det = 1.0;
    for (size_t j = 0; j < m.columns-1; ++j) {
        size_t i = j;
        elem pivot = geti(tmp, i, j);
        for (;pivot == 0.0 && i < m.rows-1;) {
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
    matremove(&tmp);
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

matrix matadamar(const matrix a, const matrix b)
{
    matrix result = {0};
    if (a.rows != b. rows || a.columns != b.columns) {
        set_err(MAT_WRONG_SHAPE);
        return result;
    }
    result = matcreate(a.rows, a.columns);
    PROPAGATE_ERROR(result);
    for (size_t i = 0; i < b.rows; ++i) {
        for (size_t j = 0; j < b.columns; ++j) {
            seti(result, geti(a, i, j) + geti(b, i, j), i, j);
        }
    }
    return result;
}

matrix mattranspose(const matrix m)
{
    matrix result = matcreate(m.rows, m.columns);
    PROPAGATE_ERROR(result);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.columns; ++j) {
            seti(result, geti(m, i, j), j , i);
        }
    }
}

matrix matidentity(const size_t size)
{
    matrix result = matcreate(size, size);
    PROPAGATE_ERROR(result);
    for (size_t i = 0; i < size; ++i) {
        seti(result, 1, i, i);
    }
    return result;
}

matrix mathorjoin(const matrix left, const matrix right)
{
    matrix result = {0};
    if (left.rows != right.rows) {
        set_err(MAT_WRONG_SHAPE);
        return result;
    }
    result = matcreate(left.rows, left.columns + right.columns);
    PROPAGATE_ERROR(result);
    for (size_t i = 0; i < left.rows; ++i) {
        for (size_t j = 0; j < left.columns; ++j) {
            seti(result, geti(left, i, j), i, j);
        }
    }
    for (size_t i = 0; i < right.rows; ++i) {
        for (size_t j = 0; j < right.columns; ++j) {
            seti(result, geti(right, i, j), i, j + left.columns);
        }
    }
    return result;
}

matrix matverjoin(const matrix up, const matrix down)
{
    matrix result = {0};
    if (up.columns != down.columns) {
        set_err(MAT_WRONG_SHAPE);
        return result;
    }
    result = matcreate(up.rows + down.rows, up.columns);
    PROPAGATE_ERROR(result);
    for (size_t i = 0; i < up.rows; ++i) {
        for (size_t j = 0; j < up.columns; ++j) {
            seti(result, geti(up, i, j), i, j);
        }
    }
    for (size_t i = 0; i < down.rows; ++i) {
        for (size_t j = 0; j < down.columns; ++j) {
            seti(result, geti(down, i, j), i + up.rows, j);
        }
    }
    return result;
}


void matinsert(const matrix source, matrix destination, size_t row, size_t col) {
    if (source.columns > destination.columns - col || 
            source.rows > destination.rows - row){
        set_err(MAT_WRONG_SHAPE);
        return;
    }
    for (size_t i = row; i < source.rows; ++i) {
        for (size_t j = col; j < source.columns; ++j) {
            seti(destination, geti(source, i, j), i, j); 
        }
    }
    PROPAGATE_ERROR();
}
