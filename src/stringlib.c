#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "stringlib.h"


char* duplicate(char* s) {
	size_t len = strlen(s) + 1;  // ensure room for NULL terminated
	char* buf = malloc(len * sizeof(char));
	if (buf == NULL)
		return NULL;
	strncpy(buf, s, len);
	buf[len] = '\0';
	return buf;
}


size_t trim(char* s) {
	size_t res = 0, i = 0, j = 0, length = strlen(s);

	// remove trailing spaces
	while (isspace(s[length - 1]) != 0) {
		s[--length] = '\0';
		res++;
	}

	// count the number of leading spaces
	while (isspace(s[j]) != 0) j++;

	// set our return value
	res += j;

	// now we need to move things
	while (j <= length) s[i++] = s[j++];

	return res;
}


char* standardize_whitespace(char* s, const char c) {
	size_t i = 0;
	while (s[i] != '\0') {
		if (isspace(s[i]))
			s[i] = c;
		i++;
	}
	return s;
}


char* sn_printf(char* fmt, ...) {
	va_list args;

	va_start(args, fmt);

	size_t len = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	char* buf = malloc((len + 1) * sizeof(char));
	if (buf == NULL)
		return NULL; // must be an error state

	va_start(args, fmt);
	vsnprintf(buf, len + 1, fmt, args);
	va_end(args);

	return buf;
}

char* remove_unwanted_chars(char* s, const char* unwanted) {
	size_t i = 0, j = 0, unwanted_len = strlen(unwanted);
	while (s[j] != '\0') {
		int changes = 0;
		for (size_t q = 0; q < unwanted_len; q++) {
			if (s[j] == unwanted[q]) {
				changes = 1;
				++j;
				break;
			}
		}

		s[i] = s[j];

		if (changes == 0){
			++i;
			++j;
		}
	}
	s[i] = '\0';
	return s;
}


char* replace_unwanted_chars(char* s, const char* unwanted, const char c) {
	size_t i = 0, j = 0, unwanted_len = strlen(unwanted);
	while (s[j] != '\0') {
		for (size_t q = 0; q < unwanted_len; q++) {
			if (s[j] == unwanted[q]) {
				s[j] = c;
				break;
			}
		}

		++i;
		++j;
	}
	return s;
}


char* s_toupper(char* s) {
	for (size_t i = 0; s[i] != '\0'; i++)
		s[i] = toupper(s[i]);
	return s;
}


char* s_tolower(char* s) {
	for (size_t i = 0; s[i] != '\0'; i++)
		s[i] = tolower(s[i]);
	return s;
}


int find(char* s, char c) {
	char* loc = strchr(s, c);
	if (loc == NULL)
		return -1;
	return loc - s;
}


int find_reverse(char* s, char c) {
	char* loc = strrchr(s, c);
	if (loc == NULL)
		return -1;
	return loc - s;
}


char* append(char* s1, char* s2) {
	size_t len = strlen(s1) ;
	s1 = realloc(s1, len + strlen(s2) + 1);
	strncat(s1, s2, strlen(s2));
	return s1;
}


char* s_concat(char* s1, char* s2) {
	char* ret = duplicate(s1);
	return append(ret, s2);
}


int s_cmp(char* s1, char* s2) {
	return s_cmp_alt(s1, s2, 1);  // we want it case sensitive
}


int s_cmp_alt(char* s1, char* s2, int casesensitive) {
	if (casesensitive) {
		return strcmp(s1, s2);
	}

	char* t1 = duplicate(s1);
	char* t2 = duplicate(s2);
	int res = strcmp(s_tolower(t1), s_tolower(t2));

	free(t1);
	free(t2);
	return res;
}


char* extract_substring(char* s, size_t start, size_t length) {
	size_t len = strlen(s);
	if (start > len)
		return NULL;
	if (start + length > len)
		return duplicate(s + start);

	char* ret = calloc(length + 1, sizeof(char));
	return strncpy(ret, s + start, length);
}


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
