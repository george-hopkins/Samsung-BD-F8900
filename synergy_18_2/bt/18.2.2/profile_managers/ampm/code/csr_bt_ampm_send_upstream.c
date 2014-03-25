/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

void CsrBtAmpmMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_AMPM_PRIM, msg);
}

void CsrBtAmpmUpstreamRegisterCfm(CsrSchedQid qid,
                                  CsrUint32 eventMask,
                                  CsrBtConnId btConnId,
                                  CsrBtDeviceAddr addr,
                                  CsrBtAmpmResult result)
{
    CsrBtAmpmRegisterCfm *prim;
    prim = (CsrBtAmpmRegisterCfm*)CsrPmemAlloc(sizeof(CsrBtAmpmRegisterCfm));
    prim->type = CSR_BT_AMPM_REGISTER_CFM;
    prim->eventMask = eventMask;
    prim->btConnId = btConnId;
    prim->addr = addr;
    prim->resultCode = result;
    prim->resultSupplier = CSR_BT_SUPPLIER_AMPM;
    CsrBtAmpmMessagePut(qid, prim);
}

void CsrBtAmpmUpstreamDeregisterCfm(CsrSchedQid qid,
                                    CsrBtConnId btConnId,
                                    CsrBtDeviceAddr addr,
                                    CsrBtAmpmResult result)
{
    CsrBtAmpmDeregisterCfm *prim;
    prim = (CsrBtAmpmDeregisterCfm*)CsrPmemAlloc(sizeof(CsrBtAmpmDeregisterCfm));
    prim->type = CSR_BT_AMPM_DEREGISTER_CFM;
    prim->btConnId = btConnId;
    prim->addr = addr;
    prim->resultCode = result;
    prim->resultSupplier = CSR_BT_SUPPLIER_AMPM;

    CsrBtAmpmMessagePut(qid, prim);
}

void CsrBtAmpmUpstreamControllerDone(CsrBtAmpmPrim type,
                                     CsrSchedQid qid,
                                     CsrBtConnId btConnId,
                                     CsrBtDeviceAddr addr,
                                     CsrUint8 ampsCount,
                                     CsrBtAmpmControllerEntry *amps,
                                     CsrBtAmpmResult resultCode)
{
    CsrBtAmpmControllerInd *prim;
    prim = (CsrBtAmpmControllerInd*)CsrPmemAlloc(sizeof(CsrBtAmpmControllerInd));

    prim->type = type;
    prim->btConnId = btConnId;
    prim->addr = addr;
    prim->ampsCount = ampsCount;
    prim->amps = amps;
    prim->resultCode = resultCode;
    prim->resultSupplier = CSR_BT_SUPPLIER_AMPM;

    CsrBtAmpmMessagePut(qid, prim);
}

void CsrBtAmpmUpstreamMoveDone(CsrBtAmpmPrim type,
                               CsrSchedQid qid,
                               CsrBtConnId btConnId,
                               CsrBtAmpController localId,
                               CsrBtAmpmResult resultCode,
                               CsrBtSupplier resultSupplier)
{
    CsrBtAmpmMoveCfm *prim = (CsrBtAmpmMoveCfm*)CsrPmemAlloc(sizeof(CsrBtAmpmMoveCfm));
    prim->type = type;
    prim->btConnId = btConnId;
    prim->localId = localId;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtAmpmMessagePut(qid, prim);
}

void CsrBtAmpmUpstreamPowerOnInd(CsrSchedQid qid,
                                 CsrBtAmpAmpControllerType ampType,
                                 CsrBtAmpController localId,
                                 CsrBool localInit)
{
    CsrBtAmpmPowerOnInd *prim;
    prim = (CsrBtAmpmPowerOnInd*)CsrPmemAlloc(sizeof(CsrBtAmpmPowerOnInd));
    prim->type = CSR_BT_AMPM_POWER_ON_IND;
    prim->ampType = ampType;
    prim->localId = localId;
    prim->localInit = localInit;
    
    CsrBtAmpmMessagePut(qid, prim);
}

void CsrBtAmpmUpstreamPalRegisterCfm(CsrSchedQid qid,
                                     CsrBtAmpmType ampType,
                                     CsrBtAmpmId ampId,
                                     CsrBtAmpmResult result,
                                     CsrUint32 flags)
{
    CsrBtAmpmPalRegisterCfm *prim;
    prim = (CsrBtAmpmPalRegisterCfm*)CsrPmemAlloc(sizeof(CsrBtAmpmPalRegisterCfm));
    prim->type = CSR_BT_AMPM_PAL_REGISTER_CFM;
    prim->ampType = ampType;
    prim->ampId = ampId;
    prim->result = result;
    prim->resultSupplier = CSR_BT_SUPPLIER_AMPM;
    prim->controlQid = CSR_BT_AMPM_IFACEQUEUE; /* HCI event receiver */
    prim->dataQid = DM_IFACEQUEUE; /* ACL data receiver */
    prim->flags = flags;

    CsrBtAmpmMessagePut(qid, prim);
}
