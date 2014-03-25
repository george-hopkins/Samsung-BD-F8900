/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_unicode.h"
#include "csr_util.h"
#include "csr_formatted_io.h"
#include "csr_file.h"

#include "csr_app_lib.h"
#include "csr_ui_lib.h"

#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_ftc_lib.h"
#include "csr_bt_ui_strings.h"
#include "csr_bt_file.h"

#include "csr_bt_obex_ftp_common.h"
#include "csr_bt_obex_ftp_client_strings.h"
#include "csr_bt_obex_ftp_client_sef.h"
#include "csr_bt_obex_ftp_client_ui_sef.h"

#define MAX_MENU_ITEM_LENGTH    128

/* Start AMP move channel */
#ifdef CSR_AMP_ENABLE
static void startMoveChannel(CsrBtObexFtpClientAppGlobalInstData *inst, CsrUint8 target)
{
    if(inst->btConnId == CSR_BT_CONN_ID_INVALID)
    {
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "FTP is not using OBEX-over-L2CAP, so move channel not possible");
        return;
    }

    if(target == MOVE_TO_BREDR)
    {
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Initiate move to BR/EDR controller...");
        CsrBtAmpmMoveReqSend(inst->btConnId,
                             CSR_BT_AMP_CONTROLLER_BREDR,
                             CSR_BT_AMP_CONTROLLER_BREDR);
    }
    else if(target == MOVE_TO_AMP)
    {
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Requesting controllers from AMPM...");
        CsrBtAmpmControllerReqSend(inst->ftcSchedQid,
                                   &inst->address,
                                   inst->btConnId);
    }
}
#endif

static void setDirectoryInputdialog(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrBtObexFtpClientAppUiInstData *csrUiVar = &inst->csrUiVar;
    if(inst->targ_remote)
    {
        inst->mkdir_remote = TRUE;
        CsrUiInputdialogSetReqSend(csrUiVar->hDirectoryInputDialog, TEXT_FTP_CLIENT_APP_REMOTE_DIR_UCS2, NULL, 0,
            NULL, MAX_DIRLEN, CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL,
            csrUiVar->hSk1Event, CSR_UI_DEFAULTACTION, csrUiVar->hBackEvent, CSR_UI_DEFAULTACTION);
    }
    else /* local dir */
    {
        inst->mkdir_remote = FALSE;
        CsrUiInputdialogSetReqSend(csrUiVar->hDirectoryInputDialog, TEXT_FTP_CLIENT_APP_LOCAL_DIR_UCS2, NULL, 0,
            NULL, MAX_DIRLEN, CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL,
            csrUiVar->hSk1Event, CSR_UI_DEFAULTACTION, csrUiVar->hBackEvent, CSR_UI_DEFAULTACTION);
    }
}

static void setCopyInputdialog(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrBtObexFtpClientAppUiInstData *csrUiVar = &inst->csrUiVar;

    CsrUiInputdialogSetReqSend(csrUiVar->hCopyInputDialog, TEXT_FTP_CLIENT_APP_COPY_GET_PATH_UCS2, NULL, 0,
                               NULL, MAX_DIRLEN, CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL,
                               csrUiVar->hSk1Event, CSR_UI_DEFAULTACTION, csrUiVar->hBackEvent, CSR_UI_DEFAULTACTION);
}

static void setMoveInputdialog(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrBtObexFtpClientAppUiInstData *csrUiVar = &inst->csrUiVar;

    CsrUiInputdialogSetReqSend(csrUiVar->hMoveInputDialog, TEXT_FTP_CLIENT_APP_MOVE_GET_PATH_UCS2, NULL, 0,
                               NULL, MAX_DIRLEN, CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL,
                               csrUiVar->hSk1Event, CSR_UI_DEFAULTACTION, csrUiVar->hBackEvent, CSR_UI_DEFAULTACTION);
}

static void setSetPermInputdialog(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrBtObexFtpClientAppUiInstData *csrUiVar = &inst->csrUiVar;

    CsrUiInputdialogSetReqSend(csrUiVar->hSetPermInputDialog, TEXT_FTP_CLIENT_APP_SET_PERM_UCS2, NULL, 0,
                               NULL, MAX_DIRLEN, CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL,
                               csrUiVar->hSk1Event, CSR_UI_DEFAULTACTION, csrUiVar->hBackEvent, CSR_UI_DEFAULTACTION);
}

void handleMakedir(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    setDirectoryInputdialog(inst);
    CsrUiUieShowReqSend(inst->csrUiVar.hDirectoryInputDialog, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
    inst->csrUiVar.directoryInputDialogUi = TRUE;
}

void handleMove(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    setMoveInputdialog(inst);
    CsrUiUieShowReqSend(inst->csrUiVar.hMoveInputDialog, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
    inst->csrUiVar.moveInputDialogUi = TRUE;
}

void handleCopy(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    setCopyInputdialog(inst);
    CsrUiUieShowReqSend(inst->csrUiVar.hCopyInputDialog, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
    inst->csrUiVar.copyInputDialogUi = TRUE;
}

void handleSetPerm(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    setSetPermInputdialog(inst);
    CsrUiUieShowReqSend(inst->csrUiVar.hSetPermInputDialog, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
    inst->csrUiVar.setPermInputDialogUi = TRUE;
}

/* Start file transfer */
void transferStart(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    char *mode;
    CsrBtStat_t info;
    CsrUint8 *unicode;

    if(CsrStrLen(inst->targ_name) == 0)
    {
        return;
    }

    /* Clean stat info */
    CsrMemSet(&info, 0, sizeof(CsrBtStat_t));

    /* Check get */
    if(inst->targ_remote)
    {
        /* Open for writing (get), file must not exist */
        mode = "wb";
        if(CsrBtStat(inst->targ_name, &info) == 0)
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "File already exists");
            return;
        }
        inst->file_size = 0;
    }
    else
    {
        /* Open for reading (put), file must exist */
        mode = "rb";
        if(CsrBtStat(inst->targ_name, &info) != 0)
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "File does not exist!");
            return;
        }
        inst->file_size = info.st_size;
    }

    /* Open */
    inst->file = CsrBtFopen(inst->targ_name, mode);
    inst->file_idx = 0;
    if(inst->file == NULL)
    {
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Could not open file!");
        return;
    }

    /* Ok, start transfer */
    {
        char buf[256];

        sprintf(buf, "Started transfer of '%s', size: %u...\n",
           inst->targ_name, inst->file_size);

        if(inst->csrUiVar.commonDialogUi)
        {
            CsrUiDialogSetReqSend(inst->csrUiVar.hCommonDialog, TEXT_FTP_CLIENT_APP_FILE_TRANSFER_UCS2, CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_ABORT_UCS2, NULL,
                inst->csrUiVar.hSk1AbortEvent, CSR_UI_DEFAULTACTION, inst->csrUiVar.hBackEvent, CSR_UI_DEFAULTACTION);
        }
        else
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
            commonDialogSet(inst, TEXT_FTP_CLIENT_APP_FILE_TRANSFER_UCS2, CONVERT_TEXT_STRING_2_UCS2(buf),
                TEXT_ABORT_UCS2, textSk2, inst->csrUiVar.hSk1AbortEvent, eventSk2);
        }
    }
    /* playMenu -> new UI */
    unicode = CsrUtf82Ucs2ByteString((CsrUint8*)inst->targ_name);

    /* Remote file, get it */
    if(inst->targ_remote)
    {
        CsrBtFtcGetObjReqSend(unicode, FALSE);
    }
    else
    {
        CsrBtFtcPutObjHeaderReqSend(unicode,
                                    info.st_size);
    }
}

/* Start local file selection */
void selectLocal(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    DIR           *dir;
    struct dirent *entry;
    CsrBtStat_t    info;
    CsrBool        is_dir;

    browserEmpty(inst);
    inst->targ_remote = FALSE;

    /* Read directory */
    dir = CsrBtOpenDir(inst->dir_local);
    if(dir)
    {
        while((entry = CsrBtReadDir(dir)) != NULL)
        {
            CsrMemSet(&info, 0, sizeof(CsrBtStat_t));
            if(CsrBtStat(entry->d_name, &info) == 0)
            {
                if((info.st_mode & S_IFDIR) == S_IFDIR)
                {
                    is_dir = TRUE;
                }
                else
                {
                    is_dir = FALSE;
                }
                browserAdd(inst, entry->d_name, info.st_size, is_dir);
            }
        }
        CsrBtCloseDir(dir);

        /* Display what we've just read */
        CsrUiMenuSetReqSend(inst->csrUiVar.hFileBrowser, TEXT_FTP_CLIENT_APP_FILEBROWSER_LOCAL_UCS2, TEXT_OPTIONS_UCS2, NULL);
        playBrowser(inst);
    }
    else
    {
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Current local directory (%s) is empty", inst->dir_local);
    }
}

/* Start remote file selection */
void selectRemote(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrUint8 *unicode;

    browserEmpty(inst);
    inst->targ_remote = TRUE;

    /* Request folder information */
    unicode = CsrUtf82Ucs2ByteString((CsrUint8 *) "");
    CsrBtFtcGetListFolderReqSend(unicode, FALSE);
    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Transferring folder information...");
}

/* Delete selected target */
void selectDelete(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrUint8 *unicode;

    /* Catch local file deletion */
    if(!inst->targ_remote)
    {
        int res;
        if(inst->targ_dir)
        {
            res = recursiveRmDir(inst->targ_name);
        }
        else
        {
            res = CsrBtRemove(inst->targ_name);
        }
        if(res != 0)
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Could not delete target '%s', error code %d",
                   inst->targ_name, res);
        }
        else
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Local target successfully deleted");
        }
        /* Set new target */
        CsrPmemFree(inst->targ_name);
        inst->targ_name = StrInst("");

        return;
    }

    /* Request delete of remote file */
    unicode = CsrUtf82Ucs2ByteString((CsrUint8*)inst->targ_name);
    CsrBtFtcDelObjReqSend(unicode);
}

/* Change directory. Does not consume the parameters */
void selectDirectory(CsrBtObexFtpClientAppGlobalInstData *inst, char *dir, CsrBool remote)
{
    /* Local */
    if(!remote)
    {
        CsrBtChdir(dir);
        CsrPmemFree(inst->dir_local);
        inst->dir_local = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH);
        CsrBtGetCwd(inst->dir_local, CSR_BT_MAX_PATH_LENGTH - 1);
        if (inst->dir_local[CsrStrLen(inst->dir_local) - 1] != '/')
        {
            CsrStrCat(inst->dir_local, PATH_SEP);
        }
        /* Set new target */
        CsrPmemFree(inst->targ_name);
        inst->targ_name = StrInst("");
        /* playMenu -> new UI */
        return;
    }

    /* Ok, it's remote. Check if's one of the two specialities */
    if(CsrStrLen(dir) == 1)
    {
        if(!CsrStrNCmp(dir, "/", 2))
        {
            /* Track remote directory */
            inst->dir_newrem = StrInst("/");

            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Changing remote directory to 'root'...");
            CsrBtFtcSetRootFolderReqSend();
            /* Set new target */
            CsrPmemFree(inst->targ_name);
            inst->targ_name = StrInst("");

            return;
        }
    }
    else if(CsrStrLen(dir) == 2)
    {
        if(!CsrStrNCmp(dir, "..", 2))
        {
            int i;
            /* Backtrack remote directory */
            i = CsrStrLen(inst->dir_remote);

            if((i > 0) && (inst->dir_remote[i-1] == '/'))
            {
                i--;
            }

            while((i > 0) &&
                  (inst->dir_remote[i-1] != '\0') &&
                  (inst->dir_remote[i-1] != '/'))
            {
                i--;
            }

            if((i > 0) && (inst->dir_remote[i-1] == '/'))
            {
                i--;
            }

            inst->dir_newrem = CsrPmemAlloc(i + 1);
            strncpy(inst->dir_newrem, inst->dir_remote, i);
            inst->dir_newrem[i] = '\0';

            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Changing remote directory to 'parent'...");
            CsrBtFtcSetBackFolderReqSend();

            /* Set new target */
            CsrPmemFree(inst->targ_name);
            inst->targ_name = StrInst("");
            return;
        }
    }

    /* Track remote directory */
    inst->dir_newrem = CsrPmemAlloc(CsrStrLen(inst->dir_remote) + 1 +
                               CsrStrLen(dir) + 1);
    CsrStrCpy(inst->dir_newrem, inst->dir_remote);
    if(inst->dir_newrem[CsrStrLen(inst->dir_newrem)-1] != '/')
    {
        CsrStrCat(inst->dir_newrem, "/");
    }
    CsrStrCat(inst->dir_newrem, dir);

    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Changing remote directory...");
    CsrBtFtcSetFolderReqSend(CsrUtf82Ucs2ByteString((CsrUint8*)dir));
    /* Set new target */
    CsrPmemFree(inst->targ_name);
    inst->targ_name = StrInst("");
}

/* Clean browser info */
void browserEmpty(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    struct FtcInfo_t *next;

    next = inst->dir_head;
    while(next)
    {
        next = inst->dir_head->next;

        CsrPmemFree(inst->dir_head->name);
        CsrPmemFree(inst->dir_head);
        inst->dir_count--;

        inst->dir_head = next;
    }
    inst->dir_head = NULL;
    inst->dir_tail = NULL;
    inst->dir_count = 0;
    inst->dir_index = 0;
}

/* Insert browser info */
void browserAdd(CsrBtObexFtpClientAppGlobalInstData *inst, char *name, CsrUint32 size, CsrBool is_dir)
{
    struct FtcInfo_t *cur;

    /* Check that entry does not already exist */
    cur = inst->dir_head;
    while(cur != NULL)
    {
        if(CsrStrCmp(name, cur->name) == 0)
        {
            return;
        }
        cur = cur->next;
    }

    cur = CsrPmemAlloc(sizeof(struct FtcInfo_t));
    cur->name = StrInst(name);
    cur->size = size;
    cur->is_dir = is_dir;
    cur->next = NULL;

    if(inst->dir_head == NULL)
    {
        inst->dir_head = cur;
        inst->dir_tail = cur;
    }
    else
    {
        inst->dir_tail->next = cur;
        inst->dir_tail = cur;
    }
    inst->dir_count++;
}

/* Pop first browser element off */
struct FtcInfo_t *browserPeek(CsrBtObexFtpClientAppGlobalInstData *inst, CsrUint16 i)
{
    struct FtcInfo_t *cur;
    CsrUint16 this;

    if((inst->dir_count == 0) || (i > inst->dir_count))
    {
        return NULL;
    }

    cur = inst->dir_head;
    this = 0;
    while((cur != NULL) && (this < i))
    {
        cur = cur->next;
        this++;
    }

    return cur;
}

/* Display current browser info entries */
void playBrowser(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrUint16 i;
    CsrUint16 c;
    char *buf_filename;
    char buf_filesize[20];/* number of digits in file size */
    struct FtcInfo_t *cur;

    buf_filename = CsrPmemZalloc(MAX_MENU_ITEM_LENGTH);

    CsrUiMenuRemoveallitemsReqSend(inst->csrUiVar.hFileBrowser);

    if((inst->dir_head == NULL) || (inst->dir_index >= inst->dir_count))
    {
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "No more files to browse!");
        CsrPmemFree(buf_filename);
        return;
    }

    i = inst->dir_index;
    c = 0;
    while(((cur = browserPeek(inst, i)) != NULL))
    {
        CsrSnprintf(buf_filename, MAX_MENU_ITEM_LENGTH,"%s", cur->name);

        if(cur->is_dir)
        {
            CsrUiMenuAdditemReqSend(inst->csrUiVar.hFileBrowser, CSR_UI_FIRST, c, CSR_UI_ICON_FOLDER,
                CONVERT_TEXT_STRING_2_UCS2(buf_filename), NULL, inst->csrUiVar.hSk1Event, inst->csrUiVar.hSk2Event, inst->csrUiVar.hBackEvent, 0);
        }
        else
        {
            CsrSnprintf(buf_filesize, 20, "size %u", cur->size);
            CsrUiMenuAdditemReqSend(inst->csrUiVar.hFileBrowser, CSR_UI_LAST, c, CSR_UI_ICON_NONE,
                CONVERT_TEXT_STRING_2_UCS2(buf_filename), CONVERT_TEXT_STRING_2_UCS2(buf_filesize), inst->csrUiVar.hSk1Event, inst->csrUiVar.hSk2Event, inst->csrUiVar.hBackEvent, 0);

        }
        i++;
        c++;
    }
    CsrUiUieShowReqSend(inst->csrUiVar.hFileBrowser, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
    inst->csrUiVar.fileBrowserUi = TRUE;
    CsrPmemFree(buf_filename);
}

void updateMainFtcMenu(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrBtObexFtpClientAppUiInstData *csrUiVar = &inst->csrUiVar;

    if(csrUiVar->hMainMenu)
    {
        CsrUiMenuRemoveallitemsReqSend(csrUiVar->hMainMenu);
    }

    CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_BROWSE_LOCAL_KEY, CSR_UI_ICON_NONE,
        TEXT_FTP_CLIENT_APP_BROWSE_LOCAL_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);

    if(inst->btConnId == CSR_BT_CONN_ID_INVALID)
    {
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_FIRST, CSR_BT_FTP_CLIENT_APP_CONNECT_AUTH_KEY, CSR_UI_ICON_NONE,
            TEXT_FTP_CLIENT_APP_CONNECT_AUTH_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_FIRST, CSR_BT_FTP_CLIENT_APP_CONNECT_KEY, CSR_UI_ICON_NONE,
            TEXT_FTP_CLIENT_APP_CONNECT_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);

    }
    else
    {
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_FIRST, CSR_BT_FTP_CLIENT_APP_BROWSE_REMOTE_KEY, CSR_UI_ICON_NONE,
            TEXT_FTP_CLIENT_APP_BROWSE_REMOTE_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_DISCONNECT_KEY, CSR_UI_ICON_NONE,
            TEXT_FTP_CLIENT_APP_DISCONNECT_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
    }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (inst->srmSupported)
    {
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_DISABLE_SRM_KEY, CSR_UI_ICON_NONE,
            TEXT_FTP_CLIENT_APP_DISABLE_SRM_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
    }
    else
    {
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_ENABLE_SRM_KEY, CSR_UI_ICON_NONE,
            TEXT_FTP_CLIENT_APP_ENABLE_SRM_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
    }
#endif
#ifdef CSR_AMP_ENABLE
    if(inst->channelInUse == CSR_BT_AMP_CONTROLLER_BREDR)
    {
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_MOVE_TO_AMP_KEY, CSR_UI_ICON_NONE,
            TEXT_FTP_CLIENT_APP_MOVE_TO_AMP_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
    }
    else
    {
        CsrUiMenuAdditemReqSend(csrUiVar->hMainMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_MOVE_TO_EDR_KEY, CSR_UI_ICON_NONE,
            TEXT_FTP_CLIENT_APP_MOVE_TO_EDR_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
    }
#endif
}

void updateSelectedOptionsMenu(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrBtObexFtpClientAppUiInstData *csrUiVar = &inst->csrUiVar;

    if(csrUiVar->hSelectedOptionsMenu)
    {
        CsrUiMenuRemoveallitemsReqSend(csrUiVar->hSelectedOptionsMenu);
    }
    /*
    CsrUiMenuAdditemReqSend(csrUiVar->hSelectedOptionsMenu, CSR_UI_FIRST, CSR_BT_FTP_CLIENT_APP_GO_TO_TOP_KEY, CSR_UI_ICON_NONE,
        TEXT_FTP_CLIENT_APP_GO_TO_TOP_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
    CsrUiMenuAdditemReqSend(csrUiVar->hSelectedOptionsMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_GO_ONE_UP_KEY, CSR_UI_ICON_NONE,
        TEXT_FTP_CLIENT_APP_GO_ONE_UP_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
    */

    if(CsrStrLen(inst->targ_name) > 0)
    {
        if(inst->targ_dir)
        {
            CsrUiMenuAdditemReqSend(csrUiVar->hSelectedOptionsMenu, CSR_UI_FIRST, CSR_BT_FTP_CLIENT_APP_CHANGE_TO_SELECTED_DIR_KEY, CSR_UI_ICON_NONE,
                TEXT_FTP_CLIENT_APP_CHANGE_TO_SELECTED_DIR_UCS2, CONVERT_TEXT_STRING_2_UCS2(inst->targ_name), csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
        }
        else
        {
            if(inst->btConnId != CSR_BT_CONN_ID_INVALID)
            {
                CsrUiMenuAdditemReqSend(csrUiVar->hSelectedOptionsMenu, CSR_UI_FIRST, CSR_BT_FTP_CLIENT_APP_TRANSFER_SELECTED_FILE_KEY, CSR_UI_ICON_NONE,
                                        TEXT_FTP_CLIENT_APP_TRANSFER_SELECTED_FILE_UCS2, CONVERT_TEXT_STRING_2_UCS2(inst->targ_name), csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
            }
        }
        
        CsrUiMenuAdditemReqSend(csrUiVar->hSelectedOptionsMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_MOVE_KEY, CSR_UI_ICON_NONE,
                            TEXT_FTP_CLIENT_APP_MOVE_UCS2, CONVERT_TEXT_STRING_2_UCS2(inst->targ_name), csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
    
        CsrUiMenuAdditemReqSend(csrUiVar->hSelectedOptionsMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_COPY_KEY, CSR_UI_ICON_NONE,
                            TEXT_FTP_CLIENT_APP_COPY_UCS2, CONVERT_TEXT_STRING_2_UCS2(inst->targ_name), csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);

        CsrUiMenuAdditemReqSend(csrUiVar->hSelectedOptionsMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_SET_PERM_KEY, CSR_UI_ICON_NONE,
                                TEXT_FTP_CLIENT_APP_SET_PERM_UCS2, CONVERT_TEXT_STRING_2_UCS2(inst->targ_name), csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
        
        CsrUiMenuAdditemReqSend(csrUiVar->hSelectedOptionsMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_DELETE_KEY, CSR_UI_ICON_NONE,
                                TEXT_FTP_CLIENT_APP_DELETE_SELECTED_TARGET_UCS2, CONVERT_TEXT_STRING_2_UCS2(inst->targ_name), csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);
    }
    
    CsrUiMenuAdditemReqSend(csrUiVar->hSelectedOptionsMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_MAKE_DIR_KEY, CSR_UI_ICON_NONE,
                            TEXT_FTP_CLIENT_APP_MAKE_DIR_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);

    CsrUiMenuAdditemReqSend(csrUiVar->hSelectedOptionsMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_PTS_NONEXISTING_SUB1_KEY, CSR_UI_ICON_NONE,
                            TEXT_FTP_CLIENT_APP_PTS_NONEXISTING_SUB1_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);

    CsrUiMenuAdditemReqSend(csrUiVar->hSelectedOptionsMenu, CSR_UI_LAST, CSR_BT_FTP_CLIENT_APP_PTS_NONEXISTING_SUB2_KEY, CSR_UI_ICON_NONE,
                            TEXT_FTP_CLIENT_APP_PTS_NONEXISTING_SUB2_UCS2, NULL, csrUiVar->hSk1Event, csrUiVar->hSk2Event, csrUiVar->hBackEvent, 0);

}

void setPasswordInputdialog(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    CsrBtObexFtpClientAppUiInstData *csrUiVar = &inst->csrUiVar;
    if(inst->auth_state == AUTH_USERID)
    {
        CsrUiInputdialogSetReqSend(csrUiVar->hObexPasskeyInputDialog, TEXT_OBEX_AUTH_UCS2, TEXT_OBEX_USERID_UCS2, CSR_UI_ICON_KEY,
            NULL, CSR_BT_OBEX_MAX_AUTH_USERID_LENGTH, CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL,
            csrUiVar->hSk1Event, CSR_UI_DEFAULTACTION, csrUiVar->hBackEvent, CSR_UI_DEFAULTACTION);
    }
    else /* inst->auth_state == AUTH_PASS */
    {
        CsrUiInputdialogSetReqSend(csrUiVar->hObexPasskeyInputDialog, TEXT_OBEX_AUTH_UCS2, TEXT_OBEX_PASSKEY_UCS2, CSR_UI_ICON_KEY,
            NULL, CSR_BT_PASSKEY_MAX_LEN, CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL,
            csrUiVar->hSk1Event, CSR_UI_DEFAULTACTION, csrUiVar->hBackEvent, CSR_UI_DEFAULTACTION);
    }
}

void commonDialogSet(CsrBtObexFtpClientAppGlobalInstData *inst,  CsrUint16 *dialogHeading,  CsrUint16 *dialogText, CsrUint16 *textSK1, CsrUint16 *textSK2, CsrUieHandle eventSK1, CsrUieHandle eventSK2)
{
    CsrUiDialogSetReqSend(inst->csrUiVar.hCommonDialog, dialogHeading, dialogText, textSK1, textSK2,
        eventSK1, eventSK2, inst->csrUiVar.hBackEvent, 0);
    CsrUiUieShowReqSend(inst->csrUiVar.hCommonDialog, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
    inst->csrUiVar.commonDialogUi = TRUE;
}

/* handler functions */
static void handleCsrUiUieCreateCfm(CsrBtObexFtpClientAppGlobalInstData *inst, CsrUiUieCreateCfm *confirm)
{
    CsrBtObexFtpClientAppUiInstData *csrUiVar = &inst->csrUiVar;

    if (confirm->elementType == CSR_UI_UIETYPE_EVENT)
    {
        if (!csrUiVar->hBackEvent)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, inst->ftcSchedQid);
            csrUiVar->hBackEvent = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_EVENT);
        }
        else if (!csrUiVar->hSk1Event)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, inst->ftcSchedQid);
            csrUiVar->hSk1Event = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_EVENT);
        }
        else if (!csrUiVar->hSk1AbortEvent)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, inst->ftcSchedQid);
            csrUiVar->hSk1AbortEvent = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_EVENT);
        }
        else if (!csrUiVar->hSk1CancelEvent)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, inst->ftcSchedQid);
            csrUiVar->hSk1CancelEvent = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_EVENT);
        }
        else if (!csrUiVar->hSk2AmpEvent)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, inst->ftcSchedQid);
            csrUiVar->hSk2AmpEvent = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_EVENT);
        }
        else if (!csrUiVar->hSk2BrEdrEvent)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, inst->ftcSchedQid);
            csrUiVar->hSk2BrEdrEvent = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_EVENT);
        }
        else if (!csrUiVar->hSk2Event)
        {
            CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_BLOCK, inst->ftcSchedQid);
            csrUiVar->hSk2Event = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_INPUTDIALOG);
        }
    }
    if (confirm->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        if (!csrUiVar->hObexPasskeyInputDialog)
        {
            CsrUiInputdialogSetReqSend(confirm->handle, TEXT_OBEX_AUTH_UCS2, TEXT_OBEX_PASSKEY_UCS2, CSR_UI_ICON_KEY,
                NULL, CSR_BT_PASSKEY_MAX_LEN, CSR_UI_KEYMAP_ALPHANUMERIC, TEXT_OK_UCS2, NULL,
                csrUiVar->hSk1Event, CSR_UI_DEFAULTACTION, csrUiVar->hBackEvent, CSR_UI_DEFAULTACTION);
            csrUiVar->hObexPasskeyInputDialog = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_INPUTDIALOG);
        }
        else if (!csrUiVar->hDirectoryInputDialog)
        {
            csrUiVar->hDirectoryInputDialog = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_INPUTDIALOG);
        }
        else if (!csrUiVar->hMoveInputDialog)
        {
            csrUiVar->hMoveInputDialog = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_INPUTDIALOG);
        }
        else if (!csrUiVar->hSetPermInputDialog)
        {
            csrUiVar->hSetPermInputDialog = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_INPUTDIALOG);
        }
        else if (!csrUiVar->hCopyInputDialog)
        {
            csrUiVar->hCopyInputDialog = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_DIALOG);
        }

    }
    if (confirm->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        if (!csrUiVar->hCommonDialog)
        {
            csrUiVar->hCommonDialog = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_MENU);
        }
    }
    if (confirm->elementType == CSR_UI_UIETYPE_MENU)
    {
        if (!csrUiVar->hFileBrowser)
        {
            CsrUiMenuSetReqSend(confirm->handle, TEXT_FTP_CLIENT_APP_FILEBROWSER_UCS2, TEXT_OPTIONS_UCS2, NULL);
            csrUiVar->hFileBrowser = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_MENU);
        }
        else if (!csrUiVar->hSelectedOptionsMenu)
        {
            CsrUiMenuSetReqSend(confirm->handle, TEXT_OPTIONS_UCS2, TEXT_SELECT_UCS2, NULL);
            csrUiVar->hSelectedOptionsMenu = confirm->handle;
            CsrUiUieCreateReqSend(inst->ftcSchedQid, CSR_UI_UIETYPE_MENU);
        }
        else if (!csrUiVar->hMainMenu)
        {
            CsrUiMenuSetReqSend(confirm->handle, TEXT_FTP_CLIENT_APP_MAIN_MENU_UCS2, TEXT_SELECT_UCS2, NULL);
            csrUiVar->hMainMenu = confirm->handle;
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "FTP Client UI elements created.");
        }

    }
}

static void handleCsrUiInputDialogGetCfmObexPassKeyInputDialog(CsrBtObexFtpClientAppGlobalInstData *inst, CsrUiInputdialogGetCfm *confirm,
                                                               CsrBtObexFtpClientAppUiInstData *csrUiVar)
{
    CsrUint8 *passkey = CsrUtf16String2Utf8(confirm->text); /* how about freeing text ??? */
    CsrUint8 ftcPasswordLength = (CsrUint8)CsrStrLen((CsrCharString *) passkey);
    
    if(inst->auth_state == AUTH_USERID)
    {
        if(ftcPasswordLength)
        {
            inst->auth_user = CsrStrDup((CsrCharString *) passkey);
        }
        /* Show password inputdialog */
        inst->auth_state = AUTH_PASS;
        setPasswordInputdialog(inst);
    }
    else
    {
        if(ftcPasswordLength)
        {
            if(inst->auth_local)
            {
                CsrBtFtcConnectReqSend( inst->ftcSchedQid,
                                        MAX_PACKET_SIZE,
                                        (inst->address),
                                        TRUE, /* authorise */
                                        0, /* realm len */
                                        NULL, /* realm */
                                        ftcPasswordLength, /* pwd len */
                                        (CsrUint8 *) CsrStrDup((CsrCharString *) passkey), /* pwd */
                                        (CsrCharString *) CsrStrDup(inst->auth_user), /* user id */
                                        0, /* length */
                                        0, /* count */
                                        0, /* windowsSize */
                                        inst->srmSupported);
            }
            else
            {
                CsrBtFtcAuthenticateResSend((CsrUint8 *) CsrStrDup((CsrCharString *) passkey),
                                            ftcPasswordLength,
                                            (CsrCharString *) CsrStrDup(inst->auth_user));
            }
        }
        else
        {
            if(inst->auth_local)
            {
                CsrBtFtcConnectReqSend( inst->ftcSchedQid,
                                        MAX_PACKET_SIZE,
                                        (inst->address),
                                        TRUE, /* authorise */
                                        0, /* realm len */
                                        NULL, /* realm */
                                        0, /* pwd len */
                                        NULL, /* pwd */
                                        NULL, /* user id */
                                        0, /* length */
                                        0, /* count */
                                        0, /* windowsSize */
                                        inst->srmSupported);
            }
            else
            {
                CsrBtFtcAuthenticateResSend(NULL,
                                            0,
                                            (CsrCharString *) CsrStrDup(inst->auth_user));
            }
        }
        
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Obex passkey reponse");
        /* Hide passkey input dialog */
        if(inst->csrUiVar.obexPasskeyUi)
        {
            CsrUiUieHideReqSend(csrUiVar->hObexPasskeyInputDialog);
            inst->csrUiVar.obexPasskeyUi = FALSE;
        }
    }
    CsrPmemFree(passkey);
}

static void handleCsrUiInputDialogGetCfmDirectoryInputDialog(CsrBtObexFtpClientAppGlobalInstData *inst, CsrUiInputdialogGetCfm *confirm,
                                                             CsrBtObexFtpClientAppUiInstData *csrUiVar)
{
    CsrUint8 *directoryName = CsrUtf16String2Utf8(confirm->text); /* how about freeing text ??? */
    CsrUint8 directoryLength = (CsrUint8)CsrStrLen((CsrCharString *) directoryName);
    
    if(directoryLength)/*is it possible to get (directoryLength < MAX_DIRLEN) when inputDIalog has max = MAX_DIRLEN*/
    {
        inst->mkdir_name  = CsrStrDup((CsrCharString *) directoryName);
        
        /* Process local makedir */
        if(!inst->mkdir_remote)
        {
            int res;
            
            res = CsrBtMkDir(inst->mkdir_name);
            
            if(res == 0)
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Successfully created local directory.");
            }
            else
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "Error creating local directory, error code %d", res);
            }
        }
        /* Remote makedir */
        else
        {
            CsrUint8 *unicode;
            
            /* Track remote directory */
            inst->dir_newrem = CsrPmemAlloc(CsrStrLen(inst->dir_remote) + 1 +
                                            CsrStrLen(inst->mkdir_name) + 1);
            CsrStrCpy(inst->dir_newrem, inst->dir_remote);
            if(inst->dir_newrem[CsrStrLen(inst->dir_newrem)-1] != '/')
            {
                CsrStrCat(inst->dir_newrem, "/");
            }
            CsrStrCat(inst->dir_newrem, inst->mkdir_name);
            
            /* Send request */
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Creating remote directory '%s'...", inst->mkdir_name);
            unicode = CsrUtf82Ucs2ByteString((CsrUint8*)inst->mkdir_name);
            CsrBtFtcSetAddFolderReqSend(unicode);
        }
        /* Free makedir things */
        CsrPmemFree(inst->mkdir_name);
        inst->mkdir_name = NULL;
    }
    else
    {
        /* Nothing entered, cancel */
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Make directory cancelled\n");
    }
    CsrPmemFree(directoryName);
}

static void handleCsrUiInputDialogGetCfmCopyInputDialog(CsrBtObexFtpClientAppGlobalInstData *inst, CsrUiInputdialogGetCfm *confirm,
                                                        CsrBtObexFtpClientAppUiInstData *csrUiVar)
{
    CsrUint8 *copyName = CsrUtf16String2Utf8(confirm->text); 
    CsrUint8 copyLength = (CsrUint8)CsrStrLen((CsrCharString *) copyName);
    
    if(copyLength)
    {
        inst->copy_name  = CsrStrDup((CsrCharString *) copyName);
        
        /* Process local copy */
        if(!inst->targ_remote)
        {
            int res;
            
            res = CsrBtCopy(inst->targ_name,inst->copy_name);
            
            if(res == 0)
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Successfully copied %s -> %c (local)",
                                     inst->targ_name,inst->copy_name);
            }
            else
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Error copying %s -> %c (local)",
                                     inst->targ_name,inst->copy_name);
            }
        }
        /* Remote copy */
        else
        {
            CsrUint8 *unicode_src;
            CsrUint8 *unicode_dest;
            
            /* Send request */
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Copying: %s -> %s ...", 
                                 inst->targ_name,inst->copy_name);
            
            unicode_src = CsrUtf82Ucs2ByteString((CsrUint8*)inst->targ_name);
            unicode_dest = CsrUtf82Ucs2ByteString((CsrUint8*)inst->copy_name);
            
            CsrBtFtcCopyingObjReqSend(unicode_src,unicode_dest);
        }
        
        /* Free copy things */
        CsrPmemFree(inst->copy_name);
        inst->copy_name = NULL;
    }
    else
    {
        /* Nothing entered, cancel */
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Copy cancelled\n");
    }
    CsrPmemFree(copyName);
}

static void handleCsrUiInputDialogGetCfmMoveInputDialog(CsrBtObexFtpClientAppGlobalInstData *inst, CsrUiInputdialogGetCfm *confirm,
                                                        CsrBtObexFtpClientAppUiInstData *csrUiVar)
{
    CsrUint8 *moveName = CsrUtf16String2Utf8(confirm->text); 
    CsrUint8 moveLength = (CsrUint8)CsrStrLen((CsrCharString *) moveName);
    
    if(moveLength)
    {
        inst->move_name  = CsrStrDup((CsrCharString *) moveName);
        
        /* local move */
        if(!inst->targ_remote)
        {
            CsrResult renameRes;
            
            renameRes = CsrFileRename((CsrUtf8String *)inst->targ_name, (CsrUtf8String *)inst->move_name);

            if(renameRes == 0)
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Successfully moved %s -> %s (local)",
                                     inst->targ_name,inst->move_name);
            }
            else
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Error moving %s -> %s (local)",
                                     inst->targ_name,inst->move_name);
            }
        }
        /* Remote move */
        else
        {
            CsrUint8 *unicode_src;
            CsrUint8 *unicode_dest;
            
            /* Send request */
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "moving from %s -> %s ...", 
                                 inst->targ_name,inst->move_name);
            
            unicode_src = CsrUtf82Ucs2ByteString((CsrUint8*)inst->targ_name);
            unicode_dest = CsrUtf82Ucs2ByteString((CsrUint8*)inst->move_name);
            
            CsrBtFtcMovingObjReqSend(unicode_src,unicode_dest);
        }
        
        /* Free copy things */
        CsrPmemFree(inst->move_name);
        inst->move_name = NULL;
    }
}

static CsrUint32 parsePermissionString(CsrUint8 *perm, CsrUint8 len)
{
    CsrUint32 permission;
    CsrInt8 shift_factor;
    CsrUint8 i;

    shift_factor = 16;
    permission = 0;
    
    for(i=0;((i<len) && (shift_factor>=0));i++)
    {
        switch (perm[i])
        {
            case 'r':
            case 'R':
                permission |= (0x00000001 << shift_factor);
                break;
                
            case 'w':
            case 'W':
                permission |= (0x00000002 << shift_factor);
                break;
                
            case 'd':
            case 'D':
                permission |= (0x00000004 << shift_factor);
                break;
                
            case 'm':
            case 'M':
                permission |= (0x00000080 << shift_factor);
                break;
                

            case ',':
            case '.':
            case '-':
            case '|':
                shift_factor -= 8;
                break;

            default:
                break;
        }
    }

    return(permission);
}

static void handleCsrUiInputDialogGetCfmSetPermInputDialog(CsrBtObexFtpClientAppGlobalInstData *inst, CsrUiInputdialogGetCfm *confirm,
                                                           CsrBtObexFtpClientAppUiInstData *csrUiVar)
{
    CsrUint8 *setPerm = CsrUtf16String2Utf8(confirm->text); 
    CsrUint8 setPermLength = (CsrUint8)CsrStrLen((CsrCharString *) setPerm);
    CsrUint32 obexPermission = 0;
    CsrFilePerms filePermission = 0;
    CsrBool mapResult = FALSE;

    if (setPermLength)
    {
        /* Parse permision */
        obexPermission  = parsePermissionString(setPerm,setPermLength);

        /* set perm local */
        if(!inst->targ_remote)
        {
            CsrResult setResult;

            filePermission = mapObexPermToFilePerm(obexPermission,&mapResult);

            if (mapResult == TRUE)
            {
                setResult = CsrFileSetPerms((CsrUtf8String *)inst->targ_name,filePermission);

                if(setResult == 0)
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "SetPerm 0x%x on %s (local)",filePermission,inst->targ_name);
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "SetPerm failed: res = 0x%x (local)",                                 
                                         setResult);
                }
            }
            else
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "Use of permission D or M is not supported");                               
            }
            
        }
        /* Remote */
        else
        {
            CsrUint8 *unicode;
            
            /* Send request */
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "SetPerm 0x%x on %s (remote)",obexPermission,inst->targ_name);
            unicode = CsrUtf82Ucs2ByteString((CsrUint8*)inst->targ_name);
            
            CsrBtFtcSetObjPermissionsReqSend(unicode,obexPermission);
        }
    }
    else
    {
        /* Free setPerm things */
        CsrPmemFree(inst->setPerm_name);
        inst->setPerm_name = NULL;
    }
}

static void handleCsrUiInputDialogGetCfm(CsrBtObexFtpClientAppGlobalInstData *inst, CsrUiInputdialogGetCfm *confirm)
{
    CsrBtObexFtpClientAppUiInstData *csrUiVar = &inst->csrUiVar;
    
    /* Create directory input handling */
    if(confirm->handle == csrUiVar->hObexPasskeyInputDialog)
    {
        handleCsrUiInputDialogGetCfmObexPassKeyInputDialog(inst,confirm,csrUiVar);
    }
    else if (confirm->handle == csrUiVar->hDirectoryInputDialog)
    {
        handleCsrUiInputDialogGetCfmDirectoryInputDialog(inst,confirm,csrUiVar);
    }
    else if (confirm->handle == csrUiVar->hCopyInputDialog)
    {
        handleCsrUiInputDialogGetCfmCopyInputDialog(inst,confirm,csrUiVar);
    }
    else if (confirm->handle == csrUiVar->hMoveInputDialog)
    {
        handleCsrUiInputDialogGetCfmMoveInputDialog(inst,confirm,csrUiVar);
    }
    else if (confirm->handle == csrUiVar->hSetPermInputDialog)
    {
        handleCsrUiInputDialogGetCfmSetPermInputDialog(inst,confirm,csrUiVar);
    }
    else
    {
        /* Do nothing */
    }
}

static void handleCsrUiMenuItemGetCfm(CsrBtObexFtpClientAppGlobalInstData *inst, 
                                      CsrUiMenuGetitemCfm *confirm)
{
    CsrBtObexFtpClientAppUiInstData *csrUiVar = &inst->csrUiVar;

    /* Create directory input handling */
    if(confirm->handle == csrUiVar->hFileBrowser)
    {
        CsrUint8 *name = CsrUtf16String2Utf8(confirm->label); /* need to free label + sublabel ??? */
        CsrBool dir = (confirm->icon == CSR_UI_ICON_FOLDER) ? TRUE : FALSE;

        inst->targ_dir = dir;
        inst->targ_name = (char *) name;

        /* playMenu -> new UI */
        updateSelectedOptionsMenu(inst);
        CsrUiUieShowReqSend(inst->csrUiVar.hSelectedOptionsMenu, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
        inst->csrUiVar.selectedOptionsMenuUi = TRUE;
    }
}

static void handleCsrUiUieEventInd(CsrBtObexFtpClientAppGlobalInstData *inst, CsrUiEventInd *indication)
{
    CsrBtObexFtpClientAppUiInstData *csrUiVar = &inst->csrUiVar;

    CsrBool hide = FALSE;

    if (indication->event == csrUiVar->hBackEvent)
    {
        hide = TRUE;
    }
    else if( (indication->displayElement == csrUiVar->hDirectoryInputDialog) && (indication->event == csrUiVar->hSk1Event) )
    {
        CsrUiInputdialogGetReqSend(inst->ftcSchedQid, csrUiVar->hDirectoryInputDialog);
        hide = TRUE;
    }
    else if( (indication->displayElement == csrUiVar->hCopyInputDialog) && (indication->event == csrUiVar->hSk1Event) )
    {
        CsrUiInputdialogGetReqSend(inst->ftcSchedQid, csrUiVar->hCopyInputDialog);
        hide = TRUE;
    }
    else if( (indication->displayElement == csrUiVar->hMoveInputDialog) && (indication->event == csrUiVar->hSk1Event) )
    {
        CsrUiInputdialogGetReqSend(inst->ftcSchedQid, csrUiVar->hMoveInputDialog);
        hide = TRUE;
    }
    else if( (indication->displayElement == csrUiVar->hSetPermInputDialog) && (indication->event == csrUiVar->hSk1Event) )
    {
        CsrUiInputdialogGetReqSend(inst->ftcSchedQid, csrUiVar->hSetPermInputDialog);
        hide = TRUE;
    }
    else if( (indication->displayElement == csrUiVar->hObexPasskeyInputDialog) && (indication->event == csrUiVar->hSk1Event) )
    {
        CsrUiInputdialogGetReqSend(inst->ftcSchedQid, csrUiVar->hObexPasskeyInputDialog);
        hide = TRUE;
    }
    else if( indication->displayElement == csrUiVar->hCommonDialog  )
    {
        hide = TRUE;
        if(indication->event == csrUiVar->hSk1AbortEvent)
        {
            CsrBtFtcAbortReqSend();
        }
        else if(indication->event == csrUiVar->hSk1CancelEvent)
        {
            CsrBtFtcCancelConnectReqSend();
        }
#ifdef CSR_AMP_ENABLE
        else if(indication->event == csrUiVar->hSk2AmpEvent)
        {
            startMoveChannel(inst, MOVE_TO_AMP);
        }
        else if(indication->event == csrUiVar->hSk2BrEdrEvent)
        {
            startMoveChannel(inst, MOVE_TO_BREDR);
        }
#endif

    }
    else if( (indication->displayElement == csrUiVar->hMainMenu) && (indication->event == csrUiVar->hSk1Event) )
    {
        /* Main Menu handling */
        switch(indication->key)
        {
            case CSR_BT_FTP_CLIENT_APP_CONNECT_KEY:
            {
                if((inst->address.lap == 0) && (inst->address.lap == 0) && (inst->address.lap == 0))
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "No device selected for connection");
                }
                else
                {
                    commonDialogSet(inst, TEXT_FTP_CLIENT_APP_MAIN_MENU_UCS2, TEXT_FTP_CLIENT_APP_CONNECTING_UCS2,
                        TEXT_CANCEL_UCS2, NULL, inst->csrUiVar.hSk1CancelEvent, CSR_UI_DEFAULTACTION);

                    CsrBtFtcConnectReqSend( inst->ftcSchedQid,
                                            MAX_PACKET_SIZE,
                                            (inst->address),
                                            FALSE, /* authorise */
                                            0, /* realm len */
                                            NULL, /* realm */
                                            0, /* pwd len */
                                            NULL, /* pwd */
                                            NULL, /* user id */
                                            0, /* length */
                                            0, /* count */
                                            0, /* windowsSize */
                                            inst->srmSupported);
                }
                break;
            }
            case CSR_BT_FTP_CLIENT_APP_CONNECT_AUTH_KEY:
            {
                if((inst->address.lap == 0) && (inst->address.lap == 0) && (inst->address.lap == 0))
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, TRUE, "No device selected for connection");
                }
                else
                {
                    inst->auth_local = TRUE;
                    inst->auth_state = AUTH_PASS;

                    setPasswordInputdialog(inst);
                    CsrUiUieShowReqSend(inst->csrUiVar.hObexPasskeyInputDialog, inst->ftcSchedQid, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
                    inst->csrUiVar.obexPasskeyUi = TRUE;
                }
                break;
            }
            case CSR_BT_FTP_CLIENT_APP_DISCONNECT_KEY:
            {
                CsrBtFtcDisconnectReqSend(TRUE);
                break;
            }
            case CSR_BT_FTP_CLIENT_APP_DISABLE_SRM_KEY:
            {
                inst->srmSupported = FALSE;
                updateMainFtcMenu(inst);
                break;
            }
            case CSR_BT_FTP_CLIENT_APP_ENABLE_SRM_KEY:
            {
                inst->srmSupported = TRUE;
                updateMainFtcMenu(inst);
                break;
            }
            case CSR_BT_FTP_CLIENT_APP_BROWSE_LOCAL_KEY:
                selectLocal(inst);
                break;

            case CSR_BT_FTP_CLIENT_APP_BROWSE_REMOTE_KEY:
                selectRemote(inst);
                break;
#ifdef CSR_AMP_ENABLE
            case CSR_BT_FTP_CLIENT_APP_MOVE_TO_AMP_KEY:
            {
                startMoveChannel(inst, MOVE_TO_AMP);
                break;
            }
            case CSR_BT_FTP_CLIENT_APP_MOVE_TO_EDR_KEY:
            {
                startMoveChannel(inst, MOVE_TO_BREDR);
                break;
            }
#endif
        }
    }
    else if( (indication->displayElement == csrUiVar->hFileBrowser) && (indication->event == csrUiVar->hSk1Event) )
    {
        CsrUiMenuGetitemReqSend(inst->ftcSchedQid, csrUiVar->hFileBrowser, indication->key);
        hide = TRUE;
    }
    else if( (indication->displayElement == csrUiVar->hSelectedOptionsMenu) && (indication->event == csrUiVar->hSk1Event) )
    {
        /* Connected Menu handling */
        switch(indication->key)
        {
            case CSR_BT_FTP_CLIENT_APP_MAKE_DIR_KEY:
                handleMakedir(inst);
                break;
                
            case CSR_BT_FTP_CLIENT_APP_SET_PERM_KEY:
                handleSetPerm(inst);
                break;
                
            case CSR_BT_FTP_CLIENT_APP_MOVE_KEY:
                handleMove(inst);
                break;
                
            case CSR_BT_FTP_CLIENT_APP_COPY_KEY:
                handleCopy(inst);
                break;
                
            case CSR_BT_FTP_CLIENT_APP_DELETE_KEY:
                {
                    if(CsrStrLen(inst->targ_name) > 0)
                    {
                        selectDelete(inst);
                    }
                    break;
                }
                
            case CSR_BT_FTP_CLIENT_APP_CHANGE_TO_SELECTED_DIR_KEY:
                {
                    if(CsrStrLen(inst->targ_name) > 0)
                    {
                        if(inst->targ_dir)
                        {
                            selectDirectory(inst, inst->targ_name, inst->targ_remote);
                        }
                    }
                    break;
                }
            
            case CSR_BT_FTP_CLIENT_APP_TRANSFER_SELECTED_FILE_KEY:
                {
                    if(CsrStrLen(inst->targ_name) > 0)
                    {
                        transferStart(inst);
                    }
                    break;
                }
                
            case CSR_BT_FTP_CLIENT_APP_PTS_NONEXISTING_SUB1_KEY:
                CsrBtFtcSetFolderReqSend(CsrUtf82Ucs2ByteString((CsrUint8*)"sub1"));
                break;

            case CSR_BT_FTP_CLIENT_APP_PTS_NONEXISTING_SUB2_KEY:
                CsrBtFtcSetFolderReqSend(CsrUtf82Ucs2ByteString((CsrUint8*)"sub2"));
                break;
        }
        hide = TRUE;
    }

    if(hide)
    {
        if(indication->displayElement == csrUiVar->hMainMenu)
        {
            csrUiVar->mainMenuUi = FALSE;
        }
        else if(indication->displayElement == csrUiVar->hFileBrowser)
        {
            csrUiVar->fileBrowserUi = FALSE;
        }
        else if(indication->displayElement == csrUiVar->hObexPasskeyInputDialog)
        {
            csrUiVar->obexPasskeyUi = FALSE;
        }
        else if(indication->displayElement == csrUiVar->hDirectoryInputDialog)
        {
            csrUiVar->directoryInputDialogUi = FALSE;
        }
        else if(indication->displayElement == csrUiVar->hCopyInputDialog)
        {
            csrUiVar->copyInputDialogUi = FALSE;
        }
        else if(indication->displayElement == csrUiVar->hMoveInputDialog)
        {
            csrUiVar->moveInputDialogUi = FALSE;
        }
        else if(indication->displayElement == csrUiVar->hSetPermInputDialog)
        {
            csrUiVar->setPermInputDialogUi = FALSE;
        }
        else if(indication->displayElement == csrUiVar->hAllowMenu)
        {
            csrUiVar->allowMenuUi = FALSE;
        }
        else if(indication->displayElement == csrUiVar->hCommonDialog)
        {
            csrUiVar->commonDialogUi = FALSE;
        }
        else if(indication->displayElement == csrUiVar->hSelectedOptionsMenu)
        {
            csrUiVar->selectedOptionsMenuUi = FALSE;
        }

        CsrUiUieHideReqSend(indication->displayElement);
    }
}

void handleFtcCsrUiPrim(CsrBtObexFtpClientAppGlobalInstData *inst)
{
    switch(*((CsrUint16 *) inst->msg))
    {
        case CSR_UI_UIE_CREATE_CFM:
        {
            handleCsrUiUieCreateCfm(inst, inst->msg);
            break;
        }
        case CSR_UI_INPUTDIALOG_GET_CFM:
        {
            handleCsrUiInputDialogGetCfm(inst, inst->msg);
            break;
        }
        case CSR_UI_MENU_GETITEM_CFM:
        {
            handleCsrUiMenuItemGetCfm(inst, inst->msg);
            break;
        }
        case CSR_UI_KEYDOWN_IND:
        {
            CsrUiKeydownInd *indication;
            indication = (CsrUiKeydownInd *) inst->msg;
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "UIKeydownInd DE: 0x%04x, key: 0x%04x", indication->displayElement, indication->key);
            break;
        }
        case CSR_UI_EVENT_IND:
        {
            handleCsrUiUieEventInd(inst, inst->msg);
            break;
        }
        default:
        {
            /* unexpected primitive received */
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_CLIENT, FALSE, "####### Unhandled CSR_UI_PRIM (0x%04x) in handleCsrUiPrim",*((CsrUint16 *) inst->msg));
            break;
        }
    }
}
