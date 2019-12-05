#include <stdio.h>
#include "../src/fileutils.h"

int main(int argc, char const *argv[]) {

    int res = fs_mkdir_alt("./tests/something/anotherlevel/andyetanother/", 1, S_IRWXU);
    printf("%d\n", res);
    // file_t f = f_init("./tests/bitarray_test.c");
    // // file_t f = f_init("./tests");
    // if (!f) {
    //     printf("Failed to load the file!\n");
    //     return -1;
    // }
    // printf("base dir:  %s\n", f_basedir(f));
    // printf("filename:  %s\n", f_filename(f));
    // printf("filesize:  %lu\n", f_filesize(f));
    // printf("extension: %s\n", f_extension(f));
    // f_free(f);
    return 0;
}
