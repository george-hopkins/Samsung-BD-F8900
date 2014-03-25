/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_OPC_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_opc_lib.h"
#include "csr_bt_opc_prim.h"

void CsrBtOpcFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_OPC_PRIM)
    {
        CsrBtOpcPrim *prim = (CsrBtOpcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES
            case CSR_BT_OPC_PUT_OBJECT_RES:
            {
                CsrBtOpcPutObjectRes *p = message;
                CsrPmemFree(p->object);
                p->object = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES
            case CSR_BT_OPC_AUTHENTICATE_RES:
            {
                CsrBtOpcAuthenticateRes *p = message;
                CsrPmemFree(p->authPassword);
                p->authPassword = NULL;
                CsrPmemFree(p->authUserId);
                p->authUserId = NULL;
                CsrPmemFree(p->chalRealm);
                p->chalRealm = NULL;
                CsrPmemFree(p->chalPassword);
                p->chalPassword = NULL;
                CsrPmemFree(p->chalUserId);
                p->chalUserId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_REQ
            case CSR_BT_OPC_CONNECT_REQ:
            {
                CsrBtOpcConnectReq *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_OPC_PUT_REQ
            case CSR_BT_OPC_PUT_REQ:
            {
                CsrBtOpcPutReq *p = message;
                CsrPmemFree(p->bodyType);
                p->bodyType = NULL;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPC_PUT_REQ */
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
#endif /* EXCLUDE_CSR_BT_OPC_MODULE */
