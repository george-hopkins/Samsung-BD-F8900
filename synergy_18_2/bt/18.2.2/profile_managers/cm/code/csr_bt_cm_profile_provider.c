/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #5 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_bccmd.h"
#include "csr_bt_cm_sdc.h"
#include "csr_bt_cm_bnep.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_cm_dm_sc_ssp_handler.h"
#include "csr_bt_cm_events_handler.h"
#include "csr_bt_cm_common_amp.h"
#include "csr_bt_cm_le.h"

/***********************************************************************************
 * CM profile provider jump table
 ***********************************************************************************/

static const SignalHandlerType cmProviderHandlers[CSR_BT_CM_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtCmInquiryReqHandler,                        /* CSR_BT_CM_INQUIRY_REQ */
    CsrBtCmCancelInquiryReqHandler,                  /* CSR_BT_CM_CANCEL_INQUIRY_REQ */
#if !defined(EXCLUDE_CSR_BT_RFC_MODULE) && !defined(EXCLUDE_CSR_BT_SCO_MODULE)
    CsrBtCmDmScoAcceptConnectReqHandler,             /* CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ */
    CsrBtCmDmScoCancelAcceptConnectReqHandler,       /* CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ */
#else
    NULL,                                            /* CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ */
    NULL,                                            /* CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ */
#endif /* EXCLUDE_CSR_BT_RFC_MODULE && EXCLUDE_CSR_BT_SCO_MODULE */
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    CsrBtCmL2caDataWriteReqHandler,                   /* CSR_BT_CM_L2CA_DATA_REQ */
#else
    NULL,                                             /* CSR_BT_CM_L2CA_DATA_REQ */
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */
    CsrBtCmSmLinkKeyRequestResHandler,                /* CSR_BT_CM_SM_LINK_KEY_REQUEST_RES */
    CsrBtCmSmAuthoriseResHandler,                     /* CSR_BT_CM_SM_AUTHORISE_RES */
    CsrBtCmSmPinRequestResHandler,                    /* CSR_BT_CM_SM_PIN_REQUEST_RES */
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    CsrBtCmRfcUnRegisterReqMsgSend,                   /* CSR_BT_CM_UNREGISTER_REQ */
    CsrBtCmRfcDataReqHandler,                         /* CSR_BT_CM_DATA_REQ */
    CsrBtCmRfcDataResHandler,                         /* CSR_BT_CM_DATA_RES */
    CsrBtCmRfcControlReqMsgSend,                      /* CSR_BT_CM_CONTROL_REQ */
    CsrBtCmRfcPortNegResHandler,                      /* CSR_BT_CM_PORTNEG_RES */
#else
    NULL,                                             /* CSR_BT_CM_UNREGISTER_REQ */
    NULL,                                             /* CSR_BT_CM_DATA_REQ */
    NULL,                                             /* CSR_BT_CM_DATA_RES */
    NULL,                                             /* CSR_BT_CM_CONTROL_REQ */
    NULL,                                             /* CSR_BT_CM_PORTNEG_RES */
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    CsrBtCmBnepExtendedDataReqHandler,                /* CSR_BT_CM_BNEP_EXTENDED_DATA_REQ */
    CsrBtCmBnepExtendedMulticastDataReqHandler,       /* CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ */
    CsrBtCmBnepDisconnectResHandler,                  /* CSR_BT_CM_BNEP_DISCONNECT_RES */
#else
    NULL,                                             /* CSR_BT_CM_BNEP_EXTENDED_DATA_REQ */
    NULL,                                             /* CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ */
    NULL,                                             /* CSR_BT_CM_BNEP_DISCONNECT_RES */
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
    CsrBtCmSdcCancelSearchReqHandler,                 /* CSR_BT_CM_SDC_CANCEL_SEARCH_REQ */
    CsrBtCmSdcAttrReqHandle,                          /* CSR_BT_CM_SDC_ATTRIBUTE_REQ */
    CsrBtCmSdcCloseReqHandler,                        /* CSR_BT_CM_SDC_CLOSE_REQ */
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    CsrBtCmCancelConnectReqHandler,                   /* CSR_BT_CM_CANCEL_CONNECT_REQ */
#else
    NULL,                                             /* CSR_BT_CM_CANCEL_CONNECT_REQ */
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
    CsrBtCmSmCancelConnectReqHandler,                 /* CSR_BT_CM_SM_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    CsrBtCml2caCancelConnectReqHandler,               /* CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ */
#else
    NULL,                                             /* CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ */
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */
#ifdef CSR_BT_INSTALL_CM_PRI_ALWAYS_SUPPORT_MASTER_ROLE
    CsrBtCmAlwaysSupportMasterRoleReqHandler,         /* CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ */
#else
    NULL,                                             /* CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ */
#endif
    CsrBtCmCancelReadRemoteNameReqHandler,            /* CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ */
    CsrBtCmSmBondingCancelReqHandler,                 /* CSR_BT_CM_SM_BONDING_CANCEL_REQ */
    CsrBtCmSmIoCapabilityRequestResHandler,           /* CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES */
    CsrBtCmSmIoCapabilityRequestNegResHandler,        /* CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES */
    CsrBtCmSmUserConfirmationRequestResHandler,       /* CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES */
    CsrBtCmSmUserConfirmationRequestNegResHandler,    /* CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES */
    CsrBtCmSmUserPasskeyRequestResHandler,            /* CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES */
    CsrBtCmSmUserPasskeyRequestNegResHandler,         /* CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES */
    CsrBtCmSmSendKeypressNotificationReqHandler,      /* CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */
    CsrBtCmSmRepairResHandler,                        /* CSR_BT_CM_SM_REPAIR_RES */
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    CsrBtCmBnepCancelConnectReqHandler,               /* CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ */
#else
    NULL,                                             /* CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ */
#endif
#ifdef CSR_BT_INSTALL_CM_ROLE_SWITCH_CONFIG
    CsrBtCmRoleSwitchConfigReqHandler,                /* CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ */
#else    
    NULL,                                             /* CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ */
#endif
    CsrBtCmSetEventMaskReqHandler,                    /* CSR_BT_CM_SET_EVENT_MASK_REQ */
#ifdef CSR_BT_INSTALL_CM_LOW_POWER_CONFIG_PUBLIC
    CsrBtCmModeChangeConfigReqHandler,                /* CSR_BT_CM_MODE_CHANGE_CONFIG_REQ */
#else    
    NULL,                                             /* CSR_BT_CM_MODE_CHANGE_CONFIG_REQ */
#endif
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
#ifdef CSR_BT_INSTALL_CM_PRI_L2CA_UNREGISTER
    CsrBtCmL2caUnRegisterReqHandler,                  /* CSR_BT_CM_L2CA_UNREGISTER_REQ */
#else
    NULL,                                             /* CSR_BT_CM_L2CA_UNREGISTER_REQ */
#endif
#else
    NULL,                                             /* CSR_BT_CM_L2CA_UNREGISTER_REQ */
#endif
#ifdef CSR_BT_INSTALL_CM_WRITE_LINK_SUPERVISION_TIMEOUT
    CsrBtCmWriteLinkSuperVTimeoutReqHandler,          /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ */
#else
    NULL,                                             /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ */
#endif
#if defined(CSR_AMP_ENABLE) && !defined(EXCLUDE_CSR_BT_L2CA_MODULE)
    CsrBtCmAmpMoveChannelReqHandler,                  /* CSR_BT_CM_MOVE_CHANNEL_REQ */
    CsrBtCmAmpMoveChannelResHandler,                  /* CSR_BT_CM_MOVE_CHANNEL_RES */
#else
    NULL,                                             /* CSR_BT_CM_MOVE_CHANNEL_REQ */
    NULL,                                             /* CSR_BT_CM_MOVE_CHANNEL_RES */
#endif
#ifdef CSR_BT_INSTALL_L2CAP_CONNLESS_SUPPORT
    CsrBtCmL2caConnlessDataReqHandler,                /* CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ */
#else
    NULL,                                             /* CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ */
#endif
    CsrBtCmLogicalChannelTypeHandler,                 /* CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ */
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    CsrBtCmRfcPortnegReqHandler,                      /* CSR_BT_CM_PORTNEG_REQ */
#else
    NULL,                                             /* CSR_BT_CM_PORTNEG_REQ */
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
    CsrBtCmReadRemoteVersionReqHandler,               /* CSR_BT_CM_READ_REMOTE_VERSION_REQ */
    CsrBtCmRegisterHandlerReqHandler,                 /* CSR_BT_CM_REGISTER_HANDLER_REQ */
#if !defined(EXCLUDE_CSR_BT_L2CA_MODULE)
    CsrBtCmL2caDataResHandler,                        /* CSR_BT_CM_L2CA_DATA_RES */
#ifdef CSR_BT_INSTALL_CM_PRI_L2CA_DATA_ABORT
    CsrBtCmL2caDataAbortReqHandler,                   /* CSR_BT_CM_L2CA_DATA_ABORT_REQ */
#else
    NULL,                                             /* CSR_BT_CM_L2CA_DATA_ABORT_REQ */
#endif
#else
    NULL,                                             /* CSR_BT_CM_L2CA_DATA_RES */
    NULL,                                             /* CSR_BT_CM_L2CA_DATA_ABORT_REQ */
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */
    CsrBtCmA2DPBitrateHandler,                        /* CSR_BT_CM_A2DP_BIT_RATE_REQ */
    CsrBtCmGetSecurityConfResHandler,                 /* CSR_BT_CM_GET_SECURITY_CONF_RES */  
    CsrBtCmDataBufferEmptyReqHandler,                 /* CSR_BT_CM_DATA_BUFFER_EMPTY_REQ */
#ifdef CSR_BT_LE_ENABLE
    CsrBtCmLeScanReqHandler,                          /* CSR_BT_CM_LE_SCAN_REQ */
    CsrBtCmLeAdvertiseReqHandler,                     /* CSR_BT_CM_LE_ADVERTISE_REQ */
    CsrBtCmLeWhitelistSetReqHandler,                  /* CSR_BT_CM_LE_WHITELIST_SET_REQ */
    CsrBtCmLeConnparamReqHandler,                     /* CSR_BT_CM_LE_CONNPARAM_REQ */
    CsrBtCmLeConnparamUpdateReqHandler,               /* CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ */
    CsrBtCmLeAcceptConnparamUpdateResHandler,         /* CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES */
    CsrBtCmSmKeyRequestResHandler,                    /* CSR_BT_CM_SM_KEY_REQUEST_RES */
#else
    NULL,                                             /* CSR_BT_CM_LE_SCAN_REQ */
    NULL,                                             /* CSR_BT_CM_LE_ADVERTISE_REQ */
    NULL,                                             /* CSR_BT_CM_LE_WHITELIST_SET_REQ */
    NULL,                                             /* CSR_BT_CM_LE_CONNPARAM_REQ */
    NULL,                                             /* CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ */
    NULL,                                             /* CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES */
    NULL,                                             /* CSR_BT_CM_SM_KEY_REQUEST_RES */
#endif /* CSR_BT_LE_ENABLE */
#ifdef CSR_BT_INSTALL_CM_EIR_FLAGS
    CsrBtCmEirFlagsReqHandler,                        /* CSR_BT_CM_EIR_FLAGS_REQ */
#else
    NULL,
#endif
    CsrBtCmReadEncryptionKeySizeReqHandler,           /* CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ */
#ifdef CSR_BT_LE_ENABLE
    CsrBtCmReadAdvertisingChTxPowerReqHandler,        /* CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ */
    CsrBtCmLePhysicalLinkStatusReqHandler,            /* CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ */
    CsrBtCmLeReceiverTestReqHandler,                  /* CSR_BT_CM_LE_RECEIVER_TEST_REQ */
    CsrBtCmLeTransmitterTestReqHandler,               /* CSR_BT_CM_LE_TRANSMITTER_TEST_REQ */
    CsrBtCmLeTestEndReqHandler,                       /* CSR_BT_CM_LE_TEST_END_REQ */
#else
    NULL,                                             /* CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ */
    NULL,                                             /* CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ */  
    NULL,                                             /* CSR_BT_CM_LE_RECEIVER_TEST_REQ */
    NULL,                                             /* CSR_BT_CM_LE_TRANSMITTER_TEST_REQ */
    NULL,                                             /* CSR_BT_CM_LE_TEST_END_REQ */
#endif /* CSR_BT_LE_ENABLE */
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
    CsrBtCmMapScoPcmResHandler,                       /* CSR_BT_CM_MAP_SCO_PCM_RES */
#else
    NULL,                                             /* CSR_BT_CM_MAP_SCO_PCM_RES */
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    CsrBtCmL2caGetChannelInfoReqHandler,              /* CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ */
#else
    NULL,                                             /* CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ */
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE*/

    /* These two must ALWAYS be at the end */
    CsrBtCmRestoreServiceManagerQueue,                /* CSR_BT_CM_SM_HOUSE_CLEANING */
    CsrBtCmDmRestoreQueueHandler                      /* CSR_BT_CM_DM_HOUSE_CLEANING */
};

/*************************************************************************************
  CsrBtCmProfileProvider:
************************************************************************************/
void CsrBtCmProfileProvider(cmInstanceData_t *cmData)
{
    CsrUint16 id = *(CsrPrim*)cmData->recvMsgP;

    if ((id >= CSR_BT_CM_PRIM_DOWNSTREAM_LOWEST) &&
             (id <= CSR_BT_CM_PRIM_DOWNSTREAM_HIGHEST) &&
             (cmProviderHandlers[(CsrUint16)(id - CSR_BT_CM_PRIM_DOWNSTREAM_LOWEST)] != NULL))
    {
        /* Use jump table */
        cmProviderHandlers[(CsrUint16) (id - CSR_BT_CM_PRIM_DOWNSTREAM_LOWEST)](cmData);
    }
    else if ((id <= CSR_BT_CM_DM_PRIM_DOWNSTREAM_HIGHEST) &&
             (id != CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ))
    {
        /* Defer to DM provider */
        CsrBtCmDmProvider(cmData);
    }
    else if (id == CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ)
    {
        /* Defer to SM provider, but avoid locking */
        CsrBtCmServiceManagerL2caConnectAcceptProvider(cmData);
    }
    else if (id == CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ)
    {
        /* Defer to SM provider, but avoid locking */
        CsrBtCmServiceManagerL2caCancelConnectAcceptProvider(cmData);
    }
    else if (id == CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ)
    {
        /* Defer to SM provider, but avoid locking */
        CsrBtCmServiceManagerCancelAcceptConnectProvider(cmData);
    }
    else if ((id >= CSR_BT_CM_SM_PRIM_DOWNSTREAM_LOWEST) &&
             (id <= CSR_BT_CM_SM_PRIM_DOWNSTREAM_HIGHEST))
    {
        /* Defer to SM provider */
        CsrBtCmServiceManagerProvider(cmData);
    }
#ifdef CSR_BT_INSTALL_CM_PRI_IAC
    else if (id == CSR_BT_CM_WRITE_IAC_IND)
    {
        CsrBtCmWriteIacIndHandler(cmData);
    }
#endif
    else
    {
        /* Event error */
        CsrBtCmStateEventException(CSR_BT_CM_PRIM, id, cmData->globalState);
        if(id >= CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)
        {
            CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, cmData->recvMsgP);
        }
        else
        {
            CsrBtCmFreeDownstreamMessageContents(CSR_BT_CM_PRIM, cmData->recvMsgP);
        }
    }
}

