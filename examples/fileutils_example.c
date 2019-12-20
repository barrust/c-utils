/*******************************************************************************
*   Demonstrate some of the uses of the fileutils library by building out a
*   directory structure, testing different pieces of information about the
*   files, etc. and then removing the directories and files.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "../src/fileutils.h"


/* private functions */
static void write_data_to_file(const char* filename);


int main() {

    char example_dir[1024] = {0};
    char tmp_dir[1024] = {0};
    char tmp_dir_2[1024] = {0};

    int len, i;


    /*  find out current working directory and then get into the example folder
        if not already there */
    char* cwd = fs_cwd();
    printf("Fileutils:\tCurrent Working Dir:\t%s\n", cwd);
    len = strlen(cwd);
    if (strcmp(cwd + len - 8, "examples") != 0)
        fs_combine_filepath_alt(cwd, "examples", example_dir);
    else
        fs_combine_filepath_alt(cwd, NULL, example_dir);
    printf("Fileutils:\tExamples Dir:    \t%s\n", example_dir);

    /* make a directory structure in one shot */
    fs_combine_filepath_alt(example_dir, "a/b/c/d/e/f/g", tmp_dir);
    fs_combine_filepath_alt(example_dir, "a", tmp_dir_2);
    printf("Fileutils:\tFull Directory:  \t%s\n", tmp_dir);

    /* recursively build the directory structure */
    fs_mkdir(tmp_dir, true);

    /* test that everything was built */
    assert(fs_identify_path(tmp_dir) == FS_DIRECTORY);

    /* touch some files */
    fs_combine_filepath_alt(tmp_dir, "foo.txt", tmp_dir);
    fs_touch(tmp_dir);
    len = strlen(tmp_dir);
    tmp_dir[len - 7] = '\0';
    fs_combine_filepath_alt(tmp_dir, "bar.txt", tmp_dir);
    fs_touch(tmp_dir);

    /* put some content in one of the files */
    write_data_to_file(tmp_dir);

    /* use the file_t object */
    printf("***************************************************************\n");
    printf("Fileutils:\tInput Filename:   \t%s\n", tmp_dir);
    file_t f = f_init(tmp_dir);
    // print some quick stats:
    printf("Fileutils:\tBase Directory:   \t%s\n", f_basedir(f));
    printf("Fileutils:\tFilename:         \t%s\n", f_filename(f));
    printf("Fileutils:\tExtension:        \t%s\n", f_extension(f));
    printf("Fileutils:\tAbsolute Path:    \t%s\n", f_absolute_path(f));
    printf("Fileutils:\tFile Size (bytes):\t%lu\n", f_filesize(f));

    /* read the file into memory */
    f_read_file(f);

    printf("\n%s\n", f_buffer(f));

    /* parse the file into "lines" */
    f_parse_lines(f);
    printf("Fileutils:\tNumber of lines  \t%lu\n", f_number_lines(f));
    for (i = 0; i < f_number_lines(f); i++)
        printf("%s\n", f_lines(f)[i]);

    f_free(f);


    /* clean some things up */
    fs_rmdir_alt(tmp_dir_2, true);
    assert(fs_identify_path(tmp_dir_2) == FS_NO_EXISTS);

    return 0;
}



static void write_data_to_file(const char* filename) {
    FILE* f;
    f = fopen(filename, "w");

    fputs("Lorem ipsum dolor sit amet,\nconsectetur adipiscing elit.\nNam aliquam sem in nisi semper pharetra.\n\r\n\nEtiam quis aliquam ex.\nAliquam justo ante, pharetra id dapibus.\n", f);

    fclose(f);
}
