/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_TFTP_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_tftp_lib.h"
#include "csr_tftp_prim.h"

void CsrTftpFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_TFTP_PRIM)
    {
        CsrTftpPrim *prim = (CsrTftpPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_TFTP_CONNECT_IND
            case CSR_TFTP_CONNECT_IND:
            {
                CsrTftpConnectInd *p = message;
                CsrPmemFree(p->fileName);
                p->fileName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_TFTP_CONNECT_IND */
#ifndef EXCLUDE_CSR_TFTP_WRITE_IND
            case CSR_TFTP_WRITE_IND:
            {
                CsrTftpWriteInd *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_TFTP_WRITE_IND */
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
#endif /* EXCLUDE_CSR_TFTP_MODULE */
