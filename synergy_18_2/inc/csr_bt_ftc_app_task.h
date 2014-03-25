#ifndef CSR_BT_FTC_APP_TASK_H__
#define CSR_BT_FTC_APP_TASK_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
    
#include "csr_types.h"
#include "csr_bt_app_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_FTC_APP_IFACEQUEUE_PRIM CSR_BT_FTC_APP_PRIM

/* Queue definition for CSR_BT_FTP_CLIENT_APP */
extern CsrUint16 CSR_BT_FTC_APP_IFACEQUEUE;

/* Task definition for CSR_APP */
void CsrBtFtcAppInit(void ** gash);
void CsrBtFtcAppDeinit(void ** gash);
void CsrBtFtcAppHandler(void ** gash);

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_FTC_APP_TASK_H__ */
