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
        CU_ASSERT_EQUAL(t.len, i);
        CU_ASSERT_TRUE(ss.isempty(&ss));
        CU_ASSERT_EQUAL(strncmp(t.text, buffer, t.len), 0);
    }
}

TEST_FUNCT(getToken_simple_sequence1) {
    stringstream ss;
    Token t;
    char *str = "2+2";
    INIT_SS(ss, str);
    for (int i = 0; i < strlen(str); ++i) {
        getToken(&ss, &t); 
        CU_ASSERT_EQUAL(t.len, 1);
        CU_ASSERT_EQUAL(ss.offset, i + 1);
        CU_ASSERT(!strncmp(t.text, str + i, t.len));
    }
}

TEST_FUNCT(getToken_simple_sequence2) {
    stringstream ss;
    Token t;
    char *str = "a+2+b-1/2*c+0";
    INIT_SS(ss, str);
    for (int i = 0; i < strlen(str); ++i) {
        getToken(&ss, &t); 
        CU_ASSERT_EQUAL(t.len, 1);
        CU_ASSERT_EQUAL(ss.offset, i + 1);
        CU_ASSERT(!strncmp(t.text, str + i, t.len));
    }
}

TEST_FUNCT(getToken_whitespaces) {
    stringstream ss;
    Token t;
    char *str = "   a   +   2  /b";
    char *str2 = "a+2/b";
    INIT_SS(ss, str);
    for (int i = 0; i < strlen(str2); ++i) {
        getToken(&ss, &t); 
        CU_ASSERT_EQUAL(t.len, 1);
        CU_ASSERT(!strncmp(t.text, str2 + i, t.len));
    }
    CU_ASSERT(ss.isempty(&ss));
}

TEST_FUNCT(getToken_sequence1) {
    stringstream ss;
    Token t;
    char *str = "abl+22.5";
    INIT_SS(ss, str);
    getToken(&ss, &t); 
    CU_ASSERT_EQUAL(t.len, 3);
    CU_ASSERT(!strncmp(t.text, "abl", t.len));
    getToken(&ss, &t); 
    CU_ASSERT_EQUAL(t.len, 1);
    CU_ASSERT(!strncmp(t.text, "+", t.len));
    getToken(&ss, &t); 
    CU_ASSERT_EQUAL(t.len, 4);
    CU_ASSERT(!strncmp(t.text, "22.5", t.len));
    CU_ASSERT(ss.isempty(&ss));
}

TEST_FUNCT(getToken_digit_dots) {
    stringstream ss;
    Token t;
    INIT_SS(ss, "22.3.4");
    getToken(&ss, &t);
    CU_ASSERT_EQUAL(t.len, 4);
    CU_ASSERT(!strncmp(t.text, "22.3", t.len));
    getToken(&ss, &t);
    CU_ASSERT_EQUAL(t.len, 1);
    CU_ASSERT(!strncmp(t.text, ".", t.len));
    getToken(&ss, &t);
    CU_ASSERT_EQUAL(t.len, 1);
    CU_ASSERT(!strncmp(t.text, "4", t.len));
    CU_ASSERT(ss.isempty(&ss));
}

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);   
    CU_pSuite suite = CU_add_suite("TokenizeTestSuite", init_suite, clean_suite);

    ADD_SUITE_TEST(suite, getToken_one_symbol);
    ADD_SUITE_TEST(suite, getToken_one_number);
    ADD_SUITE_TEST(suite, getToken_simple_sequence1);
    ADD_SUITE_TEST(suite, getToken_simple_sequence2);
    ADD_SUITE_TEST(suite, getToken_whitespaces);
    ADD_SUITE_TEST(suite, getToken_sequence1);
    ADD_SUITE_TEST(suite, getToken_digit_dots);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
