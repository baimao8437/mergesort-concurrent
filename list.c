#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "generic_printf.h"

static val_t nameToVal(char *lastName)
{
    val_t val = 0;
    for (int i = 0; i < 12; i++) {
        val *= 26;
        if (lastName[i] == '\n') {
        } else if (lastName[i]) {
            val += lastName[i] - 'a' + 1;
        }
    }
    return val;
}

/**
 * @brief Create a new node with data _val_ and set the next node to _net_
 * @param val Specifiy the data to assign to the new node
 * @param next Pointer to the next node
 * @return Pointer to the created new node
 */
static node_t *node_new(char *lastName, node_t *next)
{
    /* allocate node */
    node_t *node = malloc(sizeof(node_t));
    node->lastName = lastName;
    node->data = nameToVal(lastName);
    node->next = next;
    return node;
}

/**
 * @brief Initialize the linked list.
 *
 * The function will allocate a block of memory for _llist\_t_ and
 * initialize its members _head_ to NULL and _size_ to 0.
 *
 * @return Pointer to the allocated _llist\_t_
 */
llist_t *list_new()
{
    /* allocate list */
    llist_t *list = malloc(sizeof(llist_t));
    list->head = NULL;
    list->size = 0;
    return list;
}

/**
 * @brief Insert a new node with the given value val at the head of the _list_
 * @param list The target linked list
 * @param val Specify the value
 * @return The final size of the linked list
 */
int list_add(llist_t * const list, char *lastName)
{
    node_t *e = node_new(lastName, NULL);
    e->next = list->head;
    list->head = e;
    list->size++;
    return list->size;
}

/**
 * @brief Get the node specified by index
 * If the index is out of range, it will return NULL.
 *
 * @param list The target linked list
 * @param index Specify the index of the node in the _list_
 * @return The node at index _index_.
 */
node_t *list_get(llist_t * const list, const uint32_t index)
{
    uint32_t idx = index;
    if (!(idx < list->size))
        return NULL;
    node_t *head = list->head;
    while (idx--)
        head = head->next;
    return head;
}

/**
 * @brief Display the data of all nodes in the linked list
 * @param list The target linked list
 */
void list_print(const llist_t * const list)
{
    const node_t *cur = list->head;
    while (cur) {
        xprint(cur->lastName);
        cur = cur->next;
    }
}

/**
 * @brief Release the memory allocated to nodes in the linked list
 * @param list The target linked list
 */
void list_free_nodes(llist_t *list)
{
    node_t *cur = list->head;
    while (cur) {
        cur = cur->next;
        free(cur);
    }
    list->head = NULL;
}

/* API gateway */
struct  __LIST_API__ List = {
    .list_new=list_new,
    .list_add=list_add,
    .list_print= list_print,
    .list_get = list_get,
    .list_free_nodes=list_free_nodes,
};
