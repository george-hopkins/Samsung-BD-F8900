/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_HDP_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_hdp_lib.h"
#include "csr_bt_hdp_prim.h"

void CsrBtHdpFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_HDP_PRIM)
    {
        CsrBtHdpPrim *prim = (CsrBtHdpPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_HDP_DATA_REQ
            case CSR_BT_HDP_DATA_REQ:
            {
                CsrBtHdpDataReq *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HDP_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_RSP
            case CSR_BT_HDP_REGISTER_RSP:
            {
                CsrBtHdpRegisterRsp *p = message;
                CsrPmemFree(p->description);
                p->description = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_RSP */
#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_REQ
            case CSR_BT_HDP_REGISTER_REQ:
            {
                CsrBtHdpRegisterReq *p = message;
                CsrPmemFree(p->serviceName);
                p->serviceName = NULL;
                CsrPmemFree(p->serviceDescription);
                p->serviceDescription = NULL;
                CsrPmemFree(p->providerName);
                p->providerName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_REQ */
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
#endif /* EXCLUDE_CSR_BT_HDP_MODULE */
