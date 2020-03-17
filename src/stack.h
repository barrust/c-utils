#ifndef BARRUST_STACK_H__
#define BARRUST_STACK_H__

/*******************************************************************************
***
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***
***  Version: 0.1.0
***  Purpose: Generic stack (last in first out [LIFO]) implementation
***
***  License: MIT 2020
***
***  URL: https://github.com/barrust/c-utils
***
***  Usage:
***
***
*******************************************************************************/

#include <stdbool.h>


#include <stdbool.h>


typedef struct __linked_list stack;
typedef struct __linked_list *ll_stack_t;


#define STACK_SUCCESS           0
#define STACK_FAILURE          -1


typedef struct __stack_node {
    void* data;
    struct __stack_node* next;
} stack_node;

/*  Initialize the singly linked list
    Returns:
        NULL        - If error allocating the memory
        ll_stack_t
*/
ll_stack_t stk_init();

/*  Free the data from the singly linked list;
    NOTE: does not free the data element */
void stk_free(ll_stack_t stk);

/*  Free the data from the singly linked list;
    NOTE: to free the data, set free_data to true */
void stk_free_alt(ll_stack_t stk, bool free_data);

/*  Returns the number of nodes in the doubly linked list */
size_t stk_num_elements(ll_stack_t stk);

/*  Return the head node from the singly linked list */
stack_node* stk_first_node(ll_stack_t stk);

/*  Insert a new node into the queue */
int stk_push(ll_stack_t stk, void * data);

/*  Pop the node from the front of the queue */
void* stk_pop(ll_stack_t stk);
void  stk_pop_alt(ll_stack_t stk, bool free_data);

/*  Traverse the list easily using the following macros */
#define stk_traverse(stk, node)               for (node = stk_first_node(stk); node != NULL; node = node->next)

#endif
