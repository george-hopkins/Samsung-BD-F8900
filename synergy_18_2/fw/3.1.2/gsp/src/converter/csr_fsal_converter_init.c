/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_hand_coded.h"
#include "csr_fsal_converter_init.h"
#ifndef EXCLUDE_CSR_FSAL_MODULE
#include "csr_fsal_serialize.h"
#include "csr_fsal_prim.h"

static CsrMsgConvMsgEntry csr_fsal_conv_lut[] = {
#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM
    { CSR_FSAL_DIR_CHANGE_CFM, CsrFsalDirChangeCfmSizeof, CsrFsalDirChangeCfmSer, CsrFsalDirChangeCfmDes, CsrFsalDirChangeCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM */
#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ
    { CSR_FSAL_DIR_CHANGE_REQ, CsrFsalDirChangeReqSizeof, CsrFsalDirChangeReqSer, CsrFsalDirChangeReqDes, CsrFsalDirChangeReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ */
#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM
    { CSR_FSAL_DIR_FIND_CLOSE_CFM, CsrFsalDirFindCloseCfmSizeof, CsrFsalDirFindCloseCfmSer, CsrFsalDirFindCloseCfmDes, CsrFsalDirFindCloseCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM */
#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ
    { CSR_FSAL_DIR_FIND_CLOSE_REQ, CsrFsalDirFindCloseReqSizeof, CsrFsalDirFindCloseReqSer, CsrFsalDirFindCloseReqDes, CsrFsalDirFindCloseReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ */
#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM
    { CSR_FSAL_DIR_FIND_FIRST_CFM, CsrFsalDirFindFirstCfmSizeof, CsrFsalDirFindFirstCfmSer, CsrFsalDirFindFirstCfmDes, CsrFsalDirFindFirstCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM */
#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ
    { CSR_FSAL_DIR_FIND_FIRST_REQ, CsrFsalDirFindFirstReqSizeof, CsrFsalDirFindFirstReqSer, CsrFsalDirFindFirstReqDes, CsrFsalDirFindFirstReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ */
#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM
    { CSR_FSAL_DIR_FIND_NEXT_CFM, CsrFsalDirFindNextCfmSizeof, CsrFsalDirFindNextCfmSer, CsrFsalDirFindNextCfmDes, CsrFsalDirFindNextCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM */
#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ
    { CSR_FSAL_DIR_FIND_NEXT_REQ, CsrFsalDirFindNextReqSizeof, CsrFsalDirFindNextReqSer, CsrFsalDirFindNextReqDes, CsrFsalDirFindNextReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ */
#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_CFM
    { CSR_FSAL_DIR_MAKE_CFM, CsrFsalDirMakeCfmSizeof, CsrFsalDirMakeCfmSer, CsrFsalDirMakeCfmDes, CsrFsalDirMakeCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_CFM */
#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_REQ
    { CSR_FSAL_DIR_MAKE_REQ, CsrFsalDirMakeReqSizeof, CsrFsalDirMakeReqSer, CsrFsalDirMakeReqDes, CsrFsalDirMakeReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_REQ */
#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM
    { CSR_FSAL_FILE_CLOSE_CFM, CsrFsalFileCloseCfmSizeof, CsrFsalFileCloseCfmSer, CsrFsalFileCloseCfmDes, CsrFsalFileCloseCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM */
#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ
    { CSR_FSAL_FILE_CLOSE_REQ, CsrFsalFileCloseReqSizeof, CsrFsalFileCloseReqSer, CsrFsalFileCloseReqDes, CsrFsalFileCloseReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ */
#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_CFM
    { CSR_FSAL_FILE_OPEN_CFM, CsrFsalFileOpenCfmSizeof, CsrFsalFileOpenCfmSer, CsrFsalFileOpenCfmDes, CsrFsalFileOpenCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_CFM */
#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_REQ
    { CSR_FSAL_FILE_OPEN_REQ, CsrFsalFileOpenReqSizeof, CsrFsalFileOpenReqSer, CsrFsalFileOpenReqDes, CsrFsalFileOpenReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_REQ */
#ifndef EXCLUDE_CSR_FSAL_FILE_READ_CFM
    { CSR_FSAL_FILE_READ_CFM, CsrFsalFileReadCfmSizeof, CsrFsalFileReadCfmSer, CsrFsalFileReadCfmDes, CsrFsalFileReadCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_CFM */
#ifndef EXCLUDE_CSR_FSAL_FILE_READ_REQ
    { CSR_FSAL_FILE_READ_REQ, CsrFsalFileReadReqSizeof, CsrFsalFileReadReqSer, CsrFsalFileReadReqDes, CsrFsalFileReadReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_REQ */
#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_CFM
    { CSR_FSAL_FILE_SEEK_CFM, CsrFsalFileSeekCfmSizeof, CsrFsalFileSeekCfmSer, CsrFsalFileSeekCfmDes, CsrFsalFileSeekCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_CFM */
#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_REQ
    { CSR_FSAL_FILE_SEEK_REQ, CsrFsalFileSeekReqSizeof, CsrFsalFileSeekReqSer, CsrFsalFileSeekReqDes, CsrFsalFileSeekReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_REQ */
#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_CFM
    { CSR_FSAL_FILE_TELL_CFM, CsrFsalFileTellCfmSizeof, CsrFsalFileTellCfmSer, CsrFsalFileTellCfmDes, CsrFsalFileTellCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_CFM */
#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_REQ
    { CSR_FSAL_FILE_TELL_REQ, CsrFsalFileTellReqSizeof, CsrFsalFileTellReqSer, CsrFsalFileTellReqDes, CsrFsalFileTellReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_REQ */
#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_CFM
    { CSR_FSAL_FILE_WRITE_CFM, CsrFsalFileWriteCfmSizeof, CsrFsalFileWriteCfmSer, CsrFsalFileWriteCfmDes, CsrFsalFileWriteCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_CFM */
#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_REQ
    { CSR_FSAL_FILE_WRITE_REQ, CsrFsalFileWriteReqSizeof, CsrFsalFileWriteReqSer, CsrFsalFileWriteReqDes, CsrFsalFileWriteReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_REQ */
#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_CFM
    { CSR_FSAL_PERMISSIONS_SET_CFM, CsrFsalPermissionsSetCfmSizeof, CsrFsalPermissionsSetCfmSer, CsrFsalPermissionsSetCfmDes, CsrFsalPermissionsSetCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_CFM */
#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ
    { CSR_FSAL_PERMISSIONS_SET_REQ, CsrFsalPermissionsSetReqSizeof, CsrFsalPermissionsSetReqSer, CsrFsalPermissionsSetReqDes, CsrFsalPermissionsSetReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ */
#ifndef EXCLUDE_CSR_FSAL_REMOVE_CFM
    { CSR_FSAL_REMOVE_CFM, CsrFsalRemoveCfmSizeof, CsrFsalRemoveCfmSer, CsrFsalRemoveCfmDes, CsrFsalRemoveCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_REMOVE_CFM */
#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_CFM
    { CSR_FSAL_REMOVE_RECURSIVELY_CFM, CsrFsalRemoveRecursivelyCfmSizeof, CsrFsalRemoveRecursivelyCfmSer, CsrFsalRemoveRecursivelyCfmDes, CsrFsalRemoveRecursivelyCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_CFM */
#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ
    { CSR_FSAL_REMOVE_RECURSIVELY_REQ, CsrFsalRemoveRecursivelyReqSizeof, CsrFsalRemoveRecursivelyReqSer, CsrFsalRemoveRecursivelyReqDes, CsrFsalRemoveRecursivelyReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ */
#ifndef EXCLUDE_CSR_FSAL_REMOVE_REQ
    { CSR_FSAL_REMOVE_REQ, CsrFsalRemoveReqSizeof, CsrFsalRemoveReqSer, CsrFsalRemoveReqDes, CsrFsalRemoveReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_REMOVE_REQ */
#ifndef EXCLUDE_CSR_FSAL_RENAME_CFM
    { CSR_FSAL_RENAME_CFM, CsrFsalRenameCfmSizeof, CsrFsalRenameCfmSer, CsrFsalRenameCfmDes, CsrFsalRenameCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_RENAME_CFM */
#ifndef EXCLUDE_CSR_FSAL_RENAME_REQ
    { CSR_FSAL_RENAME_REQ, CsrFsalRenameReqSizeof, CsrFsalRenameReqSer, CsrFsalRenameReqDes, CsrFsalRenameReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_RENAME_REQ */
#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM
    { CSR_FSAL_SESSION_CREATE_CFM, CsrFsalSessionCreateCfmSizeof, CsrFsalSessionCreateCfmSer, CsrFsalSessionCreateCfmDes, CsrFsalSessionCreateCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM */
#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_REQ
    { CSR_FSAL_SESSION_CREATE_REQ, CsrFsalSessionCreateReqSizeof, CsrFsalSessionCreateReqSer, CsrFsalSessionCreateReqDes, CsrFsalSessionCreateReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_REQ */
#ifndef EXCLUDE_CSR_FSAL_SESSION_DESTROY_CFM
    { CSR_FSAL_SESSION_DESTROY_CFM, CsrFsalSessionDestroyCfmSizeof, CsrFsalSessionDestroyCfmSer, CsrFsalSessionDestroyCfmDes, CsrFsalSessionDestroyCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_SESSION_DESTROY_CFM */
#ifndef EXCLUDE_CSR_FSAL_SESSION_DESTROY_REQ
    { CSR_FSAL_SESSION_DESTROY_REQ, CsrFsalSessionDestroyReqSizeof, CsrFsalSessionDestroyReqSer, CsrFsalSessionDestroyReqDes, CsrFsalSessionDestroyReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_SESSION_DESTROY_REQ */
#ifndef EXCLUDE_CSR_FSAL_STAT_CFM
    { CSR_FSAL_STAT_CFM, CsrFsalStatCfmSizeof, CsrFsalStatCfmSer, CsrFsalStatCfmDes, CsrFsalStatCfmSerFree },
#endif /* EXCLUDE_CSR_FSAL_STAT_CFM */
#ifndef EXCLUDE_CSR_FSAL_STAT_REQ
    { CSR_FSAL_STAT_REQ, CsrFsalStatReqSizeof, CsrFsalStatReqSer, CsrFsalStatReqDes, CsrFsalStatReqSerFree },
#endif /* EXCLUDE_CSR_FSAL_STAT_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrFsalConverterInit(void)
{
    CsrMsgConvInsert(CSR_FSAL_PRIM, csr_fsal_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_fsal_conv_info = {
    CSR_FSAL_PRIM,
    "CSR_FSAL_PRIM",
    csr_fsal_conv_lut
};

CsrLogPrimitiveInformation* CsrFsalTechInfoGet(void)
{
    return &csr_fsal_conv_info;
}
#endif
#endif

