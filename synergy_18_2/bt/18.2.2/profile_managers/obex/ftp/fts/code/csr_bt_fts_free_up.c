/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_FTS_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_fts_lib.h"
#include "csr_bt_fts_prim.h"

void CsrBtFtsFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_FTS_PRIM)
    {
        CsrBtFtsPrim *prim = (CsrBtFtsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND
            case CSR_BT_FTS_SET_FOLDER_IND:
            {
                CsrBtFtsSetFolderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND
            case CSR_BT_FTS_PUT_OBJ_NEXT_IND:
            {
                CsrBtFtsPutObjNextInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND
            case CSR_BT_FTS_PUT_OBJ_IND:
            {
                CsrBtFtsPutObjInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_IND
            case CSR_BT_FTS_GET_OBJ_IND:
            {
                CsrBtFtsGetObjInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND
            case CSR_BT_FTS_MOVE_OBJ_IND:
            {
                CsrBtFtsMoveObjInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND
            case CSR_BT_FTS_GET_LIST_FOLDER_IND:
            {
                CsrBtFtsGetListFolderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_FTS_ABORT_IND
            case CSR_BT_FTS_ABORT_IND:
            {
                CsrBtFtsAbortInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND
            case CSR_BT_FTS_SET_ADD_FOLDER_IND:
            {
                CsrBtFtsSetAddFolderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND
            case CSR_BT_FTS_AUTHENTICATE_IND:
            {
                CsrBtFtsAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND
            case CSR_BT_FTS_COPY_OBJ_IND:
            {
                CsrBtFtsCopyObjInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND
            case CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND:
            {
                CsrBtFtsSetObjPermissionsInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND */
#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND
            case CSR_BT_FTS_DEL_OBJ_IND:
            {
                CsrBtFtsDelObjInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND */
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
#endif /* EXCLUDE_CSR_BT_FTS_MODULE */
