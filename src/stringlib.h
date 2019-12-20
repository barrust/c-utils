#ifndef BARRUST_STRING_LIBRARY_H__
#define BARRUST_STRING_LIBRARY_H__

/*******************************************************************************
***
***	 Author: Tyler Barrus
***	 email:  barrust@gmail.com
***
***	 Version: 0.1.1
***	 Purpose: Single source of common string functions in C
***
***	 License: MIT 2019
***
***	 URL: https://github.com/barrust/c-utils
***
***	 Usage:
***     char* str = s_duplicate("This \t is a \n test!  ");
***     s_single_space(str); // "This is a test!"
***     int pos = s_find_any(str, "!a"); // 8
***     s_reverse(str); // "!tset a si sihT"
***
*******************************************************************************/

#include <stdio.h>

#define CASE_SENSITIVE 1
#define CASE_INSENSITIVE 0

#define STRLIB_VERSION "0.1.0"


void s_free_array_of_strings(char** a, int num);

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
int s_find_str_reverse(const char* s, const char* sub);

/*  Find and return the index of any of the characters in s2; retirm -1 of not found */
int s_find_any(const char* s, const char* s2);

/*  Find and return the last index of any of the characters in s2; retirm -1 of not found */
int s_find_any_reverse(const char* s, const char* s2);

/*  Allocate a new memory location and concat both s1 and s2 into it;
    NOTE: It is up to the caller to free the resulting memory! */
char* s_concat(const char* s1, const char* s2);

/*  Append the contents of s2 onto s1; s1 must be dynamically allocated.
    s1 will be expanded to include both s1 and s2.
    NOTE:   To use s_append you must call like s1 = s_append(s1, s2) or memory
            will be leaked.
    NOTE:   To ensure that s1 is updated to point to the new location after
            memory realloc, use s_append_alt(&s1, s2)*/
char* s_append(char* s1, const char* s2);
char* s_append_alt(char* (*s1), const char* s2);

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

/*  Split the provided string by the delimiter `c`; does not keep empty strings */
char** s_split_string_c(const char* s, const char c, int* num);

/*  Split the provided string by the delimiter `sub`; does not keep empty strings */
char** s_split_string_str(const char* s, const char* sub, int* num);

/*  Split the provided string by any of the delimeters in s2; does not keep any empty strings */
char** s_split_string_any(const char* s, const char* s2, int* num);

/*  Split the provided string by newline characters (\n\r\f); does not keep any empty strings */
char** s_split_lines(const char* s, int* num);

/*  Remove duplicate spaces in a string while also trimming and standardizing whitespaces */
char* s_single_space(char* s);

/*  Count the number of occurances of the delimiter;
    NOTE: This will be >= the size of the split out strings due to the split
          strings functions not keeping blank lines; i.e., \n\n */
int s_find_cnt(const char* s, const char c);
int s_find_cnt_str(const char* s, const char* sub);
int s_find_cnt_any(const char* s, const char* s2);

/*  Find the index of the `idx` delimiter; returns -1 if not found
    NOTE: idx is 1 based (i.e., use 1 for the first instance, 2 for the second) */
int s_find_alt(const char*s, const char c, int idx);
int s_find_alt_str(const char*s, const char* sub, int idx);
int s_find_alt_any(const char*s, const char* s2, int idx);


#endif /* END BARRUST_STRING_LIBRARY_H__ */
