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

void CsrFsalFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_FSAL_PRIM)
    {
        CsrFsalPrim *prim = (CsrFsalPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM
            case CSR_FSAL_DIR_FIND_FIRST_CFM:
            {
                CsrFsalDirFindFirstCfm *p = message;
                CsrPmemFree(p->entry.name);
                p->entry.name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM */
#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM
            case CSR_FSAL_DIR_FIND_NEXT_CFM:
            {
                CsrFsalDirFindNextCfm *p = message;
                CsrPmemFree(p->entry.name);
                p->entry.name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM */
#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM
            case CSR_FSAL_SESSION_CREATE_CFM:
            {
                CsrFsalSessionCreateCfm *p = message;
                CsrPmemFree(p->currentWorkingDir);
                p->currentWorkingDir = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM */
#ifndef EXCLUDE_CSR_FSAL_STAT_CFM
            case CSR_FSAL_STAT_CFM:
            {
                CsrFsalStatCfm *p = message;
                CsrPmemFree(p->stat.name);
                p->stat.name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_STAT_CFM */
#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM
            case CSR_FSAL_DIR_CHANGE_CFM:
            {
                CsrFsalDirChangeCfm *p = message;
                CsrPmemFree(p->currentWorkingDir);
                p->currentWorkingDir = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM */
#ifndef EXCLUDE_CSR_FSAL_FILE_READ_CFM
            case CSR_FSAL_FILE_READ_CFM:
            {
                CsrFsalFileReadCfm *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_CFM */
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
