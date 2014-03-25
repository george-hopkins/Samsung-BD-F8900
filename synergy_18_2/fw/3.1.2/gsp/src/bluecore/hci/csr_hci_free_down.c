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

void CsrHciFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_HCI_PRIM)
    {
        CsrHciPrim *prim = (CsrHciPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_HCI_SCO_DATA_REQ
            case CSR_HCI_SCO_DATA_REQ:
            {
                CsrHciScoDataReq *p = message;
                CsrMblkDestroy(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_REQ */
#ifndef EXCLUDE_CSR_HCI_ACL_DATA_REQ
            case CSR_HCI_ACL_DATA_REQ:
            {
                CsrHciAclDataReq *p = message;
                CsrMblkDestroy(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_REQ */
#ifndef EXCLUDE_CSR_HCI_COMMAND_REQ
            case CSR_HCI_COMMAND_REQ:
            {
                CsrHciCommandReq *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_HCI_COMMAND_REQ */
#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ
            case CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ:
            {
                CsrHciVendorSpecificCommandReq *p = message;
                CsrMblkDestroy(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ */
#ifndef EXCLUDE_CSR_HCI_INCOMING_DATA_REQ
            case CSR_HCI_INCOMING_DATA_REQ:
            {
                CsrHciIncomingDataReq *p = message;
                CsrMblkDestroy(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_HCI_INCOMING_DATA_REQ */
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
