#ifndef CSR_APP_SERIALIZE_H__
#define CSR_APP_SERIALIZE_H__
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

void CsrAppPfree(void *ptr);

#ifndef EXCLUDE_CSR_APP_BACKLOG_REQ
CsrSize CsrAppBacklogReqSizeof(void *msg);
CsrUint8 *CsrAppBacklogReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAppBacklogReqDes(CsrUint8 *buffer, CsrSize length);
void CsrAppBacklogReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_APP_BACKLOG_REQ */

#ifndef EXCLUDE_CSR_APP_REGISTER_CFM
CsrSize CsrAppRegisterCfmSizeof(void *msg);
CsrUint8 *CsrAppRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAppRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrAppRegisterCfmSerFree CsrAppPfree
#endif /* EXCLUDE_CSR_APP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_APP_REGISTER_REQ
CsrSize CsrAppRegisterReqSizeof(void *msg);
CsrUint8 *CsrAppRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAppRegisterReqDes(CsrUint8 *buffer, CsrSize length);
void CsrAppRegisterReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_APP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_APP_TAKE_CONTROL_IND
CsrSize CsrAppTakeControlIndSizeof(void *msg);
CsrUint8 *CsrAppTakeControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAppTakeControlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrAppTakeControlIndSerFree CsrAppPfree
#endif /* EXCLUDE_CSR_APP_TAKE_CONTROL_IND */

#ifdef __cplusplus
}
#endif

#endif
