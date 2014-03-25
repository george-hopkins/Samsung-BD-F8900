/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_list.h"

void CsrCmnListInit(CsrCmnList_t *cmnList, CsrUint32 listId, CsrCmnListAddFunc_t addFunc, CsrCmnListRemoveFunc_t removeFunc)
{
    cmnList->first = NULL;
    cmnList->last = NULL;
    cmnList->count = 0;
    cmnList->listId = listId;
    cmnList->addFunc = addFunc;
    cmnList->removeFunc = removeFunc;
}

void CsrCmnListDeinit(CsrCmnList_t *cmnList) /* Free the entire list */
{
    CsrCmnListElm_t *element, *nextElement;

    for (element = cmnList->first; element; element = nextElement)
    {
        nextElement = element->next;
        CsrCmnListElementRemove(cmnList, element);
    }

    cmnList->listId = 0;
    cmnList->first = NULL;
    cmnList->last = NULL;
    cmnList->count = 0;
}

CsrCmnListElm_t *CsrCmnListElementGetFirst(CsrCmnList_t *cmnList)
{
    if (!cmnList)
    {
        return NULL;
    }
    else
    {
        return cmnList->first;
    }
}

CsrCmnListElm_t *CsrCmnListElementGetLast(CsrCmnList_t *cmnList)
{
    if (!cmnList)
    {
        return NULL;
    }
    else
    {
        return cmnList->last;
    }
}

CsrCmnListElm_t *CsrCmnListElementAddFirst(CsrCmnList_t *cmnList, CsrSize size) /* Add an element in the beginning of the list */
{
    CsrCmnListElm_t *element = CsrPmemAlloc(size);

    element->next = cmnList->first;
    element->prev = NULL;

    if (cmnList->first) /* This is not the first element in the list */
    {
        cmnList->first->prev = element;
    }
    else /* This is the first element in the list */
    {
        cmnList->last = element;
    }

    cmnList->first = element;
    cmnList->count++;

    if (cmnList->addFunc)
    {
        cmnList->addFunc(element);
    }

    return element;
}

CsrCmnListElm_t *CsrCmnListElementAddLast(CsrCmnList_t *cmnList, CsrSize size) /* Add an element in the end of the list */
{
    CsrCmnListElm_t *element = CsrPmemAlloc(size);

    element->next = NULL;
    element->prev = cmnList->last;

    if (cmnList->last) /* An element is already present in the list */
    {
        cmnList->last->next = element;
    }
    else /* This is the first element in the list */
    {
        cmnList->first = element;
    }

    cmnList->last = element;
    cmnList->count++;

    if (cmnList->addFunc)
    {
        cmnList->addFunc(element);
    }

    return element;
}

void CsrCmnListElementRemove(CsrCmnList_t *cmnList, CsrCmnListElm_t *element)
{
    if (element->prev) /* This is not the first element in the list */
    {
        element->prev->next = element->next;
    }
    else /* This is the first element in the list */
    {
        cmnList->first = element->next;
    }

    if (element->next) /* This is not the last element in the list */
    {
        element->next->prev = element->prev;
    }
    else /* This is the last element in the list */
    {
        cmnList->last = element->prev;
    }

    cmnList->count--;

    if (cmnList->removeFunc) /* Call associated function on the element to remove */
    {
        cmnList->removeFunc(element);
    }

    CsrPmemFree(element);
}

CsrCmnListElm_t *CsrCmnListGetFromIndex(CsrCmnList_t *cmnList, CsrUint32 index) /* Get an element from the 'index' of an element (starting from the beginning of the list) */
{
    CsrCmnListElm_t *elem = NULL;

    for (elem = cmnList->first; elem; elem = elem->next, index--) /* Skip to the correct element */
    {
        if (index == 0)
        {
            break;
        }
    }

    return elem;
}

void CsrCmnListIterate(CsrCmnList_t *cmnList, CsrCmnListIterateFunc_t iterateFunc, void *data)
{
    CsrCmnListElm_t *elem;

    for (elem = cmnList->first; elem; elem = elem->next)
    {
        iterateFunc(elem, data);
    }
}

void CsrCmnListIterateAllowRemove(CsrCmnList_t *cmnList, CsrCmnListIterateAllowRemoveFunc_t iterateRemoveFunc, void *data)
{
    CsrCmnListElm_t *elem = cmnList->first;
    CsrCmnListElm_t *nextElem;

    while (elem)
    {
        if (iterateRemoveFunc(elem, data)) /* Element should be removed - save link to next element */
        {
            nextElem = elem->next;
            CsrCmnListElementRemove(cmnList, elem);
            elem = nextElem;
        }
        else /* Element should not be removed - existing element is valid */
        {
            elem = elem->next;
        }
    }
}

CsrCmnListElm_t *CsrCmnListSearch(CsrCmnList_t *cmnList, CsrCmnListSearchFunc_t searchFunc, void *data)
{
    CsrCmnListElm_t *currentElem;

    for (currentElem = cmnList->first; currentElem; currentElem = currentElem->next)
    {
        if (searchFunc(currentElem, data) == TRUE)
        {
            break;
        }
    }

    return currentElem;
}

static void csrCmnListMove(CsrCmnList_t *cmnList, CsrCmnListElm_t *dst, CsrCmnListElm_t *src) /* Will insert src before dst */
{   /* Update old links around src */
    if (src->prev)
    {
        src->prev->next = src->next;
    }
    else /* The entry to move is the first in the list */
    {
        cmnList->first = src->next;
    }

    if (src->next)
    {
        src->next->prev = src->prev;
    }
    else /* The entry to move is the last in the list */
    {
        cmnList->last = src->prev;
    }

    /* Update new links around src */
    src->next = dst;
    src->prev = dst->prev;

    /* Update links around dst */
    if (dst->prev)
    {
        dst->prev->next = src;
    }
    else /* dst is the first in the list */
    {
        cmnList->first = src;
    }
    dst->prev = src;
}

void CsrCmnListSort(CsrCmnList_t *cmnList, CsrCmnListSortFunc_t sortFunc) /* Using insertion sort */
{
    CsrCmnListElm_t *currentElem = cmnList->first, *nextElem, *tmpElem;

    while (currentElem && currentElem->next)
    {
        nextElem = currentElem->next;

        if (sortFunc(currentElem, nextElem) > 0) /* nextElem should be moved - (currentElem > nextElem) */
        {
            currentElem = currentElem->next;
            /* Find the correct place to insert the element */
            for (tmpElem = cmnList->first; tmpElem; tmpElem = tmpElem->next)
            {
                if (sortFunc(nextElem, tmpElem) <= 0) /* Insert element before tmpElem - (nextElem <= tmpElem) */
                {
                    csrCmnListMove(cmnList, tmpElem, nextElem);
                    break;
                }
            }
        }
        else
        {
            currentElem = currentElem->next;
        }
    }
}

CsrCmnListElm_t *CsrCmnListSearchOffsetUint8(CsrCmnList_t *cmnList, CsrSize offset, CsrUint8 value)
{
    CsrCmnListElm_t *currentElem;

    for (currentElem = cmnList->first; currentElem; currentElem = currentElem->next)
    {
        if (*(CsrUint8 *) (((CsrInt8 *) currentElem) + offset) == value)
        {
            return currentElem;
        }
    }

    return NULL;
}

CsrCmnListElm_t *CsrCmnListSearchOffsetUint16(CsrCmnList_t *cmnList, CsrSize offset, CsrUint16 value)
{
    CsrCmnListElm_t *currentElem;

    for (currentElem = cmnList->first; currentElem; currentElem = currentElem->next)
    {
        if (*(CsrUint16 *) (((CsrInt8 *) currentElem) + offset) == value)
        {
            return currentElem;
        }
    }

    return NULL;
}

CsrCmnListElm_t *CsrCmnListSearchOffsetUint32(CsrCmnList_t *cmnList, CsrSize offset, CsrUint32 value)
{
    CsrCmnListElm_t *currentElem;

    for (currentElem = cmnList->first; currentElem; currentElem = currentElem->next)
    {
        if (*(CsrUint32 *) (((CsrInt8 *) currentElem) + offset) == value)
        {
            return currentElem;
        }
    }

    return NULL;
}
