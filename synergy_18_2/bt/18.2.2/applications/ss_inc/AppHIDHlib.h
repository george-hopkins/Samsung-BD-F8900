#ifndef APP_HIDH_LIB_H__
#define APP_HIDH_LIB_H__
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #5 $
****************************************************************************/

#include "csr_bt_profiles.h"
#include "csr_types.h"
#include "AppHIDHidhData.h"


#ifdef __cplusplus
extern "C" {
#endif


void CsrAppServerHidhConnetInd(BD_ADDR_T * pBdaddr);

void CsrAppServerHidhActivateCfm(void);

void CsrAppServerHidhDeactivateCfm(void);

void CsrAppServerHidhConnetCfm(    CsrBtResultCode       resultCode, CsrBtSupplier   resultSupplier , CsrAppBtHidhSdpInfoData * infodata);

void CsrAppServerHidhDisconnectInd(BD_ADDR_T * pBdaddr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier);

void CsrAppServerHidhDisconnectCfm(BD_ADDR_T * pBdaddr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier);

void CsrAppServerHidhDataInd(CsrUint16              dataLen, CsrUint8               *data);

void CsrAppServerHidhStatusInd(CsrBtHidhStatus         status);

#ifdef __cplusplus
}
#endif

#endif

