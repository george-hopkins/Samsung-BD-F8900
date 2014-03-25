/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_bt_lsl_prim.h"
#include "csr_bt_dunc_prim.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

void * ConvertPrimToLsl(void * message, CsrUint16 event,CsrUint8 state)
{
    switch(event)
    {
        case CSR_BT_DUNC_PRIM:
            {
                CsrBtDuncPrim type;
                type = *((CsrBtDuncPrim *) message);

                switch(type)
                {
                    case CSR_BT_DUNC_DATA_CFM:
                        {
                            /* not the same structure - hence create a new and free old */
                            CsrBtLslDataCfm * lslCfm;
                            CsrBtDuncDataCfm * dataCfm;

                            dataCfm = (CsrBtDuncDataCfm *) message;
                            lslCfm = (CsrBtLslDataCfm *) CsrPmemAlloc(sizeof(CsrBtLslDataCfm));
                            lslCfm->type = CSR_BT_LSL_DATA_CFM;
                            CsrPmemFree(dataCfm);
                            return (void *)lslCfm;
                            break;
                        }

                    case CSR_BT_DUNC_DATA_IND:
                        {
                            /* not the same structure - hence create a new */
                            CsrBtDuncDataInd * dataInd;
                            CsrBtLslDataInd * lslInd;

                            dataInd = (CsrBtDuncDataInd *) message;
                            lslInd = (CsrBtLslDataInd *) CsrPmemAlloc(sizeof(CsrBtLslDataInd));
                            lslInd->type = CSR_BT_LSL_DATA_IND;
                            lslInd->payload = dataInd->data;
                            lslInd->length = dataInd->dataLength;
                            CsrPmemFree(dataInd);
                            return (void  *)lslInd;
                            break;
                        }

                    case CSR_BT_DUNC_STATUS_IND:
                        {
                            /* not the same structure - hence create a new */
                            CsrBtDuncStatusInd * statusInd;
                            CsrBtLslStatusInd * lslInd;

                            statusInd = (CsrBtDuncStatusInd *) message;
                            lslInd = (CsrBtLslStatusInd *) CsrPmemAlloc(sizeof(CsrBtLslStatusInd));
                            lslInd->type = CSR_BT_LSL_STATUS_IND;
                            lslInd->maxPayloadLength = statusInd->maxMsgSize;
                            lslInd->reason = 0xFF; /* ignore */
                            lslInd->connected = statusInd->connected;
                            CsrPmemFree(statusInd);
                            return (void *)lslInd;
                            break;
                        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    default:
                        {
                            CsrStateEventException("PPP",
                                                    CSR_BT_DUNC_PRIM,
                                                    type,
                                                    state);
                            break;
                        }
#endif
                }
                break;
            }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                CsrStateEventException("PPP", event, 0,state);
                break;
            }
#endif
    }
    return NULL;
}
