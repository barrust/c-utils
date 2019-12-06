#include <stdbool.h>
#include <sys/stat.h>


typedef struct __file_struct file_struct;
typedef struct __file_struct *file_t;


#define FS_NOT_VALID        -10
#define FS_FILE             -9
#define FS_DIRECTORY        -8
#define FS_SYMLINK          -7

#define FS_EXISTS            0
#define FS_NO_EXISTS        -2

#define FS_SUCCESS           0
#define FS_FAILURE          -1


/*******************************************************************************
*   Utility Functions
*******************************************************************************/

/*  Identify the type of object pointed to by path
    Returns:
        FS_DIRECTORY    - Path is a directory
        FS_FILE         - Path is a file
        FS_SYMLINK      - Path is a symlink
        FS_NO_EXISTS    - Path does not point to anything
        FS_NOT_VALID    - invalid path name
*/
int fs_identify_path(const char* path);

/*  Returns the current working directory
    NOTE: Up to the user to free the resulting memory */
char* fs_cwd();  // current working directory

/*  Resolve the path provided by turning it into an absolute path
    NOTE: Up to the caller to free the resulting memory
    NOTE: This does not validate the resulting path! */
char* fs_resolve_path(const char* path);
char* fs_absolute_path(const char* path);

/*  Rename or move a file from current path to the new path
    Returns:
        FS_SUCCESS
        FS_FAILURE
        FS_NOT_VALID    - invalid path name
*/
int fs_rename(const char* path, const char* new_path);
int fs_move(const char* path, const char* new_path);

/*  Touch a file at path by creating it if it doesn't already exist
    Returns:
        FS_SUCCESS
        FS_FAILURE
        FS_NOT_VALID    - invalid path name
*/
int fs_touch(const char* path);
int fs_touch_alt(const char* path, mode_t mode);

/*  Make the directory structure pointed to by path; recursively if desired
    Returns:
        FS_EXISTS       - Path already exists or was created and now exists
        FS_FAILURE      - Unable to create a directory
        FS_NOT_VALID    - invalid path name
*/
int fs_mkdir(const char* path, bool recursive);
int fs_mkdir_alt(const char* path, bool recursive, mode_t mode);

/*  Turn the permissions mode_t (int) into a printable format "drwxrwxrwx"
    NOTE: Up to the caller to free the returned memory */
char* fs_mode_to_string(mode_t mode);

/*  Turn the permissions mode_t (int) into a printable format "drwxrwxrwx"
    NOTE: If res is NULL then memory will be allocated and the caller will
          need to free the memory
    NOTE: If res is memory already allocated to hold the string, it must be at
          least 10 characters in length */
char* fs_mode_to_string_alt(mode_t mode, char* res);

/*  Turn a permissions string "drwxrwxrwx" into the mode flag (int) */
mode_t fs_string_to_mode(const char* s);



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

mode_t f_mode(file_t f);

// void f_read_file(file_t f);  // NOTE: put it into the buffer...

// void f_read_lines(file_t f);  // TODO: rethink this name...
