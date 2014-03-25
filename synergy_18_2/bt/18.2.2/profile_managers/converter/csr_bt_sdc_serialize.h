#ifndef CSR_BT_SDC_SERIALIZE_H__
#define CSR_BT_SDC_SERIALIZE_H__
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

void CsrBtSdcPfree(void *ptr);

#ifndef EXCLUDE_SDC_TERMINATE_PRIMITIVE_REQ
CsrSize SDC_TERMINATE_PRIMITIVE_REQ_TSizeof(void *msg);
CsrUint8 *SDC_TERMINATE_PRIMITIVE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_TERMINATE_PRIMITIVE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define SDC_TERMINATE_PRIMITIVE_REQ_TSerFree CsrBtSdcPfree
#endif /* EXCLUDE_SDC_TERMINATE_PRIMITIVE_REQ */

#ifndef EXCLUDE_SDC_OPEN_SEARCH_REQ
CsrSize SDC_OPEN_SEARCH_REQ_TSizeof(void *msg);
CsrUint8 *SDC_OPEN_SEARCH_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_OPEN_SEARCH_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define SDC_OPEN_SEARCH_REQ_TSerFree CsrBtSdcPfree
#endif /* EXCLUDE_SDC_OPEN_SEARCH_REQ */

#ifndef EXCLUDE_SDC_CLOSE_SEARCH_IND
CsrSize SDC_CLOSE_SEARCH_IND_TSizeof(void *msg);
CsrUint8 *SDC_CLOSE_SEARCH_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_CLOSE_SEARCH_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define SDC_CLOSE_SEARCH_IND_TSerFree CsrBtSdcPfree
#endif /* EXCLUDE_SDC_CLOSE_SEARCH_IND */

#ifndef EXCLUDE_SDC_SERVICE_ATTRIBUTE_REQ
CsrSize SDC_SERVICE_ATTRIBUTE_REQ_TSizeof(void *msg);
CsrUint8 *SDC_SERVICE_ATTRIBUTE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_SERVICE_ATTRIBUTE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void SDC_SERVICE_ATTRIBUTE_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_SDC_SERVICE_ATTRIBUTE_REQ */

#ifndef EXCLUDE_SDC_OPEN_SEARCH_CFM
CsrSize SDC_OPEN_SEARCH_CFM_TSizeof(void *msg);
CsrUint8 *SDC_OPEN_SEARCH_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_OPEN_SEARCH_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define SDC_OPEN_SEARCH_CFM_TSerFree CsrBtSdcPfree
#endif /* EXCLUDE_SDC_OPEN_SEARCH_CFM */

#ifndef EXCLUDE_SDC_CLOSE_SEARCH_REQ
CsrSize SDC_CLOSE_SEARCH_REQ_TSizeof(void *msg);
CsrUint8 *SDC_CLOSE_SEARCH_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_CLOSE_SEARCH_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define SDC_CLOSE_SEARCH_REQ_TSerFree CsrBtSdcPfree
#endif /* EXCLUDE_SDC_CLOSE_SEARCH_REQ */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_CFM
CsrSize SDC_SERVICE_SEARCH_CFM_TSizeof(void *msg);
CsrUint8 *SDC_SERVICE_SEARCH_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_SERVICE_SEARCH_CFM_TDes(CsrUint8 *buffer, CsrSize length);
void SDC_SERVICE_SEARCH_CFM_TSerFree(void *msg);
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_SDC_CONFIG_REQ
CsrSize SDC_CONFIG_REQ_TSizeof(void *msg);
CsrUint8 *SDC_CONFIG_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_CONFIG_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define SDC_CONFIG_REQ_TSerFree CsrBtSdcPfree
#endif /* EXCLUDE_SDC_CONFIG_REQ */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_CFM
CsrSize SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_TSizeof(void *msg);
CsrUint8 *SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
void SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_TSerFree(void *msg);
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_CFM */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_REQ
CsrSize SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_TSizeof(void *msg);
CsrUint8 *SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_REQ */

#ifndef EXCLUDE_SDC_SERVICE_ATTRIBUTE_CFM
CsrSize SDC_SERVICE_ATTRIBUTE_CFM_TSizeof(void *msg);
CsrUint8 *SDC_SERVICE_ATTRIBUTE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_SERVICE_ATTRIBUTE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
void SDC_SERVICE_ATTRIBUTE_CFM_TSerFree(void *msg);
#endif /* EXCLUDE_SDC_SERVICE_ATTRIBUTE_CFM */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_REQ
CsrSize SDC_SERVICE_SEARCH_REQ_TSizeof(void *msg);
CsrUint8 *SDC_SERVICE_SEARCH_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_SERVICE_SEARCH_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void SDC_SERVICE_SEARCH_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_SDC_CONFIG_CFM
CsrSize SDC_CONFIG_CFM_TSizeof(void *msg);
CsrUint8 *SDC_CONFIG_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *SDC_CONFIG_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define SDC_CONFIG_CFM_TSerFree CsrBtSdcPfree
#endif /* EXCLUDE_SDC_CONFIG_CFM */

#ifdef __cplusplus
}
#endif

#endif
