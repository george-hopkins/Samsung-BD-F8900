/****************************************************************************

                (c) CSR plc 2010
                All rights reserved

FILE:           csr_bt_hci_convert_ulpupstream.c

DESCRIPTION:    Converter for ULP/BLE HCI events

REVISION:       $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_util.h"

#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_pmem.h"
#include "csr_bt_hci_cm.h"
#endif

#ifdef CSR_BT_LE_ENABLE

CsrBool ConvertHciUlpReadBufferSizeRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->ulp_read_buffer_size_args.data_packet_length = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[0]);
        msg->argument_ptr->ulp_read_buffer_size_args.num_data_packets = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[2]);
        return TRUE;
    }
}

CsrBool ConvertHciUlpReadLocalSupportedFeaturesRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        CsrMemCpy(msg->argument_ptr->ulp_read_local_supported_features_args.feature_set, &hciData[0], CSR_ARRAY_SIZE(msg->argument_ptr->ulp_read_local_supported_features_args.feature_set));
        return TRUE;
    }
}

CsrBool ConvertHciUlpReadAdvertisingChannelTxPowerRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->ulp_read_advertising_channel_tx_power_args.tx_power = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[0]);
        return TRUE;
    }
}

CsrBool ConvertHciUlpReadWhiteListSizeRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->ulp_read_white_list_size_args.white_list_size = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[0]);
        return TRUE;
    }
}

CsrBool ConvertHciUlpReadChannelMapRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->ulp_read_channel_map_args.connection_handle = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[0]);
        CsrMemCpy(msg->argument_ptr->ulp_read_channel_map_args.ulp_channel_map, &hciData[2], CSR_ARRAY_SIZE(msg->argument_ptr->ulp_read_channel_map_args.ulp_channel_map));

        return TRUE;
    }
}

CsrBool ConvertHciUlpEncryptRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        CsrMemCpy(msg->argument_ptr->ulp_encrypt_args.encrypted_data, &hciData[0], CSR_ARRAY_SIZE(msg->argument_ptr->ulp_encrypt_args.encrypted_data));
        return TRUE;
    }
}

CsrBool ConvertHciUlpRandRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        CsrMemCpy(msg->argument_ptr->ulp_rand_args.random_number, &hciData[0], CSR_ARRAY_SIZE(msg->argument_ptr->ulp_rand_args.random_number));

        return TRUE;
    }
}

CsrBool ConvertHciUlpLongTermKeyRequestedReplyRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    hci_connection_handle_t handle = CSR_BT_CONVERT_16_FROM_STREAM(hciData);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&handle))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->ulp_long_term_key_requested_reply_args.connection_handle = handle;
        return TRUE;
    }
}

CsrBool ConvertHciUlpLongTermKeyRequestedNegativeReplyRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    hci_connection_handle_t handle = CSR_BT_CONVERT_16_FROM_STREAM(hciData);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&handle))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->ulp_long_term_key_requested_negative_reply_args.connection_handle = handle;
        return TRUE;
    }
}

CsrBool ConvertHciUlpReadSupportedStatesRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        CsrMemCpy(msg->argument_ptr->ulp_read_supported_states_args.supported_states, &hciData[0], CSR_ARRAY_SIZE(msg->argument_ptr->ulp_read_supported_states_args.supported_states));

        return TRUE;
    }
}

CsrBool ConvertHciUlpTestEndRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->ulp_test_end_args.number_of_packets = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[0]);
        return TRUE;
    }
}

#endif

