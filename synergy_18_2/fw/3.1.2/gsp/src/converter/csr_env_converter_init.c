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
#include "csr_env_converter_init.h"
#ifndef EXCLUDE_CSR_ENV_MODULE
#include "csr_env_serialize.h"
#include "csr_env_prim.h"

static CsrMsgConvMsgEntry csr_env_conv_lut[] = {
#ifndef EXCLUDE_CSR_CLEANUP_IND
    { CSR_CLEANUP_IND, CsrCleanupIndSizeof, CsrCleanupIndSer, CsrCleanupIndDes, CsrCleanupIndSerFree },
#endif /* EXCLUDE_CSR_CLEANUP_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrEnvConverterInit(void)
{
    CsrMsgConvInsert(CSR_ENV_PRIM, csr_env_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_env_conv_info = {
    CSR_ENV_PRIM,
    "CSR_ENV_PRIM",
    csr_env_conv_lut
};

CsrLogPrimitiveInformation* CsrEnvTechInfoGet(void)
{
    return &csr_env_conv_info;
}
#endif
#endif

