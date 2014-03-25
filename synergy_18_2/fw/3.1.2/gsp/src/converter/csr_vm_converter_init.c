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
#include "csr_vm_converter_init.h"
#ifndef EXCLUDE_CSR_VM_MODULE
#include "csr_vm_serialize.h"
#include "csr_vm_prim.h"

static CsrMsgConvMsgEntry csr_vm_conv_lut[] = {
#ifndef EXCLUDE_CSR_VM_DATA_IND
    { CSR_VM_DATA_IND, CsrVmDataIndSizeof, CsrVmDataIndSer, CsrVmDataIndDes, CsrVmDataIndSerFree },
#endif /* EXCLUDE_CSR_VM_DATA_IND */
#ifndef EXCLUDE_CSR_VM_DATA_REQ
    { CSR_VM_DATA_REQ, CsrVmDataReqSizeof, CsrVmDataReqSer, CsrVmDataReqDes, CsrVmDataReqSerFree },
#endif /* EXCLUDE_CSR_VM_DATA_REQ */
#ifndef EXCLUDE_CSR_VM_REGISTER_CFM
    { CSR_VM_REGISTER_CFM, CsrVmRegisterCfmSizeof, CsrVmRegisterCfmSer, CsrVmRegisterCfmDes, CsrVmRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_VM_REGISTER_CFM */
#ifndef EXCLUDE_CSR_VM_REGISTER_REQ
    { CSR_VM_REGISTER_REQ, CsrVmRegisterReqSizeof, CsrVmRegisterReqSer, CsrVmRegisterReqDes, CsrVmRegisterReqSerFree },
#endif /* EXCLUDE_CSR_VM_REGISTER_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrVmConverterInit(void)
{
    CsrMsgConvInsert(CSR_VM_PRIM, csr_vm_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_vm_conv_info = {
    CSR_VM_PRIM,
    "CSR_VM_PRIM",
    csr_vm_conv_lut
};

CsrLogPrimitiveInformation* CsrVmTechInfoGet(void)
{
    return &csr_vm_conv_info;
}
#endif
#endif

