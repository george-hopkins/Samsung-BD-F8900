#ifndef CSR_BT_OBEX_FTP_SERVER_SEF_H__
#define CSR_BT_OBEX_FTP_SERVER_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_obex_ftp_server_app.h"

#ifdef __cplusplus
extern "C" {
#endif

void ftpServerTransferingObject(CsrBtObexFtpServerAppGlobalInstData *inst, CsrBool finalbitSet);


void handleFtsEvent(CsrBtObexFtpServerAppGlobalInstData *ftpServerInst);
void handleCmPrim(CsrBtObexFtpServerAppGlobalInstData *ftpServerInst);
#ifdef CSR_AMP_ENABLE
void handleAmpmPrim(CsrBtObexFtpServerAppGlobalInstData *ftpServerInst);
#endif

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_OBEX_FTP_SERVER_SEF_H__ */

