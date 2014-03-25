/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#ifdef CSR_BT_INSTALL_SC_OOB
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
#include "csr_bt_sc_authorise.h"



/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScReadLocalOobDataReqIdleS
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScReadLocalOobDataReqIdleS(ScInstanceData_t *scData)
{
    CsrBtScReadLocalOobDataReq *scPrim;

    scPrim = (CsrBtScReadLocalOobDataReq *) scData->recvMsgP;

    scData->appHandle       = scPrim->phandle;
    scData->restoreMsgFlag = FALSE;
    SC_CHANGE_STATE(&scData->state, SC_ST_PENDING);

    CsrBtCmScDmReadLocalOobDataReq();
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScAddRemoteOobDataReqHandler
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScAddRemoteOobDataReqHandler(ScInstanceData_t *scData)
{
    CsrBtScAddRemoteOobDataReq *scPrim;

    scPrim = (CsrBtScAddRemoteOobDataReq *) scData->recvMsgP;

    scData->sspData.deviceAddr = scPrim->deviceAddr;
    CsrMemCpy(scData->sspData.oobHashC, scPrim->oobHashC, SIZE_OOB_DATA);
    CsrMemCpy(scData->sspData.oobRandR, scPrim->oobRandR, SIZE_OOB_DATA);
}


/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSmReadLocalOobDataCfmPendingS
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSmReadLocalOobDataCfmPendingS(ScInstanceData_t *scData)
{
    DM_SM_READ_LOCAL_OOB_DATA_CFM_T *dmPrim;
    CsrBtResultCode result;

    dmPrim = (DM_SM_READ_LOCAL_OOB_DATA_CFM_T *) scData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        result = CSR_BT_RESULT_CODE_SC_SUCCESS;
    }
    else
    {
        result = CSR_BT_RESULT_CODE_SC_ERROR_UNSPECIFIED;
        dmPrim->oob_hash_c = NULL;
        dmPrim->oob_rand_r = NULL;
    }

    CsrBtScReadLocalOobDataCfmSend(scData->localAddr,
                                   scData->appHandle,
                                   dmPrim->oob_hash_c,
                                   dmPrim->oob_rand_r,
                                   result,
                                   CSR_BT_SUPPLIER_SC,
                                   CSR_BT_ADDR_PUBLIC); /* local address, always public */

    /* Either these are already NULL, or we've transferred ownership
     * to the application */
    dmPrim->oob_hash_c = NULL;
    dmPrim->oob_rand_r = NULL;

    CsrBtScResetInstData(scData);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScHasOobDataFor
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
CsrBool CsrBtScHasOobDataFor(ScInstanceData_t *scData, CsrBtDeviceAddr deviceAddr)
{
    if (CsrBtBdAddrEq(&scData->sspData.deviceAddr, &deviceAddr) &&
        !CsrBtBdAddrEqZero(&scData->sspData.deviceAddr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


#endif /*INSTALL_OOB*/
