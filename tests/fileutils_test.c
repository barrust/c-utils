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
static int   __make_test_file(char* s, size_t len, char c);
static void  __sort(char** arr, int n);
static int   __cmp_str(const void* a, const void* b);


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
    mu_assert_int_eq('p', path[len-1]);  /* make sure no trailing '/' */
    free(path);

    path = fs_resolve_path(".");
    char* cwd = fs_cwd();
    mu_assert_string_eq(cwd, path);
    free(path);
    free(cwd);
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

MU_TEST(test_resolve_path_null) {
    mu_assert_string_eq(NULL, fs_resolve_path(NULL));
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

    mu_assert_int_eq(FS_NOT_VALID, fs_identify_path(NULL));
}

/*******************************************************************************
*    Test get / set permissions
*******************************************************************************/
MU_TEST(test_get_permissions) {
    char* filepath = __str_snprintf("%s/test.txt", test_dir);
    char* filepath2 = __str_snprintf("%s/test-2.txt", test_dir);
    int vals[] = {0664, 0644};
    mu_assert_int_in(vals, 2, fs_get_permissions(filepath));
    mu_assert_int_eq(FS_NOT_VALID, fs_get_permissions(NULL));
    mu_assert_int_eq(FS_NO_EXISTS, fs_get_permissions(filepath2));
    /* test a directory */
    int dir_vals[] = {0775, 0755};
    mu_assert_int_in(dir_vals, 2, fs_get_permissions(test_dir));
    free(filepath);
    free(filepath2);
}

MU_TEST(test_set_permissions) {
    char* filepath = __str_snprintf("%s/test-3.txt", test_dir);
    /* test errors */
    mu_assert_int_eq(FS_NOT_VALID, fs_set_permissions(NULL, 0777));

    /* make new file and test */
    fs_touch_alt(filepath, 0666);
    mu_assert_int_eq(0666, fs_get_permissions(filepath));
    mu_assert_int_eq(FS_SUCCESS, fs_set_permissions(filepath, 0777));
    mu_assert_int_eq(0777, fs_get_permissions(filepath));
    fs_remove_file(filepath);
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

    mu_assert_int_eq(FS_INVALID_MODE, fs_string_to_mode("-rw-rw-"));
}

/*******************************************************************************
*    Test touch
*******************************************************************************/
MU_TEST(test_touch) {
    char* filepath = __str_snprintf("%s/no-test.txt", test_dir);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    mu_assert_int_eq(FS_SUCCESS, fs_touch(filepath));
    mu_assert_int_eq(FS_FILE, fs_identify_path(filepath));
    mu_assert_int_eq(FS_NOT_VALID, fs_touch(NULL));
    fs_remove_file(filepath);

    free(filepath);
}

/*******************************************************************************
*    Test remove
*******************************************************************************/
MU_TEST(test_remove) {
    char* filepath = __str_snprintf("%s/no-test.txt", test_dir);
    mu_assert_int_eq(FS_NOT_VALID, fs_remove_file(test_dir));
    fs_touch(filepath);
    mu_assert_int_eq(FS_SUCCESS, fs_remove_file(filepath));
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
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));  /* make sure no longer there */
    mu_assert_int_eq(FS_FILE, fs_identify_path(new_filepath));  /* make sure this one is! */
    fs_remove_file(new_filepath);

    /* Test bad input */
    mu_assert_int_eq(FS_NOT_VALID, fs_rename(filepath, NULL));
    mu_assert_int_eq(FS_NOT_VALID, fs_rename(NULL, filepath));
    mu_assert_int_eq(FS_NO_EXISTS, fs_rename(filepath, new_filepath));

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
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));  /* make sure no longer there */
    mu_assert_int_eq(FS_FILE, fs_identify_path(new_filepath));  /* make sure this one is! */
    unlink(new_filepath);

    free(filepath);
    free(new_filepath);
}

/*******************************************************************************
*    Test mkdir
*******************************************************************************/
MU_TEST(test_mkdir_errors) {
    mu_assert_int_eq(FS_NOT_VALID, fs_mkdir(NULL, false)); /* Test the passing of NULL */
    mu_assert_int_eq(FS_NOT_VALID, fs_mkdir("", false));   /* Test an empty string */
    mu_assert_int_eq(FS_EXISTS, fs_mkdir(test_dir, false));   /* Test existing dir */
}

MU_TEST(test_mkdir_non_recursive) {
    char* filepath = __str_snprintf("%s/test/", test_dir);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));   /* Test missing dir */
    mu_assert_int_eq(FS_SUCCESS, fs_mkdir(filepath, false)); /* start with non-recursive; one level */
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(filepath));
    int vals[] = {0744, 0764};
    mu_assert_int_in(vals, 2, fs_get_permissions(filepath));

    rmdir(filepath);  /* replace with the fs_rmdir once it is written */
    free(filepath);
}

MU_TEST(test_mkdir_non_recursive_error) {
    char* filepath = __str_snprintf("%s/test/second/", test_dir);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));   /* Test missing dir */
    mu_assert_int_eq(FS_FAILURE, fs_mkdir(filepath, false)); /* start with non-recursive; multi-level */
    free(filepath);
}

MU_TEST(test_mkdir_recursive) {
    /* Build filepaths we can test */
    char* filepath = __str_snprintf("%s/test-rec", test_dir);
    char* filepath2 = __str_snprintf("%s/foo", filepath);
    char* filepath3 = __str_snprintf("%s/bar", filepath2);
    /* ensure directories are missing */
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath3));
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath2));
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));

    mu_assert_int_eq(FS_SUCCESS, fs_mkdir(filepath3, true));

    /* ensure directories are now present */
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(filepath));
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(filepath2));
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(filepath3));
    /* check the new directories permissions */
    int vals[] = {0744, 0764};
    mu_assert_int_in(vals, 2, fs_get_permissions(filepath));
    mu_assert_int_in(vals, 2, fs_get_permissions(filepath2));
    mu_assert_int_in(vals, 2, fs_get_permissions(filepath3));

    /* clean up! replace with the fs_rmdir once it is written */
    rmdir(filepath3);
    rmdir(filepath2);
    rmdir(filepath);
    free(filepath);
    free(filepath2);
    free(filepath3);
}

/*******************************************************************************
*    Test rmdir
*******************************************************************************/
MU_TEST(test_rmdir_errors) {
    char* filepath = __str_snprintf("%s/test.txt", test_dir);
    mu_assert_int_eq(FS_NOT_VALID, fs_rmdir(filepath));
    free(filepath);

    filepath = __str_snprintf("%s/nodir", test_dir);  // doesn't exist, nothing to do!
    mu_assert_int_eq(FS_NO_EXISTS, fs_rmdir(filepath));
    free(filepath);

    mu_assert_int_eq(FS_NOT_EMPTY, fs_rmdir_alt(test_dir, false));
}

MU_TEST(test_rmdir_non_recursive) {
    char* filepath = __str_snprintf("%s/newlevl", test_dir);

    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    fs_mkdir(filepath, false);
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(filepath));

    fs_rmdir(filepath);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    free(filepath);
}


MU_TEST(test_rmdir_recursive) {
    char* path = __str_snprintf("%s/newlevl", test_dir);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(path));
    fs_mkdir(path, false);
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(path));

    char* full_depth = __str_snprintf("%s/a/b/c/d/e/f/g", path);
    fs_mkdir(full_depth, true);
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(full_depth));

    /* add some files */
    char* tmp = __str_snprintf("%s/a.txt", full_depth);
    int len = strlen(tmp);
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'a'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'b'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'c'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'd'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'e'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'f'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'g'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'h'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'i'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'j'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'k'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'l'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'm'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'n'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'o'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'p'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'q'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'r'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 's'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 't'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'u'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'v'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'w'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'x'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'y'));
    mu_assert_int_eq(FS_FILE, __make_test_file(tmp, len, 'z'));

    mu_assert_int_eq(FS_FILE, fs_identify_path(tmp));

    fs_rmdir_alt(path, true);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(path));
    free(path);
    free(full_depth);
    free(tmp);
}

MU_TEST(test_list_dir) {
    int items;
    char** recs = fs_list_dir(test_dir, &items);
    mu_assert_int_eq(3, items);
    __sort(recs, 3);
    mu_assert_string_eq(".gitkeep", recs[0]);
    mu_assert_string_eq("lvl2", recs[1]);
    mu_assert_string_eq("test.txt", recs[2]);

    int i;
    for(i = 0; i < items; i++)
        free(recs[i]);
    free(recs);


    /* test the error case */
    char** val = fs_list_dir("./foo", &items);
    mu_assert_string_eq(NULL, (void*)val);
    mu_assert_int_eq(0, items);
}

MU_TEST(test_combine_filepath) {
    char* filepath = __str_snprintf("%s/test.txt", test_dir);

    /* test error cases */
    mu_assert_string_eq(NULL, fs_combine_filepath(NULL, NULL));
    char* res = fs_combine_filepath(NULL, "test.txt");
    mu_assert_string_eq("test.txt", res);
    free(res);
    res = fs_combine_filepath("./", NULL);
    mu_assert_string_eq("./", res);
    free(res);

    /* test actuals */
    res = fs_combine_filepath(test_dir, "test.txt");
    mu_assert_string_eq(filepath, res);
    free(res);
    free(filepath);

    /* other permutations */
    char tmp[1024] = {0};
    fs_combine_filepath_alt("./test", NULL, tmp);
    mu_assert_string_eq("./test", tmp);
    fs_combine_filepath_alt(NULL, "./t", tmp);
    mu_assert_string_eq("./t", tmp);
    fs_combine_filepath_alt("./test/a/", "foo.txt", tmp);
    mu_assert_string_eq("./test/a/foo.txt", tmp);
}

/***************************************************************************
*   file_t - usage
***************************************************************************/
MU_TEST(test_file_t_init) {
    char* filepath = __str_snprintf("%s/test.txt", test_dir);
    file_t f = f_init(filepath);

    /* ensure things are correct! */
    mu_assert_string_eq(filepath, f_absolute_path(f));  /* vad test, but this is already absolute */
    mu_assert_string_eq("test.txt", f_filename(f));
    mu_assert_string_eq(test_dir, f_basedir(f));
    mu_assert_string_eq("txt", f_extension(f));
    int vals[] = {0644, 0664};
    mu_assert_int_in(vals, 2, f_permissions(f)); /* 0664 is the value for linux, 0644 OSX */
    mu_assert_int_eq(3259 , f_filesize(f));
    mu_assert_int_eq(false , f_is_symlink(f));
    /* haven't loaded the file, so these should be the defaults! */
    mu_assert_int_eq(0 , f_number_lines(f));
    mu_assert_string_eq(NULL , f_buffer(f));
    mu_assert(f_lines(f) == NULL, "Expected lines to be NULL, if was not...");
    free(filepath);
    f_free(f);
}

MU_TEST(test_file_t_init_non_file) {
    file_t f = f_init(test_dir);
    mu_assert_string_eq(NULL, (void*)f);

    char* filepath = __str_snprintf("%s/test-2.txt", test_dir);
    f = f_init(filepath);
    mu_assert_string_eq(NULL, (void*)f);
    free(filepath);
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
    /* check line lengths */
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
    /*
    size_t i;
    for (i = 0; i < f_number_lines(f); i++) {
         printf("Line %lu: %s\n", i, buf[i]);
    } */

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
    /* cwd */
    MU_RUN_TEST(test_cwd);

    /* resolve */
    MU_RUN_TEST(test_setup_resolve_paths);
    MU_RUN_TEST(test_resolve_path_file);
    MU_RUN_TEST(test_resolve_path_no_exist);
    MU_RUN_TEST(test_resolve_path_null);

    /* combine filepath */
    MU_RUN_TEST(test_combine_filepath);

    /* fs_identify_path */
    MU_RUN_TEST(test_identify_path);

    /* get / set permissions */
    MU_RUN_TEST(test_get_permissions);
    MU_RUN_TEST(test_set_permissions);

    /* mode conversions */
    MU_RUN_TEST(test_mode_to_string);
    MU_RUN_TEST(test_string_to_mode);

    /* touch */
    MU_RUN_TEST(test_touch);

    /* rename / move */
    MU_RUN_TEST(test_rename);
    MU_RUN_TEST(test_move);

    /* remove file */
    MU_RUN_TEST(test_remove);

    /* mkdir */
    MU_RUN_TEST(test_mkdir_errors);
    MU_RUN_TEST(test_mkdir_non_recursive_error);
    MU_RUN_TEST(test_mkdir_non_recursive);
    MU_RUN_TEST(test_mkdir_recursive);

    /* rmdir */
    MU_RUN_TEST(test_rmdir_errors);
    MU_RUN_TEST(test_rmdir_non_recursive);
    MU_RUN_TEST(test_rmdir_recursive);

    /* list dir */
    MU_RUN_TEST(test_list_dir);

    /***************************************************************************
    *   file_t
    ***************************************************************************/
    MU_RUN_TEST(test_file_t_init);
    MU_RUN_TEST(test_file_t_init_non_file);
    MU_RUN_TEST(test_file_t_read_file);
    MU_RUN_TEST(test_file_t_parse_lines);

    /***************************************************************************
    *   directory_t
    ***************************************************************************/

}



int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
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
        return NULL;

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
    size_t len = strlen(s);
    char* buf = malloc((len + 1) * sizeof(char));
    if (buf == NULL)
        return NULL;
    strcpy(buf, s);
    buf[len] = '\0';
    return buf;
}

static int __make_test_file(char* s, size_t len, char c) {
    /* very specific filename changes... */
    s[len - 5] = c;
    fs_touch(s);
    return fs_identify_path(s);
}

// Function to sort the array
void __sort(char** arr, int n) {
    qsort(arr, n, sizeof(const char*), __cmp_str);
}

static int __cmp_str(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}
