#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "stringlib.h"


char* s_duplicate(char* s) {
    size_t len = strlen(s);  // ensure room for NULL terminated
    char* buf = malloc((len + 1) * sizeof(char));
    if (buf == NULL)
        return NULL;
    strncpy(buf, s, len);
    buf[len] = '\0';
    return buf;
}


size_t s_trim(char* s) {
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


char* s_standardize_whitespace(char* s, const char c) {
    size_t i = 0;
    while (s[i] != '\0') {
        if (isspace(s[i]))
            s[i] = c;
        i++;
    }
    return s;
}


char* s_snprintf(char* fmt, ...) {
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

char* s_remove_unwanted_chars(char* s, const char* unwanted) {
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

        if (changes == 0) {
            ++i;
            ++j;
        }
    }
    s[i] = '\0';
    return s;
}


char* s_replace_unwanted_chars(char* s, const char* unwanted, const char c) {
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


int s_find(char* s, char c) {
    char* loc = strchr(s, c);
    if (loc == NULL)
        return -1;
    return loc - s;
}


int s_find_reverse(char* s, char c) {
    char* loc = strrchr(s, c);
    if (loc == NULL)
        return -1;
    return loc - s;
}


int s_find_str(char* s, char* sub) {
    char* loc = strstr(s, sub);
    if (loc == NULL)
        return -1;
    return loc - s;
}


char* s_append(char* s1, char* s2) {
    size_t len =  strlen(s2);
    char* res = realloc(s1, strlen(s1) + len + 1);
    strncat(res, s2, len);
    // set s1 pointer to the res pointer
    s1 = res;
    res = NULL;
    return s1;
}


char* s_concat(char* s1, char* s2) {
    char* ret = s_duplicate(s1);
    return s_append(ret, s2);
}


int s_cmp(char* s1, char* s2) {
    return s_cmp_alt(s1, s2, CASE_SENSITIVE);  // we want it case sensitive
}


int s_cmp_alt(char* s1, char* s2, int casesensitive) {
    if (casesensitive) {
        return strcmp(s1, s2);
    }

    char* t1 = s_duplicate(s1);
    char* t2 = s_duplicate(s2);
    int res = strcmp(s_tolower(t1), s_tolower(t2));

    free(t1);
    free(t2);
    return res;
}


char* s_extract_substring(char* s, size_t start, size_t length) {
    size_t len = strlen(s);
    if (start >= len)
        return NULL;
    if (start + length > len)
        return s_duplicate(s + start);

    char* ret = calloc(length + 1, sizeof(char));
    return strncpy(ret, s + start, length);
}


char* s_extract_substring_str(char* s, char* sub, size_t length) {
    int start = s_find_str(s, sub);
    if (start == -1)
        return NULL;
    return s_extract_substring(s, start, length);
}


char* s_extract_substring_c(char* s, char c, size_t length) {
    int start = s_find(s, c);
    if (start == -1)
        return NULL;
    return s_extract_substring(s, start, length);
}
