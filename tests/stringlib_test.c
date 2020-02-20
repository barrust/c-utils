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
MU_TEST(test_s_standardize_whitespace_space) {
    mu_assert_string_eq("   The quick brown fox jumped over the lazy dog.    ", s_standardize_whitespace(foostring, ' '));
}

MU_TEST(test_s_standardize_whitespace_dash) {
    mu_assert_string_eq("---The-quick-brown-fox-jumped-over-the-lazy-dog.----", s_standardize_whitespace(foostring, '-'));
}

/*******************************************************************************
*   Test duplicating string
*******************************************************************************/
MU_TEST(test_duplicating_string) {
    char* res = s_duplicate(foostring);
    mu_assert_string_eq(foostring, res);
    free(res);
}

MU_TEST(test_duplicating_partial) {
    char* res = s_duplicate(foostring + 30);
    mu_assert_string_eq("over the lazy\tdog. \n\r\t", res);
    free(res);
}


/*******************************************************************************
*   Test reverse
*******************************************************************************/
MU_TEST(test_reverse) {
    char s[] = "this is a test";
    mu_assert_string_eq("tset a si siht", s_reverse(s));
    char t[] = "tim";
    mu_assert_string_eq("mit", s_reverse(t));
    char q[] = "tiny";
    mu_assert_string_eq("ynit", s_reverse(q));
}

/*******************************************************************************
*   Test s_trim
*******************************************************************************/
MU_TEST(test_s_trim_both) {
    s_standardize_whitespace(foostring, ' ');
    size_t res = s_trim(foostring);
    mu_assert_string_eq("The quick brown fox jumped over the lazy dog.", foostring);
    mu_assert_int_eq(7, res);
}

MU_TEST(test_s_trim_trailing) {
    char test[] = "This is a test.  \n";
    size_t res = s_trim(test);
    mu_assert_string_eq("This is a test.", test);
    mu_assert_int_eq(3, res);
}

MU_TEST(test_s_trim_leading) {
    char test[] = " \n\r This is a test.";
    size_t res = s_trim(test);
    mu_assert_string_eq("This is a test.", test);
    mu_assert_int_eq(4, res);
}


/*******************************************************************************
*   Test s_snprintf
*******************************************************************************/
MU_TEST(test_s_snprintf) {
    char* res = s_snprintf("This is an int: %d", 15);
    mu_assert_string_eq("This is an int: 15", res);
    free(res);

    char* res_d = s_snprintf("This is a double: %.2f", 3.141592);
    mu_assert_string_eq("This is a double: 3.14", res_d);
    free(res_d);

    char* res1 = s_snprintf("%s\t%s", "temp", "something");
    mu_assert_string_eq("temp\tsomething", res1);
    free(res1);
}


/*******************************************************************************
*   Test remove unwanted characters
*******************************************************************************/
MU_TEST(test_remove_unwanted_chars_single) {
    s_trim(s_standardize_whitespace(foostring, ' '));
    mu_assert_string_eq("The quick brwn fx jumped ver the lazy dg.", s_remove_unwanted_chars(foostring, "o"));
}

MU_TEST(test_remove_unwanted_chars_mult) {
    s_trim(s_standardize_whitespace(foostring, ' '));
    mu_assert_string_eq("Th quick brwn fx jump vr th laz g", s_remove_unwanted_chars(foostring, "oedy."));
}


/*******************************************************************************
*   Test replace unwanted characters
*******************************************************************************/
MU_TEST(test_replace_unwanted_chars_single) {
    s_trim(s_standardize_whitespace(foostring, ' '));
    mu_assert_string_eq("The quick brawn fax jumped aver the lazy dag.", s_replace_unwanted_chars(foostring, "o", 'a'));
}

MU_TEST(test_replace_unwanted_chars_multi) {
    s_trim(s_standardize_whitespace(foostring, ' '));
    mu_assert_string_eq("Tha quick brawn fax jumpaa avar tha laza aag.", s_replace_unwanted_chars(foostring, "oedy", 'a'));
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
*   Test s_find, s_find reverse, and s_find count
*******************************************************************************/
MU_TEST(test_s_find) {
    char test[] = "This is a test of the system!";
    mu_assert_int_eq(8, s_find(test, 'a'));
    mu_assert_int_eq(11, s_find(test, 'e'));
    mu_assert_int_eq(-1, s_find(test, 'z'));
    mu_assert_int_eq(28, s_find(test, '!'));
}

MU_TEST(test_s_find_alt) {
    char test[] = "This is a test of the system!";
    mu_assert_int_eq(6, s_find_alt(test, 's', 2));
    mu_assert_int_eq(11, s_find_alt(test, 'e', 1));
    mu_assert_int_eq(20, s_find_alt(test, 'e', 2));
    mu_assert_int_eq(26, s_find_alt(test, 'e', 3));
    mu_assert_int_eq(-1, s_find_alt(test, 'e', 4));
    mu_assert_int_eq(-1, s_find_alt(test, 'z', 1));
    mu_assert_int_eq(28, s_find_alt(test, '!', 1));
}

MU_TEST(test_s_find_reverse) {
    char test[] = "This is a test of the system!";
    mu_assert_int_eq(8, s_find_reverse(test, 'a'));
    mu_assert_int_eq(26, s_find_reverse(test, 'e'));
    mu_assert_int_eq(-1, s_find_reverse(test, 'z'));
    mu_assert_int_eq(28, s_find_reverse(test, '!'));
}

MU_TEST(test_s_find_cnt) {
    char test[] = "This is a test of the system!";
    mu_assert_int_eq(5, s_find_cnt(test, 's'));
    mu_assert_int_eq(6, s_find_cnt(test, ' '));
    mu_assert_int_eq(0, s_find_cnt(test, '.'));
    mu_assert_int_eq(4, s_find_cnt(test, 't'));
}

MU_TEST(test_s_find_str) {
    char test[] = "This is a test of the system!";
    mu_assert_int_eq(10, s_find_str(test, "test"));
    mu_assert_int_eq(27, s_find_str(test, "m!"));
    mu_assert_int_eq(-1, s_find_str(test, "system."));
    mu_assert_int_eq(28, s_find_str(test, "!"));
}

MU_TEST(test_s_find_cnt_str) {
    char test[] = "This is a test of the system!";
    mu_assert_int_eq(1, s_find_cnt_str(test, "test"));
    mu_assert_int_eq(2, s_find_cnt_str(test, "is"));
    mu_assert_int_eq(0, s_find_cnt_str(test, "system."));
    mu_assert_int_eq(4, s_find_cnt_str(test, "t"));
}

MU_TEST(test_s_find_alt_str) {
    char test[] = "This is a test of the system! And it is the best!";
    mu_assert_int_eq(10, s_find_alt_str(test, "test", 1));
    mu_assert_int_eq(2, s_find_alt_str(test, "is", 1));
    mu_assert_int_eq(5, s_find_alt_str(test, "is", 2));
    mu_assert_int_eq(37, s_find_alt_str(test, "is", 3));
    mu_assert_int_eq(-1, s_find_alt_str(test, "is", 4));
    mu_assert_int_eq(27, s_find_alt_str(test, "m!", 1));
}

MU_TEST(test_s_strrstr) {
    char test[] = "This is a test of the test system!";
    mu_assert_string_eq("test system!", s_strrstr(test, "test"));  /* should pick up the second one! */
    mu_assert_string_eq("is a test of the test system!", s_strrstr(test, "is"));  /* should pick up the second one! */
    mu_assert_string_eq(NULL, s_strrstr(test, "foo"));
}

MU_TEST(test_s_find_reverse_str) {
    char test[] = "This is a test of the test system!";
    mu_assert_int_eq(22, s_find_str_reverse(test, "test"));  /* should pick up the second one! */
    mu_assert_int_eq(5, s_find_str_reverse(test, "is"));  /* should pick up the second one! */
    mu_assert_int_eq(-1, s_find_str_reverse(test, "foo"));
}

MU_TEST(test_s_find_any) {
    mu_assert_int_eq(2, s_find_any(foostring, "\n\t\r"));
    mu_assert_int_eq(21, s_find_any(foostring, "jx!"));
}

MU_TEST(test_s_find_any_reverse) {
    mu_assert_int_eq(51, s_find_any_reverse(foostring, "\n\t\r"));
    mu_assert_int_eq(23, s_find_any_reverse(foostring, "jx!"));
    mu_assert_int_eq(-1, s_find_any_reverse(foostring, ";:"));
}

MU_TEST(test_s_find_cnt_any) {
    char test[] = "This is a test of the system!";
    mu_assert_int_eq(5, s_find_cnt_any(test, "tT"));
    mu_assert_int_eq(7, s_find_cnt_any(test, "is"));
    mu_assert_int_eq(0, s_find_cnt_any(test, "\n\r"));
    mu_assert_int_eq(6, s_find_cnt_any(test, " \n\r"));
}

MU_TEST(test_s_find_alt_any) {
    char test[] = "This is a test of the system!";
    mu_assert_int_eq(0, s_find_alt_any(test, "tT", 1));
    mu_assert_int_eq(10, s_find_alt_any(test, "tT", 2));
    mu_assert_int_eq(13, s_find_alt_any(test, "tT", 3));
    mu_assert_int_eq(18, s_find_alt_any(test, "tT", 4));
    mu_assert_int_eq(25, s_find_alt_any(test, "tT", 5));
    mu_assert_int_eq(-1, s_find_alt_any(test, "tT", 6));
    mu_assert_int_eq(-1, s_find_alt_any(test, "qv", 1));
}


/*******************************************************************************
*   Test append
*******************************************************************************/
MU_TEST(test_append) {
    char* test = s_duplicate("This is a test");
    test = s_append(test, " of the system!");
    mu_assert_string_eq("This is a test of the system!", test);
    free(test);
}


/*******************************************************************************
*   Test concat
*******************************************************************************/
MU_TEST(test_concat) {
    char* test = s_duplicate("This is a test");
    char* t2 = s_duplicate(" of the system!");
    char* res = s_concat(test, t2);

    mu_assert_string_eq("This is a test of the system!", res);
    /* show that all the pointers are different! */
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
    char* test = s_duplicate("This is a test");
    mu_assert(s_cmp(test, "This is a test") == 0, "s_cmp failed!");
    mu_assert(s_cmp(test, "THIS IS A TEST") != 0, "s_cmp failed!");
    free(test);
}

MU_TEST(test_cmp_case_sensitivity) {
    char* test = s_duplicate("This is a test");
    /* case sensitive */
    mu_assert(s_cmp_alt(test, "This is a test", CASE_SENSITIVE) == 0, "s_cmp_alt failed for case sensitive!");
    mu_assert(s_cmp_alt(test, "THIS IS A TEST", CASE_SENSITIVE) != 0, "s_cmp_alt failed for case sensitive!");
    /* case insensitive */
    mu_assert(s_cmp_alt(test, "This is a test", CASE_INSENSITIVE) == 0, "s_cmp_alt failed for case insensitive!");
    mu_assert(s_cmp_alt(test, "THIS IS A TEST", CASE_INSENSITIVE) == 0, "s_cmp_alt failed for case insensitive!");
    free(test);
}


/*******************************************************************************
*   Test the compare functions
*******************************************************************************/
MU_TEST(test_extract_substring) {
    char test[] = "The quick brown fox jumped over the lazy dog.";
    char* r1 = s_extract_substring(test, 4, 5);
    mu_assert_string_eq("quick", r1);
    free(r1);
    char* r2 = s_extract_substring(test, 10, 250);
    mu_assert_string_eq("brown fox jumped over the lazy dog.", r2);
    free(r2);
}

MU_TEST(test_extract_substring_bad_start) {
    char test[] = "The quick brown fox jumped over the lazy dog.";
    char* r1 = s_extract_substring(test, 45, 5); /* close but still to long */
    mu_assert_string_eq(NULL, r1);
    free(r1);
    char* r2 = s_extract_substring(test, 145, 5); /* not even close */
    mu_assert_string_eq(NULL, r2);
    free(r2);
}

MU_TEST(test_extract_substring_str) {
    char test[] = "The quick brown fox jumped over the lazy dog.";
    char* r1 = s_extract_substring_str(test, "quick", 15);
    mu_assert_string_eq("quick brown fox", r1);
    free(r1);

    /* now check one that is not present */
    char* r2 = s_extract_substring_str(test, "qiuck", 15);
    mu_assert_string_eq(NULL, r2);
    free(r2);

    char* r3 = s_extract_substring_str(test, "lazy", 15);
    mu_assert_string_eq("lazy dog.", r3);
    free(r3);
}

MU_TEST(test_extract_substring_c) {
    char test[] = "The quick brown fox jumped over the lazy dog.";
    char* r1 = s_extract_substring_c(test, 'q', 15);
    mu_assert_string_eq("quick brown fox", r1);
    free(r1);

    /* now check one that is not present */
    char* r2 = s_extract_substring_c(test, '!', 15);
    mu_assert_string_eq(NULL, r2);
    free(r2);
}


/*******************************************************************************
*   Test split line functions
*******************************************************************************/
MU_TEST(test_split_string_c) {
    char test[] = "The quick brown fox jumped over the lazy dog.";
    int num = 0;
    char** res = s_split_string_c(test, ' ', &num);

    mu_assert_int_eq(9, num);
    mu_assert_string_eq("The", res[0]);
    mu_assert_string_eq("quick", res[1]);
    mu_assert_string_eq("brown", res[2]);
    mu_assert_string_eq("fox", res[3]);
    mu_assert_string_eq("jumped", res[4]);
    mu_assert_string_eq("over", res[5]);
    mu_assert_string_eq("the", res[6]);
    mu_assert_string_eq("lazy", res[7]);
    mu_assert_string_eq("dog.", res[8]);

    s_free_array_of_strings(res, num);

    res = s_split_string_c(test, '!', &num);
    mu_assert_int_eq(1, num);
    mu_assert_string_eq("The quick brown fox jumped over the lazy dog.", res[0]);

    s_free_array_of_strings(res, num);

    char test2[] = "This  is  a  test.";
    res = s_split_string_c(test2, ' ', &num);
    mu_assert_int_eq(4, num);
    mu_assert_string_eq("This", res[0]);
    mu_assert_string_eq("is", res[1]);
    mu_assert_string_eq("a", res[2]);
    mu_assert_string_eq("test.", res[3]);

    s_free_array_of_strings(res, num);
}

MU_TEST(test_split_string_str) {
    char test[] = "This is a test.";
    int num = 0;
    char** res = s_split_string_str(test, "is", &num);

    mu_assert_int_eq(3, num);
    mu_assert_string_eq("Th", res[0]);
    mu_assert_string_eq(" ", res[1]);
    mu_assert_string_eq(" a test.", res[2]);

    s_free_array_of_strings(res, num);

    char test2[] = "This  is  a  test.";
    res = s_split_string_str(test2, "is ", &num);
    mu_assert_int_eq(3, num);
    mu_assert_string_eq("Th", res[0]);
    mu_assert_string_eq(" ", res[1]);
    mu_assert_string_eq(" a  test.", res[2]);

    s_free_array_of_strings(res, num);

    res = s_split_string_c(test, '!', &num);
    mu_assert_int_eq(1, num);
    mu_assert_string_eq("This is a test.", res[0]);
    s_free_array_of_strings(res, num);


    res = s_split_string_str("thissss is a test.", "s", &num);
    mu_assert_int_eq(4, num);
    mu_assert_string_eq("thi", res[0]);
    mu_assert_string_eq(" i", res[1]);
    mu_assert_string_eq(" a te", res[2]);
    mu_assert_string_eq("t.", res[3]);
    s_free_array_of_strings(res, num);
}

MU_TEST(test_split_string_any) {
    char test[] = "The quick brown fox jumped over the lazy dog.";
    int num = 0;
    char** res = s_split_string_any(test, "qbfz", &num);
    mu_assert_int_eq(5, num);
    mu_assert_string_eq("The ", res[0]);
    mu_assert_string_eq("uick ", res[1]);
    mu_assert_string_eq("rown ", res[2]);
    mu_assert_string_eq("ox jumped over the la", res[3]);
    mu_assert_string_eq("y dog.", res[4]);
    s_free_array_of_strings(res, num);

    res = s_split_string_any(foostring, NULL, &num);
    mu_assert_int_eq(9, num);
    mu_assert_string_eq("The", res[0]);
    mu_assert_string_eq("quick", res[1]);
    mu_assert_string_eq("brown", res[2]);
    mu_assert_string_eq("fox", res[3]);
    mu_assert_string_eq("jumped", res[4]);
    mu_assert_string_eq("over", res[5]);
    mu_assert_string_eq("the", res[6]);
    mu_assert_string_eq("lazy", res[7]);
    mu_assert_string_eq("dog.", res[8]);
    s_free_array_of_strings(res, num);

    res = s_split_string_any(foostring, "!?", &num);
    mu_assert_int_eq(1, num);
    mu_assert_string_eq(foostring, res[0]);
    s_free_array_of_strings(res, num);
}

MU_TEST(test_s_split_lines) {
    char test[] = "Two roads diverged in a yellow wood,\n\rAnd sorry I could not travel both\f\n\rAnd be one traveler, long I stood\nAnd looked down one as far as I could\rTo where it bent in the undergrowth;";
    int num = 0;
    char** res = s_split_lines(test, &num);

    mu_assert_int_eq(5, num);
    mu_assert_string_eq("Two roads diverged in a yellow wood,", res[0]);
    mu_assert_string_eq("And sorry I could not travel both", res[1]);
    mu_assert_string_eq("And be one traveler, long I stood", res[2]);
    mu_assert_string_eq("And looked down one as far as I could", res[3]);
    mu_assert_string_eq("To where it bent in the undergrowth;", res[4]);
    s_free_array_of_strings(res, num);
}


/*******************************************************************************
*   Test single space
*******************************************************************************/
MU_TEST(test_s_single_space) {
    char test[] = "  This\t \n\r is \n\r a test! \n\r\f\t";
    s_single_space(test);
    mu_assert_string_eq("This is a test!", test);
}


/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
    MU_RUN_TEST(test_default_setup);

    /* standardize whitespaces */
    MU_RUN_TEST(test_s_standardize_whitespace_space);
    MU_RUN_TEST(test_s_standardize_whitespace_dash);

    /* s_duplicate string */
    MU_RUN_TEST(test_duplicating_string);
    MU_RUN_TEST(test_duplicating_partial);

    /* s_reverse */
    MU_RUN_TEST(test_reverse);

    /* s_trim */
    MU_RUN_TEST(test_s_trim_both);
    MU_RUN_TEST(test_s_trim_trailing);
    MU_RUN_TEST(test_s_trim_leading);

    /* s_snprintf */
    MU_RUN_TEST(test_s_snprintf);

    /* remove unwanted chars */
    MU_RUN_TEST(test_remove_unwanted_chars_single);
    MU_RUN_TEST(test_remove_unwanted_chars_mult);

    /* replace unwanted chars */
    MU_RUN_TEST(test_replace_unwanted_chars_single);
    MU_RUN_TEST(test_replace_unwanted_chars_multi);

    /* to upper & lower case */
    MU_RUN_TEST(test_to_upper);
    MU_RUN_TEST(test_to_lower);

    /* s_find & s_find reverse */
    MU_RUN_TEST(test_s_find);
    MU_RUN_TEST(test_s_find_alt);
    MU_RUN_TEST(test_s_find_reverse);
    MU_RUN_TEST(test_s_find_cnt);
    MU_RUN_TEST(test_s_find_str);
    MU_RUN_TEST(test_s_find_alt_str);
    MU_RUN_TEST(test_s_strrstr);
    MU_RUN_TEST(test_s_find_reverse_str);
    MU_RUN_TEST(test_s_find_cnt_str);
    MU_RUN_TEST(test_s_find_any);
    MU_RUN_TEST(test_s_find_alt_any);
    MU_RUN_TEST(test_s_find_cnt_any);
    MU_RUN_TEST(test_s_find_any_reverse);

    /* append */
    MU_RUN_TEST(test_append);

    /* concat */
    MU_RUN_TEST(test_concat);

    /* compare & compare case insensitive */
    MU_RUN_TEST(test_cmp_basic);
    MU_RUN_TEST(test_cmp_case_sensitivity);

    /* string extraction */
    MU_RUN_TEST(test_extract_substring);
    MU_RUN_TEST(test_extract_substring_bad_start);
    MU_RUN_TEST(test_extract_substring_str);
    MU_RUN_TEST(test_extract_substring_c);

    /* split string */
    MU_RUN_TEST(test_split_string_c);
    MU_RUN_TEST(test_split_string_str);
    MU_RUN_TEST(test_split_string_any);
    MU_RUN_TEST(test_s_split_lines);

    /* single space */
    MU_RUN_TEST(test_s_single_space);
}


int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}
