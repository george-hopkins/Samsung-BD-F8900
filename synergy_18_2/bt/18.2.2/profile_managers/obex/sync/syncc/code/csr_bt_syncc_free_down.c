/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_syncc_lib.h"
#include "csr_bt_syncc_prim.h"

void CsrBtSynccFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SYNCC_PRIM)
    {
        CsrBtSynccPrim *prim = (CsrBtSynccPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES
            case CSR_BT_SYNCC_MODIFY_OBJECT_RES:
            {
                CsrBtSynccModifyObjectRes *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ
            case CSR_BT_SYNCC_CONNECT_REQ:
            {
                CsrBtSynccConnectReq *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ
            case CSR_BT_SYNCC_MODIFY_OBJECT_REQ:
            {
                CsrBtSynccModifyObjectReq *p = message;
                CsrPmemFree(p->maxExpectedChangeCounter);
                p->maxExpectedChangeCounter = NULL;
                CsrPmemFree(p->objectName);
                p->objectName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ
            case CSR_BT_SYNCC_ADD_OBJECT_REQ:
            {
                CsrBtSynccAddObjectReq *p = message;
                CsrPmemFree(p->objectName);
                p->objectName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES
            case CSR_BT_SYNCC_ADD_OBJECT_RES:
            {
                CsrBtSynccAddObjectRes *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES
            case CSR_BT_SYNCC_AUTHENTICATE_RES:
            {
                CsrBtSynccAuthenticateRes *p = message;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ
            case CSR_BT_SYNCC_GET_OBJECT_REQ:
            {
                CsrBtSynccGetObjectReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ
            case CSR_BT_SYNCC_DELETE_OBJECT_REQ:
            {
                CsrBtSynccDeleteObjectReq *p = message;
                CsrPmemFree(p->maxExpectedChangeCounter);
                p->maxExpectedChangeCounter = NULL;
                CsrPmemFree(p->objectName);
                p->objectName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ */
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
#endif /* EXCLUDE_CSR_BT_SYNCC_MODULE */
