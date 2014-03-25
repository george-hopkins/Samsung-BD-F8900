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

void CsrBtSynccFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SYNCC_PRIM)
    {
        CsrBtSynccPrim *prim = (CsrBtSynccPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND
            case CSR_BT_SYNCC_SYNC_COMMAND_IND:
            {
                CsrBtSynccSyncCommandInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND */
#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM
            case CSR_BT_SYNCC_DELETE_OBJECT_CFM:
            {
                CsrBtSynccDeleteObjectCfm *p = message;
                CsrPmemFree(p->luid);
                p->luid = NULL;
                CsrPmemFree(p->changeCounter);
                p->changeCounter = NULL;
                CsrPmemFree(p->timeStamp);
                p->timeStamp = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND
            case CSR_BT_SYNCC_AUTHENTICATE_IND:
            {
                CsrBtSynccAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM
            case CSR_BT_SYNCC_GET_OBJECT_CFM:
            {
                CsrBtSynccGetObjectCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND
            case CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND:
            {
                CsrBtSynccSyncCommandAbortInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND
            case CSR_BT_SYNCC_GET_OBJECT_IND:
            {
                CsrBtSynccGetObjectInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM
            case CSR_BT_SYNCC_MODIFY_OBJECT_CFM:
            {
                CsrBtSynccModifyObjectCfm *p = message;
                CsrPmemFree(p->luid);
                p->luid = NULL;
                CsrPmemFree(p->changeCounter);
                p->changeCounter = NULL;
                CsrPmemFree(p->timeStamp);
                p->timeStamp = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM
            case CSR_BT_SYNCC_ADD_OBJECT_CFM:
            {
                CsrBtSynccAddObjectCfm *p = message;
                CsrPmemFree(p->luid);
                p->luid = NULL;
                CsrPmemFree(p->changeCounter);
                p->changeCounter = NULL;
                CsrPmemFree(p->timeStamp);
                p->timeStamp = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM */
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
