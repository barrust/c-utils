#ifndef BARRUST_DOUBLY_LINKED_LIST_H__
#define BARRUST_DOUBLY_LINKED_LIST_H__


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

int dll_insert(dllist_t l, void * data, int idx);

// void* dll_remove(dllist_t l, size_t idx);
// void dll_remove_alt(dllist_t l, size_t idx, bool free_data);


#endif
