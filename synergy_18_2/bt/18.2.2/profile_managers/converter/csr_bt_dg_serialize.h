#ifndef CSR_BT_DG_SERIALIZE_H__
#define CSR_BT_DG_SERIALIZE_H__
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

void CsrBtDgPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_CFM
CsrSize CsrBtDgPortnegCfmSizeof(void *msg);
CsrUint8 *CsrBtDgPortnegCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgPortnegCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgPortnegCfmSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_RES
CsrSize CsrBtDgPortnegResSizeof(void *msg);
CsrUint8 *CsrBtDgPortnegResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgPortnegResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgPortnegResSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_DG_ACTIVATE_REQ
CsrSize CsrBtDgActivateReqSizeof(void *msg);
CsrUint8 *CsrBtDgActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgActivateReqSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DISCONNECT_IND
CsrSize CsrBtDgDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtDgDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgDisconnectIndSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DG_CONTROL_IND
CsrSize CsrBtDgControlIndSizeof(void *msg);
CsrUint8 *CsrBtDgControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgControlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgControlIndSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_DG_CONTROL_REQ
CsrSize CsrBtDgControlReqSizeof(void *msg);
CsrUint8 *CsrBtDgControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgControlReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgControlReqSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DATA_IND
CsrSize CsrBtDgDataIndSizeof(void *msg);
CsrUint8 *CsrBtDgDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtDgDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_DG_DATA_IND */

#ifndef EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_REQ
CsrSize CsrBtDgDataPathStatusReqSizeof(void *msg);
CsrUint8 *CsrBtDgDataPathStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgDataPathStatusReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgDataPathStatusReqSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DEACTIVATE_REQ
CsrSize CsrBtDgDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtDgDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgDeactivateReqSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_STATUS_IND
CsrSize CsrBtDgStatusIndSizeof(void *msg);
CsrUint8 *CsrBtDgStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgStatusIndSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_DG_DEACTIVATE_CFM
CsrSize CsrBtDgDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtDgDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgDeactivateCfmSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ
CsrSize CsrBtDgRegisterDataPathHandleReqSizeof(void *msg);
CsrUint8 *CsrBtDgRegisterDataPathHandleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgRegisterDataPathHandleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgRegisterDataPathHandleReqSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DATA_CFM
CsrSize CsrBtDgDataCfmSizeof(void *msg);
CsrUint8 *CsrBtDgDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgDataCfmSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_DG_DISCONNECT_REQ
CsrSize CsrBtDgDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtDgDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgDisconnectReqSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_DG_HOUSE_CLEANING
CsrSize CsrBtDgHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtDgHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgHouseCleaningSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_IND
CsrSize CsrBtDgPortnegIndSizeof(void *msg);
CsrUint8 *CsrBtDgPortnegIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgPortnegIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgPortnegIndSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM
CsrSize CsrBtDgRegisterDataPathHandleCfmSizeof(void *msg);
CsrUint8 *CsrBtDgRegisterDataPathHandleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgRegisterDataPathHandleCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgRegisterDataPathHandleCfmSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_DG_SECURITY_IN_REQ
CsrSize CsrBtDgSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtDgSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgSecurityInReqSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_DG_SECURITY_IN_CFM
CsrSize CsrBtDgSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtDgSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgSecurityInCfmSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_DG_PORTNEG_REQ
CsrSize CsrBtDgPortnegReqSizeof(void *msg);
CsrUint8 *CsrBtDgPortnegReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgPortnegReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgPortnegReqSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_DG_CONNECT_IND
CsrSize CsrBtDgConnectIndSizeof(void *msg);
CsrUint8 *CsrBtDgConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgConnectIndSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_DG_DATA_REQ
CsrSize CsrBtDgDataReqSizeof(void *msg);
CsrUint8 *CsrBtDgDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtDgDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_DG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_DG_DATA_RES
CsrSize CsrBtDgDataResSizeof(void *msg);
CsrUint8 *CsrBtDgDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgDataResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgDataResSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_DATA_RES */

#ifndef EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_IND
CsrSize CsrBtDgDataPathStatusIndSizeof(void *msg);
CsrUint8 *CsrBtDgDataPathStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDgDataPathStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDgDataPathStatusIndSerFree CsrBtDgPfree
#endif /* EXCLUDE_CSR_BT_DG_DATA_PATH_STATUS_IND */

#ifdef __cplusplus
}
#endif

#endif
