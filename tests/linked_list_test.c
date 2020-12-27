#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/minunit.h"
#include "../src/llist.h"

llist_t l;

void test_setup(void) {
    l = ll_init();
}

void test_teardown(void) {
    ll_free_alt(l, true);
    l = NULL;
}


/* private test function - uncomment if needed! */
/*
static void printlist(llist_t l) {
    int i = 0;
    ll_node* n = ll_first_node(l);
    printf("\n");
    while (n != NULL) {
        int* val = (int*) n->data;
        printf("idx: %d\tval: %d\n", i++, *val);
        n = n->next;
    }
}
*/

/*******************************************************************************
*   Test the setup
*******************************************************************************/
MU_TEST(test_default_setup) {
    mu_assert_int_eq(0, ll_num_elements(l));
    mu_assert_null(ll_first_node(l));
}


/*******************************************************************************
*   Test Free-ing
*******************************************************************************/
MU_TEST(test_freeing) {
    llist_t a = ll_init();
    mu_assert_int_eq(0, ll_num_elements(l));
    ll_free(a);
}


/*******************************************************************************
*   Test traversing elements
*******************************************************************************/
MU_TEST(test_traverse) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = (int*)calloc(1, sizeof(int));
        *t = i;
        ll_append(l, t);
    }

    int j = 0;
    ll_node* n;
    ll_traverse(l, n) {
        int* val = (int*)n->data;
        mu_assert_int_eq(j++, *val);
    }
}


/*******************************************************************************
*   Test appending elements
*******************************************************************************/
MU_TEST(test_append) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = (int*)calloc(1, sizeof(int));
        *t = i;
        ll_append(l, t);
    }

    int j = 0;
    ll_node* n = ll_first_node(l);
    while (n != NULL) {
        int* val = (int*)n->data;
        mu_assert_int_eq(j++, *val);
        n = n->next;
    }
}


/*******************************************************************************
*   Test inserting elements
*******************************************************************************/
MU_TEST(test_insert_beginning) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = (int*)calloc(1, sizeof(int));
        *t = i;
        ll_append(l, t);
    }

    int* q = (int*)calloc(1, sizeof(int));
    *q = 15;
    ll_insert(l, q, 0); // insert at the beginning

    ll_node* n = ll_first_node(l);
    mu_assert_int_eq(15, *(int*)n->data);
    mu_assert_int_eq(0, *(int*)n->next->data);
    mu_assert_int_eq(6, ll_num_elements(l));

    int w = 0;
    n = n->next;
    while(n != NULL) {
        int* val = (int*)n->data;
        mu_assert_int_eq(w++, *val);
        n = n->next;
    }
}

MU_TEST(test_insert_mid) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = (int*)calloc(1, sizeof(int));
        *t = i;
        ll_append(l, t);
    }

    int* q = (int*)calloc(1, sizeof(int));
    *q = 15;
    ll_insert(l, q, 2); // insert somewhere in the middle
    // printlist(l);
    mu_assert_int_eq(6, ll_num_elements(l));

    ll_node* n = ll_first_node(l);
    n = n->next->next;
    mu_assert_int_eq(15, *(int*)n->data);
}

MU_TEST(test_insert_end_to_large) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = (int*)calloc(1, sizeof(int));
        *t = i;
        ll_append(l, t);
    }

    int* q = (int*)calloc(1, sizeof(int));
    *q = 15;
    ll_insert(l, q, 7); // insert somewhere after the end...
    mu_assert_int_eq(6, ll_num_elements(l));
}


/*******************************************************************************
*   Test removing elements
*******************************************************************************/
MU_TEST(test_remove_last_element) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = (int*)calloc(1, sizeof(int));
        *t = i;
        ll_append(l, t);
    }
    mu_assert_int_eq(5, ll_num_elements(l));
    /* remove the last element */
    void* res = ll_remove(l, 4);  /* remember it is 0 based */
    mu_assert_int_eq(4, ll_num_elements(l));
    mu_assert_int_eq(4, *(int*)res);

    int w = 0;
    ll_node* n = ll_first_node(l);
    while(n != NULL) {
        int* val = (int*)n->data;
        mu_assert_int_eq(w++, *val);
        n = n->next;
    }

    free(res);
}

MU_TEST(test_remove_mid_element) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = (int*)calloc(1, sizeof(int));
        *t = i;
        ll_append(l, t);
    }
    mu_assert_int_eq(5, ll_num_elements(l));
    /* remove the last element */
    void* res = ll_remove(l, 1);  /* remember it is 0 based */
    mu_assert_int_eq(4, ll_num_elements(l));
    mu_assert_int_eq(1, *(int*)res);


    int w = 0;
    ll_node* n = ll_first_node(l);
    while(n != NULL) {
        int* val = (int*)n->data;
        if (w < 1)
            mu_assert_int_eq(w++, *val);
        else
            mu_assert_int_eq(1 + w++, *val);
        n = n->next;
    }
    free(res);
}

MU_TEST(test_remove_first_element) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = (int*)calloc(1, sizeof(int));
        *t = i;
        ll_append(l, t);
    }
    mu_assert_int_eq(5, ll_num_elements(l));
    /* remove the last element */
    void* res = ll_remove(l, 0);  /* remember it is 0 based */
    mu_assert_int_eq(4, ll_num_elements(l));
    mu_assert_int_eq(0, *(int*)res);
    free(res);

    int w = 1; /* 0 should have been removed! */
    ll_node* n = ll_first_node(l);
    while(n != NULL) {
        int* val = (int*)n->data;
        mu_assert_int_eq(w++, *val);
        n = n->next;
    }
}

MU_TEST(test_remove_final_element) {
    /* do something here! */
    int i;
    for (i = 0; i < 1; i++) {
        int* t = (int*)calloc(1, sizeof(int));
        *t = i;
        ll_append(l, t);
    }

    void* res = ll_remove(l, 0); /* the only element */
    mu_assert_int_eq(0, ll_num_elements(l));
    mu_assert_int_eq(0, *(int*)res);
    free(res);
}

MU_TEST(test_remove_alt) {
    /* remove alt free's the memory if desired */
    int i;
    for (i = 0; i < 5; i++) {
        int* t = (int*)calloc(1, sizeof(int));
        *t = i;
        ll_append(l, t);
    }
    ll_remove_alt(l, 0, true);  /* remember it is 0 based */
    mu_assert_int_eq(4, ll_num_elements(l));
}

MU_TEST(test_remove_error) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = (int*)calloc(1, sizeof(int));
        *t = i;
        ll_append(l, t);
    }
    mu_assert_int_eq(5, ll_num_elements(l));

    void* res = ll_remove(l, 5);  /* remember it is 0 based so this is out of bounds */
    mu_assert_null(res);
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

    /* traverse */
    MU_RUN_TEST(test_traverse);

    /* insert */
    MU_RUN_TEST(test_append);
    MU_RUN_TEST(test_insert_beginning);
    MU_RUN_TEST(test_insert_mid);
    MU_RUN_TEST(test_insert_end_to_large);

    /* remove */
    MU_RUN_TEST(test_remove_last_element);
    MU_RUN_TEST(test_remove_mid_element);
    MU_RUN_TEST(test_remove_first_element);
    MU_RUN_TEST(test_remove_alt);
    MU_RUN_TEST(test_remove_final_element);
    MU_RUN_TEST(test_remove_error);
}


int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}
