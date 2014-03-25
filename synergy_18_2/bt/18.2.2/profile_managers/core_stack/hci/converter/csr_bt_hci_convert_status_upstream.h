#ifndef    HCI_CONVERT_STATUS_UP_STREAM_H_
#define    HCI_CONVERT_STATUS_UP_STREAM_H_

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_status_upstream.h

DESCRIPTION:            converter for _hci _link _policy commands and events

REVISION:        $Revision: #1 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertHciReadAfhChannelMapRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciReadClockRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciGetLinkQualityRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciReadRssiRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciReadFailedContactCountRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciReadEncryptionKeySizeRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);

#ifdef __cplusplus
}
#endif

#endif
