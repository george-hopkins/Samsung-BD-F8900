#ifndef CSR_VM_SERIALIZE_H__
#define CSR_VM_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrVmPfree(void *ptr);

#ifndef EXCLUDE_CSR_VM_REGISTER_REQ
CsrSize CsrVmRegisterReqSizeof(void *msg);
CsrUint8 *CsrVmRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrVmRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrVmRegisterReqSerFree CsrVmPfree
#endif /* EXCLUDE_CSR_VM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_VM_REGISTER_CFM
CsrSize CsrVmRegisterCfmSizeof(void *msg);
CsrUint8 *CsrVmRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrVmRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrVmRegisterCfmSerFree CsrVmPfree
#endif /* EXCLUDE_CSR_VM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_VM_DATA_REQ
CsrSize CsrVmDataReqSizeof(void *msg);
CsrUint8 *CsrVmDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrVmDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrVmDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_VM_DATA_REQ */

#ifndef EXCLUDE_CSR_VM_DATA_IND
CsrSize CsrVmDataIndSizeof(void *msg);
CsrUint8 *CsrVmDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrVmDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrVmDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_VM_DATA_IND */

#ifdef __cplusplus
}
#endif

#endif
