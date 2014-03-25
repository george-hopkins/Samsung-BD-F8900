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
#include "csr_bt_hcrp_converter_init.h"
#ifndef EXCLUDE_CSR_BT_HCRP_MODULE
#include "csr_bt_hcrp_serialize.h"
#include "csr_bt_hcrp_prim.h"

static CsrMsgConvMsgEntry csr_bt_hcrp_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ
    { CSR_BT_HCRP_ACTIVATE_REQ, CsrBtHcrpActivateReqSizeof, CsrBtHcrpActivateReqSer, CsrBtHcrpActivateReqDes, CsrBtHcrpActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_HCRP_DEACTIVATE_REQ
    { CSR_BT_HCRP_DEACTIVATE_REQ, CsrBtHcrpDeactivateReqSizeof, CsrBtHcrpDeactivateReqSer, CsrBtHcrpDeactivateReqDes, CsrBtHcrpDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE_1284_IND
    { CSR_BT_HCRP_GET_IEEE_1284_IND, CsrBtHcrpGetIeee1284IndSizeof, CsrBtHcrpGetIeee1284IndSer, CsrBtHcrpGetIeee1284IndDes, CsrBtHcrpGetIeee1284IndSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE_1284_IND */
#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE_1284_RES
    { CSR_BT_HCRP_GET_IEEE_1284_RES, CsrBtHcrpGetIeee1284ResSizeof, CsrBtHcrpGetIeee1284ResSer, CsrBtHcrpGetIeee1284ResDes, CsrBtHcrpGetIeee1284ResSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE_1284_RES */
#ifndef EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_IND
    { CSR_BT_HCRP_GET_LPT_STATUS_IND, CsrBtHcrpGetLptStatusIndSizeof, CsrBtHcrpGetLptStatusIndSer, CsrBtHcrpGetLptStatusIndDes, CsrBtHcrpGetLptStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_RES
    { CSR_BT_HCRP_GET_LPT_STATUS_RES, CsrBtHcrpGetLptStatusResSizeof, CsrBtHcrpGetLptStatusResSer, CsrBtHcrpGetLptStatusResDes, CsrBtHcrpGetLptStatusResSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_RES */
#ifndef EXCLUDE_CSR_BT_HCRP_HOUSE_CLEANING
    { CSR_BT_HCRP_HOUSE_CLEANING, CsrBtHcrpHouseCleaningSizeof, CsrBtHcrpHouseCleaningSer, CsrBtHcrpHouseCleaningDes, CsrBtHcrpHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_CONNECT_IND
    { CSR_BT_HCRP_L2CA_CONNECT_IND, CsrBtHcrpL2caConnectIndSizeof, CsrBtHcrpL2caConnectIndSer, CsrBtHcrpL2caConnectIndDes, CsrBtHcrpL2caConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND
    { CSR_BT_HCRP_L2CA_DATA_IND, CsrBtHcrpL2caDataIndSizeof, CsrBtHcrpL2caDataIndSer, CsrBtHcrpL2caDataIndDes, CsrBtHcrpL2caDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND */
#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DISCONNECT_IND
    { CSR_BT_HCRP_L2CA_DISCONNECT_IND, CsrBtHcrpL2caDisconnectIndSizeof, CsrBtHcrpL2caDisconnectIndSer, CsrBtHcrpL2caDisconnectIndDes, CsrBtHcrpL2caDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_CFM
    { CSR_BT_HCRP_REGISTER_DATA_PATH_CFM, CsrBtHcrpRegisterDataPathCfmSizeof, CsrBtHcrpRegisterDataPathCfmSer, CsrBtHcrpRegisterDataPathCfmDes, CsrBtHcrpRegisterDataPathCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_CFM */
#ifndef EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_REQ
    { CSR_BT_HCRP_REGISTER_DATA_PATH_REQ, CsrBtHcrpRegisterDataPathReqSizeof, CsrBtHcrpRegisterDataPathReqSer, CsrBtHcrpRegisterDataPathReqDes, CsrBtHcrpRegisterDataPathReqSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_REQ */
#ifndef EXCLUDE_CSR_BT_HCRP_SECURITY_IN_CFM
    { CSR_BT_HCRP_SECURITY_IN_CFM, CsrBtHcrpSecurityInCfmSizeof, CsrBtHcrpSecurityInCfmSer, CsrBtHcrpSecurityInCfmDes, CsrBtHcrpSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_HCRP_SECURITY_IN_REQ
    { CSR_BT_HCRP_SECURITY_IN_REQ, CsrBtHcrpSecurityInReqSizeof, CsrBtHcrpSecurityInReqSer, CsrBtHcrpSecurityInReqDes, CsrBtHcrpSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_HCRP_STATUS_IND
    { CSR_BT_HCRP_STATUS_IND, CsrBtHcrpStatusIndSizeof, CsrBtHcrpStatusIndSer, CsrBtHcrpStatusIndDes, CsrBtHcrpStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_HCRP_STATUS_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtHcrpConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_HCRP_PRIM, csr_bt_hcrp_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_hcrp_conv_info = {
    CSR_BT_HCRP_PRIM,
    "CSR_BT_HCRP_PRIM",
    csr_bt_hcrp_conv_lut
};

CsrLogPrimitiveInformation* CsrBtHcrpTechInfoGet(void)
{
    return &csr_bt_hcrp_conv_info;
}
#endif
#endif

