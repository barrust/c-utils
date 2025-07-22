/*******************************************************************************
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***  License: MIT 2019
*******************************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "dllist.h"


typedef struct __doubly_linked_list {
    dll_node* head;
    dll_node* tail;
    size_t elms;
} doubly_linked_list;


dllist_t dll_init(void) {
    dllist_t l = (dllist_t)calloc(1, sizeof(doubly_linked_list));
    l->head = NULL;
    l->tail = NULL;
    l->elms = 0;
    return l;
}

void dll_free(dllist_t l) {
    dll_free_alt(l, false);
}

void dll_free_alt(dllist_t l, bool free_data) {
    dll_node* n = l->head;
    while (n != NULL) {
        if (free_data == true) {
            free(n->data);
            n->data = NULL;
        }
        dll_node* t = n;
        n = n->next;
        free(t);
        t = NULL;
    }
    l->head = NULL;
    l->tail = NULL;
    l->elms = 0;
    free(l);
}

size_t dll_num_elements(dllist_t l) {
    return l->elms;
}

dll_node* dll_first_node(dllist_t l) {
    return l->head;
}

dll_node* dll_last_node(dllist_t l) {
    return l->tail;
}

int dll_append(dllist_t l, void* data) {
    return dll_insert(l, data, -1);
}

int dll_insert(dllist_t l, void* data, int idx) {
    if (idx != -1 && idx < 0 && idx <= (-1 * (int)l->elms))
        return DLL_FAILURE;

    if (idx < 0)
        idx = l->elms + idx + 1;  /* adding 1 makes -1 like 0 from the tail */

    /* setup the node to add */
    dll_node* n = (dll_node*)calloc(1, sizeof(dll_node));
    if (n == NULL)
        return DLL_FAILURE;

    n->data = data;
    n->next = NULL;
    n->prev = NULL;

    if (l->elms == 0) { /* first node to be added edge case */
        l->head = n;
        l->tail = n;
    } else if (idx == 0) { /* first node */
        n->next = l->head;
        n->next->prev = n;
        l->head = n;
    } else if (idx >= (int)l->elms) { /* last node */
        l->tail->next = n;
        n->prev = l->tail; /* we want the tail to point to the end */
        l->tail = n;
    } else { /* mid node; determine which direction would be fastest to get to it */
        int i;
        dll_node* t;
        if (idx <= (int)l->elms / 2) {
            t = dll_first_node(l);
            for (i = 1; i < idx; ++i)
                t = t->next;
        } else {
            /* start from the tail and go backwards! */
            int stop = l->elms - idx;
            t = dll_last_node(l);
            for (i = 0; i < stop; ++i)
                t = t->prev;
        }

        n->next = t->next;
        t->next = n;
        n->next->prev = n;
        n->prev = t;
    }

    ++(l->elms);
    return DLL_SUCCESS;
}

void dll_remove_alt(dllist_t l, size_t idx, bool free_data) {
    void* ret = dll_remove(l, idx);
    if (free_data == true)
        free(ret);
}

void* dll_remove(dllist_t l, int idx) {
    if (idx < 0 && (idx * -1) >= (int)l->elms)
        return NULL;

    if (idx < 0)
        idx = l->elms + idx;

    void* data;
    dll_node* ret;
    if (l->elms == 1) { /* remove the final node */
        ret = l->head;
        l->head = l->tail = NULL;
    } else if (idx == 0) { /* remove the first node */
        ret = l->head;
        l->head = l->head->next;
        l->head->prev = NULL;
    } else if (idx >= (int)(l->elms - 1)) {
        ret = l->tail;
        l->tail = ret->prev;
        ret->prev->next = NULL;
    } else {
        int i;
        if (idx <= (int)l->elms / 2) {
            ret = dll_first_node(l);
            for (i = 1; i < idx; ++i)
                ret = ret->next;
        } else {
            /* start from the tail and go backwards! */
            int stop = l->elms - idx;
            ret = dll_last_node(l);
            for (i = 0; i < stop; ++i)
                ret = ret->prev;
        }

        /* move the nodes before and after's pointers around */
        ret->prev->next = ret->next;
        ret->next->prev = ret->prev;
    }
    data = ret->data;
    --(l->elms);
    free(ret);
    ret = NULL;
    return data;
}
