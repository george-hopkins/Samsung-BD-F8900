#ifndef    HCI_CONVERT_STATUS_DOWN_STREAM_H_
#define    HCI_CONVERT_STATUS_DOWN_STREAM_H_

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_status_downstream.h

DESCRIPTION:            converter for _hci _converter _status _parameters

REVISION:        $Revision: #1 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertReadAfhChannelMap(CONVERT_PARAMS);
CsrBool ConvertHciReadClock(CONVERT_PARAMS);
CsrBool ConvertHciGetLinkQuality(CONVERT_PARAMS);
CsrBool ConvertHciReadRssi(CONVERT_PARAMS);
CsrBool ConvertHciReadFailedContactCount(CONVERT_PARAMS);
CsrBool ConvertHciReadEncryptionKeySize(CONVERT_PARAMS);

#ifdef __cplusplus
}
#endif

#endif
