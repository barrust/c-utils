#ifndef BARRUST_FILE_UTILS_H__
#define BARRUST_FILE_UTILS_H__

/*******************************************************************************
***
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***
***  Version: 0.1.1
***  Purpose: Simple file system library with utility functions
***
***  License: MIT 2019
***
***  URL: https://github.com/barrust/c-utils
***
***  Usage:
***     file_t f = f_init(filepath);
***     f_parse_lines(f);
***     // print all lines from file
***     for (size_t i = 0; i < f_number_lines(f); i++)
***         printf("Line %lu:\t%s\n", i, f_lines[i]);
***     // free memory
***     f_free(f);
***
*******************************************************************************/

#include <stdbool.h>


typedef struct __file_struct file_struct;
typedef struct __file_struct *file_t;

typedef struct __dir_struct dir_struct;
typedef struct __dir_struct *dir_t;


#define FS_INVALID_MODE     65535
#define FS_NOT_VALID        -10
#define FS_FILE             -9
#define FS_DIRECTORY        -8
#define FS_SYMLINK          -7

#define FS_NOT_EMPTY        -6

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
char* fs_cwd();

/*  Resolve the path provided by turning it into an absolute path;
    does not keep filename!
    NOTE: Up to the caller to free the resulting memory
    NOTE: This does not validate the resulting path! */
char* fs_resolve_path(const char* path);

/*  Build and return the canonical absolute path to the file using the path
    and filename provided
    NOTE: Up to the caller to free the resulting memory
    NOTE: Up to the caller to pass in an array large enough for the results "res"
*/
char* fs_combine_filepath(const char* path, const char* filename);
char* fs_combine_filepath_alt(const char* path, const char* filename, char* res);

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

/*  Remove the file to which path points
    Returns:
        FS_SUCCESS
        FS_FAILURE
        FS_NOT_VALID    - invalid path name or not a file
*/
int fs_remove_file(const char* path);

/*  Make the directory structure pointed to by path; recursively if desired
    Returns:
        FS_EXISTS       - Path already exists
        FS_SUCCESS      - Path successfully created
        FS_FAILURE      - Unable to create a directory
        FS_NOT_VALID    - invalid path name
*/
int fs_mkdir(const char* path, bool recursive);
int fs_mkdir_alt(const char* path, bool recursive, mode_t mode);

/*  Return the raw mode including filetype information
    Returns:
        FS_NOT_VALID
        FS_NO_EXISTS
        FS_FAILURE
        mode_t as int
*/
int fs_get_raw_mode(const char* path);

/*  Retrieve the permissions of the file or directory at which path points
    Returns:
        FS_NOT_VALID
        FS_NO_EXISTS
        FS_FAILURE
        mode_t as int
*/
int fs_get_permissions(const char* path);

/*  Set the permission of the file or directory
    Returns:
        FS_NOT_VALID
        FS_SUCCESS
        FS_FAILURE
*/
int fs_set_permissions(const char* path, mode_t mode);

/*  Turn the permissions mode_t (int) into a printable format "drwxrwxrwx"
    NOTE: Up to the caller to free the returned memory */
char* fs_mode_to_string(mode_t mode);

/*  Turn the permissions mode_t (int) into a printable format "drwxrwxrwx"
    NOTE: If res is NULL then memory will be allocated and the caller will
          need to free the memory
    NOTE: If res is memory already allocated to hold the string, it must be at
          least 11 characters in length */
char* fs_mode_to_string_alt(mode_t mode, char* res);

/*  Turn a permissions string "drwxrwxrwx" into the mode flag (int) or
    FS_INVALID_MODE if the provided string does not contain enough
    information or is invalid. */
unsigned short fs_string_to_mode(const char* s);

/*  Remove the directory pointed to by path; to remove all sub-directories and
    files, use the `fs_rmdir_alt` function and set recursive to `true`.
    Returns:
        FS_NOT_VALID    - bad input
        FS_NO_EXISTS    - directory already does not exist
        FS_FAILURE      - error, likely read/write access or change during operation
        FS_SUCCESS      - on success signifies that the directory no longer exists
*/
int fs_rmdir(const char* path);
int fs_rmdir_alt(const char* path, bool recursive);

/*  List all the files and directories of the provided path. Items is a passed
    by reference int that, on completion, will identify how many records are
    returned. If error, items will be set to 0
    Returns:
        NULL if error or if path is not a directory
*/
char** fs_list_dir(const char* path, int* items);

/*******************************************************************************
*   Directory Functionality Encapsulation
*******************************************************************************/
dir_t d_init(const char* path);

void d_free(dir_t d);

const char* d_fullpath(dir_t d);

char** d_list_dir(dir_t d);

int d_num_subitems(dir_t d);

int d_update_list(dir_t d);

char** d_sub_dirs(dir_t d);

int d_num_sub_dirs(dir_t d);

char** d_sub_files(dir_t d);

int d_num_sub_files(dir_t d);

/*******************************************************************************
*   File Functionality Encapsulation
*******************************************************************************/

/*  Initialize the file_t object and pull information about the file pointed to
    by filepath
    Returns:
        NULL    - If filepath doesn't point to a file or symlink
        file_t  - Allocated memory with all members set except for reading the
                  file into memory
*/
file_t f_init(const char* filepath);

/*  Free the memory held by the file_t object */
void f_free(file_t f);

bool f_is_symlink(file_t f);

/*  Returns the absolute path of the file */
const char* f_absolute_path(file_t f);

/*  Returns the base directory (path) of the file */
const char* f_basedir(file_t f);

/*  Returns the filename of the file */
const char* f_filename(file_t f);

/*  Returns the file extension (without the .) */
const char* f_extension(file_t f);

/*  Returns the size of the file, in bytes */
size_t f_filesize(file_t f);

/*  Returns the permissions bitmask of the file */
unsigned short f_permissions(file_t f);

/*  Return the number of lines extracted, will be 0 if lines have not been
    extracted from the file */
size_t f_number_lines(file_t f);

/*  Return access to the lines extracted, will be NULL if lines have not been
    extracted from the file */
char** f_lines(file_t f);

/*  Return the full buffer contents of the file; originally will have newlines
    (if text) but those will be set to \0 if lines are extracted. This will
    allow for only needing to read the contents once.
    NOTE: Will be of size f_filesize + 1
    NOTE: If lines have been extracted, printing buffer will only show the
          first line!
*/
char* f_buffer(file_t f);

/*  Read the contents of file_t f and copy them into buffer (f_buffer);
    NOTE: Returns NULL if error occured; check errno if NULL */
const char* f_read_file(file_t f);

/*  Read the contents of file_t (if necessary) and modify buffer to place NULLs
    at each \n\r\f characters; lines points to each location for the line. This
    allows for not needing double the space for buffer and lines.
    NOTE: Modifies the buffer member so f_buffer() == f_lines()[0]
    NOTE: Will read the file if not already read
    NOTE: Returns NULL if error reading file; check errno for reason
    NOTE: Sets the f_number_lines() member
*/
char** f_parse_lines(file_t f);


#endif      /*  BARRUST_FILE_UTILS_H__  */
