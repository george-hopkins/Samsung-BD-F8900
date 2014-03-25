/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_hci_sco.h"
#include "csr_hci_util.h"

/* array for the ScoFunctionCalls */
static CsrHciScoHandleStructure *ScoFunctionArray = NULL;

/***********************************************************************
CsrHciInitScoHandle
***********************************************************************/
void CsrHciInitScoHandle(void)
{
    CsrHciScoHandleStructure *prev, *next = ScoFunctionArray;

    while (next)
    {
        prev = next;
        next = next->next;
        CsrPmemFree(prev);
    }
    ScoFunctionArray = NULL;
}

/***********************************************************************
registerScoHandle
***********************************************************************/
CsrBool CsrHciRegisterScoHandle(CsrUint16 theScoHandle, CsrHciScoHandlerFuncType theFunctionPtr)
{
    CsrHciScoHandleStructure *next = CsrPmemAlloc(sizeof(CsrHciScoHandleStructure));

    next->next = NULL;
    next->scoHandle = theScoHandle;
    next->scoHandlerFunc = theFunctionPtr;

    if (ScoFunctionArray)
    {
        CsrHciScoHandleStructure *prev;

        prev = ScoFunctionArray;

        while (prev->next)
        {
            prev = prev->next;
        }
        prev->next = next;
    }
    else
    {
        ScoFunctionArray = next;
    }

    return TRUE;
}

/***********************************************************************
CsrHciDeRegisterScoHandle
***********************************************************************/
void CsrHciDeRegisterScoHandle(CsrUint16 theScoHandle)
{
    CsrHciScoHandleStructure *prev, *next;

    prev = NULL;
    next = ScoFunctionArray;

    while (next)
    {
        if (next->scoHandle == theScoHandle)
        {
            if (prev == NULL)
            {
                ScoFunctionArray = next->next;
            }
            else
            {
                prev->next = next->next;
            }
            CsrPmemFree(next);
            break;
        }
        prev = next;
        next = next->next;
    }
}

/***********************************************************************
 *CsrHciLookForScoHandle
 * Copy SCO data before sending to the application.
 *
 ***********************************************************************/
CsrBool CsrHciLookForScoHandle(const CsrUint8 *theBuf)
{
    CsrHciScoHandleStructure *next = ScoFunctionArray;
    CsrUint16 theScoHandle;

    theScoHandle = CSR_HCI_GET_XAP_UINT16(theBuf) & 0x0fff;

    while (next)
    {
        if (next->scoHandle == theScoHandle)
        {
            CsrUint8 *data;
            data = CsrPmemAlloc(theBuf[2] + 3);
            CsrMemCpy(data, theBuf, theBuf[2] + 3);
            next->scoHandlerFunc(data);
            return TRUE;
        }
        next = next->next;
    }
    return FALSE;
}
