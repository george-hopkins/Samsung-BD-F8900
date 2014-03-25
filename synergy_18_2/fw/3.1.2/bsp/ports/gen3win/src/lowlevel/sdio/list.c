/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "list.h"

/**
 * Initialize an empty list.
 *
 * @ingroup list
 */
void os_list_init(struct os_list *list)
{
    list->head.next = list->head.prev = &list->head;
}

/**
 * Is the list empty?
 *
 * @return true iff the list contains no nodes.
 *
 * @ingroup list
 */
int os_list_empty(struct os_list *list)
{
    return list->head.next == &list->head;
}

static void os_list_add(struct os_list_node *prev, struct os_list_node *new,
    struct os_list_node *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/**
 * Add a node to the tail of the list.
 *
 * @param list the list.
 * @param node the list node to add.
 *
 * @ingroup list
 */
void os_list_add_tail(struct os_list *list, struct os_list_node *node)
{
    os_list_add(list->head.prev, node, &list->head);
}

/**
 * Add a node to the head of the list.
 *
 * @param list the list.
 * @param node the list node to add.
 *
 * @ingroup list
 */
void os_list_add_head(struct os_list *list, struct os_list_node *node)
{
    os_list_add(&list->head, node, list->head.next);
}

/**
 * Remove a node from a list.
 *
 * @param node the node to remove.
 *
 * @ingroup list
 */
void os_list_del(struct os_list_node *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;

    node->prev = node->next = NULL;
}

/**
 * The node at the head of the list.
 *
 * @param list the list.
 *
 * @return the node at the head of the list; or os_list_end() if the
 * list is empty.
 *
 * @ingroup list
 */
struct os_list_node *os_list_head(struct os_list *list)
{
    return list->head.next;
}

/**
 * The node at the tail of the list.
 *
 * @param list the list.
 *
 * @return the node at the tail of the list; or os_list_end() if the
 * list is empty.
 *
 * @ingroup list
 */
struct os_list_node *os_list_tail(struct os_list *list)
{
    return list->head.prev;
}

/**
 * The node marking the end of a list.
 *
 * @param list the list.
 *
 * @return the node that marks the end of the list.
 *
 * @ingroup list
 */
struct os_list_node *os_list_end(struct os_list *list)
{
    return &list->head;
}

/**
 * Find the first node in a list matching some criteria.
 *
 * \e match should return true if the node matches; false otherwise.
 *
 * @param list  The list to search through.
 * @param match The function to test if a node matches.
 * @param arg   The argument to pass to \e match.
 *
 * @return The matching list node; or NULL if no matching node is
 * found.
 *
 * @ingroup list
 */
struct os_list_node *os_list_find(struct os_list *list, os_list_match_t match, void *arg)
{
    struct os_list_node *node;

    for (node = os_list_head(list); node != os_list_end(list); node = node->next)
    {
        if (match(node, arg))
        {
            return node;
        }
    }
    return NULL;
}

/**
 * Call a function for every node in a list.
 *
 * The \e func may safely call os_list_del() on its \e node parameter.
 *
 * @param list The list to iterate through.
 * @param func The function to call for each node.
 * @param arg  The argument to pass to \e func.
 *
 * @ingroup list
 */
void os_list_for_each(struct os_list *list, os_list_func_t func, void *arg)
{
    struct os_list_node *node, *next;

    for (node = os_list_head(list), next = node->next;
         node != os_list_end(list);
         node = next, next = node->next)
    {
        func(node, arg);
    }
}
