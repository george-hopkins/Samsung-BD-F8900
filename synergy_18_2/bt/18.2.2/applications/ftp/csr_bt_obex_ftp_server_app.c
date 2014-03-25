/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdlib.h>
#include <errno.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_unicode.h"
#include "csr_exceptionhandler.h"

#include "csr_ui_lib.h"
#include "csr_app_lib.h"

#include "csr_bt_platform.h"
#include "csr_bt_ui_strings.h"
#include "csr_bt_file.h"
#include "csr_bt_util.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"

#include "csr_bt_obex_ftp_strings.h"
#include "csr_bt_obex_ftp_common.h"
#include "csr_bt_obex_ftp_server_app.h"
#include "csr_bt_obex_ftp_server_ui_sef.h"
#include "csr_bt_obex_ftp_server_sef.h"

void ftsAppInitUI(CsrBtObexFtpServerAppUiInstData *uiVar)
{
    uiVar->commonDialogUi           = FALSE;
    uiVar->allowMenuUi              = FALSE;
    uiVar->obexPasskeyUi            = OBEX_PASSKEY_RESPONSE;

    uiVar->hMainMenu                = CSR_UI_DEFAULTACTION;
    uiVar->hBackEvent               = CSR_UI_DEFAULTACTION;
    uiVar->hSk1Event                = CSR_UI_DEFAULTACTION;
    uiVar->hSk2Event                = CSR_UI_DEFAULTACTION;
    uiVar->hPasskeySk1Event         = CSR_UI_DEFAULTACTION;
    uiVar->hObexPasskeyInputDialog  = CSR_UI_DEFAULTACTION;
    uiVar->hAllowMenu               = CSR_UI_DEFAULTACTION;
    uiVar->hCommonDialog            = CSR_UI_DEFAULTACTION;
}

/* Initialise demo application */
void CsrBtFtsAppInit(void **gash)
{
    CsrBtObexFtpServerAppGlobalInstData *ftpServerInst;
    char buffer[CSR_BT_MAX_PATH_LENGTH];

    *gash = CsrPmemAlloc(sizeof(CsrBtObexFtpServerAppGlobalInstData));
    CsrMemSet(*gash, 0, sizeof(CsrBtObexFtpServerAppGlobalInstData));
    ftpServerInst = *gash;

    ftpServerInst->ftsPasswordLength        = 0;
    ftpServerInst->lastfileSaved            = NULL;
    ftpServerInst->bytesReceived            = 0;
    ftpServerInst->totalBytes               = 0;
    ftpServerInst->file                     = NULL;
    ftpServerInst->folderListingString      = NULL;
    ftpServerInst->ftsPutObjIndMsg          = NULL;
    ftpServerInst->ftsSetAddFolderIndMsg    = NULL;
    ftpServerInst->ftsGetObjIndMsg          = NULL;
    ftpServerInst->srmSupported             = FALSE;
    ftpServerInst->transferObject           = FALSE;
    /* Application handle */
    ftpServerInst->queueId                  = CsrSchedTaskQueueGet();

    ftsAppInitUI(&(ftpServerInst->csrUiVar));

    /* Start creation cycle */
    CsrUiUieCreateReqSend(ftpServerInst->queueId, CSR_UI_UIETYPE_EVENT);
    CsrAppRegisterReqSend(ftpServerInst->queueId, TECH_BT, PROFILE_FTP_SERVER);

    /* Get the current working directory: */
    if((CsrBtGetCwd(buffer, CSR_BT_MAX_PATH_LENGTH)) == NULL)
    {
        perror("CsrBtGetCwd error");
    }
    sprintf(ftpServerInst->startupPath, "%s%s", buffer,PATH_SEP);

}

/* Shutdown demo application */
#ifdef ENABLE_SHUTDOWN
void CsrBtFtsAppDeinit(void **gash)
{
    CsrUint16    msgType=0;
    void        *msgData=NULL;

    CsrBtObexFtpServerAppGlobalInstData *ftpServerInst = *gash;

    /* Continue popping messages off the queue */
    while(CsrSchedMessageGet(&msgType, &msgData))
    {
        switch (msgType)
        {
        case CSR_BT_FTS_PRIM:
            CsrBtFtsFreeUpstreamMessageContents(msgType, msgData);
            break;
        case CSR_BT_CM_PRIM:
            CsrBtCmFreeUpstreamMessageContents(msgType, msgData);
            break;
#ifdef CSR_AMP_ENABLE
        case CSR_BT_AMPM_PRIM:
            CsrBtAmpmFreeUpstreamMessageContents(msgType, msgData);
            break;
#endif
        }
        CsrPmemFree(msgData);
    }
    if(ftpServerInst->file != NULL)
    {
        CsrBtFclose(ftpServerInst->file);
    }
    CsrPmemFree(ftpServerInst->folderListingString);
    if(ftpServerInst->ftsGetObjIndMsg)
    {
        CsrPmemFree(ftpServerInst->ftsGetObjIndMsg->payload);
        CsrPmemFree(ftpServerInst->ftsGetObjIndMsg);
    }
    if(ftpServerInst->ftsPutObjIndMsg)
    {
        CsrPmemFree(ftpServerInst->ftsPutObjIndMsg->payload);
        CsrPmemFree(ftpServerInst->ftsPutObjIndMsg);
    }
    if(ftpServerInst->ftsSetAddFolderIndMsg)
    {
        CsrPmemFree(ftpServerInst->ftsSetAddFolderIndMsg->payload);
        CsrPmemFree(ftpServerInst->ftsSetAddFolderIndMsg);
    }
    CsrPmemFree(ftpServerInst);
}
#else
void CsrBtFtsAppDeinit(void **gash)
{
    CSR_UNUSED(gash);
}
#endif

/* The FTP Server demo application handler */
void CsrBtFtsAppHandler(void **gash)
{
    CsrBtObexFtpServerAppGlobalInstData *ftpServerInst = *gash;

    CsrSchedMessageGet(&ftpServerInst->event, &ftpServerInst->message);

    switch(ftpServerInst->event)
    {
        case CSR_BT_FTS_PRIM:
            handleFtsEvent(ftpServerInst);
            break;

        case CSR_BT_CM_PRIM:
            handleCmPrim(ftpServerInst);
            break;

#ifdef CSR_AMP_ENABLE
        case CSR_BT_AMPM_PRIM:
            handleAmpmPrim(ftpServerInst);
            break;
#endif

        case CSR_APP_PRIM:
        {
            CsrPrim *prim = (CsrPrim *) ftpServerInst->message;

            if (*prim == CSR_APP_TAKE_CONTROL_IND)
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Received Take Control Ind");
                CsrUiUieShowReqSend(ftpServerInst->csrUiVar.hMainMenu, ftpServerInst->queueId, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
            }
            else if (*prim == CSR_APP_REGISTER_CFM)
            {
                /* Just ignore this primitive since we only register one menu item we don't need to remember the controlId */
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Obex Ftp Server demo registered");
            }
            else
            {
                /* unexpected primitive received */
                CsrGeneralException("CSR_BT_FTP_SERVER_APP", 0, *prim, "####### Unhandled CSR_APP_PRIM in CsrBtFtsAppHandler");
            }
            break;
        }
        case CSR_UI_PRIM:
        {
            CsrBtFtpServerAppHandleCsrUiPrim(ftpServerInst);
            break;
        }

        default:
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unhandled event class 0x%04x", ftpServerInst->event);
            break;
    }
    CsrPmemFree(ftpServerInst->message);
}

