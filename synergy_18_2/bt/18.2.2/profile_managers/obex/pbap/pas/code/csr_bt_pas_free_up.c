/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_PAS_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_pas_lib.h"
#include "csr_bt_pas_prim.h"

void CsrBtPasFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_PAS_PRIM)
    {
        CsrBtPasPrim *prim = (CsrBtPasPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND
            case CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND:
            {
                CsrBtPasPullVcardMchListInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND */
#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND
            case CSR_BT_PAS_SET_FOLDER_IND:
            {
                CsrBtPasSetFolderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND
            case CSR_BT_PAS_AUTHENTICATE_IND:
            {
                CsrBtPasAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND
            case CSR_BT_PAS_PULL_PB_SIZE_IND:
            {
                CsrBtPasPullPbSizeInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND
            case CSR_BT_PAS_PULL_VCARD_LIST_IND:
            {
                CsrBtPasPullVcardListInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND
            case CSR_BT_PAS_PULL_VCARD_ENTRY_IND:
            {
                CsrBtPasPullVcardEntryInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_IND
            case CSR_BT_PAS_PULL_PB_IND:
            {
                CsrBtPasPullPbInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND
            case CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND:
            {
                CsrBtPasPullVcardListSizeInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND */
#ifndef EXCLUDE_CSR_BT_PAS_ABORT_IND
            case CSR_BT_PAS_ABORT_IND:
            {
                CsrBtPasAbortInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_ABORT_IND */
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
#endif /* EXCLUDE_CSR_BT_PAS_MODULE */
