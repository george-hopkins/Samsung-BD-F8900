#ifndef CSR_BT_OBEX_FTP_CLIENT_UI_SEF_H__
#define CSR_BT_OBEX_FTP_CLIENT_UI_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_obex_ftp_client_app.h"

#ifdef __cplusplus
extern "C" {
#endif

/* File info container */
struct FtcInfo_t
{
    char              *name;
    CsrUint32          size;
    CsrBool            is_dir;
    struct FtcInfo_t  *next;
};

void selectRemote(CsrBtObexFtpClientAppGlobalInstData *inst);
void browserEmpty(CsrBtObexFtpClientAppGlobalInstData *inst);
void browserAdd(CsrBtObexFtpClientAppGlobalInstData *inst, char *name, CsrUint32 size, CsrBool is_dir);
struct FtcInfo_t *browserPeek(CsrBtObexFtpClientAppGlobalInstData *inst, CsrUint16 i);
void playBrowser(CsrBtObexFtpClientAppGlobalInstData *inst);

void updateMainFtcMenu(CsrBtObexFtpClientAppGlobalInstData *inst);
void setPasswordInputdialog(CsrBtObexFtpClientAppGlobalInstData *inst);
void commonDialogSet(CsrBtObexFtpClientAppGlobalInstData *inst,  CsrUint16 *dialogHeading,  CsrUint16 *dialogText, CsrUint16 *textSK1, CsrUint16 *textSK2, CsrUieHandle eventSK1, CsrUieHandle eventSK2);

void handleFtcCsrUiPrim(CsrBtObexFtpClientAppGlobalInstData  *inst);


#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_OBEX_FTP_CLIENT_UI_SEF_H__ */

