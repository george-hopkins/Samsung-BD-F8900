#ifndef CSR_BT_PROX_SRV_DB_H__
#define CSR_BT_PROX_SRV_DB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_profiles.h"
#include "csr_bt_gatt_prim.h"
#include "csr_bt_gatt_utils.h"
#ifdef __cplusplus
extern "C" {
#endif


CsrBtGattDb *CsrBtProxSrvDbCreate(CsrUint16 handleOffset);

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_PROX_SRV_DB_H__ */

