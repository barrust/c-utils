#include <stdlib.h>
#include "../src/stringlib.h"
#include "minunit.h"


char* foostring = NULL;
char original_str[] = "  \tThe quick brown\nfox jumped\rover the lazy\tdog. \n\r\t";


void test_setup(void) {
    foostring = calloc(strlen(original_str) + 1, sizeof(char));
    strcpy(foostring, original_str);
}

void test_teardown(void) {
    free(foostring);
}

/*******************************************************************************
*   Test the setup
*******************************************************************************/

MU_TEST(test_default_setup) {
    mu_assert_string_eq(foostring, original_str);
}

/*******************************************************************************
*   Test standardizing whitespaces
*******************************************************************************/
MU_TEST(test_standardize_whitespace_space) {
    mu_assert_string_eq("   The quick brown fox jumped over the lazy dog.    ", standardize_whitespace(foostring, ' '));
}

MU_TEST(test_standardize_whitespace_dash) {
    mu_assert_string_eq("---The-quick-brown-fox-jumped-over-the-lazy-dog.----", standardize_whitespace(foostring, '-'));
}

/*******************************************************************************
*   Test duplicating string
*******************************************************************************/
MU_TEST(test_duplicating_string) {
    char* res = duplicate(foostring);
    mu_assert_string_eq(foostring, res);
    free(res);
}

MU_TEST(test_duplicating_partial) {
    char* res = duplicate(foostring + 30);
    mu_assert_string_eq("over the lazy\tdog. \n\r\t", res);
    free(res);
}


/*******************************************************************************
*   Test trim
*******************************************************************************/
MU_TEST(test_trim_both) {
    standardize_whitespace(foostring, ' ');
    size_t res = trim(foostring);
    mu_assert_string_eq("The quick brown fox jumped over the lazy dog.", foostring);
    mu_assert_int_eq(7, res);
}

MU_TEST(test_trim_trailing) {
    char test[] = "This is a test.  \n";
    size_t res = trim(test);
    mu_assert_string_eq("This is a test.", test);
    mu_assert_int_eq(3, res);
}

MU_TEST(test_trim_leading) {
    char test[] = " \n\r This is a test.";
    size_t res = trim(test);
    mu_assert_string_eq("This is a test.", test);
    mu_assert_int_eq(4, res);
}


/*******************************************************************************
*   Test sn_printf
*******************************************************************************/
MU_TEST(test_sn_printf) {
    char* res = sn_printf("This is an int: %d", 15);
    mu_assert_string_eq("This is an int: 15", res);
    free(res);

    char* res_d = sn_printf("This is a double: %.2f", 3.141592);
    mu_assert_string_eq("This is a double: 3.14", res_d);
    free(res_d);

    char* res1 = sn_printf("%s\t%s", "temp", "something");
    mu_assert_string_eq("temp\tsomething", res1);
    free(res1);
}


/*******************************************************************************
*   Test remove unwanted characters
*******************************************************************************/
MU_TEST(test_remove_unwanted_chars_single) {
    trim(standardize_whitespace(foostring, ' '));
    mu_assert_string_eq("The quick brwn fx jumped ver the lazy dg.", remove_unwanted_chars(foostring, "o"));
}

MU_TEST(test_remove_unwanted_chars_mult) {
    trim(standardize_whitespace(foostring, ' '));
    mu_assert_string_eq("Th quick brwn fx jump vr th laz g", remove_unwanted_chars(foostring, "oedy."));
}


/*******************************************************************************
*   Test replace unwanted characters
*******************************************************************************/
MU_TEST(test_replace_unwanted_chars_single) {
    trim(standardize_whitespace(foostring, ' '));
    mu_assert_string_eq("The quick brawn fax jumped aver the lazy dag.", replace_unwanted_chars(foostring, "o", 'a'));
}

MU_TEST(test_replace_unwanted_chars_multi) {
    trim(standardize_whitespace(foostring, ' '));
    mu_assert_string_eq("Tha quick brawn fax jumpaa avar tha laza aag.", replace_unwanted_chars(foostring, "oedy", 'a'));
}


/*******************************************************************************
*   Test to upper case and to lower case
*******************************************************************************/
MU_TEST(test_to_upper) {
    char test[] = "This is a test!";
    mu_assert_string_eq("THIS IS A TEST!", s_toupper(test));
}

MU_TEST(test_to_lower) {
    char test[] = "ThiS Is A Test!";
    mu_assert_string_eq("this is a test!", s_tolower(test));
}


/*******************************************************************************
*   Test find and find reverse
*******************************************************************************/
MU_TEST(test_find) {
    char test[] = "This is a test of the system!";
    mu_assert_int_eq(8, find(test, 'a'));
    mu_assert_int_eq(11, find(test, 'e'));
    mu_assert_int_eq(-1, find(test, 'z'));
    mu_assert_int_eq(28, find(test, '!'));
}

MU_TEST(test_find_reverse) {
    char test[] = "This is a test of the system!";
    mu_assert_int_eq(8, find_reverse(test, 'a'));
    mu_assert_int_eq(26, find_reverse(test, 'e'));
    mu_assert_int_eq(-1, find_reverse(test, 'z'));
    mu_assert_int_eq(28, find_reverse(test, '!'));
}


/*******************************************************************************
*   Test append
*******************************************************************************/
MU_TEST(test_append) {
    char* test = duplicate("This is a test");
    test = append(test, " of the system!");
    mu_assert_string_eq("This is a test of the system!", test);
    free(test);
}


/*******************************************************************************
*   Test concat
*******************************************************************************/
MU_TEST(test_concat) {
    char* test = duplicate("This is a test");
    char* t2 = duplicate(" of the system!");
    char* res = s_concat(test, t2);

    mu_assert_string_eq("This is a test of the system!", res);
    // show that all the pointers are different!
    mu_assert(test != t2, "Pointers test and t1 are equal when they should not be!");
    mu_assert(test != res, "Pointers test and res are equal when they should not be!");
    mu_assert(t2 != res, "Pointers t1 and res are equal when they should not be!");

    free(test);
    free(t2);
    free(res);
}


/*******************************************************************************
*   Test the compare functions
*******************************************************************************/
MU_TEST(test_cmp_basic) {
    char* test = duplicate("This is a test");
    mu_assert(s_cmp(test, "This is a test") == 0, "s_cmp failed!");
    mu_assert(s_cmp(test, "THIS IS A TEST") != 0, "s_cmp failed!");
    free(test);
}

MU_TEST(test_cmp_case_sensitivity) {
    char* test = duplicate("This is a test");
    mu_assert(s_cmp_alt(test, "This is a test", CASE_SENSITIVE) == 0, "s_cmp_alt failed for case sensitive!");  // case sensitive
    mu_assert(s_cmp_alt(test, "THIS IS A TEST", CASE_SENSITIVE) != 0, "s_cmp_alt failed for case sensitive!");  // case sensitive
    mu_assert(s_cmp_alt(test, "This is a test", CASE_INSENSITIVE) == 0, "s_cmp_alt failed for case insensitive!");  // case insensitive
    mu_assert(s_cmp_alt(test, "THIS IS A TEST", CASE_INSENSITIVE) == 0, "s_cmp_alt failed for case insensitive!");  // case insensitive
    free(test);
}


/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
    MU_RUN_TEST(test_default_setup);

    /* standardize whitespaces */
    MU_RUN_TEST(test_standardize_whitespace_space);
    MU_RUN_TEST(test_standardize_whitespace_dash);

    /* duplicate string */
    MU_RUN_TEST(test_duplicating_string);
    MU_RUN_TEST(test_duplicating_partial);

    /* trim */
    MU_RUN_TEST(test_trim_both);
    MU_RUN_TEST(test_trim_trailing);
    MU_RUN_TEST(test_trim_leading);

    /* sn_printf */
    MU_RUN_TEST(test_sn_printf);

    /* remove unwanted chars */
    MU_RUN_TEST(test_remove_unwanted_chars_single);
    MU_RUN_TEST(test_remove_unwanted_chars_mult);

    /* replace unwanted chars */
    MU_RUN_TEST(test_replace_unwanted_chars_single);
    MU_RUN_TEST(test_replace_unwanted_chars_multi);

    /* to upper & lower case */
    MU_RUN_TEST(test_to_upper);
    MU_RUN_TEST(test_to_lower);

    /* find & find reverse */
    MU_RUN_TEST(test_find);
    MU_RUN_TEST(test_find_reverse);

    /* append */
    MU_RUN_TEST(test_append);

    /* concat */
    MU_RUN_TEST(test_concat);

    /* compare & compare case insensitive */
    MU_RUN_TEST(test_cmp_basic);

    MU_RUN_TEST(test_cmp_case_sensitivity);

}


int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}
