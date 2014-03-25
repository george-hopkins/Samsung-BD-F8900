/****************************************************************************

       Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
      
                    All rights reserved.

FILE:            csr_bt_hci_convert.c

DESCRIPTION:            top level converter for _hci commands and events

REVISION:        $Revision: #4 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_util.h"
#include "csr_hci_task.h"
#include "dm_hci_interface.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_lmdownstream.h"
#include "csr_bt_hci_convert_lpdownstream.h"
#include "csr_bt_hci_convert_hcdownstream.h"
#include "csr_bt_hci_convert_info_downstream.h"
#include "csr_bt_hci_convert_status_downstream.h"
#include "csr_bt_hci_convert_test_downstream.h"
#include "csr_bt_hci_convert_event_upstream.h"

#ifdef CSR_BT_LE_ENABLE
#include "tbdaddr.h"
#include "csr_bt_hci_convert_ulp.h"
#include "csr_bt_hci_convert_ulpdownstream.h"
#endif

typedef struct {
    hci_op_code_t opcode;
    CsrBool (*handler)(CONVERT_PARAMS);
} csrBtHciCmdConverter;

static const csrBtHciCmdConverter hciCmdLut[] = {
    {HCI_INQUIRY, ConvertHciInquiry},
    {HCI_INQUIRY_CANCEL, ConvertHciInquiryCancel},
    {HCI_CREATE_CONNECTION, ConvertHciCreateConnection},
    {HCI_DISCONNECT, ConvertHciDisconnect},
    {HCI_CREATE_CONNECTION_CANCEL, ConvertHciCreateConnectionCancel},
    {HCI_ACCEPT_CONNECTION_REQ, ConvertHciAcceptConnectionReq},
    {HCI_REJECT_CONNECTION_REQ, ConvertHciRejectConnectionReq},
    {HCI_LINK_KEY_REQ_REPLY, ConvertHciLinkKeyReqReply},
    {HCI_LINK_KEY_REQ_NEG_REPLY, ConvertHciLinkKeyReqNegReply},
    {HCI_PIN_CODE_REQ_REPLY, ConvertHciPinCodeReqReply},
    {HCI_PIN_CODE_REQ_NEG_REPLY, ConvertHciPinCodeReqNegReply},
#ifndef CSR_BT_EXCLUDE_HCI_CHANGE_ACL_CONN_PKT_TYPE
    {HCI_CHANGE_CONN_PKT_TYPE, ConvertHciChangeConnPktType},
#endif
    {HCI_AUTH_REQ, ConvertHciAuthReq},
    {HCI_SET_CONN_ENCRYPTION, ConvertHciSetConnEncryption},
    {HCI_REMOTE_NAME_REQ, ConvertHciRemoteNameReq},
    {HCI_REMOTE_NAME_REQ_CANCEL, ConvertHciRemoteNameReqCancel},
    {HCI_READ_REMOTE_SUPP_FEATURES, ConvertHciReadRemoteSuppFeatures},
    {HCI_READ_REMOTE_EXT_FEATURES, ConvertHciReadRemoteExtFeatures},
    {HCI_READ_REMOTE_VER_INFO, ConvertHciReadRemoteVerInfo},
    {HCI_READ_CLOCK_OFFSET, ConvertHciReadClockOffset},
    {HCI_SETUP_SYNCHRONOUS_CONN, ConvertHciSetupSynchronousConn},
    {HCI_ACCEPT_SYNCHRONOUS_CONN_REQ, ConvertHciAcceptSynchronousConnReq},
    {HCI_REJECT_SYNCHRONOUS_CONN_REQ, ConvertHciRejectSynchronousConn},
    {HCI_IO_CAPABILITY_RESPONSE, ConvertHciIoCapabilityResponse},
    {HCI_USER_CONFIRMATION_REQUEST_REPLY, ConvertHciUserConfirmationRequestReply},
    {HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY, ConvertHciUserConfirmationRequestNegReply},
    {HCI_USER_PASSKEY_REQUEST_REPLY, ConvertHciUserPasskeyRequestReply},
    {HCI_USER_PASSKEY_REQUEST_NEG_REPLY, ConvertHciUserPasskeyRequestNegReply},
    {HCI_REMOTE_OOB_DATA_REQUEST_REPLY, ConvertHciRemoteOobDataRequestReply},
    {HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY, ConvertHciRemoteOobDataRequestNegReply},
    {HCI_IO_CAPABILITY_REQUEST_NEG_REPLY, ConvertHciIoCapabilityRequestNegReply},
    {HCI_SNIFF_MODE, ConvertHciSniffMode},
    {HCI_EXIT_SNIFF_MODE, ConvertHciExitSniffMode},
#ifndef CSR_BT_EXCLUDE_HCI_PARK_MODE
    {HCI_PARK_MODE, ConvertHciParkMode},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_EXIT_PARK_MODE
    {HCI_EXIT_PARK_MODE, ConvertHciExitParkMode},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_HOLD_MODE
    {HCI_HOLD_MODE, ConvertHciHoldMode},
#endif
    {HCI_QOS_SETUP, ConvertHciQosSetup},
    {HCI_ROLE_DISCOVERY, ConvertHciRoleDiscovery},
    {HCI_SWITCH_ROLE, ConvertHciSwitchRole},
    {HCI_WRITE_LINK_POLICY_SETTINGS, ConvertHciWriteLinkPolicySettings},
    {HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS, ConvertHciWriteDefaultLinkPolicySettings},
    {HCI_SNIFF_SUB_RATE, ConvertHciSniffSubRate},
    {HCI_SET_EVENT_MASK, ConvertHciSetEventMask},
    {HCI_RESET, ConvertHciReset},
    {HCI_SET_EVENT_FILTER, ConvertHciSetEventFilter},
    {HCI_DELETE_STORED_LINK_KEY, ConvertHciDeleteStoredLinkKey},
    {HCI_CHANGE_LOCAL_NAME, ConvertHciChangeLocalName},
#ifndef CSR_BT_EXCLUDE_HCI_READ_LOCAL_NAME
    {HCI_READ_LOCAL_NAME, ConvertHciReadLocalName},
#endif
    {HCI_WRITE_SCAN_ENABLE, ConvertHciWriteScanEnable},
#ifndef CSR_BT_EXCLUDE_HCI_READ_SCAN_ENABLE
    {HCI_READ_SCAN_ENABLE, ConvertHciReadScanEnable},
#endif
    {HCI_WRITE_PAGESCAN_ACTIVITY, ConvertHciWritePagescanActivity},
    {HCI_WRITE_INQUIRYSCAN_ACTIVITY, ConvertHciWriteInquiryscanActivity},
    {HCI_WRITE_AUTH_ENABLE, ConvertHciWriteAuthEnable},
    {HCI_WRITE_ENC_MODE, ConvertHciWriteEncMode},
#ifndef CSR_BT_EXCLUDE_HCI_READ_CLASS_OF_DEVICE
    {HCI_READ_CLASS_OF_DEVICE, ConvertHciReadClassOfDevice},
#endif
    {HCI_WRITE_CLASS_OF_DEVICE, ConvertHciWriteClassOfDevice},
#ifndef CSR_BT_EXCLUDE_HCI_WRITE_VOICE_SETTING
    {HCI_WRITE_VOICE_SETTING, ConvertHciWriteVoiceSetting},
#endif
    {HCI_HOST_BUFFER_SIZE, ConvertHciHostBufferSize},
    {HCI_WRITE_PAGE_TIMEOUT, ConvertHciWritePageTimeout},
    {HCI_WRITE_LINK_SUPERV_TIMEOUT, ConvertHciWriteLinkSupervTimeout},
#ifndef CSR_BT_EXCLUDE_HCI_READ_TX_POWER_LEVEL
    {HCI_READ_TX_POWER_LEVEL, ConvertHciReadTxPowerLevel},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_SET_AFH_CHANNEL_CLASS
    {HCI_SET_AFH_CHANNEL_CLASS, ConvertHciSetAfhChannelClass},
#endif
    {HCI_WRITE_INQUIRY_SCAN_TYPE, ConvertHciWriteInquiryScanType},
    {HCI_WRITE_INQUIRY_MODE, ConvertHciWriteInquiryMode},
    {HCI_WRITE_PAGE_SCAN_TYPE, ConvertHciWritePageScanType},
#ifndef CSR_BT_EXCLUDE_HCI_READ_AFH_CHANNEL_CLASS_M
    {HCI_READ_AFH_CHANNEL_CLASS_M, ConvertHciReadAfhChannelClassM},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_WRITE_AFH_CHANNEL_CLASS_M
    {HCI_WRITE_AFH_CHANNEL_CLASS_M, ConvertHciWriteAfhChannelClassM},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_NUM_SUPPORTED_IAC
    {HCI_READ_NUM_SUPPORTED_IAC, ConvertHciReadNumSupportedIac},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_CURRENT_IAC_LAP
    {HCI_READ_CURRENT_IAC_LAP, ConvertHciReadCurrentIacLap},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_WRITE_CURRENT_IAC_LAP
    {HCI_WRITE_CURRENT_IAC_LAP, ConvertHciWriteCurrentIacLap},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA
    {HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA, ConvertHciWriteExtendedInquiryResponseData},
#endif
    {HCI_REFRESH_ENCRYPTION_KEY, ConvertHciRefreshEncryptionKey},
    {HCI_WRITE_SIMPLE_PAIRING_MODE, ConvertHciWriteSimplePairingMode},
#ifndef CSR_BT_EXCLUDE_SM_READ_LOCAL_OOB_DATA
    {HCI_READ_LOCAL_OOB_DATA, ConvertHciReadLocalOobData},
#endif
    {HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL, ConvertHciWriteInquiryTransmitPowerLevel},
#ifndef CSR_BT_EXCLUDE_SM_SEND_KEYPRESS_NOTIFICATION
    {HCI_SEND_KEYPRESS_NOTIFICATION, ConvertHciSendKeypressNotification},
#endif
#ifdef CSR_BT_LE_ENABLE
    {HCI_WRITE_LE_HOST_SUPPORT, ConvertHciWriteLeHostSupport},
#endif
    {HCI_READ_LOCAL_SUPP_FEATURES, ConvertHciReadLocalSuppFeatures},
    {HCI_READ_BD_ADDR, ConvertHciReadBdAddr},
    {HCI_READ_BUFFER_SIZE, ConvertHciReadBufferSize},
    {HCI_READ_LOCAL_VER_INFO, ConvertHciReadLocalVersionInfo},
#ifndef CSR_BT_EXCLUDE_HCI_READ_LOCAL_EXT_FEATURES
    {HCI_READ_LOCAL_EXT_FEATURES, ConvertHciReadLocalExtFeatures},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_AFH_CHANNEL_MAP
    {HCI_READ_AFH_CHANNEL_MAP, ConvertReadAfhChannelMap},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_CLOCK
    {HCI_READ_CLOCK, ConvertHciReadClock},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_GET_LINK_QUALITY
    {HCI_GET_LINK_QUALITY, ConvertHciGetLinkQuality},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_RSSI
    {HCI_READ_RSSI, ConvertHciReadRssi},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_FAILED_CONTACT_COUNT
    {HCI_READ_FAILED_CONTACT_COUNT, ConvertHciReadFailedContactCount},
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_ENCRYPTION_KEY_SIZE
    {HCI_READ_ENCRYPTION_KEY_SIZE, ConvertHciReadEncryptionKeySize},
#endif
    {HCI_ENABLE_DUT_MODE, ConvertHciEnableDutMode},
    {HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE, ConvertHciWriteSimplePairingDebugMode},
#ifdef CSR_BT_LE_ENABLE
    {HCI_ULP_SET_EVENT_MASK, ConvertHciUlpSetEventMask},
    {HCI_ULP_READ_BUFFER_SIZE, ConvertHciUlpReadBufferSize},
    {HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES, ConvertHciUlpReadLocalSupportedFeatures},
    {HCI_ULP_SET_RANDOM_ADDRESS, ConvertHciUlpSetRandomAddress},
    {HCI_ULP_SET_ADVERTISING_PARAMETERS, ConvertHciUlpSetAdvertisingParameters},
    {HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER, ConvertHciUlpReadAdvertisingChannelTxPower},
    {HCI_ULP_SET_ADVERTISING_DATA, ConvertHciUlpSetAdvertisingData},
    {HCI_ULP_SET_SCAN_RESPONSE_DATA, ConvertHciUlpSetScanResponseData},
    {HCI_ULP_SET_ADVERTISE_ENABLE, ConvertHciUlpSetAdvertiseEnable},
    {HCI_ULP_SET_SCAN_PARAMETERS, ConvertHciUlpSetScanParameters},
    {HCI_ULP_SET_SCAN_ENABLE, ConvertHciUlpSetScanEnable},
    {HCI_ULP_CREATE_CONNECTION, ConvertHciUlpCreateConnection},
    {HCI_ULP_CREATE_CONNECTION_CANCEL, ConvertHciUlpCreateConnectionCancel},
    {HCI_ULP_READ_WHITE_LIST_SIZE, ConvertHciUlpReadWhiteListSize},
    {HCI_ULP_CLEAR_WHITE_LIST, ConvertHciUlpClearWhiteList},
    {HCI_ULP_ADD_DEVICE_TO_WHITE_LIST, ConvertHciUlpAddDeviceToWhiteList},
    {HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST, ConvertHciUlpRemoveDeviceFromWhiteList},
    {HCI_ULP_CONNECTION_UPDATE, ConvertHciUlpConnectionUpdate},
    {HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION, ConvertHciUlpSetHostChannelClassification},
    {HCI_ULP_READ_CHANNEL_MAP, ConvertHciUlpReadChannelMap},
    {HCI_ULP_READ_REMOTE_USED_FEATURES, ConvertHciUlpReadRemoteUsedFeatures},
    {HCI_ULP_ENCRYPT, ConvertHciUlpEncrypt},
    {HCI_ULP_RAND, ConvertHciUlpRand},
    {HCI_ULP_START_ENCRYPTION, ConvertHciUlpStartEncryption},
    {HCI_ULP_LONG_TERM_KEY_REQUESTED_REPLY, ConvertHciUlpLongTermKeyRequestedReply},
    {HCI_ULP_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY, ConvertHciUlpLongTermKeyRequestedNegativeReply},
    {HCI_ULP_READ_SUPPORTED_STATES, ConvertHciUlpReadSupportedStates},
    {HCI_ULP_RECEIVER_TEST, ConvertHciUlpReceiverTest},
    {HCI_ULP_TRANSMITTER_TEST, ConvertHciUlpTransmitterTest},
    {HCI_ULP_TEST_END, ConvertHciUlpTestEnd}
#endif
};

CsrBool CsrBtConvertHciCommand(HCI_UPRIM_T *prim, CsrUint8 * buffer, CsrBool *sendHciCmd)
{
    CsrUint32 index;

    for (index = 0; index < CSR_ARRAY_SIZE(hciCmdLut); ++index)
    {
        if (hciCmdLut[index].opcode == prim->op_code)
        {
            *sendHciCmd = hciCmdLut[index].handler(CONVERT_CALLING_PARAMS);
            return TRUE;
        }
    }

    *sendHciCmd = FALSE;
    return FALSE;
}

typedef struct {
    hci_event_code_t event_code;
    CsrBool (*handler)(CsrUint8 *hciData, HCI_UPRIM_T *prim);
} csrBtHciEvtConverter;

static const csrBtHciEvtConverter hciEvtLut[] = {
    {HCI_EV_INQUIRY_COMPLETE, ConvertHciEvInquiryComplete},
    {HCI_EV_INQUIRY_RESULT, ConvertHciEvInquiryResult},
    {HCI_EV_CONN_COMPLETE, ConvertHciEvConnComplete},
    {HCI_EV_CONN_REQUEST, ConvertHciEvConnRequest},
    {HCI_EV_DISCONNECT_COMPLETE, ConvertHciEvDisconnectComplete},
    {HCI_EV_AUTH_COMPLETE, ConvertHciEvAuthComplete},
    {HCI_EV_REMOTE_NAME_REQ_COMPLETE, ConvertHciEvRemoteNameReqComplete},
    {HCI_EV_ENCRYPTION_CHANGE, ConvertHciEvEncryptionChange},
    {HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE, ConvertHciEvReadRemSuppFeaturesComplete},
    {HCI_EV_READ_REMOTE_VER_INFO_COMPLETE, ConvertHciEvReadRemoteVerInfoComplete},
    {HCI_EV_QOS_SETUP_COMPLETE, ConvertHciEvQosSetupComplete},
    {HCI_EV_COMMAND_COMPLETE, ConvertHciEvCommandComplete},
    {HCI_EV_COMMAND_STATUS, ConvertHciEvCommandStatus},
    {HCI_EV_HARDWARE_ERROR, ConvertHciEvHardwareError},
    {HCI_EV_ROLE_CHANGE, ConvertHciEvRoleChange},
    {HCI_EV_NUMBER_COMPLETED_PKTS, ConvertHciEvNumberCompletedPkts},
    {HCI_EV_MODE_CHANGE, ConvertHciEvModeChange},
    {HCI_EV_PIN_CODE_REQ, ConvertHciEvPinCodeReq},
    {HCI_EV_LINK_KEY_REQ, ConvertHciEvLinkKeyReq},
    {HCI_EV_LINK_KEY_NOTIFICATION, ConvertHciEvLinkKeyNotification},
    {HCI_EV_DATA_BUFFER_OVERFLOW, ConvertHciEvDataBufferOverflow},
    {HCI_EV_MAX_SLOTS_CHANGE, ConvertHciEvMaxSlotsChange},
    {HCI_EV_READ_CLOCK_OFFSET_COMPLETE, ConvertHciEvReadClockOffsetComplete},
    {HCI_EV_QOS_VIOLATION, ConvertHciEvQosViolation},
#ifndef CSR_BT_EXCLUDE_HCI_CHANGE_ACL_CONN_PKT_TYPE
    {HCI_EV_CONN_PACKET_TYPE_CHANGED, ConvertHciEvConnPacketTypeChanged},
#endif
    {HCI_EV_PAGE_SCAN_MODE_CHANGE, ConvertHciEvPageScanModeChange},
    {HCI_EV_PAGE_SCAN_REP_MODE_CHANGE, ConvertHciEvPageScanRepModeChange},
    {HCI_EV_INQUIRY_RESULT_WITH_RSSI, ConvertHciEvInquiryResultWithRssi},
    {HCI_EV_READ_REM_EXT_FEATURES_COMPLETE, ConvertHciEvReadRemExtFeaturesComplete},
    {HCI_EV_SYNC_CONN_COMPLETE, ConvertHciEvSyncConnComplete},
    {HCI_EV_SYNC_CONN_CHANGED, ConvertHciEvSyncConnChangedComplete},
    {HCI_EV_SNIFF_SUB_RATE, ConvertHciEvSniffSubRate},
    {HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE, ConvertHciEvEncryptionKeyRefreshComplete},
    {HCI_EV_EXTENDED_INQUIRY_RESULT, ConvertHciEvExtendedInquiryResult},
    {HCI_EV_IO_CAPABILITY_REQUEST, ConvertHciEvIoCapabilityRequest},
    {HCI_EV_IO_CAPABILITY_RESPONSE, ConvertHciEvIoCapabilityResponse},
    {HCI_EV_USER_CONFIRMATION_REQUEST, ConvertHciEvUserConfirmationRequest},
    {HCI_EV_USER_PASSKEY_REQUEST, ConvertHciEvUserPasskeyRequest},
    {HCI_EV_REMOTE_OOB_DATA_REQUEST, ConvertHciEvRemoteOobDataRequest},
    {HCI_EV_SIMPLE_PAIRING_COMPLETE, ConvertHciEvSimplePairingComplete},
    {HCI_EV_LST_CHANGE, ConvertHciEvLstChange},
    {HCI_EV_USER_PASSKEY_NOTIFICATION, ConvertHciEvUserPasskeyNotification},
    {HCI_EV_KEYPRESS_NOTIFICATION, ConvertHciEvKeypressNotification},
    {HCI_EV_REM_HOST_SUPPORTED_FEATURES, ConvertHciEvRemHostSupportedFeatures},
#ifdef CSR_BT_LE_ENABLE
    {HCI_EV_ULP, ConvertHciEvUlpMainUpstream},
#endif
};

/* Specific HCI event decoder. This does not handle command status or
 * command complete events, which are handled in the specific
 * group-handler files. We still catch the CS and CC events here
 * though but they need more specialised handling... */
HCI_UPRIM_T *CsrBtConvertHciEvent(CsrUint8 *hciData)
{
    CsrUint8 length = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[1]);
    
    if (length > 0)
    {
        HCI_UPRIM_T *prim;
        HCI_EVENT_COMMON_T *msg;
        CsrBool sendHciEvent;
        CsrUint32 index;

        prim = CsrPmemZalloc(sizeof(HCI_UPRIM_T));
        msg = (HCI_EVENT_COMMON_T *) prim;

        msg->event_code = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
        msg->length = length;
        hciData = &hciData[2];

        sendHciEvent = FALSE;

        for (index = 0; index < CSR_ARRAY_SIZE(hciEvtLut); ++index)
        {
            if (hciEvtLut[index].event_code == msg->event_code)
            {
                sendHciEvent = hciEvtLut[index].handler(hciData, prim);
                break;
            }
        }

        if (sendHciEvent)
        {
            return prim;
        }
        else
        {
            CsrPmemFree(prim);
            return NULL;
        }
    }
    return NULL;
}

#ifdef CSR_CHIP_MANAGER_ENABLE
CsrBool CsrBtHciCmCompareConnectionHandles(void *cmdArg, const void *eventArg)
{
    if ( (cmdArg != NULL) && (eventArg != NULL)
        && (*((hci_connection_handle_t *) cmdArg) == *((hci_connection_handle_t *) eventArg)) )
    {
        return TRUE;
    }
    return FALSE;
}

CsrBool CsrBtHciCmCompareBdaddr(void *cmdArg, const void *eventArg)
{
    if( (cmdArg == NULL) || (eventArg == NULL) )
    {
        return FALSE;
    }

    return (CsrBtBdAddrEq((const BD_ADDR_T *) cmdArg, (const BD_ADDR_T *) eventArg));
}

#ifdef CSR_BT_LE_ENABLE
CsrBool CsrBtHciCmCompareBdaddrt(void *cmdArg, const void *eventArg)
{
    if( (cmdArg == NULL) || (eventArg == NULL) )
    {
        return FALSE;
    }

    return (tbdaddr_eq((const TYPED_BD_ADDR_T *) cmdArg, (const TYPED_BD_ADDR_T *) eventArg));
}
#endif

void CsrBtHciCmHciCommonStatusEventHandler(CsrUint8 numHciCommandPackets, hci_return_t hciStatus, hci_op_code_t opcode)
{
    HCI_EV_COMMAND_STATUS_T evt;
    evt.event.event_code           = HCI_EV_COMMAND_STATUS; 
    evt.event.length               = HCI_EV_COMMAND_STATUS_PARAM_LEN;
    evt.status                     = hciStatus;
    evt.num_hci_command_pkts       = numHciCommandPackets;
    evt.op_code                    = opcode;
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
    /*CsrSchedMessagePut(DM_HCI_IFACEQUEUE, CSR_HCI_PRIM, prim);*/
}

hci_connection_handle_t *CsrBtHciConnectionHandleDup(hci_connection_handle_t handle)
{
    hci_connection_handle_t *resHandle = NULL;

    resHandle = (hci_connection_handle_t *) CsrPmemAlloc(sizeof(hci_connection_handle_t));
    *resHandle = handle;

    return resHandle;
}

#endif /* CSR_CHIP_MANAGER_ENABLE */
