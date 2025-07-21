#include <stdlib.h>
#include <stdio.h>
#include "../src/minunit.h"


#define NUM_FAILED_TESTS    23
#define NUM_TESTS           46


char* res_str;

void test_setup(void) {
    res_str = (char*)calloc(25, sizeof(char));
    strcpy(res_str, "This is a test!");
}

void test_teardown(void) {
    free(res_str);
}


/*******************************************************************************
*   Check / Assert / Fail
*******************************************************************************/
MU_TEST(test_check) {
    mu_check(1==1);
}

MU_TEST(test_check_fail) {
    mu_check(1==0);  // failure
}

MU_TEST(test_assert) {
    mu_assert(1==1, "This should work!");
}

MU_TEST(test_assert_fail) {
    mu_assert(1==0, "This should fail!");  // failure
}

MU_TEST(test_fail) {
    mu_fail("This function has not been implemented"); // failure
}


/*******************************************************************************
*   Int tests
*******************************************************************************/
MU_TEST(test_int_eq) {
    mu_assert_int_eq(0, 0);
}

MU_TEST(test_int_eq_fail) {
    mu_assert_int_eq(0, 1);  // failure
}

MU_TEST(test_int_not_eq) {
    mu_assert_int_not_eq(0, 1);
}

MU_TEST(test_int_not_eq_fail) {
    mu_assert_int_not_eq(0, 0);  // failure
}

MU_TEST(test_int_greater_than) {
    mu_assert_int_greater_than(5, 6);
}

MU_TEST(test_int_greater_than_fail1) {
    mu_assert_int_greater_than(5, 5);  // failure
}

MU_TEST(test_int_greater_than_fail2) {
    mu_assert_int_greater_than(5, 4);  // failure
}

MU_TEST(test_int_less_than) {
    mu_assert_int_less_than(5, 4);
}

MU_TEST(test_int_less_than_fail1) {
    mu_assert_int_less_than(5, 5);  // failure
}

MU_TEST(test_int_less_than_fail2) {
    mu_assert_int_less_than(5, 6);  // failure
}

MU_TEST(test_int_between) {
    // NOTE: between is inclusive
    mu_assert_int_between(1, 5, 1);
    mu_assert_int_between(1, 5, 2);
    mu_assert_int_between(1, 5, 5);
}
MU_TEST(test_int_between_fail1) {
    mu_assert_int_between(1, 5, 6);  // failure
}

MU_TEST(test_int_between_fail2) {
    mu_assert_int_between(1, 5, 0);  // failure
}

MU_TEST(test_int_in) {
    int vals[] = {0, 1, 2, 3, 4, 5};

    /* test first, last, middle of list */
    mu_assert_int_in(vals, 6, 0);
    mu_assert_int_in(vals, 6, 3);
    mu_assert_int_in(vals, 6, 5);
}

MU_TEST(test_int_in_fail) {
    int vals[] = {0, 1, 2, 3, 4, 5};
    mu_assert_int_in(vals, 6, 6); // failure
}


/*******************************************************************************
*   Double tests
*******************************************************************************/
MU_TEST(test_double_eq) {
    mu_assert_double_eq(6.0, 6);
    mu_assert_double_eq(6.0, 6.000000000000001);  // close enough!
}

MU_TEST(test_double_eq_fail) {
    mu_assert_double_eq(6.0, 6.000001);  // failure
}

MU_TEST(test_double_greater_than) {
    mu_assert_double_greater_than(5.0, 6.0);
}

MU_TEST(test_double_greater_than_fail1) {
    mu_assert_double_greater_than(5.0, 5.0);  // failure
}

MU_TEST(test_double_greater_than_fail2) {
    mu_assert_double_greater_than(5.0, 4.0);  // failure
}

MU_TEST(test_double_less_than) {
    mu_assert_double_less_than(5.0, 4.0);
}

MU_TEST(test_double_less_than_fail1) {
    mu_assert_double_less_than(5.0, 5.0);  // failure
}

MU_TEST(test_double_less_than_fail2) {
    mu_assert_double_less_than(5.0, 6.0);  // failure
}

MU_TEST(test_double_between) {
    // NOTE: between is inclusive
    mu_assert_double_between(1.0, 1.5, 1.0);
    mu_assert_double_between(1.0, 1.5, 1.25);
    mu_assert_double_between(1.0, 1.5, 1.5);
}

MU_TEST(test_double_between_fail1) {
    mu_assert_double_between(1.0, 1.5, 1.51);  // failure
}

MU_TEST(test_double_between_fail2) {
    mu_assert_double_between(0.90, 1.5, 1.51);  // failure
}


/*******************************************************************************
*   String tests
*******************************************************************************/
MU_TEST(test_str_eq) {
    mu_assert_string_eq("This is a test!", res_str);

}

MU_TEST(test_str_eq_fail) {
    mu_assert_string_eq("This is also a test!", res_str); // failure
}


/*******************************************************************************
*   Pointer tests
*******************************************************************************/
MU_TEST(test_pointer_null) {
    mu_assert_null(NULL);
}

MU_TEST(test_pointer_null_fail) {
    mu_assert_null(res_str);  // failure
}

MU_TEST(test_pointer_not_null) {
    mu_assert_null(NULL);
}

MU_TEST(test_pointer_not_null_fail) {
    mu_assert_null(res_str);  // failure
}

MU_TEST(test_pointer_eq) {
    char* tmp = res_str;
    mu_assert_pointers_eq(res_str, tmp);
}

MU_TEST(test_pointer_eq_fail) {
    char* tmp = (res_str + 1);
    mu_assert_pointers_eq(res_str, tmp);  // failure
}

/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    /* check, assert, fail */
    MU_RUN_TEST(test_check);
    MU_RUN_TEST(test_check_fail);
    MU_RUN_TEST(test_assert);
    MU_RUN_TEST(test_assert_fail);
    MU_RUN_TEST(test_fail);

    /* ints */
    MU_RUN_TEST(test_int_eq);
    MU_RUN_TEST(test_int_eq_fail);
    MU_RUN_TEST(test_int_not_eq);
    MU_RUN_TEST(test_int_not_eq_fail);
    MU_RUN_TEST(test_int_greater_than);
    MU_RUN_TEST(test_int_greater_than_fail1);
    MU_RUN_TEST(test_int_greater_than_fail2);
    MU_RUN_TEST(test_int_less_than);
    MU_RUN_TEST(test_int_less_than_fail1);
    MU_RUN_TEST(test_int_less_than_fail2);
    MU_RUN_TEST(test_int_between);
    MU_RUN_TEST(test_int_between_fail1);
    MU_RUN_TEST(test_int_between_fail2);
    MU_RUN_TEST(test_int_in);
    MU_RUN_TEST(test_int_in_fail);

    /* doubles */
    MU_RUN_TEST(test_double_eq);
    MU_RUN_TEST(test_double_eq_fail);
    MU_RUN_TEST(test_double_greater_than);
    MU_RUN_TEST(test_double_greater_than_fail1);
    MU_RUN_TEST(test_double_greater_than_fail2);
    MU_RUN_TEST(test_double_less_than);
    MU_RUN_TEST(test_double_less_than_fail1);
    MU_RUN_TEST(test_double_less_than_fail2);
    MU_RUN_TEST(test_double_between);
    MU_RUN_TEST(test_double_between_fail1);
    MU_RUN_TEST(test_double_between_fail2);

    /* strings */
    MU_RUN_TEST(test_str_eq);
    MU_RUN_TEST(test_str_eq_fail);

    /* pointers */
    MU_RUN_TEST(test_pointer_null);
    MU_RUN_TEST(test_pointer_null_fail);
    MU_RUN_TEST(test_pointer_not_null);
    MU_RUN_TEST(test_pointer_not_null_fail);
    MU_RUN_TEST(test_pointer_eq);
    MU_RUN_TEST(test_pointer_eq_fail);
}


int main() {
    printf("\nRunning minunit tests...\n");
    MU_RUN_SUITE(test_suite);
    MU_REPORT();

    printf("Number asserts: %d\n", minunit_assert);
    printf("Number failed tests: %d\n", minunit_fail);
    printf("Number successful tests: %d\n", minunit_assert - minunit_fail);
    if (minunit_assert != NUM_TESTS)
        return 1;
    else if (minunit_fail != NUM_FAILED_TESTS)
        return -1;
    return 0;
}
