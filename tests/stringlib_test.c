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
	mu_assert_string_eq(res, foostring);
	free(res);
}

MU_TEST(test_duplicating_partial) {
	char* res = duplicate(foostring + 30);
	mu_assert_string_eq(res, "over the lazy\tdog. \n\r\t");
	free(res);
}


/*******************************************************************************
*	Test trim
*******************************************************************************/
MU_TEST(test_trim_both) {
	standardize_whitespace(foostring, ' ');
	size_t res = trim(foostring);
	mu_assert_string_eq("The quick brown fox jumped over the lazy dog.", foostring);
	mu_assert_int_eq(res, 7);
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

	/* */
	MU_RUN_TEST(test_trim_both);
}


int main(int argc, char *argv[]) {
	MU_RUN_SUITE(test_suite);


	MU_REPORT();
	return 0;
}

// 	char test[50] = " This is a \t\ttest   \n\n\t\r";
// 	printf("(%s)\n", test);
// 	int ret = trim(test);
// 	printf("(%s)\n", test);
// 	printf("num chars removed: %d\n", ret);
//
// 	standardize_whitespace(test, '_');
// 	printf("(%s)\n", test);
//
// 	remove_unwanted_chars(test, "Ti");
// 	printf("(%s)\n", test);
//
// 	char* val = sn_printf("This is the string: (%s)", "%sss");
// 	printf("%s\n", val);
//
// 	append(val, "this is a test");
// 	printf("%s\n", val);
//
// 	s_toupper(val);
// 	printf("%s\n", val);
// 	s_tolower(val);
// 	printf("%s\n", val);
// 	replace_unwanted_chars(val, "hs", '-');
// 	printf("%s\n", val);
// 	free(val);
//
// 	char* tmp = duplicate(test);
// 	printf("%s\n", tmp);
// 	printf("%d\n", find(tmp, 's'));
// 	printf("%d\n", find_reverse(tmp, 's'));
//
// 	printf("case sensitive (true) %d\n", s_cmp("this", "this"));
//
// 	printf("case sensitive (false) %d\n", s_cmp("this", "This"));
//
// 	printf("case insensitive (true) %d\n", s_cmp_alt("this", "This", 0));
//
// 	printf("case insensitive (fase) %d\n", s_cmp_alt("athis2", "This1", 0));
//
//
// 	char blah[175] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//
// 	printf("%s\n", extract_substring(blah, 0, 175));  // whole thing
// 	printf("%s\n", extract_substring(blah, 5, 10));
// 	printf("%s\n", extract_substring(blah, 1, 26));
// 	if(extract_substring(blah, 53, 10) == NULL)
// 		printf("SUCCESS: Handled null correctly\n");
// 	return 0;
// }
