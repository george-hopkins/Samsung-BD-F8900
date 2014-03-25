/****************************************************************************

                (c) CSR plc 2008

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
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
#include "csr_bt_sc_encrypt.h"
#include "csr_bt_sc_authenticate.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_ui.h"

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScEncryptionReqIdleS
*
*  DESCRIPTION
*      Set the encryption mode on the given BD_ADDR_T.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScEncryptionReqIdleS(ScInstanceData_t *scData)
{
    CsrBtScEncryptionReq *scPrim;
    CsrBtScDbDeviceRecord      scDbDeviceInfo;

    scPrim = (CsrBtScEncryptionReq *) scData->recvMsgP;

    scData->appHandle          = scPrim->phandle;
    scData->restoreMsgFlag     = FALSE;
    SC_CHANGE_STATE(&scData->state, SC_ST_ENCRYPT);
    SC_CHANGE_STATE(&scData->bondData.scState, SC_ACTIVE);
    scData->bondData.initiator = INITIATOR_ENCRYPT;

    CsrBtBdAddrCopy(&scData->deviceAddr, &scPrim->deviceAddr);

    if (CsrBtScDbRead(&scPrim->deviceAddr,&scDbDeviceInfo) &&
        scDbDeviceInfo.linkkeyValid)
    {
        CsrBtCmScDmEncryptionReq(scPrim->phandle, scPrim->deviceAddr, scPrim->encEnable);
    }
    else
    {
        scData->encEnable = scPrim->encEnable;
        CsrBtCmScDmAuthenticateReq(scPrim->deviceAddr);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmAuthenticateCfmEncryptionS
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmAuthenticateCfmEncryptionS(ScInstanceData_t *scData)
{
    DM_SM_AUTHENTICATE_CFM_T *dmPrim;

    dmPrim = (DM_SM_AUTHENTICATE_CFM_T *)scData->recvMsgP;

    if (dmPrim->success)
    {
        if (scData->accept)
        {
            if (scData->addDevice)
            {/* Save in SCDB if requested */
                CsrBtScScDbUpdate(scData);
            }

            /* Start encryption */
            CsrBtCmScDmEncryptionReq(scData->appHandle, scData->deviceAddr, scData->encEnable);
        }
        else
        {
            CsrBtScEncryptionCfmSend(scData->appHandle, CSR_BT_RESULT_CODE_SC_BONDING_FAILED, CSR_BT_SUPPLIER_SC, FALSE, scData->deviceAddr);
            CsrBtScResetInstData(scData);
        }
    }
    else
    {
        CsrBtScEncryptionCfmSend(scData->appHandle, CSR_BT_RESULT_CODE_SC_BONDING_FAILED, CSR_BT_SUPPLIER_SC, FALSE, scData->deviceAddr);
        CsrBtScResetInstData(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmEncryptCfmEncryptionS
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmEncryptCfmEncryptionS(ScInstanceData_t *scData)
{
    DM_SM_ENCRYPT_CFM_T *dmPrim;
    CsrUint8 result;

    dmPrim = (DM_SM_ENCRYPT_CFM_T *) scData->recvMsgP;

    result = dmPrim->success == TRUE ? CSR_BT_RESULT_CODE_SC_SUCCESS : CSR_BT_RESULT_CODE_SC_ERROR_UNSPECIFIED;

    CsrBtScEncryptionCfmSend( scData->appHandle, result, CSR_BT_SUPPLIER_SC, dmPrim->encrypted, dmPrim->bd_addr);

    CsrBtScResetInstData(scData);
}
#endif /*CSR_BT_INSTALL_SC_ENCRYPTION*/

