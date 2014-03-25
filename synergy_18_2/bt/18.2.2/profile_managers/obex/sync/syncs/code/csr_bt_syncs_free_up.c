/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_SYNCS_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_syncs_lib.h"
#include "csr_bt_syncs_prim.h"

void CsrBtSyncsFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SYNCS_PRIM)
    {
        CsrBtSyncsPrim *prim = (CsrBtSyncsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND
            case CSR_BT_SYNCS_GET_PB_ENTRY_IND:
            {
                CsrBtSyncsGetPbEntryInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_IND
            case CSR_BT_SYNCS_PUT_IND:
            {
                CsrBtSyncsPutInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND
            case CSR_BT_SYNCS_PUT_PB_ENTRY_IND:
            {
                CsrBtSyncsPutPbEntryInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_IND
            case CSR_BT_SYNCS_GET_IND:
            {
                CsrBtSyncsGetInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND
            case CSR_BT_SYNCS_PUT_CAL_ENTRY_IND:
            {
                CsrBtSyncsPutCalEntryInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND
            case CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND:
            {
                CsrBtSyncsPutCalAddEntryInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND
            case CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND:
            {
                CsrBtSyncsGetPbChangeLogInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND
            case CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND:
            {
                CsrBtSyncsPutPbAddEntryInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND
            case CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND:
            {
                CsrBtSyncsGetCalChangeLogInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND
            case CSR_BT_SYNCS_AUTHENTICATE_IND:
            {
                CsrBtSyncsAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND
            case CSR_BT_SYNCS_GET_CAL_ENTRY_IND:
            {
                CsrBtSyncsGetCalEntryInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_ABORT_IND
            case CSR_BT_SYNCS_ABORT_IND:
            {
                CsrBtSyncsAbortInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND
            case CSR_BT_SYNCS_PUT_NEXT_IND:
            {
                CsrBtSyncsPutNextInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND */
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
#endif /* EXCLUDE_CSR_BT_SYNCS_MODULE */
