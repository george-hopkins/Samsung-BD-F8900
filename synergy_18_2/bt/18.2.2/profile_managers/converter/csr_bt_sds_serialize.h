#ifndef CSR_BT_SDS_SERIALIZE_H__
#define CSR_BT_SDS_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtSdsPfree(void *ptr);

#ifndef EXCLUDE_SDS_UNREGISTER_CFM
CsrSize SDS_UNREGISTER_CFM_TSizeof(void *msg);
CsrUint8 *SDS_UNREGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDS_UNREGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define SDS_UNREGISTER_CFM_TSerFree CsrBtSdsPfree
#endif /* EXCLUDE_SDS_UNREGISTER_CFM */

#ifndef EXCLUDE_SDS_REGISTER_CFM
CsrSize SDS_REGISTER_CFM_TSizeof(void *msg);
CsrUint8 *SDS_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDS_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define SDS_REGISTER_CFM_TSerFree CsrBtSdsPfree
#endif /* EXCLUDE_SDS_REGISTER_CFM */

#ifndef EXCLUDE_SDS_CONFIG_CFM
CsrSize SDS_CONFIG_CFM_TSizeof(void *msg);
CsrUint8 *SDS_CONFIG_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDS_CONFIG_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define SDS_CONFIG_CFM_TSerFree CsrBtSdsPfree
#endif /* EXCLUDE_SDS_CONFIG_CFM */

#ifndef EXCLUDE_SDS_CONFIG_REQ
CsrSize SDS_CONFIG_REQ_TSizeof(void *msg);
CsrUint8 *SDS_CONFIG_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDS_CONFIG_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define SDS_CONFIG_REQ_TSerFree CsrBtSdsPfree
#endif /* EXCLUDE_SDS_CONFIG_REQ */

#ifndef EXCLUDE_SDS_REGISTER_REQ
CsrSize SDS_REGISTER_REQ_TSizeof(void *msg);
CsrUint8 *SDS_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDS_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void SDS_REGISTER_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_SDS_REGISTER_REQ */

#ifndef EXCLUDE_SDS_UNREGISTER_REQ
CsrSize SDS_UNREGISTER_REQ_TSizeof(void *msg);
CsrUint8 *SDS_UNREGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDS_UNREGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define SDS_UNREGISTER_REQ_TSerFree CsrBtSdsPfree
#endif /* EXCLUDE_SDS_UNREGISTER_REQ */

#ifdef __cplusplus
}
#endif

#endif
