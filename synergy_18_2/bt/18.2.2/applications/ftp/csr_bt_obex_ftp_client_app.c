/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
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
#include "csr_bt_xml_lib.h"

#include "csr_bt_gap_app_lib.h"
#include "csr_bt_gap_app_prim.h"

#include "csr_bt_obex_ftp_common.h"
#include "csr_bt_obex_ftp_client_strings.h"
#include "csr_bt_obex_ftp_client_app.h"
#include "csr_bt_obex_ftp_client_ui_sef.h"
#include "csr_bt_obex_ftp_client_sef.h"

#define DUMMY_FILE_SIZE     30*1024     /* size in kBytes, e.g. 30*1024 = 30 MB */

/* Clean instance */
void cleanInstance(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrPmemFree(inst->targ_name);
    inst->targ_name = StrInst("");

    CsrPmemFree(inst->dir_remote);
    inst->dir_remote = StrInst("/");

    CsrPmemFree(inst->dir_local);
    inst->dir_local = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH);
    CsrBtGetCwd(inst->dir_local, CSR_BT_MAX_PATH_LENGTH - 1);
    if (inst->dir_local[CsrStrLen(inst->dir_local) - 1] != '/')
    {
        CsrStrCat(inst->dir_local, PATH_SEP);
    }

    if(inst->file)
    {
        CsrBtFclose(inst->file);
    }
    inst->file = NULL;
    inst->file_idx = 0;
    inst->file_size = 0;
    inst->file_time = CsrTimeGet(NULL);
}

#ifdef CSR_AMP_ENABLE
#ifdef FAKE_FILE
static void createDummyFile(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    inst->targ_name = StrInst("dummy.dat");
    inst->file = CsrBtFopen(inst->targ_name, "wb");
    if (inst->file)
    {
        CsrUint32 i;
        CsrUint8 *pBuf = CsrPmemAlloc(DUMMY_FILE_SIZE);
        
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Creating dummy file: %s...", inst->targ_name);
        
        /* dump dummy data in the file */
        for(i=0;i<1024;i++)
        {
            CsrBtFwrite(pBuf, 1, DUMMY_FILE_SIZE, inst->file);
        }
        CsrPmemFree(pBuf);
        
        CsrBtFclose(inst->file);
    }
    else
    {
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Could not create dummy file: %s...", inst->targ_name);
        exit(1);
    }

    inst->file = NULL;
}
#endif
#endif

void ftcAppInitUi(CsrBtObexFtpClientAppUiInstData *uiVar)
{
    uiVar->hMainMenu                = CSR_UI_DEFAULTACTION;
    uiVar->hSelectedOptionsMenu     = CSR_UI_DEFAULTACTION;
    uiVar->hFileBrowser             = CSR_UI_DEFAULTACTION;
    uiVar->hObexPasskeyInputDialog  = CSR_UI_DEFAULTACTION;
    uiVar->hDirectoryInputDialog    = CSR_UI_DEFAULTACTION;
    uiVar->hCopyInputDialog         = CSR_UI_DEFAULTACTION;
    uiVar->hMoveInputDialog         = CSR_UI_DEFAULTACTION;
    uiVar->hSetPermInputDialog      = CSR_UI_DEFAULTACTION;
    uiVar->hAllowMenu               = CSR_UI_DEFAULTACTION;
    uiVar->hCommonDialog            = CSR_UI_DEFAULTACTION;


    uiVar->hBackEvent               = CSR_UI_DEFAULTACTION;
    uiVar->hSk1Event                = CSR_UI_DEFAULTACTION;
    uiVar->hSk1AbortEvent           = CSR_UI_DEFAULTACTION;
    uiVar->hSk1CancelEvent          = CSR_UI_DEFAULTACTION;
    uiVar->hSk2Event                = CSR_UI_DEFAULTACTION;
    uiVar->hSk2AmpEvent             = CSR_UI_DEFAULTACTION;
    uiVar->hSk2BrEdrEvent           = CSR_UI_DEFAULTACTION;

    uiVar->mainMenuUi               = FALSE;
    uiVar->selectedOptionsMenuUi    = FALSE;;
    uiVar->fileBrowserUi            = FALSE;
    uiVar->commonDialogUi           = FALSE;
    uiVar->obexPasskeyUi            = OBEX_PASSKEY_RESPONSE;
    uiVar->allowMenuUi              = FALSE;
    uiVar->directoryInputDialogUi   = FALSE;
}

/* Initialize demo app */
void CsrBtFtcAppInit(void **gash)
{
    CsrBtObexFtpClientAppGlobalInstData *inst;

    inst = CsrPmemAlloc(sizeof(CsrBtObexFtpClientAppGlobalInstData));
    CsrMemSet(inst, 0, sizeof(CsrBtObexFtpClientAppGlobalInstData));
    *gash = inst;

    /* Initialize instance */
    inst->address.nap = 0;
    inst->address.uap = 0;
    inst->address.lap = 0;
    inst->srmSupported  = FALSE;
    inst->channelInUse = CSR_BT_AMP_CONTROLLER_BREDR;
    inst->auth_local = FALSE;
    ftcAppInitUi(&(inst->csrUiVar));
    cleanInstance(inst);

    /* get scheduler task id */
    inst->ftcSchedQid   = CsrSchedTaskQueueGet();

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE
    /* Set CM event mask */
    CsrBtCmSetEventMaskReqSend(inst->ftcSchedQid ,
                               CSR_BT_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE,
                               CSR_BT_CM_EVENT_MASK_COND_SUCCESS);
#endif

    /* Start creation cycle */
    CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_EVENT);
    CsrAppRegisterReqSend(inst->ftcSchedQid, TECH_BT, PROFILE_FTP_CLIENT);

#ifdef CSR_AMP_ENABLE
#ifdef FAKE_FILE
    createDummyFile(inst);
#endif
#endif /* amp-enable */
}


/* Demo application handler */
void CsrBtFtcAppHandler(void **gash)
{
    CsrBtObexFtpClientAppGlobalInstData *inst = *gash;

    if(CsrSchedMessageGet(&(inst->event), &(inst->msg)))
    {
        switch (inst->event)
        {
            case CSR_BT_FTC_PRIM:
            {
                handleFtcEvents(inst);
                break;
            }

            case CSR_BT_CM_PRIM:
                handleFtcCmPrim(inst);
                break;

#ifdef CSR_AMP_ENABLE
            case CSR_BT_AMPM_PRIM:
                handleFtcAmpmPrim(inst);
                break;
#endif
            case CSR_APP_PRIM:
            {
                CsrPrim *prim = (CsrPrim *) inst->msg;
                if (*prim == CSR_APP_TAKE_CONTROL_IND)
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Received Take Control Ind");
                    CsrBtGapAppGetSelectedDeviceReqSend(inst->ftcSchedQid);
                }
                else if (*prim == CSR_APP_REGISTER_CFM)
                {
                    /* Just ignore this primitive since we only register one menu item we don't need to remember the controlId */
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Obex Ftp Client demo registered");
                }
                else
                {
                    /* unexpected primitive received */
                    CsrGeneralException("CSR_BT_FTP_CLIENT_APP", 0, *prim, "####### Unhandled CSR_APP_PRIM in CsrBtFtcAppHandler");
                }
                break;
            }
            case CSR_BT_GAP_APP_PRIM:
            {
                CsrPrim *prim = (CsrPrim *) inst->msg;

                if (*prim == CSR_BT_GAP_APP_GET_SELECTED_DEVICE_CFM)
                {
                    CsrBtGapAppGetSelectedDeviceCfm *msg = (CsrBtGapAppGetSelectedDeviceCfm *) prim;
                    inst->address.lap = msg->deviceAddr.lap;
                    inst->address.nap = msg->deviceAddr.nap;
                    inst->address.uap = msg->deviceAddr.uap;
                    updateMainFtcMenu(inst);
                    CsrUiUieShowReqSend(inst->csrUiVar.hMainMenu, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
                }
                else
                {
                    /* unexpected primitive received */
                    CsrGeneralException("CSR_BT_FTP_CLIENT_APP", 0, *prim, "####### Unhandled CSR_BT_GAP_APP_PRIM in CsrBtFtcAppHandler");
                }
                break;
            }

            case CSR_UI_PRIM:
            {
                handleFtcCsrUiPrim(inst);
                break;
            }

            default:
            {
                CsrPrim *prim = (CsrPrim *) inst->msg;
                CsrGeneralException("CSR_BT_FTP_CLIENT_APP", 0, *prim, "####### Unhandled event in CsrBtFtcAppHandler");
                break;
            }

        }

        if(inst->msg != NULL)
        {
            CsrPmemFree(inst->msg);
            inst->msg = NULL;
        }
    }
}

/* Demo application deinitialise */
#ifdef ENABLE_SHUTDOWN
void CsrBtFtcAppDeinit(void **gash)
{
    CsrUint16 msgType=0;
    void *msgData=NULL;
    CsrBtObexFtpClientAppGlobalInstData *inst;

    inst = *gash;

    /* Get a message from the demoapplication message queue. The message is returned in prim
     * and the event type in eventType */
    while(CsrSchedMessageGet(&msgType, &msgData))
    {
        switch (msgType)
        {
            case CSR_BT_FTC_PRIM:
            {
                CsrBtFtcFreeUpstreamMessageContents(msgType, msgData);
                break;
            }
            case CSR_BT_CM_PRIM:
            {
                CsrBtCmFreeUpstreamMessageContents(msgType, msgData);
                break;
            }
#ifdef CSR_AMP_ENABLE
            case CSR_BT_AMPM_PRIM:
            {
                CsrBtAmpmFreeUpstreamMessageContents(msgType, msgData);
                break;
            }
#endif
        }
        CsrPmemFree(msgData);
    }

    /* Free browser info */
    browserEmpty(inst);

    /* Free instance */
    if(inst->file)
    {
        CsrBtFclose(inst->file);
    }
    CsrPmemFree(inst->folder);
    CsrPmemFree(inst->dir_newrem);
    CsrPmemFree(inst->mkdir_name);
    CsrPmemFree(inst->targ_name);
    CsrPmemFree(inst->dir_local);
    CsrPmemFree(inst->dir_remote);
    CsrPmemFree(inst);
}
#else
void CsrBtFtcAppDeinit(void **gash)
{
    CSR_UNUSED(gash);
}
#endif
