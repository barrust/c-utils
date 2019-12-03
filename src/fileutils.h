


typedef struct __file_struct file_struct;
typedef struct __file_struct *file_t;


/*******************************************************************************
*   Directory Functionality Encapsulation
*******************************************************************************/
int d_is_directory(const char* path);

int d_mkdir(const char* path, int recursive);

char* d_list_dir(const char* path);

char* d_resolve_directory(const char* path);

/*******************************************************************************
*   File Functionality Encapsulation
*******************************************************************************/

file_t f_init(const char* filepath);

void f_free(file_t f);

const char* f_basedir(file_t f);

const char* f_filename(file_t f);

const char* f_extension(file_t f);

size_t f_filesize(file_t f);

void f_read_file(file_t f);
