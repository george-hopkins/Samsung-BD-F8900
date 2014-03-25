#ifndef HCI_CONVERT_LM_UP_STREAM_H_
#define HCI_CONVERT_LM_UP_STREAM_H_

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:           csr_bt_hci_convert_lmupstream.h

DESCRIPTION:    converter for _hci _lm commands and events

REVISION:       $Revision: #1 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertHciCreateConnectionCancelRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciRemoteNameReqCancelRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciLinkKeyReqRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);


#ifdef __cplusplus
}
#endif

#endif
