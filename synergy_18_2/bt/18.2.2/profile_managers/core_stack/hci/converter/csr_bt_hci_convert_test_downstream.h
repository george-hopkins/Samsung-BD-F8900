#ifndef HCI_CONVERT_TEST_DOWN_STREAM_H_
#define HCI_CONVERT_TEST_DOWN_STREAM_H_

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2007

                All rights reserved

FILE:            csr_bt_hci_convert_test_downstream.h

DESCRIPTION:    Converter for HCI Converter Informational Parameters

REVISION:        $Revision: #1 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertHciEnableDutMode(CONVERT_PARAMS);
CsrBool ConvertHciWriteSimplePairingDebugMode(CONVERT_PARAMS);

#ifdef __cplusplus
}
#endif

#endif
