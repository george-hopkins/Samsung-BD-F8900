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
 *      CsrFsalSessionCreateCfm
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalSessionCreateCfm *CsrFsalSessionCreateCfm_struct(
    CsrFsalSession sessionId,
    CsrUtf8String *currentWorkingDir,
    CsrResult result)
{
    CsrFsalSessionCreateCfm *prim;
    prim = (CsrFsalSessionCreateCfm *) CsrPmemAlloc(sizeof(CsrFsalSessionCreateCfm));
    prim->type = CSR_FSAL_SESSION_CREATE_CFM;
    prim->sessionId = sessionId;
    prim->currentWorkingDir = currentWorkingDir;
    prim->result = result;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalSessionDestroyCfm
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalSessionDestroyCfm *CsrFsalSessionDestroyCfm_struct(
    CsrFsalSession sessionId,
    CsrResult result)
{
    CsrFsalSessionDestroyCfm *prim;
    prim = (CsrFsalSessionDestroyCfm *) CsrPmemAlloc(sizeof(CsrFsalSessionDestroyCfm));
    prim->type = CSR_FSAL_SESSION_DESTROY_CFM;
    prim->sessionId = sessionId;
    prim->result = result;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileOpenCfmSend
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      handle:         ...
 *      result:              ...
 *      filesize:              ...
 *----------------------------------------------------------------------------*/
CsrFsalFileOpenCfm *CsrFsalFileOpenCfmSend_struct(
    CsrFsalSession sessionId,
    CsrFsalHandle handle,
    CsrResult result,
    CsrUint32 size)
{
    CsrFsalFileOpenCfm *prim;

    prim = (CsrFsalFileOpenCfm *) CsrPmemAlloc(sizeof(CsrFsalFileOpenCfm));
    prim->type = CSR_FSAL_FILE_OPEN_CFM;
    prim->sessionId = sessionId;
    prim->handle = handle;
    prim->result = result;
    prim->size = size;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileCloseCfmSend
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFsalFileCloseCfm *CsrFsalFileCloseCfmSend_struct(
    CsrFsalHandle handle,
    CsrResult result)
{
    CsrFsalFileCloseCfm *prim;

    prim = (CsrFsalFileCloseCfm *) CsrPmemAlloc(sizeof(CsrFsalFileCloseCfm));
    prim->type = CSR_FSAL_FILE_CLOSE_CFM;
    prim->handle = handle;
    prim->result = result;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileSeekCfmSend
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      handle:         ...
 *      result:              ...
 *----------------------------------------------------------------------------*/
CsrFsalFileSeekCfm *CsrFsalFileSeekCfmSend_struct(
    CsrFsalHandle handle,
    CsrResult result)
{
    CsrFsalFileSeekCfm *prim;

    prim = (CsrFsalFileSeekCfm *) CsrPmemAlloc(sizeof(CsrFsalFileSeekCfm));
    prim->type = CSR_FSAL_FILE_SEEK_CFM;
    prim->handle = handle;
    prim->result = result;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileReadCfmSend
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      handle:       ...
 *      result:           ...
 *      count:            ...
 *      dataLen:          ...
 *      data:             ...
 *----------------------------------------------------------------------------*/
CsrFsalFileReadCfm *CsrFsalFileReadCfmSend_struct(
    CsrFsalHandle handle,
    CsrResult result,
    CsrUint32 dataLen,
    CsrUint8 *data)
{
    CsrFsalFileReadCfm *prim;

    prim = (CsrFsalFileReadCfm *) CsrPmemAlloc(sizeof(CsrFsalFileReadCfm));
    prim->type = CSR_FSAL_FILE_READ_CFM;
    prim->handle = handle;
    prim->result = result;
    prim->dataLen = dataLen;
    prim->data = data;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileWriteCfmSend
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      handle:         ...
 *      result:               ...
 *      count:                ...
 *----------------------------------------------------------------------------*/
CsrFsalFileWriteCfm *CsrFsalFileWriteCfmSend_struct(
    CsrFsalHandle handle,
    CsrResult result,
    CsrUint32 bytesWritten)
{
    CsrFsalFileWriteCfm *prim;

    prim = (CsrFsalFileWriteCfm *) CsrPmemAlloc(sizeof(CsrFsalFileWriteCfm));
    prim->type = CSR_FSAL_FILE_WRITE_CFM;
    prim->handle = handle;
    prim->result = result;
    prim->bytesWritten = bytesWritten;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileTellCfmSend
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      handle:         ...
 *      result:         ...
 *----------------------------------------------------------------------------*/
CsrFsalFileTellCfm *CsrFsalFileTellCfmSend_struct(
    CsrFsalHandle handle,
    CsrUint32 position,
    CsrResult result)
{
    CsrFsalFileTellCfm *prim;

    prim = (CsrFsalFileTellCfm *) CsrPmemAlloc(sizeof(CsrFsalFileTellCfm));
    prim->type = CSR_FSAL_FILE_TELL_CFM;
    prim->handle = handle;
    prim->position = position;
    prim->result = result;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalStatCfmSend
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      attribute:         ...
 *      result:            ...
 *      seqNum:            ...
 *----------------------------------------------------------------------------*/
CsrFsalStatCfm *CsrFsalStatCfmSend_struct(
    CsrFsalSession sessionId,
    CsrResult result,
    CsrFsalDirEntry *stat)
{
    CsrFsalStatCfm *prim;

    prim = (CsrFsalStatCfm *) CsrPmemAlloc(sizeof(CsrFsalStatCfm));
    prim->type = CSR_FSAL_STAT_CFM;
    prim->result = result;
    if (stat)
    {
        prim->stat = *stat;
    }
    else
    {
        CsrMemSet(&prim->stat, 0, sizeof(prim->stat));
    }

    prim->sessionId = sessionId;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalRemoveCfmSend
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      result:              ...
 *      seqNum:
 *----------------------------------------------------------------------------*/
CsrFsalRemoveCfm *CsrFsalRemoveCfmSend_struct(
    CsrResult result,
    CsrFsalSession sessionId)
{
    CsrFsalRemoveCfm *prim;

    prim = (CsrFsalRemoveCfm *) CsrPmemAlloc(sizeof(CsrFsalRemoveCfm));
    prim->type = CSR_FSAL_REMOVE_CFM;
    prim->result = result;
    prim->sessionId = sessionId;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalRemoveRecursivelyCfmSend
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      result:              ...
 *      seqNum:
 *----------------------------------------------------------------------------*/
CsrFsalRemoveRecursivelyCfm *CsrFsalRemoveRecursivelyCfmSend_struct(
    CsrResult result,
    CsrFsalSession sessionId)
{
    CsrFsalRemoveRecursivelyCfm *prim;

    prim = (CsrFsalRemoveRecursivelyCfm *) CsrPmemAlloc(sizeof(CsrFsalRemoveRecursivelyCfm));
    prim->type = CSR_FSAL_REMOVE_RECURSIVELY_CFM;
    prim->result = result;
    prim->sessionId = sessionId;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalRenameCfm
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      handle:         ...
 *      result:         ...
 *----------------------------------------------------------------------------*/
CsrFsalRenameCfm *CsrFsalRenameCfmSend_struct(
    CsrFsalSession sessionId,
    CsrResult result)
{
    CsrFsalRenameCfm *prim;

    prim = (CsrFsalRenameCfm *) CsrPmemAlloc(sizeof(CsrFsalRenameCfm));
    prim->type = CSR_FSAL_RENAME_CFM;
    prim->result = result;
    prim->sessionId = sessionId;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalPermissionsSetCfm
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      handle:         ...
 *      result:         ...
 *----------------------------------------------------------------------------*/
CsrFsalPermissionsSetCfm *CsrFsalPermissionsSetCfmSend_struct(
    CsrFsalSession sessionId,
    CsrResult result)
{
    CsrFsalPermissionsSetCfm *prim;

    prim = (CsrFsalPermissionsSetCfm *) CsrPmemAlloc(sizeof(CsrFsalPermissionsSetCfm));
    prim->type = CSR_FSAL_PERMISSIONS_SET_CFM;
    prim->result = result;
    prim->sessionId = sessionId;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalDirMakeCfmSend
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      result:
 *      seqNum:
 *----------------------------------------------------------------------------*/
CsrFsalDirMakeCfm *CsrFsalDirMakeCfmSend_struct(
    CsrFsalSession sessionId,
    CsrResult result)
{
    CsrFsalDirMakeCfm *prim;

    prim = (CsrFsalDirMakeCfm *) CsrPmemAlloc(sizeof(CsrFsalDirMakeCfm));
    prim->type = CSR_FSAL_DIR_MAKE_CFM;
    prim->result = result;
    prim->sessionId = sessionId;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalDirChangeCfm
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      result:
 *      seqNum:
 *----------------------------------------------------------------------------*/
CsrFsalDirChangeCfm *CsrFsalDirChangeCfmSend_struct(
    CsrFsalSession sessionId,
    CsrUtf8String *currentWorkingDir,
    CsrResult result)
{
    CsrFsalDirChangeCfm *prim;

    prim = (CsrFsalDirChangeCfm *) CsrPmemAlloc(sizeof(CsrFsalDirChangeCfm));
    prim->type = CSR_FSAL_DIR_CHANGE_CFM;
    prim->result = result;
    prim->currentWorkingDir = currentWorkingDir;
    prim->sessionId = sessionId;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalDirFindFirstCfmSend
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      handle:
 *      result:
 *      entry:
 *      seqNum:
 *----------------------------------------------------------------------------*/
CsrFsalDirFindFirstCfm *CsrFsalDirFindFirstCfmSend_struct(
    CsrFsalSession sessionId,
    CsrFsalHandle handle,
    CsrUint8 result,
    CsrFsalDirEntry *entry)
{
    CsrFsalDirFindFirstCfm *prim;

    prim = (CsrFsalDirFindFirstCfm *) CsrPmemAlloc(sizeof(CsrFsalDirFindFirstCfm));
    prim->type = CSR_FSAL_DIR_FIND_FIRST_CFM;
    prim->handle = handle;
    prim->result = result;
    if (entry)
    {
        prim->entry = *entry;
    }
    else
    {
        CsrMemSet(&prim->entry, 0, sizeof(prim->entry));
    }
    prim->sessionId = sessionId;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalDirFindNextCfm
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      handle:
 *      result:
 *      entries:
 *      list:
 *----------------------------------------------------------------------------*/
CsrFsalDirFindNextCfm *CsrFsalDirFindNextCfmSend_struct(
    CsrFsalHandle handle,
    CsrResult result,
    CsrFsalDirEntry *entry)
{
    CsrFsalDirFindNextCfm *prim;

    prim = (CsrFsalDirFindNextCfm *) CsrPmemAlloc(sizeof(CsrFsalDirFindNextCfm));
    prim->type = CSR_FSAL_DIR_FIND_NEXT_CFM;
    prim->handle = handle;
    prim->result = result;

    if (entry)
    {
        prim->entry = *entry;
    }
    else
    {
        CsrMemSet(&prim->entry, 0, sizeof(prim->entry));
    }

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalDirFindCloseCfm
 *
 *  DESCRIPTION
 *
 *
 *  PARAMETERS
 *      appHandle:
 *      handle:
 *      result:
 *----------------------------------------------------------------------------*/
CsrFsalDirFindCloseCfm *CsrFsalDirFindCloseCfmSend_struct(
    CsrFsalHandle handle,
    CsrResult result)
{
    CsrFsalDirFindCloseCfm *prim;

    prim = (CsrFsalDirFindCloseCfm *) CsrPmemAlloc(sizeof(CsrFsalDirFindCloseCfm));
    prim->type = CSR_FSAL_DIR_FIND_CLOSE_CFM;
    prim->handle = handle;
    prim->result = result;

    return prim;
}

#ifdef __cplusplus
}
#endif
