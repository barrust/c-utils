#ifndef BARRUST_LINKED_LIST_H__
#define BARRUST_LINKED_LIST_H__

/*******************************************************************************
***
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***
***  Version: 0.1.0
***  Purpose: Generic singly linked list implementation
***
***  License: MIT 2019
***
***  URL: https://github.com/barrust/c-utils
***
***  Usage:
***     #include "llist.h"
***     llist_t l = ll_init();
***     int i;
***     for (i = 0; i < 5; i++) {
***         int* t = calloc(1, sizeof(int));
***         *t = i;
***         ll_append(l, t);
***     }
***     // loop over the linked list
***     ll_node* n = ll_first_node(l);
***     while (n != NULL) {
***         // do something!
***         n = n->next;
***     }
***     ll_free_alt(l, true);
***
*******************************************************************************/

#include <stdbool.h>


typedef struct __linked_list llist;
typedef struct __linked_list *llist_t;


#define LL_SUCCESS           0
#define LL_FAILURE          -1


typedef struct __ll_node {
    void* data;
    struct __ll_node* next;
} ll_node;

/*  Initialize a linked list
    Returns:
        NULL    - if unable to allocate the memory
        llist_t
*/
llist_t ll_init();

/*  Free the data from the linked list
    NOTE: does not free the data element */
void ll_free(llist_t l);

/*  Free the data from the linked list;
    NOTE: to free the data, set free_data to true */
void ll_free_alt(llist_t l, bool free_data);

/*  Returns the number of nodes in the linked list */
size_t ll_num_elements(llist_t l);

/*  Return the head node from the linked list */
ll_node* ll_first_node(llist_t l);

/*  Insert a new node to the end of the linked list */
int ll_append(llist_t l, void* data);

/*  Insert a new node at the position pointed to by `idx` */
int ll_insert(llist_t l, void* data, size_t idx);

/*  Remove a node that at the position `idx` */
void* ll_remove(llist_t l, size_t idx);
void  ll_remove_alt(llist_t l, size_t idx, bool free_data);

/*  Traverse the list easily using the following macros */
#define ll_traverse(l, node)        for (node = ll_first_node(l); node != NULL; node = node->next)

#endif  /* BARRUST_LINKED_LIST_H__ */
