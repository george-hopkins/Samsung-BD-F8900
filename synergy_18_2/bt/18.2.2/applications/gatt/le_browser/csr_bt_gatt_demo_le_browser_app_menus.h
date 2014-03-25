#ifndef CSR_BT_LE_BROWSERC_APP_MENUS_H__
#define CSR_BT_LE_BROWSERC_APP_MENUS_H__

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

REVISION:      $Revision: #1 $
 ****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_GATT_LE_BROWSER_LVL_0    0x00
#define CSR_BT_GATT_LE_BROWSER_LVL_1    0x01
#define CSR_BT_GATT_LE_BROWSER_LVL_2    0x02


    void CsrBtGattAppHandleLeBrowserCsrUiListDatabaseEntries(CsrBtLeBrowserAppInstData *inst, CsrUint16 menuState);
    void CsrBtGattLeBrowserUpdateDbElement(CsrBtLeBrowserAppInstData *inst, CsrUint16 level);

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_LE_BROWSER_APP_MENUS_H__ */

