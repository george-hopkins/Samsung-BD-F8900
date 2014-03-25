/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifndef EXCLUDE_CSR_TLS_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_tls_lib.h"
#include "csr_tls_prim.h"

void CsrTlsFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_TLS_PRIM)
    {
        CsrTlsPrim *prim = (CsrTlsPrim *) message;
        switch (*prim)
        {
            case CSR_TLS_CONNECTION_CERT_IND:
            {
                CsrTlsConnectionCertInd *p = message;
                CsrPmemFree(p->cert);
                p->cert = NULL;
                break;
            }
            case CSR_TLS_CONNECTION_DATA_IND:
            {
                CsrTlsConnectionDataInd *p = message;
                CsrPmemFree(p->buf);
                p->buf = NULL;
                break;
            }
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

#endif /* EXCLUDE_CSR_TLS_MODULE */
