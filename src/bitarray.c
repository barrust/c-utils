/*******************************************************************************
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***  License: MIT 2019
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // memset
#include "bitarray.h"


#define CHECK_BIT(A, k)     (A[((k) / 8)] &   (1 << ((k) % 8)))
#define SET_BIT(A,k)        (A[((k) / 8)] |=  (1 << ((k) % 8)))
#define CLEAR_BIT(A,k)      (A[((k) / 8)] &= ~(1 << ((k) % 8)))
#define TOGGLE_BIT(A,k)     (A[((k) / 8)] ^=  (1 << ((k) % 8)))


/* see: https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetTable */
#define B2(n) n,     n+1,     n+1,     n+2
#define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
static const unsigned char bits_set_table[256] = {B6(0), B6(1), B6(1), B6(2)};


typedef struct __bitarray {
    unsigned char* arr;
    size_t num_bits;
    size_t num_chars;
} __bitarray;


/* NOTE: This does zero error checking because it is guaranteed to have
         a denominator of 8 and the numerator is guaranteed to be positive
   NOTE: This is close in timing to the math version when not using
         compiler optimizations but can be faster when optimized */
#define CEILING(n, d)  (((n) / (d)) + ((n) % (d) > 0))


bitarray_t ba_init(size_t bits) {
    bitarray_t ba = (bitarray_t)calloc(1, sizeof(bitarray));
    ba->num_bits = bits;
    size_t num_chars = CEILING(bits, 8);
    ba->num_chars = num_chars;
    /* the extra one is for the null byte! */
    ba->arr = (unsigned char*)calloc(num_chars + 1, sizeof(unsigned char));
    return ba;
}


size_t ba_array_size(bitarray_t ba) {
    return ba->num_chars;
}


size_t ba_number_bits(bitarray_t ba) {
    return ba->num_bits;
}


const unsigned char* ba_get_bitarray(bitarray_t ba) {
    return ba->arr;
}


void ba_free(bitarray_t ba) {
    free(ba->arr);
    ba->arr = NULL;
    ba->num_bits = 0;
    ba->num_chars = 0;
    free(ba);
}


int ba_set_bit(bitarray_t ba, size_t bit) {
    if (bit >= ba->num_bits)
        return BITARRAY_INDEX_ERROR;
    SET_BIT(ba->arr, bit);
    return BIT_SET;  /* no reason this should ever fail... */
}


int ba_check_bit(bitarray_t ba, size_t bit) {
    if (bit >= ba->num_bits)
        return BITARRAY_INDEX_ERROR;
    return (CHECK_BIT(ba->arr, bit) != 0) ? BIT_SET : BIT_NOT_SET;
}

int ba_check_and_set_bit(bitarray_t ba, size_t bit) {
    if (bit >= ba->num_bits)
        return BITARRAY_INDEX_ERROR;
    int was_previously_set = (CHECK_BIT(ba->arr, bit) != 0) ? BIT_SET : BIT_NOT_SET;
    SET_BIT(ba->arr, bit);
    return was_previously_set;
}

int ba_toggle_bit(bitarray_t ba, size_t bit) {
    if (bit >= ba->num_bits)
        return BITARRAY_INDEX_ERROR;
    TOGGLE_BIT(ba->arr, bit);
    return ba_check_bit(ba, bit);
}


int ba_clear_bit(bitarray_t ba, size_t bit) {
    if (bit >= ba->num_bits)
        return BITARRAY_INDEX_ERROR;
    CLEAR_BIT(ba->arr, bit);
    return BIT_NOT_SET;
}


int ba_reset(bitarray_t ba) {
    memset(ba->arr, 0, ba->num_chars);
    return BIT_NOT_SET;
}


char* ba_to_string(bitarray_t ba) {
    char* res = (char*)calloc(ba->num_bits + 1, sizeof(char));
    size_t i;
    for (i = 0; i < ba->num_bits; ++i)
        res[i] = (CHECK_BIT(ba->arr, i) != 0) ? '1' : '0';
    return res;
}


size_t ba_number_bits_set(bitarray_t ba) {
    size_t res = 0;
    size_t i;
    for (i = 0; i < ba->num_chars; ++i)
        res += bits_set_table[ba->arr[i]];
    return res;
}
