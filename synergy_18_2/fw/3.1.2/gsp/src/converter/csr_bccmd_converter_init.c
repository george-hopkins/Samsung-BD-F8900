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
#include "csr_bccmd_converter_init.h"
#ifndef EXCLUDE_CSR_BCCMD_MODULE
#include "csr_bccmd_serialize.h"
#include "csr_bccmd_private_prim.h"

static CsrMsgConvMsgEntry csr_bccmd_conv_lut[] = {
#ifndef EXCLUDE_CSR_BCCMD_BOOTSTRAP_CFM
    { CSR_BCCMD_BOOTSTRAP_CFM, CsrBccmdBootstrapCfmSizeof, CsrBccmdBootstrapCfmSer, CsrBccmdBootstrapCfmDes, CsrBccmdBootstrapCfmSerFree },
#endif /* EXCLUDE_CSR_BCCMD_BOOTSTRAP_CFM */
#ifndef EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ
    { CSR_BCCMD_BOOTSTRAP_REQ, CsrBccmdBootstrapReqSizeof, CsrBccmdBootstrapReqSer, CsrBccmdBootstrapReqDes, CsrBccmdBootstrapReqSerFree },
#endif /* EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ */
#ifndef EXCLUDE_CSR_BCCMD_CFM
    { CSR_BCCMD_CFM, CsrBccmdCfmSizeof, CsrBccmdCfmSer, CsrBccmdCfmDes, CsrBccmdCfmSerFree },
#endif /* EXCLUDE_CSR_BCCMD_CFM */
#ifndef EXCLUDE_CSR_BCCMD_FLUSH_CFM
    { CSR_BCCMD_FLUSH_CFM, CsrBccmdFlushCfmSizeof, CsrBccmdFlushCfmSer, CsrBccmdFlushCfmDes, CsrBccmdFlushCfmSerFree },
#endif /* EXCLUDE_CSR_BCCMD_FLUSH_CFM */
#ifndef EXCLUDE_CSR_BCCMD_FLUSH_REQ
    { CSR_BCCMD_FLUSH_REQ, CsrBccmdFlushReqSizeof, CsrBccmdFlushReqSer, CsrBccmdFlushReqDes, CsrBccmdFlushReqSerFree },
#endif /* EXCLUDE_CSR_BCCMD_FLUSH_REQ */
#ifndef EXCLUDE_CSR_BCCMD_GET_BUILD_ID_CFM
    { CSR_BCCMD_GET_BUILD_ID_CFM, CsrBccmdGetBuildIdCfmSizeof, CsrBccmdGetBuildIdCfmSer, CsrBccmdGetBuildIdCfmDes, CsrBccmdGetBuildIdCfmSerFree },
#endif /* EXCLUDE_CSR_BCCMD_GET_BUILD_ID_CFM */
#ifndef EXCLUDE_CSR_BCCMD_GET_BUILD_ID_REQ
    { CSR_BCCMD_GET_BUILD_ID_REQ, CsrBccmdGetBuildIdReqSizeof, CsrBccmdGetBuildIdReqSer, CsrBccmdGetBuildIdReqDes, CsrBccmdGetBuildIdReqSerFree },
#endif /* EXCLUDE_CSR_BCCMD_GET_BUILD_ID_REQ */
#ifndef EXCLUDE_CSR_BCCMD_PANIC_ARGS_CFM
    { CSR_BCCMD_PANIC_ARGS_CFM, CsrBccmdPanicArgsCfmSizeof, CsrBccmdPanicArgsCfmSer, CsrBccmdPanicArgsCfmDes, CsrBccmdPanicArgsCfmSerFree },
#endif /* EXCLUDE_CSR_BCCMD_PANIC_ARGS_CFM */
#ifndef EXCLUDE_CSR_BCCMD_PANIC_ARGS_REQ
    { CSR_BCCMD_PANIC_ARGS_REQ, CsrBccmdPanicArgsReqSizeof, CsrBccmdPanicArgsReqSer, CsrBccmdPanicArgsReqDes, CsrBccmdPanicArgsReqSerFree },
#endif /* EXCLUDE_CSR_BCCMD_PANIC_ARGS_REQ */
#ifndef EXCLUDE_CSR_BCCMD_PANIC_IND
    { CSR_BCCMD_PANIC_IND, CsrBccmdPanicIndSizeof, CsrBccmdPanicIndSer, CsrBccmdPanicIndDes, CsrBccmdPanicIndSerFree },
#endif /* EXCLUDE_CSR_BCCMD_PANIC_IND */
#ifndef EXCLUDE_CSR_BCCMD_QUERY_REQ
    { CSR_BCCMD_QUERY_REQ, CsrBccmdQueryReqSizeof, CsrBccmdQueryReqSer, CsrBccmdQueryReqDes, CsrBccmdQueryReqSerFree },
#endif /* EXCLUDE_CSR_BCCMD_QUERY_REQ */
#ifndef EXCLUDE_CSR_BCCMD_REQ
    { CSR_BCCMD_REQ, CsrBccmdReqSizeof, CsrBccmdReqSer, CsrBccmdReqDes, CsrBccmdReqSerFree },
#endif /* EXCLUDE_CSR_BCCMD_REQ */
#ifndef EXCLUDE_CSR_BCCMD_RESET_IND
    { CSR_BCCMD_RESET_IND, CsrBccmdResetIndSizeof, CsrBccmdResetIndSer, CsrBccmdResetIndDes, CsrBccmdResetIndSerFree },
#endif /* EXCLUDE_CSR_BCCMD_RESET_IND */
#ifndef EXCLUDE_CSR_BCCMD_RESET_RES
    { CSR_BCCMD_RESET_RES, CsrBccmdResetResSizeof, CsrBccmdResetResSer, CsrBccmdResetResDes, CsrBccmdResetResSerFree },
#endif /* EXCLUDE_CSR_BCCMD_RESET_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBccmdConverterInit(void)
{
    CsrMsgConvInsert(CSR_BCCMD_PRIM, csr_bccmd_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bccmd_conv_info = {
    CSR_BCCMD_PRIM,
    "CSR_BCCMD_PRIM",
    csr_bccmd_conv_lut
};

CsrLogPrimitiveInformation* CsrBccmdTechInfoGet(void)
{
    return &csr_bccmd_conv_info;
}
#endif
#endif

