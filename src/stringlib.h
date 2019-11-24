
#ifndef BARRUST_STRING_LIBRARY_H__
#define BARRUST_STRING_LIBRARY_H__

#include <stdio.h>
#include <stdbool.h>

#define CASE_SENSITIVE 1
#define CASE_INSENSITIVE 0

/*  Duplicate a string into a newly allocated memory location that is
    guaranteed to be NULL terminated;
    NOTE: It is up to the caller to free the resulting memory! */
char* duplicate(char* s);

/*  Trim leading and trailing whitespace, in place.
     Returns the number of characters removed */
size_t trim(char* s);

/*  Replace all whitespace characters with the value of `c`; useful for
    removing tabs, newlines, etc. from a string. Does the changes in-place */
char* standardize_whitespace(char* s, const char c);

/*  Format the provided string into a new, allocated memory location ensuring
    that the resulting memory is large enough for the data to be held
    NOTE: It is up to the caller to free the resulting memory! */
char* sn_printf(char* fmt, ...);

/*  Remove all occurances of the unwanted characters in unwanted */
char* remove_unwanted_chars(char* s, const char* unwanted);

/*  Replace all occurances of the unwanted characters in unwanted */
char* replace_unwanted_chars(char* s, const char* unwanted, const char c);

/*  Convert all characters to upper case, in place */
char* s_toupper(char* s);

/*  Convert all characters to lower case, in place */
char* s_tolower(char* s);

/*  Find and return the index of the char `c` in s; if not found, returns -1 */
int find(char* s, char c);

/*  Find and return the last index of char `c` in s; if not found, returns -1 */
int find_reverse(char* s, char c);

/*  Allocate a new memory location and concat both s1 and s2 into it;
    NOTE: It is up to the caller to free the resulting memory! */
char* s_concat(char* s1, char* s2);

/*  Append the contents of s2 onto s1; s1 must be dynamically allocated.
    s1 will be expanded to include both s1 and s2. */
char* append(char* s1, char* s2);

/*  Perform a standard case sensitive string compare */
int s_cmp(char* s1, char* s2);

/*  Perform an optionally case sensitive or case insensitive string compare */
int s_cmp_alt(char* s1, char* s2, int casesensitive);

/*  Extract a substring of size lenght starting at start */
char* extract_substring(char* s, size_t start, size_t length);

// TODO: everything below!
char* s_extract_substring(char* s, char* sub, size_t length);


#endif /* END BARRUST_STRING_LIBRARY_H__ */
