#ifndef    HCI_CONVERT_INFO_UP_STREAM_H_
#define    HCI_CONVERT_INFO_UP_STREAM_H_

#include "csr_synergy.h"

/****************************************************************************

                  (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_info_upstream.h

DESCRIPTION:    Converter for HCI Converter Informational Parameters

REVISION:        $Revision: #1 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertHciReadLocalSuppFeaturesRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciReadBdAddrRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciReadBufferSizeRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciReadLocalVersionInfoRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciReadLocalExtFeaturesRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);

#ifdef __cplusplus
}
#endif

#endif
