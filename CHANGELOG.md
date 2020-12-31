# C-Utils

## Version 0.2.4
***New Libraries:***
* permutations

***Updates:***

*All Libraries*
* cpp guards
* Ensure cast of `malloc`, `calloc`, and `realloc` calls
* Added Defensive freeing of memory

*strlib*
* Fix trim of string that only contains spaces
* `str_append_alt` - handle if s2 is not NULL while s1 is NULL

*fileutils*
* Combined filepath handles `NULL` input
* Simplify input checks

*minunit.h*
* Fix output for `between` checks on failed assertion



## Version 0.2.3
***Updates:***

*strlib*
* Add input sanitation checks

*bitarray*
* Remove unused `math.h` library header
* Minor optimizations for resetting the bitarray and calculating number of bits set

*minunit*
* Additional assert types
* Test suite using minunit to ensure minunit works as desired


## Version 0.2.2
***New Libraries:***
* queue
* stack
* minunit.h

***Updates:***

*All Libraries*
* Micro optimizations


## Version 0.2.1
***Updates:***

*Graph:*
* Traversal of the graph from a particular vertex
* Breadth First
* Depth First
* OpenMP support

*StringLib:*
* Micro optimizations for s_trim and s_standardize_whitespace


## Version 0.2.0
***New Libraries:***
* Graph
* Linked List
* Doubly Linked List

***Updates:***
* added `dir_t` and supporting functionality to `fileutils`


## Version 0.1.0
***New Libraries:***
* stringlib
* fileutils
    * filesystem functions
    * file_t functions
* timing
* bitarray
