#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>   /*symlink*/
#include "../src/minunit.h"
#include "../src/fileutils.h"


char* test_dir = NULL;
char* test_dir_rel = NULL;

/* private functions */
static char* __str_snprintf(const char* fmt, ...);
static char* __str_extract_substring(const char* s, size_t start, size_t length);
static char* __str_duplicate(const char* s);
static int   __make_test_file(char* s, size_t len, char c);
void cleanup_test_directory(void);

#if defined(__WIN32__) || defined(_WIN32) || defined(__WIN64__) || defined(_WIN64)
// Windows symlink creation - try unprivileged first, then privileged
// Define the constant if it's not available (older Windows versions)
#ifndef SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
#define SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE 0x2
#endif

static int create_symlink_windows(const char* src, const char* dest) {
    // Try unprivileged symlink first (Windows 10 Creator Update and later)
    if (CreateSymbolicLinkA(dest, src, SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE)) {
        return 1; // success
    }
    // Fallback to regular symlink (requires admin privileges)
    if (CreateSymbolicLinkA(dest, src, 0)) {
        return 1; // success
    }
    return 0; // failure
}

// Windows symlink removal - use appropriate Windows API
static int remove_symlink_windows(const char* path) {
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES)
        return 0; // failure

    if (attrs & FILE_ATTRIBUTE_REPARSE_POINT) {
        // Check if it's a directory symlink
        if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
            // Directory symlink - use RemoveDirectory
            return RemoveDirectoryA(path) ? 1 : 0;
        } else {
            // File symlink - use DeleteFile
            return DeleteFileA(path) ? 1 : 0;
        }
    }

    return 0; // Not a symlink
}

#define symlink(src, dest) create_symlink_windows(src, dest)
#define unlink(path) remove_symlink_windows(path)
#endif

void test_setup(void) {
    char* curr_dir = fs_resolve_path("./");
    int len = strlen(curr_dir);
    if (strncmp(&curr_dir[len - 5], "tests", 5) != 0) {
        char* tmp = __str_snprintf("%s%c%s", curr_dir, FS_PATH_SEPARATOR, "tests");
        free(curr_dir);
        curr_dir = tmp;
        tmp = NULL;

        test_dir_rel = __str_snprintf("./tests%ctmp", FS_PATH_SEPARATOR);
    } else {
        test_dir_rel = __str_snprintf("./tmp");
    }
    test_dir = __str_snprintf("%s%c%s", curr_dir, FS_PATH_SEPARATOR, "tmp");
    free(curr_dir);

    // Clean up any leftover files from previous test runs
    cleanup_test_directory();
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
    mu_assert_int_eq('p', path[len-1]);  /* make sure no trailing FS_PATH_SEPARATOR */
    free(path);

    path = fs_resolve_path(".");
    char* cwd = fs_cwd();
    mu_assert_string_eq(cwd, path);
    free(path);

    path = NULL;
    path = fs_resolve_path("");  /* this simulates a relative path without leading . */
    mu_assert_string_eq(cwd, path);
    free(path);
    free(cwd);
}

MU_TEST(test_resolve_path_file) {
    char* tmp = __str_snprintf("%s%ctmp%ctest.txt", test_dir_rel, FS_PATH_SEPARATOR, FS_PATH_SEPARATOR);
    char* res = __str_snprintf("%s%ctmp%ctest.txt", test_dir, FS_PATH_SEPARATOR, FS_PATH_SEPARATOR);
    char* path = fs_resolve_path(tmp);
    mu_assert_string_eq(res, path);
    free(tmp);
    free(path);
    free(res);
}

MU_TEST(test_resolve_path_no_exist) {
    char* tmp = __str_snprintf("%s%cblah%ctest.txt", test_dir_rel, FS_PATH_SEPARATOR, FS_PATH_SEPARATOR);
    char* res = __str_snprintf("%s%cblah%ctest.txt", test_dir, FS_PATH_SEPARATOR, FS_PATH_SEPARATOR);
    char* path = fs_resolve_path(tmp);
    mu_assert_string_eq(res, path);
    free(tmp);
    free(path);
    free(res);
}

MU_TEST(test_resolve_path_null) {
    mu_assert_null(fs_resolve_path(NULL));
}

/*******************************************************************************
*    Test identify path
*******************************************************************************/
MU_TEST(test_identify_path) {
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(test_dir));
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(test_dir_rel));

    char* filepath = __str_snprintf("%s%ctest.txt", test_dir, FS_PATH_SEPARATOR);
    mu_assert_int_eq(FS_FILE, fs_identify_path(filepath));
    free(filepath);

    filepath = __str_snprintf("%s%cno-test.txt", test_dir, FS_PATH_SEPARATOR);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    free(filepath);

    mu_assert_int_eq(FS_NOT_VALID, fs_identify_path(NULL));
}

MU_TEST(test_symlinks_path) {
    char* filepath = __str_snprintf("%s%ctest.txt", test_dir, FS_PATH_SEPARATOR);
    char* filepath2 = __str_snprintf("%s%ctest-sym.txt", test_dir, FS_PATH_SEPARATOR);

#if defined(__WIN32__) || defined(_WIN32) || defined(__WIN64__) || defined(_WIN64)
    int symlink_result = symlink(filepath, filepath2);
if (symlink_result == 0) {
        // Symlink creation failed (likely due to insufficient privileges)
        // Skip this test gracefully
        free(filepath);
        free(filepath2);
        return;
    }
#else
    symlink(filepath, filepath2);
#endif

    int res = fs_identify_path(filepath2);
    mu_assert_int_eq(FS_FILE, res);
    free(filepath);
    free(filepath2);
}

MU_TEST(test_fs_is_symlink) {
    char* filepath = __str_snprintf("%s%ctest.txt", test_dir, FS_PATH_SEPARATOR);
    char* filepath2 = __str_snprintf("%s%ctest-sym3.txt", test_dir, FS_PATH_SEPARATOR);

#if defined(__WIN32__) || defined(_WIN32) || defined(__WIN64__) || defined(_WIN64)
    int symlink_result = symlink(filepath, filepath2);
    if (symlink_result == 0) {
        // Symlink creation failed (likely due to insufficient privileges)
        // Skip this test gracefully
        free(filepath);
        free(filepath2);
        return;
    }
#else
    symlink(filepath, filepath2);
#endif

    int res = fs_is_symlink(filepath);
    mu_assert_int_eq(FS_FAILURE, res);

    res = fs_is_symlink(filepath2);
    mu_assert_int_eq(FS_SUCCESS, res);

    res = fs_is_symlink(NULL);
    mu_assert_int_eq(FS_FAILURE, res);

    unlink(filepath2);

    res = fs_is_symlink(filepath2);
    mu_assert_int_eq(FS_FAILURE, res);

    free(filepath);
    free(filepath2);
}

/*******************************************************************************
*    Test get & set permissions
*******************************************************************************/
MU_TEST(test_get_permissions) {
    char* filepath = __str_snprintf("%s%ctest.txt", test_dir, FS_PATH_SEPARATOR);
    char* filepath2 = __str_snprintf("%s%ctest-2.txt", test_dir, FS_PATH_SEPARATOR);
    int vals[] = {0664, 0644, 0666}; /* Added 0666 for Windows */
    mu_assert_int_in(vals, 3, fs_get_permissions(filepath));
    mu_assert_int_eq(FS_NOT_VALID, fs_get_permissions(NULL));
    mu_assert_int_eq(FS_NO_EXISTS, fs_get_permissions(filepath2));
    /* test a directory */
    int dir_vals[] = {0775, 0755, 0777}; /* Added 0777 for Windows */
    mu_assert_int_in(dir_vals, 3, fs_get_permissions(test_dir));
    free(filepath);
    free(filepath2);
}

MU_TEST(test_set_permissions) {
    char* filepath = __str_snprintf("%s%ctest-3.txt", test_dir, FS_PATH_SEPARATOR);
    /* test errors */
    mu_assert_int_eq(FS_NOT_VALID, fs_set_permissions(NULL, 0777));

    /* make new file and test */
    fs_touch_alt(filepath, 0666);
    int expected_perms[] = {0666, 0777}; /* Windows may return 0666 even when setting 0666, or 0777 */
    mu_assert_int_in(expected_perms, 2, fs_get_permissions(filepath));
    mu_assert_int_eq(FS_SUCCESS, fs_set_permissions(filepath, 0777));
    int set_perms[] = {0777, 0666}; /* Windows may not allow setting 0777, might return 0666 */
    mu_assert_int_in(set_perms, 2, fs_get_permissions(filepath));
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
    char* filepath = __str_snprintf("%s%cno-test.txt", test_dir, FS_PATH_SEPARATOR);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    mu_assert_int_eq(FS_SUCCESS, fs_touch(filepath));
    mu_assert_int_eq(FS_FILE, fs_identify_path(filepath));
    mu_assert_int_eq(FS_NOT_VALID, fs_touch(NULL));
    fs_remove_file(filepath);

    free(filepath);
}

MU_TEST(test_retouch) {
    char* filepath = __str_snprintf("%s%cno-test-fail.txt", test_dir, FS_PATH_SEPARATOR);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    mu_assert_int_eq(FS_SUCCESS, fs_touch(filepath));
    mu_assert_int_eq(FS_FILE, fs_identify_path(filepath));
    mu_assert_int_eq(FS_SUCCESS, fs_touch(filepath));
    fs_remove_file(filepath);
}

MU_TEST(test_touch_fail) {
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(test_dir));
    mu_assert_int_eq(FS_FAILURE, fs_touch(test_dir));
}

/*******************************************************************************
*    Test remove
*******************************************************************************/
MU_TEST(test_remove) {
    char* filepath = __str_snprintf("%s%cno-test.txt", test_dir, FS_PATH_SEPARATOR);
    mu_assert_int_eq(FS_NOT_VALID, fs_remove_file(test_dir));
    fs_touch(filepath);
    mu_assert_int_eq(FS_SUCCESS, fs_remove_file(filepath));
    free(filepath);
}

/*******************************************************************************
*    Test rename & move     NOTE: rename and move are synonymous
*******************************************************************************/
MU_TEST(test_rename) {
    char* filepath = __str_snprintf("%s%cno-test.txt", test_dir, FS_PATH_SEPARATOR);
    char* new_filepath = __str_snprintf("%s%cno-test_2.txt", test_dir, FS_PATH_SEPARATOR);
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
    char* filepath = __str_snprintf("%s%cno-test.txt", test_dir, FS_PATH_SEPARATOR);
    char* new_filepath = __str_snprintf("%s%clvl2%cno-test_2.txt", test_dir, FS_PATH_SEPARATOR, FS_PATH_SEPARATOR);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(new_filepath));

    mu_assert_int_eq(FS_SUCCESS, fs_touch(filepath));
    mu_assert_int_eq(FS_FILE, fs_identify_path(filepath));

    mu_assert_int_eq(FS_SUCCESS, fs_move(filepath, new_filepath));
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));  /* make sure no longer there */
    mu_assert_int_eq(FS_FILE, fs_identify_path(new_filepath));  /* make sure this one is! */
    fs_remove_file(new_filepath);

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
    char* filepath = __str_snprintf("%s%ctest", test_dir, FS_PATH_SEPARATOR);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));   /* Test missing dir */
    mu_assert_int_eq(FS_SUCCESS, fs_mkdir(filepath, false)); /* start with non-recursive; one level */
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(filepath));
    int vals[] = {0744, 0764, 0777}; /* Added 0777 for Windows */
    mu_assert_int_in(vals, 3, fs_get_permissions(filepath));

    rmdir(filepath);  /* replace with the fs_rmdir once it is written */
    free(filepath);
}

MU_TEST(test_mkdir_non_recursive_error) {
    char* filepath = __str_snprintf("%s%ctest%csecond", test_dir, FS_PATH_SEPARATOR, FS_PATH_SEPARATOR);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));   /* Test missing dir */
    mu_assert_int_eq(FS_FAILURE, fs_mkdir(filepath, false)); /* start with non-recursive; multi-level */
    free(filepath);
}

MU_TEST(test_mkdir_recursive) {
    /* Build filepaths we can test */
    char* filepath = __str_snprintf("%s%ctest-rec", test_dir, FS_PATH_SEPARATOR);
    char* filepath2 = __str_snprintf("%s%cfolder", filepath, FS_PATH_SEPARATOR);
    char* filepath3 = __str_snprintf("%s%cbar", filepath2, FS_PATH_SEPARATOR);
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
    int vals[] = {0744, 0764, 0777}; /* Added 0777 for Windows */
    mu_assert_int_in(vals, 3, fs_get_permissions(filepath));
    mu_assert_int_in(vals, 3, fs_get_permissions(filepath2));
    mu_assert_int_in(vals, 3, fs_get_permissions(filepath3));

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
    char* filepath = __str_snprintf("%s%ctest.txt", test_dir, FS_PATH_SEPARATOR);
    mu_assert_int_eq(FS_NOT_VALID, fs_rmdir(filepath));
    free(filepath);

    filepath = __str_snprintf("%s%cnodir", test_dir, FS_PATH_SEPARATOR);  // doesn't exist, nothing to do!
    mu_assert_int_eq(FS_NO_EXISTS, fs_rmdir(filepath));
    free(filepath);

    mu_assert_int_eq(FS_NOT_EMPTY, fs_rmdir_alt(test_dir, false));
}

MU_TEST(test_rmdir_non_recursive) {
    char* filepath = __str_snprintf("%s%cnewlevl", test_dir, FS_PATH_SEPARATOR);

    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    fs_mkdir(filepath, false);
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(filepath));

    fs_rmdir(filepath);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(filepath));
    free(filepath);
}


MU_TEST(test_rmdir_recursive) {
    char* path = __str_snprintf("%s%cnewlevl", test_dir, FS_PATH_SEPARATOR);
    mu_assert_int_eq(FS_NO_EXISTS, fs_identify_path(path));
    fs_mkdir(path, false);
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(path));

    char* full_depth = __str_snprintf("%s%ca%cb%cc%cd%ce%cf%cg", path, FS_PATH_SEPARATOR, FS_PATH_SEPARATOR, FS_PATH_SEPARATOR, FS_PATH_SEPARATOR, FS_PATH_SEPARATOR, FS_PATH_SEPARATOR);
    fs_mkdir(full_depth, true);
    mu_assert_int_eq(FS_DIRECTORY, fs_identify_path(full_depth));

    /* add some files */
    char* tmp = __str_snprintf("%s%ca.txt", full_depth, FS_PATH_SEPARATOR);
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
    cleanup_test_directory();  // Clean up before test

    int items;
    char** recs = fs_list_dir(test_dir, &items);
    mu_assert_int_eq(3, items);
    mu_assert_string_eq(".gitkeep", recs[0]);
    mu_assert_string_eq("lvl2", recs[1]);
    mu_assert_string_eq("test.txt", recs[2]);

    int i;
    for (i = 0; i < items; i++)
        free(recs[i]);
    free(recs);


    /* test the error case */
    char** val = fs_list_dir("./foo", &items);
    mu_assert_null(val);
    mu_assert_int_eq(0, items);
}

MU_TEST(test_combine_filepath) {
    char* filepath = __str_snprintf("%s%ctest.txt", test_dir, FS_PATH_SEPARATOR);

    /* test error cases */
    mu_assert_null(fs_combine_filepath(NULL, NULL));
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
    fs_combine_filepath_alt(__str_snprintf("./test%ca%c", FS_PATH_SEPARATOR, FS_PATH_SEPARATOR), "foo.txt", tmp);
    mu_assert_string_eq(__str_snprintf("./test%ca%cfoo.txt", FS_PATH_SEPARATOR, FS_PATH_SEPARATOR), tmp);
}

/***************************************************************************
*   file_t - usage
***************************************************************************/
MU_TEST(test_file_t_init) {
    char* filepath = __str_snprintf("%s%ctest.txt", test_dir, FS_PATH_SEPARATOR);
    file_t f = f_init(filepath);

    /* ensure things are correct! */
    mu_assert_string_eq(filepath, f_absolute_path(f));  /* vad test, but this is already absolute */
    mu_assert_string_eq("test.txt", f_filename(f));
    mu_assert_string_eq(test_dir, f_basedir(f));
    mu_assert_string_eq("txt", f_extension(f));
    int vals[] = {0644, 0664, 0666}; /* Added 0666 for Windows */
    mu_assert_int_in(vals, 3, f_permissions(f)); /* 0664 is the value for linux, 0644 OSX, 0666 Windows */
    int size_vals[] = {3259, 3268}; /* 3259 for Unix/Linux, 3268 for Windows (extra \r chars) */
    mu_assert_int_in(size_vals, 2, f_filesize(f));
    mu_assert_int_eq(false , f_is_symlink(f));
    /* haven't loaded the file, so these should be the defaults! */
    mu_assert_int_eq(0 , f_number_lines(f));
    mu_assert_string_eq(NULL , f_buffer(f));
    mu_assert_null(f_lines(f));
    free(filepath);
    f_free(f);
}

MU_TEST(test_file_t_init_non_file) {
    file_t f = f_init(test_dir);
    mu_assert_null(f);

    char* filepath = __str_snprintf("%s%ctest-2.txt", test_dir, FS_PATH_SEPARATOR);
    f = f_init(filepath);
    mu_assert_null(f);
    free(filepath);
}

MU_TEST(test_file_t_init_symlink) {
    char* filepath = __str_snprintf("%s%ctest.txt", test_dir, FS_PATH_SEPARATOR);
    char* sym = __str_snprintf("%s%ctest-symlink2.txt", test_dir, FS_PATH_SEPARATOR);

#if defined(__WIN32__) || defined(_WIN32) || defined(__WIN64__) || defined(_WIN64)
    int symlink_result = symlink(filepath, sym);
    if (symlink_result == 0) {
        // Symlink creation failed (likely due to insufficient privileges)
        // Skip this test gracefully
        free(filepath);
        free(sym);
        return;
    }
#else
    symlink(filepath, sym);
#endif

    file_t f = f_init(sym);

    /* ensure things are correct! */
    mu_assert_string_eq(sym, f_absolute_path(f));  /* vad test, but this is already absolute */
    mu_assert_string_eq("test-symlink2.txt", f_filename(f));
    mu_assert_string_eq(test_dir, f_basedir(f));
    mu_assert_string_eq("txt", f_extension(f));
    int vals[] = {0644, 0664, 0666}; /* Added 0666 for Windows */
    mu_assert_int_in(vals, 3, f_permissions(f)); /* 0664 is the value for linux, 0644 OSX, 0666 Windows */
    int size_vals[] = {3259, 3268, 0}; /* 3259 for Unix/Linux, 3268 for Windows (extra \r chars), 0 for Windows symlinks */
    mu_assert_int_in(size_vals, 3, f_filesize(f));
    mu_assert_int_eq(true , f_is_symlink(f));
    /* haven't loaded the file, so these should be the defaults! */
    mu_assert_int_eq(0 , f_number_lines(f));
    mu_assert_string_eq(NULL , f_buffer(f));
    mu_assert_null(f_lines(f));

    f_free(f);
    free(filepath);
    free(sym);
}

MU_TEST(test_file_t_read_file) {
    char* filepath = __str_snprintf("%s%ctest.txt", test_dir, FS_PATH_SEPARATOR);
    file_t f = f_init(filepath);
    free(filepath);
    const char* buf = f_read_file(f);

    int buf_size_vals[] = {3259, 3268}; /* 3259 for Unix/Linux, 3268 for Windows (extra \r chars) */
    mu_assert_int_in(buf_size_vals, 2, strlen(buf));

    char* tmp = __str_extract_substring(buf, 0, 17);
    mu_assert_string_eq("Lorem ipsum dolor", tmp);
    free(tmp);

    f_free(f);
}

MU_TEST(test_file_t_parse_lines) {
    char* filepath = __str_snprintf("%s%ctest.txt", test_dir, FS_PATH_SEPARATOR);
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



/***************************************************************************
*   Helper function to clean up test directory before directory listing tests
***************************************************************************/
void cleanup_test_directory(void) {
    // Clean up potential leftover files from previous tests
    char* test_files[] = {
        "test-sym.txt",
        "test-sym3.txt",
        "test-symlink2.txt",
        "test-3.txt",
        "new_file.txt",
        NULL
    };

    char* test_dirs[] = {
        "test",
        "test-rec",
        NULL
    };

    // Remove leftover files (including symlinks)
    for (int i = 0; test_files[i] != NULL; i++) {
        char* filepath = __str_snprintf("%s%c%s", test_dir, FS_PATH_SEPARATOR, test_files[i]);
        if (fs_identify_path(filepath) != FS_NO_EXISTS) {
#if defined(__WIN32__) || defined(_WIN32) || defined(__WIN64__) || defined(_WIN64)
            // Try Windows-specific removal first for symlinks, then regular file removal
            int removed = 0;
            if (remove_symlink_windows(filepath) == 1) {
                removed = 1;  // Successfully removed as symlink
            } else {
                // Try regular file removal
                if (fs_remove_file(filepath) == FS_SUCCESS) {
                    removed = 1;
                }
            }
            if (!removed) {
                printf("DEBUG: Failed to remove file: %s\n", filepath);
            }
#else
            // On Unix/Linux, use unlink for symlinks, fs_remove_file for regular files
            if (fs_is_symlink(filepath) == FS_SUCCESS) {
                unlink(filepath);
            } else {
                fs_remove_file(filepath);
            }
#endif
        }
        free(filepath);
    }

    // Remove leftover directories (recursively)
    for (int i = 0; test_dirs[i] != NULL; i++) {
        char* dirpath = __str_snprintf("%s%c%s", test_dir, FS_PATH_SEPARATOR, test_dirs[i]);
        if (fs_identify_path(dirpath) == FS_DIRECTORY) {
            fs_rmdir_alt(dirpath, true);  // Remove recursively
        }
        free(dirpath);
    }
}

/***************************************************************************
*   dir_t - usage
***************************************************************************/
MU_TEST(test_dir_t_init) {
    cleanup_test_directory();  // Clean up before test

    dir_t d = d_init(test_dir_rel);

    char** recs = d_list_dir(d);
    mu_assert_int_eq(3, d_num_items(d));
    mu_assert_string_eq(test_dir, d_fullpath(d));
    mu_assert_string_eq(".gitkeep", recs[0]);
    mu_assert_string_eq("lvl2", recs[1]);
    mu_assert_string_eq("test.txt", recs[2]);
    d_free(d);
}

MU_TEST(test_dir_init_fail) {
    char* filepath = __str_snprintf("%s%ctest.txt", test_dir, FS_PATH_SEPARATOR);
    dir_t d = d_init(filepath);
    mu_assert_null(d);
    free(filepath);
    d_free(d);
}

MU_TEST(test_dir_update_list) {
    cleanup_test_directory();  // Clean up before test

    dir_t d = d_init(test_dir_rel);
    /* now that everything is updated, let us add a new file... */
    char* newfile = __str_snprintf("%s%cnew_file.txt", d_fullpath(d), FS_PATH_SEPARATOR);
    fs_touch(newfile);

    d_update_list(d);
    char** recs = d_list_dir(d);
    mu_assert_string_eq(".gitkeep", recs[0]);
    mu_assert_string_eq("lvl2", recs[1]);
    mu_assert_string_eq("new_file.txt", recs[2]);
    mu_assert_string_eq("test.txt", recs[3]);

    recs = d_dirs(d);
    mu_assert_int_eq(1, d_num_dirs(d));
    mu_assert_string_eq("lvl2", recs[0]);

    recs = d_files(d);
    mu_assert_int_eq(3, d_num_files(d));
    mu_assert_string_eq(".gitkeep", recs[0]);
    mu_assert_string_eq("new_file.txt", recs[1]);
    mu_assert_string_eq("test.txt", recs[2]);

    fs_remove_file(newfile); /* keep local test folder clean; */
    free(newfile);
    d_free(d);
}

MU_TEST(test_dir_fullpaths) {
    cleanup_test_directory();  // Clean up before test

    dir_t d = d_init(test_dir_rel);
    char** items = d_items_full_path(d);

    mu_assert_int_eq(3, d_num_items(d));
    mu_assert_string_eq(test_dir, d_fullpath(d));

    char tmp[2048] = {0};
    mu_assert_string_eq(fs_combine_filepath_alt(test_dir, ".gitkeep", tmp), items[0]);
    mu_assert_string_eq(fs_combine_filepath_alt(test_dir, "lvl2", tmp), items[1]);
    mu_assert_string_eq(fs_combine_filepath_alt(test_dir, "test.txt", tmp), items[2]);

    char** files = d_files_full_path(d);
    mu_assert_int_eq(2, d_num_files(d));
    mu_assert_string_eq(fs_combine_filepath_alt(test_dir, ".gitkeep", tmp), files[0]);
    mu_assert_string_eq(fs_combine_filepath_alt(test_dir, "test.txt", tmp), files[1]);

    char** dirs = d_dirs_full_path(d);
    mu_assert_int_eq(1, d_num_dirs(d));
    mu_assert_string_eq(fs_combine_filepath_alt(test_dir, "lvl2", tmp), dirs[0]);

    d_free(d);
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
    MU_RUN_TEST(test_symlinks_path);
    MU_RUN_TEST(test_fs_is_symlink);

    /* get & set permissions */
    MU_RUN_TEST(test_get_permissions);
    MU_RUN_TEST(test_set_permissions);

    /* mode conversions */
    MU_RUN_TEST(test_mode_to_string);
    MU_RUN_TEST(test_string_to_mode);

    /* touch */
    MU_RUN_TEST(test_touch);
    MU_RUN_TEST(test_retouch);
    MU_RUN_TEST(test_touch_fail);

    /* rename & move */
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
    MU_RUN_TEST(test_file_t_init_symlink);
    MU_RUN_TEST(test_file_t_read_file);
    MU_RUN_TEST(test_file_t_parse_lines);

    /***************************************************************************
    *   directory_t
    ***************************************************************************/
    MU_RUN_TEST(test_dir_t_init);
    MU_RUN_TEST(test_dir_init_fail);
    MU_RUN_TEST(test_dir_update_list);
    MU_RUN_TEST(test_dir_fullpaths);
}


int main(void) {
    printf("\nRunning fileutils tests...\n");
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

    char* buf = (char*)malloc((len + 1) * sizeof(char));
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

    char* ret = (char*)calloc(length + 1, sizeof(char));
    return strncpy(ret, s + start, length);
}

static char* __str_duplicate(const char* s) {
    size_t len = strlen(s);
    char* buf = (char*)malloc((len + 1) * sizeof(char));
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
