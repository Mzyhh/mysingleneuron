#include "matrix_test.h"

extern int materrorno;
matrix i2x2, m2x2, m2x3, zeros4x4;

int init_suite(void)
{
    i2x2 = matcreate(2, 2);
    seti(i2x2, 1, 0, 0);
    seti(i2x2, 0, 0, 1);
    seti(i2x2, 0, 1, 0);
    seti(i2x2, 1, 1, 1);
    PROPAGATE_ERROR(get_err());

    m2x2 = matcreate(2, 2);
    seti(m2x2, 2, 0, 0);
    seti(m2x2, 3, 0, 1);
    seti(m2x2, -1, 1, 0);
    seti(m2x2, 1, 1, 1);
    PROPAGATE_ERROR(get_err());

    m2x3 = matcreate(2, 3);
    seti(m2x3, 5, 0, 0);
    seti(m2x3, 2, 0, 1);
    seti(m2x3, 3, 0, 2);
    seti(m2x3, -5, 1, 0);
    seti(m2x3, 1, 1, 1);
    seti(m2x3, 1, 1, 2);
    PROPAGATE_ERROR(get_err());

    zeros4x4 = matcreate(4, 4);

    return get_err();
}

int clean_suite(void)
{
    matremove(&i2x2);
    matremove(&m2x2);
    matremove(&m2x3);
    matremove(&zeros4x4);
    return get_err();
}

void test_matdet(void)
{
    CU_ASSERT(matdet(i2x2) == 1.0); 

    CU_ASSERT(matdet(m2x2) == 5.0); 

    CU_ASSERT(matdet(zeros4x4) == 0.0);
}

void test_matdet_errors(void)
{
    matdet(i2x2);
    CU_ASSERT(get_err() == EXIT_SUCCESS);

    matdet(m2x2);
    CU_ASSERT(get_err() == EXIT_SUCCESS);

    matdet(zeros4x4);
    CU_ASSERT(get_err() == EXIT_SUCCESS);
    
    CU_ASSERT(matdet(m2x3) == 0.0);
    CU_ASSERT(get_err() == MAT_WRONG_SHAPE);
}

void test_geti(void)
{
    CU_ASSERT(geti(i2x2, 0, 0) == 1.0);
    CU_ASSERT(geti(i2x2, 0, 1) == 0.0);
    CU_ASSERT(geti(i2x2, 1, 0) == 0.0);
    CU_ASSERT(geti(i2x2, 1, 1) == 1.0);

    CU_ASSERT(geti(m2x2, 0, 0) == 2.0);
    CU_ASSERT(geti(m2x2, 0, 1) == 3.0);
    CU_ASSERT(geti(m2x2, 1, 0) == -1.0);
    CU_ASSERT(geti(m2x2, 1, 1) == 1.0);

    CU_ASSERT(geti(m2x3, 0, 0) == 5);
    CU_ASSERT(geti(m2x3, 0, 1) == 2);
    CU_ASSERT(geti(m2x3, 0, 2) == 3);
    CU_ASSERT(geti(m2x3, 1, 0) == -5);
    CU_ASSERT(geti(m2x3, 1, 1) == 1);
    CU_ASSERT(geti(m2x3, 1, 2) == 1);
}

void test_geti_errors(void)
{
    geti(i2x2, 0, -1);
    CU_ASSERT(get_err() == MAT_OUT_OF_BOUNDARIES);

    geti(i2x2, 2, 1);
    CU_ASSERT(get_err() == MAT_OUT_OF_BOUNDARIES);

    geti(i2x2, 2, -1);
    CU_ASSERT(get_err() == MAT_OUT_OF_BOUNDARIES);

    geti(i2x2, 0, 2);
    CU_ASSERT(get_err() == MAT_OUT_OF_BOUNDARIES);

    geti(i2x2, 2, 1);
    CU_ASSERT(get_err() == MAT_OUT_OF_BOUNDARIES);
}

void test_mateq(void)
{
    CU_ASSERT_TRUE(mateq(i2x2, i2x2));
    CU_ASSERT_TRUE(mateq(m2x2, m2x2));
    CU_ASSERT_TRUE(mateq(m2x3, m2x3));
    CU_ASSERT_TRUE(mateq(zeros4x4, zeros4x4));

    CU_ASSERT_FALSE(mateq(i2x2, m2x2));
    CU_ASSERT_FALSE(mateq(i2x2, m2x3));
    CU_ASSERT_FALSE(mateq(m2x2, m2x3));
    CU_ASSERT_FALSE(mateq(zeros4x4, m2x2));
}

void test_matadd(void)
{
    matrix sum = matadd(i2x2, m2x2);
    matrix true_sum = matcreate(2, 2);
    seti(true_sum, 3, 0, 0);
    seti(true_sum, 3, 0, 1);
    seti(true_sum, -1, 1, 0);
    seti(true_sum, 2, 1, 1);
    CU_ASSERT_TRUE(mateq(sum, true_sum));
    matremove(&sum);
    matremove(&true_sum);
}

int main()
{
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }
    CU_pSuite suite = CU_add_suite("ImmutableMatrixTestSuite", init_suite, clean_suite);
    CU_add_test(suite, "test of matdet()", test_matdet);
    CU_add_test(suite, "test error code of matdet()", test_matdet_errors);
    CU_add_test(suite, "test of geti()", test_geti);
    CU_add_test(suite, "test error codes of geti()", test_geti_errors);
    CU_add_test(suite, "test of mateq()", test_mateq);
    CU_add_test(suite, "test of matadd()", test_matadd);

    int code = create_singleton_suite(); 
    if (code != CUE_SUCCESS) {
        return code; 
    }
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
