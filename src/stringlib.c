#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "stringlib.h"



void s_free_array_of_strings(char** a, int num) {
    for (int i = 0; i < num; i++)
        free(a[i]);
    free(a);
}


char* s_duplicate(const char* s) {
    size_t len = strlen(s);  // ensure room for NULL terminated
    char* buf = malloc((len + 1) * sizeof(char));
    if (buf == NULL)
        return NULL;
    strncpy(buf, s, len);
    buf[len] = '\0';
    return buf;
}


char* s_reverse(char* s) {
    int j = strlen(s) - 1;
    int i = 0;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        ++i;
        --j;
    }
    return s;
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


char* s_snprintf(const char* fmt, ...) {
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


int s_find(const char* s, const char c) {
    char* loc = strchr(s, c);
    if (loc == NULL)
        return -1;
    return loc - s;
}


int s_find_reverse(const char* s, const char c) {
    char* loc = strrchr(s, c);
    if (loc == NULL)
        return -1;
    return loc - s;
}


int s_find_cnt(const char* s, const char c) {
    int res = 0;
    char* loc = strchr(s, c);
    while (loc != NULL) {
        ++res;
        loc = strchr(loc + 1, c);
    }
    return res;
}


int s_find_alt(const char*s, const char c, int idx) {
    int i = 0;
    char* loc = strchr(s, c);
    while (loc != NULL) {
        ++i;
        if (i == idx)
            break;
        loc = strchr(loc + 1, c);
    }
    if (loc == NULL)
        return -1;
    return loc - s;
}


int s_find_str(const char* s, const char* sub) {
    char* loc = strstr(s, sub);
    if (loc == NULL)
        return -1;
    return loc - s;
}


char* s_strrstr(const char* s, const char* sub) {
    int len = strlen(sub);
    char* res = NULL;
    char* loc = strstr(s, sub);

    while (loc != NULL) {
        res = loc;
        loc = strstr(loc + len, sub);
    }
    return res;
}


int s_find_str_reverse(const char* s, const char* sub) {
    char* loc = s_strrstr(s, sub);
    if (loc == NULL)
        return -1;
    return loc - s;
}


int s_find_cnt_str(const char* s, const char* sub) {
    int res = 0;
    int len = strlen(sub);
    char* loc = strstr(s, sub);
    while (loc != NULL) {
        ++res;
        loc = strstr(loc + len, sub);
    }
    return res;
}


int s_find_alt_str(const char*s, const char* sub, int idx) {
    int i = 0;
    int len = strlen(sub);
    char* loc = strstr(s, sub);
    while (loc != NULL) {
        ++i;
        if (i == idx)
            break;
        loc = strstr(loc + len, sub);
    }
    if (loc == NULL)
        return -1;
    return loc - s;
}


int s_find_any(const char* s, const char* s2) {
    char* loc = strpbrk(s, s2);
    if (loc == NULL)
        return -1;
    return loc - s;
}


int s_find_any_reverse(const char* s, const char* s2) {
    char* res = NULL;
    char* loc = strpbrk(s, s2);
    if (loc == NULL)  // quick exit
        return -1;

    while (loc != NULL) {
        res = loc; // it matches a single element...
        loc = strpbrk(loc + 1, s2);
    }
    return res - s;
}


int s_find_cnt_any(const char* s, const char* s2) {
    int res = 0;
    char* loc = strpbrk(s, s2);
    while (loc != NULL) {
        ++res;
        loc = strpbrk(loc + 1, s2);
    }
    return res;
}


int s_find_alt_any(const char*s, const char* s2, int idx) {
    int i = 0;
    char* loc = strpbrk(s, s2);
    while (loc != NULL) {
        ++i;
        if (i == idx)
            break;
        loc = strpbrk(loc + 1, s2);
    }
    if (loc == NULL)
        return -1;
    return loc - s;
}


char* s_append(char* s1, const char* s2) {
    size_t len =  strlen(s2);
    char* res = realloc(s1, strlen(s1) + len + 1);
    strncat(res, s2, len);
    // set s1 pointer to the res pointer
    s1 = res;
    res = NULL;
    return s1;
}


char* s_concat(const char* s1, const char* s2) {
    char* ret = s_duplicate(s1);
    return s_append(ret, s2);
}


int s_cmp(const char* s1, const char* s2) {
    return s_cmp_alt(s1, s2, CASE_SENSITIVE);  // we want it case sensitive
}


int s_cmp_alt(const char* s1, const char* s2, int casesensitive) {
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


char* s_extract_substring(const char* s, size_t start, size_t length) {
    size_t len = strlen(s);
    if (start >= len)
        return NULL;
    if (start + length > len)
        return s_duplicate(s + start);

    char* ret = calloc(length + 1, sizeof(char));
    return strncpy(ret, s + start, length);
}


char* s_extract_substring_str(const char* s, const char* sub, size_t length) {
    int start = s_find_str(s, sub);
    if (start == -1)
        return NULL;
    return s_extract_substring(s, start, length);
}


char* s_extract_substring_c(const char* s, const char c, size_t length) {
    int start = s_find(s, c);
    if (start == -1)
        return NULL;
    return s_extract_substring(s, start, length);
}


char** s_split_string_c(const char* s, const char c, int* num) {
    int max_size = s_find_cnt(s, c);
    char** results = malloc(max_size * sizeof(char*));  // will be cut down for empty lines...

    const char* loc = s;
    int cnt = 0;
    int len = s_find(loc, c);

    while (len != -1) {
        if (len == 0) {
            len = s_find(++loc, c);
            continue;
        }
        results[cnt++] = s_extract_substring(loc, 0, len);
        loc += len + 1;
        len = s_find(loc, c);
    }
    if (loc[0] != '\0')
        results[cnt++] = s_duplicate(loc);
    *num = cnt;

    char** v = realloc(results, cnt * sizeof(char*));
    return v;
}


char** s_split_string_str(const char* s, const char* sub, int* num) {
    int max_size = s_find_cnt_str(s, sub);
    char** results = malloc(max_size * sizeof(char*));  // will be cut down for empty lines...

    int sub_len = strlen(sub);

    const char* loc = s;
    int cnt = 0;
    int len = s_find_str(loc, sub);

    while (len != -1) {
        if (len == 0) {
            loc += sub_len;
            len = s_find_str(loc, sub);
            continue;
        }
        results[cnt++] = s_extract_substring(loc, 0, len);
        loc += len + sub_len;
        len = s_find_str(loc, sub);
    }
    if (loc[0] != '\0')
        results[cnt++] = s_duplicate(loc);
    *num = cnt;

    char** v = realloc(results, cnt * sizeof(char*));
    return v;
}


char** s_split_string_any(const char* s, const char* s2, int* num) {
    const char* find;
    if (s2 == NULL)
        find = " \n\r\f\v\t";
    else
        find = s2;

    int max_size = s_find_cnt_any(s, find);
    char** results = malloc(max_size * sizeof(char*));
    const char* loc = s;
    int cnt = 0;
    int len = s_find_any(loc, find);

    while (len != -1) {
        if (len == 0) {
            len = s_find_any(++loc, find);
            continue;
        }
        results[cnt++] = s_extract_substring(loc, 0, len);
        loc += len + 1;
        len = s_find_any(loc, find);
    }
    if (loc[0] != '\0')
        results[cnt++] = s_duplicate(loc);
    *num = cnt;

    char** v = realloc(results, cnt * sizeof(char*));
    return v;
}


char** s_split_lines(const char* s, int* num) {
    return s_split_string_any(s, "\n\r\f", num);
}


char* s_single_space(char* s) {
    s_trim(s);

    int i = 0, j = 0, found = 0;
    while (s[j] != '\0') {
        int space = isspace(s[j]);
        if (space != 0 && found > 0) {
            ++found;
            ++j;
        } else if (space != 0) {
            ++found;
            s[i++] = ' ';
            ++j;
        } else {
            s[i++] = s[j++];
            found = 0;
        }
    }
    s[i] = '\0';

    return s;
}
