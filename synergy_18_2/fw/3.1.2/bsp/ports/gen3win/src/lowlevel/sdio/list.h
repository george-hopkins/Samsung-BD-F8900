#ifndef LIST_H__
#define LIST_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup list Linked Lists
 *
 * Generic linked list implementations suitable for all platforms.
 *
 *   - Circular, doubly-linked list (struct os_list).
 */

/**
 * A list node.
 *
 * This list node structure should be the first field within any
 * structure that is to be stored in a list.
 *
 * @see struct os_list
 * @ingroup list
 */
struct os_list_node
{
    /**
     * The pointer to the previous node in the list, or os_list_end()
     * if the end of the list has been reached.
     */
    struct os_list_node *prev;
    /**
     * The pointer to the next node in the list, or os_list_end() if
     * the end of the list has been reached.
     */
    struct os_list_node *next;
};

/**
 * A circular, doubly-linked list of nodes.
 *
 * Structures to be stored in a list should contains a struct
 * os_list_node as the \e first field.
 * \code
 *   struct foo {
 *      struct os_list_node node;
 *      int bar;
 *      ...
 *   };
 * \endcode
 * Going to/from a struct foo to a list node is then simple.
 * \code
 *   struct os_list_node *node;
 *   struct foo *foo;
 *   [...]
 *   node = &foo->node;
 *   foo = (struct foo *)node
 * \endcode
 * Lists must be initialized with os_list_init() before adding nodes
 * with os_list_add_tail().  The node at the head of the list is
 * obtained with os_list_head().  Nodes are removed from the list with
 * os_list_del().
 *
 * A list can be interated from the head to the tail using:
 * \code
 *   struct os_list_node *node;
 *   for (node = os_list_head(list); node != os_list_end(list); node = node->next) {
 *      struct foo *foo = (struct foo *)node;
 *      ...
 *   }
 * \endcode
 *
 * In the above loop, the current list node cannot be removed (with
 * os_list_del()).  If this is required use this form of loop:
 * \code
 *   struct os_list_node *node, *next;
 *   for (node = os_list_head(list), next = node->next;
 *        node != os_list_end(list);
 *        node = next, next = node->next) {
 *      struct foo *foo = (struct foo *)node;
 *      ...
 *      os_list_del(node);
 *      ...
 *   }
 * \endcode
 *
 * A list can be be iterated in reverse order from the head to the
 * tail using:
 *
 * \code
 *   struct os_list_node *node;
 *   for (node = os_list_tail(list); node != os_list_end(list); node = node->prev) {
 *      struct foo *foo = (struct foo *)node;
 *      ...
 *   }
 * \endcode
 *
 * @ingroup list
 */
struct os_list
{
    /**
     * @internal
     * The dummy node marking the end of the list.
     */
    struct os_list_node head;
};

/**
 * Matching function for os_list_find().
 *
 * @param node The list node to check if it matches.
 * @param arg  The argument passed to os_list_find().
 *
 * @return true if node matches; false otherwise.
 *
 * @ingroup list
 */
typedef CsrBool (*os_list_match_t)(struct os_list_node *node, void *arg);

/**
 * Function for os_list_for_each().
 *
 * @param node The list node to operate on.
 * @param arg  The argument passed to os_list_for_each().
 *
 * @ingroup list
 */
typedef void (*os_list_func_t)(struct os_list_node *node, void *arg);

void os_list_init(struct os_list *list);
int os_list_empty(struct os_list *list);
void os_list_add_tail(struct os_list *list, struct os_list_node *node);
void os_list_add_head(struct os_list *list, struct os_list_node *node);
void os_list_del(struct os_list_node *node);
struct os_list_node *os_list_head(struct os_list *list);
struct os_list_node *os_list_tail(struct os_list *list);
struct os_list_node *os_list_end(struct os_list *list);
struct os_list_node *os_list_find(struct os_list *list, os_list_match_t match, void *arg);
void os_list_for_each(struct os_list *list, os_list_func_t func, void *arg);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef LIST_H__ */
