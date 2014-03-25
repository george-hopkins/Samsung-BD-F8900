/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_FSAL_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_fsal_lib.h"
#include "csr_fsal_prim.h"

void CsrFsalFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_FSAL_PRIM)
    {
        CsrFsalPrim *prim = (CsrFsalPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ
            case CSR_FSAL_REMOVE_RECURSIVELY_REQ:
            {
                CsrFsalRemoveRecursivelyReq *p = message;
                CsrPmemFree(p->dir);
                p->dir = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ */
#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_REQ
            case CSR_FSAL_DIR_MAKE_REQ:
            {
                CsrFsalDirMakeReq *p = message;
                CsrPmemFree(p->dirName);
                p->dirName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_REQ */
#ifndef EXCLUDE_CSR_FSAL_STAT_REQ
            case CSR_FSAL_STAT_REQ:
            {
                CsrFsalStatReq *p = message;
                CsrPmemFree(p->path);
                p->path = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_STAT_REQ */
#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_REQ
            case CSR_FSAL_FILE_OPEN_REQ:
            {
                CsrFsalFileOpenReq *p = message;
                CsrPmemFree(p->fileName);
                p->fileName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_REQ */
#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_REQ
            case CSR_FSAL_FILE_WRITE_REQ:
            {
                CsrFsalFileWriteReq *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_REQ */
#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ
            case CSR_FSAL_PERMISSIONS_SET_REQ:
            {
                CsrFsalPermissionsSetReq *p = message;
                CsrPmemFree(p->path);
                p->path = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ */
#ifndef EXCLUDE_CSR_FSAL_RENAME_REQ
            case CSR_FSAL_RENAME_REQ:
            {
                CsrFsalRenameReq *p = message;
                CsrPmemFree(p->oldPath);
                p->oldPath = NULL;
                CsrPmemFree(p->newPath);
                p->newPath = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_RENAME_REQ */
#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ
            case CSR_FSAL_DIR_FIND_FIRST_REQ:
            {
                CsrFsalDirFindFirstReq *p = message;
                CsrPmemFree(p->searchPattern);
                p->searchPattern = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ */
#ifndef EXCLUDE_CSR_FSAL_REMOVE_REQ
            case CSR_FSAL_REMOVE_REQ:
            {
                CsrFsalRemoveReq *p = message;
                CsrPmemFree(p->path);
                p->path = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_REMOVE_REQ */
#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ
            case CSR_FSAL_DIR_CHANGE_REQ:
            {
                CsrFsalDirChangeReq *p = message;
                CsrPmemFree(p->path);
                p->path = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ */
            default:
            {
                break;
            }
        } /* End switch */
    } /* End if */
    else
    {
        /* Unknown primitive type, exception handling */
    }
}
#endif /* EXCLUDE_CSR_FSAL_MODULE */
