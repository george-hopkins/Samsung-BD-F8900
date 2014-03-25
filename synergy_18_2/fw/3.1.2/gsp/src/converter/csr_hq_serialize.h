#ifndef CSR_HQ_SERIALIZE_H__
#define CSR_HQ_SERIALIZE_H__
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

void CsrHqPfree(void *ptr);

#ifndef EXCLUDE_CSR_HQ_DEREGISTER_REQ
CsrSize CsrHqDeregisterReqSizeof(void *msg);
CsrUint8 *CsrHqDeregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHqDeregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrHqDeregisterReqSerFree CsrHqPfree
#endif /* EXCLUDE_CSR_HQ_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_HQ_MSG_RES
CsrSize CsrHqMsgResSizeof(void *msg);
CsrUint8 *CsrHqMsgResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHqMsgResDes(CsrUint8 *buffer, CsrSize length);
void CsrHqMsgResSerFree(void *msg);
#endif /* EXCLUDE_CSR_HQ_MSG_RES */

#ifndef EXCLUDE_CSR_HQ_DEREGISTER_CFM
CsrSize CsrHqDeregisterCfmSizeof(void *msg);
CsrUint8 *CsrHqDeregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHqDeregisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrHqDeregisterCfmSerFree CsrHqPfree
#endif /* EXCLUDE_CSR_HQ_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_HQ_REGISTER_REQ
CsrSize CsrHqRegisterReqSizeof(void *msg);
CsrUint8 *CsrHqRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHqRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrHqRegisterReqSerFree CsrHqPfree
#endif /* EXCLUDE_CSR_HQ_REGISTER_REQ */

#ifndef EXCLUDE_CSR_HQ_REGISTER_CFM
CsrSize CsrHqRegisterCfmSizeof(void *msg);
CsrUint8 *CsrHqRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHqRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrHqRegisterCfmSerFree CsrHqPfree
#endif /* EXCLUDE_CSR_HQ_REGISTER_CFM */

#ifndef EXCLUDE_CSR_HQ_MSG_IND
CsrSize CsrHqMsgIndSizeof(void *msg);
CsrUint8 *CsrHqMsgIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHqMsgIndDes(CsrUint8 *buffer, CsrSize length);
void CsrHqMsgIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_HQ_MSG_IND */

#ifdef __cplusplus
}
#endif

#endif
