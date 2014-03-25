/****************************************************************************

                (c) CSR plc 2008

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "dm_prim.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_main.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_authenticate.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_bond_unified.h"
#include "csr_bt_sc_bond_legacy.h"
#include "csr_bt_sc_bond_local.h"
#include "csr_bt_sc_bond_remote.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_sc_lib_upstream.h"

#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
void CsrBtScAuthenticateReqIdleS(ScInstanceData_t *scData)
{
    CsrBtScAuthenticateReq  *scPrim;

    scPrim                     = (CsrBtScAuthenticateReq *)scData->recvMsgP;
    scData->appHandle          = scPrim->phandle;
    scData->restoreMsgFlag     = FALSE;
    scData->bondData.initiator = INITIATOR_AUTHENTICATE;
    scData->accept             = TRUE;
    scData->addDevice          = TRUE;

    SC_CHANGE_STATE(&scData->state, SC_ST_AUTHENTICATE);
    SC_CHANGE_STATE(&scData->bondData.dmState, DM_PENDING);
    SC_CHANGE_STATE(&scData->bondData.scState, SC_ACTIVE);

    CsrBtBdAddrCopy(&scData->deviceAddr, &scPrim->deviceAddr);

    CsrBtCmScDmAuthenticateReq(scPrim->deviceAddr);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScDmSmLinkKeyIndAuthenticateS
 *
 *  DESCRIPTION
 *      A link key has been generated due to an authentication request
 *
 *  RETURNS/MODIFIES
 *
 *----------------------------------------------------------------------------*/
#if defined(CSR_BT_INSTALL_SC_ENCRYPTION) || defined(CSR_BT_INSTALL_SC_AUTHENTICATE)
void CsrBtScDmSmLinkKeyIndAuthenticateS(ScInstanceData_t *scData)
{
    DM_SM_LINK_KEY_IND_T    *dmPrim;

    dmPrim = (DM_SM_LINK_KEY_IND_T *)scData->recvMsgP;

    CsrBtScMapToDmKeyType(scData);

    CsrMemCpy(&scData->bondData.linkKey, &dmPrim->key, SIZE_LINK_KEY);
    scData->bondData.linkKeyType = dmPrim->key_type;

    if (!CsrBtBdAddrEq(&dmPrim->bd_addr, &scData->deviceAddr))
    {
        CsrBtScDmSmLinkKeyIndIdleS(scData);
    }
}
#endif

#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScDmSmAuthenticateCfmAuthenticateS
 *
 *  DESCRIPTION
 *      Authentication completed
 *
 *  RETURNS/MODIFIES
 *
 *----------------------------------------------------------------------------*/
void CsrBtScDmSmAuthenticateCfmAuthenticateS(ScInstanceData_t *scData)
{
    DM_SM_AUTHENTICATE_CFM_T *dmPrim;
    CsrUint8 result;

    dmPrim = (DM_SM_AUTHENTICATE_CFM_T *) scData->recvMsgP;

    result = dmPrim->success ? CSR_BT_RESULT_CODE_SC_SUCCESS : CSR_BT_RESULT_CODE_SC_BONDING_FAILED;

    dmPrim = (DM_SM_AUTHENTICATE_CFM_T *)scData->recvMsgP;

    if (dmPrim->success && scData->accept)
    {
        CsrBtScNotifyUpperBondIndSimple(scData,
                                        scData->appHandle,
                                        CSR_BT_RESULT_CODE_SC_SUCCESS,
                                        HCI_SUCCESS,
                                        scData->addDevice);
                                    
        CsrBtScAuthenticateCfmSend(scData->appHandle, result,
                                   CSR_BT_SUPPLIER_SC,
                                   scData->deviceAddr,
                                   scData->addressType);

        CsrBtScPairingSuccessUpdateDeviceReset(scData);
    }
    else
    {
        CsrBtScNotifyUpperBondIndSimple(scData,
                                        scData->appHandle,
                                        CSR_BT_RESULT_CODE_SC_BONDING_FAILED,
                                        scData->hciStatus,
                                        FALSE);
        CsrBtScAuthenticateCfmSend(scData->appHandle, result, CSR_BT_SUPPLIER_SC,
                                   scData->deviceAddr,
                                   scData->addressType);

        CsrBtScPairingFailedCleanupDeviceReset(scData);
    }
}
#endif
