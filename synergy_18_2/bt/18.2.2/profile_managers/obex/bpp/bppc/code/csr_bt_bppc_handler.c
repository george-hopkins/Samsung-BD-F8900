/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_bppc_handler.h"
#include "csr_bt_bppc_sef.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"

#define CSR_BT_BPPC_JOB_CHANNEL_INST_ID         0x01
#define CSR_BT_BPPC_STATUS_CHANNEL_INST_ID      0x02
#define CSR_BT_BPPC_OBJECT_CHANNEL_INST_ID      0x03

static const BppcStateHandlerType bppcMsgHandlers[CSR_BT_BPPC_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtBppcActivateReqHandler,            /* CSR_BT_BPPC_ACTIVATE_REQ                 */
    CsrBtBppcDeactivateReqHandler,          /* CSR_BT_BPPC_DEACTIVATE_REQ               */
    CsrBtBppcConnectResHandler,             /* CSR_BT_BPPC_CONNECT_RES                  */
    CsrBtBppcConnectReqHandler,             /* CSR_BT_BPPC_CONNECT_REQ                  */
    CsrBtBppcAbortReqHandler,               /* CSR_BT_BPPC_ABORT_REQ                    */
    CsrBtBppcDisconnectReqHandler,          /* CSR_BT_BPPC_DISCONNECT_REQ               */
    CsrBtBppcAuthenticateResHandler,        /* CSR_BT_BPPC_AUTHENTICATE_RES             */
    CsrBtBppcGetPrintAttrReqHandler,        /* CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ   */
    CsrBtBppcGetPrintAttrResHandler,        /* CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES   */
    CsrBtBppcCreateJobReqHandler,           /* CSR_BT_BPPC_CREATE_JOB_REQ               */
    CsrBtBppcDocumentReqHandler,            /* CSR_BT_BPPC_SEND_DOCUMENT_REQ            */
    CsrBtBppcDocumentResHandler,            /* CSR_BT_BPPC_SEND_DOCUMENT_RES            */
    CsrBtBppcGetReferenceObjectResHandler,  /* CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES     */
    CsrBtBppcGetJobAttrReqHandler,          /* CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ       */
    CsrBtBppcGetJobAttrResHandler,          /* CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES       */
    CsrBtBppcCancelJobReqHandler,           /* CSR_BT_BPPC_CANCEL_JOB_REQ               */
    CsrBtBppcGetEventReqHandler,            /* CSR_BT_BPPC_GET_EVENT_REQ                */
    CsrBtBppcGetEventResHandler,            /* CSR_BT_BPPC_GET_EVENT_RES                */
    CsrBtBppcCancelConnectReqHandler,       /* CSR_BT_BPPC_CANCEL_CONNECT_REQ           */
    CsrBtBppcSecurityOutReqHandler,         /* CSR_BT_BPPC_SECURITY_OUT_REQ             */
};

void CsrBtBppcInit(void **gash)
{
    BppcInstanceData *pInst;

    *gash = CsrPmemZalloc(sizeof(BppcInstanceData));

    pInst = (BppcInstanceData *) *gash;

    pInst->obexJobChannelInst           = ObexUtilInit(CSR_BT_BPPC_IFACEQUEUE, pInst, CSR_BT_BPPC_JOB_CHANNEL_INST_ID);
    pInst->obexStatusChannelInst        = ObexUtilInit(CSR_BT_BPPC_IFACEQUEUE, pInst, CSR_BT_BPPC_STATUS_CHANNEL_INST_ID);
    pInst->obexObjectChannelInst        = ObexUtilInit(CSR_BT_BPPC_IFACEQUEUE, pInst, CSR_BT_BPPC_OBJECT_CHANNEL_INST_ID);
    pInst->appHandle                    = CSR_SCHED_QID_INVALID;
    pInst->objectChLocalServerCh        = CSR_BT_NO_SERVER;

    CsrBtScSetSecOutLevel(&pInst->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_OBEX_PRINTING_INITIATOR_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_OBEX_PRINTING_INITIATOR_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    /* Tell the SD that it must look for the CSR_BT_DIRECT_PRINTING_UUID and
       the CSR_BT_PRINTING_STATUS_UUID service, when it perform a
       SD_READ_AVAILABLE_SERVICE_REQ                                    */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_DIRECT_PRINTING_UUID);
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_PRINTING_STATUS_UUID);
}

void CsrBtBppcHandler(void **gash)
{
    CsrUint16        eventClass=0;
    CsrPrim          *type;
    void             *msg   = NULL;
    BppcInstanceData *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &msg);
    type = (CsrPrim *) msg;

    switch(eventClass)
    {
        case CSR_BT_BPPC_PRIM:
        {
            if ((*type < CSR_BT_BPPC_PRIM_DOWNSTREAM_COUNT) &&
                (bppcMsgHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }
            else
            { /* State/Event ERROR!                                             */
                CsrBtBppcStateEventException(eventClass, *type);
                CsrBtBppcFreeDownstreamMessageContents(CSR_BT_SMLS_PRIM, msg);
            }
            break;
        }
        case CSR_BT_CM_PRIM:
        {
            CsrUint8 id;
            CsrUint8 returnValue = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;

            if (ObexUtilGetInstIdentifierFromCmUpstreamMessage(msg, &id))
            {
                if (id == CSR_BT_BPPC_JOB_CHANNEL_INST_ID)
                {
                    returnValue = ObexUtilCmMsgHandler(pInst->obexJobChannelInst, &msg);
                }
                else if (id == CSR_BT_BPPC_OBJECT_CHANNEL_INST_ID)
                {
                    returnValue = ObexUtilCmMsgHandler(pInst->obexObjectChannelInst, &msg);
                }
                else if (id == CSR_BT_BPPC_STATUS_CHANNEL_INST_ID)
                {
                    returnValue = ObexUtilCmMsgHandler(pInst->obexStatusChannelInst, &msg);
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
                CsrBtBppcStateEventException(eventClass, *type);
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
            }
            break;
        }
        case CSR_SCHED_PRIM:
        {
            break;
        }
        default:
        { /* Unknown event type!?!?                                             */
            CsrBtBppcStateEventException(eventClass, *type);
            break;
        }

    }
    CsrPmemFree(msg);
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
void CsrBtBppcDeinit(void **gash)
{
    CsrUint16        eventClass;
    void             *msg;
    CsrBool          lastMsg = FALSE;
    BppcInstanceData *pInst = *gash;

    /* continue to poll any message of the input queue                          */
    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if(!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_BPPC_PRIM:
                {
                    CsrBtBppcFreeDownstreamMessageContents(eventClass, msg);
                    break;
                }

                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                    break;

                }
                default:
                {   /* Unknown event type!?!? */
                    CsrBtBppcStateEventException(eventClass, 0);
                    break;
                }
            }
            CsrPmemFree (msg);
        }
    }
    ObexUtilDeinit(&pInst->obexJobChannelInst);
    ObexUtilDeinit(&pInst->obexStatusChannelInst);
    ObexUtilDeinit(&pInst->obexObjectChannelInst);
    CsrPmemFree(pInst->soapBody);
    CsrPmemFree(pInst->objectChName);
    CsrPmemFree(pInst);
}
#endif  /* ENABLE_SHUTDOWN */

