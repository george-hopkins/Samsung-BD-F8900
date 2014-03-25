#ifndef    HCI_CONVERT_HC_DOWN_STREAM_H_
#define    HCI_CONVERT_HC_DOWN_STREAM_H_

#include "csr_synergy.h"
/****************************************************************************

                  (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_hcdownstream.h

DESCRIPTION:            converter for _hci _host _controller and _baseband
                        commands and events

REVISION:        $Revision: #2 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertHciSetEventFilter(CONVERT_PARAMS);
CsrBool ConvertHciReset(CONVERT_PARAMS);
CsrBool ConvertHciWritePageTimeout(CONVERT_PARAMS);
CsrBool ConvertHciDeleteStoredLinkKey(CONVERT_PARAMS);
CsrBool ConvertHciChangeLocalName(CONVERT_PARAMS);
CsrBool ConvertHciReadLocalName(CONVERT_PARAMS);
CsrBool ConvertHciWriteScanEnable(CONVERT_PARAMS);
CsrBool ConvertHciReadScanEnable(CONVERT_PARAMS);
CsrBool ConvertHciWritePagescanActivity(CONVERT_PARAMS);
CsrBool ConvertHciWriteInquiryscanActivity(CONVERT_PARAMS);
CsrBool ConvertHciWriteAuthEnable(CONVERT_PARAMS);
CsrBool ConvertHciWriteEncMode(CONVERT_PARAMS);
CsrBool ConvertHciReadClassOfDevice(CONVERT_PARAMS);
CsrBool ConvertHciWriteClassOfDevice(CONVERT_PARAMS);
CsrBool ConvertHciWriteVoiceSetting(CONVERT_PARAMS);
CsrBool ConvertHciWriteScoFlowConEnable(CONVERT_PARAMS);
CsrBool ConvertHciHostBufferSize(CONVERT_PARAMS);
CsrBool ConvertHciWriteLinkSupervTimeout(CONVERT_PARAMS);
CsrBool ConvertHciReadTxPowerLevel(CONVERT_PARAMS);

CsrBool ConvertHciSetAfhChannelClass(CONVERT_PARAMS);
CsrBool ConvertHciWriteInquiryScanType(CONVERT_PARAMS);
CsrBool ConvertHciWriteInquiryMode(CONVERT_PARAMS);
CsrBool ConvertHciWritePageScanType(CONVERT_PARAMS);
CsrBool ConvertHciReadAfhChannelClassM(CONVERT_PARAMS);
CsrBool ConvertHciWriteAfhChannelClassM(CONVERT_PARAMS);
CsrBool ConvertHciReadNumSupportedIac(CONVERT_PARAMS);
CsrBool ConvertHciReadCurrentIacLap(CONVERT_PARAMS);
CsrBool ConvertHciWriteCurrentIacLap(CONVERT_PARAMS);
/* [*] 2.1 features */
CsrBool ConvertHciWriteExtendedInquiryResponseData(CONVERT_PARAMS);
CsrBool ConvertHciRefreshEncryptionKey(CONVERT_PARAMS);
CsrBool ConvertHciWriteSimplePairingMode(CONVERT_PARAMS);
CsrBool ConvertHciReadLocalOobData(CONVERT_PARAMS);
CsrBool ConvertHciWriteInquiryTransmitPowerLevel(CONVERT_PARAMS);
CsrBool ConvertHciSendKeypressNotification(CONVERT_PARAMS);
CsrBool ConvertHciSetEventMask(CONVERT_PARAMS);

/* 4.0 BLE features */
#ifdef CSR_BT_LE_ENABLE
CsrBool ConvertHciWriteLeHostSupport(CONVERT_PARAMS);
#endif

#ifdef __cplusplus
}
#endif

#endif
