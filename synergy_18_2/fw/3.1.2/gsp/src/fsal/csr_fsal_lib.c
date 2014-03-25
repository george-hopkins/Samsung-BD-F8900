/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_fsal_prim.h"
#include "csr_pmem.h"
#include "csr_fsal_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalSessionCreateReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalSessionCreateReq *CsrFsalSessionCreateReq_struct(CsrSchedQid appHandle)
{
    CsrFsalSessionCreateReq *prim;
    prim = (CsrFsalSessionCreateReq *) CsrPmemAlloc(sizeof(CsrFsalSessionCreateReq));
    prim->type = CSR_FSAL_SESSION_CREATE_REQ;
    prim->appHandle = appHandle;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalSessionDestroyReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalSessionDestroyReq *CsrFsalSessionDestroyReq_struct(CsrFsalSession sessionId)
{
    CsrFsalSessionDestroyReq *prim;
    prim = (CsrFsalSessionDestroyReq *) CsrPmemAlloc(sizeof(CsrFsalSessionDestroyReq));
    prim->type = CSR_FSAL_SESSION_DESTROY_REQ;
    prim->sessionId = sessionId;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileOpenReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalFileOpenReq *CsrFsalFileOpenReq_struct(
    CsrFsalSession sessionId,
    CsrUtf8String *fileName,
    CsrFsalFileOpenFlags flags,
    CsrFsalFileOpenPerms perms)
{
    CsrFsalFileOpenReq *prim;
    prim = (CsrFsalFileOpenReq *) CsrPmemAlloc(sizeof(CsrFsalFileOpenReq));
    prim->type = CSR_FSAL_FILE_OPEN_REQ;
    prim->sessionId = sessionId;
    prim->fileName = fileName;
    prim->flags = flags;
    prim->permissions = perms;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileCloseReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalFileCloseReq *CsrFsalFileCloseReq_struct(CsrFsalHandle handle)
{
    CsrFsalFileCloseReq *prim;

    prim = (CsrFsalFileCloseReq *) CsrPmemAlloc(sizeof(CsrFsalFileCloseReq));
    prim->type = CSR_FSAL_FILE_CLOSE_REQ;
    prim->handle = handle;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileSeekReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalFileSeekReq *CsrFsalFileSeekReq_struct(CsrFsalHandle handle,
    CsrInt32 offset,
    CsrFsalOrigin origin)
{
    CsrFsalFileSeekReq *prim;

    prim = (CsrFsalFileSeekReq *) CsrPmemAlloc(sizeof(CsrFsalFileSeekReq));
    prim->type = CSR_FSAL_FILE_SEEK_REQ;
    prim->handle = handle;
    prim->offset = offset;
    prim->origin = origin;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileReadReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalFileReadReq *CsrFsalFileReadReq_struct(CsrFsalHandle handle,
    CsrUint32 bytesToRead)
{
    CsrFsalFileReadReq *prim;

    prim = (CsrFsalFileReadReq *) CsrPmemAlloc(sizeof(CsrFsalFileReadReq));
    prim->type = CSR_FSAL_FILE_READ_REQ;
    prim->handle = handle;
    prim->bytesToRead = bytesToRead;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileWriteReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalFileWriteReq *CsrFsalFileWriteReq_struct(CsrFsalHandle handle,
    CsrUint32 dataLen,
    CsrUint8 *data)
{
    CsrFsalFileWriteReq *prim;

    prim = (CsrFsalFileWriteReq *) CsrPmemAlloc(sizeof(CsrFsalFileWriteReq));
    prim->type = CSR_FSAL_FILE_WRITE_REQ;
    prim->handle = handle;
    prim->dataLen = dataLen;
    prim->data = data;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileTellReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalFileTellReq *CsrFsalFileTellReq_struct(CsrFsalHandle handle)
{
    CsrFsalFileTellReq *prim;

    prim = (CsrFsalFileTellReq *) CsrPmemAlloc(sizeof(CsrFsalFileTellReq));
    prim->type = CSR_FSAL_FILE_TELL_REQ;
    prim->handle = handle;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalStatReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalStatReq *CsrFsalStatReq_struct(
    CsrFsalSession sessionId,
    CsrUtf8String *path)
{
    CsrFsalStatReq *prim;
    prim = (CsrFsalStatReq *) CsrPmemAlloc(sizeof(CsrFsalStatReq));
    prim->type = CSR_FSAL_STAT_REQ;
    prim->sessionId = sessionId;
    prim->path = path;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalRemoveReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalRemoveReq *CsrFsalRemoveReq_struct(
    CsrFsalSession sessionId,
    CsrUtf8String *path)
{
    CsrFsalRemoveReq *prim;

    prim = (CsrFsalRemoveReq *) CsrPmemAlloc(sizeof(CsrFsalRemoveReq));
    prim->type = CSR_FSAL_REMOVE_REQ;
    prim->sessionId = sessionId;
    prim->path = path;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalRemoveRecursivelyReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalRemoveRecursivelyReq *CsrFsalRemoveRecursivelyReq_struct(
    CsrFsalSession sessionId,
    CsrUtf8String *dir)
{
    CsrFsalRemoveRecursivelyReq *prim;

    prim = (CsrFsalRemoveRecursivelyReq *) CsrPmemAlloc(sizeof(CsrFsalRemoveRecursivelyReq));
    prim->type = CSR_FSAL_REMOVE_RECURSIVELY_REQ;
    prim->sessionId = sessionId;
    prim->dir = dir;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalRenameReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalRenameReq *CsrFsalRenameReq_struct(
    CsrFsalSession sessionId,
    CsrUtf8String *oldPath,
    CsrUtf8String *newPath)
{
    CsrFsalRenameReq *prim;

    prim = (CsrFsalRenameReq *) CsrPmemAlloc(sizeof(CsrFsalRenameReq));
    prim->type = CSR_FSAL_RENAME_REQ;
    prim->sessionId = sessionId;
    prim->oldPath = oldPath;
    prim->newPath = newPath;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalPermissionsSetReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalPermissionsSetReq *CsrFsalPermissionsSetReq_struct(
    CsrFsalSession sessionId,
    CsrUtf8String *path,
    CsrFsalPermissions permissions)
{
    CsrFsalPermissionsSetReq *prim;

    prim = (CsrFsalPermissionsSetReq *) CsrPmemAlloc(sizeof(CsrFsalPermissionsSetReq));
    prim->type = CSR_FSAL_PERMISSIONS_SET_REQ;
    prim->sessionId = sessionId;
    prim->path = path;
    prim->permissions = permissions;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalDirMakeReqSend
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalDirMakeReq *CsrFsalDirMakeReq_struct(
    CsrFsalSession sessionId,
    CsrUtf8String *dirName)
{
    CsrFsalDirMakeReq *prim;

    prim = (CsrFsalDirMakeReq *) CsrPmemAlloc(sizeof(CsrFsalDirMakeReq));
    prim->type = CSR_FSAL_DIR_MAKE_REQ;
    prim->sessionId = sessionId;
    prim->dirName = dirName;
    return prim;
}

CsrFsalDirChangeReq *CsrFsalDirChangeReq_struct(
    CsrFsalSession sessionId,
    CsrUtf8String *path)
{
    CsrFsalDirChangeReq *prim;

    prim = (CsrFsalDirChangeReq *) CsrPmemAlloc(sizeof(CsrFsalDirChangeReq));
    prim->type = CSR_FSAL_DIR_CHANGE_REQ;
    prim->sessionId = sessionId;
    prim->path = path;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalDirFindFirstReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalDirFindFirstReq *CsrFsalDirFindFirstReq_struct(
    CsrFsalSession sessionId,
    CsrUtf8String *searchPattern)
{
    CsrFsalDirFindFirstReq *prim;

    prim = (CsrFsalDirFindFirstReq *) CsrPmemAlloc(sizeof(CsrFsalDirFindFirstReq));
    prim->type = CSR_FSAL_DIR_FIND_FIRST_REQ;
    prim->sessionId = sessionId;
    prim->searchPattern = searchPattern;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalDirFindNextReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalDirFindNextReq *CsrFsalDirFindNextReq_struct(CsrFsalHandle handle)
{
    CsrFsalDirFindNextReq *prim;

    prim = (CsrFsalDirFindNextReq *) CsrPmemAlloc(sizeof(CsrFsalDirFindNextReq));
    prim->type = CSR_FSAL_DIR_FIND_NEXT_REQ;
    prim->handle = handle;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalDirFindCloseReq
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalDirFindCloseReq *CsrFsalDirFindCloseReq_struct(CsrFsalHandle handle)
{
    CsrFsalDirFindCloseReq *prim;

    prim = (CsrFsalDirFindCloseReq *) CsrPmemAlloc(sizeof(CsrFsalDirFindCloseReq));
    prim->type = CSR_FSAL_DIR_FIND_CLOSE_REQ;
    prim->handle = handle;
    return prim;
}

#ifdef __cplusplus
}
#endif
