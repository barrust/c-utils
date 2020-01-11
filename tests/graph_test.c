#include <stdlib.h>
#include <stdbool.h>
#include "../src/graph.h"
#include "minunit.h"


graph_t g;

void test_setup(void) {
    g = g_init();
}

void test_teardown(void) {
    g_free(g);
    g = NULL;
}


/*******************************************************************************
*   Test the setup
*******************************************************************************/
MU_TEST(test_default_setup) {
    mu_assert_int_eq(0, g_num_edges(g));
    mu_assert_int_eq(0, g_num_vertices(g));
}



/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_default_setup);
}



int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}
