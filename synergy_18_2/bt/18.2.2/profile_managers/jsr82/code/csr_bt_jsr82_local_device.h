#ifndef CSR_BT_JSR82_LOCAL_DEVICE_H__
#define CSR_BT_JSR82_LOCAL_DEVICE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void   CsrBtJsr82LdGetBluetoothAddress(Jsr82MainInstanceData *instData);
CsrBool CsrBtJsr82CmGetBdAddrCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);
void   CsrBtJsr82LdGetFriendlyName(Jsr82MainInstanceData *instData);
CsrBool CsrBtJsr82CmReadLocalNameCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);
void   CsrBtJsr82LdGetDiscoverableReqHandler(Jsr82MainInstanceData *instData);
CsrBool CsrBtJsr82CmReadScanEnableCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);
CsrBool CsrBtJsr82CmReadIacCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);
void   CsrBtJsr82LdSetDiscoverableReqHandler(Jsr82MainInstanceData *instData);
CsrBool CsrBtJsr82CmWriteIacIndHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);
void   CsrBtJsr82LdGetProperty(Jsr82MainInstanceData *instData);
void   CsrBtJsr82LdGetDeviceClassReqHandler(Jsr82MainInstanceData *instData);
void   CsrBtJsr82LdSetDeviceClassReqHandler(Jsr82MainInstanceData *instData);
CsrBool CsrBtJsr82CmReadCodCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);
CsrBool CsrBtJsr82CmWriteCodCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);
CsrBool CsrBtJsr82SetCodCmReadCodCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);
void   CsrBtJsr82LdGetSecurityLevelReqHandler(Jsr82MainInstanceData *instData);
void   CsrBtJsr82LdIsMasterReqHandler(Jsr82MainInstanceData *instData);
CsrBool CsrBtJsr82CmRoleDiscoveryCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);

#ifdef __cplusplus
}
#endif

#endif /* not defined JSR82_LOCAL_DEVICE_H_ */

