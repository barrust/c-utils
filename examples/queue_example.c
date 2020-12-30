/*******************************************************************************
*   Demonstrate that the use of a doubly linked list as a queue works the same
*   since this example is identical to the doubly linked list version.
*
*   Use -v to include more debugging information
*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "../src/queue.h"


typedef struct person {
    int age;
    char gender;
    int height; /* feet */
    int id; /* order of getting here; for testing */
} _person;


#define NUM_ELEMENTS 1000000

int main(int argc, char const *argv[]) {
    bool verbose = false;

    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        verbose = true;
    }
    int i;
    time_t t;
    srand((unsigned) time(&t));

    queue_list_t q = q_init();

    for (i = 0; i < NUM_ELEMENTS; ++i) {
        _person* p = (_person*)malloc(sizeof(_person));
        p->age = rand() % 110 + 1;
        p->gender = rand() % 2 == 0 ? 'm' : 'f';
        p->height = rand() % 7 + 1;
        p->id = i;
        if (q_push(q, p) == QUEUE_FAILURE) {
            printf("Failed to allocate memory!\n");
        }

        assert(q_num_elements(q) == (unsigned int)(i + 1));
    }

    int cnt = 0;
    queue_node* n;
    q_reverse_traverse(q, n) { /* traverse for testing in reverse (from tail) */
        _person* w = (_person*) n->data;
        assert(w->id == NUM_ELEMENTS - ++cnt);
    }
    cnt = 0;
    q_traverse(q, n) { /* traverse for testing from the head node */
        _person* w = (_person*) n->data;
        assert(w->id == cnt++);
    }

    for (i = 1; i <= NUM_ELEMENTS; ++i) {
        _person* w = (_person*) q_pop(q);
        assert((unsigned int)(NUM_ELEMENTS - i) == q_num_elements(q));
        if (verbose) {
            printf("person: %d\tage: %d  \tgender: %c\theight (feet): %d\n", w->id, w->age, w->gender, w->height);
        }
        free(w); /* it was pop'd but not yet free'd */
    }

    q_free(q);

    printf("Completed successfully!\n");
    return 0;
}
