#ifndef APP_AV_LIB_H__
#define APP_AV_LIB_H__
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #5 $
****************************************************************************/

#include "csr_bt_profiles.h"
#include "csr_types.h"
#include "csr_bt_sc_db.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void CsrAppBtScPassKeyInd(CsrBtDeviceAddr *      pDeviceAddr, CsrBtDeviceName   *   pDeviceName,  CsrUint8    *   pPaired,CsrUint32               cod);

extern void CsrAppBtScAuthorizeInd(CsrBtDeviceAddr *      pDeviceAddr, CsrBtDeviceName   *   pDeviceName,  CsrUint16    *   pService);

extern void CsrAppBtScSspJustworksInd(CsrBtDeviceAddr *      pDeviceAddr, CsrBtDeviceName   *   pDeviceName,  CsrUint8    *   pPaired,CsrUint32               cod);

extern void CsrAppBtScBondInd(CsrBool * pAddedToScDbList,  CsrBtDeviceAddr * pDeviceAddr,  CsrBtResultCode   resultCode,CsrBtSupplier    resultSupplier,CsrBtScDbDeviceRecord *pTheRecord);

extern void CsrAppBtScDeBondCfm(CsrBtDeviceAddr         deviceAddr,CsrBtResultCode   resultCode,CsrBtSupplier    resultSupplier);

extern void CsrAppBtGapCmLocalBdaddrCfm(CsrBtDeviceAddr * pDeviceAddr);

extern void CsrAppBtGapCmLocalNameCfm(CsrBtDeviceName * pname);

extern void CsrAppBtScEncryptionCfm(CsrBtDeviceAddr         deviceAddr,    CsrBtResultCode         resultCode,CsrBtSupplier           resultSupplier);

extern void CsrAppBtGapSdSearchInd(CsrBtDeviceAddr * pdeviceAddr,CsrBtClassOfDevice deviceClass, CsrBtDeviceName * friendlyName, CsrUint32 vidpid,CsrInt8 rssi);

extern void CsrAppBtGapSdSearchClose(void);

extern void CsrAppBtGapGetDbgLevel(CsrUint32 logLevel);

extern void CsrAppBtGapError(void);

extern void CsrAppBtGapInitInd(void);


#ifdef __cplusplus
}
#endif

#endif

