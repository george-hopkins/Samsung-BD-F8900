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

void CsrBtJsr82FreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_JSR82_PRIM)
    {
        CsrBtJsr82Prim *prim = (CsrBtJsr82Prim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ
            case CSR_BT_JSR82_DA_SELECT_SERVICE_REQ:
            {
                CsrBtJsr82DaSelectServiceReq *p = message;
                CsrPmemFree(p->requestedUuid);
                p->requestedUuid = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ
            case CSR_BT_JSR82_L2CA_TX_DATA_REQ:
            {
                CsrBtJsr82L2caTxDataReq *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ
            case CSR_BT_JSR82_LD_UPDATE_RECORD_REQ:
            {
                CsrBtJsr82LdUpdateRecordReq *p = message;
                CsrPmemFree(p->serviceRecord);
                p->serviceRecord = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ
            case CSR_BT_JSR82_SR_POPULATE_RECORD_REQ:
            {
                CsrBtJsr82SrPopulateRecordReq *p = message;
                CsrPmemFree(p->attrSet);
                p->attrSet = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ
            case CSR_BT_JSR82_RFC_SEND_DATA_REQ:
            {
                CsrBtJsr82RfcSendDataReq *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ
            case CSR_BT_JSR82_SR_REGISTER_RECORD_REQ:
            {
                CsrBtJsr82SrRegisterRecordReq *p = message;
                CsrPmemFree(p->serviceRecord);
                p->serviceRecord = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ
            case CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ:
            {
                CsrBtJsr82DaSearchServicesReq *p = message;
                CsrPmemFree(p->uuidSet);
                p->uuidSet = NULL;
                CsrPmemFree(p->attrSet);
                p->attrSet = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */
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
