#ifndef    HCI_CONVERT_LP_DOWN_STREAM_H_
#define    HCI_CONVERT_LP_DOWN_STREAM_H_

#include "csr_synergy.h"
/****************************************************************************

                  (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_lpdownstream.h

DESCRIPTION:            converter for _hci _link _policy commands and events

REVISION:        $Revision: #1 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertHciSniffMode(CONVERT_PARAMS);
CsrBool ConvertHciExitSniffMode(CONVERT_PARAMS);
CsrBool ConvertHciParkMode(CONVERT_PARAMS);
CsrBool ConvertHciExitParkMode(CONVERT_PARAMS);
CsrBool ConvertHciHoldMode(CONVERT_PARAMS);
CsrBool ConvertHciQosSetup(CONVERT_PARAMS);
CsrBool ConvertHciSwitchRole(CONVERT_PARAMS);
CsrBool ConvertHciRoleDiscovery(CONVERT_PARAMS);
CsrBool ConvertHciWriteLinkPolicySettings(CONVERT_PARAMS);
CsrBool ConvertHciWriteDefaultLinkPolicySettings(CONVERT_PARAMS);

/* [*] 2.1 features */
CsrBool ConvertHciSniffSubRate(CONVERT_PARAMS);

#ifdef __cplusplus
}
#endif

#endif
