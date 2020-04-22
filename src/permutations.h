#ifndef BARRUST_PERMUTATIONS_H__
#define BARRUST_PERMUTATIONS_H__


typedef struct __permutations permutations;
typedef struct __permutations *permutations_t;

permutations_t perm_init(unsigned int input_len, const char* alphabet);

/*  Free all the memory */
void perm_free(permutations_t p);

void perm_inc(permutations_t p);
void perm_add(permutations_t p, unsigned int num);

void perm_dec(permutations_t p);
void perm_sub(permutations_t p, unsigned int num);


const unsigned short* perm_current_permutation(permutations_t p);

const char* perm_to_string(permutations_t p);

#endif  /* BARRUST_PERMUTATIONS_H__ */
