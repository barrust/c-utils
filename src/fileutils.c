#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include "fileutils.h"

typedef struct __file_struct {
    size_t filesize;
    mode_t mode;
    size_t numlines;
    int is_symlink;
    char* basepath;
    char* filename;
    char* extension;
    char* buffer;       // this will hold the whole file and lines will index into it
    char** lines;
} __file_struct;



/* private functions */
char*   __str_duplicate(const char* s);
char*   __str_extract_substring(const char* s, size_t start, size_t length);
int     __str_find_reverse(const char* s, const char c);
void    __parse_file_info(const char* full_filepath, char** filepath, char** filename);
void    __print_out_stat_errno(int e);



int fs_identify_path(const char* path) {
    errno = 0;
    struct stat stats;
    if (stat(path, &stats) == -1) {
        if (errno == ENOENT)
            return FS_NOT_VALID;
    }
    if (S_ISDIR(stats.st_mode) != 0)
        return FS_DIRECTORY;
    if (S_ISREG(stats.st_mode) != 0)
        return FS_FILE;
    if (S_ISLNK(stats.st_mode) != 0)
        return FS_SYMLINK;
    return FS_NOT_VALID;
}

int fs_mkdir(const char* path, bool recursive) {
    return fs_mkdir_alt(path, recursive, S_IRWXU | S_IRGRP | S_IWGRP);
}

int fs_mkdir_alt(const char* path, bool recursive, mode_t mode) {
    size_t len = strlen(path);
    if (path == NULL || len == 0)
        return FS_NOT_VALID;  // do something with this...

    errno = 0;
    struct stat stats;
    int stat_res = stat(path, &stats);
    if (stat_res != -1) {
        return FS_EXISTS;
    }

    if (!recursive) {
        if (mkdir(path, mode) == -1) {
            if (errno != EEXIST)
                return FS_FAILURE;
        }
        return FS_EXISTS;
    }

    // need to start by finding a way to resolve the relative paths!
    char* new_path = NULL;
    char* tmp = __str_duplicate(path);
    int pos = __str_find_reverse(tmp, '/');
    while (pos != -1) {
        tmp[pos] = '\0';
        char* p = realpath(tmp, NULL);
        if (p != NULL) {
            char* s = tmp + (pos + 1);
            int p_len = strlen(p), t_len = strlen(s);
            new_path = calloc(p_len + t_len + 2, sizeof(char));  // include slash and \0
            snprintf(new_path, p_len + 1 + t_len, "%s/%s", p, s);
            free(p);
            break;
        }
        int tmp_pos = __str_find_reverse(tmp, '/');
        pos[tmp] = '/';
        pos = tmp_pos;
    }
    free(tmp);

    if (new_path == NULL)
        return FS_NOT_VALID;

    char* p;
    for (p = strchr(new_path + 1, '/'); p != NULL; p = strchr(p + 1, '/')) {
        *p = '\0';
        // printf("attempt to make%s\n", new_path);
        if (mkdir(new_path, mode) == -1) {
            if (errno != EEXIST) {
                free(new_path);
                return FS_FAILURE;
            }
        }
        *p = '/';
    }
    free(new_path);
    return FS_EXISTS;
}

/*******************************************************************************
*   file Objects
*******************************************************************************/

file_t f_init(const char* filepath) {
    errno = 0;
    struct stat stats;
    if (stat(filepath, &stats) == -1) {
        __print_out_stat_errno(errno);
        return NULL;
    }

    file_t f = calloc(1, sizeof(file_struct));
    // set the defaults
    f->filename = NULL;
    f->extension = NULL;
    f->filesize = 0;
    f->numlines = 0;
    f->extension = NULL;
    f->mode = stats.st_mode;

    if (S_ISDIR(stats.st_mode) != 0) {
        f->basepath = realpath(filepath, NULL);
    } else if (S_ISREG(stats.st_mode) != 0) {
        char* path = NULL;
        __parse_file_info(filepath, &path, &f->filename);
        f->basepath = realpath(path, NULL);
        free(path);
        int ex_pos = __str_find_reverse(f->filename, '.');
        if (ex_pos != -1)
            f->extension = __str_duplicate(f->filename + ex_pos + 1);
        f->filesize = stats.st_size;
    } else if (S_ISLNK(stats.st_mode) != 0) {
        // do something with symlinks?
    }
    return f;
}


void f_free(file_t f) {
    free(f->basepath);
    free(f->filename);
    free(f->extension);
    for (size_t i = 0; i < f->numlines; i++)
        f->lines[i] = NULL;
    free(f->lines);
    free(f->buffer);
    free(f);
}


const char* f_basedir(file_t f) {
    return f->basepath;
}

const char* f_filename(file_t f) {
    return f->filename;
}

const char* f_extension(file_t f) {
    return f->extension;
}

size_t f_filesize(file_t f) {
    return f->filesize;
}


/*******************************************************************************
*   Directory Objects
*******************************************************************************/




/*******************************************************************************
*   PRIVATE FUNCTIONS
*******************************************************************************/
char* __str_duplicate(const char* s) {
    size_t len = strlen(s);  // ensure room for NULL terminated
    char* buf = malloc((len + 1) * sizeof(char));
    if (buf == NULL)
        return NULL;
    strncpy(buf, s, len);
    buf[len] = '\0';
    return buf;
}

int __str_find_reverse(const char* s, const char c) {
    char* loc = strrchr(s, c);
    if (loc == NULL)
        return -1;
    return loc - s;
}

char* __str_extract_substring(const char* s, size_t start, size_t length) {
    unsigned int len = strlen(s);
    if (start >= len)
        return NULL;
    if (start + length > len)
        return __str_duplicate(s + start);

    char* ret = calloc(length + 1, sizeof(char));
    return strncpy(ret, s + start, length);
}

void __parse_file_info(const char* full_filepath, char** filepath, char** filename) {
    // ensure path and filename are not leaking memory
    free(*filepath);
    free(*filename);

    int pathlen = strlen(full_filepath);

    if (full_filepath == NULL || pathlen == 0) {
        (*filepath) = NULL;
        (*filename) = NULL;
        return;
    }

    int slash_loc = __str_find_reverse(full_filepath, '/');
    if (slash_loc == -1) {
        (*filepath) = __str_duplicate(".");
        (*filename) = __str_duplicate(full_filepath);
        return;
    }

    *filepath = __str_extract_substring(full_filepath, 0, slash_loc + 1);
    *filename = __str_extract_substring(full_filepath, slash_loc + 1, pathlen);
    return;
}

void __print_out_stat_errno(int e) {
    switch(e) {
        case EACCES:
            printf("EACCES\n"); break;
        case EFAULT:
            printf("EFAULT\n"); break;
        case ENAMETOOLONG:
            printf("ENAMETOOLONG\n"); break;
        case ENOENT:
            printf("ENOENT\n"); break;
        case ENOMEM:
            printf("ENOMEM\n"); break;
        case ENOTDIR:
            printf("ENOTDIR\n"); break;
        case EOVERFLOW:
            printf("EOVERFLOW\n"); break;
        case EBADF:
            printf("EBADF\n"); break;
        case EINVAL:
            printf("EINVAL\n"); break;
        default:
            printf("errno: %d\n", errno); break;
    }
}
