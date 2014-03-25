/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_panic.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#include "csr_bt_spp_main.h"
#include "csr_bt_spp_prim.h"
#include "csr_bt_spp_sef.h"
#include "sds_prim.h"

#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"

void CsrBtSppSaveMessage(SppInstanceData_t *instData)
{
    CsrMessageQueuePush(&instData->saveQueue, CSR_BT_SPP_PRIM, instData->recvMsgP);
    instData->recvMsgP = NULL;
}

/* ---------- SPP jump table ----------*/
static const SppStateHandlerType sppStateHandlers[SppNumberOfStates][CSR_BT_SPP_PRIM_DOWNSTREAM_COUNT] =
{
    /* init_s */
    {
        CsrBtSppSaveMessage,                                             /* CSR_BT_SPP_CONNECT_REQ */
        CsrBtSppSaveMessage,                                             /* CSR_BT_SPP_ACTIVATE_REQ */
        CsrBtSppSaveMessage,                                             /* CSR_BT_SPP_DEACTIVATE_REQ */
        CsrBtSppSaveMessage,                                             /* CSR_BT_SPP_DISCONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_DATA_REQ */
        NULL,                                                            /* CSR_BT_SPP_DATA_RES */
        NULL,                                                            /* CSR_BT_SPP_CONTROL_REQ */
        NULL,                                                            /* CSR_BT_SPP_PORTNEG_RES */
        CsrBtSppXStatePortnegReqHandler,                                 /* CSR_BT_SPP_PORTNEG_REQ */
        NULL,                                                            /* CSR_BT_SPP_SERVICE_NAME_RES */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_ACCEPT_AUDIO_REQ */
        NULL,                                                            /* CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_DISCONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_MODE_CHANGE_REQ */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppSaveMessage,                                             /* CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */
#else
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        CsrBtSppAnyStateRegisterDataPathHandleReqHandler,                /* CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ */
        NULL,                                                            /* CSR_BT_SPP_DATA_PATH_STATUS_REQ */
        CsrBtSppSaveMessage,                                             /* SPP_GET_INSTANCE_QID_REQ */
        CsrBtSppAllStateSppRegisterQIDReqHandler,                        /* CSR_BT_SPP_REGISTER_QID_REQ */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ */
        CsrBtSppInitStateSppCancelConnectReqHandler,                     /* CSR_BT_SPP_CANCEL_CONNECT_REQ */
        CsrBtSppSecurityInReqHandler,                                    /* CSR_BT_SPP_SECURITY_IN_REQ */
        CsrBtSppSecurityOutReqHandler,                                   /* CSR_BT_SPP_SECURITY_OUT_REQ */
    },
    /* idle_s */
    {
        CsrBtSppIdleStateSppConnectReqHandler,                           /* CSR_BT_SPP_CONNECT_REQ */
        CsrBtSppIdleStateSppActivateReqHandler,                          /* CSR_BT_SPP_ACTIVATE_REQ */
        CsrBtSppIdleStateSppDeactivateReqHandler,                        /* CSR_BT_SPP_DEACTIVATE_REQ */
        CsrBtSppIgnoreMessageHandler,                                    /* CSR_BT_SPP_DISCONNECT_REQ */
        CsrBtSppNotConnectedSppDataReqHandler,                           /* CSR_BT_SPP_DATA_REQ */
        CsrBtSppIgnoreMessageHandler,                                    /* CSR_BT_SPP_DATA_RES */
        NULL,                                                            /* CSR_BT_SPP_CONTROL_REQ */
        CsrBtSppConnectedStateSppPortnegResHandler,                      /* CSR_BT_SPP_PORTNEG_RES */
        CsrBtSppXStatePortnegReqHandler,                                 /* CSR_BT_SPP_PORTNEG_REQ */
        CsrBtSppIdleStateSppServiceNameResHandler,                       /* CSR_BT_SPP_SERVICE_NAME_RES */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_ACCEPT_AUDIO_REQ */
        NULL,                                                            /* CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_DISCONNECT_REQ */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppIdleStateSppExtendedConnectReqHandler,                   /* CSR_BT_SPP_EXTENDED_CONNECT_REQ */
        CsrBtSppIdleStateSppExtendedUuidConnectReqHandler,               /* CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */
#else
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        NULL,                                                            /* CSR_BT_SPP_MODE_CHANGE_REQ */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppIdleStateSppExtendedActivateReqHandler,                  /* CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */
#else
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        CsrBtSppAnyStateRegisterDataPathHandleReqHandler,                /* CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ */
        NULL,                                                            /* CSR_BT_SPP_DATA_PATH_STATUS_REQ */
        CsrBtSppAllStateSppGetInstancesQIDReqHandler,                    /* CSR_BT_SPP_GET_INSTANCES_QID_REQ */
        CsrBtSppAllStateSppRegisterQIDReqHandler,                        /* CSR_BT_SPP_REGISTER_QID_REQ */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ */
        CsrBtSppXStateSppCancelConnectReqHandler,                        /* CSR_BT_SPP_CANCEL_CONNECT_REQ */
        CsrBtSppSecurityInReqHandler,                                    /* CSR_BT_SPP_SECURITY_IN_REQ */
        CsrBtSppSecurityOutReqHandler,                                   /* CSR_BT_SPP_SECURITY_OUT_REQ */
    },
    /* activated_s */
    {
        CsrBtSppActivateStateSppConnectReqHandler,                       /* CSR_BT_SPP_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_ACTIVATE_REQ */
        CsrBtSppActivateStateSppDeactivateReqHandler,                    /* CSR_BT_SPP_DEACTIVATE_REQ */
        NULL,                                                            /* CSR_BT_SPP_DISCONNECT_REQ */
        CsrBtSppNotConnectedSppDataReqHandler,                           /* CSR_BT_SPP_DATA_REQ */
        NULL,                                                            /* CSR_BT_SPP_DATA_RES */
        NULL,                                                            /* CSR_BT_SPP_CONTROL_REQ */
        CsrBtSppConnectedStateSppPortnegResHandler,                      /* CSR_BT_SPP_PORTNEG_RES */
        CsrBtSppXStatePortnegReqHandler,                                 /* CSR_BT_SPP_PORTNEG_REQ */
        NULL,                                                            /* CSR_BT_SPP_SERVICE_NAME_RES */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_ACCEPT_AUDIO_REQ */
        NULL,                                                            /* CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_DISCONNECT_REQ */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppActivateStateSppExtendedConnectReqHandler,               /* CSR_BT_SPP_EXTENDED_CONNECT_REQ */
        CsrBtSppActivateStateSppExtendedUuidConnectReqHandler,           /* CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */
#else
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        NULL,                                                            /* CSR_BT_SPP_MODE_CHANGE_REQ */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppDummyStateSppActivateReqHandler,                         /* CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */
#else
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        CsrBtSppAnyStateRegisterDataPathHandleReqHandler,                /* CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ */
        NULL,                                                            /* CSR_BT_SPP_DATA_PATH_STATUS_REQ */
        CsrBtSppAllStateSppGetInstancesQIDReqHandler,                    /* CSR_BT_SPP_GET_INSTANCES_QID_REQ */
        CsrBtSppAllStateSppRegisterQIDReqHandler,                        /* CSR_BT_SPP_REGISTER_QID_REQ */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ */
        CsrBtSppXStateSppCancelConnectReqHandler,                        /* CSR_BT_SPP_CANCEL_CONNECT_REQ */
        CsrBtSppSecurityInReqHandler,                                    /* CSR_BT_SPP_SECURITY_IN_REQ */
        CsrBtSppSecurityOutReqHandler,                                   /* CSR_BT_SPP_SECURITY_OUT_REQ */
    },
    /* connected_s */
    {
        NULL,                                                            /* CSR_BT_SPP_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_ACTIVATE_REQ */
        CsrBtSppConnectedStateSppDeactivateReqHandler,                   /* CSR_BT_SPP_DEACTIVATE_REQ */
        CsrBtSppConnectedStateSppDisconnectReqHandler,                   /* CSR_BT_SPP_DISCONNECT_REQ */
        CsrBtSppConnectedStateSppDataReqHandler,                         /* CSR_BT_SPP_DATA_REQ */
        CsrBtSppConnectedStateSppDataResHandler,                         /* CSR_BT_SPP_DATA_RES */
        CsrBtSppConnectedStateSppControlReqHandler,                      /* CSR_BT_SPP_CONTROL_REQ */
        CsrBtSppConnectedStateSppPortnegResHandler,                      /* CSR_BT_SPP_PORTNEG_RES */
        CsrBtSppXStatePortnegReqHandler,                                 /* CSR_BT_SPP_PORTNEG_REQ */
        NULL,                                                            /* CSR_BT_SPP_SERVICE_NAME_RES */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppConnectedStateSppAudioReqHandler,                        /* CSR_BT_SPP_AUDIO_CONNECT_REQ */
        CsrBtSppConnectedStateSppAcceptAudioReqHandler,                  /* CSR_BT_SPP_ACCEPT_AUDIO_REQ */
        CsrBtSppConnectedStateSppCancelAcceptAudioReqHandler,            /* CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ */
        CsrBtSppConnectedStateSppAudioReleaseReqHandler,                 /* CSR_BT_SPP_AUDIO_DISCONNECT_REQ */
#else
        NULL,                                                            /* CSR_BT_SPP_AUDIO_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_ACCEPT_AUDIO_REQ */
        NULL,                                                            /* CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_DISCONNECT_REQ */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */
        CsrBtSppConnectedStateSppModeChangeReqHandler,                   /* CSR_BT_SPP_MODE_CHANGE_REQ */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppDummyStateSppActivateReqHandler,                         /* CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */
#else
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        CsrBtSppAnyStateRegisterDataPathHandleReqHandler,                /* CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ */
        CsrBtSppConnectedStateSppDataPathStatusReqHandler,               /* CSR_BT_SPP_DATA_PATH_STATUS_REQ */
        CsrBtSppAllStateSppGetInstancesQIDReqHandler,                    /* CSR_BT_SPP_GET_INSTANCES_QID_REQ */
        CsrBtSppAllStateSppRegisterQIDReqHandler,                        /* CSR_BT_SPP_REGISTER_QID_REQ */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppMapScoPcmResHandler,                                     /* CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */
        CsrBtSppConnectedStateSppAudioRenegotiateReqHandler,             /* CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ */
#else
        NULL,                                                            /* CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        CsrBtSppConnectedStateSppCancelConnectReqHandler,                /* CSR_BT_SPP_CANCEL_CONNECT_REQ */
        CsrBtSppSecurityInReqHandler,                                    /* CSR_BT_SPP_SECURITY_IN_REQ */
        CsrBtSppSecurityOutReqHandler,                                   /* CSR_BT_SPP_SECURITY_OUT_REQ */
    },
    /* deactivate_s */
    {
        NULL,                                                            /* CSR_BT_SPP_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_ACTIVATE_REQ */
        NULL,                                                            /* CSR_BT_SPP_DEACTIVATE_REQ */
        NULL,                                                            /* CSR_BT_SPP_DISCONNECT_REQ */
        CsrBtSppNotConnectedSppDataReqHandler,                           /* CSR_BT_SPP_DATA_REQ */
        NULL,                                                            /* CSR_BT_SPP_DATA_RES */
        NULL,                                                            /* CSR_BT_SPP_CONTROL_REQ */
        NULL,                                                            /* CSR_BT_SPP_PORTNEG_RES */
        CsrBtSppXStatePortnegReqHandler,                                 /* CSR_BT_SPP_PORTNEG_REQ */
        NULL,                                                            /* CSR_BT_SPP_SERVICE_NAME_RES */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_ACCEPT_AUDIO_REQ */
        NULL,                                                            /* CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_DISCONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */
        NULL,                                                            /* CSR_BT_SPP_MODE_CHANGE_REQ */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppDummyStateSppActivateReqHandler,                         /* CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */
#else
        NULL,                                                            /* CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        CsrBtSppAnyStateRegisterDataPathHandleReqHandler,                /* CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ */
        NULL,                                                            /* CSR_BT_SPP_DATA_PATH_STATUS_REQ */
        CsrBtSppAllStateSppGetInstancesQIDReqHandler,                    /* CSR_BT_SPP_GET_INSTANCES_QID_REQ */
        CsrBtSppAllStateSppRegisterQIDReqHandler,                        /* CSR_BT_SPP_REGISTER_QID_REQ */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppMapScoPcmResHandler,                                     /* CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */
#else
        NULL,                                                            /* CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        NULL,                                                            /* CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ */
        CsrBtSppDeactivateStateSppCancelConnectReqHandler,               /* CSR_BT_SPP_CANCEL_CONNECT_REQ */
        CsrBtSppSecurityInReqHandler,                                    /* CSR_BT_SPP_SECURITY_IN_REQ */
        CsrBtSppSecurityOutReqHandler,                                   /* CSR_BT_SPP_SECURITY_OUT_REQ */
    }
};

/* ---------- CM jump table ---------- */
static const SppStateHandlerType cmStateHandlers[SppNumberOfStates][CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT] =
{
    /* init_s */
    {
        NULL,                                                            /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtSppInitStateCmRegisterCfmHandler,                           /* CSR_BT_CM_REGISTER_CFM */
        NULL,                                                            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                            /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                            /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtSppNotConnectedCmDataIndHandler,                            /* CSR_BT_CM_DATA_IND */
        NULL,                                                            /* CSR_BT_CM_DATA_CFM */
        NULL,                                                            /* CSR_BT_CM_CONTROL_IND */
        NULL,                                                            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                            /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                            /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* idle_s */
    {
        CsrBtSppIdleStateCmCancelAcceptConnectCfmHandler,                /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtSppInitStateCmRegisterCfmHandler,                           /* CSR_BT_CM_REGISTER_CFM */
        CsrBtSppIdleStateCmDisconnectIndHandler,                         /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                            /* CSR_BT_CM_SCO_CONNECT_CFM */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppIgnoreMessageHandler,                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
#else
        NULL,                                                            /* CSR_BT_CM_SCO_DISCONNECT_IND */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        NULL,                                                            /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtSppNotConnectedCmDataIndHandler,                            /* CSR_BT_CM_DATA_IND */
        NULL,                                                            /* CSR_BT_CM_DATA_CFM */
        NULL,                                                            /* CSR_BT_CM_CONTROL_IND */
        NULL,                                                            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                            /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                            /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* activate_s */
    {
        CsrBtSppActivateStateCmCancelAcceptConnectCfmHandler,            /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_CONNECT_CFM */
        CsrBtSppDeOrActivatedStateCmConnectAcceptCfmHandler,             /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                            /* CSR_BT_CM_REGISTER_CFM */
        NULL,                                                            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                            /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                            /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtSppNotConnectedCmDataIndHandler,                            /* CSR_BT_CM_DATA_IND */
        NULL,                                                            /* CSR_BT_CM_DATA_CFM */
        NULL,                                                            /* CSR_BT_CM_CONTROL_IND */
        NULL,                                                            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                            /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                            /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* connected_s */
    {
        NULL,                                                            /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                            /* CSR_BT_CM_REGISTER_CFM */
        CsrBtSppConnectedStateCmDisconnectIndHandler,                    /* CSR_BT_CM_DISCONNECT_IND */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppConnectedStateCmScoConnectCfmHandler,                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        CsrBtSppConnectedStateCmScoDisconnectIndHandler,                 /* CSR_BT_CM_SCO_DISCONNECT_IND */
        CsrBtSppConnectedStateCmScoAcceptConnectCfmHandler,              /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
#else
        NULL,                                                            /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                            /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        CsrBtSppConnectedStateCmDataIndHandler,                          /* CSR_BT_CM_DATA_IND */
        CsrBtSppConnectedStateCmDataCfmHandler,                          /* CSR_BT_CM_DATA_CFM */
        CsrBtSppConnectedStateCmControlIndHandler,                       /* CSR_BT_CM_CONTROL_IND */
        CsrBtSppConnectedStateCmModeChangeIndHandler,                    /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                            /* CSR_BT_CM_PORTNEG_IND */
        CsrBtCppConnectedStateCmPortnegCfmHandler,                       /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* deactivate_s */
    {
        CsrBtSppDeactivateStateCmCancelAcceptConnectCfmHandler,          /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_CONNECT_CFM */
        CsrBtSppDeOrActivatedStateCmConnectAcceptCfmHandler,             /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                            /* CSR_BT_CM_REGISTER_CFM */
        CsrBtSppDeactivateStateCmDisconnectIndHandler,                   /* CSR_BT_CM_DISCONNECT_IND */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        CsrBtSppDeactivateStateCmScoXHandler,                            /* CSR_BT_CM_SCO_CONNECT_CFM */
        CsrBtSppDeactivateStateCmScoXHandler,                            /* CSR_BT_CM_SCO_DISCONNECT_IND */
        CsrBtSppDeactivateStateCmScoXHandler,                            /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
#else
        NULL,                                                            /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                            /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                            /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        CsrBtSppNotConnectedCmDataIndHandler,                            /* CSR_BT_CM_DATA_IND */
        NULL,                                                            /* CSR_BT_CM_DATA_CFM */
        NULL,                                                            /* CSR_BT_CM_CONTROL_IND */
        NULL,                                                            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                            /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                            /* CSR_BT_CM_PORTNEG_CFM */
    }
};
/* ---------- End of jump tables ---------- */

static void csrBtSppInitCommon(SppInstanceData_t  *sppData)
{
    /* clear own instance data for first time use */
    sppData->sppConnId = SPP_NO_CONNID;

    CsrBtScSetSecInLevel(&sppData->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_SERIAL_PORT_MANDATORY_SECURITY_INCOMING,
        CSR_BT_SERIAL_PORT_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_SPP_SUCCESS,
        CSR_BT_RESULT_CODE_SPP_UNACCEPTABLE_PARAMETER);

    CsrBtScSetSecOutLevel(&sppData->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_SERIAL_PORT_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_SERIAL_PORT_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_SPP_SUCCESS,
        CSR_BT_RESULT_CODE_SPP_UNACCEPTABLE_PARAMETER);

    /* Both control and data app handles must be invalid */
    sppData->ctrlHandle             = CSR_SCHED_QID_INVALID;
    sppData->dataHandle             = CSR_SCHED_QID_INVALID;

    /* Common lib search stuff */
    sppData->sdpSppSearchConData    = CsrBtUtilSdpRfcConInit(CsrBtSppSdcSelectServiceHandler,
        NULL, CsrBtSppRfcSdcConResultHandler, sppData->myAppHandle);
    sppData->modemStatus            = CSR_BT_MODEM_SEND_CTRL_DTE_DEFAULT;
    sppData->breakSignal            = CSR_BT_DEFAULT_BREAK_SIGNAL;
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
    sppData->amConnId               = CSR_AM_NO_CONN_ID;
    CsrAmInitReqSend(CSR_BT_SPP_IFACEQUEUE);
    sppData->amSppCallBack          = CsrSppAmInitCfm;
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */

    if (sppData->myAppHandle == CSR_BT_SPP_IFACEQUEUE)
    {
        /* This is the SPP manager */
        sppData->sppInstances = CsrPmemZalloc(sizeof(SppInstancesPool_t));
    }
    else
    {
        /* This is one of the extra instances */
        sppData->sppInstances = NULL;
    }

     /* Tell the SD that it must look for the CSR_BT_SPP_PROFILE_UUID
        service, when it perform a SD_READ_AVAILABLE_SERVICE_REQ    */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_SPP_PROFILE_UUID);

    /* send a register to CM to allocate the internal server channel number for the client role. */
    CsrBtCmRegisterReqSend(sppData->myAppHandle);
}


/* define ten init functions to support up to ten spp profiles simultaneously. Only the
   init functions are needed to be multiplied, i.e. the same message handler function is used. */
void CsrBtSppInit(void **gash)
{
    SppInstanceData_t  *sppData;
    CsrBtSppRegisterQidReq *sppPrim;

    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemZalloc(sizeof(SppInstanceData_t));
    sppData = (SppInstanceData_t *) *gash;
    sppData->myAppHandle = CsrSchedTaskQueueGet();

    /* init the instance data */
    csrBtSppInitCommon(sppData);

    sppPrim          = (CsrBtSppRegisterQidReq *)CsrPmemAlloc(sizeof(CsrBtSppRegisterQidReq));
    sppPrim->type    = CSR_BT_SPP_REGISTER_QID_REQ;
    sppPrim->phandle = sppData->myAppHandle;
    CsrBtSppMessagePut(CSR_BT_SPP_IFACEQUEUE, sppPrim);
}


#ifdef ENABLE_SHUTDOWN
void releaseMessage(CsrUint16 msg_type, void *msg_data)
{
    switch (msg_type)
    {
        case CSR_BT_CM_PRIM:
        {
            CsrBtCmFreeUpstreamMessageContents(msg_type, msg_data);
            break;
        }
        case CSR_BT_SPP_PRIM:
        {
            CsrBtSppFreeDownstreamMessageContents(msg_type, msg_data);
            break;
        }

        default:
            {
                break;
            }
    }
}

/****************************************************************************
    This function is called by the scheduler to perform a graceful shutdown
    of a scheduler task.
    This function must:
    1)    empty the input message queue and free any allocated memory in the
        messages.
    2)    free any instance data that may be allocated.
****************************************************************************/
void CsrBtSppDeinit(void **gash)
{
    CsrUint16              msg_type=0;
    void                *msg_data=NULL;
    CsrBool              lastMsg;
    SppInstanceData_t   *sppData;
    SppInstancesPool_t  *ptr;
    SppInstancesPool_t  *next;

    sppData = (SppInstanceData_t *) *gash;

    CsrBtUtilSdcRfcDeinit(&(sppData->sdpSppSearchConData));

    /* continue to poll any message of the input queue */
    lastMsg = FALSE;

    while (!lastMsg)
    {
        if (!CsrMessageQueuePop(&sppData->saveQueue, &msg_type, &msg_data))
        {
            lastMsg = !CsrSchedMessageGet(&msg_type, &msg_data);
        }

        if (!lastMsg)
        {
            releaseMessage(msg_type, msg_data);
            CsrPmemFree (msg_data);
        }
    }

#ifdef CSR_BT_INSTALL_SPP_EXTENDED
    if(sppData->serviceRecordExt)
    {
        CsrPmemFree(sppData->serviceRecord);
    }
    else
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
    {
        CsrPmemFree(sppData->serviceName);
    }
    CsrPmemFree(sppData->sdpServiceNameList);
    CsrPmemFree(sppData->serviceHandleList);

    if (sppData->sppInstances)
    {
        ptr = sppData->sppInstances;
        next = NULL;

        while(ptr)
        {
            next = ptr->next;
            CsrPmemFree(ptr);
            ptr = next;
        }
    }
    else
    {
        /* Do nothing */
    }

    CsrPmemFree(sppData);

}
#endif


static void CsrBtSppRaiseException(SppInstanceData_t *sppData, CsrUint16 primType, CsrUint16 eventClass)
{
    switch(eventClass)
    {
        case CSR_BT_SPP_PRIM:
        {
            #ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("SPP", sppData->state,
                                 primType, "Unknown SPP prim or undefined state");
            #endif
            CsrBtSppFreeDownstreamMessageContents(eventClass, sppData->recvMsgP);
            break;
        }
        case CSR_BT_CM_PRIM:
        {
            #ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("SPP", sppData->state,
                                 primType, "Unknown CM prim or undefined state");
            #endif
            CsrBtCmFreeUpstreamMessageContents(eventClass, sppData->recvMsgP);
            break;
        }
        case CSR_AM_PRIM:
        {
            #ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("SPP", sppData->state,
                                 primType, "Unknown AM prim or no callback active");
            #endif
            break;
        }
        default:
        {
            #ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("SPP",
                                     sppData->state,
                                     0xFF,
                                     "Unknown primitive type received in main");
            #endif
            break;
        }
    }
}


void CsrBtSppHandler(void **gash)
{
    SppInstanceData_t    *sppData;
    CsrUint16            eventClass=0;
    void                *msg=NULL;

    sppData = (SppInstanceData_t *) (*gash);

    if(!sppData->restoreSppFlag)
    {
        CsrSchedMessageGet(&eventClass , &msg);
    }
    else
    {
        if(!CsrMessageQueuePop(&sppData->saveQueue, &eventClass, &msg))
        {
            sppData->restoreSppFlag = FALSE;
            CsrSchedMessageGet(&eventClass , &msg);
        }
    }
    sppData->recvMsgP = msg;

    switch(eventClass)
    {
        case CSR_BT_SPP_PRIM:
            {
                CsrBtSppPrim         *primType;
                /* find the message type */
                primType = (CsrBtSppPrim *)msg;
                if ((*primType < CSR_BT_SPP_PRIM_DOWNSTREAM_COUNT) && sppStateHandlers[sppData->state][*primType] != NULL)
                {
                    sppStateHandlers[sppData->state][*primType](sppData);
                }
                else if (*primType != CSR_BT_SPP_HOUSE_CLEANING)
                {  /* State/Event ERROR! */
                    CsrBtSppRaiseException(sppData,*primType,eventClass);
                }
                break;
            }

        case CSR_BT_CM_PRIM:
            {
                CsrPrim *primType;
                /* find the message type */
                primType = (CsrPrim *)msg;

                if(((CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST) < CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT) &&
                   (cmStateHandlers[sppData->state][(CsrUint16) (*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)] != NULL))
                {
                    cmStateHandlers[sppData->state][(CsrUint16) (*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)](sppData);
                }
                else
                {
                    switch(*primType)
                    {
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
                        case CSR_BT_CM_SCO_RENEGOTIATE_CFM:
                        {
                            CsrBtSppCmScoRenegotiateCfmHandler(sppData);
                            break;
                        }
                        case CSR_BT_CM_SCO_RENEGOTIATE_IND:
                        {
                            if (sppData->state == Connected_s)
                            {
                                CsrBtSppCmScoRenegotiateIndHandler(sppData);
                            }
                            else
                            {
                                ;
                            }
                            break;
                        }
                        case CSR_BT_CM_MAP_SCO_PCM_IND:
                        {
                            CsrBtSppCmMapScoPcmHandler(sppData);
                            break;
                        }
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
                        case CSR_BT_CM_SDS_UNREGISTER_CFM:
                        {
                            switch(sppData->state)
                            {
                                case Idle_s:
                                case Connected_s:
                                {
                                    CsrBtSppIdleOrConnectedStateSdsUnregisterCfmHandler(sppData);
                                    break;
                                }
                                case Activated_s:
                                {
                                    CsrBtSppActivatedStateSdsUnregisterCfmHandler(sppData);
                                    break;
                                }
                                case Deactivate_s:
                                {
                                    CsrBtSppDeactivateStateSdsUnregisterCfmHandler(sppData);
                                    break;
                                }
                                default:
                                {
                                    CsrBtSppRaiseException(sppData,*primType,eventClass);
                                    break;
                                }

                            }
                            break;
                        }
                        case CSR_BT_CM_SDS_REGISTER_CFM:
                        {
                            if ((sppData->state == Init_s) || (sppData->state == Idle_s))
                            {
                                CsrBtSppInitStateSdsRegisterCfmHandler(sppData);
                            }
                            else
                            {
                                CsrBtSppRaiseException(sppData,*primType,eventClass);
                            }
                            break;
                        }
                        case CSR_BT_CM_PORTNEG_IND:
                        {
                            if ((sppData->state == Connected_s) || (sppData->state == Activated_s))
                            {
                                CsrBtSppConnectedStateCmPortnegIndHandler(sppData);
                                break;
                            }
                            /*else no break intended!*/
                        }
                        case CSR_BT_CM_SDC_UUID128_SEARCH_IND:
                        case CSR_BT_CM_SDC_ATTRIBUTE_CFM:
                        case CSR_BT_CM_SDC_CLOSE_IND:
                        case CSR_BT_CM_SDC_SEARCH_CFM:
                        case CSR_BT_CM_SDC_SEARCH_IND:
                        {
                            if ((sppData->state == Idle_s) && (CsrBtUtilRfcConVerifyCmMsg(sppData->recvMsgP)))
                            {
                                CsrBtUtilRfcConCmMsgHandler(sppData, sppData->sdpSppSearchConData, sppData->recvMsgP);
                            }
                            else
                            {
                                CsrBtSppRaiseException(sppData,*primType,eventClass);
                            }
                            break;
                        }
                        case CSR_BT_CM_CONNECT_CFM:
                        case CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM:
                        {
                            if (CsrBtUtilRfcConVerifyCmMsg(sppData->recvMsgP))
                            {
                                CsrBtUtilRfcConCmMsgHandler(sppData, sppData->sdpSppSearchConData, sppData->recvMsgP);
                            }
                            break;
                        }
                        default:
                        {
                            CsrBtSppRaiseException(sppData,*primType,eventClass);
                            break;
                        }
                    }
                }
                break;
            }

        case CSR_SCHED_PRIM:
            {
                /* Environment cleanup */
                CsrEnvPrim *primType;
                primType = (CsrEnvPrim*)msg;
                if(*primType == CSR_CLEANUP_IND)
                {
                    CsrBtSppEnvironmentCleanupHandler(sppData);
                }
                break;
            }
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
        case CSR_AM_PRIM:
            {
                if (sppData->amSppCallBack != NULL)
                {
                    sppData->amSppCallBack(sppData);
                }
                else
                {
                    CsrAmPrim *primType = (CsrAmPrim *)msg;
                    CsrBtSppRaiseException(sppData,*primType,eventClass);
                }
                break;
            }
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
        default:
            {  /* State/Event ERROR! */
                CsrBtSppRaiseException(sppData,0xFFFF,eventClass);
                break;
            }
    }

    /* free the received message. if the ptr is NULL then CsrSched just ignores */
    CsrPmemFree(sppData->recvMsgP);
}

