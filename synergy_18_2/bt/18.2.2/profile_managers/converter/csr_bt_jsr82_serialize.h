#ifndef CSR_BT_JSR82_SERIALIZE_H__
#define CSR_BT_JSR82_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtJsr82Pfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND
CsrSize CsrBtJsr82DlDeviceDiscoveredIndSizeof(void *msg);
CsrUint8 *CsrBtJsr82DlDeviceDiscoveredIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DlDeviceDiscoveredIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82DlDeviceDiscoveredIndSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_REQ
CsrSize CsrBtJsr82RfcDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcDisconnectReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_CFM
CsrSize CsrBtJsr82L2caRxReadyCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caRxReadyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caRxReadyCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caRxReadyCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_REQ
CsrSize CsrBtJsr82RfcCloseReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcCloseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcCloseReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ
CsrSize CsrBtJsr82RdIsEncryptedReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdIsEncryptedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdIsEncryptedReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdIsEncryptedReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ
CsrSize CsrBtJsr82DaSelectServiceReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82DaSelectServiceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DaSelectServiceReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82DaSelectServiceReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_REQ
CsrSize CsrBtJsr82SrRemoveRecordReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82SrRemoveRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82SrRemoveRecordReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82SrRemoveRecordReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM
CsrSize CsrBtJsr82L2caRxDataCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caRxDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caRxDataCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82L2caRxDataCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_CFM
CsrSize CsrBtJsr82RdAuthenticateCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdAuthenticateCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_REQ
CsrSize CsrBtJsr82L2caAcceptReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caAcceptReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caAcceptReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_REQ
CsrSize CsrBtJsr82L2caOpenReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caOpenReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caOpenReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_CFM
CsrSize CsrBtJsr82LdUpdateRecordCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdUpdateRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdUpdateRecordCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdUpdateRecordCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM
CsrSize CsrBtJsr82RfcReceiveDataCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcReceiveDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcReceiveDataCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82RfcReceiveDataCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_REQ
CsrSize CsrBtJsr82RdEncryptReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdEncryptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdEncryptReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdEncryptReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_REQ
CsrSize CsrBtJsr82RfcConnectReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcConnectReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM
CsrSize CsrBtJsr82SrPopulateRecordCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82SrPopulateRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82SrPopulateRecordCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82SrPopulateRecordCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ
CsrSize CsrBtJsr82L2caTxDataReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caTxDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caTxDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82L2caTxDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM
CsrSize CsrBtJsr82RdGetFriendlyNameCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdGetFriendlyNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdGetFriendlyNameCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82RdGetFriendlyNameCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_CFM
CsrSize CsrBtJsr82SrRegisterRecordCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82SrRegisterRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82SrRegisterRecordCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82SrRegisterRecordCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_IND
CsrSize CsrBtJsr82RfcCloseIndSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcCloseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcCloseIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcCloseIndSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_HOUSE_CLEANING
CsrSize CsrBtJsr82HouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtJsr82HouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82HouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82HouseCleaningSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_REQ
CsrSize CsrBtJsr82L2caRxDataReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caRxDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caRxDataReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caRxDataReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM
CsrSize CsrBtJsr82LdGetSecurityLevelCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetSecurityLevelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetSecurityLevelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdGetSecurityLevelCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ
CsrSize CsrBtJsr82RfcGetServerChannelReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcGetServerChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcGetServerChannelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcGetServerChannelReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_IND
CsrSize CsrBtJsr82L2caCloseIndSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caCloseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caCloseIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caCloseIndSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ
CsrSize CsrBtJsr82RdIsAuthenticatedReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdIsAuthenticatedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdIsAuthenticatedReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdIsAuthenticatedReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_REQ
CsrSize CsrBtJsr82L2caGetConfigReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caGetConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caGetConfigReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caGetConfigReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ
CsrSize CsrBtJsr82LdUpdateRecordReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdUpdateRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdUpdateRecordReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82LdUpdateRecordReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_CFM
CsrSize CsrBtJsr82L2caAcceptCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caAcceptCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caAcceptCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ
CsrSize CsrBtJsr82LdSetDiscoverableReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdSetDiscoverableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdSetDiscoverableReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdSetDiscoverableReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ
CsrSize CsrBtJsr82RfcReceiveDataReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcReceiveDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcReceiveDataReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcReceiveDataReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM
CsrSize CsrBtJsr82RdIsEncryptedCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdIsEncryptedCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdIsEncryptedCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdIsEncryptedCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_REQ
CsrSize CsrBtJsr82L2caRxReadyReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caRxReadyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caRxReadyReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caRxReadyReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_REQ
CsrSize CsrBtJsr82SetEventMaskReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82SetEventMaskReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82SetEventMaskReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82SetEventMaskReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ
CsrSize CsrBtJsr82LdGetSecurityLevelReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetSecurityLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetSecurityLevelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdGetSecurityLevelReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM
CsrSize CsrBtJsr82LdGetBluetoothAddressCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetBluetoothAddressCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetBluetoothAddressCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdGetBluetoothAddressCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM
CsrSize CsrBtJsr82RdIsAuthenticatedCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdIsAuthenticatedCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdIsAuthenticatedCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdIsAuthenticatedCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_CFM
CsrSize CsrBtJsr82DaSelectServiceCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82DaSelectServiceCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DaSelectServiceCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82DaSelectServiceCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_CFM
CsrSize CsrBtJsr82L2caGetConfigCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caGetConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caGetConfigCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caGetConfigCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_CLEANUP_REQ
CsrSize CsrBtJsr82CleanupReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82CleanupReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82CleanupReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82CleanupReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_CLEANUP_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_REQ
CsrSize CsrBtJsr82LdGetPropertyReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetPropertyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetPropertyReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdGetPropertyReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ
CsrSize CsrBtJsr82DaCancelServiceSearchReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82DaCancelServiceSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DaCancelServiceSearchReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82DaCancelServiceSearchReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ
CsrSize CsrBtJsr82SrPopulateRecordReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82SrPopulateRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82SrPopulateRecordReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82SrPopulateRecordReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_REQ
CsrSize CsrBtJsr82L2caCloseReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caCloseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caCloseReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ
CsrSize CsrBtJsr82DaCancelInquiryReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82DaCancelInquiryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DaCancelInquiryReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82DaCancelInquiryReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM
CsrSize CsrBtJsr82RfcAcceptAndOpenCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcAcceptAndOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcAcceptAndOpenCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcAcceptAndOpenCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND
CsrSize CsrBtJsr82DlInquiryCompletedIndSizeof(void *msg);
CsrUint8 *CsrBtJsr82DlInquiryCompletedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DlInquiryCompletedIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82DlInquiryCompletedIndSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ
CsrSize CsrBtJsr82LdGetDeviceClassReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetDeviceClassReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetDeviceClassReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdGetDeviceClassReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_CFM
CsrSize CsrBtJsr82SrCreateRecordCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82SrCreateRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82SrCreateRecordCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82SrCreateRecordCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_REQ
CsrSize CsrBtJsr82RdAuthenticateReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdAuthenticateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdAuthenticateReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM
CsrSize CsrBtJsr82DaRetrieveDevicesCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82DaRetrieveDevicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DaRetrieveDevicesCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82DaRetrieveDevicesCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_CFM
CsrSize CsrBtJsr82RfcConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcConnectCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ
CsrSize CsrBtJsr82RdGetFriendlyNameReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdGetFriendlyNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdGetFriendlyNameReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdGetFriendlyNameReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ
CsrSize CsrBtJsr82RfcSendDataReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcSendDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcSendDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82RfcSendDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ
CsrSize CsrBtJsr82RfcAcceptAndOpenReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcAcceptAndOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcAcceptAndOpenReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcAcceptAndOpenReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA
CsrSize CsrBtJsr82ContinueRfcSendDataSizeof(void *msg);
CsrUint8 *CsrBtJsr82ContinueRfcSendDataSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82ContinueRfcSendDataDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82ContinueRfcSendDataSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ
CsrSize CsrBtJsr82RfcGetAvailableReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcGetAvailableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcGetAvailableReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcGetAvailableReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM
CsrSize CsrBtJsr82RfcGetAvailableCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcGetAvailableCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcGetAvailableCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcGetAvailableCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND
CsrSize CsrBtJsr82DlServicesDiscoveredIndSizeof(void *msg);
CsrUint8 *CsrBtJsr82DlServicesDiscoveredIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DlServicesDiscoveredIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82DlServicesDiscoveredIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_REQ
CsrSize CsrBtJsr82L2caGetPsmReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caGetPsmReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caGetPsmReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caGetPsmReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_CFM
CsrSize CsrBtJsr82RdEncryptCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdEncryptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdEncryptCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdEncryptCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_CFM
CsrSize CsrBtJsr82RfcDisconnectCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcDisconnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcDisconnectCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ
CsrSize CsrBtJsr82LdGetBluetoothAddressReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetBluetoothAddressReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetBluetoothAddressReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdGetBluetoothAddressReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ
CsrSize CsrBtJsr82LdGetDiscoverableReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetDiscoverableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetDiscoverableReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdGetDiscoverableReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_CFM
CsrSize CsrBtJsr82SetEventMaskCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82SetEventMaskCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82SetEventMaskCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82SetEventMaskCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_CFM
CsrSize CsrBtJsr82SrRemoveRecordCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82SrRemoveRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82SrRemoveRecordCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82SrRemoveRecordCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ
CsrSize CsrBtJsr82SrRegisterRecordReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82SrRegisterRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82SrRegisterRecordReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82SrRegisterRecordReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM
CsrSize CsrBtJsr82LdGetDiscoverableCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetDiscoverableCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetDiscoverableCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdGetDiscoverableCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_REQ
CsrSize CsrBtJsr82LdIsMasterReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdIsMasterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdIsMasterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdIsMasterReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_CFM
CsrSize CsrBtJsr82L2caGetPsmCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caGetPsmCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caGetPsmCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caGetPsmCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_REQ
CsrSize CsrBtJsr82SrCreateRecordReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82SrCreateRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82SrCreateRecordReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82SrCreateRecordReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM
CsrSize CsrBtJsr82LdGetDeviceClassCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetDeviceClassCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetDeviceClassCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdGetDeviceClassCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ
CsrSize CsrBtJsr82LdGetFriendlyNameReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetFriendlyNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetFriendlyNameReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdGetFriendlyNameReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ
CsrSize CsrBtJsr82DaRetrieveDevicesReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82DaRetrieveDevicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DaRetrieveDevicesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82DaRetrieveDevicesReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_CFM
CsrSize CsrBtJsr82L2caCloseCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caCloseCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caCloseCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_CFM
CsrSize CsrBtJsr82L2caTxDataCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caTxDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caTxDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caTxDataCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_CFM
CsrSize CsrBtJsr82L2caOpenCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caOpenCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caOpenCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_START_INQUIRY_REQ
CsrSize CsrBtJsr82DaStartInquiryReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82DaStartInquiryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DaStartInquiryReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82DaStartInquiryReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_DA_START_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM
CsrSize CsrBtJsr82RfcGetServerChannelCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcGetServerChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcGetServerChannelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcGetServerChannelCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ
CsrSize CsrBtJsr82LdSetDeviceClassReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdSetDeviceClassReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdSetDeviceClassReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdSetDeviceClassReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_REQ
CsrSize CsrBtJsr82L2caDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caDisconnectReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM
CsrSize CsrBtJsr82LdGetFriendlyNameCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetFriendlyNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetFriendlyNameCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82LdGetFriendlyNameCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM
CsrSize CsrBtJsr82LdSetDiscoverableCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdSetDiscoverableCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdSetDiscoverableCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdSetDiscoverableCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND
CsrSize CsrBtJsr82DlServiceSearchCompletedIndSizeof(void *msg);
CsrUint8 *CsrBtJsr82DlServiceSearchCompletedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DlServiceSearchCompletedIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82DlServiceSearchCompletedIndSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_CFM
CsrSize CsrBtJsr82LdIsMasterCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdIsMasterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdIsMasterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdIsMasterCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_REQ
CsrSize CsrBtJsr82RdIsConnectedReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdIsConnectedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdIsConnectedReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdIsConnectedReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_REQ
CsrSize CsrBtJsr82RdIsTrustedReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdIsTrustedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdIsTrustedReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdIsTrustedReqSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ
CsrSize CsrBtJsr82DaSearchServicesReqSizeof(void *msg);
CsrUint8 *CsrBtJsr82DaSearchServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82DaSearchServicesReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtJsr82DaSearchServicesReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_CFM
CsrSize CsrBtJsr82RfcCloseCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcCloseCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcCloseCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_CFM
CsrSize CsrBtJsr82RfcSendDataCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RfcSendDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RfcSendDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RfcSendDataCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_CFM
CsrSize CsrBtJsr82RdIsTrustedCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdIsTrustedCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdIsTrustedCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdIsTrustedCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM
CsrSize CsrBtJsr82LdSetDeviceClassCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdSetDeviceClassCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdSetDeviceClassCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdSetDeviceClassCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_CFM
CsrSize CsrBtJsr82L2caDisconnectCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82L2caDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82L2caDisconnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82L2caDisconnectCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_CFM
CsrSize CsrBtJsr82RdIsConnectedCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82RdIsConnectedCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82RdIsConnectedCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82RdIsConnectedCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_CFM
CsrSize CsrBtJsr82LdGetPropertyCfmSizeof(void *msg);
CsrUint8 *CsrBtJsr82LdGetPropertyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtJsr82LdGetPropertyCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtJsr82LdGetPropertyCfmSerFree CsrBtJsr82Pfree
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_CFM */

#ifdef __cplusplus
}
#endif

#endif
