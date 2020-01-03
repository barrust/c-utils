#ifndef BARRUST_DOUBLY_LINKED_LIST_H__
#define BARRUST_DOUBLY_LINKED_LIST_H__

/*******************************************************************************
***
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***
***  Version: 0.1.0
***  Purpose: Generic doubly linked list implementation
***
***  License: MIT 2019
***
***  URL: https://github.com/barrust/c-utils
***
***  Usage:
***     #include "dllist.h"
***     dllist_t l = dll_init();
***     int i;
***     for (i = 0; i < 5; i++) {
***         int* t = calloc(1, sizeof(int));
***         *t = i;
***         dll_append(l, t);
***     }
***     // loop over the linked list
***     dll_node* n = dll_first_node(l);
***     while (n != NULL) {
***         // do something!
***         n = n->next;
***     }
***     dll_free_alt(l, true);
***
*******************************************************************************/


#include <stdbool.h>



typedef struct __doubly_linked_list dllist;
typedef struct __doubly_linked_list *dllist_t;


#define DLL_SUCCESS          0
#define DLL_FAILURE         -1


typedef struct __dll_node {
    void* data;
    struct __dll_node* next;
    struct __dll_node* prev;
} dll_node;


/*  Initialize the doubly linked list
    Returns:
        NULL        - If error allocating the memory
        dllist_t
*/
dllist_t dll_init();

/*  Free the data from the doubly linked list;
    NOTE: does not free the data element */
void dll_free(dllist_t l);

/*  Free the data from the doubly linked list;
    NOTE: to free the data, set free_data to true */
void dll_free_alt(dllist_t l, bool free_data);

/*  Returns the number of nodes in the doubly linked list */
size_t dll_num_elements(dllist_t l);

/*  Return the head node from the doubly linked list */
dll_node* dll_first_node(dllist_t l);

/*  Return the tail node from the doubly linked list */
dll_node* dll_last_node(dllist_t l);

/*  Insert a new node to the end of the doubly linked list */
int dll_append(dllist_t l, void* data);

/*  Insert a new node at the position pointed to by `idx` */
int dll_insert(dllist_t l, void * data, int idx);

/*  Remove a node that at the position `idx` */
void* dll_remove(dllist_t l, int idx);
void  dll_remove_alt(dllist_t l, size_t idx, bool free_data);

/*  Traverse the list easily using the following macros */
#define dll_traverse(l, node)               for (node = dll_first_node(l); node != NULL; node = node->next)
#define dll_reverse_traverse(l, node)       for (node = dll_last_node(l); node != NULL; node = node->prev)
#endif
