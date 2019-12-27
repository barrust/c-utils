#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include "llist.h"


typedef struct __linked_list {
    node* head;
    size_t elms;
} linked_list;


/*  PRIVATE FUNCTIONS */



llist_t ll_init() {
    llist_t l = calloc(1, sizeof(linked_list));
    l->head = NULL;
    l->elms = 0;
    return l;
}

void ll_free(llist_t l) {
    ll_free_alt(l, false);
}

void ll_free_alt(llist_t l, bool free_data) {
    node* n = l->head;
    while (n != NULL) {
        if (free_data == true)
            free(n->data);
        node* t = n->next;
        free(n);
        n = t;
    }
    free(l);
}

size_t ll_num_elements(llist_t l) {
    return l->elms;
}

node* ll_first_node(llist_t l) {
    if (l->elms == 0)
        return NULL;
    return l->head;
}

int ll_append(llist_t l, void* data) {
    node* n = calloc(1, sizeof(node));
    if (n == NULL)
        return LL_FAILURE;

    n->data = data; /* no copying; required by the user */
    n->next = NULL;
    /* now we need to place it at the end of the list... or at the beginning! */
    if (l->elms == 0) {
        l->head = n;
        ++l->elms;
        return LL_SUCCESS;
    }
    node* q = l->head;
    node* t = l->head;
    while (q != NULL) {
        t = q;
        q = q->next;
    }
    t->next = n;
    ++l->elms;
    return LL_SUCCESS;
}

int ll_insert(llist_t l, void* data, size_t idx) {
    if (idx >= l->elms)
        return LL_FAILURE;
    node* t = calloc(1, sizeof(node));
    t->data = data;

    if (idx == 0) {
        t->next = l->head;
        l->head = t;
        ++l->elms;
        return LL_SUCCESS;
    }

    // printf("idx: %d\n", (int)idx);

    node* n = l->head;
    node* trail = NULL;
    size_t i;
    for (i = 1; i < idx; i++) {
        trail = n;
        n = n->next;
    }
    trail->next = t;
    t->next = n;

    ++l->elms;
    return LL_SUCCESS;
}

void* ll_remove(llist_t l, size_t idx) {
    if (idx >= l->elms) {
        return NULL;
    }

    node* ret = l->head;
    if (idx == 0) {
        l->head = ret->next;
        --l->elms;
        void* data = ret->data;
        free(ret);
        return data;
    }
    node* t = NULL;
    size_t i;
    for (i = 0; i < idx; i++) {
        t = ret;
        ret = ret->next;
    }
    /* update the trailing "next" */
    t->next = ret->next;
    void* data = ret->data;
    free(ret);
    --l->elms;
    return data;
}

void ll_remove_alt(llist_t l, size_t idx, bool free_data) {
    void* res = ll_remove(l, idx);
    if (free_data == true)
        free(res);
}
