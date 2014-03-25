#ifndef CSR_BT_CM_DM_H__
#define CSR_BT_CM_DM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_bt_cm_main.h"
#include "dmlib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NO_SCO                  (0xffff)
#define SCOBUSY_ACCEPT          (0xfffe)

#define RFC_SCO_CONNECT         (0x0000)
#define INCOMING_SCO_CON        (0x0200)

typedef struct {
    hci_pkt_type_t          packetType;
    CsrUint8                 featureBit;
} cmHciPacketFeatureType_t;

/*----------------------------------------------------------------------------*
 *
 *   Condition for Connection_Setup_Filter_Condition_Type
 *
 *---------------------------------------------------------------------------*/
#define HCI_COND_SETUP_AUTO_ACCEPT_FLAG_OFF          ((filter_condition_type_t)0x01)
#define HCI_COND_SETUP_AUTO_ACCEPT_FLAG_RS_OFF       ((filter_condition_type_t)0x02)
#define HCI_COND_SETUP_AUTO_ACCEPT_FLAG_RS_ON        ((filter_condition_type_t)0x03)



/* Used for Park link policy settings. Currently it is reserved for future 
   use and must be set to 0                                                     */
#define CSR_BT_PARK_IDLE_TIME                               0x00000000

/* Time allowed to be discoverable during inquiry */
#ifndef SCAN_TIMER_DEFAULT
#define SCAN_TIMER_DEFAULT                                      (1000)
#endif /* SCAN_TIMER_DEFAULT */

/* Time to do inquiry when wanting discoverability also */
#ifndef HCI_INQUIRY_LENGTH_SHORTENED
#define HCI_INQUIRY_LENGTH_SHORTENED                            (5)
#endif /* HCI_INQUIRY_LENGTH_SHORTENED */


/*************************************************************************************
 These function are found under csr_bt_cm_dm_provider.c
************************************************************************************/
void CsrBtCmDmRestoreQueueHandler(cmInstanceData_t *cmData);
void CsrBtCmDmLocalQueueHandler(void);
void CsrBtCmDmProvider(cmInstanceData_t *cmData);
CsrBool cancelDmMsg(cmInstanceData_t *cmData, CsrBtCmPrim type, CsrSchedQid phandle, CsrBtDeviceAddr bd_addr);

/*************************************************************************************
 These function are found under cm_dm_handler.c
************************************************************************************/
void cmDmHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_arrival_handler.c
************************************************************************************/
void CsrBtCmDmArrivalHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_maintenance_handler.c
************************************************************************************/
void CsrBtCmDmLockQueue(cmInstanceData_t *cmData);
void CsrBtCmDmUnlockQueue(cmInstanceData_t *cmData);
void CsrBtCmSmUnlockQueue(cmInstanceData_t *cmData);
void CsrBtCmSmLockQueue(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under cm_dm_connect_able_handler.c
************************************************************************************/
void CsrBtCmDmHciWriteClassOfDeviceCompleteHandler(cmInstanceData_t *cmData);
#ifdef CSR_BT_INSTALL_CM_WRITE_COD
void CsrBtCmWriteCodReqHandler(cmInstanceData_t *cmData);
#endif
#ifdef CSR_BT_INSTALL_CM_READ_COD
void CsrBtCmReadCodReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciReadClassOfDeviceCompleteHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmDmHciWriteScanEnableCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmWriteScanEnableCompleteSwitch(cmInstanceData_t *cmData, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtCmRfcCancelConnectAcceptReqHandler(cmInstanceData_t *cmData);
void CsrBtCmWriteClassOfDeviceReqHandler(cmInstanceData_t *cmData);
void CsrBtCmWriteClassOfDevice(CsrUint8 thePlayer);
void CsrBtCmWriteScanEnableReqHandler(cmInstanceData_t *cmData);
void CsrBtCmConnectAbleReqHandler(cmInstanceData_t *cmData);
#ifdef CSR_BT_INSTALL_CM_READ_SCAN_EANBLE
void CsrBtCmReadScanEnableReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciReadScanEnableCompleteHandler(cmInstanceData_t *cmData);
#endif
/*************************************************************************************
 These function are found under csr_bt_cm_dm_sco_handler.c
************************************************************************************/
void CsrBtCmDmScoCancelAcceptConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmScoAcceptConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmScoDisconnectReqHandler(cmInstanceData_t *cmData);
#ifdef CSR_BT_INSTALL_CM_PRI_SCO_RENEGOTIATE
void CsrBtCmDmScoRenegotiateReqHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmDmSyncRenegotiateIndHandler(cmInstanceData_t *cmData);
#ifdef CSR_BT_INSTALL_CM_PRI_SCO_RENEGOTIATE
void CsrBtCmDmSyncRenegotiateCfmHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmDmSyncDisconnectIndHandler(cmInstanceData_t *cmData);
void CsrBtCmDmSyncDisconnectCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmDmSyncConnectCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmDmSyncConnectIndHandler(cmInstanceData_t *cmData);
void CsrBtCmDmScoConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmSyncConnectCompleteIndHandler(cmInstanceData_t *cmData);
void CsrBtCmDmSyncRegisterCfmHandler(cmInstanceData_t *cmData);
CsrUint8 returnNumberOfScoConnection(cmInstanceData_t *cmData);
void CsrBtCmBccmdMapScoPcmReqSend(CsrUint8 pcmSlot, CsrUint16 seqNo);
void CsrBtCmBccmdMapScoPcmCfmHandler(cmInstanceData_t *cmData);
CsrUint8 CsrBtCmDmFindFreePcmSlot(cmInstanceData_t *cmData);
void CsrBtCmIncomingScoReqHandler(cmInstanceData_t *cmData);
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
void CsrBtCmMapScoPcmResHandler(cmInstanceData_t *cmData);
#else
#define CsrBtCmMapScoPcmResHandler NULL
#endif
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
void CsrBtCmL2caMapScoPcmResHandler(cmInstanceData_t *cmData);
#else
#define CsrBtCmL2caMapScoPcmResHandler NULL
#endif
void CsrBtCmBccmdGetScoParametersCfmHandler(cmInstanceData_t *cmData);

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
void CsrBtCmScoFreePacketTypeArray(cmSyncNegotiationCntType_t **negotiateCnt);
void CsrBtCmDmSyncClearPcmSlotFromTable(cmInstanceData_t *cmData, eScoParmVars *eScoParms);
#else
#define CsrBtCmScoFreePacketTypeArray(negotiateCnt)
#define CsrBtCmDmSyncClearPcmSlotFromTable(cmData, eScoParms)
#endif
CsrBool CsrBtCmDmIsEdrEsco(hci_pkt_type_t packetType);
CsrBool CsrBtCmDmIsSco(hci_pkt_type_t packetType);
void CsrBtCmScoSetEScoParms(eScoParmVars *escoParms, CsrBtCmScoCommonParms *parms, hci_connection_handle_t handle);
CsrBool CsrBtCmScoGetNextNegotiateParms(cmSyncNegotiationCntType_t *negotiateCnt, CsrBtCmScoCommonParms *parms);
CsrBool CsrBtCmScoCurrentNegotiateParmsIsSco(CsrBtCmScoCommonParms *parms);
CsrBool CsrBtCmScoGetCurrentNegotiateParms(cmSyncNegotiationCntType_t *negotiateCnt, CsrBtCmScoCommonParms *parms);
CsrBool CsrBtCmScoSeekToNextScoOnlyNegotiateParms(cmSyncNegotiationCntType_t *negotiateCnt);
CsrUint16 CsrBtCmScoCreatePacketTypeArray(cmInstanceData_t                 *cmData,
                                          CsrBtCmScoCommonParms     **primParms,
                                          CsrUint16                   primParmsLen,
                                          CsrBtDeviceAddr               deviceAddr,
                                          cmSyncNegotiationCntType_t **negotiateCnt);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_acl_detach_handler.c
************************************************************************************/
void CsrBtCmAclDetachReqHandler(cmInstanceData_t *cmData);
CsrBool CsrBtCmDmCancelPageOrDetach(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr);
void CsrBtCmDmAclDetachAclCloseHandler(cmInstanceData_t *cmData, aclTable *aclConnectionElement,
                                       CsrBtDeviceAddr deviceAddr, CsrUint8 reason);

/*************************************************************************************
 These function are found under csr_bt_cm_rfc_sco_handler.c
************************************************************************************/
void CsrBtCmDmScoConnectCfmMsgSend(CsrSchedQid               phandle,
                              CsrBtConnId            btConnId,
                              DM_SYNC_CONNECT_CFM_T  *dmPrim,
                              CsrUint8                pcmSlot,
                              CsrBtResultCode        resultCode,
                              CsrBtSupplier    resultSupplier);

/*************************************************************************************
 These function are found under csr_bt_cm_l2cap_sco_handler.c
************************************************************************************/
void CsrBtCmDmScoL2capConnectCfmMsgSend(CsrSchedQid                phandle,
                                    CsrBtConnId             btConnId,
                                    DM_SYNC_CONNECT_CFM_T  *dmPrim,
                                    CsrUint8                pcmSlot,
                                    CsrBtResultCode        resultCode,
                                    CsrBtSupplier    resultSupplier);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_read_local_bd_addr_handler.c
************************************************************************************/
void CsrBtCmDmHciReadBdAddrCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmReadBdAddrReqHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_write_link_super_visiontimeout.c
************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_WRITE_LINK_SUPERVISION_TIMEOUT
void CsrBtCmWriteLinkSuperVTimeoutReqHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmDmHciWriteLinkSuperVisionTimeoutCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciLinkSupervisionTimeoutIndHandler(cmInstanceData_t *cmData);
void CsrBtCmWriteDmLinkSuperVisionTimeoutHandler(cmInstanceData_t   *cmData,
                                                 CsrSchedQid             phandle,
                                                 CsrUint16          timeout,
                                                 CsrBtDeviceAddr    deviceAddr);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_write_page_to_handler.c
************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_WRITE_PAGE_TO
void CsrBtCmDmWritePageToReqHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmDmHciWritePageToCompleteHandler(cmInstanceData_t *cmData);
/*************************************************************************************
 These function are found under csr_bt_cm_dm_read_local_bd_addr_handler.c
************************************************************************************/
void CsrBtCmRfcPortNegCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcPortNegIndHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcPortNegResHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcPortnegReqHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_sc_handler.c
************************************************************************************/
void CsrBtCmGetSecurityConfIndSend(cmInstanceData_t *cmData, CsrUint8 lmpVersion);
void CsrBtCmDmSmInitCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmDmSmAccessIndHandler(cmInstanceData_t * cmData);
void CsrBtCmDmSmClearRebondData(cmInstanceData_t *cmData);
CsrBool CsrBtCmDmSmRebondNeeded(cmInstanceData_t *cmData);
void CsrBtCmDmAclOpenCfm(cmInstanceData_t *cmData,
                         BD_ADDR_T *p_bd_addr, CsrBool success);
void CsrBtCmSmDeleteStoreLinkKeyReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmSetDefaultSecLevelReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmLinkKeyRequestResHandler(cmInstanceData_t *cmData);
void CsrBtCmSmRemoveDeviceReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmAclOpenCfmScStateHandler(cmInstanceData_t * cmData);
void CsrBtCmSmAuthenticateReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmEncryptionReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmSetSecModeReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmUnRegisterReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmPinRequestResHandler(cmInstanceData_t *cmData);
void cmSmAddDeviceReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmAuthoriseResHandler(cmInstanceData_t *cmData);
void CsrBtCmSmRegisterReqHandler(cmInstanceData_t *cmData);
void CsrBtCmScRejectedForSecurityReasonMsgSend(cmInstanceData_t *cmData,
                                               CsrBtDeviceAddr theAddr,
                                               CsrBool cancelInitiated);
void CsrBtCmSmCancelConnectReqHandler(cmInstanceData_t *cmData);
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
void CsrBtCmReadEncryptionStatusReqHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmSmAclOpenReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmAclCloseReqHandler(cmInstanceData_t *cmData);
void cmEnEnableEnhancementsReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmInitReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmAuthoriseIndHandler(cmInstanceData_t *cmData);
void CsrBtCmScCleanupVar(cmInstanceData_t *cmData);
#ifdef CSR_BT_LE_ENABLE
void CsrBtCmSmKeyRequestResHandler(cmInstanceData_t *cmData);
#endif

/*************************************************************************************
 These function are found under csr_bt_cm_dm_set_local_name_handler.c
************************************************************************************/
void CsrBtCmDmHciChangeLocalNameCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmSetLocalNameReqHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_read_remote_name_handler.c
************************************************************************************/
void CsrBtCmReadRemoteNameCfmSend(cmInstanceData_t *cmData, CsrSchedQid phandle,
                      CsrBtDeviceAddr deviceAddr, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtCmReadRemoteNameSetup(cmInstanceData_t *cmData);
void CsrBtCmDmHciRemoteNameCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmReadRemoteNameReqHandler(cmInstanceData_t *cmData);
void CsrBtCmCancelReadRemoteNameReqHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_inquiry_handler.c
************************************************************************************/
void CsrBtCmStartInquiry(cmInstanceData_t *cmData);
void CsrBtCmInquiryReqHandler(cmInstanceData_t *cmData);
void CsrBtCmCancelInquiryReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciInquiryCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciInquiryCancelCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciInquiryResultHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciInquiryResultWithRssiHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciExtendedInquiryResultIndHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciWriteInquiryTransmitPowerLevelCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmBccmdInquiryPriorityCfmHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_mode_change_handler.c
************************************************************************************/
void CsrBtCmDmModeChangeReqMsgSend(CsrUint8 player, CsrUint8 theIndex, CsrUint8 modeRequest, CsrBtDeviceAddr deviceAddr);
void CsrBtCmDmStoreActualMode(cmInstanceData_t *cmData);
CsrUint8 CsrBtCmDmReturnActualMode(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr);
CsrUint8 CsrBtCmDmReturnRequestedMode(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr);
void CsrBtCmDmModeChangeReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciModeChangeEventHandler(cmInstanceData_t *cmData);
void CsrBtCmModeChangeConfigReqHandler(cmInstanceData_t *cmData);
void CsrBtCmModeChangeReqHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcModeChangeReqHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caModeChangeReqHandler(cmInstanceData_t *cmData);
void CsrBtCmBnepModeChangeReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciModeChangeReqMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr, CsrUint8 theNewMode, CsrUint8 actualMode);
void CsrBtCmRfcModeChangeIndMsgSend(cmRfcConnInstType * theLogicalLink, CsrUint16 length,
                                    CsrUint8 mode, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtCmL2caModeChangeIndMsgSend(cmL2caConnInstType *l2CaConnection, CsrUint16 length,
                                     CsrUint8 mode, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtCmDmHciModeChangeFromActive(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr, CsrUint8 newMode);
/*************************************************************************************
 These function are found under csr_bt_cm_dm_write_lp_settings_handler.c
************************************************************************************/
void CsrBtCmDmWriteLpSettingsReqMsgSendExt(CsrBtDeviceAddr deviceAddr, CsrUint8 player, link_policy_settings_t link_policy_settings);
void CsrBtCmDmWriteLpSettingsReqMsgSend(CsrBtDeviceAddr deviceAddr, CsrUint8 player);
void CsrBtCmDmHciWriteLpSettingsCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmWriteLpSettingsReqHandler(cmInstanceData_t *cmData);
void CsrBtCmWriteLinkPolicyReqHandler(cmInstanceData_t *cmData);
void CsrBtCmReadLinkPolicyReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciWriteDefaultLinkPolicySettingsCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmAlwaysMasterDevicesReqHandler(cmInstanceData_t *cmData);
void CsrBtCmAlwaysMasterDevicesCfmHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_maintenance_handler.c
************************************************************************************/
cmRfcConnElement * returnReserveScoIndexToThisAddress(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr);
CsrUint8 returnConnectAbleParameters(cmInstanceData_t *cmData);
link_policy_settings_t CsrBtCmDmReturnLinkPolicySettings(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr);
void CsrBtCmDmWriteLpSettingsAllMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, CsrUint8 player);
void CsrBtCmDmWriteLpSettingsOnlyLowPowerMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, CsrUint8 player);
void CsrBtCmDmWriteLpSettingsOnlyMssMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, CsrUint8 player);
void CsrBtCmDmWriteLpSettingsOverrideMssMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, CsrUint8 player, link_policy_settings_t mssSettings);
void CsrBtCmDmWriteLpSettingsForceLinkPolicyMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, CsrUint8 player, link_policy_settings_t linkPolicySettings);
#ifdef CSR_BT_INSTALL_CM_WRITE_VOICE_SETTINGS
void CsrBtCmWriteVoiceSettingsReqHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmDmHciWriteVoiceSettingCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmAclCloseIndHandler(cmInstanceData_t *cmData);
void CsrBtCmDmAclConnHandleIndHandler(cmInstanceData_t *cmData);
void CsrBtCmReturnLowPowerSettings(cmInstanceData_t *cmData, CsrBtDeviceAddr devAddr,
                              link_policy_settings_t *linkPolicySettings);
CsrUint8 returnAclConnectionElement(cmInstanceData_t *cmData, CsrBtDeviceAddr devAddr, aclTable **aclConnectionElement);
void returnAclConnectionFromIndex(cmInstanceData_t *cmData, CsrUint8 index, aclTable **aclConnectionElement);
void returnNextAclConnectionElement(cmInstanceData_t *cmData, aclTable **aclConnectionElement);
void returnNextAvailableAclConnectionElement(cmInstanceData_t *cmData, aclTable **aclConnectionElement);
CsrUint8 returnNumOfAclConnection(cmInstanceData_t *cmData);
void CsrBtCmDmSmEncryptionChangeHandler(cmInstanceData_t *cmData);
CsrUint8 CsrBtCmDmSmEncryptCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmSmSppRepairIndSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr);
void CsrBtCmSmCancelSppRepairInd(cmInstanceData_t *cmData);
void CsrBtCmLogicalChannelTypeHandler(cmInstanceData_t *cmData);
void CsrBtCmA2DPBitrateHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_switch_role_handler.c
************************************************************************************/
CsrUint8 CsrBtCmDmAclReturnNextMarkedForMss(cmInstanceData_t *cmData, aclTable **aclConnectionElement);
CsrUint8 CsrBtCmDmAclMarkForMss(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr);
void CsrBtCmDmAclProcessNextMarkedForMss(cmInstanceData_t *cmData);
aclRoleVars_t* CsrBtCmDmGetAclRoleVars(aclTable *aclConnectionElement);
void CsrBtCmDmAclRoleVarsClear(aclRoleVars_t *roleVars);
#ifdef CSR_BT_INSTALL_CM_SWITCH_ROLE_PUBLIC
void CsrBtCmSwitchRoleReqHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmRoleSwitchCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
                              CsrBtSupplier resultSupplier,
                              CsrBtDeviceAddr deviceAddr, CsrUint8 role, CsrBtCmRoleType roleType);
void CsrBtCmDmHciRoleDiscoveryCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciSwitchRoleCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmRoleDiscoveryReqHandler(cmInstanceData_t *cmData);
void CsrBtCmPrivateSwitchRoleReqHandler(cmInstanceData_t *cmData);
void CsrBtCmPrivateSwitchRoleReqSend(CsrBtDeviceAddr deviceAddr, CsrUint8 role);
void cmDmAclKickRoleSwitch(cmInstanceData_t *cmData);
void CsrBtCmDmSwitchRoleReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmModeChangeRoleSwitchHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr);
void CsrBtCmAlwaysSupportMasterRoleReqHandler(cmInstanceData_t *cmData);
void CsrBtCmRoleSwitchConfigReqHandler(cmInstanceData_t *cmData);
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
CsrBool CsrBtCmRoleSwitchBeforeScoSetupNeeded(cmInstanceData_t *cmData);
#endif
CsrBool CsrBtCmRoleSwitchAllowedByUpperLayer(aclTable *aclConnectionElement);
CsrBool CsrBtCmDmIsScoPacketType(hci_pkt_type_t packetType);
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
void CsrBtCmRfcScoConnectReqHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr);
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
void CsrBtCmL2caScoConnectReqHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr);
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */
void CsrBtCmDmClearRoleCheckedFlag(cmInstanceData_t *cmData);

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
void CsrBtCmBnepSwitchRoleReqHandler(cmInstanceData_t *cmData);
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

/*************************************************************************************
 These function are found under csr_bt_cm_dm_iac_handler.c
************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_PRI_IAC
void CsrBtCmReadIacReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciReadIacCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmWriteIacIndHandler(cmInstanceData_t *cmData);
void CsrBtCmWriteIacReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciWriteIacCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmFinishWriteIac(cmInstanceData_t *cmData, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtCmDmHciReadNumSupportedIacCompleteHandler(cmInstanceData_t *cmData);
#endif
/*************************************************************************************
 These function are found under csr_bt_cm_dm_read_local_name_handler.c
************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_READ_LOCAL_NAME
void CsrBtCmReadLocalNameReqHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmDmHciReadLocalNameCompleteHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_dm_dut_handler.c
************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_DUT_MODE
void CsrBtCmDeviceUnderTestReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciDeviceUnderTestCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmSendDeviceUnderTestComplete(CsrSchedQid appHandle, CsrUint8 status, CsrUint8 step);
void CsrBtCmDeviceUnderTestDisableReqHandler(cmInstanceData_t *cmData);
#endif

/*************************************************************************************
 These function are found under csr_bt_cm_dm_read_local_version_handler.c
************************************************************************************/
void CsrBtCmDmHciReadLocalVersionCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmReadLocalVersionReqHandler(cmInstanceData_t *cmData);
void CsrBtCmGetSecurityConfResHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 This function are found under csr_bt_cm_sdc_handler.c
************************************************************************************/
void CsrBtCmDmAclOpenCfmInSdcCloseStateHandler(cmInstanceData_t * cmData);
void CsrBtCmSdcDecAclRefCountTo(cmInstanceData_t * cmData, CsrBtDeviceAddr deviceAddr);

/*************************************************************************************
These functions are found under csr_bt_cm_dm_read_tx_power_level_handler.c
**************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_READ_TX_POWER_LEVEL
void CsrBtCmDmHciReadTxPowerLevelCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmReadTxPowerLevelReqHandler(cmInstanceData_t *cmData);
#endif
#ifdef CSR_BT_LE_ENABLE
void CsrBtCmReadAdvertisingChTxPowerReqHandler(cmInstanceData_t *cmData);
#endif
/*************************************************************************************
These functions are found under csr_bt_cm_dm_get_link_quality_handler.c
**************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_GET_LINK_QUALITY
void CsrBtCmDmHciGetLinkQualityCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmGetLinkQualityReqHandler(cmInstanceData_t *cmData);
#endif
/*************************************************************************************
These functions are found under csr_bt_cm_dm_read_rssi_handler.c
**************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_READ_RSSI
void CsrBtCmDmHciReadRssiCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmReadRssiReqHandler(cmInstanceData_t *cmData);
#endif
/*************************************************************************************
These functions are found under csr_bt_cm_dm_features_handler.c
**************************************************************************************/
void CsrBtCmDmHciReadSuppFeaturesCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciReadRemoteFeaturesCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciReadRemoteExtFeaturesCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmReadRemoteExtFeaturesReqHandler(cmInstanceData_t *cmData);
#ifdef CSR_BT_INSTALL_CM_READ_LOCAL_EXT_FEATURES
void CsrBtCmDmHciReadLocalExtFeaturesCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmReadLocalExtFeaturesReqHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmReadRemoteFeaturesReqHandler(cmInstanceData_t * cmData);

/*************************************************************************************
These functions are found under csr_bt_cm_dm_afh_handler.c
**************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_AFH
void CsrBtCmDmHciSetAfhChannelClassCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmSetAfhChannelClassReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciReadAfhChannelAssesModeCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmReadAfhChannelAssesModeReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciWriteAfhChannelAssesModeCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmWriteAfhChannelAssesModeReqHandler(cmInstanceData_t *cmData);
void CsrBtCmReadAfhChannelMapCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmReadAfhChannelMapReqHandler(cmInstanceData_t *cmData);
#endif
/*************************************************************************************
These functions are found under csr_bt_cm_dm_read_clock_handler.c
**************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_READ_CLOCK
void CsrBtCmDmHciReadClockCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmReadClockReqHandler(cmInstanceData_t *cmData);
#endif
/*************************************************************************************
These functions are found under csr_bt_cm_dm_read_remote_version_handler.c
**************************************************************************************/
void CsrBtCmDmHciReadRemoteVersionCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmReadRemoteVersionReqHandler(cmInstanceData_t *cmData);

/*************************************************************************************
These functions are found under csr_bt_cm_dm_set_event_filter_handler.c
**************************************************************************************/
void CsrBtCmSetEventFilterBdaddrReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSetEventFilterCodReqHandler(cmInstanceData_t *cmData);
void CsrBtCmClearEventFilterReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSetEventFilterCommonCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmBccmdMaxEventFilterHandler(cmInstanceData_t *cmData);


/*************************************************************************************
These functions are found under csr_bt_cm_dm_cache_params_handler.c
**************************************************************************************/
void CsrBtCmDmWriteCachedPageModeCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmDmWriteCacheParamsReqSend(CsrUint8                activePlayer,
                             CsrBtDeviceAddr           devAddr,
                             CsrUint16               clockOffset,
                             page_scan_mode_t       pageScanMode,
                             page_scan_rep_mode_t   pageScanRepMode);

void CsrBtCmDmUpdateAndClearCachedParamReqSend(CsrBtDeviceAddr  devAddr);
void CsrBtCmDmUpdateAndClearCachedParamDirect(cmInstanceData_t *cmData, CsrBtDeviceAddr  devAddr);
void CsrBtCmDmUpdateAndClearCachedParamReqHandler(cmInstanceData_t *cmData);


void CsrBtCmDmWriteCacheParamsReqHandler(cmInstanceData_t *cmData);

CsrBool isCacheParamsKnown(cmInstanceData_t      *cmData,
                          CsrBtDeviceAddr          devAddr,
                          CsrUint16              *clockOffset,
                          page_scan_mode_t      *pageScanMode,
                          page_scan_rep_mode_t  *pageScanRepMode );

void CsrBtCmDmStoreCacheParams(cmInstanceData_t     *cmData,
                          CsrBtDeviceAddr         devAddr,
                          CsrUint16             clockOffset,
                          page_scan_mode_t      pageScanMode,
                          page_scan_rep_mode_t pageScanRepMode);

void CsrBtCmDmWriteCachedClockOffsetCfmHandler(cmInstanceData_t *cmData);

void CsrBtCmDmWriteCacheParamsDirect(cmInstanceData_t         *cmData,
                                CsrBtDeviceAddr             devAddr,
                                CsrUint16                 clockOffset,
                                page_scan_mode_t         pageScanMode,
                                page_scan_rep_mode_t     pageScanRepMode,
                                CsrUint8                  activePlayer,
                                CsrBtCmPrim                dmMsgTypeInProgress);

#ifdef ENABLE_SHUTDOWN
void CsrBtCmRemoveCacheParamTable(cmInstanceData_t *cmData);
#endif
void CsrBtCmDmClearParamCacheCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciReadClockOffsetCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmFlushCmCacheTimeout(CsrUint8 dummy, cmInstanceData_t *cmData);
void CsrBtCmFlushCmCacheStopTimer(cmInstanceData_t *cmData);
void CsrBtCmFlushCmCacheStartTimer(cmInstanceData_t *cmData);

/*************************************************************************************
These functions are found under cm_dm_write_scan_handler.c
**************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_WRITE_PAGE_SCAN
void CsrBtCmWritePageScanSettingsReqHandler(cmInstanceData_t *cmData);
void CsrBtCmWritePageScanTypeReqHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmDmHciWritePageScanActivityCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciWritePageScanTypeCompleteHandler(cmInstanceData_t *cmData);
/*************************************************************************************
These functions are found under csr_bt_cm_dm_write_inquiry_handler.c
**************************************************************************************/
void CsrBtCmWriteInquiryScanSettingsReqHandler(cmInstanceData_t *cmData);
void CsrBtCmWriteInquiryScanTypeReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciWriteInquiryScanActivityCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciWriteInquiryScanTypeCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciWriteInquiryModeCompleteHandler(cmInstanceData_t *cmData);

/*************************************************************************************
These functions are found under csr_bt_cm_dm_sniff_sub_rate_handler.c
**************************************************************************************/
void CsrBtCmDmHciSniffSubRatingIndHandler(cmInstanceData_t *cmData);
void CsrBtCmDmHciSniffSubRateCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmDmCheckSsrReqSend(CsrBtDeviceAddr deviceAddr);
void CsrBtCmDmCheckSsrReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmModeSettingsReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmL2caModeSettingsReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmBnepModeSettingsReqHandler(cmInstanceData_t *cmData);

/*************************************************************************************
These functions are found under csr_bt_cm_dm_extended_inquiry_response.c
**************************************************************************************/
void CsrBtCmEirInitData(cmInstanceData_t *cmData);
void CsrBtCmEirExtractServicesFromRecord(cmInstanceData_t *cmData, CsrUint16 serviceRecordLen, CsrUint8 *serviceRecord);
void CsrBtCmEirAddServiceRecord(cmInstanceData_t *cmData, CsrUint32 serviceRecordHandle);
void CsrBtCmEirRemoveServiceRecord(cmInstanceData_t *cmData, CsrUint32 serviceRecordHandle);
void CsrBtCmDmHciWriteExtendedInquiryResponseDataCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmEirUpdateName(cmInstanceData_t *cmData);
void CsrBtCmEirUpdateManufacturerReqHandler(cmInstanceData_t *cmData);
void CsrBtCmEirSendUpdateManufacturerCfm(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtCmEirFlagsReqHandler(cmInstanceData_t *cmData);

/*************************************************************************************
These functions are found under csr_bt_cm_dm_write_auto_flush_timeout.c
**************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_READ_FAILED_CONTACT_COUNTER
void CsrBtCmReadFailedContactCounterReqHandler(cmInstanceData_t * cmData);
void CsrBtCmDmHciReadFailedContactCounterCompleteHandler(cmInstanceData_t * cmData);
#endif
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
void CsrBtCmDmWriteAutoFlushTimeoutReqHandler(cmInstanceData_t * cmData);
void CsrBtCmDmHciWriteAutoFlushTimeoutCompleteHandler(cmInstanceData_t *cmData);
void CsrBtCmWriteAutoFlushTimeout(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr);
#else
#define CsrBtCmDmWriteAutoFlushTimeoutReqHandler    NULL
#define CsrBtCmDmHciWriteAutoFlushTimeoutCompleteHandler(cmData)
#define CsrBtCmWriteAutoFlushTimeout(cmData, deviceAddr)
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

/*************************************************************************************
These functions are found under csr_bt_cm_dm_encryption_key_size.c
**************************************************************************************/
void CsrBtCmReadEncryptionKeySizeReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmSetEncryptionKeySizeReqHandler(cmInstanceData_t *cmData);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_CM_DM_H__ */
