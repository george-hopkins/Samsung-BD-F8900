#ifndef CSR_BT_OBEX_FTP_CLIENT_SEF_H__
#define CSR_BT_OBEX_FTP_CLIENT_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_obex_ftp_client_app.h"

#ifdef __cplusplus
extern "C" {
#endif

char *StrInst(char *s);

void handleFtcEvents(CsrBtObexFtpClientAppGlobalInstData *inst);
void handleFtcCmPrim(CsrBtObexFtpClientAppGlobalInstData *inst);
#ifdef CSR_AMP_ENABLE
void handleFtcAmpmPrim(CsrBtObexFtpClientAppGlobalInstData *inst);
#endif

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_OBEX_FTP_CLIENT_SEF_H__ */

