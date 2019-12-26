/*******************************************************************************
*   Use the stringlib library to make some common modifications to a standard,
*   un-formatted string.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "../src/stringlib.h"



int main(int argc, char const *argv[]) {
    /* parse verbose */
    bool verbose = false;
    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        verbose = true;
    }

    printf("Verbosity: %s\n", verbose ? "true" : "false");
    printf("\n***************************************************************\n");

    char* tmp = s_duplicate(" \t The quick\nbrown\tfox jumped  \t over  the  lazy  dog.\t\n\r\f\t   ");

    if (verbose) {
        printf("Stringlib:\tOriginal String:\t[%s]\n", tmp);
        printf("String length: %lu\n", strlen(tmp));
    }

    printf("Stringlib:\tStandardize Whitespace: \t[%s]\n", s_standardize_whitespace(tmp, ' '));
    s_trim(tmp);  /* this returns the number of chars trimmed */
    printf("Stringlib:\tTrim:                   \t[%s]\n", tmp);
    printf("Stringlib:\tTo Uppercase:           \t[%s]\n", s_toupper(tmp));
    printf("Stringlib:\tTo Lowercase:           \t[%s]\n", s_tolower(tmp));
    printf("Stringlib:\tSingle Space:           \t[%s]\n", s_single_space(tmp));
    printf("Stringlib:\tAppend:                 \t[%s]\n", s_append_alt(&tmp, " And nobody noticed!"));

    printf("\n***************************************************************\n");

    printf("Stringlib:\tString Comparisons\n\n");
    printf("Stringlib:\tCase Sensitive:  \tTrue  \t%d\n", s_cmp("apple", "apple"));
    printf("Stringlib:\tCase Sensitive:  \tFalse \t%d\n", s_cmp("apple", "Apple"));
    printf("Stringlib:\tCase Insensitive:\tTrue  \t%d\n", s_cmp_alt("apple", "APPle", true));
    printf("Stringlib:\tCase Insensitive:\tFalse \t%d\n", s_cmp_alt("apple", "apples", true));

    printf("\n***************************************************************\n");

    printf("Stringlib: String Find\n\n");

    printf("Stringlib: Not found                    Index:\t%d\n", s_find(tmp, '?'));
    printf("Stringlib: Found Char 'o'               Index:\t%d\n", s_find(tmp, 'o'));
    printf("Stringlib: Found Str 'ox'               Index:\t%d\n", s_find_str(tmp, "ox"));
    printf("Stringlib: Found Any 'qbr'              Index:\t%d\n", s_find_any(tmp, "qbr"));

    printf("\n***************************************************************\n");
    free(tmp);
}
