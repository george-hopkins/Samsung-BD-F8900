#ifndef CSR_BT_AMPM_GEN_LIST_H
#define CSR_BT_AMPM_GEN_LIST_H

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CsrBtAmpmGenListMember
{
    struct CsrBtAmpmGenListMember *previous;
    struct CsrBtAmpmGenListMember *next;
} CsrBtAmpmGenListMember;

typedef struct
{
    CsrBtAmpmGenListMember *head;
    CsrBtAmpmGenListMember *tail;
    int count;
} CstBtAmpmGenListAnchor;

/* Provides the caller with an appropriately initialised double linked
 * list anchor structure. Prevents the caller having to know how the
 * anchor is initialised. */
extern CstBtAmpmGenListAnchor *CsrBtAmpmListCreateAnchor(
    void);

/* Will destroy the list anchor. NOTE: the list must be empty as this
 * routine doesn't free any list members the owner of the list is
 * responsible for that. The list pointer is set to NULL. */
extern void CsrBtAmpmListDestroyAnchor(
    CstBtAmpmGenListAnchor **pList);

/* Removes item from start of list and returns item. */
extern CsrBtAmpmGenListMember *CsrBtAmpmListRemoveHead(
    CstBtAmpmGenListAnchor * pList);

/* Removes item from end of list and returns item. */
extern CsrBtAmpmGenListMember *CsrBtAmpmListRemoveTail(
    CstBtAmpmGenListAnchor * pList);

/* Removes the specified item from the list. */
extern void CsrBtAmpmListRemoveMember(
    CstBtAmpmGenListAnchor * pList,
    CsrBtAmpmGenListMember * pRemoveMember);

/* Iterates over the list using the matching function find a member
 * that matches the criteria and then remove it from the
 * list. Returning apointer to any removed element. Only the first
 * matching entry is removed. */
extern CsrBtAmpmGenListMember *CsrBtAmpmListFindMatchingMember(
    const CstBtAmpmGenListAnchor * pList,
    CsrBool(*fpisRequiredMember) (const void *, const void *),
    const void *pMatchCriteria);

extern CsrBtAmpmGenListMember *CsrBtAmpmListRemoveMatchingMember(
    CstBtAmpmGenListAnchor * pList,
    CsrBool(*fpisRequiredEntry) (const void *, const void *),
    const void *pMatchCriteria);

/* Adds new member to end of list. */
extern void CsrBtAmpmListAddTail(
    CstBtAmpmGenListAnchor * pList,
    CsrBtAmpmGenListMember * pNewMember);

/* Adds new member to head of list. */
extern void CsrBtAmpmListAddHead(
    CstBtAmpmGenListAnchor * pList,
    CsrBtAmpmGenListMember * pNewMember);

/* Returns whether the list is empty or not */
extern CsrBool CsrBtAmpmListIsEmpty(
    const CstBtAmpmGenListAnchor * pList);

/* Returns the number of elements in the list */
extern int CsrBtAmpmListCount(
    const CstBtAmpmGenListAnchor * pList);

/* Returns TRUE if the entry matches the pointer */
extern CsrBool CsrBtAmpmListEntryMatchesPtr(
    const void *pCriteria,
    const void *pListEntry);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_AMPM_GEN_LIST_H */
