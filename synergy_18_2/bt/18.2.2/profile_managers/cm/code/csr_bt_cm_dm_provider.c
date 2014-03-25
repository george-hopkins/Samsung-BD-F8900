/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_cm_dm_sc_ssp_handler.h"

static const SignalHandlerType dmProviderHandler[CSR_BT_CM_DM_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtCmSetLocalNameReqHandler,                        /* CSR_BT_CM_SET_LOCAL_NAME_REQ */
    CsrBtCmReadRemoteNameReqHandler,                      /* CSR_BT_CM_READ_REMOTE_NAME_REQ */
#if !defined(EXCLUDE_CSR_BT_RFC_MODULE) && !defined(EXCLUDE_CSR_BT_SCO_MODULE)
    CsrBtCmDmScoConnectReqHandler,                        /* CSR_BT_CM_SCO_CONNECT_REQ */
#ifdef CSR_BT_INSTALL_CM_PRI_SCO_RENEGOTIATE
    CsrBtCmDmScoRenegotiateReqHandler,                    /* CSR_BT_CM_SCO_RENEGOTIATE_REQ */
#else
    NULL,
#endif
    CsrBtCmDmScoDisconnectReqHandler,                     /* CSR_BT_CM_SCO_DISCONNECT_REQ */
#else
    NULL,                                            /* CSR_BT_CM_SCO_CONNECT_REQ */
    NULL,                                            /* CSR_BT_CM_SCO_RENEGOTIATE_REQ */
    NULL,                                            /* CSR_BT_CM_SCO_DISCONNECT_REQ */
#endif /* EXCLUDE_CSR_BT_RFC_MODULE && EXCLUDE_CSR_BT_SCO_MODULE */
    CsrBtCmWriteClassOfDeviceReqHandler,                  /* CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ */
    CsrBtCmDmModeChangeReqHandler,                        /* CSR_BT_CM_DM_MODE_CHANGE_REQ */
    CsrBtCmSmDeleteStoreLinkKeyReqHandler,                /* CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ */
    CsrBtCmSmRemoveDeviceReqHandler,                      /* CSR_BT_CM_SM_REMOVE_DEVICE_REQ */
#ifdef CSR_BT_INSTALL_SC_SECURITY_MODE
    CsrBtCmSmSetSecModeReqHandler,                        /* CSR_BT_CM_SM_SET_SEC_MODE_REQ */
#else
    NULL,
#endif    
    CsrBtCmSmAclOpenReqHandler,                           /* CSR_BT_CM_SM_ACL_OPEN_REQ */
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
    CsrBtCmSmAuthenticateReqHandler,                      /* CSR_BT_CM_SM_AUTHENTICATE_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
    CsrBtCmSmEncryptionReqHandler,                        /* CSR_BT_CM_SM_ENCRYPTION_REQ */
#else
    NULL,
#endif
    CsrBtCmSmAclCloseReqHandler,                          /* CSR_BT_CM_SM_ACL_CLOSE_REQ */
    CsrBtCmSmSetDefaultSecLevelReqHandler,                /* CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ */
    CsrBtCmSmUnRegisterReqHandler,                        /* CSR_BT_CM_SM_UNREGISTER_REQ */
    CsrBtCmSmRegisterReqHandler,                          /* CSR_BT_CM_SM_REGISTER_REQ */
    CsrBtCmDmWriteLpSettingsReqHandler,                   /* CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ */
    CsrBtCmDmSwitchRoleReqHandler,                        /* CSR_BT_CM_DM_SWITCH_ROLE_REQ */
    CsrBtCmDmRoleDiscoveryReqHandler,                     /* CSR_BT_CM_ROLE_DISCOVERY_REQ */
    CsrBtCmReadBdAddrReqHandler,                          /* CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ */
#ifdef CSR_BT_INSTALL_CM_READ_LOCAL_NAME 
    CsrBtCmReadLocalNameReqHandler,                       /* CSR_BT_CM_READ_LOCAL_NAME_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_DUT_MODE
    CsrBtCmDeviceUnderTestReqHandler,                     /* CSR_BT_CM_ENABLE_DUT_MODE_REQ */
#else
    NULL,
#endif
    CsrBtCmWriteScanEnableReqHandler,                     /* CSR_BT_CM_WRITE_SCAN_ENABLE_REQ */
#ifdef CSR_BT_INSTALL_CM_READ_SCAN_EANBLE
    CsrBtCmReadScanEnableReqHandler,                      /* CSR_BT_CM_READ_SCAN_ENABLE_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_CONNECTABLE
    CsrBtCmConnectAbleReqHandler,                         /* CSR_BT_CM_CONNECTABLE_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_WRITE_PAGE_TO
    CsrBtCmDmWritePageToReqHandler,                       /* CSR_BT_CM_WRITE_PAGE_TO_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_READ_TX_POWER_LEVEL
    CsrBtCmReadTxPowerLevelReqHandler,                    /* CSR_BT_CM_READ_TX_POWER_LEVEL_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_GET_LINK_QUALITY
    CsrBtCmGetLinkQualityReqHandler,                      /* CSR_BT_CM_GET_LINK_QUALITY_REQ */
#else
    NULL,
#endif    
#ifdef CSR_BT_INSTALL_CM_READ_RSSI
    CsrBtCmReadRssiReqHandler,                            /* CSR_BT_CM_READ_RSSI_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_WRITE_COD
    CsrBtCmWriteCodReqHandler,                            /* CSR_BT_CM_WRITE_COD_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_READ_COD
    CsrBtCmReadCodReqHandler,                             /* CSR_BT_CM_READ_COD_REQ */
#else
    NULL,
#endif
    CsrBtCmReadRemoteExtFeaturesReqHandler,               /* CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ */
#ifdef CSR_BT_INSTALL_CM_AFH
    CsrBtCmSetAfhChannelClassReqHandler,                  /* CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ */
    CsrBtCmReadAfhChannelAssesModeReqHandler,             /* CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ */
    CsrBtCmWriteAfhChannelAssesModeReqHandler,            /* CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ */
#else
    NULL,
    NULL,
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_READ_LOCAL_EXT_FEATURES
    CsrBtCmReadLocalExtFeaturesReqHandler,                /* CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_AFH
    CsrBtCmReadAfhChannelMapReqHandler,                   /* CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_READ_CLOCK
    CsrBtCmReadClockReqHandler,                           /* CSR_BT_CM_READ_CLOCK_REQ */
#else
    NULL,
#endif
    CsrBtCmReadLocalVersionReqHandler,                    /* CSR_BT_CM_READ_LOCAL_VERSION_REQ */
#ifdef CSR_BT_INSTALL_CM_PRI_SET_EVENT_FILTER_BDADDR
    CsrBtCmSetEventFilterBdaddrReqHandler,                /* CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ */
#else
    NULL,
#endif
    CsrBtCmSetEventFilterCodReqHandler,                   /* CSR_BT_CM_SET_EVENT_FILTER_COD_REQ */
    CsrBtCmClearEventFilterReqHandler,                    /* CSR_BT_CM_CLEAR_EVENT_FILTER_REQ */
#ifdef CSR_BT_INSTALL_CM_PRI_IAC
    CsrBtCmReadIacReqHandler,                             /* CSR_BT_CM_READ_IAC_REQ */
    CsrBtCmWriteIacReqHandler,                            /* CSR_BT_CM_WRITE_IAC_REQ */
#else
    NULL,
    NULL,
#endif
    CsrBtCmDmWriteCacheParamsReqHandler,                  /* CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ */
    CsrBtCmDmUpdateAndClearCachedParamReqHandler,         /* CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ */
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
    CsrBtCmReadEncryptionStatusReqHandler,                /* CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ */
#else
    NULL,
#endif

#ifdef CSR_BT_INSTALL_CM_WRITE_PAGE_SCAN
    CsrBtCmWritePageScanSettingsReqHandler,               /* CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ */
    CsrBtCmWritePageScanTypeReqHandler,                   /* CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ */
#else
    NULL,
    NULL,
#endif
    CsrBtCmWriteInquiryScanSettingsReqHandler,            /* CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ */
#ifdef CSR_BT_INSTALL_CM_WRITE_INQUIRY_SCAN_TYPE
    CsrBtCmWriteInquiryScanTypeReqHandler,                /* CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ */
#else
    NULL,
#endif
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
#ifdef CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS
    CsrBtCmDmModeSettingsReqHandler,                      /* CsrBtCmDmModeSettingsReq */
#else
    NULL,
#endif
#else
    NULL,                                            /* CsrBtCmDmModeSettingsReq */
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
#ifdef CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS
    CsrBtCmDmL2caModeSettingsReqHandler,                  /* CsrBtCmDmL2caModeSettingsReq */
#else
    NULL,
#endif
#else
    NULL,                                            /* CsrBtCmDmL2caModeSettingsReq */
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
#ifdef CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS
    CsrBtCmDmBnepModeSettingsReqHandler,                  /* CsrBtCmDmBnepModeSettingsReq */
#else
    NULL,
#endif
#else
    NULL,
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
    CsrBtCmDmCheckSsrReqHandler,                          /* CSR_BT_CM_DM_CHECK_SSR_REQ */
#else
    NULL,
    NULL,
    NULL,
    NULL,
#endif /* CSR_BT_BT_VERSION */
    CsrBtCmSmBondingReqHandler,                           /* CSR_BT_CM_SM_BONDING_REQ */
#ifdef CSR_BT_INSTALL_CM_SC_MODE_CONFIG
    CsrBtCmSmSecModeConfigReqHandler,                     /* CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_SC_OOB
    CsrBtCmSmReadLocalOobDataReqHandler,                  /* CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ */
#else
    NULL,                                                 /* CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ */
#endif
    NULL,                                                 /* CSR_BT_CM_SM_READ_DEVICE_REQ */
    NULL,                                                 /* CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ */
    CsrBtCmEirUpdateManufacturerReqHandler,               /* CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ */
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    CsrBtCmDmWriteAutoFlushTimeoutReqHandler,             /* CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_READ_FAILED_CONTACT_COUNTER
    CsrBtCmReadFailedContactCounterReqHandler,            /* CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ */
#else
    NULL,
#endif
    CsrBtCmReadRemoteFeaturesReqHandler,                  /* CSR_BT_CM_READ_REMOTE_FEATURES_REQ */
#ifdef CSR_BT_INSTALL_CM_WRITE_VOICE_SETTINGS
    CsrBtCmWriteVoiceSettingsReqHandler,                  /* CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ */
#else
    NULL,                                                 /* CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ */
#endif
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    CsrBtCmSmAccessReqHandler,                            /* CSR_BT_CM_SM_ACCESS_REQ */
#else
    NULL,                                                 /* CSR_BT_CM_SM_ACCESS_REQ */
#endif
    CsrBtCmAclDetachReqHandler,                           /* CSR_BT_CM_ACL_DETACH_REQ */
#ifdef CSR_BT_INSTALL_CM_SWITCH_ROLE_PUBLIC
    CsrBtCmAlwaysMasterDevicesReqHandler,                 /* CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ */  
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_CM_DUT_MODE
    CsrBtCmDeviceUnderTestDisableReqHandler,              /* CSR_BT_CM_DISABLE_DUT_MODE_REQ */
#else
    NULL,
#endif
#ifdef CSR_BT_LE_ENABLE
    CsrBtCmSmLeSecurityReqHandler,                        /* CSR_BT_CM_SM_LE_SECURITY_REQ */
    CsrBtCmSmSetEncryptionKeySizeReqHandler,              /* CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ */
#else
    NULL,
    NULL,
#endif
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
    CsrBtCmIncomingScoReqHandler,                         /* CSR_BT_CM_INCOMING_SCO_REQ */
#else
    NULL,                                                 /* CSR_BT_CM_INCOMING_SCO_REQ */
#endif
};

void CsrBtCmDmSignalHandler(cmInstanceData_t *cmData)
{
    CsrPrim         *primPtr;

    primPtr = (CsrPrim *) cmData->recvMsgP;
    if((*primPtr - CSR_BT_CM_DM_PRIM_DOWNSTREAM_LOWEST < CSR_BT_CM_DM_PRIM_DOWNSTREAM_COUNT) && dmProviderHandler[*primPtr] != NULL)
    {
        CsrBtCmDmLockQueue(cmData);
        dmProviderHandler[*primPtr](cmData);
    }

    else
    {
        CsrBtCmStateEventException(CSR_BT_CM_PRIM, *primPtr, cmData->globalState);
    }
}

void CsrBtCmDmProvider(cmInstanceData_t *cmData)
{
    CsrPrim         *primPtr;

    primPtr = (CsrPrim *) cmData->recvMsgP;

    if ((cmData->dmVar.dmInProgress) && (*primPtr != CSR_BT_CM_DISABLE_DUT_MODE_REQ))
    {    /* Need to save signal, because we are waiting for a DM complete signal.
            CSR_BT_CM_DISABLE_DUT_MODE_REQ shall be handled right away */
        CsrMessageQueuePush(&cmData->dmVar.SaveQueue, CSR_BT_CM_PRIM, cmData->recvMsgP);
        cmData->recvMsgP = NULL;
    }
    else
    {    /* The DM is ready just proceed */
        CsrBtCmDmSignalHandler(cmData);
    }
}

void CsrBtCmDmLocalQueueHandler(void)
{
    CsrBtCmPrim  *prim;

    prim = CsrPmemAlloc(sizeof(CsrBtCmPrim));
    *prim = CSR_BT_CM_DM_HOUSE_CLEANING;
    CsrBtCmPutMessage(CSR_BT_CM_IFACEQUEUE, prim);
}

void CsrBtCmDmRestoreQueueHandler(cmInstanceData_t *cmData)
{
    CsrUint16          eventClass;
    void *              msg;

    CsrBtCmDmUnlockQueue(cmData);

    if(CsrMessageQueuePop(&cmData->dmVar.SaveQueue, &eventClass, &msg))
    {
        CsrPmemFree(cmData->recvMsgP);
        cmData->recvMsgP = msg;
        CsrBtCmDmSignalHandler(cmData);
    }
}

CsrBool cancelDmMsg(cmInstanceData_t *cmData, CsrBtCmPrim type, CsrSchedQid phandle, CsrBtDeviceAddr bd_addr)
{
    CsrUint16                eventClass;
    void                    *msg;
    CsrBool                  cancelMsg   = FALSE;
    CsrMessageQueueType    *tempQueue  = NULL;

    while(CsrMessageQueuePop(&cmData->dmVar.SaveQueue, &eventClass, &msg))
    {
        if (!cancelMsg && eventClass == CSR_BT_CM_PRIM && (type == (*((CsrBtCmPrim *) msg))))
        {
            switch (type)
            {
                case CSR_BT_CM_READ_REMOTE_NAME_REQ:
                    {
                        CsrBtCmReadRemoteNameReq * prim = (CsrBtCmReadRemoteNameReq *) msg;

                        if (phandle == prim->phandle && bd_addr_eq(&(prim->deviceAddr), &(bd_addr)))
                        {
                            cancelMsg = TRUE;
                            CsrPmemFree(msg);
                        }
                        else
                        {
                            CsrMessageQueuePush(&tempQueue, eventClass, msg);
                        }
                        break;
                    }
                default:
                    {
                        CsrMessageQueuePush(&tempQueue, eventClass, msg);
                        break;
                    }
            }
        }
        else
        {
            CsrMessageQueuePush(&tempQueue, eventClass, msg);
        }
    }
    cmData->dmVar.SaveQueue = tempQueue;
    return (cancelMsg);
}

