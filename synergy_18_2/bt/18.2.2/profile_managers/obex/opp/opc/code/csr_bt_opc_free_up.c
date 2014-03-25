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

void CsrBtOpcFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_OPC_PRIM)
    {
        CsrBtOpcPrim *prim = (CsrBtOpcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM
            case CSR_BT_OPC_GET_HEADER_CFM:
            {
                CsrBtOpcGetHeaderCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM */
#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND
            case CSR_BT_OPC_AUTHENTICATE_IND:
            {
                CsrBtOpcAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM
            case CSR_BT_OPC_GET_OBJECT_CFM:
            {
                CsrBtOpcGetObjectCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM */
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
