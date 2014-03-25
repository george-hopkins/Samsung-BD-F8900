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
#include "csr_bt_sds_converter_init.h"
#ifndef EXCLUDE_CSR_BT_SDS_MODULE
#include "csr_bt_sds_serialize.h"
#include "sds_prim.h"

static CsrMsgConvMsgEntry csr_bt_sds_conv_lut[] = {
#ifndef EXCLUDE_SDS_CONFIG_CFM
    { SDS_CONFIG_CFM, SDS_CONFIG_CFM_TSizeof, SDS_CONFIG_CFM_TSer, SDS_CONFIG_CFM_TDes, SDS_CONFIG_CFM_TSerFree },
#endif /* EXCLUDE_SDS_CONFIG_CFM */
#ifndef EXCLUDE_SDS_CONFIG_REQ
    { SDS_CONFIG_REQ, SDS_CONFIG_REQ_TSizeof, SDS_CONFIG_REQ_TSer, SDS_CONFIG_REQ_TDes, SDS_CONFIG_REQ_TSerFree },
#endif /* EXCLUDE_SDS_CONFIG_REQ */
#ifndef EXCLUDE_SDS_REGISTER_CFM
    { SDS_REGISTER_CFM, SDS_REGISTER_CFM_TSizeof, SDS_REGISTER_CFM_TSer, SDS_REGISTER_CFM_TDes, SDS_REGISTER_CFM_TSerFree },
#endif /* EXCLUDE_SDS_REGISTER_CFM */
#ifndef EXCLUDE_SDS_REGISTER_REQ
    { SDS_REGISTER_REQ, SDS_REGISTER_REQ_TSizeof, SDS_REGISTER_REQ_TSer, SDS_REGISTER_REQ_TDes, SDS_REGISTER_REQ_TSerFree },
#endif /* EXCLUDE_SDS_REGISTER_REQ */
#ifndef EXCLUDE_SDS_UNREGISTER_CFM
    { SDS_UNREGISTER_CFM, SDS_UNREGISTER_CFM_TSizeof, SDS_UNREGISTER_CFM_TSer, SDS_UNREGISTER_CFM_TDes, SDS_UNREGISTER_CFM_TSerFree },
#endif /* EXCLUDE_SDS_UNREGISTER_CFM */
#ifndef EXCLUDE_SDS_UNREGISTER_REQ
    { SDS_UNREGISTER_REQ, SDS_UNREGISTER_REQ_TSizeof, SDS_UNREGISTER_REQ_TSer, SDS_UNREGISTER_REQ_TDes, SDS_UNREGISTER_REQ_TSerFree },
#endif /* EXCLUDE_SDS_UNREGISTER_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtSdsConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_SDS_PRIM, csr_bt_sds_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_sds_conv_info = {
    CSR_BT_SDS_PRIM,
    "CSR_BT_SDS_PRIM",
    csr_bt_sds_conv_lut
};

CsrLogPrimitiveInformation* CsrBtSdsTechInfoGet(void)
{
    return &csr_bt_sds_conv_info;
}
#endif
#endif

