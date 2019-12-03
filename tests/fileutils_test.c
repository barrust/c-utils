#include <stdio.h>
#include "../src/fileutils.h"

int main(int argc, char const *argv[]) {

    file_t f = f_init("./tests/bitarray_test.c");
    // file_t f = f_init("./tests");
    printf("base dir:  %s\n", f_basedir(f));
    printf("filename:  %s\n", f_filename(f));
    printf("filesize:  %lu\n", f_filesize(f));
    printf("extension: %s\n", f_extension(f));
    return 0;
}
