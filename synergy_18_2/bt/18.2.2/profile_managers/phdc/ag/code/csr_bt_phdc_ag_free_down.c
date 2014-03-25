/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_phdc_ag_lib.h"
#include "csr_bt_phdc_ag_prim.h"

void CsrBtPhdcAgFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_PHDC_AG_PRIM)
    {
        CsrBtPhdcAgPrim *prim = (CsrBtPhdcAgPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES
            case CSR_BT_PHDC_AG_DIM_ATTRIB_RES:
            {
                CsrBtPhdcAgDimAttribRes *p = message;
                CsrPmemFree(p->attribInfo);
                p->attribInfo = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ
            case CSR_BT_PHDC_AG_DATA_REQ:
            {
                CsrBtPhdcAgDataReq *p = message;
                CsrPmemFree(p->data.data);
                p->data.data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES
            case CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES:
            {
                CsrBtPhdcAgConfigureEndpointRes *p = message;
                CsrPmemFree(p->description);
                p->description = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES
            case CSR_BT_PHDC_AG_DIM_OBJECT_RES:
            {
                CsrBtPhdcAgDimObjectRes *p = message;
                CsrPmemFree(p->dataValue);
                p->dataValue = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ
            case CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ:
            {
                CsrBtPhdcAgConfigureMdsReq *p = message;
                CsrPmemFree(p->manufacturer);
                p->manufacturer = NULL;
                CsrPmemFree(p->modelNumber);
                p->modelNumber = NULL;
                CsrPmemFree(p->serialNumber);
                p->serialNumber = NULL;
                CsrPmemFree(p->fwVersion);
                p->fwVersion = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ
            case CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ:
            {
                CsrBtPhdcAgConfigureEndpointReq *p = message;
                CsrPmemFree(p->serviceName);
                p->serviceName = NULL;
                CsrPmemFree(p->serviceDescription);
                p->serviceDescription = NULL;
                CsrPmemFree(p->providerName);
                p->providerName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ */
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
#endif /* EXCLUDE_CSR_BT_PHDC_AG_MODULE */
