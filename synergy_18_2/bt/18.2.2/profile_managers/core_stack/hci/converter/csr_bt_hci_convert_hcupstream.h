#ifndef HCI_CONVERT_HC_UP_STREAM_H_
#define HCI_CONVERT_HC_UP_STREAM_H_

#include "csr_synergy.h"

/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:           csr_bt_hci_convert_hcupstream.h

DESCRIPTION:    Converter for HCI Host Controller and Baseband commands and events

REVISION:       $Revision: #1 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertWritePageTimeoutRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *prim);
CsrBool ConvertHciDeleteStoredLinkKeyRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *prim);
CsrBool ConvertReadClassOfDeviceRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *prim);
CsrBool ConvertWriteLinkSupervTimeoutRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertReadLocalNameRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *prim);
CsrBool ConvertReadTxPowerLevelRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *prim);
CsrBool ConvertHciReadScanEnableRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *prim);
CsrBool ConvertReadAfhChannelClassMRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *prim);
CsrBool ConvertHciReadNumSupportedIacRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciReadCurrentIacLapRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciReadLocalOobDataRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciSendKeypressNotificationRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);

#ifdef __cplusplus
}
#endif

#endif

