#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "../src/queue.h"


queue_t q;

void test_setup(void) {
    q = q_init();
}

void test_teardown(void) {
    q_free_alt(q, true);
    q = NULL;
}

/*******************************************************************************
*   Setup
*******************************************************************************/
MU_TEST(test_default_setup) {
    /* q_init(q);  This is done during setup */
    mu_assert_int_eq(0, q_num_elements(q));
    mu_assert_null(q_first_node(q));
    mu_assert_null(q_last_node(q));
}

/*******************************************************************************
*   Push
*******************************************************************************/
MU_TEST(test_simple_pushes) {
    q_push(q, NULL);
    mu_assert_int_eq(1, q_num_elements(q));
    mu_assert_not_null(q_first_node(q));
    mu_assert_not_null(q_last_node(q));
    mu_assert_pointers_eq(q_first_node(q), q_last_node(q));

    q_push(q, NULL);
    mu_assert_int_eq(2, q_num_elements(q));
    mu_assert_not_null(q_first_node(q));
    mu_assert_not_null(q_last_node(q));
    mu_assert_pointers_not_eq(q_first_node(q), q_last_node(q));
}

/*******************************************************************************
*   Pop
*******************************************************************************/
MU_TEST(test_simple_pops) {
    q_push(q, NULL);
    q_push(q, NULL);
    q_push(q, NULL);
    mu_assert_int_eq(3, q_num_elements(q));

    void* tail = q_last_node(q);
    q_pop(q);
    mu_assert_int_eq(2, q_num_elements(q));
    /* make sure we haven't moved the tail... */
    mu_assert_pointers_eq(tail, q_last_node(q));
    mu_assert_pointers_not_eq(tail, q_first_node(q));

    q_pop(q);
    mu_assert_int_eq(1, q_num_elements(q));
    /*  make sure we haven't moved the tail and that now, with only one node
        that the head and tail are the same */
    mu_assert_pointers_eq(tail, q_last_node(q));
    mu_assert_pointers_eq(tail, q_first_node(q));

    tail = NULL;

    q_pop(q);
    mu_assert_int_eq(0, q_num_elements(q));
    mu_assert_null(q_first_node(q));
    mu_assert_null(q_last_node(q));
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
