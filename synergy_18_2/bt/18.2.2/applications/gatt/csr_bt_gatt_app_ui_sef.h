#ifndef CSR_BT_GATT_APP_UI_SEF_H__
#define CSR_BT_GATT_APP_UI_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_bt_gatt_demo_app.h"
#include "csr_bt_gatt_app_ui_strings.h"

#ifdef __cplusplus
extern "C" {
#endif

/* generel key defines */
#define CSR_BT_GATT_FIRST_DYNAMIC_KEY                                    0x0001
#define CSR_BT_GATT_INVALID_KEY                                          0x7FFF

/* key defines for the HF menu UI */

/* Handler functions for the CSR_UI upstream messages */
void CsrBtGattCsrUiUieCreateCfmHandler(DemoInstdata_t * inst);
void CsrBtGattCsrUiInputdialogGetCfmHandler(DemoInstdata_t* inst);
void CsrBtGattCsrUiDisplayGetHandleCfmHandler(DemoInstdata_t* inst);
void CsrBtGattCsrUiEventIndHandler(DemoInstdata_t * inst);
void updateGattMainMenu(DemoInstdata_t *inst);
void CsrBtGattShowUi(DemoInstdata_t * inst, CsrUint8 UiIndex,
                     CsrUint16 inputMode, CsrUint16 priority);

#ifdef __cplusplus
}
#endif



#endif /* CSR_BT_GATT_APP_UI_SEF_H__ */
