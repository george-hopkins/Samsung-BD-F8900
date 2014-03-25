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

void CsrBtSmlsFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SMLS_PRIM)
    {
        CsrBtSmlsPrim *prim = (CsrBtSmlsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SMLS_ABORT_IND
            case CSR_BT_SMLS_ABORT_IND:
            {
                CsrBtSmlsAbortInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SMLS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND
            case CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND:
            {
                CsrBtSmlsPutSmlMsgObjInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND
            case CSR_BT_SMLS_AUTHENTICATE_IND:
            {
                CsrBtSmlsAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND
            case CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND:
            {
                CsrBtSmlsPutSmlMsgObjNextInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND */
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
