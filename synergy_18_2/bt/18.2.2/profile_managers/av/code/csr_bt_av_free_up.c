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

void CsrBtAvFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_AV_PRIM)
    {
        CsrBtAvPrim *prim = (CsrBtAvPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND
            case CSR_BT_AV_SECURITY_CONTROL_IND:
            {
                CsrBtAvSecurityControlInd *p = message;
                CsrPmemFree(p->contProtMethodData);
                p->contProtMethodData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND */
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM
            case CSR_BT_AV_SECURITY_CONTROL_CFM:
            {
                CsrBtAvSecurityControlCfm *p = message;
                CsrPmemFree(p->contProtMethodData);
                p->contProtMethodData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM */
#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_IND
            case CSR_BT_AV_STREAM_DATA_IND:
            {
                CsrBtAvStreamDataInd *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_IND */
#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM
            case CSR_BT_AV_GET_CONFIGURATION_CFM:
            {
                CsrBtAvGetConfigurationCfm *p = message;
                CsrPmemFree(p->servCapData);
                p->servCapData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM */
#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_IND
            case CSR_BT_AV_RECONFIGURE_IND:
            {
                CsrBtAvReconfigureInd *p = message;
                CsrPmemFree(p->servCapData);
                p->servCapData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_IND */
#ifndef EXCLUDE_CSR_BT_AV_START_IND
            case CSR_BT_AV_START_IND:
            {
                CsrBtAvStartInd *p = message;
                CsrPmemFree(p->list);
                p->list = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_START_IND */
#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_CFM
            case CSR_BT_AV_DISCOVER_CFM:
            {
                CsrBtAvDiscoverCfm *p = message;
                CsrPmemFree(p->seidInfo);
                p->seidInfo = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_CFM */
#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM
            case CSR_BT_AV_GET_CAPABILITIES_CFM:
            {
                CsrBtAvGetCapabilitiesCfm *p = message;
                CsrPmemFree(p->servCapData);
                p->servCapData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM */
#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_IND
            case CSR_BT_AV_SUSPEND_IND:
            {
                CsrBtAvSuspendInd *p = message;
                CsrPmemFree(p->list);
                p->list = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_IND */
#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND
            case CSR_BT_AV_SET_CONFIGURATION_IND:
            {
                CsrBtAvSetConfigurationInd *p = message;
                CsrPmemFree(p->servCapData);
                p->servCapData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND */
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
