#include <stdbool.h>
#include <sys/stat.h>


typedef struct __file_struct file_struct;
typedef struct __file_struct *file_t;


#define FS_NOT_VALID        -10
#define FS_FILE             -9
#define FS_DIRECTORY        -8
#define FS_SYMLINK          -7

#define FS_EXISTS           0
#define FS_NO_EXISTS        -2

#define FS_SUCCESS          0
#define FS_FAILURE          -1


/*******************************************************************************
*   Utility Functions
*******************************************************************************/

int fs_identify_path(const char* path);

char* fs_cwd();  // current working directory

char* fs_resolve_path(const char* path); // NOTE: This is a proposed path, not validated

int fs_rename(const char* path, const char* new_path);
int fs_move(const char* path, const char* new_path);

int fs_touch(const char* path);

int fs_touch_alt(const char* path, mode_t mode);

int fs_mkdir(const char* path, bool recursive);

int fs_mkdir_alt(const char* path, bool recursive, mode_t mode);

// int fs_rmdir(const char* path, bool recursive);  // TODO: implement
// int fs_rmdir_alt(const char* path, bool recursive);  // TODO: implement




/*******************************************************************************
*   Directory Functionality Encapsulation
*******************************************************************************/
// int d_is_directory(const char* path);

// int d_mkdir(const char* path, int recursive);

// char* d_list_dir(const char* path);

// char* d_resolve_directory(const char* path);

/*******************************************************************************
*   File Functionality Encapsulation
*******************************************************************************/

file_t f_init(const char* filepath);

void f_free(file_t f);

const char* f_basedir(file_t f);

const char* f_filename(file_t f);

const char* f_extension(file_t f);

size_t f_filesize(file_t f);

// void f_read_file(file_t f);  // NOTE: put it into the buffer...

// void f_read_lines(file_t f);  // TODO: rethink this name...
