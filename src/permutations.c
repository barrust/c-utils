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
    size_t input_len;
    char* alphabet;
    unsigned short* cur_perm;
    char* cur_perm_str;
} __permutations;


/* private functions */
static void __update_current_str(permutations_t p, size_t pos);


permutations_t perm_init(size_t input_len, const char* alphabet) {
    permutations_t p = calloc(1, sizeof(permutations));
    p->alphabet_len = strlen(alphabet);
    p->input_len = input_len;
    // setup the alphabet
    p->alphabet = calloc(p->alphabet_len + 1, sizeof(char));
    strcpy(p->alphabet, alphabet);
    // set up the permutations
    p->cur_perm = calloc(input_len + 1, sizeof(unsigned short));
    p->cur_perm_str = calloc(input_len + 1, sizeof(char));
    __update_current_str(p, 0);

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

const char* perm_alphabet(permutations_t p) {
    return p->alphabet;
}

unsigned short perm_alphabet_length(permutations_t p) {
    return p->alphabet_len;
}

size_t perm_input_size(permutations_t p) {
    return p->input_len;
}

void perm_inc(permutations_t p) {
    perm_add(p, 1);
}

void perm_add(permutations_t p, size_t num) {
    size_t i, max_val = 1;
    for (i = 0; i < num; ++i) {
        size_t cnt = 1, pos = p->input_len - 1;
        ++p->cur_perm[pos];
        while (p->cur_perm[pos] == p->alphabet_len) {
            ++cnt;
            p->cur_perm[pos--] = 0;
            ++p->cur_perm[pos];
            /* check for wrap around */
            if (pos == 0 && p->cur_perm[pos] == p->alphabet_len) {
                p->cur_perm[pos] = 0;
                __update_current_str(p, 0);
                break;
            }
        }
        if (cnt > max_val)
            max_val = cnt;
    }
    __update_current_str(p, p->input_len - max_val);
}

void perm_dec(permutations_t p) {
    perm_sub(p, 1);
}

void perm_sub(permutations_t p, size_t num) {
    size_t i, j, max_val = 1;
    for (i = 0; i < num; ++i) {
        size_t cnt = 1, pos = p->input_len - 1;
        if (p->cur_perm[pos] > 0) {
            --p->cur_perm[pos];
            continue;
        }

        while (pos > 0 && p->cur_perm[pos] == 0) {
            ++cnt;
            --pos;
        }

        /* avoid overflowing */
        if (pos == 0 && p->cur_perm[pos] == 0) {
            return;
        }

        --p->cur_perm[pos++];
        for (j = pos; j < p->input_len; ++j)
            p->cur_perm[j] = p->alphabet_len - 1;

        if (cnt > max_val)
            max_val = cnt;
    }
    __update_current_str(p, p->input_len - max_val);
}


/*******************************************************************************
*   private functions
*******************************************************************************/
static void __update_current_str(permutations_t p, size_t pos) {
    size_t i;
    for (i = pos; i < p->input_len; ++i) {
        p->cur_perm_str[i] = p->alphabet[p->cur_perm[i]];
    }
}
