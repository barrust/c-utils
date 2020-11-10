#ifndef BARRUST_PERMUTATIONS_H__
#define BARRUST_PERMUTATIONS_H__

/*******************************************************************************
***
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***
***  Version: 0.1.0
***  Purpose: Generic permutations library
***
***  License: MIT 2020
***
***  URL: https://github.com/barrust/c-utils
***
***  Usage:
***     #include "permutations.h"
***     permutations_t p = perm_init(8, "ATCG");
***     for (int i = 0; i < 99999; ++i) {
***         perm_to_string(p);  // do something with this!
***         perm_inc(p); // go to the next one
***     }
***     perm_free(p);
***
*******************************************************************************/

typedef struct __permutations permutations;
typedef struct __permutations *permutations_t;

/*  Initialize a permutations object of size input_len using the provided
    alphabet */
permutations_t perm_init(size_t input_len, const char* alphabet);

/*  Free all the memory */
void perm_free(permutations_t p);

/*  Increment the permutation to the next value */
void perm_inc(permutations_t p);

/*  Increment the permutation by num values */
void perm_add(permutations_t p, size_t num);

/*  Decrement the permutation to the previous value */
void perm_dec(permutations_t p);

/*  Subtract or rewind the permutation by num values */
void perm_sub(permutations_t p, size_t num);

/*  Get access to the current perumtation state, prepresented as numbers
    where each position is in the range 0 - (alphabet length - 1) */
const unsigned short* perm_current_permutation(permutations_t p);

/*  Get access to the string reprensentation of the permutation */
const char* perm_to_string(permutations_t p);

/*  Get access to the alphabet that makes up this permutation */
const char* perm_alphabet(permutations_t p);

/*  Get access to the length of the alphabet */
unsigned short perm_alphabet_length(permutations_t p);

/*  Get access to the input size, or output string, of the permutation */
size_t perm_input_size(permutations_t p);


#endif  /* BARRUST_PERMUTATIONS_H__ */
