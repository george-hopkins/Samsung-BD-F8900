/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_lib.h"
#include "csr_bt_ampm_main.h"
#include "dm_prim.h"
#include "l2caplib.h"
#include "csr_bt_util.h"

/* These internal functions are used by WIFI */
/* Construct the CsrBtAmpmPalRegisterReq primitive */
CsrBtAmpmPalRegisterReq *CsrBtAmpmPalRegisterReq_struct(CsrBtAmpmType ampType,
                                                        CsrSchedQid controlQid,
                                                        CsrSchedQid dataQid,
                                                        CsrUint32 flags)
{
    CsrBtAmpmPalRegisterReq *prim;
    prim = (CsrBtAmpmPalRegisterReq*)CsrPmemAlloc(sizeof(CsrBtAmpmPalRegisterReq));

    prim->type = CSR_BT_AMPM_PAL_REGISTER_REQ;
    prim->ampType = ampType;
    prim->controlQid = controlQid;
    prim->dataQid = dataQid;
    prim->flags = flags;

    return prim;
}

/* Construct the CsrBtAmpmPalDeregisterReq primitive */
CsrBtAmpmPalDeregisterReq *CsrBtAmpmPalDeregisterReq_struct(CsrBtAmpmId ampId)
{
    CsrBtAmpmPalDeregisterReq *prim;
    prim = (CsrBtAmpmPalDeregisterReq*)CsrPmemAlloc(sizeof(CsrBtAmpmPalDeregisterReq));
    prim->type = CSR_BT_AMPM_PAL_DEREGISTER_REQ;
    prim->ampId = ampId;

    return prim;
}

/* Construct the CsrBtAmpmPalHciEventReq primitive */
CsrBtAmpmPalHciEventReq *CsrBtAmpmPalHciEventReq_struct(CsrUint8 ampId,
                                                        CsrUint16 length,
                                                        CsrUint8 *data)
{
    CsrBtAmpmPalHciEventReq *prim;
    prim = (CsrBtAmpmPalHciEventReq*)CsrPmemAlloc(sizeof(CsrBtAmpmPalHciEventReq));

    prim->type = CSR_BT_AMPM_PAL_HCI_EVENT_REQ;
    prim->ampId = ampId;
    prim->length = length;
    prim->data = data;

    return prim;
}
/* These internal functions are used by WIFI  -  END */

void CsrBtAmpmMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_AMPM_IFACEQUEUE, CSR_BT_AMPM_PRIM, msg);
}

/* Construct the CsrBtAmpmPrfMultiConnReq primitive */
void CsrBtAmpmPrfMultiConnReqSend(CsrBtConnId master, CsrUint8 slavesCount, const CsrBtConnId *slaves)
{
    CsrBtAmpmPrfMultiConnReq *prim = (CsrBtAmpmPrfMultiConnReq*)CsrPmemAlloc(sizeof(CsrBtAmpmPrfMultiConnReq));
    prim->type = CSR_BT_AMPM_PRF_MULTI_CONN_REQ;
    prim->master = master;
    prim->slavesCount = slavesCount;
    prim->slaves = (CsrBtConnId*)((slavesCount > 0) ? CsrMemDup(slaves, sizeof(CsrBtConnId)*slavesCount) : NULL);

    CsrBtAmpmMsgTransport(prim);
}
