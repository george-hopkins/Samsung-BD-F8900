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
#include "csr_bt_lsl_converter_init.h"
#ifndef EXCLUDE_CSR_BT_LSL_MODULE
#include "csr_bt_lsl_serialize.h"
#include "csr_bt_lsl_prim.h"

static CsrMsgConvMsgEntry csr_bt_lsl_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_LSL_CONNECT_REQ
    { CSR_BT_LSL_CONNECT_REQ, CsrBtLslConnectReqSizeof, CsrBtLslConnectReqSer, CsrBtLslConnectReqDes, CsrBtLslConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_LSL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_LSL_DATA_CFM
    { CSR_BT_LSL_DATA_CFM, CsrBtLslDataCfmSizeof, CsrBtLslDataCfmSer, CsrBtLslDataCfmDes, CsrBtLslDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_LSL_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_LSL_DATA_IND
    { CSR_BT_LSL_DATA_IND, CsrBtLslDataIndSizeof, CsrBtLslDataIndSer, CsrBtLslDataIndDes, CsrBtLslDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_LSL_DATA_IND */
#ifndef EXCLUDE_CSR_BT_LSL_DATA_REQ
    { CSR_BT_LSL_DATA_REQ, CsrBtLslDataReqSizeof, CsrBtLslDataReqSer, CsrBtLslDataReqDes, CsrBtLslDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_LSL_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_LSL_DATA_RES
    { CSR_BT_LSL_DATA_RES, CsrBtLslDataResSizeof, CsrBtLslDataResSer, CsrBtLslDataResDes, CsrBtLslDataResSerFree },
#endif /* EXCLUDE_CSR_BT_LSL_DATA_RES */
#ifndef EXCLUDE_CSR_BT_LSL_DISCONNECT_REQ
    { CSR_BT_LSL_DISCONNECT_REQ, CsrBtLslDisconnectReqSizeof, CsrBtLslDisconnectReqSer, CsrBtLslDisconnectReqDes, CsrBtLslDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_LSL_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_LSL_STATUS_IND
    { CSR_BT_LSL_STATUS_IND, CsrBtLslStatusIndSizeof, CsrBtLslStatusIndSer, CsrBtLslStatusIndDes, CsrBtLslStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_LSL_STATUS_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtLslConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_LSL_PRIM, csr_bt_lsl_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_lsl_conv_info = {
    CSR_BT_LSL_PRIM,
    "CSR_BT_LSL_PRIM",
    csr_bt_lsl_conv_lut
};

CsrLogPrimitiveInformation* CsrBtLslTechInfoGet(void)
{
    return &csr_bt_lsl_conv_info;
}
#endif
#endif

