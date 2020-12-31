# c-utils

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![GitHub release](https://img.shields.io/github/release/barrust/c-utils.svg)](https://github.com/barrust/c-utils/releases)
[![C/C++ CI](https://github.com/barrust/c-utils/workflows/C/C++%20CI/badge.svg?branch=master)](https://github.com/barrust/c-utils/actions)
[![codecov](https://codecov.io/gh/barrust/c-utils/branch/master/graph/badge.svg)](https://codecov.io/gh/barrust/c-utils)


This project provides a collection of utility libraries to help reduce the need to write similar code for each project on an ad-hoc basis. The need is based on what I have needed in most projects but are ended up written, as needed, and usually differently each time and without unit tests. The goal is to provide a single place to store each of these libraries and to provide unit tests.

If there are other commonly used code or data-structures that should be added, please add a feature request!

##### Table of Contents:
* [stringlib](#stringlib) - C-string utilities
* [fileutils](#fileutils) - File system utilities
* [bitarray](#bitarray)
* [linked list](#linkedlist)
* [doubly linked list](#doublylinkedlist)
* [stack](#stack)
* [queue](#queue)
* [graph](#graph)
* [permutations](#permutations)
* [timing-c](#timing-c) - Code timing utility
* [minunit](#minunit) - Unit testing

##### Recommended External Libraries
* [set](https://github.com/barrust/set)
* [hashmap](https://github.com/barrust/hashmap)
* [bloom filter](https://github.com/barrust/bloom)
* [kseq](https://github.com/lh3/readfq) - A fasta/fastq parser library
* [utf-8](https://github.com/sheredom/utf8.h) - Single header UTF-8 string functions for C and C++


##### Unit tests

Unit tests are provided using the [minunit](#minunit) library. Each function is, **hopefully**, fully covered. Any help in getting as close to 100% coverage would be much appreciated!

To run the unit-test suite, simply compile the test files using the provided `Makefile` with the command `make test`. Then you can execute the tests using the executables `./dist/bitarray`, `./dist/strlib`, `./dist/fileutils`, `./dist/graph`, `./dist/llist`, `./dist/dllist`, `./dist/stack`, `./dist/queue`, `./dist/permutations`, `./dist/minunit`, or `./dist/timing`.

#### Issues

If an unexpected outcome occurs, please submit an [issue on github](https://github.com/barrust/c-utils/issues). Please also provide a ***minimal code example*** that encapsulates the error.

A great [issue](https://github.com/barrust/c-utils/issues) would provide the following:
> s_remove_unwanted_chars shows duplicate entries after removal.
> ``` c
> char test[] = "This is a test";
> // expect "Ths s a es"
> // get "Ths s a esest"
> // Notice the extra `est`; likely due to not erasing the trailing chars
> s_remove_unwanted_chars(test, "ti");  
> ```

#### Examples

Example programs are provided in the `./examples` folder. You can compile these examples using `make examples`. They can be run from the `./dist` folder and are named prepended with `ex_`.

Not all functionality is demonstrated for all libraries, but hopefully enough is present to help make using these libraries easier. All functionality for each library is documented in the `.h` files.


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

Unfortunately, I have not been able to test this library on Windows machines. If errors arise on Windows, please submit an issue or, even better, a ***pull request***! If something is shown to work on Windows, that information would also be very helpful!

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

The bit array library is provided to allow for a drop in bit array. It uses the smallest binary size possible for the array (`char`) to reduce the few extra bits needed compared to using an int (8 bits per element vs 32). It also tracks how many bits were desired and how many elements were used to hold the bit array.

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

This library adds a generic linked list implementation. Any type of data can be added to the list as the data type of the data is `void*`. Elements can be added or removed to the end or any location within the list. If you have fewer access and removal needs it may be better to use a [stack](#stack) which provides the same structure.

All functions are documented within the `llist.h` file.

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `linked list` library

#### Usage

To use, simply copy the `llist.h` and `llist.c` files into your project and include it where needed.

``` c
#include "llist.h"

llist_t l = ll_init();

int* get_random() {
    int* t = calloc(1, sizeof(int));
    *t = rand() % 100000;
    return t;
}

int i;
for (i = 0; i < 1000; i++) {
    int* t = get_random();
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

This library adds a generic doubly linked list implementation. Any type of data can be added to the list as the data type of the data is `void*`. Elements can be added or removed to the end or any location within the list. This is useful when you need to control where nodes are inserted and the order that they are removed. If you have fewer access and removal needs it may be better to use a [queue](#queue) which provides the same structure.

All functions are documented within the `dllist.h` file.

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `doubly linked list` library

#### Usage

To use, simply copy the `dllist.h` and `dllist.c` files into your project and include it where needed.

``` c
#include "dllist.h"

dllist_t l = dll_init();

int* get_random() {
    int* t = calloc(1, sizeof(int));
    *t = rand() % 100000;
    return t;
}

int i;
for (i = 0; i < 1000; i++) {
    int* t = get_random();
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

## stack

Built using the linked list code, this provides a special implementation of a linked list that always inserts and removes the first element in the linked list. This is useful in instances that there is no need for arbitrary insertion and removal locations from the linked list. As such, this version can be slightly faster than a linked list since insertion is constant.

All functions are documented within the `stack.h` file.

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `stack` library

#### Usage

To use, simply copy the `stack.h` and `stack.c` files into your project and include it where needed.

``` c
#include "stack.h"

stack_list_t stk = stk_init();

int* get_random() {
    int* t = calloc(1, sizeof(int));
    *t = rand() % 100000;
    return t;
}

int i;
for (i = 0; i < 1000; i++) {
    int* t = get_random();
    stk_push(stk, t);
}

int* val = (int*) stk_pop(stk);  // this will be the last item pushed on the stack!
```

## queue

Built using the doubly linked list code, this provides a special implementation of a doubly linked list that always inserts at the end of the list and always removes the first element. This is useful in instances that there is no need for arbitrary insertion and removal locations from the linked list but you want to either be able to access all nodes from both directions or you want to maintain a FIFO (first in first out) approach. As such, this version can be slightly faster than a doubly linked list since insertion and deletion are constant.

All functions are documented within the `queue.h` file.

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `queue` library

#### Usage

To use, simply copy the `queue.h` and `queue.c` files into your project and include it where needed.

``` c
#include "queue.h"

queue_list_t q = q_init();

int* get_random() {
    int* t = calloc(1, sizeof(int));
    *int = rand() % 100000;
    return t;
}

int i;
for (i = 0; i < 1000; i++) {
    int* t = get_random();
    q_push(q, t);
}

int* val = (int*) q_pop(q);  // this will be the first item pushed on the queue!
```

## graph

This library adds a directed graph implementation that allows for any data type to be used for vertex or edge metadata. It tracks all the vertices and edges inserted into the graph and helps ensure that there are no dangling edges.

There are several ways to traverse the graph or to easily loop over vertices and edges. Macros are provided to allow for iterating over vertices or over the edges that emanate from the vertex: `g_iterate_vertices` and `g_iterate_edges`. There are also to helper functions to do either a breadth first or depth first traverse starting from a particular vertex: `g_breadth_first_traverse` and `g_depth_first_traverse`.

All functions are documented within the `graph.h` file.

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `graph` library

#### Usage

To use, simply copy the `graph.h` and `graph.c` files into your project and include it where needed.

``` c
#include "graph.h"

graph_t g = g_init();

// add some verticies
g_vertex_add(g, "Washington D.C.");
g_vertex_add(g, "Raleigh NC");
g_vertex_add(g, "Boston, Mass");
g_vertex_add(g, "Cincinati, OH");

// add edges
g_edge_add(g, 0, 1, "250 miles"); // washington to raliegh
g_edge_add(g, 0, 2, "150 miles"); // washington to boston
g_edge_add(g, 0, 3, "300 miles"); // washington to cincinati
g_edge_add(g, 1, 3, "500 miles"); // raliegh to cincinati
g_edge_add(g, 2, 3, "400 miles"); // boston to cincinati

// iterate over the verticies
vertex_t v;
unsigned int i, j;
g_iterate_vertices(g, v, i) {
    printf("idx: %d\tcity: %s\n", i, g_vertex_metadata(v));

    // iterate over the edges!
    edge_t e;
    g_iterate_edges(v, e, j) {
        vertex_t dest = g_vertex_get(g_edge_dest(e));
        printf("\tto: %s\tdistance: %s\n", g_vertex_metadata(dest), g_edge_metadata(e));
    }
}

g_free_alt(g, false);
```

## permutations

There are times when one needs to run over all (most?) of the permutations of a set of characters, or numbers. This library allows the user to pass initialize a permutation list and give it a length and an alphabet and get the permutations as needed such as to check for all possible k-mers in a genome, among other uses!

#### Compiler Flags

***NONE*** - There are no needed compiler flags for the `permutations` library

#### Usage

To use, simply copy the `permutations.h` and `permutations.c` files into your project and include it where needed.

``` c
#include "permutations.h"

permutations_t p = perm_init(5, "ATCG");

// number permutations is alphabet length raised to the input length
for (unsigned int i = 0; i < 1024; ++i) {
    perm_inc(p);
    // do something with the new permutation
    printf("%s\n", perm_to_string(p));
}
perm_free(p);
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

## minunit

This header utility is a testing framework for C programs. It is a fork of [siu/mununit](https://github.com/siu/minunit) that adds several assertions that are not in the base library. License ([MIT](https://github.com/siu/minunit/blob/master/MIT-LICENSE.txt)) information is contained in the header file.

### Compiler Flags

***NONE*** - There are no needed compiler flags for the `minunit.h` testing framework.

### Usage

For full examples, please view the tests in the `./test` folder. A quick run down of setting up the tests is provided below along with a quick set of function documentation.

``` c
#include <stdio.h>
#include <stdlib.h>
#include <minunit.h>

int arr[25];

void test_setup(void) {
    for (int i = 0; i < 25; ++i)
        arr[i] = i;
}

void test_teardown(void) {
    // no teardown required
}

MU_TEST(test_simple) {
    mu_assert_int_eq(0, arr[0]);
}

// Set up the test suite by configuring and stating which tests should be run
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_simple);
}

int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}
```

### Documentation

* **mu_check(test)**: Checks to verify that the passed boolean expression test is `true`; fails otherwise.
* **mu_fail(message)**: Automatically fails the assertion and returns the provided message; useful for non-implemented features, etc.
* **mu_assert(test, message)**: Assert that the boolean expression `test` is true, otherwise fail and print the passed `message`.
* **mu_assert_int_eq(expected, result)**: Assert that the `expected` int is the same as the passed `result`.
* **mu_assert_int_not_eq(expected, result)**: Assert that the `result` does not equal `expected`; not this is useful for checking comparison functions, etc.
* **mu_assert_int_greater_than(val, result)**: Assert that `result` is greater than `val`.
* **mu_assert_int_less_than(val, result)**:  Assert that `result` is less than `val`.
* **mu_assert_int_between(expected_lower, expected_upper, result)**: Assert that the `result` is between (inclusive) `expected_lower` and `expected_upper`; if upper and lower are reversed, then it is **not** between!
* **mu_assert_int_in(expected, array_length, result)**: Assert that the `result` is a member of the `expected` array; `array_length` is needed to know the number of elements in the array.
* **mu_assert_double_eq(expected, result)**: Assert that the double in `result` is the same as the `expected` double.
* **mu_assert_double_greater_than(val, result)**: Assert that `result` is greater than `val`.
* **mu_assert_double_less_than(val, result)**:  Assert that `result` is less than `val`.
* **mu_assert_double_between(expected_lower, expected_upper, result)**: Assert that `result` is between (inclusive) `expected_lower` and `expected_upper`; if upper and lower are reversed, then it is **not** between!
* **mu_assert_string_eq(expected, result)**: Assert that the `result` string (char* or char[]) is the same as the `expected` string.
* **mu_assert_null(result)**: Assert that the passed `result` pointer is `NULL`.
* **mu_assert_not_null(result)**: Assert that the passed `result` pointer is not `NULL`.
* **mu_assert_pointers_eq(pointer1, pointer2)**: Assert that `pointer1` and `pointer2` point to the same memory location.
* **mu_assert_pointers_not_eq(pointer1, pointer2)**: Assert that `pointer1` and `pointer2` do not point to the same memory location.
