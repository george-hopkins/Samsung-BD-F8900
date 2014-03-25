#ifndef    HCI_CONVERT_EVENT_UP_STREAM_H_
#define    HCI_CONVERT_EVENT_UP_STREAM_H_

#include "csr_synergy.h"
/****************************************************************************

       Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
      
                    All rights reserved.

FILE:            csr_bt_hci_convert_event_upstream.h

DESCRIPTION:    Converter for HCI Event

REVISION:        $Revision: #3 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertHciEvSyncConnComplete(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvSyncConnChangedComplete(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvInquiryComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvInquiryResult(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvConnComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvConnRequest(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvDisconnectComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvAuthComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvRemoteNameReqComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvEncryptionChange(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvReadRemSuppFeaturesComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvReadRemoteVerInfoComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvQosSetupComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvCommandComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvCommandStatus(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvHardwareError(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvRoleChange(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvNumberCompletedPkts(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvModeChange(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvPinCodeReq(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvLinkKeyReq(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvLinkKeyNotification(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvDataBufferOverflow(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvMaxSlotsChange(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvReadClockOffsetComplete(CsrUint8 *hciData, HCI_UPRIM_T *prim);
#ifndef CSR_BT_EXCLUDE_HCI_CHANGE_ACL_CONN_PKT_TYPE
CsrBool ConvertHciEvConnPacketTypeChanged(CsrUint8 *hciData, HCI_UPRIM_T *prim);
#endif
CsrBool ConvertHciEvQosViolation(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvPageScanModeChange(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvPageScanRepModeChange(CsrUint8 *hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvInquiryResultWithRssi(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvReadRemExtFeaturesComplete(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvSniffSubRate(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvEncryptionKeyRefreshComplete(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvExtendedInquiryResult(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvIoCapabilityRequest(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvIoCapabilityResponse(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvUserConfirmationRequest(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvUserPasskeyRequest(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvRemoteOobDataRequest(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvSimplePairingComplete(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvLstChange(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvUserPasskeyNotification(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvKeypressNotification(CsrUint8 * hciData, HCI_UPRIM_T *prim);
CsrBool ConvertHciEvRemHostSupportedFeatures(CsrUint8 * hciData, HCI_UPRIM_T *prim);

#ifdef __cplusplus
}
#endif

#endif
