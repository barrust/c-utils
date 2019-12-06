#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
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
int     __fs_mkdir(const char* path, mode_t mode);



int fs_identify_path(const char* path) {
    if (path == NULL)
        return FS_NOT_VALID;

    errno = 0;
    struct stat stats;
    if (stat(path, &stats) == -1) {
        if (errno == ENOENT)
            return FS_NO_EXISTS;
    }
    if (S_ISDIR(stats.st_mode) != 0)
        return FS_DIRECTORY;
    if (S_ISREG(stats.st_mode) != 0)
        return FS_FILE;
    if (S_ISLNK(stats.st_mode) != 0)
        return FS_SYMLINK;
    return FS_NOT_VALID;
}

char* fs_resolve_path(const char* path) {
    if (path == NULL)
        return NULL;

    if (strlen(path) == 1 && strcmp(path, ".") == 0)
        return fs_cwd();

    char* new_path = NULL;
    char* tmp = __str_duplicate(path);
    int pos = __str_find_reverse(tmp, '/');
    while (pos != -1) {
        tmp[pos] = '\0';
        char* p = realpath(tmp, NULL);
        if (p != NULL) {
            char* s = tmp + (pos + 1);
            int p_len = strlen(p), t_len = strlen(s);
            new_path = calloc(p_len + t_len + 3, sizeof(char));  // include slash x2 and \0
            snprintf(new_path, p_len + 2 + t_len, "%s/%s", p, s);
            free(p);
            break;
        }
        int tmp_pos = __str_find_reverse(tmp, '/');
        pos[tmp] = '/';
        pos = tmp_pos;
    }
    free(tmp);

    // ensure no trailing '/'
    int len = strlen(new_path);
    if (new_path[len - 1] == '/')
        new_path[len - 1] = '\0';

    return new_path;
}

char* fs_cwd() {
    size_t malsize = 16; // some defult power of 2...
    char* buf = malloc(malsize * sizeof(char));

    while(getcwd(buf, malsize) == NULL && errno == ERANGE) {
        malsize *= 2;
        buf = (char*)realloc(buf, malsize * sizeof(char));
    }
    return buf;
}

int fs_rename(const char* path, const char* new_path) {
    if (path == NULL || new_path == NULL)
        return FS_NOT_VALID;

    errno = 0;
    int res = rename(path, new_path);
    if (res == 0)
        return FS_SUCCESS;
    if (errno == ENOENT)
        return FS_NO_EXISTS;
    return FS_FAILURE;
}

int fs_move(const char* path, const char* new_path) {
    return fs_rename(path, new_path);
}

int fs_touch(const char* path) {
    return fs_touch_alt(path, S_IRWXU | S_IRGRP | S_IWGRP);
}

int fs_touch_alt(const char* path, mode_t mode) {
    if (path == NULL)
        return FS_NOT_VALID;

    open(path, O_CREAT, mode);
    if (fs_identify_path(path) == FS_FILE)
        return FS_SUCCESS;
    return FS_FAILURE;
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
        return __fs_mkdir(path, mode);
    }

    // need to start by finding a way to resolve the relative paths!
    char* new_path = fs_resolve_path(path);
    if (new_path == NULL)
        return FS_NOT_VALID;

    // add a trailing '/' for the loop to work!
    len = strlen(new_path);
    char* tmp = realloc(new_path, len + 2);
    tmp[len] = '/';
    tmp[len + 1] = '\0';
    new_path = tmp;
    tmp = NULL;

    // printf("new_path: %s\n", new_path);
    char* p;
    for (p = strchr(new_path + 1, '/'); p != NULL; p = strchr(p + 1, '/')) {
        *p = '\0';
        int res = __fs_mkdir(new_path, mode);
        if (res == FS_FAILURE) {
            free(new_path);
            return FS_FAILURE;
        }
        printf("tmp_path: %s\n", new_path);
        *p = '/';
    }
    free(new_path);
    return FS_EXISTS;
}

char* fs_mode_to_string(mode_t mode) {
    return fs_mode_to_string_alt(mode, NULL);
}

char* fs_mode_to_string_alt(mode_t mode, char* res) {
    char* tmp;
    if (res == NULL)
        tmp = calloc(10, sizeof(char));
    else
        tmp = res;

    tmp[0] = S_ISDIR(mode) == 0 ? '-' : 'd';
    tmp[1] = mode & S_IRUSR ? 'r' : '-';
    tmp[2] = mode & S_IWUSR ? 'w' : '-';
    tmp[3] = mode & S_IXUSR ? 'x' : '-';
    tmp[4] = mode & S_IRGRP ? 'r' : '-';
    tmp[5] = mode & S_IWGRP ? 'w' : '-';
    tmp[6] = mode & S_IXGRP ? 'x' : '-';
    tmp[7] = mode & S_IROTH ? 'r' : '-';
    tmp[8] = mode & S_IWOTH ? 'w' : '-';
    tmp[9] = mode & S_IXOTH ? 'x' : '-';
    tmp[10] = '\0';
    return tmp;
}

mode_t fs_string_to_mode(const char* s) {
    mode_t res = 0;
    if (s == NULL || strlen(s) != 10)
        return FS_FAILURE;
    // skip the directory char
    res |= s[1] == 'r' ? S_IRUSR : 0;
    res |= s[2] == 'w' ? S_IWUSR : 0;
    res |= s[3] == 'x' ? S_IXUSR : 0;
    res |= s[4] == 'r' ? S_IRGRP : 0;
    res |= s[5] == 'w' ? S_IWGRP : 0;
    res |= s[6] == 'x' ? S_IXGRP : 0;
    res |= s[7] == 'r' ? S_IROTH : 0;
    res |= s[8] == 'w' ? S_IWOTH : 0;
    res |= s[9] == 'x' ? S_IXOTH : 0;
    return res;
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

mode_t f_mode(file_t f) {
    return f->mode;
}


/*******************************************************************************
*   Directory Objects
*******************************************************************************/




/*******************************************************************************
*   PRIVATE FUNCTIONS
*******************************************************************************/
int __fs_mkdir(const char* path, mode_t mode) {
    if (mkdir(path, mode) == -1) {
        if (errno != EEXIST) {
            return FS_FAILURE;
        }
    }
    return FS_EXISTS;
}

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