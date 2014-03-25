/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_bips_handler.h"
#include "csr_bt_bips_sef.h"
#include "csr_bt_sc_private_lib.h"

#define CSR_BT_BIPS_SERV_INST_ID                0x01
#define CSR_BT_BIPS_CLI_INST_ID                 0x02

/* ---------- BIP Server jump table ---------- */
static const BipsStateHandlerType bipsCommonStateHandlers[CSR_BT_BIPS_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtBipsActivateReqHandler,                        /* CSR_BT_BIPS_ACTIVATE_REQ                             */
    CsrBtBipsDeactivateReqHandler,                      /* CSR_BT_BIPS_DEACTIVATE_REQ                           */
    CsrBtBipsConnectResHandler,                         /* CSR_BT_BIPS_CONNECT_RES                              */
    CsrBtBipsAuthenticateResHandler,                    /* CSR_BT_BIPS_AUTHENTICATE_RES                         */
    CsrBtBipsDisconnectReqHandler,                      /* CSR_BT_BIPS_DISCONNECT_REQ                           */
    CsrBtBipsGetInstanceQidReqHandler,                  /* CSR_BT_BIPS_GET_INSTANCE_QID_REQ                     */
    CsrBtBipsRegisterQidReqHandler,                     /* CSR_BT_BIPS_REGISTER_QID_REQ                         */
    CsrBtBipsSecurityInReqHandler,                      /* CSR_BT_BIPS_SECURITY_IN_REQ                          */
    CsrBtBipsChallengeResHandler,                       /* CSR_BT_BIPS_CHALLENGE_RES                            */
};

/* ---------- BIP Server Push jump table ----- */
static const BipsStateHandlerType bipsPushStateHandlers[CSR_BT_BIPS_PUSH_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtBipsPushGetCapabilitiesHeaderResHandler,       /* CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES         */
    CsrBtBipsPushGetCapabilitiesObjectResHandler,       /* CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES         */
    CsrBtBipsPushPutImageHeaderResHandler,              /* CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES                */
    CsrBtBipsPushPutImageFileResHandler,                /* CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES                  */
    CsrBtBipsPushPutLinkedThumbnailHeaderResHandler,    /* CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES     */
    CsrBtBipsPushPutLinkedThumbnailFileResHandler,      /* CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES       */
    CsrBtBipsPushPutLinkedAttachmentHeaderResHandler,   /* CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES    */
    CsrBtBipsPushPutLinkedAttachmentFileResHandler,     /* CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES      */
};

/* ---------- BIP Server Remote Camera jump table --- */
static const BipsStateHandlerType bipsRcStateHandlers[CSR_BT_BIPS_RC_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtBipsRcGetMonitoringHeaderResHandler,           /* CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES       */
    CsrBtBipsRcGetMonitoringObjectResHandler,           /* CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES       */
    CsrBtBipsRcGetImagePropertiesHeaderResHandler,      /* CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES       */
    CsrBtBipsRcGetImagePropertiesObjectResHandler,      /* CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES       */
    CsrBtBipsRcGetImageHeaderResHandler,                /* CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES                  */
    CsrBtBipsRcGetImageObjectResHandler,                /* CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES                  */
    CsrBtBipsRcGetLinkedThumbnailHeaderResHandler,      /* CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES       */
    CsrBtBipsRcGetLinkedThumbnailObjectResHandler,      /* CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES       */
};

/* ---------- BIP Server Auto Archive jump table ---------- */
static const BipsStateHandlerType bipsAaStateHandlers[CSR_BT_BIPS_AA_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtBipsAaGetImageListReqHandler,                  /* CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ                    */
    CsrBtBipsAaGetImageListHeaderResHandler,            /* CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES             */
    CsrBtBipsAaGetImageListResHandler,                  /* CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES                    */
    CsrBtBipsAaGetCapabilitiesReqHandler,               /* CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ                  */
    CsrBtBipsAaGetCapabilitiesResHandler,               /* CSR_BT_BIPS_AA_GET_CAPABILITIES_RES                  */
    CsrBtBipsAaGetImagePropertiesReqHandler,            /* CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ              */
    CsrBtBipsAaGetImagePropertiesResHandler,            /* CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES              */
    CsrBtBipsAaGetImageReqHandler,                      /* CSR_BT_BIPS_AA_GET_IMAGE_REQ                         */
    CsrBtBipsAaGetImageResHandler,                      /* CSR_BT_BIPS_AA_GET_IMAGE_RES                         */
    CsrBtBipsAaGetLinkedAttachmentReqHandler,           /* CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ             */
    CsrBtBipsAaGetLinkedAttachmentResHandler,           /* CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES             */
    CsrBtBipsAaGetLinkedThumbnailReqHandler,            /* BIPS_AA_GET_THUMBNAIL_REQ                            */
    CsrBtBipsAaGetLinkedThumbnailResHandler,            /* BIPS_AA_GET_THUMBNAIL_RES                            */
    CsrBtBipsAaDeleteImageReqHandler,                   /* CSR_BT_BIPS_AA_DELETE_IMAGE_REQ                      */
    CsrBtBipsAaAbortReqHandler,                         /* CSR_BT_BIPS_AA_ABORT_REQ                             */
};

void CsrBtBipsInit(void **gash)
{
    BipsInstanceData        *pInst;
    CsrBtBipsRegisterQidReq *prim = (CsrBtBipsRegisterQidReq *)CsrPmemZalloc(sizeof(CsrBtBipsRegisterQidReq));
    *gash                         = (void *) CsrPmemZalloc(sizeof(BipsInstanceData));
    pInst                         = (BipsInstanceData *) *gash;
    pInst->localServerCh          = CSR_BT_NO_SERVER;
    pInst->myAppHandle            = CsrSchedTaskQueueGet();
    pInst->obexServInst           = ObexUtilInit(pInst->myAppHandle, pInst, CSR_BT_BIPS_SERV_INST_ID);
    pInst->obexCliInst            = ObexUtilInit(pInst->myAppHandle, pInst, CSR_BT_BIPS_CLI_INST_ID);
    pInst->connectType            = CSR_BT_BIPS_INVALID_CONNECT_TYPE;
    pInst->getStatusResponseCode  = CSR_BT_OBEX_NOT_ACCEPTABLE_RESPONSE_CODE;

    if (pInst->myAppHandle == CSR_BT_BIPS_IFACEQUEUE)
    { /* This is one of the main instances                                      */
        pInst->bipsInstances                = CsrPmemZalloc(sizeof(BipsInstancesPool));
    }

    CsrBtScSetSecInLevel(&pInst->secIncoming,
                         CSR_BT_SEC_DEFAULT,
                         CSR_BT_OBEX_IMAGING_RESPONDER_MANDATORY_SECURITY_INCOMING,
                         CSR_BT_OBEX_IMAGING_RESPONDER_DEFAULT_SECURITY_INCOMING,
                         CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                         CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    prim->type    = CSR_BT_BIPS_REGISTER_QID_REQ;
    prim->qId     = pInst->myAppHandle;
    CsrBtBipsMessagePut(CSR_BT_BIPS_IFACEQUEUE, prim);
}

void CsrBtBipsHandler(void **gash)
{
    CsrUint16        eventClass=0;
    void             *msg       = NULL;
    BipsInstanceData *pInst     = *gash;
    CsrUint8 returnValue        = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
    CsrSchedMessageGet(&eventClass, &msg);

    switch(eventClass)
    {
        case CSR_BT_BIPS_PRIM:
        {
            CsrBtBipsPrim *type = msg;

            if ((*type < (CSR_BT_BIPS_PRIM_DOWNSTREAM_LOWEST + CSR_BT_BIPS_PRIM_DOWNSTREAM_COUNT)) &&
                (bipsCommonStateHandlers[(*type) - CSR_BT_BIPS_PRIM_DOWNSTREAM_LOWEST] != NULL))
            {
                returnValue = bipsCommonStateHandlers[(*type) - CSR_BT_BIPS_PRIM_DOWNSTREAM_LOWEST](pInst, msg);
            }

            /* Check to see if message is for push jump table */
            else if ((*type >= CSR_BT_BIPS_PUSH_PRIM_DOWNSTREAM_LOWEST) &&
                (*type < (CSR_BT_BIPS_PUSH_PRIM_DOWNSTREAM_LOWEST + CSR_BT_BIPS_PUSH_PRIM_DOWNSTREAM_COUNT)) &&
                (bipsPushStateHandlers[(*type) - CSR_BT_BIPS_PUSH_PRIM_DOWNSTREAM_LOWEST] != NULL))
            {
                returnValue = bipsPushStateHandlers[(*type) - CSR_BT_BIPS_PUSH_PRIM_DOWNSTREAM_LOWEST](pInst, msg);
            }

            /* Check to see if message is for remote camera jump table */
            else if ((*type >= CSR_BT_BIPS_RC_PRIM_DOWNSTREAM_LOWEST) &&
                (*type < (CSR_BT_BIPS_RC_PRIM_DOWNSTREAM_LOWEST + CSR_BT_BIPS_RC_PRIM_DOWNSTREAM_COUNT)) &&
                (bipsRcStateHandlers[(*type) - CSR_BT_BIPS_RC_PRIM_DOWNSTREAM_LOWEST] != NULL))
            {
                returnValue = bipsRcStateHandlers[(*type) - CSR_BT_BIPS_RC_PRIM_DOWNSTREAM_LOWEST](pInst, msg);
            }

            /* Check to see if message is for auto archive jump table */
            else if ((*type >= CSR_BT_BIPS_AA_PRIM_DOWNSTREAM_LOWEST) &&
                (*type < (CSR_BT_BIPS_AA_PRIM_DOWNSTREAM_LOWEST + CSR_BT_BIPS_AA_PRIM_DOWNSTREAM_COUNT)) &&
                (bipsAaStateHandlers[(*type) - CSR_BT_BIPS_AA_PRIM_DOWNSTREAM_LOWEST] != NULL))
            {
                if (pInst->clientConnected)
                {
                    returnValue = bipsAaStateHandlers[(*type) - CSR_BT_BIPS_AA_PRIM_DOWNSTREAM_LOWEST](pInst, msg);
                }
                else
                { /* This signal is crossing with a disconnect, ignore it       */
                    CsrBtBipsFreeDownstreamMessageContents(CSR_BT_BIPS_PRIM, msg);
                }
            }
            else
            {
                returnValue = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
            }

            if (returnValue == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
            { /* State/Event ERROR! */
                CsrBtBipsStateEventException(eventClass, *type);
                CsrBtBipsFreeDownstreamMessageContents(CSR_BT_BIPS_PRIM, msg);
            }
            break;
        }
        case CSR_BT_CM_PRIM:
        {
            CsrUint8 id;

            if (ObexUtilGetInstIdentifierFromCmUpstreamMessage(msg, &id))
            {
                if (id == CSR_BT_BIPS_SERV_INST_ID)
                {
                    returnValue = ObexUtilCmMsgHandler(pInst->obexServInst, &msg);
                }
                else if (id == CSR_BT_BIPS_CLI_INST_ID)
                {
                    returnValue = ObexUtilCmMsgHandler(pInst->obexCliInst, &msg);
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
            { /* State/Event ERROR! */
                CsrBtBipsStateEventException(eventClass, *((CsrBtCmPrim*) msg));
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
            }
            break;
        }
        case CSR_SCHED_PRIM:
        {
            break;
        }
        default:
        {
            CsrBtBipsStateEventException(eventClass, *((CsrPrim*) msg));
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
    1) empty the input message queue and free any allocated memory in the
          messages.
    2) free any instance data that may be allocated.
****************************************************************************/
void CsrBtBipsDeinit(void ** gash)
{
    CsrUint16        eventClass=0;
    void             *msg=NULL;
    CsrBool          lastMsg = FALSE;
    BipsInstanceData *pInst = *gash;


    /* continue to poll any message of the input queue */
    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if(!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_BIPS_PRIM:
                {
                    CsrBtBipsFreeDownstreamMessageContents(eventClass, msg);
                    break;
                }

                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                    break;

                }
                default:
                {   /* Unknown event type!?!? */
                    CsrBtBipsStateEventException(eventClass, 0);
                    break;
                }
            }
            CsrPmemFree (msg);
        }
    }
    ObexUtilDeinit(&pInst->obexCliInst);
    ObexUtilDeinit(&pInst->obexServInst);
    CsrBtBipsPfreeCollectedInfo(pInst);
    CsrBtBipsPfreeAaObexPacket(pInst);
    if (pInst->bipsInstances)
    {
        BipsInstancesPool  *ptr  = pInst->bipsInstances;
        BipsInstancesPool  *next = NULL;

        while(ptr)
        {
            next = ptr->next;
            CsrPmemFree(ptr);
            ptr = next;
        }
    }
    CsrPmemFree(pInst);
}
#endif /* ENABLE_SHUTDOWN */

