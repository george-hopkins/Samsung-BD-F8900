/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_HCI_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_hci_lib.h"
#include "csr_hci_prim.h"

void CsrHciFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_HCI_PRIM)
    {
        CsrHciPrim *prim = (CsrHciPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_HCI_SCO_DATA_IND
            case CSR_HCI_SCO_DATA_IND:
            {
                CsrHciScoDataInd *p = message;
                CsrMblkDestroy(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_IND */
#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND
            case CSR_HCI_VENDOR_SPECIFIC_EVENT_IND:
            {
                CsrHciVendorSpecificEventInd *p = message;
                CsrMblkDestroy(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND */
#ifndef EXCLUDE_CSR_HCI_ACL_DATA_IND
            case CSR_HCI_ACL_DATA_IND:
            {
                CsrHciAclDataInd *p = message;
                CsrMblkDestroy(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_IND */
#ifndef EXCLUDE_CSR_HCI_EVENT_IND
            case CSR_HCI_EVENT_IND:
            {
                CsrHciEventInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_HCI_EVENT_IND */
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
#endif /* EXCLUDE_CSR_HCI_MODULE */
