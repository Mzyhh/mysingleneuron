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

TEST_FUNCT(getToken_one_number) {
    stringstream ss;
    char buffer[128];
    Token t;
    for (int i = 1; i < 6; i++) {
        buffer[i-1] = i + '0';
        buffer[i] = '\0';
        INIT_SS(ss, buffer);
        getToken(&ss, &t);
        CU_ASSERT(t.len = i);
        CU_ASSERT(ss.isempty(&ss));
        CU_ASSERT(!strncmp(t.text, buffer, t.len));
    }
}

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);   
    CU_pSuite suite = CU_add_suite("TokenizeTestSuite", init_suite, clean_suite);

    ADD_SUITE_TEST(suite, getToken_one_symbol);
    ADD_SUITE_TEST(suite, getToken_one_number);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
