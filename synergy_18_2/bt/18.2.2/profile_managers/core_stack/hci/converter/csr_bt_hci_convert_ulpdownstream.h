#ifndef HCI_CONVERT_ULPDOWNSTREAM_H_
#define HCI_CONVERT_ULPDOWNSTREAM_H_

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2010
                All rights reserved

FILE:           csr_bt_hci_convert_ulpdownstream.h

DESCRIPTION:    Converter for ULP/BLE HCI commands

REVISION:       $Revision: #1 $

****************************************************************************/

#ifdef CSR_BT_LE_ENABLE

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertHciUlpSetEventMask(CONVERT_PARAMS);
CsrBool ConvertHciUlpReadBufferSize(CONVERT_PARAMS);
CsrBool ConvertHciUlpReadLocalSupportedFeatures(CONVERT_PARAMS);
CsrBool ConvertHciUlpSetRandomAddress(CONVERT_PARAMS);
CsrBool ConvertHciUlpSetAdvertisingParameters(CONVERT_PARAMS);
CsrBool ConvertHciUlpReadAdvertisingChannelTxPower(CONVERT_PARAMS);
CsrBool ConvertHciUlpSetAdvertisingData(CONVERT_PARAMS);
CsrBool ConvertHciUlpSetScanResponseData(CONVERT_PARAMS);
CsrBool ConvertHciUlpSetAdvertiseEnable(CONVERT_PARAMS);
CsrBool ConvertHciUlpSetScanParameters(CONVERT_PARAMS);
CsrBool ConvertHciUlpSetScanEnable(CONVERT_PARAMS);
CsrBool ConvertHciUlpCreateConnection(CONVERT_PARAMS);
CsrBool ConvertHciUlpCreateConnectionCancel(CONVERT_PARAMS);
CsrBool ConvertHciUlpReadWhiteListSize(CONVERT_PARAMS);
CsrBool ConvertHciUlpClearWhiteList(CONVERT_PARAMS);
CsrBool ConvertHciUlpAddDeviceToWhiteList(CONVERT_PARAMS);
CsrBool ConvertHciUlpRemoveDeviceFromWhiteList(CONVERT_PARAMS);
CsrBool ConvertHciUlpConnectionUpdate(CONVERT_PARAMS);
CsrBool ConvertHciUlpSetHostChannelClassification(CONVERT_PARAMS);
CsrBool ConvertHciUlpReadChannelMap(CONVERT_PARAMS);
CsrBool ConvertHciUlpReadRemoteUsedFeatures(CONVERT_PARAMS);
CsrBool ConvertHciUlpEncrypt(CONVERT_PARAMS);
CsrBool ConvertHciUlpRand(CONVERT_PARAMS);
CsrBool ConvertHciUlpStartEncryption(CONVERT_PARAMS);
CsrBool ConvertHciUlpLongTermKeyRequestedReply(CONVERT_PARAMS);
CsrBool ConvertHciUlpLongTermKeyRequestedNegativeReply(CONVERT_PARAMS);
CsrBool ConvertHciUlpReadSupportedStates(CONVERT_PARAMS);
CsrBool ConvertHciUlpReceiverTest(CONVERT_PARAMS);
CsrBool ConvertHciUlpTransmitterTest(CONVERT_PARAMS);
CsrBool ConvertHciUlpTestEnd(CONVERT_PARAMS);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_LE_ENABLE */

#endif

