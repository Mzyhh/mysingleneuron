#include "test_matrix.h"

matrix a, b, c;

int init_singleton_suite(void)
{
    a = matcreate(1, 1);
    seti(a, 1, 0, 0);

    b = matcreate(1, 1);
    seti(b, -2, 0, 0);

    c = matcreate(1, 1);
    seti(c, 0, 0, 0);

    return get_err();
}

int clean_singleton_suite(void)
{
    matremove(&a);
    matremove(&b);
    matremove(&c);
    return get_err();
}

void test_singleton_create(void)
{
    CU_ASSERT_EQUAL(a.columns, 1);
    CU_ASSERT_EQUAL(a.rows, 1);
    CU_ASSERT_NOT_EQUAL(a.matrix, NULL);
}

void test_singleton_matdet(void)
{
    CU_ASSERT_EQUAL(matdet(a), 1);
    CU_ASSERT_EQUAL(matdet(b), -2);
    CU_ASSERT_EQUAL(matdet(c), 0);
}

void test_singleton_matadd(void)
{
    matrix sum = matadd(a, b);
    CU_ASSERT_EQUAL(sum.rows, 1);
    CU_ASSERT_EQUAL(sum.columns, 1);
    CU_ASSERT_EQUAL(geti(sum, 0, 0), -1.0);
    matremove(&sum);

    sum = matadd(a, c);
    CU_ASSERT_EQUAL(sum.rows, 1);
    CU_ASSERT_EQUAL(sum.columns, 1);
    CU_ASSERT_EQUAL(geti(sum, 0, 0), 1.0);
    matremove(&sum);

    sum = matadd(b, c);
    CU_ASSERT_EQUAL(sum.rows, 1);
    CU_ASSERT_EQUAL(sum.columns, 1);
    CU_ASSERT_EQUAL(geti(sum, 0, 0), -2.0);
    matremove(&sum);
}

void test_singleton_matmul(void)
{
    matrix mul = matmul(a, b);
    CU_ASSERT_EQUAL(mul.rows, 1);
    CU_ASSERT_EQUAL(mul.columns, 1);
    CU_ASSERT_EQUAL(geti(mul, 0, 0), -2);
    mul = matmul(a, c);
    CU_ASSERT_EQUAL(mul.rows, 1);
    CU_ASSERT_EQUAL(mul.columns, 1);
    CU_ASSERT_EQUAL(geti(mul, 0, 0), 0);
    
    mul = matmul(c, b);
    CU_ASSERT_EQUAL(mul.rows, 1);
    CU_ASSERT_EQUAL(mul.columns, 1);
    CU_ASSERT_EQUAL(geti(mul, 0, 0), 0);
}

void test_singleton_seti(void)
{
    matrix t = matcreate(1, 1);
    seti(t, 10.8, 0, 0);
    CU_ASSERT_EQUAL(geti(t, 0, 0), 10.8);
}

void test_singleton_matremove(void)
{
    matrix t = matcreate(1, 1);
    matremove(&t);
    CU_ASSERT_EQUAL(t.matrix, NULL);
    CU_ASSERT_EQUAL(t.rows, 0);
    CU_ASSERT_EQUAL(t.columns, 0);
}

void test_singleton_mateq(void)
{
    CU_ASSERT_FALSE(mateq(a, b));
    CU_ASSERT_FALSE(mateq(b, c));
    CU_ASSERT_TRUE(mateq(a, a));
}

void test_singleton_matcopy(void)
{
    matrix t = matcopy(a);
    CU_ASSERT_TRUE(mateq(t, a));
    matremove(&t);
}

void test_singleton_mataddassign(void)
{
    matrix t = matcreate(1, 1);
    mataddassign(t, a);
    CU_ASSERT_EQUAL(geti(t, 0, 0), 1.0);
    
    mataddassign(t, a);
    CU_ASSERT_EQUAL(geti(t, 0, 0), 2.0);

    mataddassign(t, b);
    CU_ASSERT_EQUAL(geti(t, 0, 0), 0.0);

    matremove(&t);
}

void test_singleton_matmap(void)
{
    matrix t = matmap(a, sqrt);
    CU_ASSERT_EQUAL(geti(t, 0, 0), 1.0); 
    t = matmap(a, log2);
    CU_ASSERT_EQUAL(geti(t, 0, 0), 0.0);
    matremove(&t);
}

void test_singleton_matmulnum(void)
{
    matrix t = matmulnum(b, 2.5);
    CU_ASSERT_EQUAL(geti(t, 0, 0), -5.0);
    matremove(&t);
}

int create_singleton_suite()
{
    CU_pSuite suite = CU_add_suite("SingletonMatrixSuite", init_singleton_suite, clean_singleton_suite);
    if (suite == NULL) {
        return CU_get_error(); 
    }

    if (CU_add_test(suite, "test of matdet()", test_singleton_matdet) == NULL) {
        return CU_get_error();
    }

    if (CU_add_test(suite, "test of matadd()", test_singleton_matadd) == NULL) {
        return CU_get_error();
    }

    if (CU_add_test(suite, "test of matmul()", test_singleton_matmul) == NULL) {
        return CU_get_error();
    }

    if (CU_add_test(suite, "test of matcreate()", test_singleton_create) == NULL) {
        return CU_get_error();
    }

    if (CU_add_test(suite, "test of seti()", test_singleton_seti) == NULL) {
        return CU_get_error();
    }

    if (CU_add_test(suite, "test of matremove()", test_singleton_matremove) == NULL) {
        return CU_get_error();
    }

    if (CU_add_test(suite, "test of mateq()", test_singleton_mateq) == NULL) {
        return CU_get_error();
    }

    if (CU_add_test(suite, "test of matcopy()", test_singleton_matcopy) == NULL) {
        return CU_get_error();
    }

    if (CU_add_test(suite, "test of mataddassign()", test_singleton_mataddassign) == NULL) {
        return CU_get_error();
    }

    if (CU_add_test(suite, "test of matmap()", test_singleton_matmap) == NULL) {
        return CU_get_error();
    }

    if (CU_add_test(suite, "test of matmulnum()", test_singleton_matmulnum) == NULL) {
        return CU_get_error();
    }
    return CUE_SUCCESS;
}
