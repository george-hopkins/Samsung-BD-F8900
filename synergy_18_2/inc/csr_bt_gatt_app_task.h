#ifndef CSR_BT_GATT_APP_TASK_H__
#define CSR_BT_GATT_APP_TASK_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_app_prim.h"
#include "csr_bt_gatt_prim.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_PROXS_APP_IFACEQUEUE_PRIM CSR_BT_GATT_APP_PRIM
#define CSR_BT_PROXC_APP_IFACEQUEUE_PRIM CSR_BT_GATT_APP_PRIM
#define CSR_BT_PROXC_APP_EXTRA_IFACEQUEUE_PRIM CSR_BT_GATT_APP_PRIM


#define CSR_BT_GENERIC_SERVER_APP_IFACEQUEUE_PRIM CSR_BT_GATT_APP_PRIM
#define CSR_BT_GENERIC_SERVER_APP_EXTRA_IFACEQUEUE_PRIM CSR_BT_GATT_APP_PRIM
#define CSR_BT_LE_BROWSER_APP_IFACEQUEUE_PRIM CSR_BT_GATT_APP_PRIM
#define CSR_BT_LE_BROWSER_APP_EXTRA_IFACEQUEUE_PRIM CSR_BT_GATT_APP_PRIM
#define CSR_BT_THERMC_APP_IFACEQUEUE_PRIM CSR_BT_GATT_APP_PRIM
#define CSR_BT_THERMC_APP_EXTRA_IFACEQUEUE_PRIM CSR_BT_GATT_APP_PRIM
#define CSR_BT_THERMS_APP_IFACEQUEUE_PRIM CSR_BT_GATT_APP_PRIM
#define CSR_BT_HIDC_APP_IFACEQUEUE_PRIM CSR_BT_GATT_APP_PRIM

/* Queue definition for CSR_BT_PROXC_APP */
extern CsrUint16 CSR_BT_PROXC_APP_IFACEQUEUE;
extern CsrUint16 CSR_BT_PROXC_APP_EXTRA_IFACEQUEUE;
extern CsrUint16 CSR_BT_PROXS_APP_IFACEQUEUE;
extern CsrUint16 CSR_BT_GENERIC_SERVER_APP_IFACEQUEUE;
extern CsrUint16 CSR_BT_GENERIC_SERVER_APP_EXTRA_IFACEQUEUE;
extern CsrUint16 CSR_BT_LE_BROWSER_APP_IFACEQUEUE;
extern CsrUint16 CSR_BT_LE_BROWSER_APP_EXTRA_IFACEQUEUE;
extern CsrUint16 CSR_BT_THERMC_APP_IFACEQUEUE;
extern CsrUint16 CSR_BT_THERMC_APP_EXTRA_IFACEQUEUE;
extern CsrUint16 CSR_BT_THERMS_APP_IFACEQUEUE;
extern CsrUint16 CSR_BT_HIDC_APP_IFACEQUEUE;

/* Task definition for Proximity Client */
void CsrBtProxcAppInit(void ** gash);
void CsrBtProxcAppDeinit(void ** gash);
void CsrBtProxcAppHandler(void ** gash);
/* Task definition for Proximity Server */
void CsrBtProxsAppInit(void ** gash);
void CsrBtProxsAppDeinit(void ** gash);
void CsrBtProxsAppHandler(void ** gash);
/* Task definition for Generic Server */
void CsrBtGenericServerAppInit(void ** gash);
void CsrBtGenericServerAppDeinit(void ** gash);
void CsrBtGenericServerAppHandler(void ** gash);

/* Task definition for LE Browser */
void CsrBtLeBrowserAppInit(void ** gash);
void CsrBtLeBrowserAppDeinit(void ** gash);
void CsrBtLeBrowserAppHandler(void ** gash);
/* Task definition for Thermometer Client */
void CsrBtThermcAppInit(void ** gash);
void CsrBtThermcAppDeinit(void ** gash);
void CsrBtThermcAppHandler(void ** gash);
/* Task definition for Thermometer server */
void CsrBtThermsAppInit(void ** gash);
void CsrBtThermsAppDeinit(void ** gash);
void CsrBtThermsAppHandler(void ** gash);

/* Task definition for Hid Client (HID host) */
void CsrBtHidcAppInit(void ** gash);
void CsrBtHidcAppDeinit(void ** gash);
void CsrBtHidcAppHandler(void ** gash);



#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_GATT_APP_TASK_H__ */
