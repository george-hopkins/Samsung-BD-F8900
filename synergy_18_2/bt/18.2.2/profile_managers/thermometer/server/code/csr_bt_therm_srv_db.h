#ifndef CSR_BT_THERM_SRV_DB_H__
#define CSR_BT_THERM_SRV_DB_H__

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

   REVISION:      $Revision: #1 $
 ****************************************************************************/

#include "csr_bt_profiles.h"
#include "csr_bt_gatt_prim.h"
#include "csr_bt_gatt_utils.h"
#include "csr_bt_gatt_services_defines.h"
#ifdef __cplusplus
extern "C" {
#endif

CsrBtGattDb *CsrBtThermSrvDbCreate(CsrUint16 handleOffset);

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_THERM_SRV_DB_H__ */

