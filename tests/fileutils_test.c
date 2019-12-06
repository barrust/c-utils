#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "../src/fileutils.h"


char* test_dir = NULL;
char* test_dir_rel = NULL;

void test_setup(void) {
    char* curr_dir = fs_resolve_path("./");
    int len = strlen(curr_dir);
    if (strncmp(&curr_dir[len - 4], "test", 4) != 0) {
        char* tmp = realloc(curr_dir, len + 6);
        strcpy(tmp + len, "test/");
        curr_dir = tmp;
        tmp = NULL;

        test_dir_rel = calloc(12, sizeof(char));
        snprintf(test_dir_rel, 12, "./test/tmp/");
    } else {
        test_dir_rel = calloc(7, sizeof(char));
        snprintf(test_dir_rel, 7, "./tmp/");
    }

    test_dir = calloc(strlen(curr_dir) + 5, sizeof(char));
    snprintf(test_dir, strlen(curr_dir) + 5, "%s%s", curr_dir, "tmp/");
    free(curr_dir);
}

void test_teardown(void) {
    free(test_dir);
    free(test_dir_rel);
}


MU_TEST(test_default_setup) {
    mu_assert_string_eq(NULL, NULL);
}


/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_default_setup);
}



int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}
