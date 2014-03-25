/****************************************************************************

                  (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_info_downstream.c

DESCRIPTION:    Converter for HCI Converter Informational Parameters

REVISION:        $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_info_downstream.h"
#include "csr_bt_hci_cm.h"

CsrBool ConvertHciReadLocalSuppFeatures(CONVERT_PARAMS)
{
    HCI_READ_LOCAL_SUPP_FEATURES_T * msg;

    msg = (HCI_READ_LOCAL_SUPP_FEATURES_T *) prim;

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

CsrBool ConvertHciReadBdAddr(CONVERT_PARAMS)
{
    HCI_READ_BD_ADDR_T * msg;

    msg = (HCI_READ_BD_ADDR_T *) prim;

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

CsrBool ConvertHciReadBufferSize(CONVERT_PARAMS)
{
    HCI_READ_BUFFER_SIZE_T * msg;

    msg = (HCI_READ_BUFFER_SIZE_T *) prim;

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

CsrBool ConvertHciReadLocalVersionInfo(CONVERT_PARAMS)
{
    HCI_READ_LOCAL_VER_INFO_T * msg;

    msg = (HCI_READ_LOCAL_VER_INFO_T *) prim;

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

#ifndef CSR_BT_EXCLUDE_HCI_READ_LOCAL_EXT_FEATURES
CsrBool ConvertHciReadLocalExtFeatures(CONVERT_PARAMS)
{
    HCI_READ_LOCAL_EXT_FEATURES_T * msg;

    msg = (HCI_READ_LOCAL_EXT_FEATURES_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, buffer+2);
    CSR_BT_CONVERT_8_TO_STREAM(msg->page_num, buffer+3);
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
#endif
