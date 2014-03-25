/****************************************************************************
                (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_status_downstream.c

DESCRIPTION:    Converter for HCI Converter. Informational Parameters

REVISION:        $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_status_downstream.h"
#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_pmem.h"
#include "csr_bt_hci_cm.h"
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_AFH_CHANNEL_MAP
CsrBool ConvertReadAfhChannelMap(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_READ_AFH_CHANNEL_MAP_T *msg;

    msg = (HCI_READ_AFH_CHANNEL_MAP_T *) prim;

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
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_CLOCK
CsrBool ConvertHciReadClock(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
    hci_connection_handle_t handleTmp;
#endif
    HCI_READ_CLOCK_T *msg;

    msg =(HCI_READ_CLOCK_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->which_clock, &buffer[5]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handleTmp = msg->which_clock == 0 ? 0 : msg->handle; /* Needed to support VC6 */
    handle = CsrBtHciConnectionHandleDup(handleTmp);
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

#ifndef CSR_BT_EXCLUDE_HCI_GET_LINK_QUALITY
CsrBool ConvertHciGetLinkQuality(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_GET_LINK_QUALITY_T *msg;

    msg =(HCI_GET_LINK_QUALITY_T *) prim;

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
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_RSSI
CsrBool ConvertHciReadRssi(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_READ_RSSI_T *msg;

    msg =(HCI_READ_RSSI_T *) prim;

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
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_FAILED_CONTACT_COUNT
CsrBool ConvertHciReadFailedContactCount(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_READ_FAILED_CONTACT_COUNT_T *msg;

    msg =(HCI_READ_FAILED_CONTACT_COUNT_T *) prim;

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
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_ENCRYPTION_KEY_SIZE
CsrBool ConvertHciReadEncryptionKeySize(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_READ_ENCRYPTION_KEY_SIZE_T *msg;

    msg = (HCI_READ_ENCRYPTION_KEY_SIZE_T*)prim;

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
#endif

