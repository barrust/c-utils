/*******************************************************************************
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***  License: MIT 2020
*******************************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "stack.h"


typedef struct __linked_list {
    stack_node* head;
    size_t elms;
} linked_list;


stack_list_t stk_init() {
    stack_list_t stk = (stack_list_t)calloc(1, sizeof(linked_list));
    stk->head = NULL;
    stk->elms = 0;
    return stk;
}

void stk_free(stack_list_t stk) {
    stk_free_alt(stk, false);
}

void stk_free_alt(stack_list_t stk, bool free_data) {
    stack_node* n = stk->head;
    while (n != NULL) {
        if (free_data == true)
            free(n->data);
        n->data = NULL;
        stack_node* t = n;
        n = n->next;
        free(t);
    }
    stk->elms = 0;
    stk->head = NULL;
    free(stk);
}

size_t stk_num_elements(stack_list_t stk) {
    return stk->elms;
}

stack_node* stk_first_node(stack_list_t stk) {
    return stk->head;
}

int stk_push(stack_list_t stk, void* data) {
    /* setup the node to add */
    stack_node* n = (stack_node*)calloc(1, sizeof(stack_node));
    if (n == NULL)
        return STACK_FAILURE;

    n->data = data;
    n->next = stk->head;
    stk->head = n;

    ++(stk->elms);
    return STACK_SUCCESS;
}

void stk_pop_alt(stack_list_t stk, bool free_data) {
    void* ret = stk_pop(stk);
    if (free_data == true)
        free(ret);
}

void* stk_pop(stack_list_t stk) {
    if (stk->elms == 0)
        return NULL;

    void* data;
    stack_node* ret = stk->head;
    stk->head = ret->next;

    data = ret->data;
    --(stk->elms);
    free(ret);
    return data;
}
