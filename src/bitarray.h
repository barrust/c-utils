

#define BIT_SET 1
#define BIT_NOT_SET 0
#define BITARRAY_INDEX_ERROR -1

typedef struct __bitarray bitarray;
typedef struct __bitarray *bitarray_t;

bitarray_t ba_init(size_t bits);


void ba_initialize(bitarray* ba, size_t bits);

size_t ba_array_size(bitarray_t ba);
size_t ba_number_bits(bitarray_t ba);
const unsigned char* const ba_get_bitarray(bitarray_t ba);

int ba_set_bit(bitarray_t ba, size_t bit);
int ba_check_bit(bitarray_t ba, size_t bit);
int ba_clear_bit(bitarray_t ba, size_t bit);

int ba_reset_bitarray(bitarray_t ba);

void ba_free(bitarray_t ba);
