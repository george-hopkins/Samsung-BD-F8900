#ifndef CSR_BT_SD_PRIVATE_PRIM_H__
#define CSR_BT_SD_PRIVATE_PRIM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_sd_prim.h"
#include "csr_bt_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="CsrBtSdPrim" */
/* conversion_rule="UPPERCASE_START_AND_REMOVE_UNDERSCORES" */

/* Downstream */
#define CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ           ((CsrBtSdPrim) (0x0000 + CSR_BT_SD_PRIM_DOWNSTREAM_COUNT))
#define CSR_BT_SD_UPDATE_DEVICE_REQ                        ((CsrBtSdPrim) (0x0001 + CSR_BT_SD_PRIM_DOWNSTREAM_COUNT))
#define CSR_BT_SD_ALL_DOWNSTREAM_COUNT               (CSR_BT_SD_UPDATE_DEVICE_REQ + 1)

#define CSR_BT_SD_PRIVATE_PRIM (CSR_BT_SD_PRIM)

/* ---------- End of Primitives ----------*/

typedef struct
{
    CsrBtSdPrim          type;
    CsrBtUuid32          service;
} CsrBtSdRegisterAvailableServiceReq;

#define SD_UPDATE_DEVICE_IGNORE     (0x00000000) /* Excludes other flags */
#define SD_UPDATE_DEVICE_BOND       (0x00000001) /* Excludes SD_UPDATE_DEVICE_DEBOND */
#define SD_UPDATE_DEVICE_DEBOND     (0x00000002) /* Excludes SD_UPDATE_DEVICE_BOND */
#define SD_UPDATE_DEVICE_TRUSTED    (0x00000004) /* Only use together with SD_UPDATE_DEVICE_BOND */
#define SD_UPDATE_SERVICE_CHANGED   (0x00000008) /* Only use by GATT */   


typedef struct
{
    CsrBtSdPrim          type;
    CsrBtDeviceAddr      addr;
    CsrBtClassOfDevice   cod;
    CsrUint32            details;
    CsrBtAddressType     addressType;
    CsrUint32            serviceChangeHandle;
} CsrBtSdUpdateDeviceReq;

void CsrBtSdPrivateFreeUpstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_SD_PRIVATE_PRIM_H__ */

