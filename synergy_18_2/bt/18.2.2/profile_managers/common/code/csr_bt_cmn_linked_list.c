/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_bt_util.h"

#include "csr_bt_cmn_linked_list.h"

/* **************************************************** */
/* **************************************************** */
/* **************************************************** */
/* **************************************************** */


/* Call a function for each element in the list */
/* Call a function for each element in the list - allow elements to be removed during iteration */


CmnCsrBtLinkedListStruct *CsrBtUtilBllCreateNewEntry(CmnCsrBtLinkedListStruct *list,
                                                     void *data_p,
                                                     uuid16_t dataLen)
{
    CmnCsrBtLinkedListStruct *cur;
    CmnCsrBtLinkedListStruct *new;

    new = CsrPmemZalloc(sizeof(CmnCsrBtLinkedListStruct));
    new->data = data_p;
    new->dataLen = dataLen;

    if(list == NULL)
    {
        return new;
    }
    else
    {
        cur = list;
        while(cur->nextEntry != NULL)
        {
            cur = cur->nextEntry;
        }
        cur->nextEntry = new;
        new->prevEntry = cur;
        return list;
    }
}

CsrUintFast16 CsrBtUtilBllGetNofEntriesEx(CmnCsrBtLinkedListStruct *bll_p)
{
    CsrUintFast16              nofEntriess = 0;
    CmnCsrBtLinkedListStruct *bllTemp_p  = bll_p;

    while (bllTemp_p)
    {
        nofEntriess++;
        bllTemp_p = bllTemp_p->nextEntry;
    }
    return nofEntriess;
}

CmnCsrBtLinkedListStruct *CsrBtUtilBllGetEntryPtrFromIndexEx(CmnCsrBtLinkedListStruct *bll_p,
                                                    CsrUintFast16                 index)
{
    CmnCsrBtLinkedListStruct *returnBll_p = bll_p;

    if (index < CsrBtUtilBllGetNofEntriesEx(bll_p))
    {
        CsrUintFast16 x;

        for (x=0; x<index; x++)
        {
            returnBll_p = returnBll_p->nextEntry;
        }
    }
    else
    {
        returnBll_p = NULL;
    }

    return returnBll_p;
}

#ifdef CSR_BT_INSTALL_CM_AFH
void CsrBtUtilBllForEachLinkedList(CmnCsrBtLinkedListStruct *bll_pp,
                              CmnBllForEachDataFuncType  cmnBllForEachHandler,
                              void *data_p)
{
    while (bll_pp)
    {
        if (bll_pp->data)
        {
            if (!cmnBllForEachHandler(bll_pp->data, data_p))
            {
                return;
            }
        }
        bll_pp = bll_pp->nextEntry;
    }
}
#endif
void CsrBtUtilBllFreeLinkedList(CmnCsrBtLinkedListStruct **bll_pp,
                              CmnBllFreeDataFunctype  cmnBllFreeDataHandler)
{
    CmnCsrBtLinkedListStruct *bllTemp_p = *bll_pp;

    while (bllTemp_p)
    {
        if (bllTemp_p->data)
        {
            cmnBllFreeDataHandler(bllTemp_p->data);
        }
        if (bllTemp_p->nextEntry == NULL)
        {
            CsrPmemFree(bllTemp_p);
            bllTemp_p = NULL;
        }
        else
        {
            bllTemp_p = bllTemp_p->nextEntry;
            CsrPmemFree (bllTemp_p->prevEntry);
        }
    }
    *bll_pp = NULL;
}

CmnCsrBtLinkedListStruct *CsrBtUtilBllFreeLinkedListEntryEx(CmnCsrBtLinkedListStruct *bll_p,
                                                       CsrUintFast16                 index,
                                                       CmnBllFreeDataFunctype  cmnBllFreeDataHandler)
{
    CmnCsrBtLinkedListStruct *bllTemp_p = NULL;
    CmnCsrBtLinkedListStruct *retBll_p = bll_p;

    if (index < CsrBtUtilBllGetNofEntriesEx(bll_p))
    {
        bllTemp_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, index);
    }

    if (bllTemp_p)
    {
        CsrPmemFree(bllTemp_p->data);

        if (0 == index)
        {
            retBll_p = bll_p->nextEntry;
        }
        if (bllTemp_p->prevEntry)
        {
            (bllTemp_p->prevEntry)->nextEntry = bllTemp_p->nextEntry;
        }
        if (bllTemp_p->nextEntry)
        {
            (bllTemp_p->nextEntry)->prevEntry = bllTemp_p->prevEntry;
        }

        CsrPmemFree(bllTemp_p);
    }
    return retBll_p;
}

CmnCsrBtLinkedListStruct *CsrBtUtilBllFreeLinkedListEntryByDataPointer(CmnCsrBtLinkedListStruct *bll_p,
                                                                              void *data_p,
                                                                              CmnBllFreeDataFunctype  cmnBllFreeDataHandler)
{
    CmnCsrBtLinkedListStruct *bllTemp_p = NULL;
    CsrUintFast16 noEntries = CsrBtUtilBllGetNofEntriesEx(bll_p);
    CsrUintFast16 index;

    for (index=0; index < noEntries; ++index)
    {
        bllTemp_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, index);

        if (bllTemp_p && bllTemp_p->data == (CsrUint8 *) data_p)
        {
            bll_p = CsrBtUtilBllFreeLinkedListEntryEx(bll_p, index, cmnBllFreeDataHandler);
            --index;
            --noEntries;
        }
    }

    return bll_p;
}

#if 0 /* UNUSED*/
void CsrBtUtilBllFreeLinkedListDataEx(CmnCsrBtLinkedListStruct *bll_p,
                                  CsrUintFast16                 index)
{


    if (index < CsrBtUtilBllGetNofEntriesEx(bll_p))
    {
        CmnCsrBtLinkedListStruct *bllTemp_p = NULL;

        bllTemp_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, index);

        if (bllTemp_p)
        {
            CsrPmemFree(bllTemp_p->data);
            bllTemp_p->data = NULL;
        }
    }
}
#endif

/* **************************************************** */
/* **************************************************** */
/* **************************************************** */
/* **************************************************** */

void *CsrBtUtilBllGetDataPointerEx(CmnCsrBtLinkedListStruct *bll_p,
                              CsrUintFast16                 serviceHandleIndex,
                              CsrUint16                     *dataLen_p)
{
    CmnCsrBtLinkedListStruct *bllTemp_p = NULL;
    CsrUint8  *return_p;

    if (bll_p)
    {
        if (serviceHandleIndex < CsrBtUtilBllGetNofEntriesEx(bll_p))
        {
            bllTemp_p = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);
        }
    }

    if (bllTemp_p)
    {
        *dataLen_p = bllTemp_p->dataLen;
        return_p   = bllTemp_p->data;
    }
    else
    {
        dataLen_p = 0;
        return_p = NULL;
    }
    return return_p;
}

CsrBool CsrBtUtilBllSetDataPointerEx(CmnCsrBtLinkedListStruct *bll_p,
                            CsrUintFast16                 serviceHandleIndex,
                            void                    *data_p,
                            CsrUint16                 dataLen)
{
    CsrBool retBool = FALSE;

    if (bll_p)
    {
        if (serviceHandleIndex < CsrBtUtilBllGetNofEntriesEx(bll_p))
        {
            CmnCsrBtLinkedListStruct *bllTemp_p = NULL;
            bllTemp_p          = CsrBtUtilBllGetEntryPtrFromIndexEx(bll_p, serviceHandleIndex);

            if (bllTemp_p)
            {
                bllTemp_p->dataLen = dataLen;
                bllTemp_p->data    = data_p;
                retBool            = TRUE;
            }
        }
    }

    return retBool;
}

void * CsrBtUtilBllFindDataPointer(CmnCsrBtLinkedListStruct         *bll_p,
                                 void                            *buf1_p,
                                 CmnBllCompareDataFuncType       cmnBllCompareDataHandler)
{
    if (buf1_p)
    {
        CmnCsrBtLinkedListStruct *bllTemp_p = bll_p;

        while (bllTemp_p)
        {
            if (bllTemp_p->data)
            {
                if (cmnBllCompareDataHandler(buf1_p, bllTemp_p->data) == 0)
                {
                    return (bllTemp_p->data);
                }
            }
            bllTemp_p = bllTemp_p->nextEntry;
        }
    }
    return NULL;
}

CsrInt16 CsrBtUtilBllCompareDataUint16FuncType(void * buf1, void * buf2)
{
    CsrUint16 *pUnit16Buf1 = (CsrUint16 *) buf1;
    CsrUint16 *pUnit16Buf2 = (CsrUint16 *) buf2;

    if (*pUnit16Buf1 == *pUnit16Buf2)
    {
        return 0;
    }
    else if (*pUnit16Buf1 < *pUnit16Buf2)
    {
        return -1;
    }
    else /* if (*pUnit16Buf1 > *pUnit16Buf2) */
    {
        return 1;
    }
}

CsrCmnListElm_t *CsrCmnListSearchOffsetAddr(CsrCmnList_t *cmnList, size_t offset, CsrBtDeviceAddr *addr)
{
    CsrCmnListElm_t *currentElem;

    for (currentElem = cmnList->first; currentElem; currentElem = currentElem->next)
    {
        if (CSR_BT_BD_ADDR_EQ(*(CsrBtDeviceAddr *)(((CsrInt8 *)currentElem) + offset), *addr))
        {
            return currentElem;
        }
    }

    return NULL;
}


void CsrBtUtilBllPfreeWrapper(void * ptr)
{
    CsrPmemFree(ptr);
}


