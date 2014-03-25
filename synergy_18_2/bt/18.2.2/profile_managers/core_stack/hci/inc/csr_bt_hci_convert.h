#ifndef    HCI_CONVERT_H_
#define    HCI_CONVERT_H_

#include "csr_synergy.h"

/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert.h

DESCRIPTION:    A top level converter for _hci commands and events

REVISION:        $Revision: #2 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONVERT_CALLING_PARAMS  prim, buffer
#define CONVERT_PARAMS          HCI_UPRIM_T *prim, CsrUint8 *buffer

#define CSR_BT_CONVERT_8_TO_STREAM(value, pointer) \
    ((CsrUint8 *) pointer)[0] = (value)
#define CSR_BT_CONVERT_16_TO_STREAM(value, pointer) \
    ((CsrUint8 *) pointer)[0] = ((CsrUint8) ((value) & 0x00FF)); \
    ((CsrUint8 *) pointer)[1] = ((CsrUint8) ((value) >> 8))
#define CSR_BT_CONVERT_24_TO_STREAM(value, pointer) \
    ((CsrUint8 *) pointer)[0] = ((CsrUint8) ((value) & 0x000000FF)); \
    ((CsrUint8 *) pointer)[1] = ((CsrUint8) (((value) >> 8) & 0x000000FF)); \
    ((CsrUint8 *) pointer)[2] = ((CsrUint8) (((value) >> 16) & 0x000000FF))
#define CSR_BT_CONVERT_32_TO_STREAM(value, pointer) \
    ((CsrUint8 *) pointer)[0] = ((CsrUint8) ((value) & 0x000000FF)); \
    ((CsrUint8 *) pointer)[1] = ((CsrUint8) (((value) >> 8) & 0x000000FF)); \
    ((CsrUint8 *) pointer)[2] = ((CsrUint8) (((value) >> 16) & 0x000000FF)); \
    ((CsrUint8 *) pointer)[3] = ((CsrUint8) (((value) >> 24) & 0x000000FF))

#define CSR_BT_CONVERT_8_FROM_STREAM(pointer) \
    ((CsrUint8 *) pointer)[0]
#define CSR_BT_CONVERT_16_FROM_STREAM(pointer) \
    ((CsrUint16) ((CsrUint8 *) pointer)[1]) << 8 | \
    ((CsrUint16) ((CsrUint8 *) pointer)[0])
#define CSR_BT_CONVERT_24_FROM_STREAM(pointer) \
    ((CsrUint32) ((CsrUint8 *) pointer)[2]) << 16 | \
    ((CsrUint32) ((CsrUint8 *) pointer)[1]) << 8 | \
    ((CsrUint32) ((CsrUint8 *) pointer)[0])
#define CSR_BT_CONVERT_32_FROM_STREAM(pointer) \
    ((CsrUint32) ((CsrUint8 *) pointer)[3]) << 24 | \
    ((CsrUint32) ((CsrUint8 *) pointer)[2]) << 16 | \
    ((CsrUint32) ((CsrUint8 *) pointer)[1]) << 8 | \
    ((CsrUint32) ((CsrUint8 *) pointer)[0])

CsrBool CsrBtConvertHciCommand(HCI_UPRIM_T *prim, CsrUint8 * buffer, CsrBool *sendHciCmd);
HCI_UPRIM_T *CsrBtConvertHciEvent(CsrUint8 *hciData);

#ifdef CSR_CHIP_MANAGER_ENABLE
CsrBool CsrBtHciCmCompareConnectionHandles(void *cmdArg, const void *eventArg);
CsrBool CsrBtHciCmCompareBdaddr(void *cmdArg, const void *eventArg);
#ifdef CSR_BT_LE_ENABLE
CsrBool CsrBtHciCmCompareBdaddrt(void *cmdArg, const void *eventArg);
#endif
void CsrBtHciCmHciCommonStatusEventHandler(CsrUint8 numHciCommandPackets, hci_return_t hciStatus, hci_op_code_t opcode);
hci_connection_handle_t *CsrBtHciConnectionHandleDup(hci_connection_handle_t handle);
#endif


#ifdef __cplusplus
}
#endif

#endif
