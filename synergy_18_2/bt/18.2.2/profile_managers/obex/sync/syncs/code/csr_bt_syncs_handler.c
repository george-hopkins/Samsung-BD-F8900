/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_profiles.h"
#include "csr_bt_syncs_prim.h"
#include "csr_bt_syncs_handler.h"
#include "csr_bt_syncs_sef.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#include "csr_bt_obex.h"

/* SYNCS jump table */
static const SyncsStateHandlerType syncsStateHandlers[CSR_BT_SYNCS_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtSyncsActivateReqHandler,           /* CSR_BT_SYNCS_ACTIVATE_REQ */
    CsrBtSyncsDeactivateReqHandler,         /* CSR_BT_SYNCS_DEACTIVATE_REQ */
    CsrBtSyncsConnectResHandler,            /* CSR_BT_SYNCS_CONNECT_RES */
    CsrBtSyncsAuthenticateResHandler,       /* CSR_BT_SYNCS_AUTHENTICATE_RES */
    CsrBtSyncsGetResHandler,                /* CSR_BT_SYNCS_GET_RES */
    CsrBtSyncsGetNextResHandler,            /* CSR_BT_SYNCS_GET_NEXT_RES */
    CsrBtSyncsPutResHandler,                /* CSR_BT_SYNCS_PUT_RES */
    CsrBtSyncsPutNextResHandler,            /* CSR_BT_SYNCS_PUT_NEXT_RES */
    CsrBtSyncsGetDeviceInfoResHandler,      /* CSR_BT_SYNCS_GET_DEVICE_INFO_RES */
    CsrBtSyncsGetPbChangeLogResHandler,     /* CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES */
    CsrBtSyncsGetPbCurChangeLogResHandler,  /* CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES */
    CsrBtSyncsGetPbInfoLogResHandler,       /* CSR_BT_SYNCS_GET_PB_INFO_LOG_RES */
    CsrBtSyncsGetPbEntryResHandler,         /* CSR_BT_SYNCS_GET_PB_ENTRY_RES */
    CsrBtSyncsGetPbAllResHandler,           /* CSR_BT_SYNCS_GET_PB_ALL_RES */
    CsrBtSyncsPutPbEntryResHandler,         /* CSR_BT_SYNCS_PUT_PB_ENTRY_RES */
    CsrBtSyncsPutPbAddEntryResHandler,      /* CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES */
    CsrBtSyncsGetCalChangeLogResHandler,    /* CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES */
    CsrBtSyncsGetCalCurChangeLogResHandler, /* CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES */
    CsrBtSyncsGetCalInfoLogResHandler,      /* CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES */
    CsrBtSyncsGetCalEntryResHandler,        /* CSR_BT_SYNCS_GET_CAL_ENTRY_RES */
    CsrBtSyncsGetCalAllResHandler,          /* CSR_BT_SYNCS_GET_CAL_ALL_RES */
    CsrBtSyncsPutCalEntryResHandler,        /* CSR_BT_SYNCS_PUT_CAL_ENTRY_RES */
    CsrBtSyncsPutCalAddEntryResHandler,     /* CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES */
    CsrBtSyncsSecurityInReqHandler,         /* CSR_BT_SYNCS_SECURITY_IN_REQ */
    CsrBtSyncsAuthenticateReqHandler,       /* CSR_BT_SYNCS_AUTHENTICATE_REQ */
};

void CsrBtSyncsInit(void **gash)
{
    SyncsInstanceDataType *pInst;
    *gash = CsrPmemZalloc(sizeof(SyncsInstanceDataType));
    pInst = *gash;

    pInst->serverChannel        = CSR_BT_NO_SERVER;
    pInst->obexServerInst = ObexUtilInit(CSR_BT_SYNCS_IFACEQUEUE, pInst, CSR_BT_SYNCS_SERVER_INST_ID);

    CsrBtScSetSecInLevel(&pInst->secIncoming, CSR_BT_SEC_DEFAULT,
                        CSR_BT_OBEX_IR_MC_SYNC_MANDATORY_SECURITY_INCOMING,
                        CSR_BT_OBEX_IR_MC_SYNC_DEFAULT_SECURITY_INCOMING,
                        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);
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
void CsrBtSyncsDeinit(void **gash)
{
    SyncsInstanceDataType *pInst = *gash;
    CsrUint16 msgType=0;
    void *msgData=NULL;
    CsrBool lastMsg = FALSE;

    /*    continue to poll any message of the input queue */
    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&msgType, &msgData));

        if (!lastMsg)
        {
            switch(msgType)
            {
                case CSR_BT_SYNCS_PRIM:
                {
                    CsrBtSyncsFreeDownstreamMessageContents(msgType, msgData);
                    break;
                }

                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(msgType, msgData);
                    break;
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                default:
                {
                    CsrStateEventException("SYNC_S",msgType, 0, 0);
                    break;
                }
#endif
            }
        }
    }

    if (pInst)
    {
        if (pInst->obexServerInst)
        {
            ObexUtilDeinit(&pInst->obexServerInst);
        }
        CsrPmemFree(pInst);
        pInst = NULL;
    }
}
#endif

void CsrBtSyncsHandler(void **gash)
{
    SyncsInstanceDataType *pInst = *gash;
    void                  *msg=NULL;
    CsrUint16             eventClass=0;

    CsrSchedMessageGet(&eventClass, &msg);

    switch(eventClass)
    {
        case CSR_BT_SYNCS_PRIM:
            {
                CsrBtSyncsPrim *type;
                type = (CsrBtSyncsPrim *) msg;

                if (*type < CSR_BT_SYNCS_PRIM_DOWNSTREAM_COUNT &&
                    (syncsStateHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
                {
                    ;
                }
                else
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    /* State/Event ERROR! */
                    CsrStateEventException("Invalid SYNCS-message", eventClass, *type, 0);
#endif
                    CsrBtSyncsFreeDownstreamMessageContents(eventClass, msg);
                }
                break;
            }
        case CSR_BT_CM_PRIM :
            {
                CsrUint8        id;
                CsrBool         validMsg = FALSE;

                if (ObexUtilGetInstIdentifierFromCmUpstreamMessage(msg, &id))
                {
                    if (id == CSR_BT_SYNCS_CLIENT_INST_ID)
                    { /* A client instance is not avaialble in this profile */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        {
                            CsrBtSyncsPrim  *type;
                            type = (CsrBtSyncsPrim *) msg;
                            CsrStateEventException("SYNCS - invalid CM message", eventClass, *type, 0);
                        }
#endif
                        CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                    }
                    else if (id == CSR_BT_SYNCS_SERVER_INST_ID)
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
                    CsrBtCmPrim *type;
                    type = (CsrBtCmPrim *)msg;

                    CsrStateEventException("SYNCS - invalid CM message", eventClass, *type, 0);
#endif
                    CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                }
                break;
            }
        case CSR_SCHED_PRIM:
            break;
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            /* Unknown event type!?!? */
            CsrStateEventException("SYNC_S - Unknown event type!",
                    eventClass,
                    0,
                    0);
            break;
#endif
    }
    CsrPmemFree(msg);
}

