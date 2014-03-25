#ifndef CSR_BT_AMPWIFI_APP_TASK_H__
#define CSR_BT_AMPWIFI_APP_TASK_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_app_prim.h"

#ifdef __cplusplus
extern "C" {
#endif


#define CSR_BT_AMPWIFI_APP_IFACEQUEUE_PRIM CSR_BT_AMPWIFI_APP_PRIM

/* Queue definition for CSR_BT_AMPWIFI */
extern CsrUint16 CSR_BT_AMPWIFI_APP_IFACEQUEUE;

/* Task definition for CSR_BT_AMPWIFI */
void CsrBtAmpWifiAppInit(void ** gash);
void CsrBtAmpWifiAppDeinit(void ** gash);
void CsrBtAmpWifiAppHandler(void ** gash);
void CsrBtAmpWiFiAppUsage(void);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_AMP_WIFI_APP_TASK_H__ */
