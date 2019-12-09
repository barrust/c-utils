#include <stdlib.h>
#include <stdio.h>
#include "../src/bitarray.h"
#include "minunit.h"


void test_setup(void) {}

void test_teardown(void) {}

/*******************************************************************************
*   Test the setup
*******************************************************************************/
MU_TEST(test_default_setup) {
    bitarray_t ba = ba_init(125);  // bit array of length 125
    mu_assert_int_eq(125, ba_number_bits(ba));
    mu_assert_int_eq(16, ba_array_size(ba));
    mu_assert_int_eq(128, ba_array_size(ba) * 8);  // maximum number of bits, possible
    ba_free(ba);
}


/*******************************************************************************
*   Test setting a bit
*******************************************************************************/
MU_TEST(test_set_bit) {
    bitarray_t ba = ba_init(125);  // bit array of length 125
    int res;
    res = ba_set_bit(ba, 0);
    mu_assert_int_eq(1, res);
    res = ba_set_bit(ba, 50);
    mu_assert_int_eq(1, res);
    res = ba_set_bit(ba, 75);
    mu_assert_int_eq(1, res);
    res = ba_set_bit(ba, 125);
    mu_assert_int_eq(-1, res);

    ba_free(ba);
}


/*******************************************************************************
*   Test check bit
*******************************************************************************/
MU_TEST(test_check_bit) {
    bitarray_t ba = ba_init(125);  // bit array of length 125
    int res;
    res = ba_set_bit(ba, 0);
    res = ba_set_bit(ba, 50);
    res = ba_set_bit(ba, 75);

    // now check that these bits are set!
    res = ba_check_bit(ba, 0);
    mu_assert_int_eq(BIT_SET, res);
    res = ba_check_bit(ba, 50);
    mu_assert_int_eq(BIT_SET, res);
    res = ba_check_bit(ba, 75);
    mu_assert_int_eq(BIT_SET, res);
    res = ba_check_bit(ba, 125);
    mu_assert_int_eq(BITARRAY_INDEX_ERROR, res);
    res = ba_check_bit(ba, 15); // shouldn't be there!
    mu_assert_int_eq(BIT_NOT_SET, res);
    res = ba_check_bit(ba, 124); // shouldn't be there!
    mu_assert_int_eq(BIT_NOT_SET, res);

    ba_free(ba);
}


/*******************************************************************************
*   Test clear bit
*******************************************************************************/
MU_TEST(test_clear_bit) {
    bitarray_t ba = ba_init(125);  // bit array of length 125
    int res;
    res = ba_set_bit(ba, 0);
    res = ba_set_bit(ba, 50);
    res = ba_set_bit(ba, 75);

    res = ba_check_bit(ba, 0);
    mu_assert_int_eq(BIT_SET, res);
    res = ba_check_bit(ba, 50);
    mu_assert_int_eq(BIT_SET, res);
    res = ba_check_bit(ba, 75);
    mu_assert_int_eq(BIT_SET, res);

    // clear the bits!
    res = ba_clear_bit(ba, 0);
    mu_assert_int_eq(BIT_NOT_SET, res);
    res = ba_clear_bit(ba, 50);
    mu_assert_int_eq(BIT_NOT_SET, res);
    res = ba_clear_bit(ba, 75);
    mu_assert_int_eq(BIT_NOT_SET, res);

    ba_free(ba);
}


MU_TEST(test_reset_bitarray) {
    bitarray_t ba = ba_init(128);  // this is easier to test when all bits in the char are settable!

    // set all the bits!
    for (int i = 0; i < ba_number_bits(ba); i++)
        ba_set_bit(ba, i);

    // check that each char is 255
    const unsigned char* array = ba_get_bitarray(ba);

    int errors = 0;
    for (int i = 0; i < ba_array_size(ba); i++)
        errors += array[i] == 255 ? 0 : 1;

    mu_assert_int_eq(0, errors);

    int res = ba_reset_bitarray(ba);
    mu_assert_int_eq(BIT_NOT_SET, res);

    errors = 0;
    for (int i = 0; i < ba_array_size(ba); i++)
        errors += array[i] == 0 ? 0 : 1;

    ba_free(ba);
}


MU_TEST(test_print_array) {
    bitarray_t ba = ba_init(25);

    for (int i = 0; i < 25; i+=5)
        ba_set_bit(ba, i);

    char* res = ba_to_string(ba);

    mu_assert_string_eq("1000010000100001000010000", res);

    free(res);
    ba_free(ba);
}


MU_TEST(test_toggle_bit) {
    bitarray_t ba = ba_init(20);
    for (int i = 0; i < 10; i++)
        ba_set_bit(ba, i);
    char* res = ba_to_string(ba);
    mu_assert_string_eq("11111111110000000000", res);
    free(res);

    for(int i = 0; i < 20; i++)
        ba_toggle_bit(ba, i);
    res = ba_to_string(ba);
    mu_assert_string_eq("00000000001111111111", res);
    free(res);

    ba_free(ba);
}


MU_TEST(test_number_bits_set) {
    bitarray_t ba = ba_init(25);

    for (int i = 0; i < 25; i+=5)
        ba_set_bit(ba, i);
    mu_assert_int_eq(5, ba_number_bits_set(ba));

    for (int i = 0; i < 25; i++)
        ba_toggle_bit(ba, i);
    mu_assert_int_eq(20, ba_number_bits_set(ba));

    ba_reset_bitarray(ba);
    mu_assert_int_eq(0, ba_number_bits_set(ba));
    ba_free(ba);
}


/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_default_setup);
    MU_RUN_TEST(test_set_bit);
    MU_RUN_TEST(test_check_bit);
    MU_RUN_TEST(test_clear_bit);
    MU_RUN_TEST(test_reset_bitarray);
    MU_RUN_TEST(test_print_array);
    MU_RUN_TEST(test_toggle_bit);
    MU_RUN_TEST(test_number_bits_set);
}


int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}
