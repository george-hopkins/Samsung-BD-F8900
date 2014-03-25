#ifndef CSR_BT_OBEX_FTP_SERVER_UI_SEF_H__
#define CSR_BT_OBEX_FTP_SERVER_UI_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_obex_ftp_server_app.h"


#ifdef __cplusplus
extern "C" {
#endif

void CsrFtpServerCommonDialogSet(CsrBtObexFtpServerAppGlobalInstData *inst,  CsrUint16 *dialogHeading,  CsrUint16 *dialogText, CsrUint16 *textSK1, CsrUint16 *textSK2, CsrUieHandle eventSK1, CsrUieHandle eventSK2);
void CsrBtFtpServerAppHandleCsrUiActivateDeactivate(CsrBtObexFtpServerAppGlobalInstData *inst, CsrBool activated);
void CsrBtFtpServerAppHandleCsrUiPrim(CsrBtObexFtpServerAppGlobalInstData  *inst);

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_OBEX_FTP_SERVER_UI_SEF_H__ */

