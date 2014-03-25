/****************************************************************************

                  (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_lpupstream.c

DESCRIPTION:    Converter for HCI Link Policy commands and events

REVISION:        $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "csr_pmem.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_lpupstream.h"
#include "csr_bt_hci_cm.h"

CsrBool ConvertHciRoleDiscoveryRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    hci_connection_handle_t handle;
    hci_role_t              current_role;

    handle = CSR_BT_CONVERT_16_FROM_STREAM(hciData);
    current_role = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[2]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&handle))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->role_discovery_args.handle       = handle;
        msg->argument_ptr->role_discovery_args.current_role = current_role;
        return TRUE;
    }
}

CsrBool ConvertHciWriteLinkPolicySettingsRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
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
        msg->argument_ptr->write_link_policy_settings_args.handle = handle;
        return TRUE;
    }
}

CsrBool ConvertHciSniffSubrateRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
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
        msg->argument_ptr->sniff_sub_rate_args.handle = handle;
        return TRUE;
    }
}

