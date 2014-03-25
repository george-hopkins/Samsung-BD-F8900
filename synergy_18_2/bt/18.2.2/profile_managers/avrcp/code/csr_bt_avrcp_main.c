/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_avrcp_main.h"
#include "csr_bt_avrcp_prim.h"

typedef void (* AvrcpAppStateCmnHandlerType)(AvrcpInstanceData_t *instData);
typedef void (* AvrcpRoleHandlerType)(AvrcpInstanceData_t *instData);
typedef void (* AvrcpCmL2caHandlerType)(AvrcpInstanceData_t *instData);

/* Application (downstream) message handlers */
static const AvrcpAppStateCmnHandlerType avrcpCtrlHandler[AVRCP_STATE_APP_NUM][CSR_BT_AVRCP_PRIM_DOWNSTREAM_COUNT] =
{
    {/* IDLE */
        CsrBtAvrcpConfiqReqIdleState,                    /* CSR_BT_AVRCP_CONFIG_REQ */
        CsrBtAvrcpActivateReqIdleState,                  /* CSR_BT_AVRCP_ACTIVATE_REQ */
        CsrBtAvrcpDeactivateReqIdleState,                /* CSR_BT_AVRCP_DEACTIVATE_REQ */
        CsrBtAvrcpConnectReqIdleState,                   /* CSR_BT_AVRCP_CONNECT_REQ */
        CsrBtAvrcpCancelConnectReqIdleBusyState,         /* CSR_BT_AVRCP_CANCEL_CONNECT_REQ */
        CsrBtAvrcpDisconnectReqIdleBusyState,            /* CSR_BT_AVRCP_DISCONNECT_REQ */
        CsrBtAvrcpSecurityInReqHandler,                  /* CSR_BT_AVRCP_SECURITY_IN_REQ */
        CsrBtAvrcpSecurityOutReqHandler,                 /* CSR_BT_AVRCP_SECURITY_OUT_REQ */
    },
    {/* BUSY */
        CsrBtAvrcpUtilSaveMessage,                       /* CSR_BT_AVRCP_CONFIG_REQ */
        CsrBtAvrcpUtilSaveMessage,                       /* CSR_BT_AVRCP_ACTIVATE_REQ */
        CsrBtAvrcpUtilSaveMessage,                       /* CSR_BT_AVRCP_DEACTIVATE_REQ */
        CsrBtAvrcpUtilSaveMessage,                       /* CSR_BT_AVRCP_CONNECT_REQ */
        CsrBtAvrcpCancelConnectReqIdleBusyState,         /* CSR_BT_AVRCP_CANCEL_CONNECT_REQ */
        CsrBtAvrcpDisconnectReqIdleBusyState,            /* CSR_BT_AVRCP_DISCONNECT_REQ */
        CsrBtAvrcpSecurityInReqHandler,                  /* CSR_BT_AVRCP_SECURITY_IN_REQ */
        CsrBtAvrcpSecurityOutReqHandler,                 /* CSR_BT_AVRCP_SECURITY_OUT_REQ */
    }
};

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
/* Target specific post-connection handlers */
static const AvrcpRoleHandlerType avrcpTgHandler[] =
{
    CsrBtAvrcpTgMpRegisterReqHandler,                /* CSR_BT_AVRCP_TG_MP_REGISTER_REQ */
    CsrBtAvrcpTgMpUnregisterReqHandler,              /* CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ */
    CsrBtAvrcpTgNotiReqHandler,                      /* CSR_BT_AVRCP_TG_NOTI_REQ */
    CsrBtAvrcpTgPasSetReqHandler,                    /* CSR_BT_AVRCP_TG_PAS_SET_REQ */
    CsrBtAvrcpTgSetAddressedPlayerReqHandler,        /* CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ */
    CsrBtAvrcpTgNotiResHandler,                      /* CSR_BT_AVRCP_TG_NOTI_RES */
    CsrBtAvrcpTgPlayResHandler,                      /* CSR_BT_AVRCP_TG_PLAY_RES */
    CsrBtAvrcpTgSearchResHandler,                    /* CSR_BT_AVRCP_TG_SEARCH_RES */
    CsrBtAvrcpTgChangePathResHandler,                /* CSR_BT_AVRCP_TG_CHANGE_PATH_RES */
    CsrBtAvrcpTgPassThroughResHandler,               /* CSR_BT_AVRCP_TG_PASS_THROUGH_RES */
    CsrBtAvrcpTgPasSetResHandler,                    /* CSR_BT_AVRCP_TG_PAS_SET_RES */
    CsrBtAvrcpTgPasCurrentResHandler,                /* CSR_BT_AVRCP_TG_PAS_CURRENT_RES */
    CsrBtAvrcpTgSetVolumeResHandler,                 /* CSR_BT_AVRCP_TG_SET_VOLUME_RES */
    CsrBtAvrcpTgGetFolderItemsResHandler,            /* CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES */
    CsrBtAvrcpTgGetAttributesResHandler,             /* CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES */
    CsrBtAvrcpTgGetPlayStatusResHandler,             /* CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES */
    CsrBtAvrcpTgSetAddressedPlayerResHandler,        /* CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES */
    CsrBtAvrcpTgSetBrowsedPlayerResHandler,          /* CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES */
    CsrBtAvrcpTgAddToNowPlayingResHandler,           /* CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES */
};
#endif
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MODULE */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
/* Controller specific post-connection handlers */
static const AvrcpRoleHandlerType avrcpCtHandler[] =
{
    CsrBtAvrcpCtInformDispCharsetReqHandler,         /* CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ */
    CsrBtAvrcpCtNotiRegisterReqHandler,              /* CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ */
    CsrBtAvrcpCtPassThroughReqHandler,               /* CSR_BT_AVRCP_CT_PASS_THROUGH_REQ */
    CsrBtAvrcpCtPasAttIdReqHandler,                  /* CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ */
    CsrBtAvrcpCtPasValIdReqHandler,                  /* CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ */
    CsrBtAvrcpCtPasAttTxtReqHandler,                 /* CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ */
    CsrBtAvrcpCtPasValTxtReqHandler,                 /* CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ */
    CsrBtAvrcpCtPasAttTxtResHandler,                 /* CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES */
    CsrBtAvrcpCtPasValTxtResHandler,                 /* CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES */
    CsrBtAvrcpCtPasCurrentReqHandler,                /* CSR_BT_AVRCP_CT_PAS_CURRENT_REQ */
    CsrBtAvrcpCtPasSetReqHandler,                    /* CSR_BT_AVRCP_CT_PAS_SET_REQ */
    CsrBtAvrcpCtGetPlayStatusReqHandler,             /* CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ */
    CsrBtAvrcpCtGetAttributesReqHandler,             /* CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ */
    CsrBtAvrcpCtGetAttributesResHandler,             /* CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES */
    CsrBtAvrcpCtGetFolderItemsReqHandler,            /* CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ */
    CsrBtAvrcpCtPlayReqHandler,                      /* CSR_BT_AVRCP_CT_PLAY_REQ */
    CsrBtAvrcpCtSearchReqHandler,                    /* CSR_BT_AVRCP_CT_SEARCH_REQ */
    CsrBtAvrcpCtChangePathReqHandler,                /* CSR_BT_AVRCP_CT_CHANGE_PATH_REQ */
    CsrBtAvrcpCtSetVolumeReqHandler,                 /* CSR_BT_AVRCP_CT_SET_VOLUME_REQ */
    CsrBtAvrcpCtSetAddressedPlayerReqHandler,        /* CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ */
    CsrBtAvrcpCtSetBrowsedPlayerReqHandler,          /* CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ */
    CsrBtAvrcpCtAddToNowPlayingReqHandler,           /* CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ */
    CsrBtAvrcpCtInformBatteryStatusReqHandler,       /* CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ */
    CsrBtAvrcpCtUnitInfoCmdReqHandler,               /* CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ */
    CsrBtAvrcpCtSubUnitInfoCmdReqHandler,            /* CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ */
    CsrBtAvrcpCtSetSSVolumeReqHandler,               /*CSR_BT_AVRCP_CT_SET_SS_VOLUME_REQ*/
};
#endif
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_MODULE */

/* CM message handlers */
static const AvrcpCmL2caHandlerType avrcpCmL2caHandler[CSR_BT_CM_L2CA_PRIM_UPSTREAM_COUNT] =
{
    CsrBtAvrcpCmL2caRegisterCfmHandler,              /* CSR_BT_CM_L2CA_REGISTER_CFM */
    CsrBtAvrcpCmL2caConnectCfmHandler,               /* CSR_BT_CM_L2CA_CONNECT_CFM */
    CsrBtAvrcpCmL2caConnectAcceptCfmHandler,         /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
    CsrBtAvrcpCmL2caCancelConnectAcceptCfmHandler,   /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
    CsrBtAvrcpCmL2caDataCfmHandler,                  /* CSR_BT_CM_L2CA_DATA_CFM */
    CsrBtAvrcpCmL2caDataIndHandler,                  /* CSR_BT_CM_L2CA_DATA_IND */
    CsrBtAvrcpCmL2caDisconnectIndHandler,            /* CSR_BT_CM_L2CA_DISCONNECT_IND */
    NULL,                                            /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
    NULL,                                            /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
    NULL,                                            /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
    CsrBtAvrcpCmModeChangeIndHandler,                /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
};

/*******************************************************************************
 Profile initialize
*******************************************************************************/
void CsrBtAvrcpInit(void **gash)
{
    AvrcpInstanceData_t *instData;

    *gash = (void *) CsrPmemZalloc(sizeof(AvrcpInstanceData_t));
    instData = (AvrcpInstanceData_t *) *gash;

    /* Control/state specific */
    AVRCP_CHANGE_STATE(instData->appState, AVRCP_STATE_APP_BUSY);
    AVRCP_CHANGE_STATE(instData->activateStateCont, AVRCP_STATE_ACT_DEACTIVATED);
#ifdef CSR_BT_INSTALL_AVRCP_13_14
    AVRCP_CHANGE_STATE(instData->activateStateBrow, AVRCP_STATE_ACT_DEACTIVATED);
#endif
    instData->ctrlHandle                = CSR_SCHED_QID_INVALID;
    instData->sniffReqResendAllowed     = TRUE;
    instData->lpMode                    = CSR_BT_LINK_STATUS_DISCONNECTED;
    instData->lpWantedMode              = CSR_BT_LINK_STATUS_SNIFF;
    instData->pendingCtrlPrim           = CSR_BT_AVRCP_HOUSE_CLEANING;

    /* SDP related */
    instData->sdpSearchData             = CsrBtUtilSdcInit(CsrBtAvrcpSdpResultHandler, CSR_BT_AVRCP_IFACEQUEUE);

    /* Misc */
    CsrCmnListInit(&instData->connList, 0, NULL, CsrBtAvrcpUtilConnRemoveCB);

    /* TG specific  */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    CsrCmnListInit(&instData->tgLocal.mpList, 0, CsrBtAvrcpTgUtilMpListAddCB, CsrBtAvrcpTgUtilMpListRemoveCB);
#endif
    instData->tgLocal.srHandle          = AVRCP_SDP_INVALID_SR_HANDLE;
    instData->tgLocal.srAvrcpVersion    = CSR_BT_AVRCP_CONFIG_SR_VERSION_10;
#endif

    /* CT specific */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
    instData->ctLocal.srAvrcpVersion    = CSR_BT_AVRCP_CONFIG_SR_VERSION_10;
#endif

    CsrBtScSetSecInLevel(&instData->secIncomingCont, CSR_BT_SEC_DEFAULT,
                    CSR_BT_AV_RCP_MANDATORY_SECURITY_INCOMING,
                    CSR_BT_AV_RCP_DEFAULT_SECURITY_INCOMING,
                    CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
                    CSR_BT_RESULT_CODE_AVRCP_UNACCEPTABLE_PARAMETER);
#ifdef CSR_BT_INSTALL_AVRCP_13_14
    CsrBtScSetSecInLevel(&instData->secIncomingBrow, CSR_BT_SEC_DEFAULT,
                    CSR_BT_AV_RCP_MANDATORY_SECURITY_INCOMING,
                    CSR_BT_AV_RCP_DEFAULT_SECURITY_INCOMING,
                    CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
                    CSR_BT_RESULT_CODE_AVRCP_UNACCEPTABLE_PARAMETER);
#endif
    CsrBtScSetSecOutLevel(&instData->secOutgoingCont, CSR_BT_SEC_DEFAULT,
                     CSR_BT_AV_RCP_MANDATORY_SECURITY_OUTGOING,
                     CSR_BT_AV_RCP_DEFAULT_SECURITY_OUTGOING,
                     CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
                     CSR_BT_RESULT_CODE_AVRCP_UNACCEPTABLE_PARAMETER);

#ifdef CSR_BT_INSTALL_AVRCP_13_14
    CsrBtScSetSecOutLevel(&instData->secOutgoingBrow, CSR_BT_SEC_DEFAULT,
                     CSR_BT_AV_RCP_MANDATORY_SECURITY_OUTGOING,
                     CSR_BT_AV_RCP_DEFAULT_SECURITY_OUTGOING,
                     CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
                     CSR_BT_RESULT_CODE_AVRCP_UNACCEPTABLE_PARAMETER);
#endif
    /* Tell the SD that it must look for the CSR_BT_AV_REMOTE_CONTROL_UUID
       and the  CSR_BT_AV_REMOTE_CONTROL_TARGET_UUID service,
       when it perform a SD_READ_AVAILABLE_SERVICE_REQ              */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE  //yjkim fix 
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_AV_REMOTE_CONTROL_UUID);
#endif
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE  //yjkim fix
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_AV_REMOTE_CONTROL_TARGET_UUID);
#endif
    CsrBtCml2caRegisterReqExtSend(CSR_BT_AVRCP_IFACEQUEUE,
                                  CSR_BT_AVCTP_PSM,
                                  L2CA_MODE_MASK_ENHANCED_RETRANS,
                                  0); /*flags*/
#ifdef CSR_BT_INSTALL_AVRCP_13_14
    CsrBtCml2caRegisterReqExtSend(CSR_BT_AVRCP_IFACEQUEUE,
                                  CSR_BT_AVCTP_BROWSING_PSM,
                                  L2CA_MODE_MASK_ENHANCED_RETRANS,
                                  0); /*flags*/
#endif
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
void CsrBtAvrcpDeinit(void **gash)
{
    CsrUint16            msg_type=0;
    void                *msg_data=NULL;
    CsrBool              lastMsg;
    AvrcpInstanceData_t   *instData;

    instData = (AvrcpInstanceData_t *) (*gash);

    /* Continue to poll any message off the input queue */
    lastMsg = FALSE;

    while (!lastMsg)
    {
        if (!CsrMessageQueuePop(&instData->saveQueue, &msg_type, &msg_data))
        {
            lastMsg = (CsrBool)(!CsrSchedMessageGet(&msg_type, &msg_data));
        }
        if (!lastMsg)
        {
            switch (msg_type)
            {
                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg_data);
                    break;
                }

                case CSR_BT_AVRCP_PRIM:
                {
                    CsrBtAvrcpFreeDownstreamMessageContents(CSR_BT_AVRCP_PRIM, msg_data);
                    break;
                }

                case CSR_SCHED_PRIM:
                    break;

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                default:
                    { /* Unknown event type!?!? */
                        CsrStateEventException("AVRCP",
                        msg_type,
                        0,
                        0);
                        break;
                    }
#endif
            }
            CsrPmemFree (msg_data);
        }
    }

    /* Free ALL instance data */
    CsrBtUtilSdcRfcDeinit(&instData->sdpSearchData);
    CsrCmnListDeinit(&instData->connList);

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    CsrCmnListDeinit(&instData->tgLocal.mpList);
#else
    if(instData->tgLocal.mpDefault)
    {
        CsrPmemFree(instData->tgLocal.mpDefault->playerName);
        CsrPmemFree(instData->tgLocal.mpDefault);
    }
#endif    
#endif

    CsrPmemFree(instData);
}
#endif

void CsrBtAvrcpHandler(void **gash)
{
    AvrcpInstanceData_t   *instData;
    CsrUint16            eventClass=0;
    void                *msg=NULL;

    instData = (AvrcpInstanceData_t *) (*gash);

    if (!instData->restoreFlag)
    {
        CsrSchedMessageGet(&eventClass , &msg);
    }
    else
    {
        if(!CsrMessageQueuePop(&(instData->saveQueue), &eventClass, &msg))
        {
            instData->restoreFlag = FALSE;
            CsrSchedMessageGet(&eventClass , &msg);
        }
        else
        {
        }
    }

    instData->recvMsgP = msg;

    switch(eventClass)
    {
        case CSR_BT_AVRCP_PRIM:
            {
                CsrBtAvrcpPrim primType;
                primType = *(CsrBtAvrcpPrim *)msg;

                if (CSR_BT_AVRCP_PRIM_DOWNSTREAM_COUNT + CSR_BT_AVRCP_PRIM_DOWNSTREAM_LOWEST > primType)
                {/* Handle control message */
                    avrcpCtrlHandler[instData->appState][primType](instData);
                }
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
                else if ((CSR_BT_AVRCP_TG_PRIM_DOWNSTREAM_COUNT + CSR_BT_AVRCP_TG_PRIM_DOWNSTREAM_LOWEST> primType) && (CSR_BT_AVRCP_TG_PRIM_DOWNSTREAM_LOWEST <= primType))
                {/* Handle TG post-connection message */
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
                    avrcpTgHandler[primType - CSR_BT_AVRCP_TG_PRIM_DOWNSTREAM_LOWEST](instData);
#else
                    switch(primType)
                    {
                        case CSR_BT_AVRCP_TG_MP_REGISTER_REQ:
                        {
                            CsrBtAvrcpTgMpRegisterReqHandler(instData);
                            break;
                        }
                        case CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ:
                        {
                            CsrBtAvrcpTgMpUnregisterReqHandler(instData);
                            break;
                        }
                        case CSR_BT_AVRCP_TG_PASS_THROUGH_RES:
                        {
                            CsrBtAvrcpTgPassThroughResHandler(instData);
                            break;
                        }
                    }
#endif /* CSR_BT_INSTALL_AVRCP_TG_13_14 */
                }
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MODULE */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                else if ((CSR_BT_AVRCP_CT_PRIM_DOWNSTREAM_COUNT + CSR_BT_AVRCP_CT_PRIM_DOWNSTREAM_LOWEST > primType) && (CSR_BT_AVRCP_CT_PRIM_DOWNSTREAM_LOWEST <= primType))
                {/* Handle CT post-connection message */
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                    avrcpCtHandler[primType - CSR_BT_AVRCP_CT_PRIM_DOWNSTREAM_LOWEST](instData);
#else
                    switch(primType)
                    {
                        case CSR_BT_AVRCP_CT_PASS_THROUGH_REQ:
                        {
                            CsrBtAvrcpCtPassThroughReqHandler(instData);
                            break;
                        }
                        case CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ:
                        {
                            CsrBtAvrcpCtUnitInfoCmdReqHandler(instData);
                            break;
                        }
                        case CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ:
                        {
                            CsrBtAvrcpCtSubUnitInfoCmdReqHandler(instData);
                            break;
                        }
                    }
#endif
                }
#endif
                else /* Make sure the else-block is always included */
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    if (primType != CSR_BT_AVRCP_HOUSE_CLEANING)
                    {
                        CsrStateEventException("AVRCP",
                        eventClass,
                        primType,
                        instData->appState);
                    }
#endif
                    CsrBtAvrcpFreeDownstreamMessageContents(CSR_BT_AVRCP_PRIM, msg);
                }
                break;
            }

        case CSR_BT_CM_PRIM:
        {
            CsrBtCmPrim primType;
            primType = *(CsrBtCmPrim *) msg;

            if ((primType <= CSR_BT_CM_L2CA_PRIM_UPSTREAM_HIGHEST) &&
                (primType >= CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST))
            {
                if (avrcpCmL2caHandler[primType - CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST] != NULL)
                {
                    avrcpCmL2caHandler[primType - CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST](instData);
                }
                else
                {
                    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, instData->recvMsgP);
                }
            }
            else if (CsrBtUtilSdcVerifyCmMsg(instData->recvMsgP))
            {
                if (CsrBtUtilSdcCmMsgHandler(instData, instData->sdpSearchData, instData->recvMsgP))
                {
                    CsrBtAvrcpSdpRestartSearch(instData);
                }
            }
            else if ((primType >= CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST) && (primType <= CSR_BT_CM_SDC_PRIM_UPSTREAM_HIGHEST))
            {
                if (primType == CSR_BT_CM_SDS_REGISTER_CFM)
                {
                    CsrBtAvrcpCmSdsRegisterCfmHandler(instData);
                }
                else if (primType == CSR_BT_CM_SDS_UNREGISTER_CFM)
                {
                    CsrBtAvrcpCmSdsUnregisterCfmHandler(instData);
                }
                else
                {/* Not possible - already handled by SDC handler */
                }
            }
            else
            {
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, instData->recvMsgP);
            }
            break;
        }
        case CSR_SCHED_PRIM:
            break;

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                CsrStateEventException("AVRCP",
                    eventClass, 0, instData->appState);
            }
#endif
    }
    CsrPmemFree(instData->recvMsgP);
}


