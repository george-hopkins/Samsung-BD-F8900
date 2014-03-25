#ifndef HCI_CONVERT_ULPUPSTREAM_H_
#define HCI_CONVERT_ULPUPSTREAM_H_

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2010
                All rights reserved

FILE:           csr_bt_hci_convert_ulpupstream.h

DESCRIPTION:    Converter for ULP/BLE HCI events

REVISION:       $Revision: #1 $

****************************************************************************/

#ifdef CSR_BT_LE_ENABLE

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Specific events */
CsrBool ConvertHciEvUlpConnectionComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvUlpAdvertisingReport(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvUlpConnectionUpdateComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvUlpReadRemoteUsedFeaturesComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvUlpLongTermKeyRequested(CsrUint8 *hciData, HCI_UPRIM_T *prim);

/* Command complete return decoders */
CsrBool ConvertHciUlpReadBufferSizeRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciUlpReadLocalSupportedFeaturesRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciUlpReadAdvertisingChannelTxPowerRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciUlpReadWhiteListSizeRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciUlpReadChannelMapRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciUlpReadRemoteUsedFeaturesRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciUlpEncryptRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciUlpRandRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciUlpLongTermKeyRequestedReplyRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciUlpLongTermKeyRequestedNegativeReplyRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciUlpReadSupportedStatesRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);
CsrBool ConvertHciUlpTestEndRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_LE_ENABLE */

#endif
