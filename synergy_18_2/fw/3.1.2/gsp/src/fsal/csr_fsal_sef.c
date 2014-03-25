/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_util.h"

#include "csr_fsal_prim.h"
#include "csr_fsal_handler.h"

#include "csr_fsal_lib.h"
#include "csr_fsal_sef.h"
#include "csr_fsal_sef_utils.h"
#include "csr_fsal_utils.h"

#include "csr_file.h"
#include "csr_dir.h"

#include "csr_pmem.h"
#include "csr_log_text_2.h"

static CsrResult csrFsalMapCsrFileResultToCsrFsalResult(CsrResult result)
{
    switch (result)
    {
        case CSR_RESULT_SUCCESS:
            return CSR_RESULT_SUCCESS;
        case CSR_FILE_RESULT_ALREAD_EXISTS:
            return CSR_FSAL_FILE_OP_ALREAD_EXISTS;
        case CSR_FILE_RESULT_EOF:
            return CSR_FSAL_FILE_OP_EOF;
        case CSR_FILE_RESULT_FAILURE:
            return CSR_FSAL_FILE_OP_FAILURE;
        case CSR_FILE_RESULT_NOT_ALLOWED:
            return CSR_FSAL_FILE_OP_NOT_ALLOWED;
        case CSR_FILE_RESULT_NOT_EXIST:
            return CSR_FSAL_FILE_OP_NOT_EXIST;
        case CSR_FILE_RESULT_NO_SPACE:
            return CSR_FSAL_FILE_OP_NO_SPACE;
        case CSR_FILE_RESULT_READ_ONLY:
            return CSR_FSAL_FILE_OP_READ_ONLY;
        default:
            return CSR_FSAL_FILE_OP_FAILURE;
    }
}

/********************************************************************************
************Start of the functions which shall take care ************************
************of the answer from the external file system *************************
********************************************************************************/

/********************************************************************************
*
*   Name:
*       csrFsalHandleFileOpenCfmAnswer
*
*   Description:
*       "Reception of the open confirm from the external file system"
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleFileOpenCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*       If a function based interface to the file system outside the
*       scheduler is present the function "_wfopen" shall be changed to reflect
*       the actual interface. The function "CsrFsalFileSize" shall be ported as well
*       to reflect the actual interface to the file system.
*
*       Please use the "CsrFsalClearHandleIndexFromSeqNum" to clear the reserved
*       handle when the open fails and the fucntion
*       "CsrFsalGetAppHandleFromSeqNumAndStoreFsHandle" to get the appHandle.
*
*       Notice when creating a new file no checking is yet made of whether the
*       directory is read-only and by this the creation should fail.
*       There is also no checking of whether the file already exists as read-only.
*
*********************************************************************************/
static void csrFsalHandleFileOpenCfmAnswer(CsrFsalInstanceData *pInst,
    const CsrUtf8String *fileName,
    CsrFsalFileOpenFlags flags,
    CsrFsalFileOpenPerms perms,
    CsrFsalSession sessionId)
{
    CsrResult result;
    CsrFileHandle *fileHandle = CSR_FILE_INVALID_HANDLE;
    CsrFileOpenFlags openFlags = CsrFsalLookupOpenFlags(flags);
    CsrFilePerms openPerms = CsrFsalLookupPerms(perms);
    CsrSize filesize = 0;
    CsrFsalHandles *entry = CsrFsalGetSession(pInst, sessionId);

    result = CsrFileOpen(&fileHandle, fileName, openFlags, openPerms);

    result = csrFsalMapCsrFileResultToCsrFsalResult(result);

    if (result == CSR_RESULT_SUCCESS)
    {
        result = CsrFsalFileSize(fileName, &filesize);

        result = csrFsalMapCsrFileResultToCsrFsalResult(result);

        if (result == CSR_RESULT_SUCCESS)
        {
            CsrFsalFileHandleInit(&entry->fileHandles, fileHandle);
        }
    }

    CsrFsalFileOpenCfmSend(entry->appHandle, sessionId, (CsrFsalHandle) fileHandle, result, (CsrUint32) filesize);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleFileCloseCfmAnswer
*
*   Description:
*
*********************************************************************************/
static void csrFsalHandleFileCloseCfmAnswer(CsrFsalInstanceData *pInst,
    CsrFsalHandle fsalHandle)
{
    CsrSchedQid appHandle = CsrFsalGetAppHandleFromFileHandleAndClearFileHandleIndex(&pInst->handleList, (CsrFileHandle *) fsalHandle);

    CsrFsalFileCloseCfmSend(appHandle, fsalHandle, CSR_RESULT_SUCCESS);

    CsrFileClose((CsrFileHandle *) fsalHandle);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleFileSeekCfmAnswer
*
*   Description:
*       "Reception of the seek confirm from the external file system"
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleFileSeekCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*       If a function based interface to the file system outside the
*       scheduler is present the function "fseek" shall be changed to reflect
*       the actual interface and the corresponding result codes shall be ported
*       as well.
*
*       Please use the "CsrFsalGetAppHandleFromFsHandle" to find the appHandle
*       from the received fsHandle.
*
*********************************************************************************/
static void csrFsalHandleFileSeekCfmAnswer(CsrFsalInstanceData *pInst,
    CsrFsalHandle fsalHandle,
    CsrInt32 offset,
    CsrInt32 origin)
{
    CsrResult result;
    CsrSchedQid appHandle;

    result = CsrFileSeek((CsrFileHandle *) fsalHandle, offset, origin);

    result = csrFsalMapCsrFileResultToCsrFsalResult(result);

    appHandle = CsrFsalGetAppHandleFromFileHandle(pInst, fsalHandle);

    CsrFsalFileSeekCfmSend(appHandle, fsalHandle, result);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleFileTellCfmAnswer
*
*   Description:
*       "Reception of the tell confirm from the external file system"
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleFileTellCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*       If a function based interface to the file system outside the
*       scheduler is present the function "ftell" shall be changed to reflect
*       the actual interface and the corresponding result codes shall be ported
*       as well.
*
*       Please use the "CsrFsalGetAppHandleFromFsHandle" to find the appHandle
*       from the received fsHandle.
*
*********************************************************************************/
static void csrFsalHandleFileTellCfmAnswer(CsrFsalInstanceData *pInst,
    CsrFsalHandle fsalHandle)
{
    CsrResult result;
    CsrUint32 position;
    CsrSchedQid appHandle;

    result = CsrFileTell(((CsrFileHandle *) fsalHandle), &position);

    result = csrFsalMapCsrFileResultToCsrFsalResult(result);

    appHandle = CsrFsalGetAppHandleFromFileHandle(pInst, fsalHandle);

    CsrFsalFileTellCfmSend(appHandle, fsalHandle, position, result);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleFileReadCfmAnswer
*
*   Description:
*       "Reception of the read confirm from the external file system"
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleFileReadCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*       If a function based interface to the file system outside the
*       scheduler is present the function "fread" shall be changed to reflect
*       the actual interface and the corresponding result codes shall be ported
*       as well.
*
*       Please use the "CsrFsalGetAppHandleFromFsHandle" to find the appHandle
*       from the received fsHandle.
*
*********************************************************************************/
static void csrFsalHandleFileReadCfmAnswer(CsrFsalInstanceData *pInst,
    CsrFsalHandle fsalHandle,
    CsrUint32 length)
{
    CsrResult result;
    CsrUint8 *data = CsrPmemAlloc(length);
    CsrSize numReads;
    CsrSchedQid appHandle;

    result = CsrFileRead(data, length, (CsrFileHandle *) fsalHandle, &numReads);

    result = csrFsalMapCsrFileResultToCsrFsalResult(result);

    appHandle = CsrFsalGetAppHandleFromFileHandle(pInst, fsalHandle);

    CsrFsalFileReadCfmSend(appHandle, fsalHandle, result, (CsrUint32) numReads, data);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleFileWriteCfmAnswer
*
*   Description:
*       "Reception of the write confirm from the external file system"
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleFileWriteCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*       If a function based interface to the file system outside the
*       scheduler is present the function "fwrite" shall be changed to reflect
*       the actual interface and the corresponding result codes shall be ported
*       as well.
*
*       Please use the "CsrFsalGetAppHandleFromFsHandle" to find the appHandle
*       from the received fsHandle.
*
*********************************************************************************/
static void csrFsalHandleFileWriteCfmAnswer(CsrFsalInstanceData *pInst,
    CsrFsalHandle fsalHandle,
    CsrUint32 dataLen,
    CsrUint8 *data)
{
    CsrResult result;
    CsrSize numWrites;
    CsrSchedQid appHandle;

    result = CsrFileWrite(data, dataLen,
        ((CsrFileHandle *) fsalHandle), &numWrites);

    result = csrFsalMapCsrFileResultToCsrFsalResult(result);

    appHandle = CsrFsalGetAppHandleFromFileHandle(pInst, fsalHandle);

    CsrFsalFileWriteCfmSend(appHandle, fsalHandle, result, (CsrUint32) numWrites);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleRemoveCfmAnswer
*
*   Description:
*       "Reception of the remove confirm from the external file system"
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleRemoveCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*       If a function based interface to the file system outside the
*       scheduler is present the functions "_wstat" and "_wrmdir" shall be
*       changed to reflect the actual interface and the corresponding result
*       codes shall be ported as well.
*
*       Please use the "CsrFsalClearHandleIndexFromSeqNum" to find the appHandle
*       from the received sequence number and to clear the handle.
*
*       Notice when deleting a file or directory no checking is yet made of
*       whether the file or directory is read-only and by this the deleting should fail.
*
*********************************************************************************/
static void csrFsalHandleRemoveCfmAnswer(CsrFsalInstanceData *pInst,
    CsrUtf8String *path,
    CsrFsalSession sessionId)
{
    CsrResult result;
    CsrDirEntryStat buffer;
    CsrFsalHandles *entry = CsrFsalGetSession(pInst, sessionId);

    /* Need to find out if a file or a folder is requested to be deleted    */
    result = CsrDirStat(path, &buffer);

    if (result == CSR_RESULT_SUCCESS)
    {
        if (!(buffer.mode & CSR_DIR_MODE_DIRECTORY)) /* it is a file -> request to delete a file                                       */
        {
            result = CsrFileRemove(path);

            if (result != CSR_RESULT_SUCCESS)
            {
                if (result == CSR_FILE_RESULT_READ_ONLY)
                {
                    result = CSR_FSAL_DELETE_READ_ONLY;
                }
                else if (result == CSR_FILE_RESULT_NOT_EXIST)
                {
                    result = CSR_FSAL_DELETE_NOT_EXIST;
                }
                else
                {
                    result = CSR_FSAL_DELETE_FAILURE;
                }
            }
        }
        else /* Request to remove a folder                                     */
        {
            result = CsrDirRemove(path);

            if (result != CSR_RESULT_SUCCESS)
            {
                if (result == CSR_DIR_RESULT_NOT_EMPTY)
                {
                    result = CSR_FSAL_DELETE_NOT_EMPTY;
                }
                else if (result == CSR_DIR_RESULT_INVALID_PATH)
                {
                    result = CSR_FSAL_DELETE_NOT_EXIST;
                }
                else
                {
                    result = CSR_FSAL_DELETE_FAILURE;
                }
            }
        }
    }
    else /* The File or Folder don't exist, the path is invalid                */
    {
        result = CSR_FSAL_DELETE_NOT_EXIST;
    }

    CsrFsalRemoveCfmSend(entry->appHandle, result, sessionId);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleRemoveRecursivelyCfmAnswer
*
*   Description:
*       The function recursively handles the removal of a specified directory
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleRemoveRecursivelyCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*       If a function based interface to the file system outside the
*       scheduler is present the functions "_wstat" and "_wrmdir" shall be
*       changed to reflect the actual interface and the corresponding result
*       codes shall be ported as well.
*
*       Please use the "CsrFsalClearHandleIndexFromSeqNum" to find the appHandle
*       from the received sequence number and to clear the handle.
*
*       Notice when deleting a file or directory no checking is yet made of
*       whether the file or directory is read-only and by this the deleting should fail.
*
*********************************************************************************/
static void csrFsalHandleRemoveRecursivelyCfmAnswer(CsrFsalInstanceData *pInst,
    CsrUtf8String *path,
    CsrFsalSession sessionId)
{
    CsrResult result;
    CsrFsalHandles *entry = CsrFsalGetSession(pInst, sessionId);

    result = CsrFsalRemoveRecursively(path);

    if (result != CSR_RESULT_SUCCESS)
    {
        result = CSR_FSAL_DELETE_FAILURE;
    }
    else /* Make sure also to remove the actual requested directory not just the content */
    {
        result = CsrDirRemove(path);
        if (result != CSR_RESULT_SUCCESS)
        {
            result = CSR_FSAL_DELETE_FAILURE;
        }
    }

    CsrFsalRemoveRecursivelyCfmSend(entry->appHandle, result, sessionId);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleRenameCfmAnswer
*
*   Description:
*
*********************************************************************************/
static void csrFsalHandleRenameCfmAnswer(CsrFsalInstanceData *pInst,
    CsrUtf8String *oldPath,
    CsrUtf8String *newPath,
    CsrFsalSession sessionId)
{
    CsrResult result;
    CsrFsalHandles *entry = CsrFsalGetSession(pInst, sessionId);

    /* Need to find out if a file or a folder is requested to be deleted    */
    result = CsrFileRename(oldPath, newPath);

    result = (result == CSR_RESULT_SUCCESS ? CSR_RESULT_SUCCESS :
              result == CSR_FILE_RESULT_NOT_EXIST ? CSR_FSAL_RENAME_NOT_EXIST :
              result == CSR_FILE_RESULT_NOT_ALLOWED ? CSR_FSAL_RENAME_NOT_ALLOWED :
              CSR_FSAL_RENAME_FAILURE);

    CsrFsalRenameCfmSend(entry->appHandle, sessionId, result);
}

/********************************************************************************
*
*   Name:
*       csrFsalPermissionsSetCfmAnswer
*
*   Description:
*
*********************************************************************************/
static void csrFsalFillInFilePermsFromFsalPerms(CsrFilePerms *perms, CsrFsalPermissions permissions)
{
    *perms = permissions & CSR_FSAL_PERMISSION_USER_READ ? CSR_FILE_PERMS_USER_READ : 0;
    *perms |= permissions & CSR_FSAL_PERMISSION_USER_WRITE ? CSR_FILE_PERMS_USER_WRITE : 0;
    *perms |= permissions & CSR_FSAL_PERMISSION_USER_EXECUTE ? CSR_FILE_PERMS_USER_EXECUTE : 0;
    *perms |= permissions & CSR_FSAL_PERMISSION_GROUP_READ ? CSR_FILE_PERMS_GROUP_READ : 0;
    *perms |= permissions & CSR_FSAL_PERMISSION_GROUP_WRITE ? CSR_FILE_PERMS_GROUP_WRITE : 0;
    *perms |= permissions & CSR_FSAL_PERMISSION_GROUP_EXECUTE ? CSR_FILE_PERMS_GROUP_EXECUTE : 0;
    *perms |= permissions & CSR_FSAL_PERMISSION_OTHERS_READ ? CSR_FILE_PERMS_OTHERS_READ : 0;
    *perms |= permissions & CSR_FSAL_PERMISSION_OTHERS_WRITE ? CSR_FILE_PERMS_OTHERS_WRITE : 0;
    *perms |= permissions & CSR_FSAL_PERMISSION_OTHERS_EXECUTE ? CSR_FILE_PERMS_OTHERS_EXECUTE : 0;
}

static void csrFsalPermissionsSetCfmAnswer(CsrFsalInstanceData *pInst,
    CsrUtf8String *path,
    CsrFsalPermissions permissions,
    CsrFsalSession sessionId)
{
    CsrResult result;
    CsrFsalHandles *entry = CsrFsalGetSession(pInst, sessionId);
    CsrFilePerms perms;

    csrFsalFillInFilePermsFromFsalPerms(&perms, permissions);

    result = CsrFileSetPerms(path, perms);

    result = (result == CSR_RESULT_SUCCESS ? CSR_RESULT_SUCCESS :
              result == CSR_FILE_RESULT_NOT_EXIST ? CSR_FSAL_SET_PERMISSIONS_NOT_EXIST :
              CSR_FSAL_SET_PERMISSIONS_FAILURE);

    CsrFsalPermissionsSetCfmSend(entry->appHandle, sessionId, result);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleDirFindFirstCfmAnswer
*
*   Description:
*       "Reception of the dirctory open confirm from the external file system"
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleDirFindFirstCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*       Please use the "CsrFsalClearHandleIndexFromSeqNum" to clear the reserved
*       handle when the open directory fails and the fucntion
*       "CsrFsalGetAppHandleFromSeqNumAndStoreFsHandle" to get the appHandle.
*
*********************************************************************************/
static void csrFsalFillInDirEntryFromFindStat(CsrFsalDirEntry *entry, CsrDirFindStat *findStat)
{
    entry->mode = findStat->mode & CSR_DIR_MODE_DIRECTORY ? CSR_FSAL_MODE_DIRECTORY : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_REGULAR_FILE ? CSR_FSAL_MODE_REGULAR_FILE : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_USER_READ_PERMISSION ? CSR_FSAL_MODE_USER_READ_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_USER_WRITE_PERMISSION ? CSR_FSAL_MODE_USER_WRITE_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_USER_EXECUTE ? CSR_FSAL_MODE_USER_EXECUTE : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_GROUP_READ_PERMISSION ? CSR_FSAL_MODE_GROUP_READ_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_GROUP_WRITE_PERMISSION ? CSR_FSAL_MODE_GROUP_WRITE_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_GROUP_EXECUTE ? CSR_FSAL_MODE_GROUP_EXECUTE : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_OTHERS_READ_PERMISSION ? CSR_FSAL_MODE_OTHERS_READ_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_OTHERS_WRITE_PERMISSION ? CSR_FSAL_MODE_OTHERS_WRITE_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_OTHERS_EXECUTE ? CSR_FSAL_MODE_OTHERS_EXECUTE : 0;

    entry->time.tm_sec = findStat->time.tm_sec;
    entry->time.tm_min = findStat->time.tm_min;
    entry->time.tm_hour = findStat->time.tm_hour;
    entry->time.tm_mday = findStat->time.tm_mday;
    entry->time.tm_mon = findStat->time.tm_mon;
    entry->time.tm_year = findStat->time.tm_year;
    entry->time.tm_wday = findStat->time.tm_wday;
    entry->time.tm_yday = findStat->time.tm_yday;
    entry->time.tm_isdst = findStat->time.tm_isdst;
    entry->time.utcTime = findStat->time.utcTime;

    entry->size = findStat->size;
}

static void csrFsalHandleDirFindFirstCfmAnswer(CsrFsalInstanceData *pInst,
    const CsrUtf8String *searchPattern,
    CsrFsalSession sessionId)
{
    CsrDirFindStat findStat;
    CsrFsalDirEntry dirEntry;
    CsrFsalHandles *entry = CsrFsalGetSession(pInst, sessionId);
    CsrDirHandle *dirHandle = CsrDirFindFirst(searchPattern, &findStat);

    if (dirHandle != NULL)
    {
        dirEntry.name = findStat.name; /* We forward the pointer and leave it to the receiver task to free it */

        csrFsalFillInDirEntryFromFindStat(&dirEntry, &findStat);

        CsrFsalDirHandleInit(&entry->dirHandles, dirHandle);

        CsrFsalDirFindFirstCfmSend(entry->appHandle, sessionId, (CsrFsalHandle) dirHandle, CSR_RESULT_SUCCESS, &dirEntry);
    }
    else
    {
        CsrFsalDirFindFirstCfmSend(entry->appHandle, sessionId, CSR_FSAL_INVALID_HANDLE, CSR_FSAL_DIR_OP_FAILURE, NULL);
    }
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleDirFindNextCfmAnswer
*
*   Description:
*       "Reception of the read directory confirm from the external file system"
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleDirFindNextCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*       Please use the "CsrFsalGetAppHandleFromFsHandle" to get the appHandle from
*       received directory handle and the function "ImUtilFreeDirectoryListing"
*       to handle the clearing of the directory entries contents.
*
*********************************************************************************/
static void csrFsalHandleDirFindNextCfmAnswer(CsrFsalInstanceData *pInst,
    CsrFsalHandle fsalHandle)
{
    CsrSchedQid appHandle;
    CsrDirFindStat findStat;
    CsrFsalDirEntry entry;
    CsrResult result = CsrDirFindNext((CsrDirHandle *) fsalHandle, &findStat);

    appHandle = CsrFsalGetAppHandleFromDirHandle(pInst, (CsrDirHandle *) fsalHandle);

    if (result == CSR_RESULT_SUCCESS)
    {
        entry.name = findStat.name; /* We forward the pointer and leave it to the receiver task to free it */

        csrFsalFillInDirEntryFromFindStat(&entry, &findStat);

        CsrFsalDirFindNextCfmSend(appHandle, fsalHandle, CSR_RESULT_SUCCESS, &entry);
    }
    else
    {
        CsrFsalDirFindNextCfmSend(appHandle, fsalHandle, CSR_FSAL_DIR_OP_NO_MORE_MATCHING_ENTRIES, NULL);
    }
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleDirFindCloseCfmAnswer
*
*   Description:
*
*********************************************************************************/
static void csrFsalHandleDirFindCloseCfmAnswer(CsrFsalInstanceData *pInst,
    CsrFsalHandle fsalHandle)
{
    CsrSchedQid appHandle;
    CsrResult result = CsrDirFindClose((CsrDirHandle *) fsalHandle);

    if (result != CSR_RESULT_SUCCESS)
    {
        result = CSR_FSAL_DIR_OP_FAILURE;
    }

    appHandle = CsrFsalGetAppHandleFromDirHandleAndClearDirHandleIndex(&pInst->handleList, (CsrDirHandle *) fsalHandle);

    CsrFsalDirFindCloseCfmSend(appHandle, fsalHandle, result);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleDirMakeCfmAnswer
*
*   Description:
*       "Reception of the make directory confirm from the external file system"
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleDirMakeCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*       If a function based interface to the file system outside the
*       scheduler is present the function "_wmkdir" shall be ported
*       to reflect the actual interface and the corresponding result codes
*       shall be ported as well.
*
*       Please use the "CsrFsalClearHandleIndexFromSeqNum" to get the appHandle from
*       received sequence number and to clear the reserved handles.
*
*       Notice when creating a new directory no checking is yet made of whether the
*       parent directory is read-only and by this the creation should fail.
*
*********************************************************************************/
static void csrFsalHandleDirMakeCfmAnswer(CsrFsalInstanceData *pInst,
    const CsrUtf8String *dirName,
    CsrFsalSession sessionId)
{
    CsrResult result;
    CsrFsalHandles *entry = CsrFsalGetSession(pInst, sessionId);

    result = CsrDirMake(dirName);

    if (result != CSR_RESULT_SUCCESS) /* Fail to create a new folder                                        */
    {
        result = (result == CSR_DIR_RESULT_NOT_EXIST ? CSR_FSAL_DIR_MAKE_INVALID_PATH :
                  result == CSR_DIR_RESULT_ALREADY_EXIST ? CSR_FSAL_DIR_MAKE_EXIST :
                  CSR_FSAL_DIR_MAKE_FAILURE);
    }

    CsrFsalDirMakeCfmSend(entry->appHandle, sessionId, result);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleDirChangeCfmAnswer
*
*   Description:
*       "Reception of the change directory confirm from the external file system"
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleDirMakeCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*********************************************************************************/
static void csrFsalHandleDirChangeCfmAnswer(CsrFsalInstanceData *pInst,
    const CsrUtf8String *path,
    CsrFsalSession sessionId)
{
    CsrResult result;
    CsrFsalHandles *entry = CsrFsalGetSession(pInst, sessionId);

    result = CsrDirChange(path);

    if (result == CSR_RESULT_SUCCESS)
    {
        CsrPmemFree(entry->currentWorkingDir);
        result = CsrDirGetCurrentWorkingDir(&entry->currentWorkingDir);

        if (result != CSR_RESULT_SUCCESS) /* Fail to change to new folder                                        */
        {
            result = CSR_FSAL_DIR_CHANGE_FAILURE;
        }
    }
    else /* Fail to change to new folder                                        */
    {
        result = (result == CSR_DIR_RESULT_NOT_EXIST ? CSR_FSAL_DIR_CHANGE_NOT_EXIST : CSR_FSAL_DIR_CHANGE_FAILURE);
    }
    CsrFsalDirChangeCfmSend(entry->appHandle, sessionId, (CsrUtf8String *) CsrStrDup((CsrCharString *) entry->currentWorkingDir), result);
}

/********************************************************************************
*
*   Name:
*       csrFsalHandleGetAttributesCfmAnswer
*
*   Description:
*       "Reception of the open confirm from the external file system"
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "csrFsalHandleFileOpenCfmAnswer" shall be changed to
*       forward the converted answer (the confirm) from external file system
*       outside the scheduler.
*
*       If a function based interface to the file system outside the
*       scheduler is present the function "_wfopen" shall be changed to reflect
*       the actual interface. The function "CsrFsalFileSize" shall be ported as well
*       to reflect the actual interface to the file system.
*
*       Please use the "CsrFsalClearHandleIndexFromSeqNum" to clear the reserved
*       handle when the open fails and the fucntion
*       "CsrFsalGetAppHandleFromSeqNumAndStoreFsHandle" to get the appHandle.
*
*       Notice when creating a new file no checking is yet made of whether the
*       directory is read-only and by this the creation should fail.
*       There is also no checking of whether the file already exists as read-only.
*
*********************************************************************************/
static void csrFsalFillInDirEntryFromEntryStat(CsrFsalDirEntry *entry, CsrDirEntryStat *findStat)
{
    entry->mode = findStat->mode & CSR_DIR_MODE_DIRECTORY ? CSR_FSAL_MODE_DIRECTORY : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_REGULAR_FILE ? CSR_FSAL_MODE_REGULAR_FILE : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_USER_READ_PERMISSION ? CSR_FSAL_MODE_USER_READ_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_USER_WRITE_PERMISSION ? CSR_FSAL_MODE_USER_WRITE_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_USER_EXECUTE ? CSR_FSAL_MODE_USER_EXECUTE : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_GROUP_READ_PERMISSION ? CSR_FSAL_MODE_GROUP_READ_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_GROUP_WRITE_PERMISSION ? CSR_FSAL_MODE_GROUP_WRITE_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_GROUP_EXECUTE ? CSR_FSAL_MODE_GROUP_EXECUTE : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_OTHERS_READ_PERMISSION ? CSR_FSAL_MODE_OTHERS_READ_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_OTHERS_WRITE_PERMISSION ? CSR_FSAL_MODE_OTHERS_WRITE_PERMISSION : 0;
    entry->mode |= findStat->mode & CSR_DIR_MODE_OTHERS_EXECUTE ? CSR_FSAL_MODE_OTHERS_EXECUTE : 0;

    entry->time.tm_sec = findStat->time.tm_sec;
    entry->time.tm_min = findStat->time.tm_min;
    entry->time.tm_hour = findStat->time.tm_hour;
    entry->time.tm_mday = findStat->time.tm_mday;
    entry->time.tm_mon = findStat->time.tm_mon;
    entry->time.tm_year = findStat->time.tm_year;
    entry->time.tm_wday = findStat->time.tm_wday;
    entry->time.tm_yday = findStat->time.tm_yday;
    entry->time.tm_isdst = findStat->time.tm_isdst;
    entry->time.utcTime = findStat->time.utcTime;

    entry->size = findStat->size;
}

static void csrFsalHandleStatCfmAnswer(CsrFsalInstanceData *pInst,
    const CsrUtf8String *name,
    CsrFsalSession sessionId)
{
    CsrResult result = CSR_FSAL_STAT_FAILURE;
    CsrFsalDirEntry statEntry;
    CsrDirEntryStat findStat;
    CsrFsalHandles *entry = CsrFsalGetSession(pInst, sessionId);

    result = CsrDirStat(name, &findStat);

    if (result == CSR_RESULT_SUCCESS)
    {
        statEntry.name = (CsrUtf8String *) CsrStrDup((CsrCharString *) name);

        csrFsalFillInDirEntryFromEntryStat(&statEntry, &findStat);

        CsrFsalStatCfmSend(entry->appHandle, sessionId, result, &statEntry);
    }
    else
    {
        CsrFsalStatCfmSend(entry->appHandle, sessionId, (CsrResult) (result == CSR_DIR_RESULT_NOT_EXIST ? CSR_FSAL_STAT_NOT_EXIST : CSR_FSAL_STAT_FAILURE), NULL);
    }
}

/********************************************************************************
************Start of the handler functions which handles ************************
************messages received from the scheduler tasks **************************
********************************************************************************/

void CsrFsalSessionCreateReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalSessionCreateReq *req = (CsrFsalSessionCreateReq *) pInst->recvMsgP;
    CsrFsalHandles *entry = CsrFsalReserveNewSessionId(pInst, req->appHandle);

    CsrFsalSessionCreateCfmSend(req->appHandle, entry->sessionId, (CsrUtf8String *) CsrStrDup((CsrCharString *) entry->currentWorkingDir), CSR_RESULT_SUCCESS);
}

void CsrFsalSessionDestroyReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalSessionDestroyReq *req = (CsrFsalSessionDestroyReq *) pInst->recvMsgP;
    CsrSchedQid appHandle = CsrFsalClearHandleIndexFromSessionId(pInst, req->sessionId);

    CsrFsalSessionDestroyCfmSend(appHandle, req->sessionId, CSR_RESULT_SUCCESS);
}

/********************************************************************************
*
*   Name:
*       CsrFsalFileOpenReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting a
*       specific file to be opened.
*       It is up to the file system below to handle multiple access to the same
*       file. If this is not supported then the file system below should report
*       error to any subsequent FileOpenReq messages.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function as the porting
*       shall be made in the "csrFsalHandleFileOpenCfmAnswer" function. However,
*       the function "CsrFsalLookupMode" shall still be modified to reflect
*       the interface to the file system.
*
*       Please continue to use the function "CsrFsalReserveNewHandleIndex" to assign
*       and reserve indexes.
*
*********************************************************************************/
void CsrFsalFileOpenReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalFileOpenReq *req = (CsrFsalFileOpenReq *) pInst->recvMsgP;

    csrFsalHandleFileOpenCfmAnswer(pInst, req->fileName, req->flags, req->permissions, req->sessionId);
}

/********************************************************************************
*
*   Name:
*       CsrFsalFileCloseReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting
*       a specific file to be closed.
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "CsrFsalFileCloseReqHandler" shall be changed to
*       forward the request to the file system.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function and no
*       additional changes shall be made as this function does not require
*       an answer from the "external" file system.
*
*       Please continue to use the function "CsrFsalClearHandleIndexFromFsHandle"
*       to clear the reserved indexes.
*
*********************************************************************************/
void CsrFsalFileCloseReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalFileCloseReq *req = (CsrFsalFileCloseReq *) pInst->recvMsgP;

    csrFsalHandleFileCloseCfmAnswer(pInst, req->handle);
}

/********************************************************************************
*
*   Name:
*       CsrFsalFileSeekReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting a
*       seek in a already opened file.
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "CsrFsalFileSeekReqHandler" shall be changed to
*       forward the request to the file system.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function as the porting
*       shall be made in the "csrFsalHandleFileSeekCfmAnswer" function.
*
*********************************************************************************/
void CsrFsalFileSeekReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalFileSeekReq *req = (CsrFsalFileSeekReq *) pInst->recvMsgP;
    CsrInt32 origin = 0;

    switch (req->origin)
    {
        case CSR_FSAL_SEEK_SET:
        { /* the beginning of the file */
            origin = CSR_SEEK_SET; /* in Windows the beginning of the file is the define SEEK_SET*/
            break;
        }

        case CSR_FSAL_SEEK_CUR:
        { /* the current position of the file system */
            origin = CSR_SEEK_CUR; /* in Windows the current position of the file is the define SEEK_CUR*/
            break;
        }

        case CSR_FSAL_SEEK_END:
        { /* the end of the file */
            origin = CSR_SEEK_END; /* in Windows the end of the file is the define SEEK_END*/
            break;
        }

        default:
        {
            /*if anything else that origin is needed at some time it shall be included here*/
            CSR_LOG_TEXT_WARNING((CsrFsalLto, 0,
                                  "CSR_FSAL seek set to a value that is unknown: %u", req->origin));
        }
    }

    csrFsalHandleFileSeekCfmAnswer(pInst, req->handle, req->offset, origin);
}

/********************************************************************************
*
*   Name:
*       CsrFsalFileReadReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting a
*       read in a already opened file.
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "CsrFsalFileReadReqHandler" shall be changed to
*       forward the request to the file system.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function as the porting
*       shall be made in the "csrFsalHandleFileReadCfmAnswer" function.
*
*********************************************************************************/
void CsrFsalFileReadReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalFileReadReq *req = (CsrFsalFileReadReq *) pInst->recvMsgP;

    CSR_LOG_TEXT_CONDITIONAL_WARNING(req->bytesToRead > CSR_FSAL_MAX_DATA_SIZE, (CsrFsalLto, 0,
                                                                                 "Read request exceeded CSR_FSAL_MAX_DATA_SIZE bytes"));

    csrFsalHandleFileReadCfmAnswer(pInst, req->handle, req->bytesToRead);
}

/********************************************************************************
*
*   Name:
*       CsrFsalFileWriteReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting a
*       write in a already opened file.
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "CsrFsalFileWriteReqHandler" shall be changed to
*       forward the request to the file system.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function as the porting
*       shall be made in the "csrFsalHandleFileWriteCfmAnswer" function.
*
*********************************************************************************/
void CsrFsalFileWriteReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalFileWriteReq *req = (CsrFsalFileWriteReq *) pInst->recvMsgP;

    CSR_LOG_TEXT_CONDITIONAL_WARNING(req->dataLen > CSR_FSAL_MAX_DATA_SIZE, (CsrFsalLto, 0,
                                                                             "Write request exceeded CSR_FSAL_MAX_DATA_SIZE bytes"));

    csrFsalHandleFileWriteCfmAnswer(pInst, req->handle, req->dataLen, req->data);
}

/********************************************************************************
*
*   Name:
*       CsrFsalFileTellReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting a
*       tell in a already opened file.
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "CsrFsalFileTellReqHandler" shall be changed to
*       forward the request to the file system.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function as the porting
*       shall be made in the "csrFsalHandleFileTellCfmAnswer" function.
*
*********************************************************************************/
void CsrFsalFileTellReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalFileTellReq *req = (CsrFsalFileTellReq *) pInst->recvMsgP;

    csrFsalHandleFileTellCfmAnswer(pInst, req->handle);
}

/********************************************************************************
*
*   Name:
*       CsrFsalStatReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting a
*       specific file to be opened.
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "CsrFsalFileOpenReqHandler" shall be changed to
*       forward the request to the file system and the function "CsrFsalLookupMode"
*       shall be modified to reflect the interface to the file system.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function as the porting
*       shall be made in the "csrFsalHandleFileOpenCfmAnswer" function. However,
*       the function "CsrFsalLookupMode" shall still be modified to reflect
*       the interface to the file system.
*
*       Please continue to use the function "CsrFsalReserveNewHandleIndex" to assign
*       and reserve indexes.
*
*********************************************************************************/
void CsrFsalStatReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalStatReq *req = (CsrFsalStatReq *) pInst->recvMsgP;

    csrFsalHandleStatCfmAnswer(pInst, req->path, req->sessionId);
}

/********************************************************************************
*
*   Name:
*       CsrFsalRemoveReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting a
*       file or folder needs to be removed in a already.
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "CsrFsalRemoveReqHandler" shall be changed to
*       forward the request to the file system.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function as the porting
*       shall be made in the "csrFsalHandleRemoveCfmAnswer" function.
*
*       Please continue to use the function "CsrFsalReserveNewHandleIndex" to assign
*       and reserve indexes.
*
*********************************************************************************/
void CsrFsalRemoveReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalRemoveReq *req = (CsrFsalRemoveReq *) pInst->recvMsgP;

    csrFsalHandleRemoveCfmAnswer(pInst, req->path, req->sessionId);
}

void CsrFsalRemoveRecursivelyReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalRemoveRecursivelyReq *req = (CsrFsalRemoveRecursivelyReq *) pInst->recvMsgP;

    csrFsalHandleRemoveRecursivelyCfmAnswer(pInst, req->dir, req->sessionId);
}

void CsrFsalRenameReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalRenameReq *req = (CsrFsalRenameReq *) pInst->recvMsgP;

    csrFsalHandleRenameCfmAnswer(pInst, req->oldPath, req->newPath, req->sessionId);
}

void CsrFsalPermissionsSetReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalPermissionsSetReq *req = (CsrFsalPermissionsSetReq *) pInst->recvMsgP;

    csrFsalPermissionsSetCfmAnswer(pInst, req->path, req->permissions, req->sessionId);
}

/********************************************************************************
*
*   Name:
*       CsrFsalDirMakeReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting to create
*       a new directory.
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "CsrFsalDirMakeReqHandler" shall be changed to
*       forward the request to the file system.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function as the porting
*       shall be made in the "csrFsalHandleDirMakeCfmAnswer" function.
*
*       Please continue to use the function "CsrFsalReserveNewHandleIndex" to assign
*       and reserve indexes.
*
*********************************************************************************/
void CsrFsalDirMakeReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalDirMakeReq *req = (CsrFsalDirMakeReq *) pInst->recvMsgP;

    csrFsalHandleDirMakeCfmAnswer(pInst, req->dirName, req->sessionId);
}

void CsrFsalDirChangeReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalDirChangeReq *req = (CsrFsalDirChangeReq *) pInst->recvMsgP;

    csrFsalHandleDirChangeCfmAnswer(pInst, req->path, req->sessionId);
}

/********************************************************************************
*
*   Name:
*       CsrFsalDirFindFirstReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting a
*       directory to be opened.
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "CsrFsalDirOpenReqHandler" shall be changed to
*       forward the request to the file system.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function as the porting
*       shall be made in the "csrFsalHandleDirFindFirstCfmAnswer" function.
*
*       Please continue to use the function "CsrFsalReserveNewHandleIndex" to assign
*       and reserve indexes.
*
*********************************************************************************/
void CsrFsalDirFindFirstReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalDirFindFirstReq *req = (CsrFsalDirFindFirstReq *) pInst->recvMsgP;

    csrFsalHandleDirFindFirstCfmAnswer(pInst, req->searchPattern, req->sessionId);
}

/********************************************************************************
*
*   Name:
*       CsrFsalDirFindNextReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting a
*       an open directory to be read.
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "CsrFsalDirReadReqHandler" shall be changed to
*       forward the request to the file system.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function as the porting
*       shall be made in the "csrFsalHandleDirFindNextCfmAnswer" function.
*
*********************************************************************************/
void CsrFsalDirFindNextReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalDirFindNextReq *req = (CsrFsalDirFindNextReq *) pInst->recvMsgP;

    csrFsalHandleDirFindNextCfmAnswer(pInst, req->handle);
}

/********************************************************************************
*
*   Name:
*       CsrFsalDirFindCloseReqHandler
*
*   Description:
*       Handler function
*       This function is called when a message is received requesting
*       a specific directory to be closed.
*
*       If a message based interface to the file system outside the scheduler
*       is present the function "CsrFsalDirCloseReqHandler" shall be changed to
*       forward the request to the file system.
*
*       If a function based interface to the file system outside the
*       scheduler is present no changes is needed in this function but the
*       function "CsrFsalCloseDir" shall be changed to reflect the interface to the
*       "external" file system.
*
*       Please continue to use the function "CsrFsalClearHandleIndexFromFsHandle"
*       to clear the reserved indexes.
*
*********************************************************************************/
void CsrFsalDirFindCloseReqHandler(CsrFsalInstanceData *pInst)
{
    CsrFsalDirFindCloseReq *req = (CsrFsalDirFindCloseReq *) pInst->recvMsgP;

    csrFsalHandleDirFindCloseCfmAnswer(pInst, req->handle);
}
