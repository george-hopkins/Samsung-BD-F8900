#ifndef CSR_FP_SERIALIZE_H__
#define CSR_FP_SERIALIZE_H__
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

void CsrFpPfree(void *ptr);

#ifndef EXCLUDE_CSR_FP_CLEAR_REQ
CsrSize CsrFpClearReqSizeof(void *msg);
CsrUint8 *CsrFpClearReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFpClearReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrFpClearReqSerFree CsrFpPfree
#endif /* EXCLUDE_CSR_FP_CLEAR_REQ */

#ifndef EXCLUDE_CSR_FP_WRITE_CFM
CsrSize CsrFpWriteCfmSizeof(void *msg);
CsrUint8 *CsrFpWriteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFpWriteCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrFpWriteCfmSerFree CsrFpPfree
#endif /* EXCLUDE_CSR_FP_WRITE_CFM */

#ifndef EXCLUDE_CSR_FP_CLEAR_CFM
CsrSize CsrFpClearCfmSizeof(void *msg);
CsrUint8 *CsrFpClearCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFpClearCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrFpClearCfmSerFree CsrFpPfree
#endif /* EXCLUDE_CSR_FP_CLEAR_CFM */

#ifndef EXCLUDE_CSR_FP_READ_IND
CsrSize CsrFpReadIndSizeof(void *msg);
CsrUint8 *CsrFpReadIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFpReadIndDes(CsrUint8 *buffer, CsrSize length);
void CsrFpReadIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_FP_READ_IND */

#ifndef EXCLUDE_CSR_FP_DESTROY_REQ
CsrSize CsrFpDestroyReqSizeof(void *msg);
CsrUint8 *CsrFpDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFpDestroyReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrFpDestroyReqSerFree CsrFpPfree
#endif /* EXCLUDE_CSR_FP_DESTROY_REQ */

#ifndef EXCLUDE_CSR_FP_CREATE_REQ
CsrSize CsrFpCreateReqSizeof(void *msg);
CsrUint8 *CsrFpCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFpCreateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrFpCreateReqSerFree CsrFpPfree
#endif /* EXCLUDE_CSR_FP_CREATE_REQ */

#ifndef EXCLUDE_CSR_FP_WRITE_REQ
CsrSize CsrFpWriteReqSizeof(void *msg);
CsrUint8 *CsrFpWriteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFpWriteReqDes(CsrUint8 *buffer, CsrSize length);
void CsrFpWriteReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_FP_WRITE_REQ */

#ifndef EXCLUDE_CSR_FP_DESTROY_CFM
CsrSize CsrFpDestroyCfmSizeof(void *msg);
CsrUint8 *CsrFpDestroyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFpDestroyCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrFpDestroyCfmSerFree CsrFpPfree
#endif /* EXCLUDE_CSR_FP_DESTROY_CFM */

#ifndef EXCLUDE_CSR_FP_CREATE_CFM
CsrSize CsrFpCreateCfmSizeof(void *msg);
CsrUint8 *CsrFpCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrFpCreateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrFpCreateCfmSerFree CsrFpPfree
#endif /* EXCLUDE_CSR_FP_CREATE_CFM */

#ifdef __cplusplus
}
#endif

#endif
