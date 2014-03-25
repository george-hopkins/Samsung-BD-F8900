/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_SMLS_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_smls_lib.h"
#include "csr_bt_smls_prim.h"

void CsrBtSmlsFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SMLS_PRIM)
    {
        CsrBtSmlsPrim *prim = (CsrBtSmlsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES
            case CSR_BT_SMLS_GET_SML_MSG_OBJ_RES:
            {
                CsrBtSmlsGetSmlMsgObjRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES */
#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ
            case CSR_BT_SMLS_AUTHENTICATE_REQ:
            {
                CsrBtSmlsAuthenticateReq *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES
            case CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES:
            {
                CsrBtSmlsGetSmlMsgObjNextRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES
            case CSR_BT_SMLS_AUTHENTICATE_RES:
            {
                CsrBtSmlsAuthenticateRes *p = message;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES */
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
#endif /* EXCLUDE_CSR_BT_SMLS_MODULE */
