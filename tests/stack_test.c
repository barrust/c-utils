#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "../src/stack.h"

ll_stack_t stk;

void test_setup(void) {
    stk = stk_init();
}

void test_teardown(void) {
    stk_free_alt(stk, true);
    stk = NULL;
}

/*******************************************************************************
*   Setup
*******************************************************************************/
MU_TEST(test_default_setup) {
    /* stk_init(stk);  This is done during setup */
    mu_assert_int_eq(0, stk_num_elements(stk));
    mu_assert_null(stk_first_node(stk));
}

/*******************************************************************************
*   Push
*******************************************************************************/
MU_TEST(test_simple_pushes) {
    stk_push(stk, NULL);
    mu_assert_int_eq(1, stk_num_elements(stk));
    mu_assert_not_null(stk_first_node(stk));

    stack_node* cur = stk_first_node(stk);

    mu_assert_pointers_eq(stk_first_node(stk), cur);

    stk_push(stk, NULL);
    mu_assert_int_eq(2, stk_num_elements(stk));
    mu_assert_not_null(stk_first_node(stk));
    mu_assert_pointers_not_eq(stk_first_node(stk), cur);

    cur = NULL;
}

/*******************************************************************************
*   Pop
*******************************************************************************/
MU_TEST(test_simple_pops) {
    stk_push(stk, NULL);
    stk_push(stk, NULL);
    stk_push(stk, NULL);
    mu_assert_int_eq(3, stk_num_elements(stk));

    stk_pop(stk);
    mu_assert_int_eq(2, stk_num_elements(stk));

    stk_pop(stk);
    mu_assert_int_eq(1, stk_num_elements(stk));
    stk_pop(stk);
    mu_assert_int_eq(0, stk_num_elements(stk));
    mu_assert_null(stk_first_node(stk));
}

/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_default_setup);
    MU_RUN_TEST(test_simple_pushes);
    MU_RUN_TEST(test_simple_pops);
}


int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}
