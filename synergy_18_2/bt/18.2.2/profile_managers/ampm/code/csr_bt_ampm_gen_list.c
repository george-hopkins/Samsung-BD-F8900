/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

#define CSR_BT_AMPM_IS_VALID_LIST(pList) \
    ((0 == CsrBtAmpmListCount(pList) && \
     (NULL == pList->head && NULL == pList->tail)) || \
     (1 == CsrBtAmpmListCount(pList) && \
     (pList->head == pList->tail)) || \
     (1 < CsrBtAmpmListCount(pList) && \
     (NULL != pList->head && NULL != pList->tail)))

CstBtAmpmGenListAnchor *CsrBtAmpmListCreateAnchor(void)
{
    CstBtAmpmGenListAnchor *pNewList;

    pNewList = (CstBtAmpmGenListAnchor *) CsrPmemZalloc(sizeof(*pNewList));

    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pNewList));

    return (pNewList);
}

void CsrBtAmpmListDestroyAnchor(CstBtAmpmGenListAnchor **pList)
{
    CSR_BT_AMPM_ASSERT(*pList != NULL);
    CSR_BT_AMPM_ASSERT(TRUE == CsrBtAmpmListIsEmpty(*pList));
    CsrPmemFree((void *) *pList);
    *pList = NULL;
}

CsrBool CsrBtAmpmListIsEmpty(
    const CstBtAmpmGenListAnchor * pList)
{
    CsrBool isListEmpty = FALSE;

    CSR_BT_AMPM_ASSERT(pList != NULL);
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));

    if (NULL == pList->head && NULL == pList->tail &&
        0 == CsrBtAmpmListCount(pList))
    {
        isListEmpty = TRUE;
    }

    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));
    return (isListEmpty);
}

CsrBtAmpmGenListMember *CsrBtAmpmListRemoveHead(
    CstBtAmpmGenListAnchor * pList)
{
    CsrBtAmpmGenListMember *pHeadMember;

    /* Even if the list is empty we must first be passed a
     * valid pointer to the list header object that achors
     * the list.
     */
    CSR_BT_AMPM_ASSERT(pList != NULL);
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));

    pHeadMember = pList->head;
    if (NULL != pHeadMember)
    {
        /* Adjust the start of the list to unlink the first entry */
        pList->head = pHeadMember->next;
        if (NULL != pList->head)
        {
            pList->head->previous = NULL;
        }
        else
        {
            /* head is the tail */
            pList->tail = NULL;
        }
        --pList->count;

        /* NULL the pointers in the member that we are returning */
        pHeadMember->next = NULL;
        pHeadMember->previous = NULL;
    }
    /* Still has to be a valid list */
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));
    return (pHeadMember);
}

CsrBtAmpmGenListMember *CsrBtAmpmListRemoveTail(
    CstBtAmpmGenListAnchor * pList)
{
    CsrBtAmpmGenListMember *pTailMember;

    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));

    pTailMember = pList->tail;
    if (NULL != pTailMember)
    {
        CsrBtAmpmGenListMember *pPreviousMember = pTailMember->previous;

        if (NULL != pPreviousMember)
        {
            pPreviousMember->next = NULL;
            pList->tail = pPreviousMember;
        }
        else
        {
            /* tail is also the head */
            pList->head = NULL;
            pList->tail = NULL;
        }
        --pList->count;

        /* NULL the pointers in the member that we are returning */
        pTailMember->next = NULL;
        pTailMember->previous = NULL;
    }
    /* Still has to be a valid list */
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));
    return (pTailMember);
}

void CsrBtAmpmListRemoveMember(
    CstBtAmpmGenListAnchor * pList,
    CsrBtAmpmGenListMember * pRemoveMember)
{
    CsrBtAmpmGenListMember *pPreviousMember;
    CsrBtAmpmGenListMember *pFollowingMember;

    CSR_BT_AMPM_ASSERT(pList != NULL);
    CSR_BT_AMPM_ASSERT(pRemoveMember != NULL);
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));

    pPreviousMember = pRemoveMember->previous;
    pFollowingMember = pRemoveMember->next;

    if (NULL != pPreviousMember)
    {
        pPreviousMember->next = pFollowingMember;
    }
    else
    {
        /* Removing entry at head of list */
        pList->head = pFollowingMember;

    }
    if (NULL != pFollowingMember)
    {
        pFollowingMember->previous = pPreviousMember;
    }
    else
    {
        /* Removing entry at tail of list */
        pList->tail = pPreviousMember;
    }
    --pList->count;

    /* NULL the pointers in the member that we are returning */
    pRemoveMember->next = NULL;
    pRemoveMember->previous = NULL;

    /* Still has to be a valid list */
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));
}

CsrBtAmpmGenListMember *CsrBtAmpmListFindMatchingMember(
    const CstBtAmpmGenListAnchor * pList,
    CsrBool(*fpisRequiredMember) (const void *,
        const void *),
    const void *pMatchCriteria)
{
    CsrBtAmpmGenListMember *pMember;

    CSR_BT_AMPM_ASSERT(pList != NULL);
    CSR_BT_AMPM_ASSERT(fpisRequiredMember != NULL);
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));

    pMember = pList->head;
    while (NULL != pMember &&
        FALSE == (*fpisRequiredMember) (pMatchCriteria, pMember))
    {
        pMember = pMember->next;
    }

    /* Still has to be a valid list */
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));
    return (pMember);
}

CsrBtAmpmGenListMember *CsrBtAmpmListRemoveMatchingMember(
    CstBtAmpmGenListAnchor * pList,
    CsrBool(*fpisRequiredMember) (const void *,
        const void *),
    const void *pMatchCriteria)
{
    CsrBtAmpmGenListMember *pMember;

    CSR_BT_AMPM_ASSERT(pList != NULL);
    CSR_BT_AMPM_ASSERT(fpisRequiredMember != NULL);
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));

    pMember = pList->head;
    while (NULL != pMember &&
        FALSE == (*fpisRequiredMember) (pMatchCriteria, pMember))
    {
        pMember = pMember->next;
    }

    /* If we've found the require member then remove it from the list */
    if (NULL != pMember)
    {
        CsrBtAmpmGenListMember *pPreviousMember = pMember->previous;
        CsrBtAmpmGenListMember *pFollowingMember = pMember->next;

        if (NULL != pPreviousMember)
        {
            pPreviousMember->next = pFollowingMember;
        }
        else
        {
            /* Removing entry at head of list */
            pList->head = pFollowingMember;

        }
        if (NULL != pFollowingMember)
        {
            pFollowingMember->previous = pPreviousMember;
        }
        else
        {
            /* Removing entry at tail of list */
            pList->tail = pPreviousMember;
        }
        --pList->count;

        /* NULL the pointers in the member that we are returning */
        pMember->next = NULL;
        pMember->previous = NULL;
    }
    /* Still has to be a valid list */
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));

    return (pMember);
}

void CsrBtAmpmListAddTail(
    CstBtAmpmGenListAnchor * pList,
    CsrBtAmpmGenListMember * pNewMember)
{
    CsrBtAmpmGenListMember *pOldTail;

    CSR_BT_AMPM_ASSERT(pNewMember != NULL);
    CSR_BT_AMPM_ASSERT(pList != NULL);

    pOldTail = pList->tail;
    if (NULL != pOldTail)
    {
        pOldTail->next = pNewMember;
    }
    else
    {
        /* New tail is also the head */
        pList->head = pNewMember;
    }
    pNewMember->next = NULL;
    pNewMember->previous = pList->tail;

    /* Update the list anchor tail to point to the new final entry */
    pList->tail = pNewMember;
    ++pList->count;

    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));
}

void CsrBtAmpmListAddHead(
    CstBtAmpmGenListAnchor * pList,
    CsrBtAmpmGenListMember * pNewMember)
{
    CsrBtAmpmGenListMember *pOldHead;

    CSR_BT_AMPM_ASSERT(pNewMember != NULL);
    CSR_BT_AMPM_ASSERT(pList != NULL);

    pOldHead = pList->head;
    pNewMember->next = pOldHead;
    pNewMember->previous = NULL;
    if (NULL != pOldHead)
    {
        pOldHead->previous = pNewMember;
    }
    else
    {
        /* New head is also tail */
        pList->tail = pNewMember;
    }
    pList->head = pNewMember;

    ++pList->count;

    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_IS_VALID_LIST(pList));
}

int CsrBtAmpmListCount(
    const CstBtAmpmGenListAnchor * pList)
{
    CSR_BT_AMPM_ASSERT(pList != NULL);

#if !defined(__KERNEL__) && defined(_DEBUG)
    /* Check that the count actually matches the number of elements
     * in the list.
     */
    {
        CsrBtAmpmGenListMember *pListMember = pList->head;
        int NumOfListElements = 0;

        while (NULL != pListMember)
        {
            NumOfListElements++;
            pListMember = pListMember->next;
        }
        CSR_BT_AMPM_ASSERT(NumOfListElements == pList->count);
    }
#endif
    return (pList->count);
}


CsrBool CsrBtAmpmListEntryMatchesPtr(
    const void *pCriteria,
    const void *pListEntry)
{
    CsrBool doesEntryMatch = FALSE;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    if (pCriteria == pListEntry)
    {
        doesEntryMatch = TRUE;
    }
    return (doesEntryMatch);
}
