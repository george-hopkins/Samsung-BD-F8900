/****************************************************************************

                  (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_lpdownstream.c

DESCRIPTION:    Converter for HCI Link Policy commands and events

REVISION:        $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_lpdownstream.h"

#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_hci_cm.h"
#endif

CsrBool ConvertHciSniffMode(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_SNIFF_MODE_T * msg;

    msg = (HCI_SNIFF_MODE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->max_interval, &buffer[5]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->min_interval, &buffer[7]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->attempt, &buffer[9]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->timeout, &buffer[11]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}

CsrBool ConvertHciExitSniffMode(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_EXIT_SNIFF_MODE_T * msg;

    msg = (HCI_EXIT_SNIFF_MODE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}

#ifndef CSR_BT_EXCLUDE_HCI_PARK_MODE
CsrBool ConvertHciParkMode(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_PARK_MODE_T * msg;

    msg = (HCI_PARK_MODE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->max_interval, &buffer[5]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->min_interval, &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}
#endif

#ifndef CSR_BT_EXCLUDE_HCI_EXIT_PARK_MODE
CsrBool ConvertHciExitParkMode(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_EXIT_PARK_MODE_T * msg;

    msg = (HCI_EXIT_PARK_MODE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,
                                                  HCI_EXIT_SNIFF_MODE, /* As HCI_EXIT_PARK_MODE and HCI_EXIT_SNIFF_MODE use the same command complete signal this value is hardcoded */
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}
#endif

#ifndef CSR_BT_EXCLUDE_HCI_HOLD_MODE
CsrBool ConvertHciHoldMode(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_HOLD_MODE_T * msg;

    msg = (HCI_HOLD_MODE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->max_interval, &buffer[5]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->min_interval, &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}
#endif

CsrBool ConvertHciQosSetup(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_QOS_SETUP_T * msg;

    msg = (HCI_QOS_SETUP_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->flags, &buffer[5]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->service_type, &buffer[6]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->token_rate, &buffer[7]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->peak_bandwidth, &buffer[11]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->latency, &buffer[15]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->delay_variation, &buffer[19]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}

CsrBool ConvertHciRoleDiscovery(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_ROLE_DISCOVERY_T * msg;

    msg = (HCI_ROLE_DISCOVERY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}

CsrBool ConvertHciSwitchRole(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    HCI_SWITCH_ROLE_T * msg;

    msg = (HCI_SWITCH_ROLE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->role, &buffer[9]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  bdaddr,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciWriteLinkPolicySettings(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_WRITE_LINK_POLICY_SETTINGS_T * msg;

    msg = (HCI_WRITE_LINK_POLICY_SETTINGS_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->link_policy_settings, &buffer[5]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}

CsrBool ConvertHciWriteDefaultLinkPolicySettings(CONVERT_PARAMS)
{
    HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_T * msg;

    msg = (HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->default_lps, &buffer[3]);
    
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 

}

CsrBool ConvertHciSniffSubRate(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_SNIFF_SUB_RATE_T * msg;

    msg = (HCI_SNIFF_SUB_RATE_T *) prim;
    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->max_remote_latency, &buffer[5]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->min_remote_timeout, &buffer[7]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->min_local_timeout, &buffer[9]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}
