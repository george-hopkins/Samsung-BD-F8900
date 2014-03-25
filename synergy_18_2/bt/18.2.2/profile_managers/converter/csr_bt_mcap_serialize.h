#ifndef CSR_BT_MCAP_SERIALIZE_H__
#define CSR_BT_MCAP_SERIALIZE_H__
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

void CsrBtMcapPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_RES
CsrSize CsrBtMcapSyncSetResSizeof(void *msg);
CsrUint8 *CsrBtMcapSyncSetResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapSyncSetResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapSyncSetResSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_CFM
CsrSize CsrBtMcapDisconnectMdlCfmSizeof(void *msg);
CsrUint8 *CsrBtMcapDisconnectMdlCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDisconnectMdlCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDisconnectMdlCfmSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_CFM
CsrSize CsrBtMcapConnectMclCfmSizeof(void *msg);
CsrUint8 *CsrBtMcapConnectMclCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapConnectMclCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapConnectMclCfmSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_CFM
CsrSize CsrBtMcapReconnectMdlCfmSizeof(void *msg);
CsrUint8 *CsrBtMcapReconnectMdlCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapReconnectMdlCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapReconnectMdlCfmSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_INFO_IND
CsrSize CsrBtMcapSyncInfoIndSizeof(void *msg);
CsrUint8 *CsrBtMcapSyncInfoIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapSyncInfoIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapSyncInfoIndSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_INFO_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ
CsrSize CsrBtMcapConnectMdlReqSizeof(void *msg);
CsrUint8 *CsrBtMcapConnectMdlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapConnectMdlReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMcapConnectMdlReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_CFM
CsrSize CsrBtMcapDeleteMdlCfmSizeof(void *msg);
CsrUint8 *CsrBtMcapDeleteMdlCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDeleteMdlCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDeleteMdlCfmSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_IND
CsrSize CsrBtMcapCreateMdlIndSizeof(void *msg);
CsrUint8 *CsrBtMcapCreateMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapCreateMdlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapCreateMdlIndSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_IND
CsrSize CsrBtMcapReconnectMdlIndSizeof(void *msg);
CsrUint8 *CsrBtMcapReconnectMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapReconnectMdlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapReconnectMdlIndSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_CFM
CsrSize CsrBtMcapSyncSetCfmSizeof(void *msg);
CsrUint8 *CsrBtMcapSyncSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapSyncSetCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapSyncSetCfmSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES
CsrSize CsrBtMcapCreateMdlResSizeof(void *msg);
CsrUint8 *CsrBtMcapCreateMdlResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapCreateMdlResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMcapCreateMdlResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_CFM
CsrSize CsrBtMcapConnectMdlCfmSizeof(void *msg);
CsrUint8 *CsrBtMcapConnectMdlCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapConnectMdlCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapConnectMdlCfmSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_CFM
CsrSize CsrBtMcapDisconnectMclCfmSizeof(void *msg);
CsrUint8 *CsrBtMcapDisconnectMclCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDisconnectMclCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDisconnectMclCfmSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DEACTIVATE_CFM
CsrSize CsrBtMcapDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtMcapDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDeactivateCfmSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_ABORT_MDL_RES
CsrSize CsrBtMcapAbortMdlResSizeof(void *msg);
CsrUint8 *CsrBtMcapAbortMdlResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapAbortMdlResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapAbortMdlResSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_ABORT_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES
CsrSize CsrBtMcapReconnectMdlResSizeof(void *msg);
CsrUint8 *CsrBtMcapReconnectMdlResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapReconnectMdlResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMcapReconnectMdlResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_DEACTIVATE_REQ
CsrSize CsrBtMcapDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtMcapDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDeactivateReqSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_RES
CsrSize CsrBtMcapSyncCapResSizeof(void *msg);
CsrUint8 *CsrBtMcapSyncCapResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapSyncCapResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapSyncCapResSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_REQ
CsrSize CsrBtMcapDisconnectMclReqSizeof(void *msg);
CsrUint8 *CsrBtMcapDisconnectMclReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDisconnectMclReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDisconnectMclReqSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_IND
CsrSize CsrBtMcapDisconnectMdlIndSizeof(void *msg);
CsrUint8 *CsrBtMcapDisconnectMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDisconnectMdlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDisconnectMdlIndSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_REQ
CsrSize CsrBtMcapSyncSetReqSizeof(void *msg);
CsrUint8 *CsrBtMcapSyncSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapSyncSetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapSyncSetReqSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_REQ
CsrSize CsrBtMcapReconnectMdlReqSizeof(void *msg);
CsrUint8 *CsrBtMcapReconnectMdlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapReconnectMdlReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapReconnectMdlReqSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_IND
CsrSize CsrBtMcapConnectMclIndSizeof(void *msg);
CsrUint8 *CsrBtMcapConnectMclIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapConnectMclIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapConnectMclIndSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_CFM
CsrSize CsrBtMcapSyncCapCfmSizeof(void *msg);
CsrUint8 *CsrBtMcapSyncCapCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapSyncCapCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapSyncCapCfmSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_CFM
CsrSize CsrBtMcapDataSendCfmSizeof(void *msg);
CsrUint8 *CsrBtMcapDataSendCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDataSendCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDataSendCfmSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_IND
CsrSize CsrBtMcapSyncSetIndSizeof(void *msg);
CsrUint8 *CsrBtMcapSyncSetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapSyncSetIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapSyncSetIndSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_ACTIVATE_REQ
CsrSize CsrBtMcapActivateReqSizeof(void *msg);
CsrUint8 *CsrBtMcapActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapActivateReqSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_IND
CsrSize CsrBtMcapConnectMdlIndSizeof(void *msg);
CsrUint8 *CsrBtMcapConnectMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapConnectMdlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapConnectMdlIndSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_REQ
CsrSize CsrBtMcapDisconnectMdlReqSizeof(void *msg);
CsrUint8 *CsrBtMcapDisconnectMdlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDisconnectMdlReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDisconnectMdlReqSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_REQ
CsrSize CsrBtMcapConnectMclReqSizeof(void *msg);
CsrUint8 *CsrBtMcapConnectMclReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapConnectMclReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapConnectMclReqSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_IND
CsrSize CsrBtMcapDeleteMdlIndSizeof(void *msg);
CsrUint8 *CsrBtMcapDeleteMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDeleteMdlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDeleteMdlIndSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ
CsrSize CsrBtMcapDataSendReqSizeof(void *msg);
CsrUint8 *CsrBtMcapDataSendReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDataSendReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMcapDataSendReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_INFO_REQ
CsrSize CsrBtMcapSyncInfoReqSizeof(void *msg);
CsrUint8 *CsrBtMcapSyncInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapSyncInfoReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapSyncInfoReqSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_REQ
CsrSize CsrBtMcapDeleteMdlReqSizeof(void *msg);
CsrUint8 *CsrBtMcapDeleteMdlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDeleteMdlReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDeleteMdlReqSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_REQ
CsrSize CsrBtMcapSyncCapReqSizeof(void *msg);
CsrUint8 *CsrBtMcapSyncCapReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapSyncCapReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapSyncCapReqSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_RES
CsrSize CsrBtMcapDeleteMdlResSizeof(void *msg);
CsrUint8 *CsrBtMcapDeleteMdlResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDeleteMdlResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDeleteMdlResSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND
CsrSize CsrBtMcapDataSendIndSizeof(void *msg);
CsrUint8 *CsrBtMcapDataSendIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDataSendIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtMcapDataSendIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_IND
CsrSize CsrBtMcapSyncCapIndSizeof(void *msg);
CsrUint8 *CsrBtMcapSyncCapIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapSyncCapIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapSyncCapIndSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_ACTIVATE_CFM
CsrSize CsrBtMcapActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtMcapActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapActivateCfmSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_ABORT_MDL_IND
CsrSize CsrBtMcapAbortMdlIndSizeof(void *msg);
CsrUint8 *CsrBtMcapAbortMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapAbortMdlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapAbortMdlIndSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_ABORT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_IND
CsrSize CsrBtMcapDisconnectMclIndSizeof(void *msg);
CsrUint8 *CsrBtMcapDisconnectMclIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtMcapDisconnectMclIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtMcapDisconnectMclIndSerFree CsrBtMcapPfree
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_IND */

#ifdef __cplusplus
}
#endif

#endif
