/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_mapc_handler.h"
#include "csr_bt_mapc_sef.h"


#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

static const MapcStateHandlerType mapcStateHandlers[CSR_BT_MAPC_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtMapcGetInstanceIdsReqHandler,                    /* CSR_BT_MAPC_GET_INSTANCE_IDS_REQ */
    CsrBtMapcConnectReqHandler,                           /* CSR_BT_MAPC_CONNECT_REQ */
    CsrBtMapcCancelConnectReqHandler,                     /* CSR_BT_MAPC_CANCEL_CONNECT_REQ */
    CsrBtMapcDisconnectReqHandler,                        /* CSR_BT_MAPC_DISCONNECT_REQ */
    CsrBtMapcSelectMasInstanceResHandler,                 /* CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES */
    CsrBtMapcSetFolderReqHandler,                         /* CSR_BT_MAPC_SET_FOLDER_REQ */
    CsrBtMapcSetBackFolderReqHandler,                     /* CSR_BT_MAPC_SET_BACK_FOLDER_REQ */
    CsrBtMapcSetRootFolderReqHandler,                     /* CSR_BT_MAPC_SET_ROOT_FOLDER_REQ */
    CsrBtMapcGetFolderListingReqHandler,                  /* CSR_BT_MAPC_GET_FOLDER_LISTING_REQ */
    CsrBtMapcGetFolderListingResHandler,                  /* CSR_BT_MAPC_GET_FOLDER_LISTING_RES */
    CsrBtMapcGetMessageListingReqHandler,                 /* CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ */
    CsrBtMapcGetMessageListingResHandler,                 /* CSR_BT_MAPC_GET_MESSAGE_LISTING_RES */
    CsrBtMapcGetMessageReqHandler,                        /* CSR_BT_MAPC_GET_MESSAGE_REQ */
    CsrBtMapcGetMessageResHandler,                        /* CSR_BT_MAPC_GET_MESSAGE_RES */
    CsrBtMapcSetMessageStatusReqHandler,                  /* CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ */
    CsrBtMapcPushMessageReqHandler,                       /* CSR_BT_MAPC_PUSH_MESSAGE_REQ */
    CsrBtMapcPushMessageResHandler,                       /* CSR_BT_MAPC_PUSH_MESSAGE_RES */
    CsrBtMapcUpdateInboxReqHandler,                       /* CSR_BT_MAPC_UPDATE_INBOX_REQ */
    CsrBtMapcAbortReqHandler,                             /* CSR_BT_MAPC_ABORT_REQ */
    CsrBtMapcNotificationRegistrationReqHandler,          /* CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ */
    CsrBtMapcEventNotificationResHandler,                 /* CSR_BT_MAPC_EVENT_NOTIFICATION_RES */
    CsrBtMapcSecurityInReqHandler,                        /* CSR_BT_MAPC_SECURITY_IN_REQ */
    CsrBtMapcSecurityOutReqHandler,                       /* CSR_BT_MAPC_SECURITY_OUT_REQ */
    CsrBtMapcRegisterQIDReqHandler,                       /* CSR_BT_MAPC_REGISTER_QID_REQ */
};

static void csrBtMapcInitCommon(MapcInstanceData *pInst)
{
    pInst->obexClientInst = ObexUtilInit(pInst->mapcInstanceId, pInst, CSR_BT_MAPC_CLIENT_INST_ID);

    CsrBtScSetSecInLevel(&pInst->secIncoming, CSR_BT_SEC_DEFAULT,
                CSR_BT_OBEX_MESSAGE_ACCESS_MANDATORY_SECURITY_INCOMING,
                CSR_BT_OBEX_MESSAGE_ACCESS_DEFAULT_SECURITY_INCOMING,
                CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);


    CsrBtScSetSecOutLevel(&pInst->secOutgoing, CSR_BT_SEC_DEFAULT,
                CSR_BT_OBEX_MESSAGE_ACCESS_MANDATORY_SECURITY_OUTGOING,
                CSR_BT_OBEX_MESSAGE_ACCESS_DEFAULT_SECURITY_OUTGOING,
                CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    /* Tell the SD that it must look for the OBEX_MESSAGE_ACCESS_SERVER_UUID service,
       when it perform a SD_READ_AVAILABLE_SERVICE_REQ                          */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_OBEX_MESSAGE_ACCESS_SERVER_UUID);
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID);

    /* OBEX Server part of MAPC Client */
    pInst->maxFrameSize = CSR_BT_MAPC_PROFILE_DEFAULT_MTU_SIZE;
    pInst->serverChannel = CSR_BT_NO_SERVER;

    if (pInst->mapcInstanceId == CSR_BT_MAPC_IFACEQUEUE)
    {
        /* This is the MAPC manager */
        pInst->mapcInstances = CsrPmemZalloc(sizeof(CsrBtMapcInstancePool));
    }
}

void CsrBtMapcInit(void **gash)
{
    MapcInstanceData *pInst;
    CsrBtMapcRegisterQidReq *prim;

    *gash                           = (MapcInstanceData *) CsrPmemZalloc(sizeof(MapcInstanceData));
    pInst                           = *gash;
    pInst->mapcInstanceId           = CsrSchedTaskQueueGet();

    /* init the instance data */
    csrBtMapcInitCommon(pInst);

    prim                  = pnew(CsrBtMapcRegisterQidReq);
    prim->type            = CSR_BT_MAPC_REGISTER_QID_REQ;
    prim->mapcInstanceId  = pInst->mapcInstanceId;
    CsrBtMapcMessagePut(CSR_BT_MAPC_IFACEQUEUE, prim);
}

void CsrBtMapcHandler(void **gash)
{
    CsrUint16        eventClass=0;
    void            *msg   = NULL;
    MapcInstanceData *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &msg);

    switch(eventClass)
    {
        case CSR_BT_MAPC_PRIM:
        {
            CsrBtMapcPrim *type;
            type = (CsrBtMapcPrim*) msg;

            if (*type < CSR_BT_MAPC_PRIM_DOWNSTREAM_COUNT &&
                (mapcStateHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }

            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                /* State/Event ERROR! */
                CsrStateEventException("MAPC", eventClass, *type, 0);
#endif
                CsrBtMapcFreeDownstreamMessageContents(eventClass, msg);
            }
            break;
        }
        case CSR_BT_CM_PRIM :
        {
            CsrUint8    id;
            CsrBool     validMsg = FALSE;

            if (ObexUtilGetInstIdentifierFromCmUpstreamMessage(msg, &id))
            {
                if (id == CSR_BT_MAPC_CLIENT_INST_ID)
                {
                    if (ObexUtilCmMsgHandler(pInst->obexClientInst, &msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
                    { /* This message is handled by the common OBEX library */
                        validMsg = TRUE;
                    }
                }
                else if (id == CSR_BT_MAPC_SERVER_INST_ID)
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
                CsrBtMapcPrim *type;
                type = (CsrBtMapcPrim*) msg;

                CsrStateEventException("MAPC - invalid CM message", eventClass, *type, 0);
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
            CsrStateEventException("MAPC", eventClass, 0, 0);
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
void CsrBtMapcDeinit(void ** gash)
{
    CsrUint16        eventClass=0;
    void            *msg=NULL;
    CsrBool          lastMsg     = FALSE;
    MapcInstanceData *pInst      = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if (!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_MAPC_PRIM:
                {
                    CsrBtMapcFreeDownstreamMessageContents(eventClass, msg);
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
                    CsrStateEventException("MAPC",eventClass, 0, 0);
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
    while (pInst->mapcInstances)
    {
        CsrBtMapcInstancePool   *temp = pInst->mapcInstances;
        pInst->mapcInstances = pInst->mapcInstances->next;
        
        CsrPmemFree(temp);
    }
        
    CsrPmemFree(pInst);
}
#endif    /* ENABLE_SHUTDOWN */

