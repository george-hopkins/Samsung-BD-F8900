#ifndef CSR_BT_JSR82_RFCOMM_H__
#define CSR_BT_JSR82_RFCOMM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#define JSR82_RFC_UUID 0x0000

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtJsr82StartRfcSendData(Jsr82MainInstanceData *instData);
void CsrBtJsr82ContinueRfcSendDataHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82RfcHandleDataInd(Jsr82MainInstanceData *instData);
void CsrBtJsr82RfcReceiveData(Jsr82MainInstanceData *instData);
void CsrBtJsr82RfcHandleDataCfm(Jsr82MainInstanceData *instData);
void CsrBtJsr82RfcGetAvailable(Jsr82MainInstanceData *instData);
void CsrBtJsr82RfcGetServerChannelReqHandler(Jsr82MainInstanceData *instData);
CsrBool CsrBtJsr82CmRegisterCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);
void CsrBtJsr82RfcAcceptAndOpenReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmConnectAcceptCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82RfcConnectReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmConnectCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmControlIndHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmPortNegIndHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82RfcCloseReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82RfcDisconnectReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmDisconnectIndHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmCancelAcceptConnectCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82RfcCleanupConnection(Jsr82MainInstanceData *instData);
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
void CsrBtJsr82CmModeChangeIndHandler(Jsr82MainInstanceData *instData);
#else
#define CsrBtJsr82CmModeChangeIndHandler NULL
#endif
rfcConnectionStruct *getRfcConnection(CsrUint8 serverChannel, rfcConnectionPoolStruct *pool);
rfcConnectionStruct *getRfcConnectionFromConnId(CsrUint32 conn_id, rfcConnectionPoolStruct *pool);
#ifdef __cplusplus
}
#endif

#endif
