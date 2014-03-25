#ifndef CSR_BT_JSR82_REMOTE_DEVICE_H__
#define CSR_BT_JSR82_REMOTE_DEVICE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtJsr82RdGetFriendlyNameReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82RdAuthenticate(Jsr82MainInstanceData *instData);
void CsrBtJsr82RdForwardScPrim(Jsr82MainInstanceData *instData);
void CsrBtJsr82RdEncrypt(Jsr82MainInstanceData *instData);
void CsrBtJsr82RdIsAuthenticatedReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82RdIsEncryptedReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82RdIsTrustedReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82RemoteNameCfmHandler(Jsr82MainInstanceData *instData);
void jsr82GetConnection(Jsr82MainInstanceData *instData);
void CsrBtJsr82RdIsConnected(Jsr82MainInstanceData *instData);
void CsrBtJsr82AddConnection(Jsr82MainInstanceData *instData, CsrBtDeviceAddr deviceAddr);
void CsrBtJsr82RemoveConnection(Jsr82MainInstanceData *instData, CsrBtDeviceAddr deviceAddr);
void CsrBtJsr82SdReadDeviceInfoCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmReadEncryptionStatusCfmHandler(Jsr82MainInstanceData *instData);

#ifdef __cplusplus
}
#endif

#endif
