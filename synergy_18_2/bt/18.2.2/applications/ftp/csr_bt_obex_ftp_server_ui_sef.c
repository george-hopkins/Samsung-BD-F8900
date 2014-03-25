/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_unicode.h"
#include "csr_util.h"
#include "csr_formatted_io.h"

#include "csr_app_lib.h"
#include "csr_ui_lib.h"

#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_fts_lib.h"
#include "csr_bt_ui_strings.h"
#include "csr_bt_file.h"

#include "csr_bt_obex_ftp_strings.h"
#include "csr_bt_obex_ftp_common.h"
#include "csr_bt_obex_ftp_server_sef.h"
#include "csr_bt_obex_ftp_server_ui_sef.h"

/* handler functions */
static void handleCsrUiUieCreateCfm(CsrBtObexFtpServerAppGlobalInstData *inst, CsrUiUieCreateCfm *confirm)
{
    CsrBtObexFtpServerAppUiInstData *csrUiVar = &inst->csrUiVar;

    if (confirm->elementType == CSR_UI_UIETYPE_EVENT)
    {
        if (!csrUiVar->hBackEvent)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, inst->queueId);
            csrUiVar->hBackEvent = confirm->handle;
            CsrUiUieCreateReqSend(inst->queueId, CSR_UI_UIETYPE_EVENT);
        }
        else if (!csrUiVar->hSk1Event)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, inst->queueId);
            csrUiVar->hSk1Event = confirm->handle;
            CsrUiUieCreateReqSend(inst->queueId, CSR_UI_UIETYPE_EVENT);
        }
        else if (!csrUiVar->hSk2Event)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_BLOCK, inst->queueId);
            csrUiVar->hSk2Event = confirm->handle;
            CsrUiUieCreateReqSend(inst->queueId, CSR_UI_UIETYPE_EVENT);
        }
        else if (!csrUiVar->hPasskeySk1Event)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, inst->queueId);
            csrUiVar->hPasskeySk1Event = confirm->handle;
            CsrUiUieCreateReqSend(inst->queueId, CSR_UI_UIETYPE_INPUTDIALOG);
        }
    }
    if (confirm->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        if (!csrUiVar->hObexPasskeyInputDialog)
        {
            CsrUiInputdialogSetReqSend(confirm->handle, TEXT_OBEX_AUTH_UCS2, TEXT_OBEX_PASSKEY_UCS2, CSR_UI_ICON_KEY,
                NULL, CSR_BT_PASSKEY_MAX_LEN, CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL,
                csrUiVar->hPasskeySk1Event, CSR_UI_DEFAULTACTION, csrUiVar->hBackEvent, CSR_UI_DEFAULTACTION);
            csrUiVar->hObexPasskeyInputDialog = confirm->handle;
            CsrUiUieCreateReqSend(inst->queueId, CSR_UI_UIETYPE_DIALOG);
        }

    }
    if (confirm->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        if (!csrUiVar->hCommonDialog)
        {
            csrUiVar->hCommonDialog = confirm->handle;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
    if (confirm->elementType == CSR_UI_UIETYPE_MENU)
    {
        if (!csrUiVar->hAllowMenu)
        {
            CsrUiMenuSetReqSend(confirm->handle, TEXT_FTP_SERVER_APP_ALLOW_UCS2, TEXT_SELECT_UCS2, NULL);

            CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_FIRST, CSR_BT_FTP_SERVER_APP_YES_KEY, CSR_UI_ICON_NONE,
                TEXT_YES_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
            CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_LAST, CSR_BT_FTP_SERVER_APP_NO_KEY, CSR_UI_ICON_NONE,
                TEXT_NO_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
            CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_LAST, CSR_BT_FTP_SERVER_APP_AUTHENTICATE_KEY, CSR_UI_ICON_NONE,
                TEXT_FTP_SERVER_APP_AUTHENTICATE_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);

            csrUiVar->hAllowMenu = confirm->handle;

            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
        else if (!csrUiVar->hMainMenu)
        {
            CsrUiMenuSetReqSend(confirm->handle, TEXT_FTP_SERVER_APP_MAIN_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

            CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_FIRST, CSR_BT_FTP_SERVER_APP_ACTIVATE_WITH_AUTHENTICATION_KEY,
                CSR_UI_ICON_NONE, TEXT_FTP_SERVER_APP_ACTIVATE_WITH_AUTHENTICATION_UCS2, NULL,
                csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
            CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_FIRST, CSR_BT_FTP_SERVER_APP_ACTIVATE_KEY, CSR_UI_ICON_NONE,
                TEXT_FTP_SERVER_APP_ACTIVATE_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_LAST, CSR_BT_FTP_SERVER_APP_ENABLE_SRM_KEY, CSR_UI_ICON_NONE,
                TEXT_FTP_SERVER_APP_ENABLE_SRM_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
#endif
            csrUiVar->hMainMenu = confirm->handle;
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "FTP Server UI elements created.");

        }

    }
}

static void handleCsrUiInputDialogGetCfm(CsrBtObexFtpServerAppGlobalInstData *inst, CsrUiInputdialogGetCfm *confirm)
{
    CsrBtObexFtpServerAppUiInstData *csrUiVar = &inst->csrUiVar;

    CsrUint8 *passkey = CsrUtf16String2Utf8(confirm->text);
    inst->ftsPasswordLength = (CsrUint8)CsrStrLen((CsrCharString *) passkey);

    if(inst->csrUiVar.obexPasskeyUi == OBEX_PASSKEY_INITIATE)
    {
        /*CsrUint16 realmLength,
        CsrUint8 *realm,
        CsrUint16 passwordLength,
        CsrUint8 *password,
        CsrCharString *userId);*/
        CsrBtFtsAuthenticateReqSend(8, (CsrUint8*)CsrStrDup("Password"), inst->ftsPasswordLength, (CsrUint8*)CsrStrDup((CsrCharString *)passkey), NULL);
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Obex passkey: %s", passkey);
    }
    else /* OBEX_PASSKEY_RESPONSE */
    {
        if(!inst->ftsPasswordLength)
        {
            /*CsrUint8         * password,
            CsrUint16        passwordLength,
            CsrCharString        * userId*/
            CsrBtFtsAuthenticateResSend(NULL, 0, (CsrCharString *) CsrStrDup("CSR-SYNC"));
        }
        else
        {
            CsrBtFtsAuthenticateResSend((CsrUint8 *) CsrStrDup((CsrCharString *)passkey), inst->ftsPasswordLength, CsrStrDup("CSR-SYNC"));
        }
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Obex passkey reponse");
    }

    /* Hide passkey input dialog */
    inst->csrUiVar.obexPasskeyUi = OBEX_PASSKEY_RESPONSE;
    CsrUiUieHideReqSend(csrUiVar->hObexPasskeyInputDialog);
    CsrPmemFree(passkey);
}

static void handleCsrUiUieEventInd(CsrBtObexFtpServerAppGlobalInstData *inst, CsrUiEventInd *indication)
{
    CsrBtObexFtpServerAppUiInstData *csrUiVar = &inst->csrUiVar;

    if (indication->event == csrUiVar->hBackEvent)
    {
        CsrUiUieHideReqSend(indication->displayElement);
    }
    else if(indication->event == csrUiVar->hPasskeySk1Event)
    {
        CsrUiInputdialogGetReqSend(inst->queueId, csrUiVar->hObexPasskeyInputDialog);
    }
    else if( (indication->displayElement == csrUiVar->hMainMenu) && (indication->event == csrUiVar->hSk1Event) )
    {
        /* Main Menu handling */
        switch(indication->key)
        {
            case CSR_BT_FTP_SERVER_APP_ACTIVATE_KEY:
            {
                inst->obexAuthentication = WITHOUT_AUTHENTICATION;
                inst->modeSelect = 1;
                CsrBtFtsActivateReqSend(inst->queueId,
                                        CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                        0, /* windowSize */
                                        inst->srmSupported);

                CsrBtFtpServerAppHandleCsrUiActivateDeactivate(inst, TRUE);
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE,
                                     "FTP server activated...");
                break;
            }

            case CSR_BT_FTP_SERVER_APP_ACTIVATE_WITH_AUTHENTICATION_KEY:
            {
                inst->obexAuthentication = WITH_AUTHENTICATION;
                inst->modeSelect = 2;
                CsrBtFtsActivateReqSend(inst->queueId,
                                        CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                        0, /* windowSize */
                                        inst->srmSupported);

                CsrBtFtpServerAppHandleCsrUiActivateDeactivate(inst, TRUE);
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE,
                                     "FTP server activated with authentication...");
                break;
            }

            case CSR_BT_FTP_SERVER_APP_DISABLE_SRM_KEY:
            {
                inst->srmSupported = FALSE;
                /* Update main menu so it is no longer possible to deactivate but just to activate */
                CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_BT_FTP_SERVER_APP_ENABLE_SRM_KEY,
                    CSR_BT_FTP_SERVER_APP_ENABLE_SRM_KEY, CSR_UI_ICON_NONE, TEXT_FTP_SERVER_APP_ENABLE_SRM_UCS2, NULL,
                    csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
                CsrUiMenuRemoveitemReqSend(csrUiVar->hMainMenu, CSR_BT_FTP_SERVER_APP_DISABLE_SRM_KEY);
                break;
            }
            case CSR_BT_FTP_SERVER_APP_ENABLE_SRM_KEY:
            {
                inst->srmSupported = TRUE;
                /* Update main menu so it is no longer possible to activate but just to deactivate */
                CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_BT_FTP_SERVER_APP_DISABLE_SRM_KEY,
                    CSR_BT_FTP_SERVER_APP_DISABLE_SRM_KEY, CSR_UI_ICON_NONE, TEXT_FTP_SERVER_APP_DISABLE_SRM_UCS2, NULL,
                    csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
                CsrUiMenuRemoveitemReqSend(csrUiVar->hMainMenu, CSR_BT_FTP_SERVER_APP_ENABLE_SRM_KEY);
                break;
            }
            case CSR_BT_FTP_SERVER_APP_DEACTIVATE_KEY:
            {
                CsrBtFtsDeactivateReqSend();
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "FTP server deactivated...");
                break;
            }

            default:
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unhandled UI key 0x%04x", indication->key);
                break;
            }
        }
    }
    else if( (indication->displayElement == csrUiVar->hAllowMenu) && (indication->event == csrUiVar->hSk1Event) && inst->ftsPutObjIndMsg)
    {
        /* Allow Menu handling */
        CsrBtFtsPutObjInd *putObjInd = inst->ftsPutObjIndMsg;
        FILE *putFile;
        char fileName[MAX_FILENAMELEN];
        CsrUint8 *name;

        name = unicode2String(putObjInd->payload + putObjInd->ucs2nameOffset);

        switch(indication->key)
        {
            case CSR_BT_FTP_SERVER_APP_YES_KEY:
            {
                CsrSnprintf(fileName, MAX_FILENAMELEN, "%s", (CsrCharString *) name);
                putFile = CsrBtFopen(fileName, "wb");
                if(putFile != NULL)
                {
                    CsrBtFwrite((putObjInd->payload + putObjInd->bodyOffset),
                        1,
                        putObjInd->bodyLength,
                        putFile);
                    CsrBtFclose(putFile);
                    if(inst->totalBytes != 0)
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Saved %s, bytes received %d of %d",
                            name, inst->bytesReceived, inst->totalBytes);
                    }
                    else
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Saved %s, bytes received %d", name, inst->bytesReceived);
                    }

                    if(!putObjInd->finalFlag)
                    {
                        CsrPmemFree(inst->lastfileSaved);
                        inst->lastfileSaved = name;
                    }
                    else
                    {
                        CsrPmemFree(inst->lastfileSaved);
                        inst->lastfileSaved = NULL;
                        CsrPmemFree(name);
                    }
                    inst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to save object (%s)!", fileName);
                    inst->obexResult = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
                    CsrPmemFree(name);
                }

                ftpServerTransferingObject(inst, putObjInd->finalFlag);

                if (inst->transferObject)
                {
                    CsrFtpServerCommonDialogSet(inst,
                                                TEXT_FTP_SERVER_APP_MAIN_MENU_UCS2,
                                                TEXT_FTP_SERVER_APP_REJECT_UCS2,
                        TEXT_YES_UCS2, NULL, inst->csrUiVar.hSk1Event, 0);
                }

                CsrBtFtsPutObjResSend(putObjInd->connectionId, inst->obexResult, FALSE);
                break;
            }
            case CSR_BT_FTP_SERVER_APP_NO_KEY:
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Rejecting received object (%s)", name);

                inst->obexResult = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;

                ftpServerTransferingObject(inst, putObjInd->finalFlag);
                CsrBtFtsPutObjResSend(putObjInd->connectionId, inst->obexResult, FALSE);
                CsrPmemFree(name);
                break;
            }
            case CSR_BT_FTP_SERVER_APP_AUTHENTICATE_KEY:
            {
                /* Authenticate the client */
                inst->csrUiVar.obexPasskeyUi = OBEX_PASSKEY_INITIATE;
                CsrUiUieShowReqSend(inst->csrUiVar.hObexPasskeyInputDialog, inst->queueId, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);

                inst->obexAuthentication = WITH_AUTHENTICATION;
                inst->ftsPasswordLength = 0;
                CsrPmemFree(name);
                break;
            }
        }
        if(inst->ftsPutObjIndMsg)
        {
            CsrPmemFree(putObjInd->payload);
            CsrPmemFree(inst->ftsPutObjIndMsg);
            inst->ftsPutObjIndMsg = NULL;
        }
        if(csrUiVar->allowMenuUi)
        {
            CsrUiUieHideReqSend(csrUiVar->hAllowMenu);
            csrUiVar->allowMenuUi = FALSE;
        }
    }
    else if( (indication->displayElement == csrUiVar->hAllowMenu) && (indication->event == csrUiVar->hSk1Event) && inst->ftsGetObjIndMsg)
    {
        /* Allow Menu handling */
        CsrBtFtsGetObjInd *getObjInd = inst->ftsGetObjIndMsg;
        CsrUint8 *name;
        name = unicode2String(getObjInd->payload + getObjInd->ucs2nameOffset);

        switch(indication->key)
        {
            case CSR_BT_FTP_SERVER_APP_YES_KEY:
            {
                CsrBtStat_t statBuffer;
                int result;
                CsrUint8    *body   = NULL;
                CsrBool finalFlag   = TRUE;
                CsrUint16 bytesRead = 0;
                CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));

                result = CsrBtStat((char*)name, &statBuffer);
                inst->currentFileSize = 0;
                if(!result)
                {
                    inst->file = CsrBtFopen((char*)name, "rb");
                    if(inst->file != NULL)
                    {
                        CsrUint16 bytesToRead;
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Getting object (%s)...", name);

                        inst->currentFilePosition = 0;
                        inst->currentFileSize = statBuffer.st_size;
                        if(inst->currentFileSize - inst->currentFilePosition <= inst->obexPeerMaxPacketSize)
                        {
                            bytesToRead = (CsrUint16)(inst->currentFileSize - inst->currentFilePosition);
                        }
                        else
                        {
                            bytesToRead = inst->obexPeerMaxPacketSize;
                        }
                        body = CsrPmemAlloc(bytesToRead);
                        bytesRead = CsrBtFread(body, 1, bytesToRead, inst->file);
                        inst->currentFilePosition += bytesRead;
                        if(inst->currentFilePosition >= inst->currentFileSize)
                        {
                            CsrBtFclose(inst->file);
                            inst->file = NULL;
                            finalFlag = TRUE;
                        }
                        else
                        {
                            finalFlag = FALSE;
                        }

                        inst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                    }
                    else
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to open the file (%s)!", name);
                        inst->obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                    }
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to get information about the file (%s)!", name);
                    inst->obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                }

                ftpServerTransferingObject(inst, finalFlag);

                if (inst->transferObject)
                {
                    CsrFtpServerCommonDialogSet(inst, TEXT_FTP_SERVER_APP_MAIN_MENU_UCS2, TEXT_FTP_SERVER_APP_REJECT_UCS2,
                        TEXT_YES_UCS2, NULL, inst->csrUiVar.hSk1Event, 0);
                }
                CsrBtFtsGetObjResSend(getObjInd->connectionId, finalFlag, inst->obexResult, inst->currentFileSize, bytesRead, body, FALSE);
                break;
            }
            case CSR_BT_FTP_SERVER_APP_NO_KEY:
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Rejecting the requested object (%s)", name);
                CsrBtFtsGetObjResSend(getObjInd->connectionId, TRUE, CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE, 0, 0 , NULL, FALSE);
                break;
            }
            case CSR_BT_FTP_SERVER_APP_AUTHENTICATE_KEY:
            {
                inst->csrUiVar.obexPasskeyUi = OBEX_PASSKEY_INITIATE;
                CsrUiUieShowReqSend(inst->csrUiVar.hObexPasskeyInputDialog, inst->queueId, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);

                inst->obexAuthentication = WITH_AUTHENTICATION;
                inst->ftsPasswordLength = 0;
                break;
            }
        }
        CsrPmemFree(name);
        if(inst->ftsGetObjIndMsg)
        {
            CsrPmemFree(getObjInd->payload);
            CsrPmemFree(inst->ftsGetObjIndMsg);
            inst->ftsGetObjIndMsg = NULL;
        }
        if(csrUiVar->allowMenuUi)
        {
            CsrUiUieHideReqSend(csrUiVar->hAllowMenu);
            csrUiVar->allowMenuUi = FALSE;
        }
    }
    else if( (indication->displayElement == csrUiVar->hAllowMenu) && (indication->event == csrUiVar->hSk1Event) )
    {
        CsrBtFtsSetAddFolderInd *setAddFolderInd = inst->ftsSetAddFolderIndMsg;
        char path[CSR_BT_MAX_PATH_LENGTH];
        char buffer[CSR_BT_MAX_PATH_LENGTH];
        CsrUint8 *name;

        name = unicode2String(setAddFolderInd->payload + setAddFolderInd->ucs2nameOffset);

        switch(indication->key)
        {
            case CSR_BT_FTP_SERVER_APP_YES_KEY:
            {
                CsrSnprintf(path, CSR_BT_MAX_PATH_LENGTH, "%s%s", (CsrCharString *) inst->currentPath, (CsrCharString *) name);
                if(CsrBtMkDir(path) == 0)
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Directory %s was successfully created", name);
                    CsrBtFtsSetAddFolderResSend(setAddFolderInd->connectionId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                    if(CsrBtChdir(path)  )
                    {
                        printf("Unable to locate the directory: %s\n", path);
                    }
                    else
                    {
                        sprintf(inst->currentPath, "%s%s"PATH_SEP, (CsrCharString *) inst->currentPath, (CsrCharString *) name);
                    }
                    /* Get the current working directory: */
                    if((CsrBtGetCwd(buffer, CSR_BT_MAX_PATH_LENGTH)) == NULL)
                    {
                        perror("CsrBtGetCwd error");
                    }
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Problem creating directory %s", name);
                    CsrBtFtsSetAddFolderResSend(setAddFolderInd->connectionId, CSR_BT_OBEX_UNAUTHORIZED_CODE);
                }
                break;
            }
            case CSR_BT_FTP_SERVER_APP_NO_KEY:
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Rejecting creating the directory, read only");
                CsrBtFtsSetAddFolderResSend(setAddFolderInd->connectionId, CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE);
                break;
            }
            case CSR_BT_FTP_SERVER_APP_AUTHENTICATE_KEY:
            {
                /* Authenticate the client */
                inst->csrUiVar.obexPasskeyUi = OBEX_PASSKEY_INITIATE;
                CsrUiUieShowReqSend(inst->csrUiVar.hObexPasskeyInputDialog, inst->queueId, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);

                inst->obexAuthentication = WITH_AUTHENTICATION;
                inst->ftsPasswordLength = 0;
                break;
            }
        }
        CsrPmemFree(name);
        if(inst->ftsSetAddFolderIndMsg)
        {
            CsrPmemFree(setAddFolderInd->payload);
            CsrPmemFree(inst->ftsSetAddFolderIndMsg);
            inst->ftsSetAddFolderIndMsg = NULL;
        }
        if(csrUiVar->allowMenuUi)
        {
            CsrUiUieHideReqSend(csrUiVar->hAllowMenu);
            csrUiVar->allowMenuUi = FALSE;
        }
    }
    else if ( (indication->displayElement == csrUiVar->hCommonDialog) && (indication->event == csrUiVar->hSk1Event) )
    {
        /* Reject current operation */
        if (inst->transferObject)
        {
            inst->transferObject = FALSE;
            inst->obexResult     = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;
        }
        if(csrUiVar->commonDialogUi)
        {
            CsrUiUieHideReqSend(csrUiVar->hCommonDialog);
            csrUiVar->commonDialogUi = FALSE;
        }
    }
    else if (indication->event == csrUiVar->hSk2Event)
    {
        CsrUiUieHideReqSend(indication->displayElement);
    }
}

void CsrFtpServerCommonDialogSet(CsrBtObexFtpServerAppGlobalInstData *inst,  CsrUint16 *dialogHeading,  CsrUint16 *dialogText, CsrUint16 *textSK1, CsrUint16 *textSK2, CsrUieHandle eventSK1, CsrUieHandle eventSK2)
{
    CsrUiDialogSetReqSend(inst->csrUiVar.hCommonDialog, dialogHeading, dialogText, textSK1, textSK2,
        eventSK1, eventSK2, inst->csrUiVar.hBackEvent, 0);
    CsrUiUieShowReqSend(inst->csrUiVar.hCommonDialog, inst->queueId, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
    inst->csrUiVar.commonDialogUi = TRUE;
}

void CsrBtFtpServerAppHandleCsrUiActivateDeactivate(CsrBtObexFtpServerAppGlobalInstData *inst, CsrBool activated)
{
    CsrBtObexFtpServerAppUiInstData *csrUiVar = &inst->csrUiVar;

    if(activated)
    {
        /* Update main menu so it is no longer possible to activate but just to deactivate */
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_FIRST, CSR_BT_FTP_SERVER_APP_DEACTIVATE_KEY, CSR_UI_ICON_NONE,
            TEXT_FTP_SERVER_APP_DEACTIVATE_UCS2, NULL,
            csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);

        CsrUiMenuRemoveitemReqSend(csrUiVar->hMainMenu, CSR_BT_FTP_SERVER_APP_ACTIVATE_KEY);
        CsrUiMenuRemoveitemReqSend(csrUiVar->hMainMenu, CSR_BT_FTP_SERVER_APP_ACTIVATE_WITH_AUTHENTICATION_KEY);
    }
    else
    {
        /* Update main menu so it is no longer possible to deactivate but just to activate */
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_FIRST, CSR_BT_FTP_SERVER_APP_ACTIVATE_WITH_AUTHENTICATION_KEY,
            CSR_UI_ICON_NONE, TEXT_FTP_SERVER_APP_ACTIVATE_WITH_AUTHENTICATION_UCS2, NULL,
            csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_FIRST, CSR_BT_FTP_SERVER_APP_ACTIVATE_KEY, CSR_UI_ICON_NONE,
            TEXT_FTP_SERVER_APP_ACTIVATE_UCS2, NULL,
            csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);

        CsrUiMenuRemoveitemReqSend(csrUiVar->hMainMenu, CSR_BT_FTP_SERVER_APP_DEACTIVATE_KEY);
    }
}


void CsrBtFtpServerAppHandleCsrUiPrim(CsrBtObexFtpServerAppGlobalInstData *inst)
{
    switch(*((CsrUint16 *) inst->message))
    {
        case CSR_UI_UIE_CREATE_CFM:
        {
            handleCsrUiUieCreateCfm(inst, inst->message);
            break;
        }
        case CSR_UI_INPUTDIALOG_GET_CFM:
        {
            handleCsrUiInputDialogGetCfm(inst, inst->message);
            break;
        }
        case CSR_UI_KEYDOWN_IND:
        {
            CsrUiKeydownInd *indication;
            indication = (CsrUiKeydownInd *) inst->message;
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "UIKeydownInd DE: 0x%04x, key: 0x%04x", indication->displayElement, indication->key);
            break;
        }
        case CSR_UI_EVENT_IND:
        {
            handleCsrUiUieEventInd(inst, inst->message);
            break;
        }
        default:
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unhandled UI message 0x%04x", *((CsrUint16 *) inst->message));
            break;
        }
    }
}

