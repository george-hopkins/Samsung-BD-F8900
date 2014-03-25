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
#include "csr_app_converter_init.h"
#ifndef EXCLUDE_CSR_APP_MODULE
#include "csr_app_serialize.h"
#include "csr_app_prim.h"

static CsrMsgConvMsgEntry csr_app_conv_lut[] = {
#ifndef EXCLUDE_CSR_APP_BACKLOG_REQ
    { CSR_APP_BACKLOG_REQ, CsrAppBacklogReqSizeof, CsrAppBacklogReqSer, CsrAppBacklogReqDes, CsrAppBacklogReqSerFree },
#endif /* EXCLUDE_CSR_APP_BACKLOG_REQ */
#ifndef EXCLUDE_CSR_APP_REGISTER_CFM
    { CSR_APP_REGISTER_CFM, CsrAppRegisterCfmSizeof, CsrAppRegisterCfmSer, CsrAppRegisterCfmDes, CsrAppRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_APP_REGISTER_CFM */
#ifndef EXCLUDE_CSR_APP_REGISTER_REQ
    { CSR_APP_REGISTER_REQ, CsrAppRegisterReqSizeof, CsrAppRegisterReqSer, CsrAppRegisterReqDes, CsrAppRegisterReqSerFree },
#endif /* EXCLUDE_CSR_APP_REGISTER_REQ */
#ifndef EXCLUDE_CSR_APP_TAKE_CONTROL_IND
    { CSR_APP_TAKE_CONTROL_IND, CsrAppTakeControlIndSizeof, CsrAppTakeControlIndSer, CsrAppTakeControlIndDes, CsrAppTakeControlIndSerFree },
#endif /* EXCLUDE_CSR_APP_TAKE_CONTROL_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrAppConverterInit(void)
{
    CsrMsgConvInsert(CSR_APP_PRIM, csr_app_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_app_conv_info = {
    CSR_APP_PRIM,
    "CSR_APP_PRIM",
    csr_app_conv_lut
};

CsrLogPrimitiveInformation* CsrAppTechInfoGet(void)
{
    return &csr_app_conv_info;
}
#endif
#endif

