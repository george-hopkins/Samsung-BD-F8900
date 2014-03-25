/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_CM_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_free_handcoded.h"

void CsrBtCmFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_CM_PRIM)
    {
        CsrBtCmPrim *prim = (CsrBtCmPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND
            case CSR_BT_CM_SDC_UUID128_SEARCH_IND:
            {
                CsrBtCmSdcUuid128SearchInd *p = message;
                CsrPmemFree(p->serviceHandleList);
                p->serviceHandleList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND */
#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM
            case CSR_BT_CM_READ_LOCAL_NAME_CFM:
            {
                CsrBtCmReadLocalNameCfm *p = message;
                CsrPmemFree(p->localName);
                p->localName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM */
#ifndef EXCLUDE_CSR_BT_CM_DATA_IND
            case CSR_BT_CM_DATA_IND:
            {
                CsrBtCmDataInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_DATA_IND */
#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM
            case CSR_BT_CM_SDC_ATTRIBUTE_CFM:
            {
                CsrBtCmSdcAttributeCfm *p = message;
                CsrPmemFree(p->attributeList);
                p->attributeList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND
            case CSR_BT_CM_LOCAL_NAME_CHANGE_IND:
            {
                CsrBtCmLocalNameChangeInd *p = message;
                CsrPmemFree(p->localName);
                p->localName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND
            case CSR_BT_CM_READ_REMOTE_NAME_IND:
            {
                CsrBtCmReadRemoteNameInd *p = message;
                CsrPmemFree(p->friendlyName);
                p->friendlyName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND */
#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND
            case CSR_BT_CM_SDC_SEARCH_IND:
            {
                CsrBtCmSdcSearchInd *p = message;
                CsrPmemFree(p->serviceHandleList);
                p->serviceHandleList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_IND
            case CSR_BT_CM_L2CA_DATA_IND:
            {
                CsrBtCmL2caDataInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_IND */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND
            case CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND:
            {
                CsrBtCmL2caConnectionlessDataInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND */
#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND
            case CSR_BT_CM_INQUIRY_RESULT_IND:
            {
                CsrBtCmInquiryResultInd *p = message;
                CsrPmemFree(p->eirData);
                p->eirData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND */
#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM
            case CSR_BT_CM_SDC_SERVICE_SEARCH_CFM:
            {
                CsrBtCmSdcServiceSearchCfm *p = message;
                CsrPmemFree(p->recList);
                p->recList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND
            case CSR_BT_CM_BNEP_EXTENDED_DATA_IND:
            {
                CsrBtCmBnepExtendedDataInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND */
#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM
            case CSR_BT_CM_READ_REMOTE_NAME_CFM:
            {
                CsrBtCmReadRemoteNameCfm *p = message;
                CsrPmemFree(p->friendlyName);
                p->friendlyName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM */
            default:
            {
                CsrBtCmFreeHandcoded(prim);
                break;
            }
        } /* End switch */
    } /* End if */
    else
    {
        /* Unknown primitive type, exception handling */
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MODULE */
