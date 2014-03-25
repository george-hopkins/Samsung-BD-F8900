/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_bondable_mode.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"

#ifdef CSR_BT_INSTALL_SC_MODE
/*----------------------------------------------------------------------------*
*  NAME
*      csrBtScBondableTimerAnyS
*
*  DESCRIPTION
*      Bondable mode timer has expired. Restore the default bondable mode.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
static void csrBtScBondableTimerAnyS(CsrUint16 mi, ScInstanceData_t *scData)
{
    CSR_UNUSED(mi);
    scData->bondData.bondableTid = 0;
    scData->bondData.bondable = scData->bondData.defaultBondable;

    CsrBtScModeIndSend(scData->mainAppHandle, scData->bondData.bondable);
}

static void csrBtScStartBondableTimer(ScInstanceData_t *scData)
{
    CsrBtScModeReq *scPrim;

    scPrim = (CsrBtScModeReq *) scData->recvMsgP;

    CsrBtScStopBondableTimer(scData);

    scData->bondData.bondableTid = CsrSchedTimerSet(scPrim->duration,
                                                  (void (*) (CsrUint16, void *)) csrBtScBondableTimerAnyS,
                                                  CSR_BT_SC_IFACEQUEUE,
                                                  scData);
}

void CsrBtScStopBondableTimer(ScInstanceData_t *scData)
{
    if (scData->bondData.bondableTid)
    {
        CsrSchedTimerCancel(scData->bondData.bondableTid, NULL, NULL);
        scData->bondData.bondableTid = 0;
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScModeReqIdleS
*
*  DESCRIPTION
*      Set idle mode of the host.
*
*  RETURNS/MODIFIES
*      Sends an indication whenever default or current mode is changed/extended.
*
*----------------------------------------------------------------------------*/
void CsrBtScModeReqIdleS(ScInstanceData_t *scData)
{
    CsrBtScModeReq *scPrim;

    scPrim = (CsrBtScModeReq *) scData->recvMsgP;

    if (scPrim->mode < CSR_BT_SEC_MODE_MAX &&
        (scData->bondData.bondable != scPrim->mode ||
         scData->bondData.defaultBondable != scPrim->mode))
    {
        scData->bondData.bondable = scPrim->mode;

        if (scPrim->duration == CSR_BT_INFINITE_TIME)
        {
            scData->bondData.defaultBondable = scPrim->mode;
            CsrBtScStopBondableTimer(scData);
        }
        else
        {
            csrBtScStartBondableTimer(scData);
        }

        CsrBtScModeIndSend(scData->mainAppHandle, scPrim->mode);
    }
}
#endif

/*************************************************************************************
 CsrBtScRejectDmSmPinRequestInd
************************************************************************************/
void CsrBtScRejectDmSmPinRequestInd(ScInstanceData_t *scData)
{
    DM_SM_PIN_REQUEST_IND_T *dmPrim;
    dmPrim = (DM_SM_PIN_REQUEST_IND_T *)scData->recvMsgP;
    CsrBtCmScDmPinRequestNegRes(dmPrim->addrt.addr);
}

/*************************************************************************************
 CsrBtScRejectDmSmAuthoriseInd
************************************************************************************/
void CsrBtScRejectDmSmAuthoriseInd(ScInstanceData_t *scData)
{
    DM_SM_AUTHORISE_IND_T *dmPrim;
    dmPrim = (DM_SM_AUTHORISE_IND_T *)scData->recvMsgP;
    CsrBtCmScDmAuthoriseNegRes(dmPrim->cs.connection.addrt.addr,
                               dmPrim->cs.incoming,
                               dmPrim->cs.connection.service.channel,
                               dmPrim->cs.connection.service.protocol_id,
                               dmPrim->cs.connection.addrt.type);
}

/*************************************************************************************
 CsrBtScRejectDmSmIoCapabilityRequestInd
************************************************************************************/
void CsrBtScRejectDmSmIoCapabilityRequestInd(ScInstanceData_t *scData)
{
    DM_SM_IO_CAPABILITY_REQUEST_IND_T *dmPrim;
    dmPrim = (DM_SM_IO_CAPABILITY_REQUEST_IND_T*)scData->recvMsgP;
    CsrBtCmScDmIoCapabilityRequestNegRes(dmPrim->addrt.addr,
                                         HCI_ERROR_HOST_BUSY_PAIRING,
                                         dmPrim->addrt.type);
}

/*************************************************************************************
 CsrBtScIgnoreAnyS
************************************************************************************/
void CsrBtScIgnoreAnyS(ScInstanceData_t *scData)
{
    CSR_UNUSED(scData);
}

