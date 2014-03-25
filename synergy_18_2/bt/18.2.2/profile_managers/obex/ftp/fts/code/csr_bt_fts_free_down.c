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

void CsrBtFtsFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_FTS_PRIM)
    {
        CsrBtFtsPrim *prim = (CsrBtFtsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES
            case CSR_BT_FTS_GET_LIST_FOLDER_RES:
            {
                CsrBtFtsGetListFolderRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES */
#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES
            case CSR_BT_FTS_GET_OBJ_NEXT_RES:
            {
                CsrBtFtsGetObjNextRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ
            case CSR_BT_FTS_AUTHENTICATE_REQ:
            {
                CsrBtFtsAuthenticateReq *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_RES
            case CSR_BT_FTS_GET_OBJ_RES:
            {
                CsrBtFtsGetObjRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_RES */
#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES
            case CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES:
            {
                CsrBtFtsGetListFolderNextRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES
            case CSR_BT_FTS_AUTHENTICATE_RES:
            {
                CsrBtFtsAuthenticateRes *p = message;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES */
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
