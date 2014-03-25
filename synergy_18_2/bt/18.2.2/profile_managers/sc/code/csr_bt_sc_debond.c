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
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_bond_unified.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_register.h"
#include "csr_bt_sc_db.h"
#include "csr_bt_sd_private_prim.h"
#include "csr_bt_sd_private_lib.h"

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDebondReqIdleS
*
*  DESCRIPTION
*      Remove a bond by:
*       1) Remove the link key in the host controller
*       2) Remove any data in the registration database in SM
*       3) Clean the NVS
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDebondReqIdleS(ScInstanceData_t *scData)
{
    CsrBtScDebondReq  *scPrim;
    CsrUint8 deleteFlag;

    scPrim = (CsrBtScDebondReq *)scData->recvMsgP;

    deleteFlag = CsrBtBdAddrEqZero(&scPrim->deviceAddr) ? DELETE_ALL : DELETE_BDADDR;

    scData->appHandle      = scPrim->phandle;
    scData->restoreMsgFlag = FALSE;
    scData->deviceAddr     = scPrim->deviceAddr;
    SC_CHANGE_STATE(&scData->state, SC_ST_PENDING);

    CsrBtCmScDmDeleteStoredLinkKeyReq(scPrim->deviceAddr, deleteFlag);
}

void CsrBtScDmHciDeleteStoredLinkKeyCompletePendingS(ScInstanceData_t *scData)
{
    DM_HCI_DELETE_STORED_LINK_KEY_CFM_T *dmPrim;

    dmPrim = (DM_HCI_DELETE_STORED_LINK_KEY_CFM_T *)scData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        CsrBtScDbDeviceRecord record;
        if (CsrBtBdAddrEqZero(&scData->deviceAddr) &&
            CsrBtScDbReadFirst(&record))
        {
            /* We're purging all device in the entire stack */
            CsrBtCmScDmRemoveDeviceReq(record.deviceAddr,
#ifdef CSR_BT_LE_ENABLE
                                       record.addressType
#else
                                       CSR_BT_ADDR_PUBLIC
#endif
                                       );
            CsrBtScScDbClose();
            CsrBtScDbRemove(&record.deviceAddr);
        }
        else
        {
            CsrBtCmScDmRemoveDeviceReq(scData->deviceAddr,
                                       CSR_BT_SC_ADDR_TYPE(scData));
            CsrBtScDbRemove(&scData->deviceAddr);
        }
    }
    else
    {
        CsrBtScNotifyUpperDebondCfm(scData, CSR_BT_RESULT_CODE_SC_INTERNAL_ERROR);
        CsrBtScResetInstData(scData);
    }
}

void CsrBtScDmRemoveDeviceCfmPendingS(ScInstanceData_t *scData)
{
    CsrBtScDbDeviceRecord record;

    if (CsrBtBdAddrEqZero(&scData->deviceAddr) &&
        CsrBtScDbReadFirst(&record))
    {
        /* We're purging all device in the entire stack */
        CsrBtCmScDmRemoveDeviceReq(record.deviceAddr,
#ifdef CSR_BT_LE_ENABLE
                                   record.addressType
#else
                                   CSR_BT_ADDR_PUBLIC
#endif
                                   );
        CsrBtScScDbClose();
        CsrBtScDbRemove(&record.deviceAddr);
			printf("[CSR SC_DB REMOVE] CsrBtScDmRemoveDeviceCfmPendingS  333\n");
    }
    else
    {
        CsrBtScNotifyUpperDebondCfm(scData,CSR_BT_RESULT_CODE_SC_SUCCESS);
        CsrBtScResetInstData(scData);
    }
}


