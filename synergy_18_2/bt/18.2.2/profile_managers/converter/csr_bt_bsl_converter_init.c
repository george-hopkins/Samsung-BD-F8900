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
#include "csr_bt_bsl_converter_init.h"
#ifndef EXCLUDE_CSR_BT_BSL_MODULE
#include "csr_bt_bsl_serialize.h"
#include "csr_bt_bsl_prim.h"

static CsrMsgConvMsgEntry csr_bt_bsl_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_BSL_ACTIVATE_CFM
    { CSR_BT_BSL_ACTIVATE_CFM, CsrBtBslActivateCfmSizeof, CsrBtBslActivateCfmSer, CsrBtBslActivateCfmDes, CsrBtBslActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_BSL_ACTIVATE_REQ
    { CSR_BT_BSL_ACTIVATE_REQ, CsrBtBslActivateReqSizeof, CsrBtBslActivateReqSer, CsrBtBslActivateReqDes, CsrBtBslActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_BSL_CANCEL_CONNECT_REQ
    { CSR_BT_BSL_CANCEL_CONNECT_REQ, CsrBtBslCancelConnectReqSizeof, CsrBtBslCancelConnectReqSer, CsrBtBslCancelConnectReqDes, CsrBtBslCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_BSL_CONNECT_IND
    { CSR_BT_BSL_CONNECT_IND, CsrBtBslConnectIndSizeof, CsrBtBslConnectIndSer, CsrBtBslConnectIndDes, CsrBtBslConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_BSL_CONNECT_REQ
    { CSR_BT_BSL_CONNECT_REQ, CsrBtBslConnectReqSizeof, CsrBtBslConnectReqSer, CsrBtBslConnectReqDes, CsrBtBslConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_BSL_DATA_IND
    { CSR_BT_BSL_DATA_IND, CsrBtBslDataIndSizeof, CsrBtBslDataIndSer, CsrBtBslDataIndDes, CsrBtBslDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_DATA_IND */
#ifndef EXCLUDE_CSR_BT_BSL_DATA_REQ
    { CSR_BT_BSL_DATA_REQ, CsrBtBslDataReqSizeof, CsrBtBslDataReqSer, CsrBtBslDataReqDes, CsrBtBslDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_IND
    { CSR_BT_BSL_DISCONNECT_IND, CsrBtBslDisconnectIndSizeof, CsrBtBslDisconnectIndSer, CsrBtBslDisconnectIndDes, CsrBtBslDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_REQ
    { CSR_BT_BSL_DISCONNECT_REQ, CsrBtBslDisconnectReqSizeof, CsrBtBslDisconnectReqSer, CsrBtBslDisconnectReqDes, CsrBtBslDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_RES
    { CSR_BT_BSL_DISCONNECT_RES, CsrBtBslDisconnectResSizeof, CsrBtBslDisconnectResSer, CsrBtBslDisconnectResDes, CsrBtBslDisconnectResSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_RES */
#ifndef EXCLUDE_CSR_BT_BSL_HOUSE_CLEANING
    { CSR_BT_BSL_HOUSE_CLEANING, CsrBtBslHouseCleaningSizeof, CsrBtBslHouseCleaningSer, CsrBtBslHouseCleaningDes, CsrBtBslHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ
    { CSR_BT_BSL_MULTICAST_DATA_REQ, CsrBtBslMulticastDataReqSizeof, CsrBtBslMulticastDataReqSer, CsrBtBslMulticastDataReqDes, CsrBtBslMulticastDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_BSL_REGISTER_REQ
    { CSR_BT_BSL_REGISTER_REQ, CsrBtBslRegisterReqSizeof, CsrBtBslRegisterReqSer, CsrBtBslRegisterReqDes, CsrBtBslRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_IN_CFM
    { CSR_BT_BSL_SECURITY_IN_CFM, CsrBtBslSecurityInCfmSizeof, CsrBtBslSecurityInCfmSer, CsrBtBslSecurityInCfmDes, CsrBtBslSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_IN_REQ
    { CSR_BT_BSL_SECURITY_IN_REQ, CsrBtBslSecurityInReqSizeof, CsrBtBslSecurityInReqSer, CsrBtBslSecurityInReqDes, CsrBtBslSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_OUT_CFM
    { CSR_BT_BSL_SECURITY_OUT_CFM, CsrBtBslSecurityOutCfmSizeof, CsrBtBslSecurityOutCfmSer, CsrBtBslSecurityOutCfmDes, CsrBtBslSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_OUT_REQ
    { CSR_BT_BSL_SECURITY_OUT_REQ, CsrBtBslSecurityOutReqSizeof, CsrBtBslSecurityOutReqSer, CsrBtBslSecurityOutReqDes, CsrBtBslSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_CFM
    { CSR_BT_BSL_SERVICE_SEARCH_CFM, CsrBtBslServiceSearchCfmSizeof, CsrBtBslServiceSearchCfmSer, CsrBtBslServiceSearchCfmDes, CsrBtBslServiceSearchCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_CFM */
#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_REQ
    { CSR_BT_BSL_SERVICE_SEARCH_REQ, CsrBtBslServiceSearchReqSizeof, CsrBtBslServiceSearchReqSer, CsrBtBslServiceSearchReqDes, CsrBtBslServiceSearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND
    { CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND, CsrBtBslServiceSearchResultIndSizeof, CsrBtBslServiceSearchResultIndSer, CsrBtBslServiceSearchResultIndDes, CsrBtBslServiceSearchResultIndSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND */
#ifndef EXCLUDE_CSR_BT_BSL_STATUS_IND
    { CSR_BT_BSL_STATUS_IND, CsrBtBslStatusIndSizeof, CsrBtBslStatusIndSer, CsrBtBslStatusIndDes, CsrBtBslStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_BSL_STATUS_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtBslConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_BSL_PRIM, csr_bt_bsl_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_bsl_conv_info = {
    CSR_BT_BSL_PRIM,
    "CSR_BT_BSL_PRIM",
    csr_bt_bsl_conv_lut
};

CsrLogPrimitiveInformation* CsrBtBslTechInfoGet(void)
{
    return &csr_bt_bsl_conv_info;
}
#endif
#endif

