#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "../src/dllist.h"

dllist_t l;

void test_setup(void) {
    l = dll_init();
}

void test_teardown(void) {
    dll_free_alt(l, true);
    l = NULL;
}


/*******************************************************************************
*   Test the setup
*******************************************************************************/
MU_TEST(test_default_setup) {
    mu_assert_int_eq(0, dll_num_elements(l));
    mu_assert_string_eq(NULL, (void*)dll_first_node(l));
    mu_assert_string_eq(NULL, (void*)dll_last_node(l));
}


/*******************************************************************************
*   Test Free-ing
*******************************************************************************/
MU_TEST(test_freeing) {
    dllist_t a = dll_init();
    mu_assert_int_eq(0, dll_num_elements(l));
    dll_free(a);
}


/*******************************************************************************
*   Test appending elements
*******************************************************************************/
MU_TEST(test_append) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int j = 0;
    dll_node* n = dll_first_node(l);
    while (n != NULL) {
        int* val = (int*)n->data;
        mu_assert_int_eq(j++, *val);
        n = n->next;
    }

    /* test in reverse */
    j = 4;
    n = dll_last_node(l);
    while (n != NULL) {
        int* val = (int*)n->data;
        mu_assert_int_eq(j--, *val);
        n = n->prev;
    }
}


/*******************************************************************************
*   Test inserting elements
*******************************************************************************/
MU_TEST(test_insert_beginning) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* q = calloc(1, sizeof(int));
    *q = 15;
    dll_insert(l, q, 0); // insert at the beginning

    dll_node* n = dll_first_node(l);
    mu_assert_int_eq(15, *(int*)n->data);
    mu_assert_int_eq(6, dll_num_elements(l));
    /* ensure pointing correct */
    mu_assert_int_eq(0, *(int*)n->next->data);
    mu_assert_int_eq(15, *(int*)n->next->prev->data);
    mu_assert_string_eq(NULL, (void*)n->prev);

    int w = 0;
    n = n->next;
    while(n != NULL) {
        int* val = (int*)n->data;
        mu_assert_int_eq(w++, *val);
        n = n->next;
    }
}

MU_TEST(test_insert_end) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }
    int* q = calloc(1, sizeof(int));
    *q = 15;
    dll_insert(l, q, 4); /* insert at the end of the */
    dll_node* n = dll_last_node(l);
    mu_assert_int_eq(15, *(int*)n->data);
    mu_assert_string_eq(NULL, (void*)n->next);
}

MU_TEST(test_insert_first_half) {
    int i;
    for (i = 0; i < 10; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* q = calloc(1, sizeof(int));
    *q = 15;
    dll_insert(l, q, 2); /* insert at the first half */

    dll_node* n = dll_first_node(l);
    for (i = 0; i < 2; i++)
        n = n->next;
    mu_assert_int_eq(15, *(int*)n->data);
    mu_assert_int_eq(1, *(int*)n->prev->data);
    mu_assert_int_eq(2, *(int*)n->next->data);

    /* ensure that everything points where it is needed */
    mu_assert_int_eq(15, *(int*)n->prev->next->data);
    mu_assert_int_eq(15, *(int*)n->next->prev->data);
}

MU_TEST(test_insert_second_half) {
    int i;
    for (i = 0; i < 10; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* q = calloc(1, sizeof(int));
    *q = 15;
    dll_insert(l, q, 8); /* insert at the second half */

    dll_node* n = dll_first_node(l);
    for (i = 0; i < 8; i++)
        n = n->next;
    mu_assert_int_eq(15, *(int*)n->data);
    mu_assert_int_eq(7, *(int*)n->prev->data);
    mu_assert_int_eq(8, *(int*)n->next->data);

    /* ensure that everything points where it is needed */
    mu_assert_int_eq(15, *(int*)n->prev->next->data);
    mu_assert_int_eq(15, *(int*)n->next->prev->data);
}


/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    /* setup */
    MU_RUN_TEST(test_default_setup);

    /* free */
    MU_RUN_TEST(test_freeing);

    /* append */
    MU_RUN_TEST(test_append);

    /* insert */
    MU_RUN_TEST(test_insert_beginning);
    MU_RUN_TEST(test_insert_end);
    MU_RUN_TEST(test_insert_first_half);
    MU_RUN_TEST(test_insert_second_half);
}


int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}
