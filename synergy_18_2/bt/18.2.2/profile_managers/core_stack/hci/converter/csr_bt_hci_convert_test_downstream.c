/****************************************************************************

                (c) CSR plc 2007

                All rights reserved

FILE:            csr_bt_hci_convert_test_downstream.c

DESCRIPTION:    Converter for HCI Converter Informational Parameters

REVISION:        $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_test_downstream.h"
#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_bt_hci_cm.h"
#endif

CsrBool ConvertHciEnableDutMode(CONVERT_PARAMS)
{
    HCI_ENABLE_DUT_MODE_T *msg;

    msg = (HCI_ENABLE_DUT_MODE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciWriteSimplePairingDebugMode(CONVERT_PARAMS)
{
    HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE_T *msg;

    msg = (HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->simple_pairing_debug_mode, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

