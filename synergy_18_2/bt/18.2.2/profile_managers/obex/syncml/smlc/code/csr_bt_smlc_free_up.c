/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_SMLC_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_smlc_lib.h"
#include "csr_bt_smlc_prim.h"

void CsrBtSmlcFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SMLC_PRIM)
    {
        CsrBtSmlcPrim *prim = (CsrBtSmlcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM
            case CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM:
            {
                CsrBtSmlcGetNextChunkSmlMsgObjCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM
            case CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM:
            {
                CsrBtSmlcGetSmlMsgObjCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND
            case CSR_BT_SMLC_AUTHENTICATE_IND:
            {
                CsrBtSmlcAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND */
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
#endif /* EXCLUDE_CSR_BT_SMLC_MODULE */
