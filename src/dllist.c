#include <stdlib.h>
#include <stdbool.h>
#include "dllist.h"



typedef struct __doubly_linked_list {
    dll_node* head;
    dll_node* tail;
    size_t elms;
} doubly_linked_list;


dllist_t dll_init() {
    dllist_t l = calloc(1, sizeof(doubly_linked_list));
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
        if (free_data == true)
            free(n->data);
        dll_node* t = n;
        n = n->next;
        free(t);
    }
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
    dll_node* n = calloc(1, sizeof(dll_node));
    if (n == NULL)
        return DLL_FAILURE;

    n->data = data;
    n->next = NULL;
    n->prev = NULL;

    if (l->elms == 0) {
        l->tail = n;
        l->head = n;
        ++(l->elms);
        return DLL_SUCCESS;
    }

    l->tail->next = n;
    n->prev = l->tail;
    l->tail = n;
    ++(l->elms);

    return DLL_SUCCESS;
}

int dll_insert(dllist_t l, void * data, int idx) {
    if (idx < 0 && idx <= (-1 * (int)l->elms))
        return DLL_FAILURE;

    if (idx < 0)
        idx = l->elms + idx + 1;  /* adding 1 makes -1 like 0 from the tail */

    /* setup the node to add */
    dll_node* n = calloc(1, sizeof(dll_node));
    if (n == NULL)
        return DLL_FAILURE;

    n->data = data;
    n->next = NULL;
    n->prev = NULL;

    if (idx == 0) {
        n->next = (l->head);
        n->next->prev = n;
        l->head = n;
        ++(l->elms);
        return DLL_SUCCESS;
    } else if (idx >= (int)l->elms) {
        l->tail->next = n;
        n->prev = l->tail; /* we want the tail to point to the end */
        l->tail = n;
        ++(l->elms);
        return DLL_SUCCESS;
    }

    int i;
    dll_node* t;
    if (idx <= (int)l->elms / 2) {
        t = dll_first_node(l);
        for (i = 1; i < idx; i++)
            t = t->next;
    } else {
        /* start from the tail and go backwards! */
        int stop = l->elms - idx;
        t = dll_last_node(l);
        for (i = 0; i < stop; i++)
            t = t->prev;
    }

    n->next = t->next;
    t->next = n;
    n->next->prev = n;
    n->prev = t;
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

    void* ret;
    dll_node* n;
    if (idx == 0) { /* handle edge cases */
        n = l->head;
        ret = n->data;
        l->head = l->head->next;
        l->head->prev = NULL;
        --l->elms;
        free(n);
        return ret;
    } else if (idx >= (int)(l->elms - 1)) {
        n = l->tail;
        ret = n->data;
        l->tail = n->prev;
        n->prev->next = NULL;
        --l->elms;
        free(n);
        return ret;
    }

    int i;
    if (idx <= (int)l->elms / 2) {
        n = dll_first_node(l);
        for (i = 1; i < idx; i++)
            n = n->next;
    } else {
        /* start from the tail and go backwards! */
        int stop = l->elms - idx;
        n = dll_last_node(l);
        for (i = 0; i < stop; i++)
            n = n->prev;
    }

    /* mode the nodes before and after's pointers around */
    n->prev->next = n->next;
    n->next->prev = n->prev;
    ret = n->data;
    --l->elms;
    free(n);
    return ret;
}
