
#ifndef BARRUST_STRING_LIBRARY_H__
#define BARRUST_STRING_LIBRARY_H__

#include <stdio.h>
#include <stdbool.h>

#define CASE_SENSITIVE 1
#define CASE_INSENSITIVE 0

/*  Duplicate a string into a newly allocated memory location that is
    guaranteed to be NULL terminated;
    NOTE: It is up to the caller to free the resulting memory! */
char* s_duplicate(const char* s);

/*  Reverse a string, in place */
char* s_reverse(char* s);

/*  Trim leading and trailing whitespace, in place.
     Returns the number of characters removed */
size_t s_trim(char* s);

/*  Replace all whitespace characters with the value of `c`; useful for
    removing tabs, newlines, etc. from a string. Does the changes in-place */
char* s_standardize_whitespace(char* s, const char c);

/*  Format the provided string into a new, allocated memory location ensuring
    that the resulting memory is large enough for the data to be held
    NOTE: It is up to the caller to free the resulting memory! */
char* s_snprintf(const char* fmt, ...);

/*  Remove all occurances of the unwanted characters in unwanted */
char* s_remove_unwanted_chars(char* s, const char* unwanted);

/*  Replace all occurances of the unwanted characters in unwanted */
char* s_replace_unwanted_chars(char* s, const char* unwanted, const char c);

/*  Convert all characters to upper case, in place */
char* s_toupper(char* s);

/*  Convert all characters to lower case, in place */
char* s_tolower(char* s);

/*  Find and return the index of the char `c` in s; if not found, returns -1 */
int s_find(const char* s, const char c);

/*  Find and return the last index of char `c` in s; if not found, returns -1 */
int s_find_reverse(const char* s, const char c);

/*  Find the last instance of the substring `sub` in s; returns NULL if not found
    NOTE: This is needed since strrstr (strstr reverse) is not guaranteed to exist */
char* s_strrstr(const char* s, const char* sub);

/*  Find and return the index of the substring `sub` in s; if not found, returns -1 */
int s_find_str(const char* s, const char* sub);

/*  Find and return the last index of the substring `sub` in s; return -1 if not found */
int s_find_reverse_str(const char* s, const char* sub);

/*  Allocate a new memory location and concat both s1 and s2 into it;
    NOTE: It is up to the caller to free the resulting memory! */
char* s_concat(const char* s1, const char* s2);

/*  Append the contents of s2 onto s1; s1 must be dynamically allocated.
    s1 will be expanded to include both s1 and s2. */
char* s_append(char* s1, const char* s2);

/*  Perform a standard case sensitive string compare */
int s_cmp(const char* s1, const char* s2);

/*  Perform an optionally case sensitive or case insensitive string compare */
int s_cmp_alt(const char* s1, const char* s2, int casesensitive);

/*  Extract a substring, starting at the fist position of string sub */
char* s_extract_substring_str(const char* s, const char* sub, size_t length);

/*  Extract a substring, starting at the first position of char c */
char* s_extract_substring_c(const char* s, const char c, size_t length);

/*  Extract a substring of size lenght starting at start */
char* s_extract_substring(const char* s, size_t start, size_t length);


// NOTE: FUTURE WORK
// char** s_split_string_str(const char* s, const char* sub);
// char** s_split_string_c(const char* s, const char c);

#endif /* END BARRUST_STRING_LIBRARY_H__ */
