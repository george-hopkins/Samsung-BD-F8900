#ifndef CSR_BT_PHDC_COMMON_H__
#define CSR_BT_PHDC_COMMON_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"

#include "csr_bt_phdc_ieee_asn.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PHDC_INVALID_EVENT 0x00FF

void CsrBtPhdc20601AbrtEncodeSend(CsrSchedQid phandle, CsrUint32  dataChannelId,Abort_reason reason);
void CsrBtPhdc20601RlrqEncodeSend(CsrSchedQid phandle, CsrUint32  dataChannelId,Release_request_reason reason);
void CsrBtPhdc20601RlreEncodeSend(CsrSchedQid phandle, CsrUint32  dataChannelId,Release_response_reason reason);
void CsrBtPhdc20601RoerEncodeSend(CsrSchedQid phandle, CsrUint32  dataChannelId,CsrUint16 invokeId,CsrUint16 error);
void CsrBtPhdc20601RorjEncodeSend(CsrSchedQid phandle, CsrUint32  dataChannelId,CsrUint16 invokeId,CsrUint16 rejectResult);
void CsrBtPhdc20601AareEncodeSend(CsrSchedQid phandle, CsrUint32 dataChannelId, DataProto *pDataProto, Associate_result result);

void CsrBtPhdc20601GetMdsSend(CsrSchedQid phandle, CsrUint32 dataChannelId, CsrUint16 invokeId);
void CsrBtPhdc20601ConfigRspSend(CsrSchedQid phandle, CsrUint32 dataChannelId, CsrUint16 invokeId,CsrUint16 devConfigId,ConfigResult config_result);
void CsrBtPhdc20601DataConfirmedEventSend(CsrSchedQid phandle, CsrUint32  dataChannelId,CsrUint16  invokeId,OID_Type   eventType);

void CsrBtPhdcFreeDecodePrstObsScanFixed(APDU *pApdu);
void CsrBtPhdcFreeDecodePrstObsScanVar(APDU *pApdu);
void CsrBtPhdcFreeDecodePrstNotifyConfig(APDU *pApdu);
void CsrBtPhdcFreeDecodePrstGetMdsRsp(APDU *pApdu);
void CsrBtPhdcFreeDecodePrst(APDU *pApdu);
void CsrBtPhdcFreeDataProtoContents(DataProto  *pData);
void CsrBtPhdcFreeDecodeAarq(APDU *pApdu);
void CsrBtPhdcFreeDecodeAnyApdu(APDU *pApdu);

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /*  CSR_BT_PHDC_COMMON_H*/

