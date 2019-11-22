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
*	Test the setup
*******************************************************************************/

MU_TEST(test_default_setup) {
	mu_assert_string_eq(foostring, original_str);
}

/*******************************************************************************
*	Test standardizing whitespaces
*******************************************************************************/
MU_TEST(test_standardize_whitespace_space) {
	mu_assert_string_eq("   The quick brown fox jumped over the lazy dog.    ", standardize_whitespace(foostring, ' '));
}

MU_TEST(test_standardize_whitespace_dash) {
	mu_assert_string_eq("---The-quick-brown-fox-jumped-over-the-lazy-dog.----", standardize_whitespace(foostring, '-'));
}

/*******************************************************************************
*	Test duplicating string
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
*	Test trim
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
*	Test sn_printf
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
*	Test remove unwanted characters
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
*	Test replace unwanted characters
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
*	Test to upper case and to lower case
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
*	Test Suite Setup
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

	/* append */

	/* concat */

	/* compare & compare case insensitive */

}


int main(int argc, char *argv[]) {
	MU_RUN_SUITE(test_suite);


	MU_REPORT();
	return 0;
}
