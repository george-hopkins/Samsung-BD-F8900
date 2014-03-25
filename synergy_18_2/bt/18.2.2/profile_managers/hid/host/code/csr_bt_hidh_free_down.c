/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_HIDH_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_hidh_lib.h"
#include "csr_bt_hidh_prim.h"

void CsrBtHidhFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_HIDH_PRIM)
    {
        CsrBtHidhPrim *prim = (CsrBtHidhPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ
            case CSR_BT_HIDH_CONNECT_ACCEPT_REQ:
            {
                CsrBtHidhConnectAcceptReq *p = message;
                CsrPmemFree(p->qosIntr);
                p->qosIntr = NULL;
                CsrPmemFree(p->qosCtrl);
                p->qosCtrl = NULL;
                CsrPmemFree(p->sdpInfo);
                p->sdpInfo = NULL;
                CsrPmemFree(p->serviceName);
                p->serviceName = NULL;
                CsrPmemFree(p->descriptor);
                p->descriptor = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_CONNECT_REQ
            case CSR_BT_HIDH_CONNECT_REQ:
            {
                CsrBtHidhConnectReq *p = message;
                CsrPmemFree(p->qosCtrl);
                p->qosCtrl = NULL;
                CsrPmemFree(p->qosIntr);
                p->qosIntr = NULL;
                CsrPmemFree(p->sdpInfo);
                p->sdpInfo = NULL;
                CsrPmemFree(p->serviceName);
                p->serviceName = NULL;
                CsrPmemFree(p->descriptor);
                p->descriptor = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HIDH_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_DATA_REQ
            case CSR_BT_HIDH_DATA_REQ:
            {
                CsrBtHidhDataReq *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HIDH_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ
            case CSR_BT_HIDH_SET_REPORT_REQ:
            {
                CsrBtHidhSetReportReq *p = message;
                CsrPmemFree(p->report);
                p->report = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HIDH_SET_REPORT_REQ */
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
#endif /* EXCLUDE_CSR_BT_HIDH_MODULE */
