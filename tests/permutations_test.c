

#include "../src/minunit.h"
#include "../src/permutations.h"


permutations_t p;

void test_setup(void) {
    p = perm_init(8, "ATCG");
}

void test_teardown(void) {
    perm_free(p);
}


MU_TEST(test_default_setup) {
    mu_assert_not_null(p);
    mu_assert_int_eq(8, perm_input_size(p));\
    mu_assert_string_eq("ATCG", perm_alphabet(p));
    mu_assert_int_eq(4, perm_alphabet_length(p));
    const unsigned short* perm = perm_current_permutation(p);
    int i, res = 0;
    for (i = 0; i < 8; i++) {
        if (perm[i] != 0)
            ++res;
    }
    mu_assert_int_eq(0, res);
    mu_assert_string_eq("AAAAAAAA", perm_to_string(p));
}

MU_TEST(test_inc) {
    perm_inc(p);
    mu_assert_string_eq("AAAAAAAT", perm_to_string(p));
    perm_inc(p);
    mu_assert_string_eq("AAAAAAAC", perm_to_string(p));
    perm_inc(p);
    mu_assert_string_eq("AAAAAAAG", perm_to_string(p));
    perm_inc(p);
    mu_assert_string_eq("AAAAAATA", perm_to_string(p));
}

MU_TEST(test_add) {
    perm_add(p, 10);
    mu_assert_string_eq("AAAAAACC", perm_to_string(p));
    perm_add(p, 100);
    mu_assert_string_eq("AAAATCGC", perm_to_string(p));
    perm_add(p, 1000);
    mu_assert_string_eq("AATATTTC", perm_to_string(p));
}

MU_TEST(test_add_overflow) {
    perm_add(p, 65535); // this should be everything!
    mu_assert_string_eq("GGGGGGGG", perm_to_string(p));
    /* wrap around! */
    perm_inc(p);
    mu_assert_string_eq("AAAAAAAA", perm_to_string(p));
    perm_add(p, 100);
    mu_assert_string_eq("AAAATCTA", perm_to_string(p));
}

/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    /* setup */
    MU_RUN_TEST(test_default_setup);

    /* increment / add */
    MU_RUN_TEST(test_inc);
    MU_RUN_TEST(test_add);
    MU_RUN_TEST(test_add_overflow);
}

int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}
