#ifndef CSR_BCCMD_SERIALIZE_H__
#define CSR_BCCMD_SERIALIZE_H__
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

void CsrBccmdPfree(void *ptr);

#ifndef EXCLUDE_CSR_BCCMD_REQ
CsrSize CsrBccmdReqSizeof(void *msg);
CsrUint8 *CsrBccmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBccmdReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BCCMD_REQ */

#ifndef EXCLUDE_CSR_BCCMD_QUERY_REQ
CsrSize CsrBccmdQueryReqSizeof(void *msg);
CsrUint8 *CsrBccmdQueryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdQueryReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBccmdQueryReqSerFree CsrBccmdPfree
#endif /* EXCLUDE_CSR_BCCMD_QUERY_REQ */

#ifndef EXCLUDE_CSR_BCCMD_FLUSH_REQ
CsrSize CsrBccmdFlushReqSizeof(void *msg);
CsrUint8 *CsrBccmdFlushReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdFlushReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBccmdFlushReqSerFree CsrBccmdPfree
#endif /* EXCLUDE_CSR_BCCMD_FLUSH_REQ */

#ifndef EXCLUDE_CSR_BCCMD_RESET_IND
CsrSize CsrBccmdResetIndSizeof(void *msg);
CsrUint8 *CsrBccmdResetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdResetIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBccmdResetIndSerFree CsrBccmdPfree
#endif /* EXCLUDE_CSR_BCCMD_RESET_IND */

#ifndef EXCLUDE_CSR_BCCMD_BOOTSTRAP_CFM
CsrSize CsrBccmdBootstrapCfmSizeof(void *msg);
CsrUint8 *CsrBccmdBootstrapCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdBootstrapCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBccmdBootstrapCfmSerFree CsrBccmdPfree
#endif /* EXCLUDE_CSR_BCCMD_BOOTSTRAP_CFM */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_IND
CsrSize CsrBccmdPanicIndSizeof(void *msg);
CsrUint8 *CsrBccmdPanicIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdPanicIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBccmdPanicIndSerFree CsrBccmdPfree
#endif /* EXCLUDE_CSR_BCCMD_PANIC_IND */

#ifndef EXCLUDE_CSR_BCCMD_RESET_RES
CsrSize CsrBccmdResetResSizeof(void *msg);
CsrUint8 *CsrBccmdResetResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdResetResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBccmdResetResSerFree CsrBccmdPfree
#endif /* EXCLUDE_CSR_BCCMD_RESET_RES */

#ifndef EXCLUDE_CSR_BCCMD_GET_BUILD_ID_REQ
CsrSize CsrBccmdGetBuildIdReqSizeof(void *msg);
CsrUint8 *CsrBccmdGetBuildIdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdGetBuildIdReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBccmdGetBuildIdReqSerFree CsrBccmdPfree
#endif /* EXCLUDE_CSR_BCCMD_GET_BUILD_ID_REQ */

#ifndef EXCLUDE_CSR_BCCMD_GET_BUILD_ID_CFM
CsrSize CsrBccmdGetBuildIdCfmSizeof(void *msg);
CsrUint8 *CsrBccmdGetBuildIdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdGetBuildIdCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBccmdGetBuildIdCfmSerFree CsrBccmdPfree
#endif /* EXCLUDE_CSR_BCCMD_GET_BUILD_ID_CFM */

#ifndef EXCLUDE_CSR_BCCMD_FLUSH_CFM
CsrSize CsrBccmdFlushCfmSizeof(void *msg);
CsrUint8 *CsrBccmdFlushCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdFlushCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBccmdFlushCfmSerFree CsrBccmdPfree
#endif /* EXCLUDE_CSR_BCCMD_FLUSH_CFM */

#ifndef EXCLUDE_CSR_BCCMD_CFM
CsrSize CsrBccmdCfmSizeof(void *msg);
CsrUint8 *CsrBccmdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBccmdCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BCCMD_CFM */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_ARGS_REQ
CsrSize CsrBccmdPanicArgsReqSizeof(void *msg);
CsrUint8 *CsrBccmdPanicArgsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdPanicArgsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBccmdPanicArgsReqSerFree CsrBccmdPfree
#endif /* EXCLUDE_CSR_BCCMD_PANIC_ARGS_REQ */

#ifndef EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ
CsrSize CsrBccmdBootstrapReqSizeof(void *msg);
CsrUint8 *CsrBccmdBootstrapReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdBootstrapReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBccmdBootstrapReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BCCMD_BOOTSTRAP_REQ */

#ifndef EXCLUDE_CSR_BCCMD_PANIC_ARGS_CFM
CsrSize CsrBccmdPanicArgsCfmSizeof(void *msg);
CsrUint8 *CsrBccmdPanicArgsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBccmdPanicArgsCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBccmdPanicArgsCfmSerFree CsrBccmdPfree
#endif /* EXCLUDE_CSR_BCCMD_PANIC_ARGS_CFM */

#ifdef __cplusplus
}
#endif

#endif
