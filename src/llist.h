#ifndef BARRUST_LINKED_LIST_H__
#define BARRUST_LINKED_LIST_H__


typedef struct __linked_list llist;
typedef struct __linked_list *llist_t;


#define LL_SUCCESS          0
#define LL_FAILURE          -1


typedef struct __node {
    void* data;
    struct __node* next;
} node;

/*  Initialize a linked list
    Returns:
        NULL    - if unable to allocate the memory
        llist_t
*/
llist_t ll_init();

/*  Free the data from the linked list; does not free the data element */
void ll_free(llist_t l);

/*  Free the data from the linked list where free_data determines if the data
    is freed using malloc */
void ll_free_alt(llist_t l, bool free_data);

/*  Returns the number of nodes in the linked list */
size_t ll_num_elements(llist_t l);

/*  Return the head node from the linked list */
node* ll_first_node(llist_t l);

/*  Insert a new node to the end of the linked list */
int ll_append(llist_t l, void* data);

/*  Insert a new node at the position pointed to by `idx` */
int ll_insert(llist_t l, void* data, size_t idx);

/*  Remove a node that at the position `idx` */
void* ll_remove(llist_t l, size_t idx);
void ll_remove_alt(llist_t l, size_t idx, bool free_data);



#endif  /* BARRUST_LINKED_LIST_H__ */
