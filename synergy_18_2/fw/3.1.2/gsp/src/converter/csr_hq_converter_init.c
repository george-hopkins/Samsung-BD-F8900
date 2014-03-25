/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_hand_coded.h"
#include "csr_hq_converter_init.h"
#ifndef EXCLUDE_CSR_HQ_MODULE
#include "csr_hq_serialize.h"
#include "csr_hq_prim.h"

static CsrMsgConvMsgEntry csr_hq_conv_lut[] = {
#ifndef EXCLUDE_CSR_HQ_DEREGISTER_CFM
    { CSR_HQ_DEREGISTER_CFM, CsrHqDeregisterCfmSizeof, CsrHqDeregisterCfmSer, CsrHqDeregisterCfmDes, CsrHqDeregisterCfmSerFree },
#endif /* EXCLUDE_CSR_HQ_DEREGISTER_CFM */
#ifndef EXCLUDE_CSR_HQ_DEREGISTER_REQ
    { CSR_HQ_DEREGISTER_REQ, CsrHqDeregisterReqSizeof, CsrHqDeregisterReqSer, CsrHqDeregisterReqDes, CsrHqDeregisterReqSerFree },
#endif /* EXCLUDE_CSR_HQ_DEREGISTER_REQ */
#ifndef EXCLUDE_CSR_HQ_MSG_IND
    { CSR_HQ_MSG_IND, CsrHqMsgIndSizeof, CsrHqMsgIndSer, CsrHqMsgIndDes, CsrHqMsgIndSerFree },
#endif /* EXCLUDE_CSR_HQ_MSG_IND */
#ifndef EXCLUDE_CSR_HQ_MSG_RES
    { CSR_HQ_MSG_RES, CsrHqMsgResSizeof, CsrHqMsgResSer, CsrHqMsgResDes, CsrHqMsgResSerFree },
#endif /* EXCLUDE_CSR_HQ_MSG_RES */
#ifndef EXCLUDE_CSR_HQ_REGISTER_CFM
    { CSR_HQ_REGISTER_CFM, CsrHqRegisterCfmSizeof, CsrHqRegisterCfmSer, CsrHqRegisterCfmDes, CsrHqRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_HQ_REGISTER_CFM */
#ifndef EXCLUDE_CSR_HQ_REGISTER_REQ
    { CSR_HQ_REGISTER_REQ, CsrHqRegisterReqSizeof, CsrHqRegisterReqSer, CsrHqRegisterReqDes, CsrHqRegisterReqSerFree },
#endif /* EXCLUDE_CSR_HQ_REGISTER_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrHqConverterInit(void)
{
    CsrMsgConvInsert(CSR_HQ_PRIM, csr_hq_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_hq_conv_info = {
    CSR_HQ_PRIM,
    "CSR_HQ_PRIM",
    csr_hq_conv_lut
};

CsrLogPrimitiveInformation* CsrHqTechInfoGet(void)
{
    return &csr_hq_conv_info;
}
#endif
#endif

