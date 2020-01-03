/*******************************************************************************
*   Demonstrate some of the uses of the doubly linked list datastructure by
*   adding people into a queue! We will make a person struct, to show it being
*   used with a non-standard datatype.
*
*   NOTE: One can make a queue (FIFO) using the data structure by making a
*   macro for pop and push to ensure that one always pushes to the tail (-1)
*   and pops from the front (0). A stack (LIFO) is best implemented using a
*   doubly linked list pushes to the front and pops from the tail.
*
*   Use -v to include more debugging information
*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "../src/dllist.h"


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

    dllist_t l = dll_init();

    for (i = 0; i < NUM_ELEMENTS; i++) {
        _person* p = malloc(sizeof(_person));
        p->age = rand() % 110 + 1;
        p->gender = rand() % 2 == 0 ? 'm' : 'f';
        p->height = rand() % 7 + 1;
        p->id = i;
        if (dll_append(l, p) == DLL_FAILURE) { /* we want to append to the end each time */
            printf("Failed to allocate memory!\n");
        }

        assert(dll_num_elements(l) == (unsigned int)(i + 1));
    }

    int cnt = 0;
    dll_node* n;
    dll_reverse_traverse(l, n) { /* traverse for testing in reverse (from tail) */
        _person* q = (_person*) n->data;
        assert(q->id == NUM_ELEMENTS - ++cnt);
    }
    cnt = 0;
    dll_traverse(l, n) { /* traverse for testing from the head node */
        _person* q = (_person*) n->data;
        assert(q->id == cnt++);
    }

    for (i = 1; i <= NUM_ELEMENTS; i++) {
        _person* w = (_person*) dll_remove(l, 0);
        assert((unsigned int)(NUM_ELEMENTS - i) == dll_num_elements(l));
        if (verbose) {
            printf("person: %d\tage: %d  \tgender: %c\theight (feet): %d\n", w->id, w->age, w->gender, w->height);
        }
        free(w); /* it was pop'd but not yet free'd */
    }

    dll_free(l);

    if (verbose)
        printf("Completed successfully!\n");
    return 0;
}
