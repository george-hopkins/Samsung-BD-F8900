/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_MCAP_PRIVATE_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_mcap_private_lib.h"
#include "csr_bt_mcap_private_prim.h"

void CsrBtMcapPrivateFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_MCAP_PRIVATE_PRIM)
    {
        McapPrimType *prim = (McapPrimType *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ
            case CSR_BT_MCAP_CONNECT_MDL_REQ:
            {
                CsrBtMcapConnectMdlReq *p = message;
                CsrPmemFree(p->qos);
                p->qos = NULL;
                CsrPmemFree(p->flow);
                p->flow = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES
            case CSR_BT_MCAP_CREATE_MDL_RES:
            {
                CsrBtMcapCreateMdlRes *p = message;
                CsrPmemFree(p->qos);
                p->qos = NULL;
                CsrPmemFree(p->flow);
                p->flow = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES */
#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES
            case CSR_BT_MCAP_RECONNECT_MDL_RES:
            {
                CsrBtMcapReconnectMdlRes *p = message;
                CsrPmemFree(p->qos);
                p->qos = NULL;
                CsrPmemFree(p->flow);
                p->flow = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES */
#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ
            case CSR_BT_MCAP_DATA_SEND_REQ:
            {
                CsrBtMcapDataSendReq *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ */
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
#endif /* EXCLUDE_CSR_BT_MCAP_PRIVATE_MODULE */
