/****************************************************************************

       Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
       
                     All rights reserved.

FILE:           csr_bt_hci_convert_event_upstream.c

DESCRIPTION:    Converter for HCI Event

REVISION:       $Revision: #3 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "hci_prim.h"
#include "bluetooth.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_lmupstream.h"
#include "csr_bt_hci_convert_lpupstream.h"
#include "csr_bt_hci_convert_hcupstream.h"
#include "csr_bt_hci_convert_info_upstream.h"
#include "csr_bt_hci_convert_status_upstream.h"
#ifdef CSR_BT_LE_ENABLE
#include "csr_bt_hci_convert_ulpupstream.h"
#endif
#include "csr_bt_hci_convert_event_upstream.h"

#include "tbdaddr.h"
#include "csr_bt_util.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_hci_cm.h"

CsrBool ConvertHciEvInquiryComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    hci_return_t status = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(HCI_INQUIRY,status,TRUE,NULL,NULL))
    {
        return FALSE;
    }
#endif
    {
        HCI_EV_INQUIRY_COMPLETE_T * msg;
        msg         = (HCI_EV_INQUIRY_COMPLETE_T *) prim;
        msg->status = status;
        return TRUE;
    }
}

CsrBool ConvertHciEvInquiryResult(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if(!CsrBtHciCmSendRequest())
    {
        return FALSE;
    }
#endif
    {
        CsrUint8                     i;
        HCI_EV_INQUIRY_RESULT_T     * msg;

        msg = (HCI_EV_INQUIRY_RESULT_T *) prim;

        msg->num_responses = CSR_BT_CONVERT_8_FROM_STREAM(hciData);

        for(i = 0; i < msg->num_responses; i++)
        {
            if (0 == i % HCI_MAX_INQ_RESULT_PER_PTR)
            {
                msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR] = CsrPmemAlloc(sizeof(HCI_INQ_RESULT_T) * HCI_MAX_INQ_RESULT_PER_PTR);
            }
            msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].bd_addr.lap = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[1 + i * 14]);
            msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].bd_addr.uap = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[4 + i * 14]);
            msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].bd_addr.nap = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[5 + i * 14]);
            msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].page_scan_rep_mode = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[7 + i * 14]);
            msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].page_scan_period_mode = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[8 + i * 14]);
            msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].page_scan_mode = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[9 + i * 14]);
            msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].dev_class = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[10 + i * 14]);
            msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].clock_offset = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[13 + i * 14]);
        }
        return TRUE;
    }
}

CsrBool ConvertHciEvExtendedInquiryResult(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if(!CsrBtHciCmSendRequest())
    {
        return FALSE;
    }
#endif
    {
        HCI_EV_EXTENDED_INQUIRY_RESULT_T    *msg;
        CsrUint16                           eirLength = 0;

        msg                                 = (HCI_EV_EXTENDED_INQUIRY_RESULT_T *) prim;
        msg->num_responses                  = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
        msg->result.bd_addr.lap             = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[1]);
        msg->result.bd_addr.uap             = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[4]);
        msg->result.bd_addr.nap             = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[5]);
        msg->result.page_scan_rep_mode      = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[7]);
        msg->result.page_scan_period_mode   = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[8]);
        msg->result.dev_class               = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[9]);
        msg->result.clock_offset            = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[12]);
        msg->result.rssi                    = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[14]);

        /* Determine length of EIR-message */
        while ((eirLength < HCI_EIR_DATA_LENGTH) &&
               (hciData[eirLength + 15] != 0) &&
               ((eirLength + hciData[eirLength + 15] + 1) <= HCI_EIR_DATA_LENGTH))
        {
            /* The tag length is valid */
            eirLength += hciData[eirLength + 15] + 1;
        }

        if (eirLength != 0)
        {
            CsrUint16 i;

            /* Copy data to message */
            for (i = 0; i < eirLength; i++)
            {
                if(0 == i % HCI_EIR_DATA_BYTES_PER_PTR)
                {
                    msg->eir_data_part[i / HCI_EIR_DATA_BYTES_PER_PTR] = CsrPmemZalloc(HCI_EIR_DATA_BYTES_PER_PTR);
                }

                msg->eir_data_part[i / HCI_EIR_DATA_BYTES_PER_PTR][i % HCI_EIR_DATA_BYTES_PER_PTR] = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[i + 15]);
            }
        }

        return TRUE;
    }
}

CsrBool ConvertHciEvConnComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_CONN_COMPLETE_T * msg;

    msg = (HCI_EV_CONN_COMPLETE_T *) prim;

    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle                     = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->bd_addr.lap                = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[3]);
    msg->bd_addr.uap                = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[6]);
    msg->bd_addr.nap                = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[7]);
    msg->link_type                  = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[9]);
    msg->enc_mode                   = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[10]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmEventConnectionCompleteHandler(msg->status, msg->handle, msg->link_type, &msg->bd_addr));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvConnRequest(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_CONN_REQUEST_T * msg;

    msg = (HCI_EV_CONN_REQUEST_T *) prim;

    msg->bd_addr.lap                = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap                = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap                = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    msg->dev_class                  = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[6]);
    msg->link_type                  = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[9]);
    return TRUE;
}

CsrBool ConvertHciEvDisconnectComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    hci_return_t             status     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    hci_connection_handle_t  handle     = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventDisconnectionCompleteHandler(status, handle))
    {
        return FALSE;
    }
#endif
    {
        HCI_EV_DISCONNECT_COMPLETE_T * msg;

        msg = (HCI_EV_DISCONNECT_COMPLETE_T *) prim;

        msg->status     = status;
        msg->handle     = handle;
        msg->reason     = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
        return TRUE;
    }
}

CsrBool ConvertHciEvAuthComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_AUTH_COMPLETE_T * msg;

    msg = (HCI_EV_AUTH_COMPLETE_T *) prim;

    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle                     = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_AUTH_REQ,msg->status,TRUE,NULL,&(msg->handle));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvRemoteNameReqComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    CsrUint8                             i;
    CsrUint16                            nameLength;
    HCI_EV_REMOTE_NAME_REQ_COMPLETE_T * msg;

    msg = (HCI_EV_REMOTE_NAME_REQ_COMPLETE_T *) prim;

    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->bd_addr.lap                = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[1]);
    msg->bd_addr.uap                = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[4]);
    msg->bd_addr.nap                = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[5]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(HCI_REMOTE_NAME_REQ,msg->status,TRUE,NULL,&(msg->bd_addr)))
    {
        return FALSE;
    }
#endif
    {
        nameLength = msg->event.length - 7;

        for(i = 0; i < nameLength; i++)
        {
            if(0 == i % HCI_LOCAL_NAME_BYTES_PER_PTR)
            {
                msg->name_part[i / HCI_LOCAL_NAME_BYTES_PER_PTR] = CsrPmemAlloc(HCI_LOCAL_NAME_BYTES_PER_PTR);
            }
            msg->name_part[i / HCI_LOCAL_NAME_BYTES_PER_PTR][i % HCI_LOCAL_NAME_BYTES_PER_PTR] = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[7 + i]);

        }
        return TRUE;
    }
}

CsrBool ConvertHciEvEncryptionChange(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_ENCRYPTION_CHANGE_T * msg;

    msg = (HCI_EV_ENCRYPTION_CHANGE_T *) prim;

    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle                     = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->enc_enable                 = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_SET_CONN_ENCRYPTION,msg->status,TRUE,NULL,&(msg->handle));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvReadRemSuppFeaturesComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    CsrUint8                                     i;
    HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE_T * msg;

    msg = (HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE_T  *) prim;

    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle                     = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);

    for(i = 0; i < CSR_ARRAY_SIZE(msg->lmp_supp_features); i++)
    {
        msg->lmp_supp_features[i]   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[(3 + i * 2)]);
    }
#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_READ_REMOTE_SUPP_FEATURES,msg->status,TRUE,NULL,&(msg->handle));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvReadRemoteVerInfoComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_READ_REMOTE_VER_INFO_COMPLETE_T * msg;

    msg = (HCI_EV_READ_REMOTE_VER_INFO_COMPLETE_T *) prim;

    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle                     = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->lmp_version                = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->manuf_name                 = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    msg->lmp_subversion             = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[6]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_READ_REMOTE_VER_INFO,msg->status,TRUE,NULL,&(msg->handle));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvQosSetupComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_QOS_SETUP_COMPLETE_T * msg;

    msg = (HCI_EV_QOS_SETUP_COMPLETE_T *) prim;

    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle                     = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->flags                      = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->service_type               = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[4]);
    msg->token_rate                 = CSR_BT_CONVERT_32_FROM_STREAM(&hciData[5]);
    msg->peak_bandwidth             = CSR_BT_CONVERT_32_FROM_STREAM(&hciData[9]);
    msg->latency                    = CSR_BT_CONVERT_32_FROM_STREAM(&hciData[13]);
    msg->delay_variation            = CSR_BT_CONVERT_32_FROM_STREAM(&hciData[17]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_QOS_SETUP,msg->status,TRUE,NULL,&(msg->handle));
#else
    return TRUE;
#endif
}

typedef struct {
    hci_op_code_t opcode;
    CsrBool (*handler)(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
} csrBtHciEvtCmdCmpltConverter;

static const csrBtHciEvtCmdCmpltConverter hciEvtCmdCmpltLut[] = {
    { HCI_CREATE_CONNECTION_CANCEL, ConvertHciCreateConnectionCancelRet },
    { HCI_REMOTE_NAME_REQ_CANCEL, ConvertHciRemoteNameReqCancelRet },
    { HCI_LINK_KEY_REQ_REPLY, ConvertHciLinkKeyReqRet },
    { HCI_WRITE_LINK_POLICY_SETTINGS, ConvertHciWriteLinkPolicySettingsRet },
    { HCI_ROLE_DISCOVERY, ConvertHciRoleDiscoveryRet },
    { HCI_SNIFF_SUB_RATE, ConvertHciSniffSubrateRet },
    { HCI_DELETE_STORED_LINK_KEY, ConvertHciDeleteStoredLinkKeyRet },
#ifndef CSR_BT_EXCLUDE_HCI_READ_SCAN_ENABLE
    { HCI_READ_SCAN_ENABLE, ConvertHciReadScanEnableRet },
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_CLASS_OF_DEVICE
    { HCI_READ_CLASS_OF_DEVICE, ConvertReadClassOfDeviceRet },
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_LOCAL_NAME
    { HCI_READ_LOCAL_NAME, ConvertReadLocalNameRet },
#endif
    { HCI_WRITE_LINK_SUPERV_TIMEOUT, ConvertWriteLinkSupervTimeoutRet },
#ifndef CSR_BT_EXCLUDE_HCI_READ_TX_POWER_LEVEL
    { HCI_READ_TX_POWER_LEVEL, ConvertReadTxPowerLevelRet },
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_AFH_CHANNEL_CLASS_M
    { HCI_READ_AFH_CHANNEL_CLASS_M, ConvertReadAfhChannelClassMRet },
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_NUM_SUPPORTED_IAC
    { HCI_READ_NUM_SUPPORTED_IAC_RET, ConvertHciReadNumSupportedIacRet },
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_CURRENT_IAC_LAP
    { HCI_READ_CURRENT_IAC_LAP_RET, ConvertHciReadCurrentIacLapRet },
#endif
#ifndef CSR_BT_EXCLUDE_SM_READ_LOCAL_OOB_DATA
    { HCI_READ_LOCAL_OOB_DATA_RET, ConvertHciReadLocalOobDataRet },
#endif
#ifndef CSR_BT_EXCLUDE_SM_SEND_KEYPRESS_NOTIFICATION
    { HCI_SEND_KEYPRESS_NOTIFICATION, ConvertHciSendKeypressNotificationRet },
#endif    
    { HCI_READ_LOCAL_SUPP_FEATURES, ConvertHciReadLocalSuppFeaturesRet },
    { HCI_READ_BD_ADDR, ConvertHciReadBdAddrRet },
    { HCI_READ_BUFFER_SIZE, ConvertHciReadBufferSizeRet },
    { HCI_READ_LOCAL_VER_INFO, ConvertHciReadLocalVersionInfoRet },
#ifndef CSR_BT_EXCLUDE_HCI_READ_LOCAL_EXT_FEATURES
    { HCI_READ_LOCAL_EXT_FEATURES, ConvertHciReadLocalExtFeaturesRet },
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_AFH_CHANNEL_MAP
    { HCI_READ_AFH_CHANNEL_MAP, ConvertHciReadAfhChannelMapRet },
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_CLOCK
    { HCI_READ_CLOCK, ConvertHciReadClockRet },
#endif
#ifndef CSR_BT_EXCLUDE_HCI_GET_LINK_QUALITY
    { HCI_GET_LINK_QUALITY, ConvertHciGetLinkQualityRet },
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_RSSI
    { HCI_READ_RSSI, ConvertHciReadRssiRet },
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_FAILED_CONTACT_COUNT
    { HCI_READ_FAILED_CONTACT_COUNT, ConvertHciReadFailedContactCountRet },
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_ENCRYPTION_KEY_SIZE
    { HCI_READ_ENCRYPTION_KEY_SIZE, ConvertHciReadEncryptionKeySizeRet },
#endif
#ifdef CSR_BT_LE_ENABLE
    { HCI_ULP_READ_BUFFER_SIZE, ConvertHciUlpReadBufferSizeRet },
    { HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES, ConvertHciUlpReadLocalSupportedFeaturesRet },
    { HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER, ConvertHciUlpReadAdvertisingChannelTxPowerRet },
    { HCI_ULP_READ_WHITE_LIST_SIZE, ConvertHciUlpReadWhiteListSizeRet },
    { HCI_ULP_READ_CHANNEL_MAP, ConvertHciUlpReadChannelMapRet },
    { HCI_ULP_ENCRYPT, ConvertHciUlpEncryptRet },
    { HCI_ULP_RAND, ConvertHciUlpRandRet },
    { HCI_ULP_LONG_TERM_KEY_REQUESTED_REPLY, ConvertHciUlpLongTermKeyRequestedReplyRet },
    { HCI_ULP_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY, ConvertHciUlpLongTermKeyRequestedNegativeReplyRet },
    { HCI_ULP_READ_SUPPORTED_STATES, ConvertHciUlpReadSupportedStatesRet },
    { HCI_ULP_TEST_END, ConvertHciUlpTestEndRet },
#endif    
};

static CsrBool convertHciEvCommandCompleteArg(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    CsrUint32 index;

    for (index = 0; index < CSR_ARRAY_SIZE(hciEvtCmdCmpltLut); ++index)
    {
        if (hciEvtCmdCmpltLut[index].opcode == msg->op_code)
        {
            return hciEvtCmdCmpltLut[index].handler(hciData, msg);
        }
    }

#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL);
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvCommandComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_COMMAND_COMPLETE_T * msg;

    msg = (HCI_EV_COMMAND_COMPLETE_T *) prim;

    msg->num_hci_command_pkts       = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->op_code                    = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->argument_ptr               = CsrPmemAlloc(sizeof(*msg->argument_ptr));;

    hciData = &hciData[4];
    if (convertHciEvCommandCompleteArg(hciData, msg))
    {
        return TRUE;
    }
    else
    {
        CsrPmemFree(msg->argument_ptr);
        msg->argument_ptr = NULL;
        return FALSE;
    }
}

CsrBool ConvertHciEvCommandStatus(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_op_code_t opcode;
#endif
    HCI_EV_COMMAND_STATUS_T * msg;

    msg = (HCI_EV_COMMAND_STATUS_T *) prim;

    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->num_hci_command_pkts       = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[1]);
    msg->op_code                    = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[2]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    switch (msg->op_code)
    {
        case HCI_ACCEPT_CONNECTION_REQ:
        case HCI_REJECT_CONNECTION_REQ:
        {
            opcode = HCI_CREATE_CONNECTION;
            break;
        }
        case HCI_ACCEPT_SYNCHRONOUS_CONN_REQ:
        case HCI_REJECT_SYNCHRONOUS_CONN_REQ:
        {
            opcode = HCI_SETUP_SYNCHRONOUS_CONN;
            break;
        }
        case HCI_EXIT_PARK_MODE:
        {
            opcode = HCI_EXIT_SNIFF_MODE;
            break;
        }
        default:
        {
            opcode = msg->op_code;
            break;
        }
    }
    return CsrBtHciCmEventHandler(opcode, msg->status, FALSE, &msg->num_hci_command_pkts, NULL);
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvHardwareError(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_HARDWARE_ERROR_T * msg;

    msg = (HCI_EV_HARDWARE_ERROR_T *) prim;

    msg->hw_error                   = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    return TRUE;
}

CsrBool ConvertHciEvRoleChange(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_ROLE_CHANGE_T * msg;

    msg = (HCI_EV_ROLE_CHANGE_T *) prim;

    msg->status                 = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->bd_addr.lap            = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[1]);
    msg->bd_addr.uap            = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[4]);
    msg->bd_addr.nap            = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[5]);
    msg->new_role               = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_SWITCH_ROLE,msg->status,TRUE,NULL,&(msg->bd_addr));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvNumberCompletedPkts(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    CsrUint8                         i;
    HCI_EV_NUMBER_COMPLETED_PKTS_T * msg;

    msg = (HCI_EV_NUMBER_COMPLETED_PKTS_T *) prim;

    msg->num_handles                = CSR_BT_CONVERT_8_FROM_STREAM(hciData);

    for(i = 0; i < msg->num_handles; i++)
    {
        if (0 == i % HCI_HOST_NUM_COMPLETED_PACKETS_PER_PTR)
        {
            msg->num_completed_pkts_ptr[i / HCI_HOST_NUM_COMPLETED_PACKETS_PER_PTR] = CsrPmemAlloc(sizeof(HANDLE_COMPLETE_T) * HCI_HOST_NUM_COMPLETED_PACKETS_PER_PTR);
        }
        msg->num_completed_pkts_ptr[i / HCI_HOST_NUM_COMPLETED_PACKETS_PER_PTR][i % HCI_HOST_NUM_COMPLETED_PACKETS_PER_PTR].handle      = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[(1 + i * 4)]);
        msg->num_completed_pkts_ptr[i / HCI_HOST_NUM_COMPLETED_PACKETS_PER_PTR][i % HCI_HOST_NUM_COMPLETED_PACKETS_PER_PTR].num_completed   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[(3 + i * 4)]);
    }
    return TRUE;
}

CsrBool ConvertHciEvModeChange(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_MODE_CHANGE_T * msg;
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_op_code_t opcode;
#endif

    msg = (HCI_EV_MODE_CHANGE_T *) prim;

    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle                     = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->curr_mode                  = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->interval                   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    
#ifdef CSR_CHIP_MANAGER_ENABLE
    switch (msg->curr_mode)
    {
        case CSR_BT_HOLD_MODE:
            {
                opcode = HCI_HOLD_MODE;
                break;
            }
        case CSR_BT_SNIFF_MODE:
            {
                opcode = HCI_SNIFF_MODE;
                break;
            }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        case CSR_BT_PARK_MODE:
            {
                opcode = HCI_PARK_MODE;
                break;
            }
#endif
        case CSR_BT_ACTIVE_MODE:
        default:
            {
                opcode = HCI_EXIT_SNIFF_MODE;
                break;
            }
    }

    return CsrBtHciCmEventHandler(opcode, msg->status, TRUE, NULL, &(msg->handle));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvPinCodeReq(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_PIN_CODE_REQ_T * msg;

    msg = (HCI_EV_PIN_CODE_REQ_T *) prim;

    msg->bd_addr.lap                = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap                = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap                = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    return TRUE;
}

CsrBool ConvertHciEvLinkKeyReq(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_LINK_KEY_REQ_T * msg;

    msg = (HCI_EV_LINK_KEY_REQ_T *) prim;

    msg->bd_addr.lap                = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap                = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap                = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    return TRUE;
}

CsrBool ConvertHciEvLinkKeyNotification(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    CsrUint8                         i;
    HCI_EV_LINK_KEY_NOTIFICATION_T * msg;

    msg = (HCI_EV_LINK_KEY_NOTIFICATION_T *) prim;

    msg->bd_addr.lap                = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap                = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap                = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);

    for(i = 0; i < SIZE_LINK_KEY; i++)
    {
        msg->link_key[i]            = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[(6 + i)]);
    }

    msg->key_type                   = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[6 + SIZE_LINK_KEY]);
    return TRUE;
}

CsrBool ConvertHciEvDataBufferOverflow(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_DATA_BUFFER_OVERFLOW_T * msg;

    msg = (HCI_EV_DATA_BUFFER_OVERFLOW_T *) prim;

    msg->link_type                  = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    return TRUE;
}

CsrBool ConvertHciEvMaxSlotsChange(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_MAX_SLOTS_CHANGE_T * msg;

    msg = (HCI_EV_MAX_SLOTS_CHANGE_T *) prim;

    msg->handle                     = CSR_BT_CONVERT_16_FROM_STREAM(hciData);
    msg->lmp_max_slots              = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[2]);
    return TRUE;
}

CsrBool ConvertHciEvReadClockOffsetComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_READ_CLOCK_OFFSET_COMPLETE_T * msg;

    msg = (HCI_EV_READ_CLOCK_OFFSET_COMPLETE_T *) prim;

    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle                     = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->clock_offset               = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_READ_CLOCK_OFFSET,msg->status,TRUE,NULL,&(msg->handle));
#else
    return TRUE;
#endif
}

#ifndef CSR_BT_EXCLUDE_HCI_CHANGE_ACL_CONN_PKT_TYPE
CsrBool ConvertHciEvConnPacketTypeChanged(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_CONN_PACKET_TYPE_CHANGED_T * msg;

    msg = (HCI_EV_CONN_PACKET_TYPE_CHANGED_T *) prim;

    msg->status                     = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle                     = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->pkt_type                   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE    
    return (CsrBtHciCmEventHandler(HCI_CHANGE_CONN_PKT_TYPE, msg->status, TRUE, NULL, &(msg->handle)));
#else
    return TRUE;
#endif
}
#endif

CsrBool ConvertHciEvQosViolation(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_QOS_VIOLATION_T * msg;

    msg = (HCI_EV_QOS_VIOLATION_T *) prim;

    msg->handle                     = CSR_BT_CONVERT_16_FROM_STREAM(hciData);
    return TRUE;
}

CsrBool ConvertHciEvPageScanModeChange(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_PAGE_SCAN_MODE_CHANGE_T * msg;

    msg = (HCI_EV_PAGE_SCAN_MODE_CHANGE_T *) prim;

    msg->bd_addr.lap                = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap                = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap                = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    msg->page_scan_mode             = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[6]);
    return TRUE;
}

CsrBool ConvertHciEvPageScanRepModeChange(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_PAGE_SCAN_REP_MODE_CHANGE_T * msg;

    msg = (HCI_EV_PAGE_SCAN_REP_MODE_CHANGE_T *) prim;

    msg->bd_addr.lap                = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap                = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap                = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    msg->page_scan_rep_mode         = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[6]);
    return TRUE;
}

CsrBool ConvertHciEvInquiryResultWithRssi(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    CsrUint8                     i;
    HCI_EV_INQUIRY_RESULT_WITH_RSSI_T * msg;

    msg = (HCI_EV_INQUIRY_RESULT_WITH_RSSI_T *) prim;

    msg->num_responses              = CSR_BT_CONVERT_8_FROM_STREAM(hciData);

    for(i = 0; i < msg->num_responses; i++)
    {
        if (0 == i % HCI_MAX_INQ_RESULT_PER_PTR)
        {
            msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR] = CsrPmemAlloc(sizeof(HCI_INQ_RESULT_WITH_RSSI_T) * HCI_MAX_INQ_RESULT_PER_PTR);
        }
        msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].bd_addr.lap = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[1 + i * 14]);
        msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].bd_addr.uap = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[4 + i * 14]);
        msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].bd_addr.nap = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[5 + i * 14]);
        msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].page_scan_rep_mode = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[7 + i * 14]);
        msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].page_scan_period_mode = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[8 + i * 14]);
        msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].dev_class = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[9 + i * 14]);
        msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].clock_offset = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[12 + i * 14]);
        msg->result[i / HCI_MAX_INQ_RESULT_PER_PTR][i % HCI_MAX_INQ_RESULT_PER_PTR].rssi = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[14 + i * 14]);
    }
    return TRUE;
}

CsrBool ConvertHciEvReadRemExtFeaturesComplete(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_READ_REM_EXT_FEATURES_COMPLETE_T * msg;
    CsrUint8     n;
    msg = (HCI_EV_READ_REM_EXT_FEATURES_COMPLETE_T *) prim;

    msg->status   =         CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle   =         CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->page_num =         CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->max_page_num =     CSR_BT_CONVERT_8_FROM_STREAM(&hciData[4]);
    for (n=0; n<CSR_ARRAY_SIZE(msg->lmp_ext_features);n++)
    {
        msg->lmp_ext_features[n] = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[5+n*sizeof(CsrUint16)]);
    }
#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_READ_REMOTE_EXT_FEATURES,msg->status,TRUE,NULL,&(msg->handle));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvSyncConnComplete(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_SYNC_CONN_COMPLETE_T * msg;

    msg                     = (HCI_EV_SYNC_CONN_COMPLETE_T *) prim;
    msg->status             = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle             = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->bd_addr.lap        = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[3]);
    msg->bd_addr.uap        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[6]);
    msg->bd_addr.nap        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[7]);
    msg->link_type          = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[9]);
    msg->tx_interval        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[10]);
    msg->wesco              = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[11]);
    msg->rx_packet_length   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[12]);
    msg->tx_packet_length   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[14]);
    msg->air_mode           = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[16]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_SETUP_SYNCHRONOUS_CONN,msg->status,TRUE,NULL,NULL);
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvSyncConnChangedComplete(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_SYNC_CONN_CHANGED_T * msg;

    msg                     = (HCI_EV_SYNC_CONN_CHANGED_T *) prim;

    msg->status             = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle             = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->tx_interval        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->wesco              = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[4]);
    msg->rx_packet_length   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[5]);
    msg->tx_packet_length   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_SETUP_SYNCHRONOUS_CONN,msg->status,TRUE,NULL,NULL);
#else
    return TRUE;
#endif
}
/* [*] 2.1 primitives */
CsrBool ConvertHciEvSniffSubRate(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_SNIFF_SUB_RATE_T * msg;

    msg                     = (HCI_EV_SNIFF_SUB_RATE_T *) prim;

    msg->status             = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle             = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->transmit_latency   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[3]);
    msg->receive_latency    = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[5]);
    msg->remote_timeout     = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[7]);
    msg->local_timeout      = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[9]);
    return TRUE;
}

CsrBool ConvertHciEvEncryptionKeyRefreshComplete(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE_T * msg;

    msg                     = (HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE_T *) prim;

    msg->status             = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->handle             = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmEventHandler(HCI_REFRESH_ENCRYPTION_KEY, msg->status, TRUE, NULL, &(msg->handle)));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvIoCapabilityRequest(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_IO_CAPABILITY_REQUEST_T * msg;

    msg                     = (HCI_EV_IO_CAPABILITY_REQUEST_T *) prim;

    msg->bd_addr.lap        = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    return TRUE;
}

CsrBool ConvertHciEvIoCapabilityResponse(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_IO_CAPABILITY_RESPONSE_T * msg;

    msg                     = (HCI_EV_IO_CAPABILITY_RESPONSE_T *) prim;

    msg->bd_addr.lap        = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    msg->io_capability      = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[6]);
    msg->oob_data_present   = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[7]);
    msg->authentication_requirements = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[8]);
    return TRUE;
}

CsrBool ConvertHciEvUserConfirmationRequest(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_USER_CONFIRMATION_REQUEST_T * msg;

    msg                     = (HCI_EV_USER_CONFIRMATION_REQUEST_T *) prim;

    msg->bd_addr.lap        = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    msg->numeric_value      = CSR_BT_CONVERT_32_FROM_STREAM(&hciData[6]);
    return TRUE;
}

CsrBool ConvertHciEvUserPasskeyRequest(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_USER_PASSKEY_REQUEST_T * msg;

    msg                     = (HCI_EV_USER_PASSKEY_REQUEST_T *) prim;

    msg->bd_addr.lap        = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    return TRUE;
}

CsrBool ConvertHciEvRemoteOobDataRequest(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_REMOTE_OOB_DATA_REQUEST_T * msg;

    msg                     = (HCI_EV_REMOTE_OOB_DATA_REQUEST_T *) prim;

    msg->bd_addr.lap        = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    return TRUE;
}

CsrBool ConvertHciEvSimplePairingComplete(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_SIMPLE_PAIRING_COMPLETE_T * msg;

    msg                     = (HCI_EV_SIMPLE_PAIRING_COMPLETE_T *) prim;

    msg->status             = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->bd_addr.lap        = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[1]);
    msg->bd_addr.uap        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[4]);
    msg->bd_addr.nap        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[5]);
    return TRUE;
}

CsrBool ConvertHciEvLstChange(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_LST_CHANGE_T * msg;

    msg                     = (HCI_EV_LST_CHANGE_T *) prim;

    msg->handle             = CSR_BT_CONVERT_16_FROM_STREAM(hciData);
    msg->timeout            = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[2]);
    return TRUE;
}

CsrBool ConvertHciEvUserPasskeyNotification(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_USER_PASSKEY_NOTIFICATION_T * msg;

    msg                     = (HCI_EV_USER_PASSKEY_NOTIFICATION_T *) prim;

    msg->bd_addr.lap        = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    msg->passkey            = CSR_BT_CONVERT_32_FROM_STREAM(&hciData[6]);
    return TRUE;
}

CsrBool ConvertHciEvKeypressNotification(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_KEYPRESS_NOTIFICATION_T * msg;

    msg                     = (HCI_EV_KEYPRESS_NOTIFICATION_T *) prim;

    msg->bd_addr.lap        = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    msg->notification_type  = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[6]);
    return TRUE;
}

CsrBool ConvertHciEvRemHostSupportedFeatures(CsrUint8 * hciData, HCI_UPRIM_T *prim)
{
    CsrUint8                                i;
    HCI_EV_REM_HOST_SUPPORTED_FEATURES_T * msg;

    msg                     = (HCI_EV_REM_HOST_SUPPORTED_FEATURES_T *) prim;

    msg->bd_addr.lap        = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    msg->bd_addr.uap        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->bd_addr.nap        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);

    for(i = 0; i < CSR_ARRAY_SIZE(msg->host_features);i++)
    {
        msg->host_features[i] = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[6+i*sizeof(CsrUint16)]);
    }
    return TRUE;
}

#ifdef CSR_BT_LE_ENABLE
CsrBool ConvertHciEvUlpConnectionComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    TYPED_BD_ADDR_T bdaddr;
#endif
    HCI_EV_ULP_CONNECTION_COMPLETE_T *msg;
    msg = (HCI_EV_ULP_CONNECTION_COMPLETE_T*)prim;
    msg->status              = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[0]);
    msg->connection_handle   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->role                = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    msg->peer_address_type   = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[4]);
    msg->peer_address.lap    = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[5]);
    msg->peer_address.uap    = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[8]);
    msg->peer_address.nap    = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[9]);
    msg->conn_interval       = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[11]);
    msg->conn_latency        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[13]);
    msg->supervision_timeout = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[15]);
    msg->clock_accuracy      = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[17]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    tbdaddr_copy_from_bd_addr(&bdaddr, msg->peer_address_type, &msg->peer_address);
    return (CsrBtHciCmEventUlpConnectionCompleteHandler(msg->status, msg->connection_handle, &bdaddr));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvUlpAdvertisingReport(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if(!CsrBtHciCmSendRequest())
    {
        return FALSE;
    }
#endif
    {
        HCI_EV_ULP_ADVERTISING_REPORT_T *msg;
        CsrUint8 i;
        msg = (HCI_EV_ULP_ADVERTISING_REPORT_T*)prim;

        /* Warning: We currently only support _one_ advertisement data
         * block per event. */
        msg->num_reports        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[0]);
        msg->event_type         = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[1]);
        msg->address_type       = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[2]);
        msg->address.lap        = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[3]);
        msg->address.uap        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[6]);
        msg->address.nap        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[7]);
        msg->length_data        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[9]);
        for(i=0; i<msg->length_data; i++)
        {
            msg->data[i]        = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[10+i]);
        }
        msg->rssi               = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[10+i]);

        return TRUE;
    }
}

CsrBool ConvertHciEvUlpConnectionUpdateComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_ULP_CONNECTION_UPDATE_COMPLETE_T *msg;
    msg = (HCI_EV_ULP_CONNECTION_UPDATE_COMPLETE_T*)prim;
    msg->status              = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->connection_handle   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    msg->conn_interval       = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[3]);
    msg->conn_latency        = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[5]);
    msg->supervision_timeout = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[7]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_ULP_CONNECTION_UPDATE,msg->status,TRUE,NULL,&(msg->connection_handle));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvUlpReadRemoteUsedFeaturesComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_ULP_READ_REMOTE_USED_FEATURES_COMPLETE_T *msg;
    msg = (HCI_EV_ULP_READ_REMOTE_USED_FEATURES_COMPLETE_T*)prim;

    msg->status              = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
    msg->connection_handle   = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
    CsrMemCpy(msg->features, &hciData[3], CSR_ARRAY_SIZE(msg->features));

#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmEventHandler(HCI_ULP_READ_REMOTE_USED_FEATURES,msg->status,TRUE,NULL,&(msg->connection_handle));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciEvUlpLongTermKeyRequested(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_EV_ULP_LONG_TERM_KEY_REQUESTED_T *msg;
    msg = (HCI_EV_ULP_LONG_TERM_KEY_REQUESTED_T*)prim;

    msg->connection_handle   = CSR_BT_CONVERT_16_FROM_STREAM(hciData);
    CsrMemCpy(msg->random_number, &hciData[2], CSR_ARRAY_SIZE(msg->random_number));
    msg->encryption_diversifier  = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[10]);

    return TRUE;
}
#endif
