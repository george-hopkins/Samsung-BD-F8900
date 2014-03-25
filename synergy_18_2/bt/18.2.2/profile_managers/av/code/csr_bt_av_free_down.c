/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_AV_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_av_lib.h"
#include "csr_bt_av_prim.h"

void CsrBtAvFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_AV_PRIM)
    {
        CsrBtAvPrim *prim = (CsrBtAvPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES
            case CSR_BT_AV_SECURITY_CONTROL_RES:
            {
                CsrBtAvSecurityControlRes *p = message;
                CsrPmemFree(p->contProtMethodData);
                p->contProtMethodData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES */
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ
            case CSR_BT_AV_SECURITY_CONTROL_REQ:
            {
                CsrBtAvSecurityControlReq *p = message;
                CsrPmemFree(p->contProtMethodData);
                p->contProtMethodData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ */
#ifndef EXCLUDE_CSR_BT_AV_START_REQ
            case CSR_BT_AV_START_REQ:
            {
                CsrBtAvStartReq *p = message;
                CsrPmemFree(p->list);
                p->list = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_START_REQ */
#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_RES
            case CSR_BT_AV_SUSPEND_RES:
            {
                CsrBtAvSuspendRes *p = message;
                CsrPmemFree(p->list);
                p->list = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_RES */
#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_RES
            case CSR_BT_AV_DISCOVER_RES:
            {
                CsrBtAvDiscoverRes *p = message;
                CsrPmemFree(p->seidInfo);
                p->seidInfo = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_RES */
#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES
            case CSR_BT_AV_GET_CAPABILITIES_RES:
            {
                CsrBtAvGetCapabilitiesRes *p = message;
                CsrPmemFree(p->servCapData);
                p->servCapData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES */
#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ
            case CSR_BT_AV_SET_CONFIGURATION_REQ:
            {
                CsrBtAvSetConfigurationReq *p = message;
                CsrPmemFree(p->appServCapData);
                p->appServCapData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ */
#ifndef EXCLUDE_CSR_BT_AV_START_RES
            case CSR_BT_AV_START_RES:
            {
                CsrBtAvStartRes *p = message;
                CsrPmemFree(p->list);
                p->list = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_START_RES */
#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ
            case CSR_BT_AV_RECONFIGURE_REQ:
            {
                CsrBtAvReconfigureReq *p = message;
                CsrPmemFree(p->servCapData);
                p->servCapData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ */
#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES
            case CSR_BT_AV_GET_ALL_CAPABILITIES_RES:
            {
                CsrBtAvGetAllCapabilitiesRes *p = message;
                CsrPmemFree(p->servCapData);
                p->servCapData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES */
#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ
            case CSR_BT_AV_STREAM_DATA_REQ:
            {
                CsrBtAvStreamDataReq *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_REQ
            case CSR_BT_AV_SUSPEND_REQ:
            {
                CsrBtAvSuspendReq *p = message;
                CsrPmemFree(p->list);
                p->list = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_REQ */
#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES
            case CSR_BT_AV_GET_CONFIGURATION_RES:
            {
                CsrBtAvGetConfigurationRes *p = message;
                CsrPmemFree(p->servCapData);
                p->servCapData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES */
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
#endif /* EXCLUDE_CSR_BT_AV_MODULE */
