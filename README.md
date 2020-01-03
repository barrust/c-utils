# c-utils

[![Build Status](https://travis-ci.org/barrust/c-utils.svg?branch=master)](https://travis-ci.org/barrust/c-utils)
[![codecov](https://codecov.io/gh/barrust/c-utils/branch/master/graph/badge.svg)](https://codecov.io/gh/barrust/c-utils)


This project provides a collection of utility libraries to help reduce the need to write similar code for each project on an ad-hoc basis. The need is based on what I have needed in most projects but are ended up written, as needed, and usually differently each time and without unit tests.

If there are other commonly used code or data-structures that should be added, please add a feature request!

##### Table of Contents:
* [stringlib](#stringlib)
* [fileutils](#fileutils)
* [bitarray](#bitarray)
* [linked list](#linkedlist)
* [doubly linked list](#doublylinkedlist)
* [timing-c](#timing-c)

##### Recommended External Libraries
* [set](https://github.com/barrust/set)
* [hashmap](https://github.com/barrust/hashmap)


##### Unit tests

Unit tests are provided using the [minunit](https://github.com/siu/minunit) library. Each function is, **hopefully**, fully covered. Any help in getting as close to 100% coverage would be great!

To run the unittest suite, simply compile the test files using the provided `Makefile` with the command `make`. Then you can execute the tests using `./dist/bitarray`, `./dist/strlib`, `./dist/fileutils`, or `./dist/timing` executables.

#### Issues

If an unexpected outcome occurs, please submit an issue with a ***minimal code example*** that encapsulates the error.

A great issue would provide the following:
> s_remove_unwanted_chars does shows duplicate entries after removal.
> ``` c
> char* test[] = "This is a test";
> // expect "Ths s a es"
> // get "Ths s a esest"
> // Notice the extra `est`; likely due to not erasing
> s_remove_unwanted_chars(test, "ti");  
> ```

#### Examples

Examples output is provided in the `./examples` folder. You can compile these examples using `make examples`. They can be run using `./dist/ex_timing`, `./dist/ex_bitarray`, `./dist/ex_fileutils`, and `./dist/ex_stringlib`.

Not all functionality is demonstrated for all libraries, but hopefully enough is present to help make using these libraries easier.


## stringlib

A collection of `c-string` functions that I find myself writing for many of my projects. This library provides a single place to store this functionality while ensuring that everything is as minimalistic as possible. In most cases, the string is modified in place. No additional structs are needed as it uses vanilla `c-strings` everywhere.

Those functions that have a `const char*` do not modify the string!

Documentation of each function is found in the `stringlib.h` file.

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `stringlib` library

#### Usage

To use, copy the `stringlib.h` and `stringlib.c` files into your project folder and add them to your project.

``` c
#include "stringlib.h"

char str[] = "   This is a \t horrible \n\r string \f  to\n clean up... please help!\n\r";
// we can trim and stanardize whitespaces to a single whitespace
s_single_space(str); // "This is a horrible string to clean up... please help!"

// find the first `u`
int pos = s_find(str, 'u'); // 35
// find the first `ri`
pos = s_find_str(str, "ri"); // 13
// find the last `ri`
pos = s_find_str_reverse(str, "ri"); // 21

// remove unwanted characters
s_remove_unwanted_chars(str, "tph"); // "Tis is a orrible sring o clean u... lease el!"
```


## fileutils

The file utils library provides utility functions to deal with basic file system operations. From determining if a file exists, to making directories, to reading in a file and parsing out the lines.

All functions are documented within the `fileutils.h` file.

Unfortunately, I have not been able to test this library on non-Linux machines. If errors arise on Windows, please submit an issue or, even better, a ***pull request***! If something is shown to work on Windows, that information would also be very helpful!

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `fileutils` library

#### Usage

To use, copy the `fileutils.h` and `fileutils.c` files into your project folder and add them to your project.

``` c
#include "fileutils.h"

int res;

/* General path / file functionality */
char* current_working_directory = fs_cwd();
fs_mkdir("path_of_new_dir", false);  /* do not recursively make the dirs */

/* if the unknown path is a file, remove it! */
if (fs_identify_path("unknown_path") == FS_FILE) {
    fs_remove_file("unkown_path");
}

if (fs_rmdir_alt("path_to_non_empty_directory", true) == FS_NO_EXISTS) {
    printf("Successfully removed the directory and all sub directories and files!");
}

/* parse and read a file */
file_t f = f_init("path_to_file");
printf("Base dir: %s\n", f_basedir(f));
printf("Filename: %s\n", f_filename(f));
printf("Extension: %s\n", f_extension(f));
printf("File Size: %llu\n", f_filesize(f));
/* read the file contents into a buffer */
f_read_file(f);
/* parse the file into lines */
f_parse_lines(f);

f_free(f);
```


## bitarray

The bit array library is provided to allow for a drop in bit array. It uses the smallest binary size possible for the array (`char`) to reduce the few extra bits needed if using an int (8 bits per element vs 32). It also tracks how many bits were desired and how many elements were used to hold the bit array.

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `bitarray` library

#### Usage

To use, copy the `bitarray.h` and `bitarray.c` files into your project folder and add them to your project.

``` c
#include "bitarray.h"

bitarray_t ba = ba_init(20000000);  // we want to track 20,000,000 items!

for (int i = 0; i < 20000000; i++) {
    if (i % 5 == 0)  // whatever makes us want to set track this element
        ba_set_bit(ba, i);
}

// we can check bits easily!
if (ba_check_bit(ba, 10000000) == BIT_SET)
    printf("Bit 10,000,000 is set!\n");
else
    printf("Bit 10,000,000 is not set!\n");

// we can also clear a single bit or reset the whole array
ba_clear_bit(ba, 10000000); // a check would now be BIT_NOT_SET

ba_reset_bitarray(ba); // all the bits are set to 0

// free all the memory!
ba_free(ba);
```


## linkedlist

This library adds a generic linked list implementation. Any type of data can be added to the list as the data type of the data is `void*`. Elements can be added or removed to the end or any location within the list.

All functions are documented within the `llist.h` file.

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `linked list` library

#### Usage

To use, simply copy the `llist.h` and `llist.c` files into your project and include it where needed.

``` c
#include "llist.h"

llist_t l = ll_init();


int i;
for (i = 0; i < 1000; i++) {
    int* t = calloc(1, sizeof(int));
    *t = rand() % 100000;
    ll_append(l, t);
}

i = 0;
ll_node node;
ll_traverse(l, node) {
    int val = *(int*)node->data;
    printf("idx: %d\tval: %d\n", i++, val);
}

// or iterate the old fashion way
node = ll_first_node(l);
while (node != NULL) {
    int val = *(int*)node->data;
    printf("idx: %d\tval: %d\n", i++, val);
    node = node->next;
}

ll_free_alt(l, true); // even free the data field
```


## doublylinkedlist

This library adds a generic doubly linked list implementation. Any type of data can be added to the list as the data type of the data is `void*`. Elements can be added or removed to the end or any location within the list.

All functions are documented within the `dllist.h` file.

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `doubly linked list` library

#### Usage

To use, simply copy the `dllist.h` and `dllist.c` files into your project and include it where needed.

``` c
#include "dllist.h"

dllist_t l = dll_init();


int i;
for (i = 0; i < 1000; i++) {
    int* t = calloc(1, sizeof(int));
    *t = rand() % 100000;
    dll_append(l, t);
}

i = 0;
dll_node node;
dll_traverse(l, node) {
    int val = *(int*)node->data;
    printf("idx: %d\tval: %d\n", i++, val);
}

// or iterate the old fashion way in reverse (or dll_reverse_traverse)
node = dll_last_node(l);
while (node != NULL) {
    int val = *(int*)node->data;
    printf("idx: %d\tval: %d\n", i++, val);
    node = node->prev;
}

dll_free_alt(l, true); // even free the data field
```


## timing-c

This header utility is to be able to quickly provide timing functionality to standard `C` code. It is designed to be beneficial to me as I am tired of re-writting the same type of functionality. To use, simply copy the header into your project folder.

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `timing.h` library

#### Usage

To use, simply copy the `timing.h` file into your project and include it where needed.

``` c
#include <stdio.h>
#include <stdlib.h>
#include "timing.h"

Timing t;

timing_start(&t);
// code to time here
timing_end(&t);
printf("time elapsed: %f\n", t.timing_double);

// get to elapsed time elements easily
printf("hours:        %d\n", timing_get_hours(t));
printf("minutes:      %d\n", timing_get_minutes(t));
printf("seconds:      %d\n", timing_get_seconds(t));
printf("milliseconds: %d\n", timing_get_milliseconds(t));
printf("microseconds: %d\n", timing_get_microseconds(t));

// get to a pretty print version
char* output = format_time_diff(&t);
printf("pretty output: %s\n", output);
free(output);
```
