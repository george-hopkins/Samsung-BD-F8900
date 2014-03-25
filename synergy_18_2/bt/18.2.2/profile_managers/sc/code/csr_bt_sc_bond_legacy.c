/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_bond_unified.h"
#include "csr_bt_sc_bond_legacy.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_main.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_sc_ui.h"

static void csrBtScTimerFireRemoteBondingS(CsrUint16 mi, ScInstanceData_t *scData)
{
    CSR_UNUSED(mi);
    scData->bondData.wdTimer = 0;
    CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
}

void CsrBtScStartRemoteBondTimer(ScInstanceData_t *scData)
{
    CsrBtScStopBondTimer(scData);
    scData->bondData.wdTimer = CsrSchedTimerSet(SC_INTERNAL_SUPERVISION_TIMER,
                                              (void (*) (CsrUint16, void *)) csrBtScTimerFireRemoteBondingS,
                                              CSR_BT_SC_IFACEQUEUE,
                                              scData);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScAclPresent
*
*  DESCRIPTION
*      Determine whether an ACL to the given device is present.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
CsrBool CsrBtScAclPresent(ScInstanceData_t *scData, BD_ADDR_T bdAddr, CsrUintFast8 *index)
{
    CsrUintFast8 i;

    *index = 0;
 
    for (i = 0 ; i < MAX_NO_OF_ACL ; i++)
    {
        if (CsrBtBdAddrEq(&bdAddr, &(scData->aclList[i].deviceAddr)))
        {
            *index = i;
            return TRUE;
        }
    }
    return FALSE;
}

void CsrBtScAddAcl2List(ScInstanceData_t *scData, BD_ADDR_T bdAddr, CsrBtClassOfDevice cod)
{
    /* Find available room for new bdAddr, if none return FALSE */
    CsrUintFast8 i;
    BD_ADDR_T zeroAddr;

    bd_addr_zero(&zeroAddr);

    for (i = 0 ; i < MAX_NO_OF_ACL ; i++)
    {
        if (CsrBtBdAddrEq(&(scData->aclList[i].deviceAddr), &zeroAddr))
        {
            CsrBtBdAddrCopy(&(scData->aclList[i].deviceAddr), &bdAddr);
            scData->aclList[i].cod = cod;
            return;
        }
    }
    return;
}

void CsrBtScRemoveAclFromList(ScInstanceData_t *scData, BD_ADDR_T bdAddr)
{
    /* Remove bdAddr from ACL list */
    CsrUintFast8 i;

    for (i = 0 ; i < MAX_NO_OF_ACL ; i++)
    {
        if (CsrBtBdAddrEq(&bdAddr, &(scData->aclList[i].deviceAddr)))
        {
            bd_addr_zero(&(scData->aclList[i].deviceAddr));
            scData->aclList[i].cod = 0;
            return;
        }
    }
    return;
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScRemotePairingFailed
*
*  DESCRIPTION
*      We get here if an ACL was closed/opened during bonding. We need to find
*      out whether that's OK.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScRemotePairingFailed(ScInstanceData_t *scData)
{
    switch(scData->bondData.dmState)
    {
        case DM_PENDING:
        case DM_AUTHEN_RECV:
        case DM_AUTHEN_RESP:
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->mainAppHandle); /* allow reset */
            break;

        case DM_BOND_COMPLETE:
            /* SSP Notification: The app is (n)acking the bond procedure even though
             * bonding has already completed.  The fact that the ACL goes
             * down while this takes _is_ expected. */
            break;

        default:
            CsrBtScStateEventException(DM_PRIM, scData->pendingMsgType, scData->bondData.dmState);
            break;
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCmAclConnectIndAnyS
*
*  DESCRIPTION
*      ACL has been opened. Update the list.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCmAclConnectIndAnyS(ScInstanceData_t *scData)
{
    CsrBtCmAclConnectInd *prim;

    prim = (CsrBtCmAclConnectInd *) scData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtScAddAcl2List(scData, prim->deviceAddr, prim->cod);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCmAclConnectIndRemoteBondingS
*
*  DESCRIPTION
*      Arrival of this primitive means that bonding was completed successfully.
*      Determine whether we have a valid linkkey.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCmAclConnectIndRemoteBondingS(ScInstanceData_t *scData)
{
    CsrBtCmAclConnectInd *prim;

    prim = (CsrBtCmAclConnectInd *) scData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtScAddAcl2List(scData, prim->deviceAddr, prim->cod);
    }

    if (scData->pendingMsgType == MSG_TYPE_LEGACY &&
        CsrBtBdAddrEq(&prim->deviceAddr,&scData->deviceAddr))
    {
        if (scData->bondData.linkKeyType != HCI_KEY_TYPE_UNKNOWN)
        {
            CsrBtScNotifyUpperBondIndSimple(scData,
                                            scData->mainAppHandle,
                                            CSR_BT_RESULT_CODE_SC_SUCCESS,
                                            HCI_SUCCESS,
                                            scData->addDevice);
            CsrBtScPairingSuccessUpdateDeviceReset(scData);
        }
        else
        {
            CsrBtScRemotePairingFailed(scData);
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCmAclDisconnectIndAnyS
*
*  DESCRIPTION
*      ACL has been closed. Update the list.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCmAclDisconnectIndAnyS(ScInstanceData_t *scData)
{
    CsrBtCmAclDisconnectInd *prim;

    prim = (CsrBtCmAclDisconnectInd *) scData->recvMsgP;

    CsrBtScRemoveAclFromList(scData, prim->deviceAddr);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCmAclDisconnectIndRemoteBondingS
*
*  DESCRIPTION
*       The remote side disconnected while pairing which means trouble.
*       Note that we never get this far if pairing was successful.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCmAclDisconnectIndRemoteBondingS(ScInstanceData_t *scData)
{
    CsrBtCmAclDisconnectInd *prim;

    prim = (CsrBtCmAclDisconnectInd *) scData->recvMsgP;

    CsrBtScRemoveAclFromList(scData, prim->deviceAddr);

    if (scData->pendingMsgType == MSG_TYPE_LEGACY &&
        CsrBtBdAddrEq(&prim->deviceAddr,&scData->deviceAddr))
    {
        CsrBtScRemotePairingFailed(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmAccessIndRemoteBondingS
*
*  DESCRIPTION
*      Whenever a incoming channel establishment fails, we get notified. This is
*      our queue to stop the pairing procedure and notify the app.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmAccessIndRemoteBondingS(ScInstanceData_t *scData)
{
    DM_SM_ACCESS_IND_T *prim;

    prim = (DM_SM_ACCESS_IND_T *)scData->recvMsgP;

    if (scData->pendingMsgType == MSG_TYPE_LEGACY &&
        CsrBtBdAddrEq(&prim->conn_setup.connection.addrt.addr, &scData->deviceAddr) &&
        prim->status != HCI_SUCCESS &&
        prim->conn_setup.incoming == TRUE)
    {
        CsrBtScRemotePairingFailed(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmHciCreateConnectionCancelCompleteRemoteBonding
*
*  DESCRIPTION
*      We need to handle this because the app can choose the cancel the pairing
*      behind our back by disconnecting the ACL link.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmHciCreateConnectionCancelCompleteRemoteBonding(ScInstanceData_t *scData)
{
    DM_HCI_CREATE_CONNECTION_CANCEL_CFM_T *prim;

    prim = (DM_HCI_CREATE_CONNECTION_CANCEL_CFM_T *)scData->recvMsgP;

    if (scData->pendingMsgType == MSG_TYPE_LEGACY &&
        CsrBtBdAddrEq(&prim->bd_addr,&scData->deviceAddr))
    {
        CsrBtScRemotePairingFailed(scData);
    }
}

