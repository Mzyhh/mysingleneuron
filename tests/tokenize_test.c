#include "lexer.h"
#include "utils.h"

int init_suite(void) {
    return EXIT_SUCCESS;
}

int clean_suite(void) {
    return EXIT_SUCCESS;
}

TEST_FUNCT(getToken_one_symbol) {
    stringstream ss;
    Token t;
    char c[1];
    c[0] = '!';
    for (;c[0] <= '~'; c[0]++) {
        INIT_SS(ss, c);
        getToken(&ss, &t);
        CU_ASSERT(t.len == 1);
        CU_ASSERT(ss.isempty(&ss));
        CU_ASSERT(t.text[0] == c[0]);
    }
}

void test_one_number(void) {
    stringstream ss;
    char buffer[128];
    INIT_SS(ss, buffer);
    Token t;
    for (int i = 1, j = 1; i < 1000000; i = i*10 + 1, j++) {
        sprintf(buffer, "%d", i); 
        getToken(&ss, &t);
        CU_ASSERT(t.len = j);
        CU_ASSERT(ss.isempty(&ss));
        CU_ASSERT(strncmp(t.text, buffer, t.len));
    }
}

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }
    CU_pSuite suite = CU_add_suite("TokenizeTestSuite", init_suite, clean_suite);

    ADD_SUITE_TEST(suite, getToken_one_symbol);

    CU_add_test(suite, "test strings containing one number", test_one_number);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
