/*******************************************************************************
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***  License: MIT 2020
*******************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "permutations.h"


typedef struct __permutations {
    unsigned short alphabet_len;
    unsigned int input_len;
    char* alphabet;
    unsigned short* cur_perm;
    char* cur_perm_str;
} __permutations;


permutations_t perm_init(unsigned int input_len, const char* alphabet) {
    permutations_t p = calloc(1, sizeof(permutations));
    p->alphabet_len = strlen(alphabet);
    p->input_len = input_len;
    // setup the alphabet
    p->alphabet = calloc(p->alphabet_len + 1, sizeof(char));
    strcpy(p->alphabet, alphabet);
    // set up the permutations
    p->cur_perm = calloc(input_len + 1, sizeof(unsigned short));
    p->cur_perm_str = calloc(input_len + 1, sizeof(char));
    unsigned int i;
    for (i = 0; i < input_len; ++i)
        p->cur_perm_str[i] = p->alphabet[0];  // start at the beginning

    return p;
}

void perm_free(permutations_t p) {
    free(p->alphabet);
    free(p->cur_perm);
    free(p->cur_perm_str);
    free(p);
}


const unsigned short* perm_current_permutation(permutations_t p) {
    return p->cur_perm;
}

const char* perm_to_string(permutations_t p) {
    return p->cur_perm_str;
}

void perm_inc(permutations_t p) {
    perm_add(p, 1);
}

void perm_add(permutations_t p, unsigned int num) {
    unsigned int i, pos = p->input_len - 1;
    for (i = 0; i < num; ++i) {
        ++p->cur_perm[pos];
        while (p->cur_perm[pos] == p->alphabet_len) {
            p->cur_perm[pos--] = 0;
            ++p->cur_perm[pos];
        }
        pos = p->input_len - 1;
    }
    /* update the current string */
    for (i = 0; i < p->input_len; ++i) {
        p->cur_perm_str[i] = p->alphabet[p->cur_perm[i]];
    }
}
/*
void perm_dec(permutations_t p) {
    perm_sub(p, 1);
}

void perm_sub(permutations_t p, unsigned int num) {
    unsigned int i, pos = p->input_len - 1;
    for (i = 0; i < num; ++i) {
        ++p->cur_perm[pos];
        while (p->cur_perm[pos] == p->alphabet_len) {
            p->cur_perm[pos--] = 0;
            ++p->cur_perm[pos];
        }
        pos = p->input_len - 1;
    }
    for (i = 0; i < p->input_len; ++i) {
        p->cur_perm_str[i] = p->alphabet[p->cur_perm[i]];
    }
}
*/
