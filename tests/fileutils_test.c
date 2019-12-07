#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "minunit.h"
#include "../src/fileutils.h"


char* test_dir = NULL;
char* test_dir_rel = NULL;

/* private functions */
static char* __str_snprintf(const char* fmt, ...);
static char* __str_extract_substring(const char* s, size_t start, size_t length);
static char* __str_duplicate(const char* s);



void test_setup(void) {
    char* curr_dir = fs_resolve_path("./");
    int len = strlen(curr_dir);
    if (strncmp(&curr_dir[len - 5], "tests", 5) != 0) {
        char* tmp = __str_snprintf("%s/%s", curr_dir, "tests");
        free(curr_dir);
        curr_dir = tmp;
        tmp = NULL;

        test_dir_rel = __str_snprintf("./tests/tmp");
    } else {
        test_dir_rel = __str_snprintf("./tmp");
    }
    test_dir = __str_snprintf("%s/%s", curr_dir, "tmp");
    free(curr_dir);
}

void test_teardown(void) {
    free(test_dir);
    free(test_dir_rel);
}

/*******************************************************************************
*    Test cwd...
*******************************************************************************/
MU_TEST(test_cwd) {
    char* cwd = fs_cwd();
    char* res = fs_resolve_path("./");
    mu_assert_string_eq(res, cwd);
    free(cwd);
    free(res);
}

/*******************************************************************************
*    Test resolve paths
*******************************************************************************/
MU_TEST(test_setup_resolve_paths) {
    char* path = fs_resolve_path(test_dir_rel);
    mu_assert_string_eq(test_dir, path);
    int len = strlen(path);
    mu_assert_int_eq('p', path[len-1]);  // make sure no trailing '/'
    free(path);
}

MU_TEST(test_resolve_path_file) {
    char* tmp = __str_snprintf("%s/tmp/test.txt", test_dir_rel);
    char* res = __str_snprintf("%s/tmp/test.txt", test_dir);
    char* path = fs_resolve_path(tmp);
    mu_assert_string_eq(res, path);
    free(tmp);
    free(path);
    free(res);
}

MU_TEST(test_resolve_path_no_exist) {
    char* tmp = __str_snprintf("%s/blah/test.txt", test_dir_rel);
    char* res = __str_snprintf("%s/blah/test.txt", test_dir);
    char* path = fs_resolve_path(tmp);
    mu_assert_string_eq(res, path);
    free(tmp);
    free(path);
    free(res);
}

/*******************************************************************************
*    Test identify path
*******************************************************************************/
MU_TEST(test_identify_path) {
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(test_dir));
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(test_dir_rel));

    char* filepath = __str_snprintf("%s/test.txt", test_dir);
    mu_assert_int_eq(FS_FILE, fs_identify_path(filepath));
    free(filepath);

    filepath = __str_snprintf("%s/no-test.txt", test_dir);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    free(filepath);
}

/*******************************************************************************
*    Test mode conversions (string octal)
*******************************************************************************/
MU_TEST(test_mode_to_string) {
    char* m_str = fs_mode_to_string(0666);
    mu_assert_string_eq("-rw-rw-rw-", m_str);
    m_str = fs_mode_to_string_alt(0777, m_str);
    mu_assert_string_eq("-rwxrwxrwx", m_str);
    free(m_str);
}

MU_TEST(test_string_to_mode) {
    mu_assert_int_eq(0777, fs_string_to_mode("-rwxrwxrwx"));
    mu_assert_int_eq(0666, fs_string_to_mode("-rw-rw-rw-"));

    mu_assert_int_eq(-1, fs_string_to_mode("-rw-rw-"));
}

/*******************************************************************************
*    Test touch
*******************************************************************************/
MU_TEST(test_touch) {
    char* filepath = __str_snprintf("%s/no-test.txt", test_dir);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));

    mu_assert_int_eq(FS_SUCCESS, fs_touch(filepath));

    mu_assert_int_eq(FS_FILE, fs_identify_path(filepath));
    unlink(filepath);

    free(filepath);
}

/*******************************************************************************
*    Test rename / move     NOTE: rename and move are synonymous
*******************************************************************************/
MU_TEST(test_rename) {
    char* filepath = __str_snprintf("%s/no-test.txt", test_dir);
    char* new_filepath = __str_snprintf("%s/no-test_2.txt", test_dir);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(new_filepath));

    mu_assert_int_eq(FS_SUCCESS, fs_touch(filepath));
    mu_assert_int_eq(FS_FILE, fs_identify_path(filepath));

    mu_assert_int_eq(FS_SUCCESS, fs_rename(filepath, new_filepath));
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));  // make sure no longer there
    mu_assert_int_eq(FS_FILE, fs_identify_path(new_filepath));  // make sure this one is!
    unlink(new_filepath);

    free(filepath);
    free(new_filepath);
}

MU_TEST(test_move) {
    char* filepath = __str_snprintf("%s/no-test.txt", test_dir);
    char* new_filepath = __str_snprintf("%s/lvl2/no-test_2.txt", test_dir);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(new_filepath));

    mu_assert_int_eq(FS_SUCCESS, fs_touch(filepath));
    mu_assert_int_eq(FS_FILE, fs_identify_path(filepath));

    mu_assert_int_eq(FS_SUCCESS, fs_move(filepath, new_filepath));
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));  // make sure no longer there
    mu_assert_int_eq(FS_FILE, fs_identify_path(new_filepath));  // make sure this one is!
    unlink(new_filepath);

    free(filepath);
    free(new_filepath);
}

/***************************************************************************
*   file_t - usage
***************************************************************************/
MU_TEST(test_file_t_init) {
    char* filepath = __str_snprintf("%s/test.txt", test_dir);
    file_t f = f_init(filepath);
    free(filepath);
    // ensure things are correct!
    mu_assert_string_eq("test.txt", f_filename(f));
    mu_assert_string_eq(test_dir, f_basedir(f));
    mu_assert_string_eq("txt", f_extension(f));
    mu_assert_int_eq(0664 , f_permissions(f));
    mu_assert_int_eq(3259 , f_filesize(f));
    mu_assert_int_eq(false , f_is_symlink(f));
    // haven't loaded the file, so these should be the defaults!
    mu_assert_int_eq(0 , f_number_lines(f));
    mu_assert_string_eq(NULL , f_buffer(f));
    mu_assert(f_lines(f) == NULL, "Expected lines to be NULL, if was not...");

    f_free(f);
}

MU_TEST(test_file_t_read_file) {
    char* filepath = __str_snprintf("%s/test.txt", test_dir);
    file_t f = f_init(filepath);
    free(filepath);
    const char* buf = f_read_file(f);

    mu_assert_int_eq(3259, strlen(buf));

    char* tmp = __str_extract_substring(buf, 0, 17);
    mu_assert_string_eq("Lorem ipsum dolor", tmp);
    free(tmp);

    f_free(f);
}

MU_TEST(test_file_t_parse_lines) {
    char* filepath = __str_snprintf("%s/test.txt", test_dir);
    file_t f = f_init(filepath);
    free(filepath);
    char** buf = f_parse_lines(f);

    mu_assert_int_eq(5, f_number_lines(f));
    mu_assert_string_eq(f_buffer(f), buf[0]);
    // check line lengths
    mu_assert_int_eq(843, strlen(buf[0]));
    mu_assert(buf[0][strlen(buf[0]) - 1] == '.', "Failed to get the correct, last character!");
    mu_assert_int_eq(434, strlen(buf[1]));
    mu_assert(buf[1][strlen(buf[1]) - 1] == '.', "Failed to get the correct, last character!");
    mu_assert_int_eq(659, strlen(buf[2]));
    mu_assert(buf[2][strlen(buf[2]) - 1] == '.', "Failed to get the correct, last character!");
    mu_assert_int_eq(862, strlen(buf[3]));
    mu_assert(buf[3][strlen(buf[3]) - 1] == '.', "Failed to get the correct, last character!");
    mu_assert_int_eq(452, strlen(buf[4]));
    mu_assert(buf[4][strlen(buf[4]) - 1] == '.', "Failed to get the correct, last character!");
    // for (size_t i = 0; i < f_number_lines(f); i++) {
    //     printf("Line %lu: %s\n", i, buf[i]);
    // }

    f_free(f);
}




/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    /***************************************************************************
    *   filesystem functions
    ***************************************************************************/
    // cwd
    MU_RUN_TEST(test_cwd);

    // resolve
    MU_RUN_TEST(test_setup_resolve_paths);
    MU_RUN_TEST(test_resolve_path_file);
    MU_RUN_TEST(test_resolve_path_no_exist);

    // fs_identify_path
    MU_RUN_TEST(test_identify_path);

    // mode conversions
    MU_RUN_TEST(test_mode_to_string);
    MU_RUN_TEST(test_string_to_mode);

    // touch
    MU_RUN_TEST(test_touch);

    // rename / move
    MU_RUN_TEST(test_rename);
    MU_RUN_TEST(test_move);

    // mkdir


    /***************************************************************************
    *   file_t
    ***************************************************************************/
    MU_RUN_TEST(test_file_t_init);
    MU_RUN_TEST(test_file_t_read_file);
    MU_RUN_TEST(test_file_t_parse_lines);


}



int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}


/*******************************************************************************
*   PRIVATE FUNCTIONS
*******************************************************************************/
static char* __str_snprintf(const char* fmt, ...) {
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

static char* __str_extract_substring(const char* s, size_t start, size_t length) {
    unsigned int len = strlen(s);
    if (start >= len)
        return NULL;
    if (start + length > len)
        return __str_duplicate(s + start);

    char* ret = calloc(length + 1, sizeof(char));
    return strncpy(ret, s + start, length);
}

static char* __str_duplicate(const char* s) {
    size_t len = strlen(s);  // ensure room for NULL terminated
    char* buf = malloc((len + 1) * sizeof(char));
    if (buf == NULL)
        return NULL;
    strncpy(buf, s, len);
    buf[len] = '\0';
    return buf;
}
