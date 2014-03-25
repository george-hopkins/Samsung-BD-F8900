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
#include "csr_fp_converter_init.h"
#ifndef EXCLUDE_CSR_FP_MODULE
#include "csr_fp_serialize.h"
#include "csr_fp_prim.h"

static CsrMsgConvMsgEntry csr_fp_conv_lut[] = {
#ifndef EXCLUDE_CSR_FP_CLEAR_CFM
    { CSR_FP_CLEAR_CFM, CsrFpClearCfmSizeof, CsrFpClearCfmSer, CsrFpClearCfmDes, CsrFpClearCfmSerFree },
#endif /* EXCLUDE_CSR_FP_CLEAR_CFM */
#ifndef EXCLUDE_CSR_FP_CLEAR_REQ
    { CSR_FP_CLEAR_REQ, CsrFpClearReqSizeof, CsrFpClearReqSer, CsrFpClearReqDes, CsrFpClearReqSerFree },
#endif /* EXCLUDE_CSR_FP_CLEAR_REQ */
#ifndef EXCLUDE_CSR_FP_CREATE_CFM
    { CSR_FP_CREATE_CFM, CsrFpCreateCfmSizeof, CsrFpCreateCfmSer, CsrFpCreateCfmDes, CsrFpCreateCfmSerFree },
#endif /* EXCLUDE_CSR_FP_CREATE_CFM */
#ifndef EXCLUDE_CSR_FP_CREATE_REQ
    { CSR_FP_CREATE_REQ, CsrFpCreateReqSizeof, CsrFpCreateReqSer, CsrFpCreateReqDes, CsrFpCreateReqSerFree },
#endif /* EXCLUDE_CSR_FP_CREATE_REQ */
#ifndef EXCLUDE_CSR_FP_DESTROY_CFM
    { CSR_FP_DESTROY_CFM, CsrFpDestroyCfmSizeof, CsrFpDestroyCfmSer, CsrFpDestroyCfmDes, CsrFpDestroyCfmSerFree },
#endif /* EXCLUDE_CSR_FP_DESTROY_CFM */
#ifndef EXCLUDE_CSR_FP_DESTROY_REQ
    { CSR_FP_DESTROY_REQ, CsrFpDestroyReqSizeof, CsrFpDestroyReqSer, CsrFpDestroyReqDes, CsrFpDestroyReqSerFree },
#endif /* EXCLUDE_CSR_FP_DESTROY_REQ */
#ifndef EXCLUDE_CSR_FP_READ_IND
    { CSR_FP_READ_IND, CsrFpReadIndSizeof, CsrFpReadIndSer, CsrFpReadIndDes, CsrFpReadIndSerFree },
#endif /* EXCLUDE_CSR_FP_READ_IND */
#ifndef EXCLUDE_CSR_FP_WRITE_CFM
    { CSR_FP_WRITE_CFM, CsrFpWriteCfmSizeof, CsrFpWriteCfmSer, CsrFpWriteCfmDes, CsrFpWriteCfmSerFree },
#endif /* EXCLUDE_CSR_FP_WRITE_CFM */
#ifndef EXCLUDE_CSR_FP_WRITE_REQ
    { CSR_FP_WRITE_REQ, CsrFpWriteReqSizeof, CsrFpWriteReqSer, CsrFpWriteReqDes, CsrFpWriteReqSerFree },
#endif /* EXCLUDE_CSR_FP_WRITE_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrFpConverterInit(void)
{
    CsrMsgConvInsert(CSR_FP_PRIM, csr_fp_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_fp_conv_info = {
    CSR_FP_PRIM,
    "CSR_FP_PRIM",
    csr_fp_conv_lut
};

CsrLogPrimitiveInformation* CsrFpTechInfoGet(void)
{
    return &csr_fp_conv_info;
}
#endif
#endif

