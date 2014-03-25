/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ppp_sef.h"
#include "csr_bt_ppp_hdlc.h"
#include "csr_bt_lsl_prim.h"
#include "csr_bt_prim_to_lsl_converter.h"
#include "csr_bt_dunc_prim.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

void CsrBtPppInit(void ** gash)
{
    PppInstanceData * instanceData;

    instanceData = (PppInstanceData *) CsrPmemAlloc(sizeof(PppInstanceData));
    *gash = (void *) instanceData;

    instanceData->pppState = PPP_STATE_INITIAL;
    instanceData->transmissionAllowed = TRUE;
    instanceData->pppDataInd = NULL;
    instanceData->hdlcAndPppHeader[0] = 0xFF;
    instanceData->hdlcAndPppHeader[1] = 0x03;
    instanceData->pppDataReq = NULL;
    instanceData->queueHead = instanceData->queueTail = 0;
    instanceData->numberOnQueue = 0;
    instanceData->dataReqQueue = NULL;
    instanceData->lcpTimer = 0;
    instanceData->loginName = NULL;
    instanceData->password = NULL;
    instanceData->papTimer = 0;
    instanceData->papIdentifier = 0;
    instanceData->registeredNetworkProtocolList = NULL;
}

#ifdef ENABLE_SHUTDOWN
void CsrBtPppDeinit(void ** gash)
{
    CsrUint16 event;
    CsrBtPppDataReq * pppDataReq;
    PppInstanceData * instanceData;

    instanceData = (PppInstanceData *) *gash;

    instanceData->pppState = PPP_STATE_INITIAL;
    instanceData->applicationQueue = CSR_BT_PPP_IFACEQUEUE;
    if (instanceData->lcpTimer)
    {
        CsrSchedTimerCancel(instanceData->lcpTimer, NULL, NULL);
        instanceData->lcpTimer = 0;
    }
    while (CsrMessageQueuePop(&instanceData->dataReqQueue, &event, (void **) &pppDataReq))
    {
        CsrPmemFree(pppDataReq->payload);
        CsrPmemFree(pppDataReq);
    }
    while (instanceData->numberOnQueue--)
    {
        CsrPmemFree(instanceData->pppDataReqQueue[instanceData->queueTail]->payload);
        CsrPmemFree(instanceData->pppDataReqQueue[instanceData->queueTail++]);
        if (instanceData->queueTail >= CSR_BT_PPP_DATA_REQ_QUEUE_SIZE)
        {
            instanceData->queueTail = 0;
        }
    }
    if (instanceData->pppDataInd)
    {
        CsrPmemFree(instanceData->pppDataInd->payload);
        CsrPmemFree(instanceData->pppDataInd);
        instanceData->pppDataInd = NULL;
    }
    if (instanceData->pppDataReq)
    {
        CsrPmemFree(instanceData->pppDataReq->payload);
        CsrPmemFree(instanceData->pppDataReq);
        instanceData->pppDataReq = NULL;
    }
    if (instanceData->loginName)
    {
        CsrPmemFree(instanceData->loginName);
        instanceData->loginName = NULL;
    }
    if (instanceData->password)
    {
        CsrPmemFree(instanceData->password);
        instanceData->password = NULL;
    }
    if (instanceData->papTimer)
    {
        CsrSchedTimerCancel(instanceData->papTimer, NULL, NULL);
        instanceData->papTimer = 0;
    }
    while (instanceData->registeredNetworkProtocolList)
    {
        PppRegisteredNetworkProtocol * tmp;

        tmp = instanceData->registeredNetworkProtocolList;
        instanceData->registeredNetworkProtocolList = instanceData->registeredNetworkProtocolList->next;
        CsrPmemFree(tmp);
    }
    CsrPmemFree(instanceData);
    *gash = NULL;
}
#endif

void CsrBtPppHandler(void ** gash)
{
    PppInstanceData * instanceData;

    instanceData = (PppInstanceData *) *gash;

    if (CsrSchedMessageGet(&instanceData->event, &instanceData->receivedMessage))
    {
        switch (instanceData->event)
        {
            case CSR_BT_PPP_PRIM:
                {
                    HandlePppMessage(instanceData);
                    break;
                }
                /* here some additions must be made every time a new profile is added - unfortunately */
            case CSR_BT_DUNC_PRIM:
                {
                    instanceData->receivedMessage = ConvertPrimToLsl(instanceData->receivedMessage,
                                                                     instanceData->event,
                                                                     instanceData->pppState);
                    HandleLslMessage(instanceData);
                    break;
                }

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            default:
                CsrStateEventException("PPP",
                                        instanceData->event,
                                        0,
                                        instanceData->pppState);
                break;
#endif
        }

        CsrPmemFree(instanceData->receivedMessage);
    }
}

