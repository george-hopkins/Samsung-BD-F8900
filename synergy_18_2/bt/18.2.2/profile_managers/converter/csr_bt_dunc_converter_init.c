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
#include "csr_bt_dunc_converter_init.h"
#ifndef EXCLUDE_CSR_BT_DUNC_MODULE
#include "csr_bt_dunc_serialize.h"
#include "csr_bt_dunc_prim.h"

static CsrMsgConvMsgEntry csr_bt_dunc_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_DUNC_CANCEL_CONNECT_REQ
    { CSR_BT_DUNC_CANCEL_CONNECT_REQ, CsrBtDuncCancelConnectReqSizeof, CsrBtDuncCancelConnectReqSer, CsrBtDuncCancelConnectReqDes, CsrBtDuncCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ
    { CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ, CsrBtDuncChangeDataPathStatusReqSizeof, CsrBtDuncChangeDataPathStatusReqSer, CsrBtDuncChangeDataPathStatusReqDes, CsrBtDuncChangeDataPathStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_DUNC_CONNECT_CFM
    { CSR_BT_DUNC_CONNECT_CFM, CsrBtDuncConnectCfmSizeof, CsrBtDuncConnectCfmSer, CsrBtDuncConnectCfmDes, CsrBtDuncConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_DUNC_CONNECT_REQ
    { CSR_BT_DUNC_CONNECT_REQ, CsrBtDuncConnectReqSizeof, CsrBtDuncConnectReqSer, CsrBtDuncConnectReqDes, CsrBtDuncConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_DUNC_CONTROL_IND
    { CSR_BT_DUNC_CONTROL_IND, CsrBtDuncControlIndSizeof, CsrBtDuncControlIndSer, CsrBtDuncControlIndDes, CsrBtDuncControlIndSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_CONTROL_IND */
#ifndef EXCLUDE_CSR_BT_DUNC_CONTROL_REQ
    { CSR_BT_DUNC_CONTROL_REQ, CsrBtDuncControlReqSizeof, CsrBtDuncControlReqSer, CsrBtDuncControlReqDes, CsrBtDuncControlReqSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_CONTROL_REQ */
#ifndef EXCLUDE_CSR_BT_DUNC_DATA_CFM
    { CSR_BT_DUNC_DATA_CFM, CsrBtDuncDataCfmSizeof, CsrBtDuncDataCfmSer, CsrBtDuncDataCfmDes, CsrBtDuncDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_DUNC_DATA_IND
    { CSR_BT_DUNC_DATA_IND, CsrBtDuncDataIndSizeof, CsrBtDuncDataIndSer, CsrBtDuncDataIndDes, CsrBtDuncDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_IND */
#ifndef EXCLUDE_CSR_BT_DUNC_DATA_PATH_STATUS_IND
    { CSR_BT_DUNC_DATA_PATH_STATUS_IND, CsrBtDuncDataPathStatusIndSizeof, CsrBtDuncDataPathStatusIndSer, CsrBtDuncDataPathStatusIndDes, CsrBtDuncDataPathStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_PATH_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_DUNC_DATA_REQ
    { CSR_BT_DUNC_DATA_REQ, CsrBtDuncDataReqSizeof, CsrBtDuncDataReqSer, CsrBtDuncDataReqDes, CsrBtDuncDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_DUNC_DATA_RES
    { CSR_BT_DUNC_DATA_RES, CsrBtDuncDataResSizeof, CsrBtDuncDataResSer, CsrBtDuncDataResDes, CsrBtDuncDataResSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_RES */
#ifndef EXCLUDE_CSR_BT_DUNC_DISCONNECT_IND
    { CSR_BT_DUNC_DISCONNECT_IND, CsrBtDuncDisconnectIndSizeof, CsrBtDuncDisconnectIndSer, CsrBtDuncDisconnectIndDes, CsrBtDuncDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_DUNC_DISCONNECT_REQ
    { CSR_BT_DUNC_DISCONNECT_REQ, CsrBtDuncDisconnectReqSizeof, CsrBtDuncDisconnectReqSer, CsrBtDuncDisconnectReqDes, CsrBtDuncDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_DUNC_HOUSE_CLEANING
    { CSR_BT_DUNC_HOUSE_CLEANING, CsrBtDuncHouseCleaningSizeof, CsrBtDuncHouseCleaningSer, CsrBtDuncHouseCleaningDes, CsrBtDuncHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_CFM
    { CSR_BT_DUNC_PORTNEG_CFM, CsrBtDuncPortnegCfmSizeof, CsrBtDuncPortnegCfmSer, CsrBtDuncPortnegCfmDes, CsrBtDuncPortnegCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_CFM */
#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_IND
    { CSR_BT_DUNC_PORTNEG_IND, CsrBtDuncPortnegIndSizeof, CsrBtDuncPortnegIndSer, CsrBtDuncPortnegIndDes, CsrBtDuncPortnegIndSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_IND */
#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_REQ
    { CSR_BT_DUNC_PORTNEG_REQ, CsrBtDuncPortnegReqSizeof, CsrBtDuncPortnegReqSer, CsrBtDuncPortnegReqDes, CsrBtDuncPortnegReqSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_REQ */
#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_RES
    { CSR_BT_DUNC_PORTNEG_RES, CsrBtDuncPortnegResSizeof, CsrBtDuncPortnegResSer, CsrBtDuncPortnegResDes, CsrBtDuncPortnegResSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_RES */
#ifndef EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM
    { CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM, CsrBtDuncRegisterDataPathHandleCfmSizeof, CsrBtDuncRegisterDataPathHandleCfmSer, CsrBtDuncRegisterDataPathHandleCfmDes, CsrBtDuncRegisterDataPathHandleCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM */
#ifndef EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ
    { CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ, CsrBtDuncRegisterDataPathHandleReqSizeof, CsrBtDuncRegisterDataPathHandleReqSer, CsrBtDuncRegisterDataPathHandleReqDes, CsrBtDuncRegisterDataPathHandleReqSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ */
#ifndef EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_CFM
    { CSR_BT_DUNC_SECURITY_OUT_CFM, CsrBtDuncSecurityOutCfmSizeof, CsrBtDuncSecurityOutCfmSer, CsrBtDuncSecurityOutCfmDes, CsrBtDuncSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_REQ
    { CSR_BT_DUNC_SECURITY_OUT_REQ, CsrBtDuncSecurityOutReqSizeof, CsrBtDuncSecurityOutReqSer, CsrBtDuncSecurityOutReqDes, CsrBtDuncSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_DUNC_STATUS_IND
    { CSR_BT_DUNC_STATUS_IND, CsrBtDuncStatusIndSizeof, CsrBtDuncStatusIndSer, CsrBtDuncStatusIndDes, CsrBtDuncStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_DUNC_STATUS_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtDuncConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_DUNC_PRIM, csr_bt_dunc_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_dunc_conv_info = {
    CSR_BT_DUNC_PRIM,
    "CSR_BT_DUNC_PRIM",
    csr_bt_dunc_conv_lut
};

CsrLogPrimitiveInformation* CsrBtDuncTechInfoGet(void)
{
    return &csr_bt_dunc_conv_info;
}
#endif
#endif

