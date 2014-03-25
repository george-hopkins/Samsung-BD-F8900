/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_hq_subscriber.h"
#include "csr_pmem.h"

void CsrHqInsertSubscriber(CsrHqInstanceData *hqInst, CsrSchedQid appH, CsrHqVarIdType varId, CsrBool response)
{
    CsrHqSubscription *newSubscriber;

    newSubscriber = (CsrHqSubscription *) CsrCmnListElementAddLast(&(hqInst->subscribers), sizeof(CsrHqSubscription));
    newSubscriber->appHandle = appH;
    newSubscriber->varId = varId;
    newSubscriber->response = response;
}

static CsrBool searchSubscriberCheckFunctionSpecific(CsrCmnListElm_t *currentElem, void *xPrim)
{
    CsrHqDeregisterReq *prim = (CsrHqDeregisterReq *) xPrim;
    CsrHqSubscription *element = (CsrHqSubscription *) currentElem;

    if ((element->varId == prim->varId) && (element->appHandle == prim->phandle))
    {
        return TRUE;
    }

    return FALSE;
}

CsrResult CsrHqRemoveSubscriberSpecific(CsrHqInstanceData *hqInst, CsrHqDeregisterReq *prim)
{
    CsrCmnListElm_t *element;

    element = CsrCmnListSearch(&(hqInst->subscribers), searchSubscriberCheckFunctionSpecific, prim);
    if (element != NULL)
    {
        CsrCmnListElementRemove(&hqInst->subscribers, element);
        return CSR_RESULT_SUCCESS;
    }

    return CSR_HQ_RESULT_NO_SUCH_VARID;
}

static CsrBool searchSubscriberCheckFunctionApphandle(CsrCmnListElm_t *currentElem, void *xappHandle)
{
    CsrSchedQid *appHandle = (CsrSchedQid *) xappHandle;
    CsrHqSubscription *element = (CsrHqSubscription *) currentElem;

    if (element->appHandle == *appHandle)
    {
        return TRUE;
    }

    return FALSE;
}

void CsrHqRemoveSubscriber(CsrHqInstanceData *hqInst, CsrSchedQid appHandle)
{
    CsrCmnListElm_t *element;

    element = CsrCmnListSearch((CsrCmnList_t *) &(hqInst->subscribers), searchSubscriberCheckFunctionApphandle, &appHandle);
    while (element != NULL)
    {
        CsrCmnListElementRemove(&hqInst->subscribers, element);
        element = CsrCmnListSearch(&hqInst->subscribers, searchSubscriberCheckFunctionApphandle, &appHandle);
    }
}
