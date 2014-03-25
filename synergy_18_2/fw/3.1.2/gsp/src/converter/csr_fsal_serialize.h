#ifndef CSR_FSAL_SERIALIZE_H__
#define CSR_FSAL_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrFsalPfree(void *ptr);

#ifndef EXCLUDE_CSR_FSAL_SESSION_DESTROY_CFM
CsrSize CsrFsalSessionDestroyCfmSizeof(void *msg);
CsrUint8 *CsrFsalSessionDestroyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalSessionDestroyCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrFsalSessionDestroyCfmSerFree CsrFsalPfree
#endif /* EXCLUDE_CSR_FSAL_SESSION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_FSAL_RENAME_CFM
CsrSize CsrFsalRenameCfmSizeof(void *msg);
CsrUint8 *CsrFsalRenameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalRenameCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrFsalRenameCfmSerFree CsrFsalPfree
#endif /* EXCLUDE_CSR_FSAL_RENAME_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM
CsrSize CsrFsalDirFindCloseCfmSizeof(void *msg);
CsrUint8 *CsrFsalDirFindCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalDirFindCloseCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalDirFindCloseCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_CFM
CsrSize CsrFsalFileOpenCfmSizeof(void *msg);
CsrUint8 *CsrFsalFileOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileOpenCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileOpenCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_CFM */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ
CsrSize CsrFsalRemoveRecursivelyReqSizeof(void *msg);
CsrUint8 *CsrFsalRemoveRecursivelyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalRemoveRecursivelyReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalRemoveRecursivelyReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_REQ
CsrSize CsrFsalDirMakeReqSizeof(void *msg);
CsrUint8 *CsrFsalDirMakeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalDirMakeReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalDirMakeReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_REQ */

#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_REQ
CsrSize CsrFsalSessionCreateReqSizeof(void *msg);
CsrUint8 *CsrFsalSessionCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalSessionCreateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrFsalSessionCreateReqSerFree CsrFsalPfree
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_REQ */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_CFM
CsrSize CsrFsalRemoveCfmSizeof(void *msg);
CsrUint8 *CsrFsalRemoveCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalRemoveCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrFsalRemoveCfmSerFree CsrFsalPfree
#endif /* EXCLUDE_CSR_FSAL_REMOVE_CFM */

#ifndef EXCLUDE_CSR_FSAL_STAT_REQ
CsrSize CsrFsalStatReqSizeof(void *msg);
CsrUint8 *CsrFsalStatReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalStatReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalStatReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_STAT_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM
CsrSize CsrFsalDirFindFirstCfmSizeof(void *msg);
CsrUint8 *CsrFsalDirFindFirstCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalDirFindFirstCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalDirFindFirstCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM
CsrSize CsrFsalDirFindNextCfmSizeof(void *msg);
CsrUint8 *CsrFsalDirFindNextCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalDirFindNextCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalDirFindNextCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_REQ
CsrSize CsrFsalFileTellReqSizeof(void *msg);
CsrUint8 *CsrFsalFileTellReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileTellReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileTellReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ
CsrSize CsrFsalDirFindCloseReqSizeof(void *msg);
CsrUint8 *CsrFsalDirFindCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalDirFindCloseReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalDirFindCloseReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ */

#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM
CsrSize CsrFsalSessionCreateCfmSizeof(void *msg);
CsrUint8 *CsrFsalSessionCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalSessionCreateCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalSessionCreateCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_REQ
CsrSize CsrFsalFileOpenReqSizeof(void *msg);
CsrUint8 *CsrFsalFileOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileOpenReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileOpenReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ
CsrSize CsrFsalFileCloseReqSizeof(void *msg);
CsrUint8 *CsrFsalFileCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileCloseReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileCloseReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_CFM
CsrSize CsrFsalFileWriteCfmSizeof(void *msg);
CsrUint8 *CsrFsalFileWriteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileWriteCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileWriteCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_REQ
CsrSize CsrFsalFileWriteReqSizeof(void *msg);
CsrUint8 *CsrFsalFileWriteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileWriteReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileWriteReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ
CsrSize CsrFsalDirFindNextReqSizeof(void *msg);
CsrUint8 *CsrFsalDirFindNextReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalDirFindNextReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalDirFindNextReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ */

#ifndef EXCLUDE_CSR_FSAL_SESSION_DESTROY_REQ
CsrSize CsrFsalSessionDestroyReqSizeof(void *msg);
CsrUint8 *CsrFsalSessionDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalSessionDestroyReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrFsalSessionDestroyReqSerFree CsrFsalPfree
#endif /* EXCLUDE_CSR_FSAL_SESSION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_CFM
CsrSize CsrFsalDirMakeCfmSizeof(void *msg);
CsrUint8 *CsrFsalDirMakeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalDirMakeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrFsalDirMakeCfmSerFree CsrFsalPfree
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_READ_REQ
CsrSize CsrFsalFileReadReqSizeof(void *msg);
CsrUint8 *CsrFsalFileReadReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileReadReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileReadReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_REQ */

#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ
CsrSize CsrFsalPermissionsSetReqSizeof(void *msg);
CsrUint8 *CsrFsalPermissionsSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalPermissionsSetReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalPermissionsSetReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ */

#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_CFM
CsrSize CsrFsalPermissionsSetCfmSizeof(void *msg);
CsrUint8 *CsrFsalPermissionsSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalPermissionsSetCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrFsalPermissionsSetCfmSerFree CsrFsalPfree
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_CFM */

#ifndef EXCLUDE_CSR_FSAL_RENAME_REQ
CsrSize CsrFsalRenameReqSizeof(void *msg);
CsrUint8 *CsrFsalRenameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalRenameReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalRenameReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_RENAME_REQ */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_CFM
CsrSize CsrFsalRemoveRecursivelyCfmSizeof(void *msg);
CsrUint8 *CsrFsalRemoveRecursivelyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalRemoveRecursivelyCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrFsalRemoveRecursivelyCfmSerFree CsrFsalPfree
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ
CsrSize CsrFsalDirFindFirstReqSizeof(void *msg);
CsrUint8 *CsrFsalDirFindFirstReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalDirFindFirstReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalDirFindFirstReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM
CsrSize CsrFsalFileCloseCfmSizeof(void *msg);
CsrUint8 *CsrFsalFileCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileCloseCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileCloseCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM */

#ifndef EXCLUDE_CSR_FSAL_STAT_CFM
CsrSize CsrFsalStatCfmSizeof(void *msg);
CsrUint8 *CsrFsalStatCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalStatCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalStatCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_STAT_CFM */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_REQ
CsrSize CsrFsalRemoveReqSizeof(void *msg);
CsrUint8 *CsrFsalRemoveReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalRemoveReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalRemoveReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_REMOVE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM
CsrSize CsrFsalDirChangeCfmSizeof(void *msg);
CsrUint8 *CsrFsalDirChangeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalDirChangeCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalDirChangeCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_REQ
CsrSize CsrFsalFileSeekReqSizeof(void *msg);
CsrUint8 *CsrFsalFileSeekReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileSeekReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileSeekReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ
CsrSize CsrFsalDirChangeReqSizeof(void *msg);
CsrUint8 *CsrFsalDirChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalDirChangeReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalDirChangeReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_READ_CFM
CsrSize CsrFsalFileReadCfmSizeof(void *msg);
CsrUint8 *CsrFsalFileReadCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileReadCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileReadCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_CFM
CsrSize CsrFsalFileSeekCfmSizeof(void *msg);
CsrUint8 *CsrFsalFileSeekCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileSeekCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileSeekCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_CFM
CsrSize CsrFsalFileTellCfmSizeof(void *msg);
CsrUint8 *CsrFsalFileTellCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFsalFileTellCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrFsalFileTellCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_CFM */

#ifdef __cplusplus
}
#endif

#endif
