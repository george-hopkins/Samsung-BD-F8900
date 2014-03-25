#ifndef APP_AVRC_LIB_H__
#define APP_AVRC_LIB_H__
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #5 $
****************************************************************************/

#include "csr_bt_profiles.h"
#include "csr_types.h"


#ifdef __cplusplus
extern "C" {
#endif

void CsrAppServerAVRCCloseInd(void);

void CsrAppServerAVRCConnetInd(CsrBtDeviceAddr * pBdaddr);

void CsrAppServerAVRCRemoteFeatureInd(CsrUint16  feature);

void CsrAppServerAVRCActivateCfm(void);

void CsrAppServerAVRCDeActivateCfm(void);

void CsrAppServerAVRCCtPassThroughCfm(CsrBtAvrcpPTOpId operationId,CsrBtResultCode             resultCode,CsrBtSupplier          avResultSupplier);

void CsrAppServerAVRCTgPassThroughInd(CsrBtAvrcpPTOpId operationId, CsrBtAvrcpPTState             state);

void CsrAppServerAVRCConnetCfm( BD_ADDR_T * pBdaddr,    CsrBtResultCode        avResultCode, CsrBtSupplier          avResultSupplier);

void CsrAppServerAVRCDisconectInd(BD_ADDR_T * pBdaddr,CsrUint16 reasonCode,CsrUint16 reasonSupplier);

void CsrAppServerAVRCDisconectCfm(BD_ADDR_T * pBdaddr,CsrUint16 reasonCode,CsrUint16 reasonSupplier);

#ifdef __cplusplus
}
#endif

#endif

