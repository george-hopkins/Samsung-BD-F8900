/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_syncc_handler.h"
#include "csr_bt_syncc_sef.h"


#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

static const SynccStateHandlerType synccStateHandlers[CSR_BT_SYNCC_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtSynccConnectReqHandler,                           /* CSR_BT_SYNCC_CONNECT_REQ */
    CsrBtSynccCancelConnectReqHandler,                     /* CSR_BT_SYNCC_CANCEL_CONNECT_REQ */
    CsrBtSynccDisconnectReqHandler,                        /* CSR_BT_SYNCC_DISCONNECT_REQ */
    CsrBtSynccAuthenticateResHandler,                      /* CSR_BT_SYNCC_AUTHENTICATE_RES */
    CsrBtSynccGetObjectReqHandler,                         /* CSR_BT_SYNCC_GET_OBJECT_REQ */
    CsrBtSynccGetObjectResHandler,                         /* CSR_BT_SYNCC_GET_OBJECT_RES */
    CsrBtSynccAddObjectReqHandler,                         /* CSR_BT_SYNCC_ADD_OBJECT_REQ */
    CsrBtSynccAddObjectResHandler,                         /* CSR_BT_SYNCC_ADD_OBJECT_RES */
    CsrBtSynccModifyObjectReqHandler,                      /* CSR_BT_SYNCC_MODIFY_OBJECT_REQ */
    CsrBtSynccModifyObjectResHandler,                      /* CSR_BT_SYNCC_MODIFY_OBJECT_RES */
    CsrBtSynccDeleteObjectReqHandler,                      /* CSR_BT_SYNCC_DELETE_OBJECT_REQ */
    CsrBtSynccAbortReqHandler,                             /* CSR_BT_SYNCC_ABORT_REQ */
    CsrBtSynccActivateSyncCommandReqHandler,               /* CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ */
    CsrBtSynccSyncCommandResHandler,                       /* CSR_BT_SYNCC_SYNC_COMMAND_RES */
    CsrBtSynccDeactivateSyncCommandReqHandler,             /* CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ */
    CsrBtSynccSecurityInReqHandler,                        /* CSR_BT_SYNCC_SECURITY_IN_REQ */
    CsrBtSynccSecurityOutReqHandler,                       /* CSR_BT_SYNCC_SECURITY_OUT_REQ */
};

void CsrBtSynccInit(void **gash)
{
    SynccInstanceData *pInst;
    *gash                   = (SynccInstanceData *) CsrPmemZalloc(sizeof(SynccInstanceData));
    pInst                   = *gash;
    pInst->obexClientInst   = ObexUtilInit(CSR_BT_SYNCC_IFACEQUEUE, pInst, CSR_BT_SYNCC_CLIENT_INST_ID);

    CsrBtScSetSecInLevel(&pInst->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_OBEX_IR_MC_SYNC_MANDATORY_SECURITY_INCOMING,
        CSR_BT_OBEX_IR_MC_SYNC_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    CsrBtScSetSecOutLevel(&pInst->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_OBEX_IR_MC_SYNC_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_OBEX_IR_MC_SYNC_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    /* Tell the SD that it must look for the CSR_BT_OBEX_IR_MC_SYNC_SERVICE_UUID service,
       when it perform a SD_READ_AVAILABLE_SERVICE_REQ                          */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_OBEX_IR_MC_SYNC_SERVICE_UUID);

    /* OBEX Server part of SYNC Client */
    pInst->maxFrameSize          = CSR_BT_SYNCC_PROFILE_DEFAULT_MTU_SIZE;
    pInst->serverChannel         = CSR_BT_NO_SERVER;
}

void CsrBtSynccHandler(void **gash)
{
    CsrUint16           eventClass=0;
    void                *msg   = NULL;
    SynccInstanceData   *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &msg);

    switch(eventClass)
    {
        case CSR_BT_SYNCC_PRIM:
        {
            CsrBtSynccPrim *type;
            type = (CsrBtSynccPrim*) msg;

            if (*type < CSR_BT_SYNCC_PRIM_DOWNSTREAM_COUNT &&
                (synccStateHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }

            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                /* State/Event ERROR! */
                CsrStateEventException("SYNCC", eventClass, *type, 0);
#endif
                CsrBtSynccFreeDownstreamMessageContents(eventClass, msg);
            }
            break;
        }
        case CSR_BT_CM_PRIM :
        {
            CsrUint8    id;
            CsrBool     validMsg = FALSE;

            if (ObexUtilGetInstIdentifierFromCmUpstreamMessage(msg, &id))
            {
                if (id == CSR_BT_SYNCC_CLIENT_INST_ID)
                {
                    if (ObexUtilCmMsgHandler(pInst->obexClientInst, &msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
                    { /* This message is handled by the common OBEX library */
                        validMsg = TRUE;
                    }
                }
                else if (id == CSR_BT_SYNCC_SERVER_INST_ID)
                {
                    if (ObexUtilCmMsgHandler(pInst->obexServerInst, &msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
                    { /* This message is handled by the common OBEX library */
                        validMsg = TRUE;
                    }
                }
            }
            if (!validMsg)
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                /* State/Event ERROR! */
                CsrBtSynccPrim *type;
                type = (CsrBtSynccPrim*) msg;

                CsrStateEventException("SYNCC - invalid CM message", eventClass, *type, 0);
#endif
                CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
            }
            break;
        }
        case CSR_SCHED_PRIM:
        {
            break;
        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
        {/* Unknown event type!?!? */
            CsrStateEventException("SYNCC", eventClass, 0, 0);
            break;
        }
#endif
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
void CsrBtSynccDeinit(void ** gash)
{
    CsrUint16        eventClass=0;
    void            *msg=NULL;
    CsrBool          lastMsg     = FALSE;
    SynccInstanceData *pInst      = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if (!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_SYNCC_PRIM:
                {
                    CsrBtSynccFreeDownstreamMessageContents(eventClass, msg);
                    break;
                }

                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                    break;
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                default:
                {
                    CsrStateEventException("SYNCC",eventClass, 0, 0);
                    break;
                }
#endif
            }
            CsrPmemFree (msg);
        }
    }
    if (pInst->obexClientInst)
    {
        ObexUtilDeinit(&pInst->obexClientInst);
    }
    if (pInst->obexServerInst)
    {
        ObexUtilDeinit(&pInst->obexServerInst);
    }
    CsrPmemFree(pInst);
}
#endif    /* ENABLE_SHUTDOWN */

