#ifndef CSR_FSAL_SEF_H__
#define CSR_FSAL_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
void CsrFsalSessionCreateReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalSessionDestroyReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalFileOpenReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalFileCloseReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalFileSeekReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalFileReadReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalFileWriteReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalFileTellReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalStatReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalRemoveReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalRenameReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalPermissionsSetReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalDirMakeReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalDirChangeReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalDirFindFirstReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalDirFindNextReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalDirFindCloseReqHandler(CsrFsalInstanceData *pInst);
void CsrFsalRemoveRecursivelyReqHandler(CsrFsalInstanceData *pInst);


#ifdef __cplusplus
}
#endif

#endif /* CSR_FSAL_SEF_H__ */
