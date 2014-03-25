/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_bpps_handler.h"
#include "csr_bt_bpps_sef.h"
#include "csr_bt_sd_private_lib.h"

static const BppsStateHandlerType bppsMsgHandlers[CSR_BT_BPPS_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtBppsActivateReqHandler,            /* CSR_BT_BPPS_ACTIVATE_REQ             */
    CsrBtBppsDeactivateReqHandler,          /* CSR_BT_BPPS_DEACTIVATE_REQ           */
    CsrBtBppsConnectResHandler,             /* CSR_BT_BPPS_CONNECT_RES              */
    CsrBtBppsAuthenticateReqHandler,        /* CSR_BT_BPPS_AUTHENTICATE_REQ         */
    CsrBtBppsAuthenticateResHandler,        /* CSR_BT_BPPS_AUTHENTICATE_RES         */
    CsrBtBppsNextResHandler,                /* CSR_BT_BPPS_NEXT_RES                 */
    CsrBtBppsCreateJobResHandler,           /* CSR_BT_BPPS_CREATE_JOB_RES           */
    CsrBtBppsGetPrinterAttribsResHandler,   /* CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES  */
    CsrBtBppsGetJobAttribsResHandler,       /* CSR_BT_BPPS_GET_JOB_ATTRIBS_RES      */
    CsrBtBppsCancelJobResHandler,           /* CSR_BT_BPPS_CANCEL_JOB_RES           */
    CsrBtBppsGetReferencedObjReqHandler,    /* CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ   */
    CsrBtBppsGetReferencedObjResHandler,    /* CSR_BT_BPPS_GET_REFERENCED_OBJ_RES   */
    CsrBtBppsGetEventResHandler,            /* CSR_BT_BPPS_GET_EVENT_RES            */
    CsrBtBppsJobCompleteReqHandler,          /* CSR_BT_BPPS_JOB_COMPLETE_REQ         */
    CsrBtBppsSecurityInReqHandler,          /* CSR_BT_BPPS_SECURITY_IN_REQ          */
};

void CsrBtBppsInit(void **gash)
{
    BppsInstanceData *pInst;

    *gash =  CsrPmemZalloc(sizeof(BppsInstanceData));
    pInst = *gash;

    pInst->obexJobChannelInst            = ObexUtilInit(CSR_BT_BPPS_IFACEQUEUE, pInst, CSR_BT_BPPS_JOB_CHANNEL_INST_ID);
    pInst->obexStatusChannelInst         = ObexUtilInit(CSR_BT_BPPS_IFACEQUEUE, pInst, CSR_BT_BPPS_STATUS_CHANNEL_INST_ID);
    pInst->obexObjectChannelInst         = ObexUtilInit(CSR_BT_BPPS_IFACEQUEUE, pInst, CSR_BT_BPPS_OBJECT_CHANNEL_INST_ID);
    pInst->appHandle                     = CSR_SCHED_QID_INVALID;
    pInst->jobChLocalServerCh            = CSR_BT_NO_SERVER;
    pInst->statusChLocalServerCh         = CSR_BT_NO_SERVER;
    pInst->jobChReasonCode               = CSR_BT_RESULT_CODE_OBEX_TRANSPORT_CONNECTION_CLOSED_DIRECT;
    pInst->jobChReasonSupplier           = CSR_BT_SUPPLIER_OBEX_PROFILES;

    /* Default security levels */
    CsrBtScSetSecInLevel(&pInst->secIncoming,
                         CSR_BT_SEC_DEFAULT,
                         CSR_BT_BPPS_MANDATORY_SECURITY_INCOMING,
                         CSR_BT_BPPS_DEFAULT_SECURITY_INCOMING,
                         CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                         CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    /* Tell the SD that it must look for the CSR_BT_DIRECT_PRINTING_REFERENCE_OBJ_UUID
       service, when it perform a SD_READ_AVAILABLE_SERVICE_REQ                 */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_DIRECT_PRINTING_REFERENCE_OBJ_UUID);
}

void CsrBtBppsHandler(void **gash)
{
    CsrUint16        eventClass;
    CsrPrim          *type;
    BppsInstanceData *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &pInst->recvMsgP);
    type = (CsrPrim *) pInst->recvMsgP;

    switch(eventClass)
    {
        case CSR_BT_BPPS_PRIM:
        {
            if ((*type < CSR_BT_BPPS_PRIM_DOWNSTREAM_COUNT) &&
                (bppsMsgHandlers[*type](pInst) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }
            else
            { /* State/Event ERROR!                                             */
                CsrBtBppsStateEventException(eventClass, *type);
                CsrBtBppsFreeDownstreamMessageContents(CSR_BT_BPPS_PRIM, pInst->recvMsgP);
            }
            break;
        }
        case CSR_BT_CM_PRIM:
        {
            CsrUint8 id;
            CsrUint8 returnValue = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;

            if (ObexUtilGetInstIdentifierFromCmUpstreamMessage(pInst->recvMsgP, &id))
            {
                if (id == CSR_BT_BPPS_JOB_CHANNEL_INST_ID)
                {
                    returnValue = ObexUtilCmMsgHandler(pInst->obexJobChannelInst, &pInst->recvMsgP);
                }
                else if (id == CSR_BT_BPPS_OBJECT_CHANNEL_INST_ID)
                {
                    returnValue = ObexUtilCmMsgHandler(pInst->obexObjectChannelInst, &pInst->recvMsgP);
                }
                else if (id == CSR_BT_BPPS_STATUS_CHANNEL_INST_ID)
                {
                    returnValue = ObexUtilCmMsgHandler(pInst->obexStatusChannelInst, &pInst->recvMsgP);
                }
                else
                {
                    returnValue = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
                }
            }
            else
            {
                returnValue = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
            }

            if (returnValue == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
            { /* State/Event ERROR!                                             */
                CsrBtBppsStateEventException(eventClass, *type);
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInst->recvMsgP);
            }
            break;
        }
        case CSR_SCHED_PRIM:
        {
            break;
        }
        default:
        { /* Unknown event type!?!?                                             */
            CsrBtBppsStateEventException(eventClass, *type);
            break;
        }
    }
    CsrPmemFree(pInst->recvMsgP);
}

#ifdef ENABLE_SHUTDOWN
/****************************************************************************
    This function is called by the scheduler to perform a graceful shutdown
    of a scheduler task.
    This function must:
    1)    empty the input message queue and free any allocated memory in the
        messages.
    2)    free any instance data that may be allocated.
****************************************************************************/
void CsrBtBppsDeinit(void** gash)
{
    CsrUint16        eventClass;
    void             *msg=NULL;
    CsrBool          lastMsg = FALSE;
    BppsInstanceData *pInst  = *gash;

    /* continue to poll any message of the input queue                          */
    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if(!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_BPPS_PRIM:
                {
                    CsrBtBppsFreeDownstreamMessageContents(eventClass, msg);
                    break;
                }

                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                    break;

                }
                default:
                {   /* Unknown event type!?!? */
                    CsrBtBppsStateEventException(eventClass, 0);
                    break;
                }
            }
            CsrPmemFree (msg);
        }
    }

    if (pInst->savedJobChMsg)
    {
        CsrBtBppsFreeUpstreamMessageContents(CSR_BT_BPPS_PRIM, pInst->savedJobChMsg);
        CsrPmemFree(pInst->savedJobChMsg);
    }

    if (pInst->savedObjectChDownStreamMsg)
    {
        CsrBtBppsFreeDownstreamMessageContents(CSR_BT_BPPS_PRIM, pInst->savedObjectChDownStreamMsg);
        CsrPmemFree(pInst->savedObjectChDownStreamMsg);
    }

    if (pInst->savedObjectChUpStreamMsg)
    {
        CsrBtBppsFreeUpstreamMessageContents(CSR_BT_BPPS_PRIM, pInst->savedObjectChUpStreamMsg);
        CsrPmemFree(pInst->savedObjectChUpStreamMsg);
    }

    CsrPmemFree(pInst->serviceRecord);
    CsrPmemFree(pInst->savedStatusChSoapAction);
    CsrPmemFree(pInst->savedJobChSoapAction);
    ObexUtilDeinit(&pInst->obexJobChannelInst);
    ObexUtilDeinit(&pInst->obexStatusChannelInst);
    ObexUtilDeinit(&pInst->obexObjectChannelInst);
    CsrPmemFree(pInst);
}
#endif /* ENABLE_SHUTDOWN */

