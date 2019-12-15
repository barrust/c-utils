#include <string.h>         /* strlen, strcmp, strchr, strncpy, strpbrk */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         /* getcwd */
#include <sys/stat.h>
#include <fcntl.h>          /* O_CREAT */
#include <dirent.h>         /* */
#include <errno.h>
#include "fileutils.h"

typedef struct __file_struct {
    size_t filesize;
    mode_t mode;
    size_t num_lines;
    bool is_symlink;     /* 0 for false, 1 for true */
    char* basepath;
    char* filename;
    char* extension;
    char* buffer;       /* this will hold the whole file and lines will index into it */
    char** lines;
} __file_struct;



/* private functions */
static char*   __str_duplicate(const char* s);
static char*   __str_extract_substring(const char* s, size_t start, size_t length);
static int     __str_find_reverse(const char* s, const char c);
static char**  __str_split_string_any(char* s, const char* s2, size_t* num);
static int     __str_find_any(const char* s, const char* s2);
static size_t  __str_find_cnt_any(const char* s, const char* s2);
static void    __parse_file_info(const char* full_filepath, char** filepath, char** filename);
static void    __free_double_array(char** arr, size_t num_elms);
/* wrapper functions for windows and posix systems support */
static int     __fs_mkdir(const char* path, mode_t mode);
static int     __fs_rmdir(const char* path);
static char**  __fs_list_dir(const char* path, int* elms);



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
            new_path = calloc(p_len + t_len + 3, sizeof(char));  /* include slash x2 and \0 */
            snprintf(new_path, p_len + 2 + t_len, "%s/%s", p, s);
            free(p);
            break;
        }
        int tmp_pos = __str_find_reverse(tmp, '/');
        pos[tmp] = '/';
        pos = tmp_pos;
    }
    free(tmp);

    /* ensure no trailing '/' */
    int len = strlen(new_path);
    if (new_path[len - 1] == '/')
        new_path[len - 1] = '\0';

    return new_path;
}

char* fs_cwd() {
    size_t malsize = 16; /* some defult power of 2... */
    char* buf = malloc(malsize * sizeof(char));
    errno = 0;
    while(getcwd(buf, malsize) == NULL && errno == ERANGE) {
        malsize *= 2;
        char* tmp = realloc(buf, malsize * sizeof(char));
        buf = tmp;
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

    int pfd;
    if ((pfd = open(path, O_CREAT | mode)) == -1) {
        close(pfd);
        return FS_FAILURE;
    }
    close(pfd);

    if (fs_identify_path(path) == FS_FILE) {
        fs_set_permissions(path, mode);
        return FS_SUCCESS;
    }
    return FS_FAILURE;
}

int fs_remove_file(const char* path) {
    int res = fs_identify_path(path);
    if (res != FS_FILE)
        return FS_NOT_VALID;

    res = remove(path);  /* should supports windows and posix OSes */
    if (res == 0)
        return FS_SUCCESS;
    return FS_FAILURE;
}

int fs_mkdir(const char* path, bool recursive) {
    return fs_mkdir_alt(path, recursive, S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH);  /* drwxrw-r-- */
}

int fs_mkdir_alt(const char* path, bool recursive, mode_t mode) {
    if (path == NULL)
        return FS_NOT_VALID;
    size_t len = strlen(path);
    if (len == 0)
        return FS_NOT_VALID;

    errno = 0;
    struct stat stats;
    if (stat(path, &stats) != -1) {
        return FS_EXISTS;
    }

    if (!recursive) {
        return __fs_mkdir(path, mode);
    }

    /* need to start by finding a way to resolve the relative paths! */
    char* new_path = fs_resolve_path(path);
    if (new_path == NULL)
        return FS_NOT_VALID;

    /* add a trailing '/' for the loop to work! */
    len = strlen(new_path);
    char* tmp = realloc(new_path, len + 2);
    tmp[len] = '/';
    tmp[len + 1] = '\0';
    new_path = tmp;
    tmp = NULL;

    char* p;
    for (p = strchr(new_path + 1, '/'); p != NULL; p = strchr(p + 1, '/')) {
        *p = '\0';
        int res = __fs_mkdir(new_path, mode);
        if (res == FS_FAILURE) {
            free(new_path);
            return FS_FAILURE;
        }
        *p = '/';
    }
    free(new_path);
    return FS_EXISTS;
}

int fs_rmdir(const char* path) {
    return fs_rmdir_alt(path, false);  /* do not default to recursive! */
}

int fs_rmdir_alt(const char* path, bool recursive) {
    if (fs_identify_path(path) != FS_DIRECTORY)
        return FS_NOT_VALID;

    if (recursive == false)
        return __fs_rmdir(path);

    /* recursively go through and clean everything up... */
    if (__fs_rmdir(path) == FS_NOT_EMPTY) {
        int num_elms = 0;
        char** paths = __fs_list_dir(path, &num_elms);

        int i;
        for (i = 0; i < num_elms; i++) {
            /* TODO: This should be a function called "fs_combine_path" or something */
            char tmp[1024] = {0};
            strncpy(tmp, path, strlen(path));
            tmp[strlen(path)] = '/';
            strncpy(tmp + strlen(path) + 1, paths[i], strlen(paths[i]));

            int type = fs_identify_path(tmp);
            if (type == FS_FILE) {
                fs_remove_file(tmp);
            } else if (type == FS_DIRECTORY) {
                int val = fs_rmdir_alt(tmp, recursive);
                if (val == FS_FAILURE) {
                    /* free the paths! */
                    __free_double_array(paths, num_elms);

                    return FS_FAILURE;
                }
            } else {
                /* free the paths! */
                __free_double_array(paths, num_elms);

                return FS_FAILURE;  /* something went wrong; a symlink or something else was encountered... */
            }
        }

        /* free the paths! */
        __free_double_array(paths, num_elms);
        fs_rmdir(path);
    }
    return FS_NO_EXISTS;
}

int fs_get_permissions(const char* path) {
    if (path == NULL)
        return FS_NOT_VALID;
    struct stat stats;
    if (stat(path, &stats) == -1) {
        if (errno == ENOENT)
            return FS_NO_EXISTS;
        return FS_FAILURE;
    }
    return stats.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
}

int fs_set_permissions(const char* path, mode_t mode) {
    int res = fs_identify_path(path);
    if (res != FS_FILE && res != FS_DIRECTORY)
        return FS_NOT_VALID;

    res = chmod(path, mode);
    if (res == 0)
        return FS_SUCCESS;
    return FS_FAILURE;
}

char* fs_mode_to_string(mode_t mode) {
    return fs_mode_to_string_alt(mode, NULL);
}

char* fs_mode_to_string_alt(mode_t mode, char* res) {
    char* tmp;
    if (res == NULL)
        tmp = calloc(11, sizeof(char));
    else
        tmp = res;

    tmp[0] = S_ISDIR(mode) == 0 ? '-' : 'd';
    tmp[1] = (mode & S_IRUSR) ? 'r' : '-';
    tmp[2] = (mode & S_IWUSR) ? 'w' : '-';
    tmp[3] = (mode & S_IXUSR) ? 'x' : '-';
    tmp[4] = (mode & S_IRGRP) ? 'r' : '-';
    tmp[5] = (mode & S_IWGRP) ? 'w' : '-';
    tmp[6] = (mode & S_IXGRP) ? 'x' : '-';
    tmp[7] = (mode & S_IROTH) ? 'r' : '-';
    tmp[8] = (mode & S_IWOTH) ? 'w' : '-';
    tmp[9] = (mode & S_IXOTH) ? 'x' : '-';
    tmp[10] = '\0';
    res = tmp;
    tmp = NULL;
    return res;
}

unsigned short fs_string_to_mode(const char* s) {
    unsigned short res = 0;
    if (s == NULL || strlen(s) != 10)
        return FS_INVALID_MODE;
    /* skip the directory char */
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
        /*__print_out_stat_errno(errno); */
        return NULL;
    }

    mode_t mode = stats.st_mode;
    if (S_ISREG(mode) == 0 && S_ISLNK(mode) == 0)
        return NULL; /* it isn't a file or symlink */

    file_t f = calloc(1, sizeof(file_struct));
    /* set the defaults */
    f->filename = NULL;
    f->extension = NULL;
    f->filesize = 0;
    f->num_lines = 0;
    f->lines = NULL;
    f->is_symlink = S_ISLNK(mode) == 0 ? false : true;
    f->mode = mode;

    char* path = NULL;
    __parse_file_info(filepath, &path, &f->filename);
    f->basepath = realpath(path, NULL);
    free(path);
    int ex_pos = __str_find_reverse(f->filename, '.');
    if (ex_pos != -1)
        f->extension = __str_extract_substring(f->filename, ex_pos + 1, strlen(f->filename));
    f->filesize = stats.st_size;

    return f;
}


void f_free(file_t f) {
    free(f->basepath);
    free(f->filename);
    free(f->extension);
    size_t i;
    for (i = 0; i < f->num_lines; i++)
        f->lines[i] = NULL;
    free(f->lines);
    free(f->buffer);
    free(f);
}

bool f_is_symlink(file_t f) {
    return f->is_symlink;
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

unsigned short f_permissions(file_t f) {
    return f->mode & (S_IRWXU | S_IRWXG | S_IRWXO);
}

size_t f_number_lines(file_t f) {
    return f->num_lines;
}

char** f_lines(file_t f) {
    return f->lines;
}

char* f_buffer(file_t f) {
    return f->buffer;
}

const char* f_read_file(file_t f) {
    free(f->buffer);

    int blen = strlen(f->basepath), flen = strlen(f->filename);
    char* full_path = calloc(blen + flen + 2, sizeof(char)); /* '/' and '\0' */
    strcpy(full_path, f->basepath);
    full_path[blen] = '/';
    strcpy(full_path + blen + 1, f->filename);

    FILE* fobj = fopen(full_path, "rb");
    if (fobj == NULL) {
        free(full_path);
        return NULL;
    }
    free(full_path);

    f->buffer = calloc(f->filesize + 1, sizeof(char));
    size_t read = fread(f->buffer, sizeof(char), f->filesize, fobj);
    if (read != f->filesize) {
        fclose(fobj);
        return NULL;
    }
    fclose(fobj);
    return f->buffer;
}

char** f_parse_lines(file_t f) {
    if (f->buffer == NULL) {
        const char* buf = f_read_file(f);
        if (buf == NULL)
            return NULL;
    }

    size_t num = 0;
    char** res = __str_split_string_any(f->buffer, "\n\r\f", &num);
    f->num_lines = num;
    f->lines = res;
    return res;
}

/*******************************************************************************
*   Directory Objects
*******************************************************************************/




/*******************************************************************************
*   PRIVATE FUNCTIONS
*******************************************************************************/
static int __fs_mkdir(const char* path, mode_t mode) {
    errno = 0;
    int res = mkdir(path, mode);
    if (res == -1) {
        if (errno != EEXIST) {
            return FS_FAILURE;
        }
    }
    return FS_EXISTS;
}

static int __fs_rmdir(const char* path) {
    errno = 0;
    int res = rmdir(path);
    if (res == -1) {
        if (errno == EEXIST || errno == ENOTEMPTY)
            return FS_NOT_EMPTY;
        return FS_FAILURE;
    }
    return FS_NO_EXISTS;
}

static char** __fs_list_dir(const char* path, int* elms) {
    int growth_num = 10;
    int cur_size = growth_num;
    char** paths = calloc(cur_size, sizeof(char*));

    DIR *d;
    d = opendir(path);
    int el_num = 0;
    if (d) {
        struct dirent *dir;
        while ((dir = readdir(d)) != NULL) {
            /* need to skip "." and ".." */
            int item_len = strlen(dir->d_name);
            if (item_len == 1 && dir->d_name[0] == '.')
                continue;
            else if (item_len == 2 && strcmp(dir->d_name, "..") == 0)
                continue;
            paths[el_num++] = __str_duplicate(dir->d_name);

            if (el_num == cur_size) {
                cur_size += growth_num;
                char** tmp = realloc(paths, sizeof(char*) * cur_size);
                paths = tmp;
            }
        }
        closedir(d);
    }

    if (cur_size != el_num) {
        char** tmp = realloc(paths, sizeof(char*) * el_num);
        paths = tmp;
    }
    *elms = el_num;
    return paths;
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

static int __str_find_reverse(const char* s, const char c) {
    char* loc = strrchr(s, c);
    if (loc == NULL)
        return -1;
    return loc - s;
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

static char** __str_split_string_any(char* s, const char* s2, size_t* num) {
    const char* find;
    if (s2 == NULL)
        find = " \n\r\f\v\t";
    else
        find = s2;

    size_t max_size = __str_find_cnt_any(s, find);
    char** results = calloc(max_size + 1,  sizeof(char*));
    char* loc = s;
    int cnt = 0;
    int len = __str_find_any(loc, find);

    while (len != -1) {
        if (len == 0) {
            loc[0] = '\0';
            len = __str_find_any(++loc, find);
            continue;
        }
        results[cnt++] = loc;
        loc += len;
        loc[0] = '\0';
        len = __str_find_any(++loc, find);
    }
    if (loc[0] != '\0')
        results[cnt++] = loc;
    *num = cnt;

    char** v = realloc(results, cnt * sizeof(char*));
    return v;
}

static int __str_find_any(const char* s, const char* s2) {
    char* loc = strpbrk(s, s2);
    if (loc == NULL)
        return -1;
    return loc - s;
}

static size_t __str_find_cnt_any(const char* s, const char* s2) {
    size_t res = 0;
    char* loc = strpbrk(s, s2);
    while (loc != NULL) {
        ++res;
        loc = strpbrk(loc + 1, s2);
    }
    return res;
}

static void __free_double_array(char** arr, size_t num_elms) {
    size_t i;
    for (i = 0; i < num_elms; i++) {
        free(arr[i]);
    }
    free(arr);
}

static void __parse_file_info(const char* full_filepath, char** filepath, char** filename) {
    /* ensure path and filename are not leaking memory */
    free(*filepath);
    free(*filename);

    if (full_filepath == NULL || strlen(full_filepath) == 0) {
        (*filepath) = NULL;
        (*filename) = NULL;
        return;
    }

    int pathlen = strlen(full_filepath);

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
