#ifndef BARRUST_QUEUE_H__
#define BARRUST_QUEUE_H__

/*******************************************************************************
***
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***
***  Version: 0.1.0
***  Purpose: Generic queue (first in first out [FIFO]) implementation
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


typedef struct __doubly_linked_list q_list;
typedef struct __doubly_linked_list *queue_list_t;


#define QUEUE_SUCCESS          0
#define QUEUE_FAILURE         -1


typedef struct __dll_node {
    void* data;
    struct __dll_node* next;
    struct __dll_node* prev;
} queue_node;


/*  Initialize the doubly linked list
    Returns:
        NULL        - If error allocating the memory
        dllist_t
*/
queue_list_t q_init();

/*  Free the data from the doubly linked list;
    NOTE: does not free the data element */
void q_free(queue_list_t q);

/*  Free the data from the doubly linked list;
    NOTE: to free the data, set free_data to true */
void q_free_alt(queue_list_t q, bool free_data);

/*  Returns the number of nodes in the doubly linked list */
size_t q_num_elements(queue_list_t q);

/*  Return the head node from the doubly linked list */
queue_node* q_first_node(queue_list_t q);

/*  Return the tail node from the doubly linked list */
queue_node* q_last_node(queue_list_t q);


/*  Insert a new node into the queue */
int q_push(queue_list_t q, void * data);

/*  Pop the node from the front of the queue */
void* q_pop(queue_list_t q);
void  q_pop_alt(queue_list_t q, bool free_data);

/*  Traverse the list easily using the following macros */
#define q_traverse(q, node)               for (node = q_first_node(q); node != NULL; node = node->next)
#define q_reverse_traverse(q, node)       for (node = q_last_node(q); node != NULL; node = node->prev)

#endif
