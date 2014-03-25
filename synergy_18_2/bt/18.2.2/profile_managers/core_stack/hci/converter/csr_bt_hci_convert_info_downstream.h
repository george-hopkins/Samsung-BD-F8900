#ifndef    HCI_CONVERT_INFO_DOWN_STREAM_H_
#define    HCI_CONVERT_INFO_DOWN_STREAM_H_

#include "csr_synergy.h"

/****************************************************************************

                  (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_info_downstream.h

DESCRIPTION:    Converter for HCI Converter Informational Parameters

REVISION:        $Revision: #1 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertHciReadLocalSuppFeatures(CONVERT_PARAMS);
CsrBool ConvertHciReadBdAddr(CONVERT_PARAMS);
CsrBool ConvertHciReadBufferSize(CONVERT_PARAMS);
CsrBool ConvertHciReadLocalVersionInfo(CONVERT_PARAMS);
CsrBool ConvertHciReadLocalExtFeatures(CONVERT_PARAMS);

#ifdef __cplusplus
}
#endif

#endif
