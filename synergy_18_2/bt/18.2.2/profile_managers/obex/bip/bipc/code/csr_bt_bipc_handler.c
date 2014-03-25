/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_bipc_handler.h"
#include "csr_bt_bipc_sef.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define CSR_BT_BIPC_CLI_INST_ID                 0x01
#define CSR_BT_BIPC_SERV_INST_ID                0x02

/* ---------- BIP Client jump table ---------- */
static const BipcStateHandlerType bipcStateGenHandlers[CSR_BT_BIPC_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtBipcConnectReqHandler,                      /* CSR_BT_BIPC_CONNECT_REQ */
    CsrBtBipcAbortReqHandler,                        /* CSR_BT_BIPC_ABORT_REQ */
    CsrBtBipcDisconnectReqHandler,                   /* CSR_BT_BIPC_DISCONNECT_REQ */
    CsrBtBipcAuthenticateResHandler,                 /* CSR_BT_BIPC_AUTHENTICATE_RES */
    CsrBtBipcSecurityOutReqHandler,                  /* CSR_BT_BIPC_SECURITY_OUT_REQ */
    CsrBtBipcRemoteFeaturesReqHandler,               /* CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ */
};

static const BipcStateHandlerType bipcStatePushHandlers[CSR_BT_BIPC_PUSH_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtBipcImagePushGetCapabilitiesReqHandler,     /* CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ */
    CsrBtBipcImagePushGetCapabilitiesResHandler,     /* CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES */
    CsrBtBipcImagePushPutImageReqHandler,            /* CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ */
    CsrBtBipcImagePushPutImageFileResHandler,        /* CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES */
    CsrBtBipcImagePushPutThumbnailFileResHandler,    /* CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES */
    CsrBtBipcImagePushPutLinkedAttachmentReqHandler, /* CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ */
    CsrBtBipcImagePushPutLinkedAttachmentResHandler  /* CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES */
};

static const BipcStateHandlerType bipcStateRcHandlers[CSR_BT_BIPC_RC_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtBipcRcGetMonitoringImageReqHandler,         /* CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ */
    CsrBtBipcRcGetRcMonitoringImageHeaderResHandler, /* CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES */
    CsrBtBipcRcGetMonitoringImageFileResHandler,     /* CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES */
    CsrBtBipcRcGetImagePropertiesReqHandler,         /* CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ */
    CsrBtBipcRcGetImagePropertiesResHandler,         /* CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES */
    CsrBtBipcRcGetImageReqHandler,                   /* CSR_BT_BIPC_RC_GET_IMAGE_REQ */
    CsrBtBipcRcGetImageResHandler,                   /* CSR_BT_BIPC_RC_GET_IMAGE_RES */
    CsrBtBipcRcGetLinkedThumbnailReqHandler,         /* CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ */
    CsrBtBipcRcGetLinkedThumbnailResHandler          /* CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES */
};

static const BipcStateHandlerType bipcStateAaHandlers [CSR_BT_BIPC_AA_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtBipcAaGetImageListHeaderResHandler,         /* CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES */
    CsrBtBipcAaGetImageListObjectResHandler,         /* CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES */
    CsrBtBipcAaGetCapabilitiesHeaderResHandler,      /* CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES */
    CsrBtBipcAaGetCapabilitiesObjectResHandler,      /* CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES */
    CsrBtBipcAaGetImagePropertiesHeaderResHandler,   /* CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES */
    CsrBtBipcAaGetImagePropertiesObjectResHandler,   /* CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES */
    CsrBtBipcAaGetImageHeaderResHandler,             /* CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES */
    CsrBtBipcAaGetImageObjectResHandler,             /* CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES */
    CsrBtBipcAaGetLinkedThumbnailHeaderResHandler,   /* BIPS_AA_GET_LINKED_THUMBNAIL_HEADER_RES */
    CsrBtBipcAaGetLinkedThumbnailObjectResHandler,   /* BIPS_AA_GET_LINKED_THUMBNAIL_OBJECT_RES */
    CsrBtBipcAaGetLinkedAttachmentHeaderResHandler,  /* CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES */
    CsrBtBipcAaGetLinkedAttachmentObjectResHandler,  /* CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES */
    CsrBtBipcAaDeleteImageResHandler                 /* CSR_BT_BIPC_AA_DELETE_IMAGE_RES */

};

static void handleBipcPrim(BipcInstanceData *pInst, void *msg)
{
    CsrUint8      returnValue;
    CsrBtBipcPrim *type = msg;

    if (*type < CSR_BT_BIPC_PRIM_DOWNSTREAM_COUNT)
    { /* General                                                                */
        returnValue = bipcStateGenHandlers[*type](pInst, msg);
    }
    else
    {
        if (pInst->clientState == BIPC_CLIENT_OBEX_CONNECTED_STATE)
        { /* The primary OBEX (client) connection is valid                      */
            if(*type <= CSR_BT_BIPC_PUSH_PRIM_DOWNSTREAM_HIGHEST)
            {/* Push                                                            */
                CsrUint16 index = (CsrUint16)(*type - CSR_BT_BIPC_PUSH_PRIM_DOWNSTREAM_LOWEST);
                returnValue     = bipcStatePushHandlers[index](pInst, msg);
            }
            else if (*type <= CSR_BT_BIPC_RC_PRIM_DOWNSTREAM_HIGHEST &&
                     *type >= CSR_BT_BIPC_RC_PRIM_DOWNSTREAM_LOWEST)
            { /* Remote Camera */
                CsrUint16 index = (CsrUint16)(*type - CSR_BT_BIPC_RC_PRIM_DOWNSTREAM_LOWEST);
                returnValue     = bipcStateRcHandlers[index](pInst, msg);
            }
            else if (*type <= CSR_BT_BIPC_AA_PRIM_DOWNSTREAM_HIGHEST &&
                     *type >= CSR_BT_BIPC_AA_PRIM_DOWNSTREAM_LOWEST)
            { /* Automatic Archive */
                CsrUint16 index = (CsrUint16)(*type - CSR_BT_BIPC_AA_PRIM_DOWNSTREAM_LOWEST);
                returnValue     = bipcStateAaHandlers[index](pInst, msg);
            }
            else
            {
                returnValue = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
            }
        }
        else
        { /* The primary OBEX (client) connection is invalid, ignore the
             response/request from the application                                */
            returnValue   = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
            CsrBtBipcFreeDownstreamMessageContents(CSR_BT_BIPC_PRIM, msg);
        }
    }

    if (returnValue == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    { /* State/Event ERROR! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrStateEventException("BIPC", CSR_BT_BIPC_PRIM, *type, 0);
#endif
        CsrBtBipcFreeDownstreamMessageContents(CSR_BT_BIPC_PRIM, msg);
    }
}

void CsrBtBipcInit(void **gash)
{
    BipcInstanceData *pInst;

    *gash                        = CsrPmemZalloc(sizeof(BipcInstanceData));
    pInst                        = *gash;
    pInst->myAppHandle           = CsrSchedTaskQueueGet();
    pInst->obexCliInst           = ObexUtilInit(pInst->myAppHandle, pInst, CSR_BT_BIPC_CLI_INST_ID);
    pInst->obexServInst          = ObexUtilInit(pInst->myAppHandle, pInst, CSR_BT_BIPC_SERV_INST_ID);
    pInst->sdpSearchData         = CsrBtUtilSdcInit(CsrBtBipcExtractRemoteFeatureSdpAttributes, pInst->myAppHandle);

    CsrBtScSetSecOutLevel(&pInst->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_OBEX_IMAGING_INITIATOR_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_OBEX_IMAGING_INITIATOR_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    /* Tell the SD that it must look for the CSR_BT_OBEX_IMAGING_RESPONDER_UUID
       service, when it perform a SD_READ_AVAILABLE_SERVICE_REQ                  */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_OBEX_IMAGING_RESPONDER_UUID);
}

void CsrBtBipcHandler(void **gash)
{
    CsrUint16        eventClass=0;
    void             *msg   = NULL;
    BipcInstanceData *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &msg);

    switch(eventClass)
    {
        case CSR_BT_BIPC_PRIM:
        {
            handleBipcPrim(pInst, msg);
            break;
        }
        case CSR_BT_CM_PRIM :
        {
            CsrUint8      returnValue   = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;

            if (pInst->clientState != BIPC_CLIENT_SDP_SEARCH_STATE)
            {
                CsrUint8 id;

                if (ObexUtilGetInstIdentifierFromCmUpstreamMessage(msg, &id))
                {
                    if (id == CSR_BT_BIPC_CLI_INST_ID)
                    {
                        returnValue = ObexUtilCmMsgHandler(pInst->obexCliInst, &msg);
                    }
                    else if (id == CSR_BT_BIPC_SERV_INST_ID)
                    {
                        returnValue = ObexUtilCmMsgHandler(pInst->obexServInst, &msg);
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
            }
            else
            {
                if (CsrBtUtilSdcVerifyCmMsg(msg))
                {
                    if (CsrBtUtilSdcCmMsgHandler(pInst, pInst->sdpSearchData, msg))
                    {
                        pInst->clientState = BIPC_CLIENT_IDLE_STATE;
                    }
                    else
                    {
                        ;
                    }
                }
                else
                {
                    returnValue = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
                }
            }

            if (returnValue == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
            { /* State/Event ERROR! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrBtCmPrim *type;
                type = (CsrBtCmPrim*) msg;

                CsrStateEventException("BIPC", CSR_BT_CM_PRIM, *type, 0);
#endif
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
            }
            break;
        }
        case CSR_SCHED_PRIM:
        {
            break;
        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
        { /* Unknown event type!?!? */
            CsrPrim          *type;
            type = (CsrPrim*) msg;
            CsrStateEventException("BIPC", eventClass, *type, 0);
            break;
        }
#endif    /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
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
void CsrBtBipcDeinit(void ** gash)
{
    CsrUint16           eventClass=0;
    void                *msg=NULL;
    CsrBool             lastMsg = FALSE;
    BipcInstanceData    *pInst  = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if (!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_BIPC_PRIM:
                {
                    CsrBtBipcFreeDownstreamMessageContents(eventClass, msg);
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
                    CsrStateEventException("BIPC", eventClass, 0, 0);
                    break;
                }
#endif
            }
            CsrPmemFree (msg);
        }
    }

    ObexUtilDeinit(&pInst->obexCliInst);
    ObexUtilDeinit(&pInst->obexServInst);
    CsrBtUtilSdcRfcDeinit(&(pInst->sdpSearchData));
    CsrPmemFree(pInst->servMiscInfo);
    CsrPmemFree(pInst->clientMiscInfo);
    CsrPmemFree(pInst);
}
#endif /* ENABLE_SHUTDOWN */
