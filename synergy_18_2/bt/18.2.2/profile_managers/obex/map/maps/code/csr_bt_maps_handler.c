/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_maps_handler.h"
#include "csr_bt_maps_sef.h"


#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

static const MapsStateHandlerType mapsStateHandlers[CSR_BT_MAPS_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtMapsGetInstanceIdsReqHandler,                  /* CSR_BT_MAPS_GET_INSTANCE_IDS_REQ */
    CsrBtMapsSelectMasInstanceResHandler,               /* CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES */
    CsrBtMapsActivateReqHandler,                        /* CSR_BT_MAPS_ACTIVATE_REQ */
    CsrBtMapsDeactivateReqHandler,                      /* CSR_BT_MAPS_DEACTIVATE_REQ */
    CsrBtMapsConnectResHandler,                         /* CSR_BT_MAPS_CONNECT_RES */
    CsrBtMapsSetBackFolderResHandler,                   /* CSR_BT_MAPS_SET_BACK_FOLDER_RES */
    CsrBtMapsSetRootFolderResHandler,                   /* CSR_BT_MAPS_SET_ROOT_FOLDER_RES */
    CsrBtMapsSetFolderResHandler,                       /* CSR_BT_MAPS_SET_FOLDER_RES */
    CsrBtMapsGetFolderListingHeaderResHandler,          /* CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES */
    CsrBtMapsGetFolderListingResHandler,                /* CSR_BT_MAPS_GET_FOLDER_LISTING_RES */
    CsrBtMapsGetMessageListingHeaderResHandler,         /* CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES */
    CsrBtMapsGetMessageListingResHandler,               /* CSR_BT_MAPS_GET_MESSAGE_LISTING_RES */
    CsrBtMapsGetMessageHeaderResHandler,                /* CSR_BT_MAPS_GET_MESSAGE_HEADER_RES */
    CsrBtMapsGetMessageResHandler,                      /* CSR_BT_MAPS_GET_MESSAGE_RES */
    CsrBtMapsSetMessageStatusResHandler,                /* CSR_BT_MAPS_SET_MESSAGE_STATUS_RES */
    CsrBtMapsPushMessageHeaderResHandler,               /* CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES */
    CsrBtMapsPushMessageResHandler,                     /* CSR_BT_MAPS_PUSH_MESSAGE_RES */
    CsrBtMapsUpdateInboxResHandler,                     /* CSR_BT_MAPS_UPDATE_INBOX_RES */
    CsrBtMapsNotificationRegistrationResHandler,        /* CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES */
    CsrBtMapsEventNotificationReqHandler,               /* CSR_BT_MAPS_EVENT_NOTIFICATION_REQ */
    CsrBtMapsEventNotificationResHandler,               /* CSR_BT_MAPS_EVENT_NOTIFICATION_RES */
    CsrBtMapsSecurityLevelReqHandler,                   /* CSR_BT_MAPS_SECURITY_LEVEL_REQ */
    CsrBtMapsRegisterQIDReqHandler,                     /* CSR_BT_MAPS_REGISTER_QID_REQ */
};

static void csrBtMapsInitCommon(MapsInstanceData *pInst)
{
    pInst->obexClientInst = ObexUtilInit(pInst->mapsInstanceId, pInst, CSR_BT_MAPS_CLIENT_INST_ID);
    pInst->obexServerInst = ObexUtilInit(pInst->mapsInstanceId, pInst, CSR_BT_MAPS_SERVER_INST_ID);;

    CsrBtScSetSecInLevel(&pInst->secInLevel, CSR_BT_SEC_DEFAULT,
                CSR_BT_OBEX_MESSAGE_ACCESS_MANDATORY_SECURITY_INCOMING,
                CSR_BT_OBEX_MESSAGE_ACCESS_DEFAULT_SECURITY_INCOMING,
                CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);


    CsrBtScSetSecOutLevel(&pInst->secOutLevel, CSR_BT_SEC_DEFAULT,
                CSR_BT_OBEX_MESSAGE_ACCESS_MANDATORY_SECURITY_OUTGOING,
                CSR_BT_OBEX_MESSAGE_ACCESS_DEFAULT_SECURITY_OUTGOING,
                CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    /* Tell the SD that it must look for the OBEX_MESSAGE_NOTIFICATION_SERVER_UUID service,
       when it perform a SD_READ_AVAILABLE_SERVICE_REQ                          */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_OBEX_MESSAGE_NOTIFICATION_SERVER_UUID);
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID);

    /* OBEX Server part of MAPS Server */
    csrBtMapsResetAppHeaderTags(pInst);

    pInst->serverChannel = CSR_BT_NO_SERVER;

    if (pInst->mapsInstanceId == CSR_BT_MAPS_IFACEQUEUE)
    {
        /* This is the MAPS manager */
        pInst->mapsInstances = CsrPmemZalloc(sizeof(CsrBtMapsInstancePool));
    }
}

void CsrBtMapsInit(void **gash)
{
    MapsInstanceData *pInst;
    CsrBtMapsRegisterQidReq *prim;

    *gash                           = (MapsInstanceData *) CsrPmemZalloc(sizeof(MapsInstanceData));
    pInst                           = *gash;
    pInst->mapsInstanceId           = CsrSchedTaskQueueGet();

    /* init the instance data */
    csrBtMapsInitCommon(pInst);

    prim                  = pnew(CsrBtMapsRegisterQidReq);
    prim->type            = CSR_BT_MAPS_REGISTER_QID_REQ;
    prim->mapsInstanceId  = pInst->mapsInstanceId;
    CsrBtMapsMessagePut(CSR_BT_MAPS_IFACEQUEUE, prim);
}

void CsrBtMapsHandler(void **gash)
{
    CsrUint16        eventClass=0;
    void             *msg   = NULL;
    MapsInstanceData *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &msg);

    switch(eventClass)
    {
        case CSR_BT_MAPS_PRIM:
        {
            CsrBtMapsPrim *type;
            type = (CsrBtMapsPrim*) msg;

            if (*type < CSR_BT_MAPS_PRIM_DOWNSTREAM_COUNT &&
                (mapsStateHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }

            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                /* State/Event ERROR! */
                CsrStateEventException("MAPS - Unknown MAPS primitive", eventClass, *type, 0);
#endif
                CsrBtMapsFreeDownstreamMessageContents(eventClass, msg);
            }
            break;
        }
        case CSR_BT_CM_PRIM :
        {
            CsrUint8    id;
            CsrBool     validMsg = FALSE;

            if (ObexUtilGetInstIdentifierFromCmUpstreamMessage(msg, &id))
            {
                if (id == CSR_BT_MAPS_CLIENT_INST_ID)
                {
                    if (ObexUtilCmMsgHandler(pInst->obexClientInst, &msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
                    { /* This message is handled by the common OBEX library */
                        validMsg = TRUE;
                    }
                }
                else if (id == CSR_BT_MAPS_SERVER_INST_ID)
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
                CsrBtMapsPrim *type;
                type = (CsrBtMapsPrim*) msg;

                CsrStateEventException("MAPS - invalid CM message", eventClass, *type, 0);
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
            CsrStateEventException("MAPS - Unknown event type", eventClass, 0, 0);
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
void CsrBtMapsDeinit(void ** gash)
{
    CsrUint16           eventClass=0;
    void                *msg=NULL;
    CsrBool             lastMsg     = FALSE;
    MapsInstanceData    *pInst      = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if (!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_MAPS_PRIM:
                {
                    CsrBtMapsFreeDownstreamMessageContents(eventClass, msg);
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
                    CsrStateEventException("MAPS De-init - unknown event",eventClass, 0, 0);
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
    
    while (pInst->mapsInstances)
    {
        CsrBtMapsInstancePool   *temp = pInst->mapsInstances;
        pInst->mapsInstances = pInst->mapsInstances->next;
        
        CsrPmemFree(temp);
    }
    CsrPmemFree(pInst);
}
#endif    /* ENABLE_SHUTDOWN */

