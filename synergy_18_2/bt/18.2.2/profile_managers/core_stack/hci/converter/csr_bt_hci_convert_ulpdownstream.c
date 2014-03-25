/****************************************************************************

                (c) CSR plc 2010
                All rights reserved

FILE:           csr_bt_hci_convert_ulpdownstream.c

DESCRIPTION:    Converter for ULP/BLE HCI commands

REVISION:       $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_ulpdownstream.h"
#include "csr_bt_util.h"

#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_pmem.h"
#include "csr_bt_hci_cm.h"
#include "csr_hci_task.h"
#include "dm_hci_interface.h"
#include "tbdaddr.h"
#endif

#ifdef CSR_BT_LE_ENABLE

CsrBool ConvertHciUlpSetEventMask(CONVERT_PARAMS)
{
    HCI_ULP_SET_EVENT_MASK_T *msg;
    msg = (HCI_ULP_SET_EVENT_MASK_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);

    CSR_BT_CONVERT_32_TO_STREAM(msg->ulp_event_mask[0], &buffer[3]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->ulp_event_mask[1], &buffer[7]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpReadBufferSize(CONVERT_PARAMS)
{
    HCI_ULP_READ_BUFFER_SIZE_T *msg;
    msg = (HCI_ULP_READ_BUFFER_SIZE_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpReadLocalSupportedFeatures(CONVERT_PARAMS)
{
    HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_T *msg;
    msg = (HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpSetRandomAddress(CONVERT_PARAMS)
{
    HCI_ULP_SET_RANDOM_ADDRESS_T *msg;
    msg = (HCI_ULP_SET_RANDOM_ADDRESS_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->random_address.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->random_address.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->random_address.nap, &buffer[7]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpSetAdvertisingParameters(CONVERT_PARAMS)
{
    HCI_ULP_SET_ADVERTISING_PARAMETERS_T *msg;
    msg = (HCI_ULP_SET_ADVERTISING_PARAMETERS_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->adv_interval_min, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->adv_interval_max, &buffer[5]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->advertising_type, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->own_address_type, &buffer[8]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->direct_address_type, &buffer[9]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->direct_address.lap, &buffer[10]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->direct_address.uap, &buffer[13]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->direct_address.nap, &buffer[14]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->advertising_channel_map, &buffer[16]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->advertising_filter_policy, &buffer[17]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpReadAdvertisingChannelTxPower(CONVERT_PARAMS)
{
    HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_T *msg;
    msg = (HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpSetAdvertisingData(CONVERT_PARAMS)
{
    HCI_ULP_SET_ADVERTISING_DATA_T *msg;
    CsrUint8 i;
    msg = (HCI_ULP_SET_ADVERTISING_DATA_T*)prim;
    
    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->advertising_data_len, &buffer[3]);
    for(i=0; i<msg->advertising_data_len; i++)
    {
        CSR_BT_CONVERT_8_TO_STREAM(msg->advertising_data[i], &buffer[4+i]);
    }

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpSetScanResponseData(CONVERT_PARAMS)
{
    HCI_ULP_SET_SCAN_RESPONSE_DATA_T *msg;
    CsrUint8 i;
    msg = (HCI_ULP_SET_SCAN_RESPONSE_DATA_T*)prim;
    
    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->scan_response_data_len, &buffer[3]);
    for(i=0; i<msg->scan_response_data_len; i++)
    {
        CSR_BT_CONVERT_8_TO_STREAM(msg->scan_response_data[i], &buffer[4+i]);
    }

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpSetAdvertiseEnable(CONVERT_PARAMS)
{
    HCI_ULP_SET_ADVERTISE_ENABLE_T *msg;
    msg = (HCI_ULP_SET_ADVERTISE_ENABLE_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->advertising_enable, &buffer[3]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysToggleHandler(buffer, msg->common.op_code,
                                                       (CsrBool) !msg->advertising_enable));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpSetScanParameters(CONVERT_PARAMS)
{
    HCI_ULP_SET_SCAN_PARAMETERS_T *msg;
    msg = (HCI_ULP_SET_SCAN_PARAMETERS_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->scan_type, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->scan_interval, &buffer[4]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->scan_window, &buffer[6]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->own_address_type, &buffer[8]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->scanning_filter_policy, &buffer[9]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpSetScanEnable(CONVERT_PARAMS)
{
    HCI_ULP_SET_SCAN_ENABLE_T *msg;
    msg = (HCI_ULP_SET_SCAN_ENABLE_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->scan_enable, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->filter_duplicates, &buffer[4]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysToggleHandler(buffer, msg->common.op_code,
                                                       (CsrBool) !msg->scan_enable));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

#ifdef CSR_CHIP_MANAGER_ENABLE
void csrBcCmHciUlpCreateConStatusEventHandler(CsrUint8 numHciCommandPackets, hci_return_t hciStatus)
{
    CsrBtHciCmHciCommonStatusEventHandler(numHciCommandPackets, hciStatus, HCI_ULP_CREATE_CONNECTION);
}

void csrBcCmHciUlpConnectionCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_ULP_CONNECTION_COMPLETE_T evt;
    TYPED_BD_ADDR_T *bdaddr = (TYPED_BD_ADDR_T *) cmdArg;
    evt.event.event_code      = HCI_EV_ULP;
    evt.event.ulp_sub_opcode  = HCI_EV_ULP_CONNECTION_COMPLETE;
    evt.event.length          = HCI_EV_ULP_CONNECTION_COMPLETE_PARAM_LEN;
    evt.status                = HCI_ERROR_CONN_TIMEOUT;
    evt.connection_handle     = 0;
    evt.role                  = 0;
    evt.peer_address_type     = BDADDRT_TYPE(*bdaddr);
    evt.peer_address          = BDADDRT_ADDR(*bdaddr);
    evt.conn_interval         = 0;
    evt.conn_latency          = 0;
    evt.supervision_timeout   = 0;
    evt.clock_accuracy        = 0;
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
    CSR_UNUSED(numHciCommandPackets);
}
#endif

CsrBool ConvertHciUlpCreateConnection(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    TYPED_BD_ADDR_T *bdaddr;
#endif
    HCI_ULP_CREATE_CONNECTION_T *msg;
    msg = (HCI_ULP_CREATE_CONNECTION_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->scan_interval, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->scan_window, &buffer[5]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->initiator_filter_policy, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->peer_address_type, &buffer[8]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->peer_address.lap, &buffer[9]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->peer_address.uap, &buffer[12]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->peer_address.nap, &buffer[13]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->own_address_type, &buffer[15]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->conn_interval_min, &buffer[16]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->conn_interval_max, &buffer[18]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->conn_latency, &buffer[20]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->supervision_timeout, &buffer[22]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->minimum_ce_length, &buffer[24]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->maximum_ce_length, &buffer[26]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = (TYPED_BD_ADDR_T *) CsrPmemAlloc(sizeof(TYPED_BD_ADDR_T));
    tbdaddr_copy_from_bd_addr(bdaddr, msg->peer_address_type, &msg->peer_address);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr,
                                                msg->common.op_code,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                csrBcCmHciUlpCreateConStatusEventHandler,
                                                csrBcCmHciUlpConnectionCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddrt));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpCreateConnectionCancel(CONVERT_PARAMS)
{
    HCI_ULP_CREATE_CONNECTION_CANCEL_T *msg;
    msg = (HCI_ULP_CREATE_CONNECTION_CANCEL_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpReadWhiteListSize(CONVERT_PARAMS)
{
    HCI_ULP_READ_WHITE_LIST_SIZE_T *msg;
    msg = (HCI_ULP_READ_WHITE_LIST_SIZE_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpClearWhiteList(CONVERT_PARAMS)
{
    HCI_ULP_CLEAR_WHITE_LIST_T *msg;
    msg = (HCI_ULP_CLEAR_WHITE_LIST_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpAddDeviceToWhiteList(CONVERT_PARAMS)
{
    HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_T *msg;
    msg = (HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->address_type, &buffer[3]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->address.lap, &buffer[4]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->address.uap, &buffer[7]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->address.nap, &buffer[8]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpRemoveDeviceFromWhiteList(CONVERT_PARAMS)
{
    HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_T *msg;
    msg = (HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->address_type, &buffer[3]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->address.lap, &buffer[4]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->address.uap, &buffer[7]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->address.nap, &buffer[8]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpConnectionUpdate(CONVERT_PARAMS)
{
    HCI_ULP_CONNECTION_UPDATE_T *msg;
    msg = (HCI_ULP_CONNECTION_UPDATE_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->connection_handle, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->conn_interval_min, &buffer[5]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->conn_interval_max, &buffer[7]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->conn_latency, &buffer[9]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->supervision_timeout, &buffer[11]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->minimum_ce_length, &buffer[13]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->maximum_ce_length, &buffer[15]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpSetHostChannelClassification(CONVERT_PARAMS)
{
    HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_T *msg;
    msg = (HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CsrMemCpy(&buffer[3], msg->channel_map, CSR_ARRAY_SIZE(msg->channel_map));

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpReadChannelMap(CONVERT_PARAMS)
{
    HCI_ULP_READ_CHANNEL_MAP_T *msg;
    msg = (HCI_ULP_READ_CHANNEL_MAP_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->connection_handle, &buffer[3]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpReadRemoteUsedFeatures(CONVERT_PARAMS)
{
    HCI_ULP_READ_REMOTE_USED_FEATURES_T *msg;
    msg = (HCI_ULP_READ_REMOTE_USED_FEATURES_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->connection_handle, &buffer[3]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpEncrypt(CONVERT_PARAMS)
{
    HCI_ULP_ENCRYPT_T *msg;
    msg = (HCI_ULP_ENCRYPT_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CsrMemCpy(&buffer[3], msg->aes_key, CSR_ARRAY_SIZE(msg->aes_key));
    CsrMemCpy(&buffer[19], msg->plaintext_data, CSR_ARRAY_SIZE(msg->plaintext_data));

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpRand(CONVERT_PARAMS)
{
    HCI_ULP_RAND_T *msg;
    msg = (HCI_ULP_RAND_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpStartEncryption(CONVERT_PARAMS)
{
    HCI_ULP_START_ENCRYPTION_T *msg;
    msg = (HCI_ULP_START_ENCRYPTION_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->connection_handle, &buffer[3]);
    CsrMemCpy(&buffer[5], msg->random_number, CSR_ARRAY_SIZE(msg->random_number));
    CSR_BT_CONVERT_16_TO_STREAM(msg->encrypted_diversifier, &buffer[13]);
    CsrMemCpy(&buffer[15], msg->long_term_key, CSR_ARRAY_SIZE(msg->long_term_key));

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpLongTermKeyRequestedReply(CONVERT_PARAMS)
{
    HCI_ULP_LONG_TERM_KEY_REQUESTED_REPLY_T *msg;
    msg = (HCI_ULP_LONG_TERM_KEY_REQUESTED_REPLY_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->connection_handle, &buffer[3]);
    CsrMemCpy(&buffer[5], msg->long_term_key, CSR_ARRAY_SIZE(msg->long_term_key));

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpLongTermKeyRequestedNegativeReply(CONVERT_PARAMS)
{
    HCI_ULP_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_T *msg;
    msg = (HCI_ULP_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->connection_handle, &buffer[3]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpReadSupportedStates(CONVERT_PARAMS)
{
    HCI_ULP_READ_SUPPORTED_STATES_T *msg;
    msg = (HCI_ULP_READ_SUPPORTED_STATES_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpReceiverTest(CONVERT_PARAMS)
{
    HCI_ULP_RECEIVER_TEST_T *msg;
    msg = (HCI_ULP_RECEIVER_TEST_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->rx_channel, &buffer[3]);

#ifdef CSR_CHIP_MANAGER_ENABLE
#error Not implemented
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpTransmitterTest(CONVERT_PARAMS)
{
    HCI_ULP_TRANSMITTER_TEST_T *msg;
    msg = (HCI_ULP_TRANSMITTER_TEST_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->tx_channel, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->length_test_data, &buffer[4]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->packet_payload, &buffer[5]);

#ifdef CSR_CHIP_MANAGER_ENABLE
#error Not implemented
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUlpTestEnd(CONVERT_PARAMS)
{
    HCI_ULP_TEST_END_T *msg;
    msg = (HCI_ULP_TEST_END_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);

#ifdef CSR_CHIP_MANAGER_ENABLE
#error Not implemented
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

#endif /* CSR_BT_LE_ENABLE */
