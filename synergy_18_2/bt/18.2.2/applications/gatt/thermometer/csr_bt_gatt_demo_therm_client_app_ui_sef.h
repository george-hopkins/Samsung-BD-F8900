#ifndef CSR_BT_THERMC_APP_UI_SEF_H__
#define CSR_BT_THERMC_APP_UI_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_bt_gatt_demo_therm_client_app.h"
#ifdef __cplusplus
extern "C" {
#endif


void CsrBtGattAppHandleThermcCsrUiRegisterUnregister(CsrBtThermcAppInstData *inst, CsrBool registered);
void CsrBtGattAppHandleThermcCsrUiConnectDisconnect(CsrBtThermcAppInstData *inst, CsrBool connected, CsrBool success);
void CsrBtGattAppHandleThermcSetMainMenuHeader(CsrBtThermcAppInstData *inst);
void CsrBtGattAppHandleThermcCsrUiNoSelectedDevAddr(CsrBtThermcAppInstData *inst);
void CsrBtGattAppHandleThermcCsrUiReadValue(CsrBtThermcAppInstData *inst, char *str, CsrBool success);
void CsrBtGattAppHandleThermcCsrUiWriteValue(CsrBtThermcAppInstData *inst,  CsrBool success);
void CsrBtGattAppHandleThermcCsrUiMenuUpdate(CsrBtThermcAppInstData *inst);



void CsrBtGattAppHandleThermcCsrUiPrim(CsrBtThermcAppInstData *inst);

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_THERMC_APP_UI_SEF_H__ */

