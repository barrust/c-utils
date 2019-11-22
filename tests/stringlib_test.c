#include <stdlib.h>
#include "../src/stringlib.h"
//#include "minunit.h"


// SHOULD BE REMOVED!!!
int main(int argc, char const *argv[]) {

	char test[50] = " This is a \t\ttest   \n\n\t\r";
	printf("(%s)\n", test);
	int ret = trim(test);
	printf("(%s)\n", test);
	printf("num chars removed: %d\n", ret);

	standardize_whitespace(test, '_');
	printf("(%s)\n", test);

	remove_unwanted_chars(test, "Ti");
	printf("(%s)\n", test);

	char* val = sn_printf("This is the string: (%s)", "%sss");
	printf("%s\n", val);

	append(val, "this is a test");
	printf("%s\n", val);

	s_toupper(val);
	printf("%s\n", val);
	s_tolower(val);
	printf("%s\n", val);
	replace_unwanted_chars(val, "hs", '-');
	printf("%s\n", val);
	free(val);

	char* tmp = duplicate(test);
	printf("%s\n", tmp);
	printf("%d\n", find(tmp, 's'));
	printf("%d\n", find_reverse(tmp, 's'));

	printf("case sensitive (true) %d\n", s_cmp("this", "this"));

	printf("case sensitive (false) %d\n", s_cmp("this", "This"));

	printf("case insensitive (true) %d\n", s_cmp_alt("this", "This", 0));

	printf("case insensitive (fase) %d\n", s_cmp_alt("athis2", "This1", 0));


	char blah[175] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	printf("%s\n", extract_substring(blah, 0, 175));  // whole thing
	printf("%s\n", extract_substring(blah, 5, 10));
	printf("%s\n", extract_substring(blah, 1, 26));
	if(extract_substring(blah, 53, 10) == NULL)
		printf("SUCCESS: Handled null correctly\n");
	return 0;
}
