/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_dg_converter_init.h"
#ifndef EXCLUDE_CSR_BT_DG_MODULE
#include "csr_bt_dg_serialize.h"
#include "csr_bt_dg_prim.h"

static CsrMsgConvMsgEntry csr_bt_dg_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_DG_ACTIVATE_REQ
    { CSR_BT_DG_ACTIVATE_REQ, CsrBtDgActivateReqSizeof, CsrBtDgActivateReqSer, CsrBtDgActivateReqDes, CsrBtDgActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_DG_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_DG_CONNECT_IND
    { CSR_BT_DG_CONNECT_IND, CsrBtDgConnectIndSizeof, CsrBtDgConnectIndSer, CsrBtDgConnectIndDes, CsrBtDgConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_DG_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_DG_CONTROL_IND
    { CSR_BT_DG_CONTROL_IND, CsrBtDgControlIndSizeof, CsrBtDgControlIndSer, CsrBtDgControlIndDes, CsrBtDgControlIndSerFree },
#endif /* EXCLUDE_CSR_BT_DG_CONTROL_IND */
#ifndef EXCLUDE_CSR_BT_DG_CONTROL_REQ
    { CSR_BT_DG_CONTROL_REQ, CsrBtDgControlReqSizeof, CsrBtDgControlReqSer, CsrBtDgControlReqDes, CsrBtDgControlReqSerFree },
#endif /* EXCLUDE_CSR_BT_DG_CONTROL_REQ */
#ifndef EXCLUDE_CSR_BT_DG_DATA_CFM
    { CSR_BT_DG_DATA_CFM, CsrBtDgDataCfmSizeof, CsrBtDgDataCfmSer, CsrBtDgDataCfmDes, CsrBtDgDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DG_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_DG_DATA_IND
    { CSR_BT_DG_DATA_IND, CsrBtDgDataIndSizeof, CsrBtDgDataIndSer, CsrBtDgDataIndDes, CsrBtDgDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_DG_DATA_IND */
#ifndef EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_IND
    { CSR_BT_DG_DATA_PATH_STATUS_IND, CsrBtDgDataPathStatusIndSizeof, CsrBtDgDataPathStatusIndSer, CsrBtDgDataPathStatusIndDes, CsrBtDgDataPathStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_REQ
    { CSR_BT_DG_DATA_PATH_STATUS_REQ, CsrBtDgDataPathStatusReqSizeof, CsrBtDgDataPathStatusReqSer, CsrBtDgDataPathStatusReqDes, CsrBtDgDataPathStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_DG_DATA_REQ
    { CSR_BT_DG_DATA_REQ, CsrBtDgDataReqSizeof, CsrBtDgDataReqSer, CsrBtDgDataReqDes, CsrBtDgDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_DG_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_DG_DATA_RES
    { CSR_BT_DG_DATA_RES, CsrBtDgDataResSizeof, CsrBtDgDataResSer, CsrBtDgDataResDes, CsrBtDgDataResSerFree },
#endif /* EXCLUDE_CSR_BT_DG_DATA_RES */
#ifndef EXCLUDE_CSR_BT_DG_DEACTIVATE_CFM
    { CSR_BT_DG_DEACTIVATE_CFM, CsrBtDgDeactivateCfmSizeof, CsrBtDgDeactivateCfmSer, CsrBtDgDeactivateCfmDes, CsrBtDgDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DG_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_DG_DEACTIVATE_REQ
    { CSR_BT_DG_DEACTIVATE_REQ, CsrBtDgDeactivateReqSizeof, CsrBtDgDeactivateReqSer, CsrBtDgDeactivateReqDes, CsrBtDgDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_DG_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_DG_DISCONNECT_IND
    { CSR_BT_DG_DISCONNECT_IND, CsrBtDgDisconnectIndSizeof, CsrBtDgDisconnectIndSer, CsrBtDgDisconnectIndDes, CsrBtDgDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_DG_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_DG_DISCONNECT_REQ
    { CSR_BT_DG_DISCONNECT_REQ, CsrBtDgDisconnectReqSizeof, CsrBtDgDisconnectReqSer, CsrBtDgDisconnectReqDes, CsrBtDgDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_DG_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_DG_HOUSE_CLEANING
    { CSR_BT_DG_HOUSE_CLEANING, CsrBtDgHouseCleaningSizeof, CsrBtDgHouseCleaningSer, CsrBtDgHouseCleaningDes, CsrBtDgHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_DG_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_CFM
    { CSR_BT_DG_PORTNEG_CFM, CsrBtDgPortnegCfmSizeof, CsrBtDgPortnegCfmSer, CsrBtDgPortnegCfmDes, CsrBtDgPortnegCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_CFM */
#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_IND
    { CSR_BT_DG_PORTNEG_IND, CsrBtDgPortnegIndSizeof, CsrBtDgPortnegIndSer, CsrBtDgPortnegIndDes, CsrBtDgPortnegIndSerFree },
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_IND */
#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_REQ
    { CSR_BT_DG_PORTNEG_REQ, CsrBtDgPortnegReqSizeof, CsrBtDgPortnegReqSer, CsrBtDgPortnegReqDes, CsrBtDgPortnegReqSerFree },
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_REQ */
#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_RES
    { CSR_BT_DG_PORTNEG_RES, CsrBtDgPortnegResSizeof, CsrBtDgPortnegResSer, CsrBtDgPortnegResDes, CsrBtDgPortnegResSerFree },
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_RES */
#ifndef EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM
    { CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM, CsrBtDgRegisterDataPathHandleCfmSizeof, CsrBtDgRegisterDataPathHandleCfmSer, CsrBtDgRegisterDataPathHandleCfmDes, CsrBtDgRegisterDataPathHandleCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM */
#ifndef EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ
    { CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ, CsrBtDgRegisterDataPathHandleReqSizeof, CsrBtDgRegisterDataPathHandleReqSer, CsrBtDgRegisterDataPathHandleReqDes, CsrBtDgRegisterDataPathHandleReqSerFree },
#endif /* EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ */
#ifndef EXCLUDE_CSR_BT_DG_SECURITY_IN_CFM
    { CSR_BT_DG_SECURITY_IN_CFM, CsrBtDgSecurityInCfmSizeof, CsrBtDgSecurityInCfmSer, CsrBtDgSecurityInCfmDes, CsrBtDgSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DG_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_DG_SECURITY_IN_REQ
    { CSR_BT_DG_SECURITY_IN_REQ, CsrBtDgSecurityInReqSizeof, CsrBtDgSecurityInReqSer, CsrBtDgSecurityInReqDes, CsrBtDgSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_DG_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_DG_STATUS_IND
    { CSR_BT_DG_STATUS_IND, CsrBtDgStatusIndSizeof, CsrBtDgStatusIndSer, CsrBtDgStatusIndDes, CsrBtDgStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_DG_STATUS_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtDgConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_DG_PRIM, csr_bt_dg_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_dg_conv_info = {
    CSR_BT_DG_PRIM,
    "CSR_BT_DG_PRIM",
    csr_bt_dg_conv_lut
};

CsrLogPrimitiveInformation* CsrBtDgTechInfoGet(void)
{
    return &csr_bt_dg_conv_info;
}
#endif
#endif

