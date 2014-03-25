/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_JSR82_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_jsr82_lib.h"
#include "csr_bt_jsr82_prim.h"

void CsrBtJsr82FreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_JSR82_PRIM)
    {
        CsrBtJsr82Prim *prim = (CsrBtJsr82Prim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM
            case CSR_BT_JSR82_L2CA_RX_DATA_CFM:
            {
                CsrBtJsr82L2caRxDataCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM
            case CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM:
            {
                CsrBtJsr82RfcReceiveDataCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM
            case CSR_BT_JSR82_SR_POPULATE_RECORD_CFM:
            {
                CsrBtJsr82SrPopulateRecordCfm *p = message;
                CsrPmemFree(p->attributes);
                p->attributes = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM
            case CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM:
            {
                CsrBtJsr82RdGetFriendlyNameCfm *p = message;
                CsrPmemFree(p->remoteName);
                p->remoteName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM
            case CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM:
            {
                CsrBtJsr82DaRetrieveDevicesCfm *p = message;
                CsrPmemFree(p->devices);
                p->devices = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND
            case CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND:
            {
                CsrBtJsr82DlServicesDiscoveredInd *p = message;
                CsrPmemFree(p->attributes);
                p->attributes = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM
            case CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM:
            {
                CsrBtJsr82LdGetFriendlyNameCfm *p = message;
                CsrPmemFree(p->localName);
                p->localName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM */
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
#endif /* EXCLUDE_CSR_BT_JSR82_MODULE */
