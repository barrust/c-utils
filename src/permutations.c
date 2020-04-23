/*******************************************************************************
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***  License: MIT 2020
*******************************************************************************/
#include <stdio.h>  // remove!
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
    p = NULL;
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

void perm_add(permutations_t p, size_t num) {
    size_t i, pos, cnt = 1;
    for (i = 0; i < num; ++i) {
        pos = p->input_len - 1;
        ++p->cur_perm[pos];
        while (p->cur_perm[pos] == p->alphabet_len) {
            ++cnt;
            p->cur_perm[pos--] = 0;
            if (pos == 0)
                break;
            ++p->cur_perm[pos];
        }
    }
    __update_current_str(p, p->input_len - cnt);
}


// void perm_dec(permutations_t p) {
//     perm_sub(p, 1);
// }
//
//
// void perm_sub(permutations_t p, size_t num) {
//     size_t i, j, pos, start;
//     for (i = 0; i < num; ++i) {
//         pos = p->input_len - 1;
//         while (pos > 0 && p->cur_perm[pos] == 0) {
//             --pos;
//         }
//         // printf("Max: %d\n", max);
//         if (pos == 0 && p->cur_perm[pos] == 0) {
//             return;
//         }
//         start = p->input_len - pos;
//         --p->cur_perm[pos++];
//         for (j = pos; j < p->input_len; ++j)
//             p->cur_perm[j] = p->input_len - 1;
//     }
//     __update_current_str(p, 0);
// }

/*******************************************************************************
*   private functions
*******************************************************************************/
static void __update_current_str(permutations_t p, size_t pos) {
    size_t i;
    for (i = pos; i < p->input_len; ++i) {
        p->cur_perm_str[i] = p->alphabet[p->cur_perm[i]];
    }
}
