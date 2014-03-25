#ifndef CSR_BT_BSL_SERIALIZE_H__
#define CSR_BT_BSL_SERIALIZE_H__
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

void CsrBtBslPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_BSL_STATUS_IND
CsrSize CsrBtBslStatusIndSizeof(void *msg);
CsrUint8 *CsrBtBslStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslStatusIndSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_CFM
CsrSize CsrBtBslServiceSearchCfmSizeof(void *msg);
CsrUint8 *CsrBtBslServiceSearchCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslServiceSearchCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslServiceSearchCfmSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_REGISTER_REQ
CsrSize CsrBtBslRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtBslRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslRegisterReqSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_CONNECT_REQ
CsrSize CsrBtBslConnectReqSizeof(void *msg);
CsrUint8 *CsrBtBslConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslConnectReqSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_IN_REQ
CsrSize CsrBtBslSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtBslSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslSecurityInReqSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_OUT_REQ
CsrSize CsrBtBslSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtBslSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslSecurityOutReqSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_IND
CsrSize CsrBtBslDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtBslDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslDisconnectIndSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_REQ
CsrSize CsrBtBslDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtBslDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslDisconnectReqSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_CONNECT_IND
CsrSize CsrBtBslConnectIndSizeof(void *msg);
CsrUint8 *CsrBtBslConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslConnectIndSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_ACTIVATE_REQ
CsrSize CsrBtBslActivateReqSizeof(void *msg);
CsrUint8 *CsrBtBslActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslActivateReqSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_ACTIVATE_CFM
CsrSize CsrBtBslActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtBslActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslActivateCfmSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_DATA_REQ
CsrSize CsrBtBslDataReqSizeof(void *msg);
CsrUint8 *CsrBtBslDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBslDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BSL_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND
CsrSize CsrBtBslServiceSearchResultIndSizeof(void *msg);
CsrUint8 *CsrBtBslServiceSearchResultIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslServiceSearchResultIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBslServiceSearchResultIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_IN_CFM
CsrSize CsrBtBslSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtBslSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslSecurityInCfmSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_OUT_CFM
CsrSize CsrBtBslSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtBslSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslSecurityOutCfmSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_REQ
CsrSize CsrBtBslServiceSearchReqSizeof(void *msg);
CsrUint8 *CsrBtBslServiceSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslServiceSearchReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslServiceSearchReqSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_HOUSE_CLEANING
CsrSize CsrBtBslHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtBslHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslHouseCleaningSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ
CsrSize CsrBtBslMulticastDataReqSizeof(void *msg);
CsrUint8 *CsrBtBslMulticastDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslMulticastDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBslMulticastDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_RES
CsrSize CsrBtBslDisconnectResSizeof(void *msg);
CsrUint8 *CsrBtBslDisconnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslDisconnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslDisconnectResSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BSL_CANCEL_CONNECT_REQ
CsrSize CsrBtBslCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtBslCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBslCancelConnectReqSerFree CsrBtBslPfree
#endif /* EXCLUDE_CSR_BT_BSL_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_DATA_IND
CsrSize CsrBtBslDataIndSizeof(void *msg);
CsrUint8 *CsrBtBslDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBslDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBslDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BSL_DATA_IND */

#ifdef __cplusplus
}
#endif

#endif
