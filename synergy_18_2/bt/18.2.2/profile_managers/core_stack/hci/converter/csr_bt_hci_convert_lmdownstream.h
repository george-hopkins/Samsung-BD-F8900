#ifndef    HCI_CONVERT_LM_DOWN_STREAM_H_
#define    HCI_CONVERT_LM_DOWN_STREAM_H_

#include "csr_synergy.h"
/****************************************************************************

       Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
      
                    All rights reserved.

FILE:            csr_bt_hci_convert_lmdownstream.h

DESCRIPTION:    Converter for HCI LM commands and events

REVISION:        $Revision: #2 $

****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertHciInquiry(CONVERT_PARAMS);
CsrBool ConvertHciInquiryCancel(CONVERT_PARAMS);
CsrBool ConvertHciCreateConnection(CONVERT_PARAMS);
CsrBool ConvertHciDisconnect(CONVERT_PARAMS);
CsrBool ConvertHciAcceptConnectionReq(CONVERT_PARAMS);
CsrBool ConvertHciRejectConnectionReq(CONVERT_PARAMS);
CsrBool ConvertHciLinkKeyReqReply(CONVERT_PARAMS);
CsrBool ConvertHciLinkKeyReqNegReply(CONVERT_PARAMS);
CsrBool ConvertHciPinCodeReqReply(CONVERT_PARAMS);
CsrBool ConvertHciPinCodeReqNegReply(CONVERT_PARAMS);
#ifndef CSR_BT_EXCLUDE_HCI_CHANGE_ACL_CONN_PKT_TYPE
CsrBool ConvertHciChangeConnPktType(CONVERT_PARAMS);
#endif
CsrBool ConvertHciAuthReq(CONVERT_PARAMS);
CsrBool ConvertHciSetConnEncryption(CONVERT_PARAMS);
CsrBool ConvertHciRemoteNameReq(CONVERT_PARAMS);
CsrBool ConvertHciReadRemoteSuppFeatures(CONVERT_PARAMS);
CsrBool ConvertHciReadRemoteVerInfo(CONVERT_PARAMS);
CsrBool ConvertHciReadClockOffset(CONVERT_PARAMS);

CsrBool ConvertHciCreateConnectionCancel(CONVERT_PARAMS);
CsrBool ConvertHciRemoteNameReqCancel(CONVERT_PARAMS);
CsrBool ConvertHciReadRemoteExtFeatures(CONVERT_PARAMS);
CsrBool ConvertHciSetupSynchronousConn(CONVERT_PARAMS);
CsrBool ConvertHciAcceptSynchronousConnReq(CONVERT_PARAMS);
CsrBool ConvertHciRejectSynchronousConn(CONVERT_PARAMS);
CsrBool ConvertHciIoCapabilityResponse(CONVERT_PARAMS);
CsrBool ConvertHciUserConfirmationRequestReply(CONVERT_PARAMS);
CsrBool ConvertHciUserConfirmationRequestNegReply(CONVERT_PARAMS);
CsrBool ConvertHciUserPasskeyRequestReply(CONVERT_PARAMS);
CsrBool ConvertHciUserPasskeyRequestNegReply(CONVERT_PARAMS);
CsrBool ConvertHciRemoteOobDataRequestReply(CONVERT_PARAMS);
CsrBool ConvertHciRemoteOobDataRequestNegReply(CONVERT_PARAMS);
CsrBool ConvertHciIoCapabilityRequestNegReply(CONVERT_PARAMS);

#ifdef __cplusplus
}
#endif

#endif

