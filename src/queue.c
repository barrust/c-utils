/*******************************************************************************
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***  License: MIT 2020
*******************************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "queue.h"


typedef struct __doubly_linked_list {
    queue_node* head;
    queue_node* tail;
    size_t elms;
} doubly_linked_list;


queue_list_t q_init() {
    queue_list_t q = (queue_list_t)calloc(1, sizeof(doubly_linked_list));
    q->head = NULL;
    q->tail = NULL;
    q->elms = 0;
    return q;
}

void q_free(queue_list_t q) {
    q_free_alt(q, false);
}

void q_free_alt(queue_list_t q, bool free_data) {
    queue_node* n = q->head;
    while (n != NULL) {
        if (free_data == true)
            free(n->data);
        n->data = NULL;
        queue_node* t = n;
        n = n->next;
        free(t);
    }
    q->elms = 0;
    q->head = NULL;
    q->tail = NULL;
    free(q);
}

size_t q_num_elements(queue_list_t q) {
    return q->elms;
}

queue_node* q_first_node(queue_list_t q) {
    return q->head;
}

queue_node* q_last_node(queue_list_t q) {
    return q->tail;
}

int q_push(queue_list_t q, void* data) {
    /* setup the node to add */
    queue_node* n = (queue_node*)calloc(1, sizeof(queue_node));
    if (n == NULL)
        return QUEUE_FAILURE;

    n->data = data;
    n->next = NULL;
    n->prev = NULL;
    if (q->elms == 0) {  /* first node to be added edge case */
        q->head = n;
    } else {  /* we want the tail to point to the end */
        q->tail->next = n;
        n->prev = q->tail;
    }

    q->tail = n;
    ++(q->elms);
    return QUEUE_SUCCESS;
}

void q_pop_alt(queue_list_t q, bool free_data) {
    void* ret = q_pop(q);
    if (free_data == true)
        free(ret);
}

void* q_pop(queue_list_t q) {
    if (q->elms == 0)
        return NULL;

    void* data;
    queue_node* ret = q->head;
    if (q->elms == 1) { /* remove the final node */
        q->head = q->tail = NULL;
    } else {
        q->head = q->head->next;
        q->head->prev = NULL;
    }

    data = ret->data;
    --(q->elms);
    free(ret);
    return data;
}
