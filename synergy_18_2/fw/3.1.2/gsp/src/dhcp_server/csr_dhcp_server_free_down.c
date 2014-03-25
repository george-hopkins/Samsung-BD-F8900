/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_DHCP_SERVER_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_dhcp_server_lib.h"
#include "csr_dhcp_server_prim.h"

void CsrDhcpServerFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_DHCP_SERVER_PRIM)
    {
        CsrDhcpServerPrim *prim = (CsrDhcpServerPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ
            case CSR_DHCP_SERVER_LEASES_SET_REQ:
            {
                CsrDhcpServerLeasesSetReq *p = message;
                CsrPmemFree(p->leases);
                p->leases = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ */
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
#endif /* EXCLUDE_CSR_DHCP_SERVER_MODULE */
