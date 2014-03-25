/****************************************************************************

                  (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_status_upstream.c

DESCRIPTION:            converter for _hci _status events

REVISION:        $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_status_upstream.h"
#include "csr_util.h"

#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_pmem.h"
#include "csr_bt_hci_cm.h"
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_AFH_CHANNEL_MAP
CsrBool ConvertHciReadAfhChannelMapRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    hci_connection_handle_t handle;

    handle = CSR_BT_CONVERT_16_FROM_STREAM(hciData);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&handle))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_afh_channel_map_args.handle = handle;
        msg->argument_ptr->read_afh_channel_map_args.mode = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[2]);
        CsrMemCpy(msg->argument_ptr->read_afh_channel_map_args.map, &hciData[3], CSR_ARRAY_SIZE(msg->argument_ptr->read_afh_channel_map_args.map));

        return TRUE;
    }
}
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_CLOCK
CsrBool ConvertHciReadClockRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    hci_connection_handle_t handle;

    handle = CSR_BT_CONVERT_16_FROM_STREAM(hciData);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&handle))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_clock_args.handle   = handle;
        msg->argument_ptr->read_clock_args.clock = CSR_BT_CONVERT_32_FROM_STREAM(&hciData[2]);
        msg->argument_ptr->read_clock_args.accuracy = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[6]);
        return TRUE;
    }
}
#endif

#ifndef CSR_BT_EXCLUDE_HCI_GET_LINK_QUALITY
CsrBool ConvertHciGetLinkQualityRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    hci_connection_handle_t handle;

    handle            = CSR_BT_CONVERT_16_FROM_STREAM(hciData);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&handle))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->get_link_quality_args.handle          = handle;
        msg->argument_ptr->get_link_quality_args.link_quality    = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[2]);
        return TRUE;
    }
}
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_RSSI
CsrBool ConvertHciReadRssiRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    hci_connection_handle_t handle;

    handle = CSR_BT_CONVERT_16_FROM_STREAM(hciData);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&handle))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_rssi_args.handle    = handle;
        msg->argument_ptr->read_rssi_args.rssi      = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[2]);
        return TRUE;
    }
}
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_FAILED_CONTACT_COUNT
CsrBool ConvertHciReadFailedContactCountRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    hci_connection_handle_t handle;

    handle = CSR_BT_CONVERT_16_FROM_STREAM(hciData);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&handle))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_failed_contact_count_args.handle               = handle;
        msg->argument_ptr->read_failed_contact_count_args.failed_contact_count = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[2]);
        return TRUE;
    }
}
#endif
#ifndef CSR_BT_EXCLUDE_HCI_READ_ENCRYPTION_KEY_SIZE
CsrBool ConvertHciReadEncryptionKeySizeRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    hci_connection_handle_t handle;

    handle = CSR_BT_CONVERT_16_FROM_STREAM(hciData);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&handle))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_encryption_key_size_args.handle = handle;
        msg->argument_ptr->read_encryption_key_size_args.key_size = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[2]);
        return TRUE;
    }
}
#endif
