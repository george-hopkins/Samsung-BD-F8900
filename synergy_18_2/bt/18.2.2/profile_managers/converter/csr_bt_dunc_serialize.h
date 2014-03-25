#ifndef CSR_BT_DUNC_SERIALIZE_H__
#define CSR_BT_DUNC_SERIALIZE_H__
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

void CsrBtDuncPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_IND
CsrSize CsrBtDuncPortnegIndSizeof(void *msg);
CsrUint8 *CsrBtDuncPortnegIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncPortnegIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncPortnegIndSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ
CsrSize CsrBtDuncChangeDataPathStatusReqSizeof(void *msg);
CsrUint8 *CsrBtDuncChangeDataPathStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncChangeDataPathStatusReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncChangeDataPathStatusReqSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_DISCONNECT_IND
CsrSize CsrBtDuncDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtDuncDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncDisconnectIndSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_CFM
CsrSize CsrBtDuncDataCfmSizeof(void *msg);
CsrUint8 *CsrBtDuncDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncDataCfmSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_DISCONNECT_REQ
CsrSize CsrBtDuncDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtDuncDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncDisconnectReqSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_CFM
CsrSize CsrBtDuncSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtDuncSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncSecurityOutCfmSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_PATH_STATUS_IND
CsrSize CsrBtDuncDataPathStatusIndSizeof(void *msg);
CsrUint8 *CsrBtDuncDataPathStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncDataPathStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncDataPathStatusIndSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_REQ
CsrSize CsrBtDuncDataReqSizeof(void *msg);
CsrUint8 *CsrBtDuncDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtDuncDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_IND
CsrSize CsrBtDuncDataIndSizeof(void *msg);
CsrUint8 *CsrBtDuncDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtDuncDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM
CsrSize CsrBtDuncRegisterDataPathHandleCfmSizeof(void *msg);
CsrUint8 *CsrBtDuncRegisterDataPathHandleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncRegisterDataPathHandleCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncRegisterDataPathHandleCfmSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_CFM
CsrSize CsrBtDuncPortnegCfmSizeof(void *msg);
CsrUint8 *CsrBtDuncPortnegCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncPortnegCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncPortnegCfmSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_HOUSE_CLEANING
CsrSize CsrBtDuncHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtDuncHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncHouseCleaningSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_DUNC_DATA_RES
CsrSize CsrBtDuncDataResSizeof(void *msg);
CsrUint8 *CsrBtDuncDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncDataResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncDataResSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_DATA_RES */

#ifndef EXCLUDE_CSR_BT_DUNC_CONTROL_IND
CsrSize CsrBtDuncControlIndSizeof(void *msg);
CsrUint8 *CsrBtDuncControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncControlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncControlIndSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_CANCEL_CONNECT_REQ
CsrSize CsrBtDuncCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtDuncCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncCancelConnectReqSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_RES
CsrSize CsrBtDuncPortnegResSizeof(void *msg);
CsrUint8 *CsrBtDuncPortnegResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncPortnegResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncPortnegResSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ
CsrSize CsrBtDuncRegisterDataPathHandleReqSizeof(void *msg);
CsrUint8 *CsrBtDuncRegisterDataPathHandleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncRegisterDataPathHandleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncRegisterDataPathHandleReqSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_REQ
CsrSize CsrBtDuncSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtDuncSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncSecurityOutReqSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_CONNECT_REQ
CsrSize CsrBtDuncConnectReqSizeof(void *msg);
CsrUint8 *CsrBtDuncConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncConnectReqSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_PORTNEG_REQ
CsrSize CsrBtDuncPortnegReqSizeof(void *msg);
CsrUint8 *CsrBtDuncPortnegReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncPortnegReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncPortnegReqSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_DUNC_STATUS_IND
CsrSize CsrBtDuncStatusIndSizeof(void *msg);
CsrUint8 *CsrBtDuncStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncStatusIndSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DUNC_CONNECT_CFM
CsrSize CsrBtDuncConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtDuncConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncConnectCfmSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_DUNC_CONTROL_REQ
CsrSize CsrBtDuncControlReqSizeof(void *msg);
CsrUint8 *CsrBtDuncControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDuncControlReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDuncControlReqSerFree CsrBtDuncPfree
#endif /* EXCLUDE_CSR_BT_DUNC_CONTROL_REQ */

#ifdef __cplusplus
}
#endif

#endif
