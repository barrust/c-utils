#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "../src/stack.h"

stack_list_t stk;

void test_setup(void) {
    stk = stk_init();
}

void test_teardown(void) {
    stk_free_alt(stk, true);
    stk = NULL;
}

int* make_into_pointer(int i) {
    int* t = malloc(sizeof(int));
    *t = i;
    return t;
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
    stk_push(stk, make_into_pointer(0));
    mu_assert_int_eq(1, stk_num_elements(stk));
    mu_assert_not_null(stk_first_node(stk));

    stack_node* cur = stk_first_node(stk);

    mu_assert_pointers_eq(stk_first_node(stk), cur);

    stk_push(stk, make_into_pointer(1));
    mu_assert_int_eq(2, stk_num_elements(stk));
    mu_assert_not_null(stk_first_node(stk));
    mu_assert_pointers_not_eq(stk_first_node(stk), cur);

    cur = NULL;
}

/*******************************************************************************
*   Pop
*******************************************************************************/
MU_TEST(test_simple_pops) {
    stk_push(stk, make_into_pointer(0));
    stk_push(stk, make_into_pointer(1));
    stk_push(stk, make_into_pointer(2));
    mu_assert_int_eq(3, stk_num_elements(stk));

    int* i = stk_pop(stk);
    mu_assert_int_eq(2, stk_num_elements(stk));
    mu_assert_int_eq(2, *i);
    free(i);

    stk_pop_alt(stk, true);
    mu_assert_int_eq(1, stk_num_elements(stk));
    stk_pop_alt(stk, true);
    mu_assert_int_eq(0, stk_num_elements(stk));
    mu_assert_null(stk_first_node(stk));
}

/*******************************************************************************
*   Free
*******************************************************************************/
MU_TEST(test_stack_free) {
    stack_list_t tmp = stk_init();
    stk_push(tmp, NULL);
    stk_push(tmp, NULL);
    stk_push(tmp, NULL);

    mu_assert_int_eq(3, stk_num_elements(tmp));
    /* since there is nothing allocated outside we can call the standard free */
    stk_free(tmp);
}

/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_default_setup);
    MU_RUN_TEST(test_simple_pushes);
    MU_RUN_TEST(test_simple_pops);
    MU_RUN_TEST(test_stack_free);
}


int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}
