# c-utils

This project provides a collection of utility libraries to help reduce the need to write similar code for each project on an ad-hoc basis. The need is based on what I have needed in most projects but write as needed, usually differently each time and without unit tests.

If there are other commonly used code that should be added, please add a feature request!

##### Table of Contents:
* [stringlib](#stringlib)
* [bitarray](#bitarray)
* [timing-c](#timing-c)

##### External Libraries
* [set](https://github.com/barrust/set)
* [hashmap](https://github.com/barrust/hashmap)


##### Unit tests

Unit tests are provided using the [minunit](https://github.com/siu/minunit) library. Each function is, **hopefully**, fully tested. If not, please submit an issue with a ***minimal code example*** that encapsulates the error.

A great issue would provide the following:
> s_remove_unwanted_chars does shows duplicate entries after removal.
> ``` c
> char* test[] = "This is a test";
> // expect "Ths s a es"
> // get "Ths s a esest"
> // Notice the extra `est`; likely due to not erasing
> s_remove_unwanted_chars(test, "ti");  
> ```


## stringlib

A collection of `c-string` functions that I find myself writing for many of my projects. This library provides a single place to store this functionality while ensuring that everything is as minimalistic as possible. In most cases, the string is modified in place. No additional structs are needed as it uses vanilla `c-strings` everywhere.

Those functions that have a `const char*` do not modify the string!

Documentation of each function is found in the `stringlib.h` file.

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

## bitarray

The bit array library is provided to allow for a drop in bit array. It uses the smallest binary size possible for the array (`char`) to reduce the few extra bits needed if using an int (8 bits per element vs 32). It also tracks how many bits were desired and how many elements were used to hold the bit array.

#### Usage

To use, copy the `bitarray.h` and `bitarray.c` files into your project folder and add them to your project.

``` c
#include "bitarray.h"

bitarray_t ba = ba_init(20000000);  // we want to track 20,000,000 items!

for (int i = 0; i < 20000000; i++) {
    if (...)  // whatever makes us want to set track this element
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


## timing-c

This header utility is to be able to quickly provide timing functionality to standard `C` code. It is designed to be beneficial to me as I am tired of re-writting the same type of functionality. To use, simply copy the header into your project folder.

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
