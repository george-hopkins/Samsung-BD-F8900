/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_unicode.h"
#include "csr_util.h"
#include "csr_file.h"
#include "csr_exceptionhandler.h"

#include "csr_app_lib.h"
#include "csr_ui_lib.h"

#include "csr_bt_platform.h"
#include "csr_bt_ui_strings.h"
#include "csr_bt_util.h"
#include "csr_bt_file.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_ftc_prim.h"
#include "csr_bt_ftc_lib.h"

#include "csr_bt_obex_ftp_common.h"
#include "csr_bt_obex_ftp_client_strings.h"
#include "csr_bt_obex_ftp_client_ui_sef.h"
#include "csr_bt_obex_ftp_client_sef.h"

/* Create a copy of a string */
char *StrInst(char *s)
{
    char *t;

    t = CsrPmemAlloc(CsrStrLen(s) + 1);
    CsrStrCpy(t, s);
    t[CsrStrLen(s)] = '\0';

    return t;
}


/* Fetch an XML attribute value */
void getAttributeValue(char *element, CsrUint16 length, char **value)
{
    CsrUint16 i;

    i = 0;
    while((i < length) && (element[i] != '"'))
    {
        i++;
    }
    if(i < length)
    {
        i++;
        *value = element + i;
        while((i < length) && (element[i] != '"'))
        {
            i++;
        }
        element[i] = '\0';
    }
    else
    {
        *value = NULL;
    }
}

/* Get transfer speed */
double transferGetSpeed(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrInt32 usec;
    double speed;
    double msec;

    usec = CsrTimeSub(CsrTimeGet(NULL), inst->file_time);

    if(usec > 0)
    {
        msec = (double) (usec/1000);
        speed = 8 * (((double)inst->file_idx) / msec / 1024);
    }
    else
    {
        speed = 0.0;
    }

    return speed;
}

/* Show transfer statistics */
void transferStats(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    if( inst->file_idx > inst->file_progress )
    {
        char buf[256];

        inst->file_progress = inst->file_idx + inst->file_size/10;
        
        sprintf(buf, "Transferred %8u of %8u bytes, speed %4.3f Mbit/s\n",
               inst->file_idx,
               inst->file_size,
               transferGetSpeed(inst));

        if(inst->csrUiVar.commonDialogUi)
        {
            CsrUint16 *textSk2      = NULL;
            CsrUieHandle eventSk2   = CSR_UI_DEFAULTACTION;

#ifdef CSR_AMP_ENABLE
            if(inst->channelInUse == CSR_BT_AMP_CONTROLLER_BREDR)
            {
                textSk2     = TEXT_FTP_CLIENT_APP_TO_AMP_UCS2;
                eventSk2    = inst->csrUiVar.hSk2AmpEvent;
            }
            else
            {
                textSk2     = TEXT_FTP_CLIENT_APP_TO_EDR_UCS2;
                eventSk2    = inst->csrUiVar.hSk2BrEdrEvent;
            }
#endif

            CsrUiDialogSetReqSend(inst->csrUiVar.hCommonDialog, TEXT_FTP_CLIENT_APP_FILE_TRANSFER_UCS2, CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_ABORT_UCS2, textSk2, 
                inst->csrUiVar.hSk1AbortEvent, eventSk2, inst->csrUiVar.hBackEvent, CSR_UI_DEFAULTACTION);
        }
    }
}

/* End transfer (cleanup, etc.) */
void transferEnd(CsrBtObexFtpClientAppGlobalInstData *inst, CsrBool stats, CsrBool removeFile)
{
    if(stats)
    {
        char buf[256];
        
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Transfer speed: %4.3f Mbit", transferGetSpeed(inst));
        
        sprintf(buf, "%s operation of file '%s' completed.\n\n",
               (inst->targ_remote ? "GET" : "PUT"),
               inst->targ_name);

        if(inst->csrUiVar.commonDialogUi)
        {
            CsrUiDialogSetReqSend(inst->csrUiVar.hCommonDialog, TEXT_FTP_CLIENT_APP_FILE_TRANSFER_UCS2, CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL, 
                inst->csrUiVar.hSk1Event, CSR_UI_DEFAULTACTION, inst->csrUiVar.hBackEvent, CSR_UI_DEFAULTACTION);
        }
    }

    /* Cleanup */
    if(inst->file != NULL)
    {
        CsrBtFclose(inst->file);
        if(removeFile)
        {
            int    result;
            result  =  CsrBtRemove((char*)inst->targ_name);
            if(result == 0)
            {
                /* Success */
            }
            else
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Failed to delete file (%s)!", inst->targ_name);
            }
        }
    }
    inst->file = NULL;
    inst->file_idx = 0;
    inst->file_size = 0;
    inst->file_progress = 0;
    inst->file_time = CsrTimeGet(NULL);
}


/* Remote directory entry found, parse entries into browser-info */
void folderParseEntry(CsrBtObexFtpClientAppGlobalInstData *inst,
                      char *entry,
                      CsrUint16 length,
                      CsrBool is_dir)
{
    CsrUint32 i;
    CsrUint32 isize;
    char *name;
    char *size;

    i = 0;
    name = NULL;
    size = NULL;
    while((i < length) && CsrBtIsSpace(entry[i]))
    {
        i++;
    }
    entry[i] = '\0';
    while(i < length)
    {
        /* Find name entry */
        if((i + 4) < length && !CsrStrNCmp(entry + i, "name", 4))
        {
            i+=4;
            getAttributeValue(entry + i, (CsrUint16)(length - i), &name);
        }
        /* Find size entry */
        else if((i + 4) < length && !CsrStrNCmp(entry + i, "size", 4))
        {
            /* found "size" attribute */
            i+=4;
            getAttributeValue(entry + i, (CsrUint16)(length - i), &size);
        }

        /* Continue scanning */
        i++;
    }

    /* Make sure size is present */
    if(size == NULL)
    {
        size = "0";
    }

    /* Found both, add to browser table */
    if(name != NULL)
    {
        isize = atoi(size);
        browserAdd(inst, name, isize, is_dir);
    }
}

/* Parse current collection buffer */
void folderParseBuffer(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrUint32 index;
    CsrUint32 start;
    CsrBool found;
    CsrBool is_dir;

    /* Dummy checking */
    if((inst->folder == NULL) ||
       (inst->folder_size == 0))
    {
        return;
    }

    index = 0;
    start = 0;
    found = FALSE;
    is_dir = FALSE;
    while(index < inst->folder_size)
    {
        if(!found)
        {
            start = index;

            /* Scan for file element */
            if(((index + 5) < inst->folder_size) &&
               (CsrMemCmp((char*)inst->folder + index, "<file", 5) == 0))
            {
                found = TRUE;
                is_dir = FALSE;
                index += 5;
            }
            /* Scan for folder element */
            else if(((index + 8) < inst->folder_size) &&
                    (CsrMemCmp((char*)inst->folder + index, "<folder ", 8) == 0))
            {
                found = TRUE;
                is_dir = TRUE;
                index += 8;
            }
            /* Scan for special parent-folder element */
            else if(((index + 14) < inst->folder_size) &&
                    (CsrMemCmp((char*)inst->folder + index, "<parent-folder", 14) == 0))
            {
                /* Simply add the entry right away and continue scan */
                browserAdd(inst, "..", 0, TRUE);
                index += 15;
            }
        }
        /* Search for end of element */
        else
        {
            if(((index + 2) < inst->folder_size) &&
               (CsrMemCmp((char*)inst->folder + index, "/>", 2) == 0))
            {
                char *raw;
                char *ascii;

                /* Calculate end */
                raw = (char*)inst->folder + start;
                index += 2;

                /* Parse entry */
                ascii = CsrPmemAlloc(index - start + 1);
                CsrStrNCpy(ascii, raw, index - start);
                ascii[index - start] = '\0';
                folderParseEntry(inst,
                                 ascii,
                                 (CsrUint16)(index - start),
                                 is_dir);
                CsrPmemFree(ascii);

                /* Cull buffer */
                raw = CsrPmemAlloc(inst->folder_size - index);
                CsrMemCpy((char*)raw,
                       (char*)((char*)(inst->folder) + index),
                       inst->folder_size - index);
                CsrPmemFree(inst->folder);
                inst->folder = raw;
                inst->folder_size -= index;
                inst->folder_idx -= index;

                /* Since buffer was culled, start from beginning */
                found = FALSE;
                index = 0;
                continue;
            }
        }

        /* Continue scanning buffer */
        index++;
    }
}

/* Receive remote folder listing */
void folderReceive(CsrBtObexFtpClientAppGlobalInstData *inst,
                   void *body,
                   CsrUint16 length,
                   CsrUint32 total)
{
    /* Allocate new folder chunk */
    if(total != 0)
    {
        if(inst->folder != NULL)
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Tried to alloc new folder-list, but old one existed!");
            CsrBtFtcDisconnectReqSend(TRUE);
#ifdef ENABLE_SHUTDOWN
            CsrSchedStop();
#else
            exit(1);
#endif
            return;
        }

        if(total > MAX_FOLDER_BUFFER)
        {
            inst->folder_size = MAX_FOLDER_BUFFER;
        }
        else
        {
            inst->folder_size = total;
        }

        inst->folder = CsrPmemAlloc(inst->folder_size);
        inst->folder_idx = 0;
        CsrMemSet(inst->folder, 0, inst->folder_size);
    }

    /* Copy received data into buffer */
    if((length > 0) && (body != NULL))
    {
        if((inst->folder_idx + length) > inst->folder_size)
        {
            void *tmp;

            /* Allocate more space for the folder data */
            inst->folder_size += length;
            tmp = CsrPmemAlloc(inst->folder_size);

            /* Copy already received data into buffer */
            CsrMemCpy(tmp, inst->folder, inst->folder_idx);
            CsrPmemFree(inst->folder);
            inst->folder = tmp;
        }

        /* Copy received data into buffer */
        CsrMemCpy(((char*)inst->folder + inst->folder_idx),
               body,
               length);
        inst->folder_idx += length;
    }

    /* Parse entries to save memory in the long run */
    folderParseBuffer(inst);
}

/* Last part of remote folder listing received */
void folderFinalize(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    /* Make sure collection buffer is empty */
    folderParseBuffer(inst);

    /* Add special root folder */
    browserAdd(inst, "/", 0, TRUE);

    /* Display what we've just parsed */
    CsrUiMenuSetReqSend(inst->csrUiVar.hFileBrowser, TEXT_FTP_CLIENT_APP_FILEBROWSER_REMOTE_UCS2, TEXT_OPTIONS_UCS2, NULL);
    playBrowser(inst);

    /* Clear the collection buffer */
    CsrPmemFree(inst->folder);
    inst->folder = NULL;
    inst->folder_idx = 0;
    inst->folder_size = 0;

}


/* Handle the FTC messages */
void handleFtcEvents(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrBtFtcPrim type;

    type = *((CsrBtFtcPrim*)inst->msg);
    switch(type)
    {
        case CSR_BT_FTC_CONNECT_CFM:
            {
                CsrBtFtcConnectCfm* prim;
                prim = (CsrBtFtcConnectCfm*)inst->msg;

                if (prim->resultCode     == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE &&
                    prim->resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX)
                {
                    if(inst->csrUiVar.commonDialogUi)
                    {
                        CsrUiUieHideReqSend(inst->csrUiVar.hCommonDialog);
                        inst->csrUiVar.commonDialogUi = FALSE;
                    }

                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Connect successful! (btConnId 0x%08x)", prim->btConnId);
                    inst->channelInUse = CSR_BT_AMP_CONTROLLER_BREDR;
                    inst->btConnId = prim->btConnId;
                    updateMainFtcMenu(inst);
                    CsrUiUieShowReqSend(inst->csrUiVar.hMainMenu, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
                    inst->csrUiVar.mainMenuUi = TRUE;

#ifdef CSR_AMP_ENABLE
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Registering with AMP manager...");
                    CsrBtAmpmRegisterReqSend(inst->ftcSchedQid,
                                             0, /* flags */
                                             CSR_BT_AMPM_EVENTS_AUTO_MOVE, /* eventMask */
                                             inst->btConnId, /* btConnId */
                                             &inst->address); /* addr */
#endif
                }
                else
                {
                    char buf[256];

                    sprintf(buf, "Connect failed - resultCode: 0x%04X, resultSupplier: 0x%04X", prim->resultCode, prim->resultSupplier);

                    if(inst->csrUiVar.commonDialogUi)
                    {
                        CsrUiDialogSetReqSend(inst->csrUiVar.hCommonDialog, TEXT_FTP_CLIENT_APP_FILE_TRANSFER_UCS2, CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_OK_UCS2, NULL,
                            inst->csrUiVar.hSk1Event, CSR_UI_DEFAULTACTION, inst->csrUiVar.hBackEvent, CSR_UI_DEFAULTACTION);
                    }
                }
                break;
            }

        case CSR_BT_FTC_DISCONNECT_IND:
            {
                CsrBtFtcDisconnectInd* prim;
                char buf[256];

                prim = (CsrBtFtcDisconnectInd*)inst->msg;
                cleanInstance(inst);
                inst->btConnId = CSR_BT_CONN_ID_INVALID;
                
                sprintf(buf,"Disconnected, code: 0x%04X, sup: 0x%04X",
                       prim->reasonCode, 
                       prim->reasonSupplier);

                if(inst->csrUiVar.commonDialogUi)
                {
                    CsrUiUieHideReqSend(inst->csrUiVar.hCommonDialog);
                    inst->csrUiVar.commonDialogUi = FALSE;
                }
                
                updateMainFtcMenu(inst);
                CsrUiUieShowReqSend(inst->csrUiVar.hMainMenu, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
                inst->csrUiVar.mainMenuUi = TRUE;

                commonDialogSet(inst, TEXT_FTP_CLIENT_APP_MAIN_MENU_UCS2, CONVERT_TEXT_STRING_2_UCS2(buf),
                TEXT_OK_UCS2, NULL, inst->csrUiVar.hSk1Event, 0);

                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, buf);

#ifdef CSR_AMP_ENABLE
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Unregistering with AMPM...");
                CsrBtAmpmDeregisterReqSend(inst->ftcSchedQid,
                                           inst->btConnId,
                                           &inst->address);
#endif
                break;
            }

        case CSR_BT_FTC_AUTHENTICATE_IND:
            {
                CsrBtFtcAuthenticateInd *prim = (CsrBtFtcAuthenticateInd *) inst->msg;

                inst->auth_user = NULL;
                inst->auth_local = FALSE;
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "OBEX auth requested by server.");
                if( (prim->options & 0x01) == 0x01)
                {
                    inst->auth_state = AUTH_USERID;
                }
                else
                {
                    inst->auth_state = AUTH_PASS;
                }
                setPasswordInputdialog(inst);
                CsrUiUieShowReqSend(inst->csrUiVar.hObexPasskeyInputDialog, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
                inst->csrUiVar.obexPasskeyUi = TRUE;
                CsrPmemFree(prim->realm);
                break;
            }

        case CSR_BT_FTC_GET_LIST_FOLDER_CFM:
            {
                CsrBtFtcGetListFolderCfm *prim;

                prim = inst->msg;
                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    folderReceive(inst,
                                  prim->payload + prim->bodyOffset,
                                  prim->bodyLength,
                                  prim->lengthOfObject);

                    /* Did we receive the last part? */
                    if(prim->finalFlag)
                    {
                        folderFinalize(inst);
                    }
                    else
                    {
                        CsrBtFtcGetListFolderBodyReqSend(FALSE);
                    }
                    CsrPmemFree(prim->payload);
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Get folder list failed, OBEX responseCode 0x%04x",
                           prim->responseCode);
                }
                break;
            }

        case CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM:
            {
                CsrBtFtcGetListFolderBodyCfm *prim;

                prim = (CsrBtFtcGetListFolderBodyCfm*)inst->msg;
                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    folderReceive(inst,
                                  prim->payload + prim->bodyOffset,
                                  prim->bodyLength,
                                  0);

                    /* Did we receive the last part? */
                    if(prim->finalFlag)
                    {
                        folderFinalize(inst);
                    }
                    else
                    {
                        CsrBtFtcGetListFolderBodyReqSend(FALSE);
                    }
                    CsrPmemFree(prim->payload);
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Get folder body list failed, OBEX responseCode 0x%04x",
                           prim->responseCode);
                }
                break;
            }

        case CSR_BT_FTC_SET_FOLDER_CFM:
            /* Fallthrough */
        case CSR_BT_FTC_SET_BACK_FOLDER_CFM:
            /* Fallthrough */
        case CSR_BT_FTC_SET_ROOT_FOLDER_CFM:
            {
                CsrBtFtcSetFolderCfm *prim;
                prim = (CsrBtFtcSetFolderCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    /* Set new directory which we tracked before sending the req */
                    if(inst->dir_newrem != NULL)
                    {
                        CsrPmemFree(inst->dir_remote);
                        inst->dir_remote = inst->dir_newrem;
                        inst->dir_newrem = NULL;
                    }
                    updateMainFtcMenu(inst);
                    CsrUiUieShowReqSend(inst->csrUiVar.hMainMenu, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
                    inst->csrUiVar.mainMenuUi = TRUE;
                }
                else
                {
                    if(inst->dir_newrem != NULL)
                    {
                        CsrPmemFree(inst->dir_newrem);
                        inst->dir_newrem = NULL;
                    }
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Set folder failed, OBEX responseCode 0x%04x",
                           prim->responseCode);
                }
                break;
            }

        case CSR_BT_FTC_PUT_OBJ_HEADER_CFM:
            {
                CsrBtFtcPutObjHeaderCfm *prim;
                CsrBool finalFlag;
                CsrUint16 length;
                CsrUint8 *body;

                prim = (CsrBtFtcPutObjHeaderCfm*)inst->msg;

                /* Read in data */
                body = CsrPmemAlloc(prim->bodySize);
#ifdef FAKE_FILE
                length = prim->bodySize;
#else
                length = CsrBtFread(body,
                               1,
                               prim->bodySize,
                               inst->file);
#endif

#ifdef FAKE_FILE
                if (inst->file_idx + length >= inst->file_size)
#else
                /* End reached? */
                if(CsrBtIsAtEof(inst->file, inst->file_size))
#endif
                {
                    /* Close file now, obj-body handler will end transfer */
                    finalFlag = TRUE;
                    CsrBtFclose(inst->file);
                    inst->file = NULL;
                }
                else
                {
                    finalFlag = FALSE;
                }

                /* Show progress and send data request */
                CsrBtFtcPutObjBodyReqSend(finalFlag,
                                          length,
                                          body);
                transferStats(inst);
                break;
            }

        case CSR_BT_FTC_PUT_OBJ_BODY_CFM:
            {
                CsrBtFtcPutObjBodyCfm *prim;
                CsrBool finalFlag;
                CsrUint16 length;
                CsrUint8 *body;

                prim = (CsrBtFtcPutObjBodyCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    /* File still open, more to transfer */
                    if(inst->file != NULL)
                    {

                        if(inst->file_idx == 0){
                            /* Transfer is now accepted from server side and we can thus set the time stamp.*/
                            inst->file_time = CsrTimeGet(NULL);
                        }

                        /* Read in data */
                        body = CsrPmemAlloc(prim->bodySize);
#ifdef FAKE_FILE
                        length = prim->bodySize;
#else
                        length = CsrBtFread(body,
                                       1,
                                       prim->bodySize,
                                       inst->file);
#endif

#ifdef FAKE_FILE
                        if (inst->file_idx + length >= inst->file_size)
#else
                        /* End reached? */
                        if(CsrBtIsAtEof(inst->file, inst->file_size))
#endif
                        {
                            /* Close file now, obj-body handler will end transfer */
                            finalFlag = TRUE;
                            CsrBtFclose(inst->file);
                            inst->file = NULL;
                        }
                        else
                        {
                            finalFlag = FALSE;
                        }

                        /* Show progress and send data request */
                        inst->file_idx += length;
                        transferStats(inst);
                        CsrBtFtcPutObjBodyReqSend(finalFlag,
                                                  length,
                                                  body);
                    }

                    /* File was closed, to transfer has ended */
                    else
                    {
                        transferEnd(inst, TRUE, FALSE);
                    }
                }
                else
                {
                    transferEnd(inst, FALSE, FALSE);
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Put obj body failed, OBEX responseCode 0x%04x",
                           prim->responseCode);
                }
                break;
            }

        case CSR_BT_FTC_GET_OBJ_CFM:
            {
                CsrBtFtcGetObjCfm *prim;

                prim = (CsrBtFtcGetObjCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    /* Transfer is now accepted from server side and we can thus set the time stamp.*/
                    inst->file_time = CsrTimeGet(NULL);

                    if(inst->file)
                    {
                        /* Data to write */
                        if(prim->bodyOffset > 0)
                        {
#ifndef FAKE_FILE
                            CsrBtFwrite((prim->payload + prim->bodyOffset),
                                   1,
                                   prim->bodyLength,
                                   inst->file);
#endif
                        }

                        /* Show progress */
                        inst->file_idx = prim->bodyLength;
                        inst->file_size = prim->lengthOfObject;
                        if(inst->file_size != 0)
                        {
                            transferStats(inst);
                        }

                        /* Finished? */
                        if(prim->finalFlag)
                        {
                            /* Fully end transfer */
                            CsrBtFclose(inst->file);
                            inst->file = NULL;
                            transferEnd(inst, TRUE, FALSE);
                        }
                        else
                        {
                            CsrBtFtcGetObjBodyReqSend(FALSE);
                        }
                    }
                    CsrPmemFree(prim->payload);
                }
                else
                {
                    transferEnd(inst, FALSE, TRUE);
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Get obj failed, OBEX responseCode 0x%04x",
                           prim->responseCode);
                }
                break;
            }

        case CSR_BT_FTC_GET_OBJ_BODY_CFM:
            {
                CsrBtFtcGetObjBodyCfm *prim;

                prim = (CsrBtFtcGetObjBodyCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    /* Data to write */
                    if(prim->bodyOffset > 0)
                    {
#ifndef FAKE_FILE
                        CsrBtFwrite((prim->payload + prim->bodyOffset),
                               1,
                               prim->bodyLength,
                               inst->file);
#endif
                    }

                    /* Show progress */
                    inst->file_idx += prim->bodyLength;
                    if(inst->file_size != 0)
                    {
                        transferStats(inst);
                    }

                    /* Finished? */
                    if(prim->finalFlag)
                    {
                        /* Fully end transfer */
                        transferEnd(inst, TRUE, FALSE);
                    }
                    else
                    {
                        CsrBtFtcGetObjBodyReqSend(FALSE);
                    }
                    CsrPmemFree(prim->payload);
                }
                else
                {
                    transferEnd(inst, FALSE, FALSE);
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Get obj body failed, OBEX responseCode 0x%04x",
                           prim->responseCode);
                }
                break;
            }

        case CSR_BT_FTC_ABORT_CFM:
            {
                /* Cleanup */
                if(inst->file != NULL)
                {
                    CsrBtFclose(inst->file);

                    if(inst->targ_remote)
                    {
                        if(CsrBtRemove((char*)inst->targ_name) != 0)
                        {
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Failed to delete file (%s)!", inst->targ_name);
                        }
                    }
                }
                inst->file = NULL;
                inst->file_idx = 0;
                inst->file_size = 0;
                inst->file_progress = 0;
                inst->file_time = CsrTimeGet(NULL);

                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Abort confirm received!");
                break;
            }


        case CSR_BT_FTC_DEL_OBJ_CFM:
            {
                CsrBtFtcDelObjCfm *prim;

                prim = (CsrBtFtcDelObjCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    CsrPmemFree(inst->targ_name);
                    inst->targ_name = StrInst("");
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Successfully deleted remote file!");
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Delete obj failed, OBEX responseCode 0x%04x",
                           prim->responseCode);
                }
                break;
            }

        case CSR_BT_FTC_SET_ADD_FOLDER_CFM:
            {
                CsrBtFtcSetAddFolderCfm *prim;

                prim = (CsrBtFtcSetAddFolderCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    /* Set new directory which we tracked before sending the req */
                    if(inst->dir_newrem != NULL)
                    {
                        CsrPmemFree(inst->dir_remote);
                        inst->dir_remote = inst->dir_newrem;
                        inst->dir_newrem = NULL;
                    }
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Successfully created and changed to new directory");
                }
                else
                {
                    if(inst->dir_newrem != NULL)
                    {
                        CsrPmemFree(inst->dir_newrem);
                        inst->dir_newrem = NULL;
                    }
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Create directory failed, OBEX responseCode 0x%04x",
                           prim->responseCode);
                }
                break;
            }

        case CSR_BT_FTC_COPYING_OBJ_CFM:
            {
                CsrBtFtcCopyingObjCfm *prim;

                prim = (CsrBtFtcCopyingObjCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Successfully copying file");
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Object Copy Failed, OBEX responseCode 0x%04x",
                           prim->responseCode);
                }
                break;
            }

        case CSR_BT_FTC_MOVING_OBJ_CFM:
            {
                CsrBtFtcMovingObjCfm *prim;

                prim = (CsrBtFtcMovingObjCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Successfully moving file");
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Object Move Failed, OBEX responseCode 0x%04x",
                           prim->responseCode);
                }
                break;
            }

        case CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM:
            {
                CsrBtFtcSetObjPermissionsCfm *prim;

                prim = (CsrBtFtcSetObjPermissionsCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Successfully SetPerm");
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Set Perm Failed, OBEX responseCode 0x%04x",
                           prim->responseCode);
                }
                break;
            }

        default:
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Unhandled FTC primitive 0x%04X", type);
                CsrBtFtcFreeUpstreamMessageContents(CSR_BT_FTC_PRIM, inst->msg);
                break;
            }
    }
}


/* Handle AMPM primitives */
#ifdef CSR_AMP_ENABLE
void handleFtcAmpmPrim(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    switch(*(CsrBtAmpmPrim*)inst->msg)
    {
        case CSR_BT_AMPM_REGISTER_CFM:
            {
                CsrBtAmpmRegisterCfm *prim;
                prim = (CsrBtAmpmRegisterCfm*)inst->msg;
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "AMPM reg cfm: btConnId 0x%08x, result 0x%04x, supplier 0x%04x",
                       prim->btConnId, prim->resultCode, prim->resultSupplier);
            }
            break;

        case CSR_BT_AMPM_DEREGISTER_CFM:
            {
                CsrBtAmpmDeregisterCfm *prim;
                prim = (CsrBtAmpmDeregisterCfm*)inst->msg;
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "AMPM unreg cfm: btConnId 0x%08x, supplier 0x%04x, result 0x%04x",
                       prim->btConnId, prim->resultSupplier, prim->resultCode);
            }
            break;

        case CSR_BT_AMPM_CONTROLLER_IND:
        case CSR_BT_AMPM_CONTROLLER_CFM:
            {
                CsrBtAmpmControllerInd *prim;
                CsrUint8 i;

                prim = (CsrBtAmpmControllerInd*)inst->msg;
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "AMP ctrl listing (sup 0x%04x, code 0x%04x)",
                       prim->resultSupplier, prim->resultCode);
                
                for(i = 0; i < prim->ampsCount; i++)
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "ampType:0x%02x status:0x%02x caps:0x%08x localId:0x%04x remoteId:0x%04x",
                           prim->amps[i].ampType,
                           prim->amps[i].status,
                           prim->amps[i].caps,
                           prim->amps[i].localId,
                           prim->amps[i].remoteId);
                }
                
                if((prim->resultSupplier == CSR_BT_SUPPLIER_AMPM) &&
                   (prim->resultCode == CSR_BT_AMPM_RESULT_SUCCESS))
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Init move to ctrl 0x%04x/0x%04x...",
                           prim->amps[0].localId, prim->amps[0].remoteId);
                    CsrBtAmpmMoveReqSend(inst->btConnId, 
                                         prim->amps[0].localId, prim->amps[0].remoteId);
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Could not init move");
                }

                CsrPmemFree(prim->amps);
                prim->amps = NULL;
            }
            break;

        case CSR_BT_AMPM_MOVE_IND:
            {
                CsrBtAmpmMoveInd *prim;
                prim = (CsrBtAmpmMoveInd*)inst->msg;
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "AMP move ind, btConnId 0x%08x, localId 0x%04x - accepting...",
                       prim->btConnId, prim->localId);
                CsrBtAmpmMoveResSend(prim->btConnId, TRUE);
            }
            break;


        case CSR_BT_AMPM_MOVE_CMP_IND:
            {
                CsrBtAmpmMoveCmpInd *prim;
                prim = (CsrBtAmpmMoveCmpInd*)inst->msg;
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "AMP move cmplt ind (%s), result 0x%x, supplier 0x%x, btConnId 0x%08x",
                       (prim->localId == CSR_BT_AMP_CONTROLLER_BREDR)?"BR/EDR":"WIFI", prim->resultCode, prim->resultSupplier, prim->btConnId);

                if( (prim->resultCode == CSR_BT_AMPM_RESULT_SUCCESS) && (prim->resultSupplier == CSR_BT_SUPPLIER_AMPM) )
                {
                    inst->channelInUse = prim->localId;
                    updateMainFtcMenu(inst);
                }
            }
            break;

        case CSR_BT_AMPM_MOVE_CFM:
            {
                CsrBtAmpmMoveCfm *prim;
                prim = (CsrBtAmpmMoveCfm*)inst->msg;
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "AMP move cfm (%s), result 0x%x, supplier 0x%x, btConnId 0x%08x",
                       (prim->localId == CSR_BT_AMP_CONTROLLER_BREDR)?"BR/EDR":"WIFI", prim->resultCode, prim->resultSupplier, prim->btConnId);

                if( (prim->resultCode == CSR_BT_AMPM_RESULT_SUCCESS) && (prim->resultSupplier == CSR_BT_SUPPLIER_AMPM) )
                {
                    inst->channelInUse = prim->localId;
                    updateMainFtcMenu(inst);
                }
            }
            break;

        case CSR_BT_AMPM_AUTO_MOVE_CMP_IND:
            {
                CsrBtAmpmAutoMoveCmpInd *prim;
                prim = (CsrBtAmpmAutoMoveCmpInd*)inst->msg;
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "AMP auto move cmplt ind (%s), result 0x%x, supplier 0x%x, btConnId 0x%08x",
                       (prim->localId == CSR_BT_AMP_CONTROLLER_BREDR)?"BR/EDR":"WIFI", prim->resultCode, prim->resultSupplier, prim->btConnId);

                if( (prim->resultCode == CSR_BT_AMPM_RESULT_SUCCESS) && (prim->resultSupplier == CSR_BT_SUPPLIER_AMPM) )
                {
                    inst->channelInUse = prim->localId;
                    updateMainFtcMenu(inst);
                }
            }
            break;

        default:
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Unhandled AMPM prim received: 0x%04x\n",
                   *(CsrPrim*)inst->msg);
            break;
    }
}
#endif

/* Handle CM primitives */
void handleFtcCmPrim(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    switch(*(CsrPrim*)inst->msg)
    {
        case CSR_BT_CM_MODE_CHANGE_IND:
            {
                /*
                CsrBtCmModeChangeInd *prim;
                prim = (CsrBtCmModeChangeInd *)inst->msg;

                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "CM Mode Change Ind received.mode = 0x%x", prim->mode);

                if (prim->mode == CSR_BT_SNIFF_MODE)
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Interval = 0x%x (%d)",
                           prim->interval,prim->interval);
                }
                */
            }
            break;

        case CSR_BT_CM_SET_EVENT_MASK_CFM:
            break;

        default:
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Unhandled CM prim received: 0x%04x", *(CsrPrim*)inst->msg);
            break;
    }
}
