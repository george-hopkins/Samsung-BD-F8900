/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include <time.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_unicode.h"
#include "csr_util.h"
#include "csr_file.h"
#include "csr_exceptionhandler.h"
#include "csr_formatted_io.h"

#include "csr_app_lib.h"
#include "csr_ui_lib.h"

#include "csr_bt_platform.h"
#include "csr_bt_ui_strings.h"
#include "csr_bt_util.h"
#include "csr_bt_file.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_fts_prim.h"
#include "csr_bt_fts_lib.h"

#include "csr_bt_obex_ftp_strings.h"
#include "csr_bt_obex_ftp_common.h"
#include "csr_bt_obex_ftp_server_ui_sef.h"
#include "csr_bt_obex_ftp_server_sef.h"

#define XML_HEADER1    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
#define XML_HEADER2    "<!DOCTYPE folder-listing SYSTEM \"obex-folder-listing.dtd\">\n"
#define XML_HEADER3    "<folder-listing version=\"1.0\">\n"
#define XML_FILE       "\t<file name=\"%s\" modified=\"%04d%02d%02dT%02d%02d%02dZ\" size=\"%ld\" user-perm=\"%s\"/>\n"
#define XML_FOLDER     "\t<folder name=\"%s\" modified=\"%04d%02d%02dT%02d%02d%02dZ\"/>\n"

typedef struct FolderInfoTag
{
    char                  *name;
    unsigned short        mode;
    CsrUint32              size;
    CsrTime                  time;
    struct FolderInfoTag  *nextInfoElement;
} FolderInfo;



void ftpServerTransferingObject(CsrBtObexFtpServerAppGlobalInstData *ftpServerInst, CsrBool finalbitSet)
{
    if (ftpServerInst->obexResult != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE &&
        ftpServerInst->obexResult != CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        ftpServerInst->transferObject = FALSE;
    }
    else if (ftpServerInst->obexResult == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && finalbitSet)
    {
        ftpServerInst->transferObject = FALSE;
    }
    else
    {
        ftpServerInst->transferObject = TRUE;
    }
}

/* Free entire folder application list */
void freeFolderInformation(FolderInfo ** folderInformation)
{
    FolderInfo *tmp;

    if(*folderInformation != NULL)
    {
        for(tmp = *folderInformation; tmp != NULL; tmp = *folderInformation)
        {
            CsrPmemFree(tmp->name);
            *folderInformation = (*folderInformation)->nextInfoElement;
            CsrPmemFree(tmp);
        }
    }
}

/* Read folder information */
int getFolderInformation(char *filePattern, FolderInfo **folderInformation)
{
    DIR *dir;

    if(*folderInformation != NULL)
    {
        freeFolderInformation(folderInformation);
        *folderInformation = NULL;
    }

    if((dir = CsrBtOpenDir(filePattern)) != NULL)
    {
        struct dirent *entry;
        FolderInfo *cur;
        FolderInfo *prev;

        prev = NULL;
        while((entry = CsrBtReadDir(dir)) != NULL)
        {
            CsrBtStat_t info;
            int res;

            CsrMemSet(&info, 0, sizeof(CsrBtStat_t));

            res = CsrBtStat(entry->d_name, &info);
            if((res == 0) && (CsrStrLen(entry->d_name) > 0))
            {
                cur = CsrPmemAlloc(sizeof(FolderInfo));

                cur->nextInfoElement = NULL;
                cur->name = CsrPmemAlloc(CsrStrLen(entry->d_name) + 1);
                strcpy(cur->name, entry->d_name);
                cur->time = info.st_ctime;
                cur->size = info.st_size;
                cur->mode = info.st_mode;

                if(*folderInformation == NULL)
                {
                    *folderInformation = cur;
                    prev          = cur;
                }
                else if(prev != NULL)
                {
                    prev->nextInfoElement = cur;
                    prev = cur;
                }
            }

        }
        CsrBtCloseDir(dir);
    }
    else
    {
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Could not get folder info, error code %d", errno);
        return 0;
    }

    return 1;
}

/* Return folder list XML thingie */
int getFolderListing(char *filePattern, char **folderListingObjectString)
{
    struct tm *fileTime = NULL;
    FolderInfo *folderInformation;
    FolderInfo *currentFileInformation;
    char dummyString[256];
    long resultStringLength;
    time_t ftime;

    folderInformation  = NULL;
    resultStringLength = 0;

    if(getFolderInformation(filePattern, &folderInformation))
    {
        /* Begin counting number of chars in folder */
        resultStringLength += sprintf(dummyString, XML_HEADER1);
        resultStringLength += sprintf(dummyString, XML_HEADER2);
        resultStringLength += sprintf(dummyString, XML_HEADER3);

        if(CsrStrStr(filePattern, PATH_SEP) != NULL)
        {
            resultStringLength += sprintf(dummyString, "\t<parent-folder/>\n");
        }

        /* Directories */
        for(currentFileInformation = folderInformation;
            currentFileInformation;
            currentFileInformation = currentFileInformation->nextInfoElement)
        {
            if((CsrStrCmp(currentFileInformation->name, ".") != 0) &&
                (CsrStrCmp(currentFileInformation->name, "..") != 0) &&
                (currentFileInformation->mode & S_IFDIR))
            {
                ftime = currentFileInformation->time;

                fileTime = gmtime(&ftime);

                resultStringLength += sprintf(dummyString,
                    XML_FOLDER,
                    currentFileInformation->name,
                    fileTime->tm_year + 1900,
                    fileTime->tm_mon + 1,
                    fileTime->tm_mday,
                    fileTime->tm_hour,
                    fileTime->tm_min,
                    fileTime->tm_sec);
            }
        }

        /* Files */
        for(currentFileInformation = folderInformation;
            currentFileInformation;
            currentFileInformation = currentFileInformation->nextInfoElement)
        {
            if((CsrStrCmp(currentFileInformation->name, ".") != 0) &&
                (CsrStrCmp(currentFileInformation->name, "..") != 0) &&
                !(currentFileInformation->mode & S_IFDIR))
            {
                ftime = currentFileInformation->time;
                
                fileTime = gmtime(&ftime);

                resultStringLength += sprintf(dummyString,
                    XML_FILE,
                    currentFileInformation->name,
                    fileTime->tm_year + 1900,
                    fileTime->tm_mon + 1,
                    fileTime->tm_mday,
                    fileTime->tm_hour,
                    fileTime->tm_min,
                    fileTime->tm_sec,
                    (long)currentFileInformation->size,
                    (currentFileInformation->mode & S_IWRITE) ? "RWD" : "R");
            }
        }

        /* End count with newline and terminator */
        resultStringLength += sprintf(dummyString, "</folder-listing>\n");
        resultStringLength += 1;

        /* Start real generation */
        *folderListingObjectString = (char*)CsrPmemAlloc(resultStringLength);
        CsrMemSet(*folderListingObjectString, 0, resultStringLength);
        resultStringLength = 0;
        resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength], XML_HEADER1);
        resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength], XML_HEADER2);
        resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength], XML_HEADER3);

        if(CsrStrStr(filePattern, PATH_SEP) != NULL)
        {
            resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength],
                "\t<parent-folder/>\n");
        }

        /* Directories */
        for(currentFileInformation = folderInformation;
            currentFileInformation;
            currentFileInformation = currentFileInformation->nextInfoElement)
        {
            if((CsrStrCmp(currentFileInformation->name, ".") != 0) &&
                (CsrStrCmp(currentFileInformation->name, "..") != 0) &&
                (currentFileInformation->mode & S_IFDIR))
            {
                ftime = currentFileInformation->time;

                fileTime= gmtime(&ftime);
                resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength],
                    XML_FOLDER,
                    currentFileInformation->name,
                    fileTime->tm_year + 1900,
                    fileTime->tm_mon + 1,
                    fileTime->tm_mday,
                    fileTime->tm_hour,
                    fileTime->tm_min,
                    fileTime->tm_sec);
            }
        }

        /* Files */
        for(currentFileInformation = folderInformation;
            currentFileInformation;
            currentFileInformation = currentFileInformation->nextInfoElement)
        {
            if((CsrStrCmp(currentFileInformation->name, ".") != 0) &&
                (CsrStrCmp(currentFileInformation->name, "..") != 0) &&
                !(currentFileInformation->mode & S_IFDIR))
            {
                ftime = currentFileInformation->time;

                fileTime = gmtime(&ftime);
                resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength],
                    XML_FILE,
                    currentFileInformation->name,
                    fileTime->tm_year + 1900,
                    fileTime->tm_mon + 1,
                    fileTime->tm_mday,
                    fileTime->tm_hour,
                    fileTime->tm_min,
                    fileTime->tm_sec,
                    (long)currentFileInformation->size,
                    (currentFileInformation->mode & S_IWRITE) ? "RWD" : "R");
            }
        }
        resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength], "</folder-listing>\n");
        freeFolderInformation(&folderInformation);
    }
    else
    {
        *folderListingObjectString = NULL;
        return 0;
    }

    return 1;
}


/* Handle AMPM primitives */
#ifdef CSR_AMP_ENABLE
void handleAmpmPrim(CsrBtObexFtpServerAppGlobalInstData *ftpServerInst)
{
    switch(*(CsrBtAmpmPrim*)ftpServerInst->message)
    {
    case CSR_BT_AMPM_REGISTER_CFM:
        {
            CsrBtAmpmRegisterCfm *prim;
            prim = (CsrBtAmpmRegisterCfm*)ftpServerInst->message;
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "AMPM reg cfm: btConnId 0x%08x, result 0x%04x, supplier 0x%04x",
                prim->btConnId, prim->resultCode, prim->resultSupplier);
        }
        break;

    case CSR_BT_AMPM_DEREGISTER_CFM:
        {
            CsrBtAmpmDeregisterCfm *prim;
            prim = (CsrBtAmpmDeregisterCfm*)ftpServerInst->message;
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "AMPM unreg cfm: btConnId 0x%08x, supplier 0x%04x, result 0x%04x",
                prim->btConnId, prim->resultSupplier, prim->resultCode);
        }
        break;

    case CSR_BT_AMPM_MOVE_IND:
        {
            CsrBtAmpmMoveInd *prim;
            prim = (CsrBtAmpmMoveInd*)ftpServerInst->message;
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "AMP move ind, btConnId 0x%08x, localId 0x%04x - accepting...",
                prim->btConnId, prim->localId);
            CsrBtAmpmMoveResSend(prim->btConnId, TRUE);
        }
        break;

    case CSR_BT_AMPM_MOVE_CMP_IND:
    case CSR_BT_AMPM_AUTO_MOVE_CMP_IND:
        {
            CsrBtAmpmMoveCmpInd *prim;
            prim = (CsrBtAmpmMoveCmpInd*)ftpServerInst->message;
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "AMP move cmplt ind (%s), result 0x%x, supplier 0x%x, btConnId 0x%08x",
                (prim->localId == CSR_BT_AMP_CONTROLLER_BREDR)?"BR/EDR":"WIFI", prim->resultCode, prim->resultSupplier, prim->btConnId);

            if ((prim->resultCode == CSR_BT_AMPM_RESULT_SUCCESS) && (prim->resultSupplier == CSR_BT_SUPPLIER_AMPM))
            {
                ftpServerInst->channelInUse = prim->localId;

                ftpServerInst->dataCount = 0;
                ftpServerInst->moveTime = 0;
                if( ftpServerInst->channelInUse != CSR_BT_AMP_CONTROLLER_BREDR)
                {
                    ftpServerInst->moveTime = CsrTimeGet(NULL);
                }
            }
        }
        break;

    case CSR_BT_AMPM_MOVE_CFM:
        {
            CsrBtAmpmMoveCfm *prim;
            prim = (CsrBtAmpmMoveCfm*)ftpServerInst->message;
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "AMP move cfm, result 0x%x, supplier 0x%x, btConnId 0x%08x",
                prim->resultCode, prim->resultSupplier, prim->btConnId);
        }
        break;

    default:
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unhandled AMPM prim received: 0x%04x",
            *(CsrPrim*)ftpServerInst->message);
        break;
    }
}
#endif

/* Handle CM primitives */
void handleCmPrim(CsrBtObexFtpServerAppGlobalInstData *ftpServerInst)
{
    switch(*(CsrPrim*)ftpServerInst->message)
    {
        default:
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unhandled CM prim received: 0x%04x", *(CsrPrim*)ftpServerInst->message);
            break;
    }
}

/* Handle FTS events */
void handleFtsEvent(CsrBtObexFtpServerAppGlobalInstData *ftpServerInst)
{
    CsrBtFtsPrim *ftsPrim = ftpServerInst->message;

    switch(*ftsPrim)
    {
    case CSR_BT_FTS_CONNECT_IND:
        {
            CsrBtFtsConnectInd *connectInd = ftpServerInst->message;
            char buffer[CSR_BT_MAX_PATH_LENGTH];

            ftpServerInst->obexPeerMaxPacketSize = connectInd->obexPeerMaxPacketSize;
            ftpServerInst->connectingDevice = connectInd->deviceAddr;

            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "FTP client addr %04X:%02X:%06X is now connected (btConnId 0x%08x)",
                connectInd->deviceAddr.nap,
                connectInd->deviceAddr.uap,
                connectInd->deviceAddr.lap,
                connectInd->btConnId);

            ftpServerInst->btConnId = connectInd->btConnId;
            ftpServerInst->channelInUse = CSR_BT_AMP_CONTROLLER_BREDR;

#ifdef CSR_AMP_ENABLE
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Reg with AMP manager...");
            CsrBtAmpmRegisterReqSend(ftpServerInst->queueId,
                CSR_BT_AMPM_FLAGS_AUTO_MOVE_ACCEPT, /* flags */
                CSR_BT_AMPM_EVENTS_AUTO_MOVE, /* eventMask */
                ftpServerInst->btConnId, /* btConnId */
                &ftpServerInst->connectingDevice); /* addr */
#endif

            if(ftpServerInst->obexAuthentication == WITHOUT_AUTHENTICATION ||
                ftpServerInst->obexAuthentication == AUTHENTICATION_SUCCESS)
            {
                if(CsrBtChdir(FTP_SERVER_ROOT_PATH))
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unable to locate directory: %s", FTP_SERVER_ROOT_PATH);
                }

                /* Get the current working directory: */
                if((CsrBtGetCwd(buffer, CSR_BT_MAX_PATH_LENGTH)) == NULL)
                {
                    perror("CsrBtGetCwd error");
                }

                if (buffer[CsrStrLen(buffer) - 1] != '/')
                {
                    CsrStrCat(buffer, PATH_SEP);
                }
                CsrStrCpy(ftpServerInst->currentPath, buffer);
                
                CsrBtFtsConnectResSend(connectInd->connectionId,
                    CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
            }
            else
            {
                /*CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Enter obex initiate passkey : ");*/

                ftpServerInst->ftsPasswordLength = 0;
            }
            break;
        }

    case CSR_BT_FTS_SET_ROOT_FOLDER_IND:
        {
            CsrBtFtsSetRootFolderInd *setRootFolderInd = ftpServerInst->message;

            sprintf(ftpServerInst->currentPath, "%s", ftpServerInst->startupPath);

            if(CsrBtChdir(ftpServerInst->currentPath))
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unable to locate directory: %s", ftpServerInst->currentPath);
            }

            CsrBtFtsSetRootFolderResSend(setRootFolderInd->connectionId);
            break;
        }

    case CSR_BT_FTS_SET_FOLDER_IND:
        {
            CsrBtFtsSetFolderInd *setFolderInd = ftpServerInst->message;
            char path[CSR_BT_MAX_PATH_LENGTH];
            CsrUint8 *name;

            /* convert big endian unicode to little unicode */
            name = unicode2String(setFolderInd->payload + setFolderInd->ucs2nameOffset);
            CsrSnprintf(path, CSR_BT_MAX_PATH_LENGTH, "%s%s", (CsrCharString *) ftpServerInst->currentPath, (CsrCharString *) name);

            /* Neeed to check if the path exits, if not send CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE */
            if(CsrBtChdir(path))
            {
                CsrBtFtsSetFolderResSend(setFolderInd->connectionId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unable to locate directory: %s", path);
            }
            else
            {
                /* Check if the path is read only, send CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE */
                if((CsrBtAccess(path, 2)) == 0)
                {
                    sprintf(ftpServerInst->currentPath, "%s%s"PATH_SEP,
                        (CsrCharString *) ftpServerInst->currentPath,
                        (CsrCharString *) name);
                    CsrBtFtsSetFolderResSend(setFolderInd->connectionId,
                        CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                }
                else
                {
                    CsrBtFtsSetFolderResSend(setFolderInd->connectionId,
                        CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE);
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Specified directory forbidden, read only");
                }
            }
            CsrPmemFree(setFolderInd->payload);
            CsrPmemFree(name);
            break;
        }

    case CSR_BT_FTS_SET_BACK_FOLDER_IND:
        {
            
            CsrBtFtsSetBackFolderInd *setBackFolderInd = ftpServerInst->message;

            if(CsrStrLen(ftpServerInst->currentPath) > CsrStrLen(ftpServerInst->startupPath)) 
            {
                if(CsrStrLen(ftpServerInst->currentPath) > 3)
                {
                    CsrBtFtsSetBackFolderResSend(setBackFolderInd->connectionId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                    if(CsrBtChdir(".."))
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unable to locate the directory: %s", FTP_SERVER_ROOT_PATH);
                    }
                    else
                    {
                        char buffer[CSR_BT_MAX_PATH_LENGTH];

                        /* Get the current working directory: */
                        if((CsrBtGetCwd(buffer, CSR_BT_MAX_PATH_LENGTH)) == NULL)
                        {
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "CsrBtGetCwd error");
                        }
                        else
                        {
                            strcpy(ftpServerInst->currentPath, buffer);
                            CsrStrCat(ftpServerInst->currentPath, PATH_SEP);
                        }
                    }
                }
                else
                {
                    CsrBtFtsSetBackFolderResSend(setBackFolderInd->connectionId,
                        CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
                }
            }
            else
            {
                CsrBtFtsSetBackFolderResSend(setBackFolderInd->connectionId,
                    CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
            }
            break;
        }

    case CSR_BT_FTS_SET_ADD_FOLDER_IND:
        {
            CsrBtFtsSetAddFolderInd *setAddFolderInd = ftpServerInst->message;
            char path[CSR_BT_MAX_PATH_LENGTH];
            char buffer[CSR_BT_MAX_PATH_LENGTH];
            CsrUint8 *name;

            name = unicode2String(setAddFolderInd->payload + setAddFolderInd->ucs2nameOffset);

            if(ftpServerInst->obexAuthentication == AUTHENTICATION_SUCCESS)
            {
                sprintf(path, "%s%s", (CsrCharString *) ftpServerInst->currentPath, (CsrCharString *) name);
                if(CsrBtMkDir(path) == 0)
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Directory %s was successfully created", name);
                    CsrBtFtsSetAddFolderResSend(setAddFolderInd->connectionId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                    if(CsrBtChdir(path)  )
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unable to locate the directory: %s", path);
                    }
                    else
                    {
                        sprintf(ftpServerInst->currentPath, "%s%s\\", (CsrCharString *) ftpServerInst->currentPath, (CsrCharString *) name);
                    }

                    /* Get the current working directory: */
                    if((CsrBtGetCwd(buffer, CSR_BT_MAX_PATH_LENGTH)) == NULL)
                    {
                        perror("CsrBtGetCwd error");
                    }
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed creating directory %s", name);
                    CsrBtFtsSetAddFolderResSend(setAddFolderInd->connectionId,
                        CSR_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE);
                }

                if(ftpServerInst->modeSelect == 1)
                {
                    ftpServerInst->obexAuthentication = WITHOUT_AUTHENTICATION;
                }
                else
                {
                    ftpServerInst->obexAuthentication = WITH_AUTHENTICATION;
                }

                CsrPmemFree(setAddFolderInd->payload);
            }
            else
            {
                char buf[256];
                CsrSnprintf(buf, 256, "Create new directory (%s)?", name);
                CsrUiMenuSetReqSend(ftpServerInst->csrUiVar.hAllowMenu, CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_SELECT_UCS2, NULL);
                CsrUiUieShowReqSend(ftpServerInst->csrUiVar.hAllowMenu, ftpServerInst->queueId, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
                ftpServerInst->csrUiVar.allowMenuUi = TRUE;

                ftpServerInst->ftsSetAddFolderIndMsg = setAddFolderInd;
                ftpServerInst->message = NULL;
            }
            CsrPmemFree(name);
            break;
        }

    case CSR_BT_FTS_GET_LIST_FOLDER_IND:
        {
            CsrBtFtsGetListFolderInd *getListFolderInd;
            char path[CSR_BT_MAX_PATH_LENGTH];
            CsrUint8 *body;
            CsrBool finalFlag;
            CsrUint16 bytesToRead;
            CsrUint8 *name;

            getListFolderInd = ftpServerInst->message;
            if((getListFolderInd->payload != NULL) &&
                (getListFolderInd->payloadLength > 0))
            {
                name = unicode2String(getListFolderInd->payload + getListFolderInd->ucs2nameOffset);
            }
            else
            {
                name = NULL;
            }

            if(name == NULL)
            {
                CsrSnprintf(path, CSR_BT_MAX_PATH_LENGTH, "%s", ftpServerInst->currentPath);
            }
            else
            {
                CsrSnprintf(path, CSR_BT_MAX_PATH_LENGTH, "%s%s", ftpServerInst->currentPath, name);
            }

            ftpServerInst->folderListingString = NULL;
            getFolderListing(path, &ftpServerInst->folderListingString);
            if(ftpServerInst->folderListingString != NULL)
            {
                ftpServerInst->currentFileSize = CsrStrLen(ftpServerInst->folderListingString) + 1;
                ftpServerInst->currentFilePosition = 0;

                if(ftpServerInst->currentFileSize - ftpServerInst->currentFilePosition <= ftpServerInst->obexPeerMaxPacketSize)
                {
                    bytesToRead = (CsrUint16)(ftpServerInst->currentFileSize - ftpServerInst->currentFilePosition);
                }
                else
                {
                    bytesToRead = ftpServerInst->obexPeerMaxPacketSize;
                }

                body = CsrPmemAlloc(bytesToRead);
                CsrMemCpy(body, (void*)ftpServerInst->folderListingString, bytesToRead);

                ftpServerInst->currentFilePosition += bytesToRead;
                if(ftpServerInst->currentFilePosition >= ftpServerInst->currentFileSize)
                {
                    finalFlag = TRUE;
                    if(ftpServerInst->csrUiVar.commonDialogUi)
                    {
                        CsrUiUieHideReqSend(ftpServerInst->csrUiVar.hCommonDialog);
                        ftpServerInst->csrUiVar.commonDialogUi = FALSE;
                    }

                    CsrPmemFree(ftpServerInst->folderListingString);
                    ftpServerInst->folderListingString = NULL;
                }
                else
                {
                    finalFlag = FALSE;
                }
                ftpServerInst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                ftpServerTransferingObject(ftpServerInst, finalFlag);
                if (ftpServerInst->transferObject)
                {
                    CsrFtpServerCommonDialogSet(ftpServerInst, TEXT_FTP_SERVER_APP_MAIN_MENU_UCS2, TEXT_FTP_SERVER_APP_REJECT_UCS2,
                        TEXT_YES_UCS2, NULL, ftpServerInst->csrUiVar.hSk1Event, 0);
                }
                CsrBtFtsGetListFolderResSend(getListFolderInd->connectionId, finalFlag, ftpServerInst->obexResult,
                    ftpServerInst->currentFileSize, bytesToRead, body, FALSE);
            }
            else
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Error path does not exists");
                ftpServerInst->obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                CsrBtFtsGetListFolderResSend(getListFolderInd->connectionId, TRUE, ftpServerInst->obexResult, 0, 0, NULL, FALSE);
            }

            CsrPmemFree(getListFolderInd->payload);
            CsrPmemFree(name);
            break;
        }

    case CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND:
        {
            CsrBtFtsGetListFolderNextInd *getListFolderNextInd = ftpServerInst->message;
            CsrUint8 *body          = NULL;
            CsrBool finalFlag       = TRUE;
            CsrUint16 bytesToRead   = 0;

            if (ftpServerInst->obexResult == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE ||
                ftpServerInst->obexResult == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
            {
                if(ftpServerInst->folderListingString != NULL)
                {
                    if(ftpServerInst->currentFileSize - ftpServerInst->currentFilePosition <= ftpServerInst->obexPeerMaxPacketSize)
                    {
                        bytesToRead = (CsrUint16)(ftpServerInst->currentFileSize - ftpServerInst->currentFilePosition);
                    }
                    else
                    {
                        bytesToRead = ftpServerInst->obexPeerMaxPacketSize;
                    }
                    body = CsrPmemAlloc(bytesToRead);
                    CsrMemCpy(body, &ftpServerInst->folderListingString[ftpServerInst->currentFilePosition], bytesToRead);
                    ftpServerInst->currentFilePosition += bytesToRead;
                    if(ftpServerInst->currentFilePosition >= ftpServerInst->currentFileSize)
                    {
                        finalFlag = TRUE;
                        if(ftpServerInst->csrUiVar.commonDialogUi)
                        {
                            CsrUiUieHideReqSend(ftpServerInst->csrUiVar.hCommonDialog);
                            ftpServerInst->csrUiVar.commonDialogUi = FALSE;
                        }
                        CsrPmemFree(ftpServerInst->folderListingString);
                        ftpServerInst->folderListingString = NULL;
                    }
                    else
                    {
                        finalFlag = FALSE;
                    }
                    ftpServerInst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Get folder list failed: Already sent");
                    ftpServerInst->obexResult = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;
                }
            }
            else
            {   /* The user has rejected the GET LIST FOLDER operation            */
                ;
            }
            ftpServerTransferingObject(ftpServerInst, finalFlag);
            CsrBtFtsGetListFolderNextResSend(getListFolderNextInd->connectionId,
                finalFlag,
                ftpServerInst->obexResult,
                bytesToRead,
                body,
                FALSE);
            break;
        }

    case CSR_BT_FTS_GET_OBJ_IND:
        {
            CsrBtFtsGetObjInd  *getObjInd = ftpServerInst->message;
            CsrBtStat_t   statBuffer;
            int           result;
            CsrUint8       *name;
            char          path[CSR_BT_MAX_PATH_LENGTH];

            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));

            /* Get full ASCII path */
            name = unicode2String(getObjInd->payload + getObjInd->ucs2nameOffset);
            CsrSnprintf(path, CSR_BT_MAX_PATH_LENGTH, "%s%s", (CsrCharString *) ftpServerInst->currentPath, (CsrCharString *) name);

            if(ftpServerInst->obexAuthentication == AUTHENTICATION_SUCCESS)
            {
                CsrUint8    *body   = NULL;
                CsrBool finalFlag   = TRUE;
                CsrUint16 bytesRead = 0;

                result = CsrBtStat((char*)path, &statBuffer);
                ftpServerInst->currentFileSize = 0;

                if(result == 0)
                {
                    ftpServerInst->file = CsrBtFopen(path, "rb");
                    if(ftpServerInst->file != NULL)
                    {
                        CsrUint16 bytesToRead;

                        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Get object (%s)...", path);
                        ftpServerInst->currentFilePosition = 0;
                        ftpServerInst->currentFileSize = statBuffer.st_size;
                        if(ftpServerInst->currentFileSize - ftpServerInst->currentFilePosition <= ftpServerInst->obexPeerMaxPacketSize)
                        {
                            bytesToRead = (CsrUint16)(ftpServerInst->currentFileSize - ftpServerInst->currentFilePosition);
                        }
                        else
                        {
                            bytesToRead = ftpServerInst->obexPeerMaxPacketSize;
                        }
                        body = CsrPmemAlloc(bytesToRead);
                        bytesRead = CsrBtFread(body, 1, bytesToRead, ftpServerInst->file);
                        ftpServerInst->currentFilePosition += bytesRead;
                        if(ftpServerInst->currentFilePosition >= ftpServerInst->currentFileSize)
                        {
                            CsrBtFclose(ftpServerInst->file);
                            ftpServerInst->file = NULL;
                            finalFlag = TRUE;
                            if(ftpServerInst->csrUiVar.commonDialogUi)
                            {
                                CsrUiUieHideReqSend(ftpServerInst->csrUiVar.hCommonDialog);
                                ftpServerInst->csrUiVar.commonDialogUi = FALSE;
                            }
                        }
                        else
                        {
                            finalFlag = FALSE;
                        }
                        ftpServerInst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                    }
                    else
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to open file (%s)!", path);
                        ftpServerInst->obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                    }
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to get file info (%s)!", path);
                    ftpServerInst->obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                }

                ftpServerTransferingObject(ftpServerInst, finalFlag);

                if (ftpServerInst->transferObject)
                {
                    CsrFtpServerCommonDialogSet(ftpServerInst, TEXT_FTP_SERVER_APP_MAIN_MENU_UCS2, TEXT_FTP_SERVER_APP_REJECT_UCS2,
                        TEXT_YES_UCS2, NULL, ftpServerInst->csrUiVar.hSk1Event, 0);
                }

                CsrBtFtsGetObjResSend(getObjInd->connectionId,
                    finalFlag,
                    ftpServerInst->obexResult,
                    ftpServerInst->currentFileSize,
                    bytesRead,
                    body,
                    FALSE);
                CsrPmemFree(getObjInd->payload);
            }
            else
            {
                char buf[256];
                ftpServerInst->ftsGetObjIndMsg = getObjInd;

                sprintf(buf, "Allow get_object request?");
                CsrUiMenuSetReqSend(ftpServerInst->csrUiVar.hAllowMenu, CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_SELECT_UCS2, NULL);
                CsrUiUieShowReqSend(ftpServerInst->csrUiVar.hAllowMenu, ftpServerInst->queueId, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
                ftpServerInst->csrUiVar.allowMenuUi = TRUE;

                ftpServerInst->message = NULL;
            }
            CsrPmemFree(name);
            break;
        }

    case CSR_BT_FTS_GET_OBJ_NEXT_IND:
        {
            CsrBtFtsGetObjNextInd * getObjNextInd = ftpServerInst->message;
            CsrUint8    *body = NULL;
            CsrBool     finalFlag = TRUE;
            CsrUint16   bytesRead = 0;

            if (ftpServerInst->obexResult == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE ||
                ftpServerInst->obexResult == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
            {
                if(ftpServerInst->file != NULL)
                {
                    CsrUint16 bytesToRead;

                    if(ftpServerInst->currentFileSize - ftpServerInst->currentFilePosition <= ftpServerInst->obexPeerMaxPacketSize)
                    {
                        bytesToRead = (CsrUint16)(ftpServerInst->currentFileSize - ftpServerInst->currentFilePosition);
                    }
                    else
                    {
                        bytesToRead = ftpServerInst->obexPeerMaxPacketSize;
                    }
                    body = CsrPmemAlloc(bytesToRead);
#ifdef FAKE_FILE
                    bytesRead = bytesToRead;
#else
                    bytesRead = CsrBtFread(body, 1, bytesToRead, ftpServerInst->file);
#endif
                    ftpServerInst->dataCount += bytesRead;

                    ftpServerInst->currentFilePosition += bytesRead;
                    if(ftpServerInst->currentFilePosition >= ftpServerInst->currentFileSize)
                    {
                        if (ftpServerInst->moveTime)
                        {
                            CsrUint32 speed;
                            speed = (ftpServerInst->dataCount/((CsrTimeGet(NULL) - ftpServerInst->moveTime)/1000))<<3;
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "AMP data rate=%u kbps/s, (%u bytes in %u ms) ###",
                                                    speed, ftpServerInst->dataCount, (CsrTimeGet(NULL) - ftpServerInst->moveTime)/1000);
                            ftpServerInst->moveTime = 0;
                        }

                        CsrBtFclose(ftpServerInst->file);
                        ftpServerInst->file = NULL;
                        finalFlag = TRUE;
                        if(ftpServerInst->csrUiVar.commonDialogUi)
                        {
                            CsrUiUieHideReqSend(ftpServerInst->csrUiVar.hCommonDialog);
                            ftpServerInst->csrUiVar.commonDialogUi = FALSE;
                        }
                    }
                    else
                    {
                        finalFlag = FALSE;
                    }
                    ftpServerInst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Get Obj failed");
                    ftpServerInst->obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                }
            }
            else
            {   /* The user has rejected the GET LIST FOLDER operation            */
                ;
            }
            ftpServerTransferingObject(ftpServerInst, finalFlag);
            CsrBtFtsGetObjNextResSend(getObjNextInd->connectionId,
                finalFlag,
                ftpServerInst->obexResult,
                bytesRead,
                body,
                FALSE);
            break;
        }

    case CSR_BT_FTS_PUT_OBJ_IND:
        {
            CsrBtFtsPutObjInd *putObjInd = ftpServerInst->message;

            ftpServerInst->bytesReceived = putObjInd->bodyLength;
            ftpServerInst->totalBytes = putObjInd->lengthOfObject;
            if(ftpServerInst->obexAuthentication == AUTHENTICATION_SUCCESS)
            {
                FILE *putFile;
                char *name;

                name = (char *) unicode2String(putObjInd->payload + putObjInd->ucs2nameOffset);
                putFile = CsrBtFopen(name, "wb");
                if(putFile != NULL)
                {
                    CsrBtFwrite((putObjInd->payload + putObjInd->bodyOffset),
                        1,
                        putObjInd->bodyLength,
                        putFile);
                    CsrBtFclose(putFile);

                    ftpServerInst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                    CsrPmemFree(ftpServerInst->lastfileSaved);
                    ftpServerInst->lastfileSaved = NULL;
                    if(!putObjInd->finalFlag)
                    {
                        ftpServerInst->lastfileSaved = (CsrUint8 *) name;
                    }
                    else
                    {
                        if(ftpServerInst->totalBytes != 0)
                        {
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Saved %s, bytes received %d of %d", name,
                                ftpServerInst->bytesReceived, ftpServerInst->totalBytes);
                        }
                        else
                        {
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Saved %s, bytes received %d", name, ftpServerInst->bytesReceived);
                        }
                    }
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to save obj (%s)!", name);
                    ftpServerInst->obexResult = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
                }

                ftpServerTransferingObject(ftpServerInst, putObjInd->finalFlag);

                if (ftpServerInst->transferObject)
                {
                    CsrFtpServerCommonDialogSet(ftpServerInst, TEXT_FTP_SERVER_APP_MAIN_MENU_UCS2, TEXT_FTP_SERVER_APP_REJECT_UCS2,
                        TEXT_YES_UCS2, NULL, ftpServerInst->csrUiVar.hSk1Event, 0);
                }

                CsrBtFtsPutObjResSend(putObjInd->connectionId,
                                      ftpServerInst->obexResult,
                                      FALSE);

                if(ftpServerInst->modeSelect == 1)
                {
                    ftpServerInst->obexAuthentication = WITHOUT_AUTHENTICATION;
                }
                else
                {
                    ftpServerInst->obexAuthentication = WITH_AUTHENTICATION;
                }
                if(putObjInd->finalFlag)
                {
                    if(ftpServerInst->csrUiVar.commonDialogUi)
                    {
                        CsrUiUieHideReqSend(ftpServerInst->csrUiVar.hCommonDialog);
                        ftpServerInst->csrUiVar.commonDialogUi = FALSE;
                    }
                    CsrPmemFree(name);
                }
                CsrPmemFree(putObjInd->payload);
            }
            else
            {
                char buf[256];

                CsrUint8 *name = unicode2String(putObjInd->payload + putObjInd->ucs2nameOffset);
                ftpServerInst->ftsPutObjIndMsg = putObjInd;

                CsrSnprintf(buf, 256, "Object '%s' received. Store object?", (CsrCharString *) name);
                CsrUiMenuSetReqSend(ftpServerInst->csrUiVar.hAllowMenu, CONVERT_TEXT_STRING_2_UCS2(buf), TEXT_SELECT_UCS2, NULL);
                CsrUiUieShowReqSend(ftpServerInst->csrUiVar.hAllowMenu, ftpServerInst->queueId, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);
                ftpServerInst->csrUiVar.allowMenuUi = TRUE;

                ftpServerInst->message = NULL;
                CsrPmemFree(name);
            }
            break;
        }

    case CSR_BT_FTS_PUT_OBJ_NEXT_IND:
        {
            CsrBtFtsPutObjNextInd *putObjNextInd = ftpServerInst->message;

            if (ftpServerInst->obexResult == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE ||
                ftpServerInst->obexResult == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
            {
#ifndef FAKE_FILE
                FILE     *putFile;
                char     fileName[128];

                ftpServerInst->bytesReceived += putObjNextInd->bodyLength;
                if(ftpServerInst->totalBytes != 0)
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Append to file %s, bytes received %d of %d",
                        ftpServerInst->lastfileSaved, ftpServerInst->bytesReceived, ftpServerInst->totalBytes);
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Append to file %s, bytes received %d",
                        ftpServerInst->lastfileSaved,
                        ftpServerInst->bytesReceived);
                }

                CsrSnprintf(fileName, 128, "%s", (CsrCharString *) ftpServerInst->lastfileSaved);
                putFile = CsrBtFopen(fileName, "ab");

                if(putFile != NULL)
                {
                    CsrBtFwrite((putObjNextInd->payload + putObjNextInd->bodyOffset),
                        1,
                        putObjNextInd->bodyLength,
                        putFile);

                    CsrBtFclose(putFile);
#endif
                    ftpServerInst->dataCount += putObjNextInd->bodyLength;

                    if(putObjNextInd->finalFlag)
                    {
                        if (ftpServerInst->moveTime)
                        {
                            CsrUint32 speed;
                    
                            speed = (ftpServerInst->dataCount/((CsrTimeGet(NULL) - ftpServerInst->moveTime)/1000))<<3;
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "AMP data rate=%u kbps/s, (%u bytes in %u ms) ###",
                                   speed, ftpServerInst->dataCount, (CsrTimeGet(NULL) - ftpServerInst->moveTime)/1000);
                            ftpServerInst->moveTime = 0;
                        }
                        
                        ftpServerInst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "File '%s' was saved", ftpServerInst->lastfileSaved);
                        if(ftpServerInst->csrUiVar.commonDialogUi)
                        {
                            CsrUiUieHideReqSend(ftpServerInst->csrUiVar.hCommonDialog);
                            ftpServerInst->csrUiVar.commonDialogUi = FALSE;
                        }
                        CsrPmemFree(ftpServerInst->lastfileSaved);
                        ftpServerInst->lastfileSaved = NULL;
                    }
                    else
                    {
                        ftpServerInst->obexResult = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                    }
#ifndef FAKE_FILE
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed saving obj (%s)!", ftpServerInst->lastfileSaved);
                    ftpServerInst->obexResult = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
                }
#endif
            }
            else
            {
                /* The user has rejected the PUT operation                    */
                ;
            }

            ftpServerTransferingObject(ftpServerInst, putObjNextInd->finalFlag);

            CsrBtFtsPutObjNextResSend(putObjNextInd->connectionId, ftpServerInst->obexResult, FALSE);

            CsrPmemFree(putObjNextInd->payload);
            break;
        }

    case CSR_BT_FTS_DEL_OBJ_IND:
        {
            CsrBtFtsDelObjInd *delObjInd = ftpServerInst->message;
            CsrUint8 obexResult;
            CsrBtStat_t statBuffer;
            int result;
            CsrUint8 *name;


            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));

            name = unicode2String(delObjInd->payload + delObjInd->ucs2nameOffset);
            result = CsrBtStat((char*)name, &statBuffer);

            if(!result)
            {
                if(statBuffer.st_mode & S_IFDIR)
                {
                    /* Delete directory */
                    result = recursiveRmDir((char*)name);
                    if(result == 0)
                    {
                        /* Success */
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Directory (%s) deleted.", name);
                        obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                    }
                    else
                    {
                        if(errno & ENOTEMPTY)
                        {
                            /* Directory is not empty */
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to delete directory, not empty (%s)!", name);
                            obexResult = CSR_BT_OBEX_PRECONDITION_FAILED_RESPONSE_CODE;
                        }
                        else
                        {
                            /* Error */
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to delete directory (%s)!", name);
                            obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                        }
                    }
                }
                else if(statBuffer.st_mode & S_IFREG)
                {
                    /* Delete file */
                    result  =  CsrBtRemove((char*)name);
                    if(result == 0)
                    {
                        /* Success */
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "File (%s) deleted.", name);
                        obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                    }
                    else
                    {
                        if(errno & EACCES)
                        {
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to delete file (%s), file is read only!", name);
                            obexResult = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;
                        }
                        else
                        {
                            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to delete file (%s)!", name);
                            obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                        }
                    }
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to delete file (%s)!", name);
                    obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                }
            }
            else
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to delete obj (%s)!", name);
                obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
            }
            CsrBtFtsDelObjResSend(delObjInd->connectionId, obexResult, FALSE);

            CsrPmemFree(delObjInd->payload);
            CsrPmemFree(name);
            break;
        }

    case CSR_BT_FTS_AUTHENTICATE_IND:
        {
            CsrBtFtsAuthenticateInd *prim = (CsrBtFtsAuthenticateInd *) ftpServerInst->message;
            ftpServerInst->csrUiVar.obexPasskeyUi = OBEX_PASSKEY_RESPONSE;

            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Obex auth indication");
            CsrUiUieShowReqSend(ftpServerInst->csrUiVar.hObexPasskeyInputDialog, ftpServerInst->queueId, CSR_UI_INPUTMODE_AUTO, CSR_BT_FTP_PRIORITY);

            ftpServerInst->ftsPasswordLength = 0;
            CsrPmemFree(prim->realm);
            break;
        }

    case CSR_BT_FTS_AUTHENTICATE_CFM:
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Obex auth success");
            ftpServerInst->obexAuthentication = AUTHENTICATION_SUCCESS;
            break;
        }

    case CSR_BT_FTS_ABORT_IND:
        {
            CsrBtFtsAbortInd *abortInd = ftpServerInst->message;
            int         result;
            CsrUint8    *description;

            if(abortInd->descriptionOffset > 0)
            {
                description = unicode2String(abortInd->payload + abortInd->descriptionOffset);
            }
            else
            {
                description = CsrPmemAlloc(4);
                CsrMemCpy(description, "n/a", 4);
            }

            /* ??? */
            ftpServerInst->transferObject = FALSE;

            if(ftpServerInst->csrUiVar.commonDialogUi)
            {
                CsrUiUieHideReqSend(ftpServerInst->csrUiVar.hCommonDialog);
                ftpServerInst->csrUiVar.commonDialogUi = FALSE;
            }

            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, TRUE, "Operation aborted from client, description (%s)", description);
            if(ftpServerInst->lastfileSaved != NULL)
            {
                /* Delete file */
                result  =  CsrBtRemove((char*)ftpServerInst->lastfileSaved);
                if(result == 0)
                {
                    /* Success */
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Failed to delete file (%s)!", ftpServerInst->lastfileSaved);
                }
                CsrPmemFree(ftpServerInst->lastfileSaved);
                ftpServerInst->lastfileSaved = NULL;
            }
            if(ftpServerInst->folderListingString)
            {
                CsrPmemFree(ftpServerInst->folderListingString);
                ftpServerInst->folderListingString = NULL;
            }
            CsrPmemFree(abortInd->payload);

            CsrPmemFree(description);
            break;
        }

    case CSR_BT_FTS_DISCONNECT_IND:
        {
            CsrBtFtsDisconnectInd *disconnectInd = ftpServerInst->message;
            char buf[256];

            sprintf(buf,"Ftp client disconnected (%04X:%02X:%06X)",
                    disconnectInd->deviceAddr.nap,
                    disconnectInd->deviceAddr.uap,
                    disconnectInd->deviceAddr.lap);
            if(ftpServerInst->obexAuthentication == AUTHENTICATION_SUCCESS)
            {
                ftpServerInst->obexAuthentication = WITH_AUTHENTICATION;
            }
            
            if(ftpServerInst->csrUiVar.commonDialogUi)
            {
                CsrUiUieHideReqSend(ftpServerInst->csrUiVar.hCommonDialog);
                ftpServerInst->csrUiVar.commonDialogUi = FALSE;
            }
            CsrFtpServerCommonDialogSet(ftpServerInst, TEXT_FTP_SERVER_APP_MAIN_MENU_UCS2, CONVERT_TEXT_STRING_2_UCS2(buf), 
                        TEXT_OK_UCS2, NULL, ftpServerInst->csrUiVar.hSk1Event, 0);
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, buf);

#ifdef CSR_AMP_ENABLE
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unreg with AMPM...");
            CsrBtAmpmDeregisterReqSend(ftpServerInst->queueId,
                ftpServerInst->btConnId,
                &ftpServerInst->connectingDevice);
#endif

            if(CsrBtChdir(ftpServerInst->startupPath))
            {
                CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unable to locate the directory: %s", ftpServerInst->startupPath);
            }
            CsrPmemFree(ftpServerInst->folderListingString);
            break;
        }
    case CSR_BT_FTS_DEACTIVATE_CFM:
        {   
            if(ftpServerInst->csrUiVar.commonDialogUi)
            {
                CsrUiUieHideReqSend(ftpServerInst->csrUiVar.hCommonDialog);
                ftpServerInst->csrUiVar.commonDialogUi = FALSE;
            }
            CsrFtpServerCommonDialogSet(ftpServerInst, TEXT_FTP_SERVER_APP_MAIN_MENU_UCS2, CONVERT_TEXT_STRING_2_UCS2("Deactivated!"), 
                        TEXT_OK_UCS2, NULL, ftpServerInst->csrUiVar.hSk1Event, 0);
            CsrBtFtpServerAppHandleCsrUiActivateDeactivate(ftpServerInst, FALSE);
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Deactivated!");
            break;
        }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    case CSR_BT_FTS_MOVE_OBJ_IND:
        {
            CsrBtFtsMoveObjInd * prim = (CsrBtFtsMoveObjInd *) ftpServerInst->message;
            CsrUint8 *srcName;
            CsrUint8 *destName;
            
            CsrResult renameRes;
            
            srcName = unicode2String(prim->payload + prim->ucs2srcNameOffset);
            destName = unicode2String(prim->payload + prim->ucs2destNameOffset);
            
            renameRes = CsrFileRename(srcName,destName);
            
            switch(renameRes)
            {
                case 0:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Move %s -> %s", 
                                         srcName,destName);
                    
                    CsrBtFtsMoveObjResSend(prim->connectionId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                    break;

                case CSR_FILE_RESULT_NOT_EXIST:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Rename %s -> %s failed, files does not exists",
                                         srcName,destName);
                    CsrBtFtsSetObjPermissionsResSend(prim->connectionId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
                    break;

                case CSR_FILE_RESULT_NOT_ALLOWED:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Rename %s -> %s failed due to permission",
                                         srcName,destName);
                    CsrBtFtsSetObjPermissionsResSend(prim->connectionId, CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE);
                    break;

                default:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Rename %s -> %s failed with 0x%x", 
                                         srcName,destName,renameRes);
                    CsrBtFtsMoveObjResSend(prim->connectionId, CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE);
                    break;
            }
        }
        break;

    case CSR_BT_FTS_COPY_OBJ_IND:
        {
            CsrBtFtsCopyObjInd * prim = (CsrBtFtsCopyObjInd *) ftpServerInst->message;
            CsrUtf8String *srcName;
            CsrUtf8String *destName;
            CsrResult copyRes;
            
            srcName = unicode2String(prim->payload + prim->ucs2srcNameOffset);
            destName = unicode2String(prim->payload + prim->ucs2destNameOffset);

            copyRes = CsrBtCopy((CsrCharString *)srcName, (CsrCharString *)destName);

            switch(copyRes)
            {
                case 0:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Copy %s -> %s", 
                                         srcName,destName);
            
                    CsrBtFtsCopyObjResSend(prim->connectionId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                    break;

                case CSR_DIR_RESULT_INVALID_PATH:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Error Copy %s -> %s, NOT-FOUND", 
                                         srcName,destName);
                    
                    CsrBtFtsMoveObjResSend(prim->connectionId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
                    break;

                case CSR_DIR_RESULT_NOT_EXIST:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Error Copy %s -> %s, NOT-EXISTS", srcName,destName);
                    
                    CsrBtFtsMoveObjResSend(prim->connectionId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
                    
                    break;

                default:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Error Copy %s -> %s, res = 0x%x", 
                                         srcName,destName,copyRes);
                    CsrBtFtsMoveObjResSend(prim->connectionId, CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE);
                    break;
            }
        }
        break;

    case CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND:
    {
        CsrBtFtsSetObjPermissionsInd * prim = (CsrBtFtsSetObjPermissionsInd *) ftpServerInst->message;

        CsrUint8 *name;
        CsrUint32 obexPermissions;
        CsrResult setResult;
        CsrBool mapResult = FALSE;
        CsrFilePerms filePermission = 0;
        
        name = unicode2String(prim->payload + prim->ucs2nameOffset);
        obexPermissions = prim->permissions;
        
        filePermission = mapObexPermToFilePerm(obexPermissions,&mapResult);

        if (mapResult == TRUE)
        {
            setResult = CsrFileSetPerms(name,filePermission);

            switch(setResult)
            {
                case 0:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "SetPerm 0x%x on %s (local)",filePermission,name);
                    CsrBtFtsSetObjPermissionsResSend(prim->connectionId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                    break;

                case CSR_FILE_RESULT_NOT_EXIST:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "SetPerm failed as file (%s) did not exist",name);
                    CsrBtFtsSetObjPermissionsResSend(prim->connectionId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
                    break;

                case CSR_FILE_RESULT_NOT_ALLOWED:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "SetPerm failed on %s due to NOT_ALLOWED",name);
                    CsrBtFtsSetObjPermissionsResSend(prim->connectionId, CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE);
                    break;

                default:
                    CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "SetPerm failed: res = 0x%x",setResult);
                    CsrBtFtsSetObjPermissionsResSend(prim->connectionId, CSR_BT_OBEX_CONFLICT_RESPONSE_CODE);
            }
        }
        else
        {
            CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Use of permission D or M is not supported");                               
            CsrBtFtsSetObjPermissionsResSend(prim->connectionId, CSR_BT_OBEX_CONFLICT_RESPONSE_CODE);
        }

        break;
    }
#endif
    default:
        CsrAppBacklogReqSend(TECH_BT, PROFILE_FTP_SERVER, FALSE, "Unhandled FTS prim: 0x%04x", *ftsPrim);
        CsrBtFtsFreeUpstreamMessageContents(CSR_BT_FTS_PRIM, ftpServerInst->message);
        break;
    }
}
