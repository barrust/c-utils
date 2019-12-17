#ifndef BARRUST_BIT_ARRAY_H__
#define BARRUST_BIT_ARRAY_H__

/*******************************************************************************
***
***	 Author: Tyler Barrus
***	 email:  barrust@gmail.com
***
***	 Version: 0.1.0
***	 Purpose: Simple bit array library
***
***	 License: MIT 2019
***
***	 URL: https://github.com/barrust/c-utils
***
***	 Usage:
***     bitarray_t b = ba_initialize(1000);  // bits are 0 based array
***     ba_set_bit(b, 150);
***
***     ba_set_bit(b, 1000); // will return BITARRAY_INDEX_ERROR (-1)
***     ba_check_bit(b, 150); // will return BIT_SET (1)
***     ba_check_bit(b, 100); // will return BIT_NOT_SET (0)
***     ba_free(b);
***
*******************************************************************************/


#define BIT_SET 1
#define BIT_NOT_SET 0
#define BITARRAY_INDEX_ERROR -1

typedef struct __bitarray bitarray;
typedef struct __bitarray *bitarray_t;

/*  Initialize an empty bit array that can contain `bits` number of bits
    NOTE: This will alloc the required memory space. Up to the user to free
          the memory using `ba_free` */
bitarray_t ba_init(size_t bits);

/*  Property access of the size of the array holding the bit array */
size_t ba_array_size(bitarray_t ba);

/*  Property access of the number of bits in the bit array */
size_t ba_number_bits(bitarray_t ba);

/*  Property access to the actual bit array in an unmodifing form */
const unsigned char* const ba_get_bitarray(bitarray_t ba);

/*  Set bit `bit` to 1 */
int ba_set_bit(bitarray_t ba, size_t bit);

/*  Check if bit `bit` was previously set; return BIT_SET if true and
    BIT_NOT_SET if false */
int ba_check_bit(bitarray_t ba, size_t bit);

/*  Toggle the bit by setting it to BIT_NOT_SET if currently is BIT_SET or
    vice versa; returns the updated value of the bit */
int ba_toggle_bit(bitarray_t ba, size_t bit);

/*  Clear a bit by setting it to 0 */
int ba_clear_bit(bitarray_t ba, size_t bit);

/*  Reset the entire bitarray to 0 */
int ba_reset_bitarray(bitarray_t ba);

/*  Return a printable representation of the array as 0's and 1's;
    NOTE: It is up to the caller to free the memory */
char* ba_to_string(bitarray_t ba);

/*  Return the number of bits set */
size_t ba_number_bits_set(bitarray_t ba);

/*  Free all the memory */
void ba_free(bitarray_t ba);


#endif      /*   BARRUST_BIT_ARRAY_H__   */
