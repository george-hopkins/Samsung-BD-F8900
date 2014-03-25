#ifndef CSR_BT_HIDC_APP_UI_SEF_H__
#define CSR_BT_HIDC_APP_UI_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_gatt_demo_hid_client_app.h"
#ifdef __cplusplus
extern "C" {
#endif


void CsrBtGattAppHandleHidcCsrUiRegisterUnregister(CsrBtHidcAppInstData *inst, CsrBool registered);
void CsrBtGattAppHandleHidcCsrUiConnectDisconnect(CsrBtHidcAppInstData *inst, CsrBool connected, CsrBool success);
void CsrBtGattAppHandleHidcCsrUiWhitelistAdd(CsrBtHidcAppInstData *inst, CsrBool success);
void CsrBtGattAppHandleHidcSetMainMenuHeader(CsrBtHidcAppInstData *inst);
void CsrBtGattAppHandleHidcCsrUiNoSelectedDevAddr(CsrBtHidcAppInstData *inst);
void CsrBtGattAppHandleHidcCsrUiReadValue(CsrBtHidcAppInstData *inst, char *str, CsrBool success);
void CsrBtGattAppHandleHidcCsrUiWriteValue(CsrBtHidcAppInstData *inst,  CsrBool success);
void CsrBtGattAppHandleHidcCsrUiMenuUpdate(CsrBtHidcAppInstData *inst);
void CsrBtGattAppHandleHidcCsrUiShowError(CsrBtHidcAppInstData *inst, char *title, char *error);



void CsrBtGattAppHandleHidcCsrUiPrim(CsrBtHidcAppInstData *inst);

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_HIDC_APP_UI_SEF_H__ */

