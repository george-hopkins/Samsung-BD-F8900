#ifndef CSR_BT_NVS_HID_H__
#define CSR_BT_NVS_HID_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_sched.h"
#include "csr_bt_hidh_prim.h"

#ifdef __cplusplus
extern "C" {
#endif


void Nvs_Init(void);
CsrBool Nvs_Flush(void);
CsrBool Nvs_PutHidInfo(CsrBtDeviceAddr *bdAddr,
                      CsrBtHidhSdpInfo *sdpInfo,
                      CsrCharString *serviceName,
                      CsrUint16 descLen,
                      CsrUint8 *desc);
CsrBool Nvs_GetHidInfo(CsrBtDeviceAddr *bdAddr,
                      CsrBtHidhSdpInfo **sdpInfo,
                      CsrCharString **serviceName,
                      CsrUint16 *descLen,
                      CsrUint8 **desc);
CsrBool Nvs_RemoveHidInfo(CsrBtDeviceAddr *bdAddr);
CsrUint8 Nvs_GetHidDeviceList(CsrUint8 maxDevices,
                             CsrBtDeviceAddr *bdAddrList);

#ifdef __cplusplus
}
#endif


#endif /* _NVS_HID_H */
