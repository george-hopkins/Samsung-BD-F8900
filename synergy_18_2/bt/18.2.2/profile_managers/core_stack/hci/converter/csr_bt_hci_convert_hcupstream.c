/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:           csr_bt_hci_convert_hcupstream.c

DESCRIPTION:    Converter for HCI Host Controller and Baseband commands and events

REVISION:       $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_hcupstream.h"
#include "csr_bt_hci_cm.h"

CsrBool ConvertHciReadScanEnableRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_scan_enable_args.scan_enable = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
        return TRUE;
    }
}

CsrBool ConvertHciDeleteStoredLinkKeyRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->delete_stored_link_key_args.num_keys_deleted = CSR_BT_CONVERT_16_FROM_STREAM(hciData);
        return TRUE;
    }
}

#ifndef CSR_BT_EXCLUDE_HCI_READ_CLASS_OF_DEVICE
CsrBool ConvertReadClassOfDeviceRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_class_device_args.dev_class = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
        return TRUE;
    }
}
#endif

CsrBool ConvertWriteLinkSupervTimeoutRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    hci_connection_handle_t handle = CSR_BT_CONVERT_16_FROM_STREAM(hciData);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&handle))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->write_link_superv_timeout_args.handle = handle;
        return TRUE;
    }
}

#ifndef CSR_BT_EXCLUDE_HCI_READ_TX_POWER_LEVEL
CsrBool ConvertReadTxPowerLevelRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    hci_connection_handle_t handle = CSR_BT_CONVERT_16_FROM_STREAM(hciData);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&handle))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_tx_power_level_args.handle      = handle;
        msg->argument_ptr->read_tx_power_level_args.pwr_level   = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[2]);
        return TRUE;
    }
}
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_LOCAL_NAME
CsrBool ConvertReadLocalNameRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        CsrUint8     i;
        CsrUint16    nameLength;

        nameLength = msg->event.length-4;
        for(i = 0; i < nameLength; i++)
        {
            if(0 == i % HCI_LOCAL_NAME_BYTES_PER_PTR)
            {
                msg->argument_ptr->read_local_name_args.name_part[i / HCI_LOCAL_NAME_BYTES_PER_PTR] = CsrPmemAlloc(HCI_LOCAL_NAME_BYTES_PER_PTR);
            }
            msg->argument_ptr->read_local_name_args.name_part[i / HCI_LOCAL_NAME_BYTES_PER_PTR][i % HCI_LOCAL_NAME_BYTES_PER_PTR] = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[i]);
        }
        return TRUE;
    }
}
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_AFH_CHANNEL_CLASS_M
CsrBool ConvertReadAfhChannelClassMRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_afh_channel_class_m_args.class_mode = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
        return TRUE;
    }
}
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_NUM_SUPPORTED_IAC
CsrBool ConvertHciReadNumSupportedIacRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_num_supported_iac_args.num = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
        return TRUE;
    }
}
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_CURRENT_IAC_LAP
CsrBool ConvertHciReadCurrentIacLapRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        CsrUint8     i;
        CsrUint8 numIac;

        numIac = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
        msg->argument_ptr->read_current_iac_lap_args.num_current_iac = numIac;
        for(i = 0; i < numIac; i++)
        {
            if(0 == i % HCI_IAC_LAP_PER_PTR)
            {
                msg->argument_ptr->read_current_iac_lap_args.iac_lap[i / HCI_IAC_LAP_PER_PTR] = CsrPmemAlloc(sizeof(CsrUint24) * HCI_IAC_LAP_PER_PTR);
            }
            msg->argument_ptr->read_current_iac_lap_args.iac_lap[i / HCI_IAC_LAP_PER_PTR][i % HCI_IAC_LAP_PER_PTR] = CSR_BT_CONVERT_24_FROM_STREAM(&hciData[i * 3 + 1]);
        }
        return TRUE;
    }
}
#endif

#ifndef CSR_BT_EXCLUDE_SM_READ_LOCAL_OOB_DATA
CsrBool ConvertHciReadLocalOobDataRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        CsrUint8 i;

        for(i = 0; i < SIZE_OOB_DATA; i++)
        {
            msg->argument_ptr->read_local_oob_data_args.c[i] = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[i]);
        }

        for(i = 0; i < SIZE_OOB_DATA; i++)
        {
            msg->argument_ptr->read_local_oob_data_args.r[i] = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[i+SIZE_OOB_DATA]);
        }
        return TRUE;
    }
}
#endif

#ifndef CSR_BT_EXCLUDE_SM_SEND_KEYPRESS_NOTIFICATION
CsrBool ConvertHciSendKeypressNotificationRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    BD_ADDR_T bd_addr;

    bd_addr.lap = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    bd_addr.uap = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    bd_addr.nap = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&bd_addr))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->send_keypress_notification_args.bd_addr.lap = bd_addr.lap; 
        msg->argument_ptr->send_keypress_notification_args.bd_addr.uap = bd_addr.uap;
        msg->argument_ptr->send_keypress_notification_args.bd_addr.nap = bd_addr.nap;
        return TRUE;
    }
}
#endif
