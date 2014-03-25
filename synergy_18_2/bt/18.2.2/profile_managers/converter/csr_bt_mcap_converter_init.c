/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_mcap_converter_init.h"
#ifndef EXCLUDE_CSR_BT_MCAP_MODULE
#include "csr_bt_mcap_serialize.h"
#include "csr_bt_mcap_private_prim.h"

static CsrMsgConvMsgEntry csr_bt_mcap_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_MCAP_ABORT_MDL_IND
    { CSR_BT_MCAP_ABORT_MDL_IND, CsrBtMcapAbortMdlIndSizeof, CsrBtMcapAbortMdlIndSer, CsrBtMcapAbortMdlIndDes, CsrBtMcapAbortMdlIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_ABORT_MDL_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_ABORT_MDL_RES
    { CSR_BT_MCAP_ABORT_MDL_RES, CsrBtMcapAbortMdlResSizeof, CsrBtMcapAbortMdlResSer, CsrBtMcapAbortMdlResDes, CsrBtMcapAbortMdlResSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_ABORT_MDL_RES */
#ifndef EXCLUDE_CSR_BT_MCAP_ACTIVATE_CFM
    { CSR_BT_MCAP_ACTIVATE_CFM, CsrBtMcapActivateCfmSizeof, CsrBtMcapActivateCfmSer, CsrBtMcapActivateCfmDes, CsrBtMcapActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_MCAP_ACTIVATE_REQ
    { CSR_BT_MCAP_ACTIVATE_REQ, CsrBtMcapActivateReqSizeof, CsrBtMcapActivateReqSer, CsrBtMcapActivateReqDes, CsrBtMcapActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_CFM
    { CSR_BT_MCAP_CONNECT_MCL_CFM, CsrBtMcapConnectMclCfmSizeof, CsrBtMcapConnectMclCfmSer, CsrBtMcapConnectMclCfmDes, CsrBtMcapConnectMclCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_CFM */
#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_IND
    { CSR_BT_MCAP_CONNECT_MCL_IND, CsrBtMcapConnectMclIndSizeof, CsrBtMcapConnectMclIndSer, CsrBtMcapConnectMclIndDes, CsrBtMcapConnectMclIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_REQ
    { CSR_BT_MCAP_CONNECT_MCL_REQ, CsrBtMcapConnectMclReqSizeof, CsrBtMcapConnectMclReqSer, CsrBtMcapConnectMclReqDes, CsrBtMcapConnectMclReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_CFM
    { CSR_BT_MCAP_CONNECT_MDL_CFM, CsrBtMcapConnectMdlCfmSizeof, CsrBtMcapConnectMdlCfmSer, CsrBtMcapConnectMdlCfmDes, CsrBtMcapConnectMdlCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_CFM */
#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_IND
    { CSR_BT_MCAP_CONNECT_MDL_IND, CsrBtMcapConnectMdlIndSizeof, CsrBtMcapConnectMdlIndSer, CsrBtMcapConnectMdlIndDes, CsrBtMcapConnectMdlIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ
    { CSR_BT_MCAP_CONNECT_MDL_REQ, CsrBtMcapConnectMdlReqSizeof, CsrBtMcapConnectMdlReqSer, CsrBtMcapConnectMdlReqDes, CsrBtMcapConnectMdlReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_IND
    { CSR_BT_MCAP_CREATE_MDL_IND, CsrBtMcapCreateMdlIndSizeof, CsrBtMcapCreateMdlIndSer, CsrBtMcapCreateMdlIndDes, CsrBtMcapCreateMdlIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES
    { CSR_BT_MCAP_CREATE_MDL_RES, CsrBtMcapCreateMdlResSizeof, CsrBtMcapCreateMdlResSer, CsrBtMcapCreateMdlResDes, CsrBtMcapCreateMdlResSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES */
#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_CFM
    { CSR_BT_MCAP_DATA_SEND_CFM, CsrBtMcapDataSendCfmSizeof, CsrBtMcapDataSendCfmSer, CsrBtMcapDataSendCfmDes, CsrBtMcapDataSendCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_CFM */
#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND
    { CSR_BT_MCAP_DATA_SEND_IND, CsrBtMcapDataSendIndSizeof, CsrBtMcapDataSendIndSer, CsrBtMcapDataSendIndDes, CsrBtMcapDataSendIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ
    { CSR_BT_MCAP_DATA_SEND_REQ, CsrBtMcapDataSendReqSizeof, CsrBtMcapDataSendReqSer, CsrBtMcapDataSendReqDes, CsrBtMcapDataSendReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_DEACTIVATE_CFM
    { CSR_BT_MCAP_DEACTIVATE_CFM, CsrBtMcapDeactivateCfmSizeof, CsrBtMcapDeactivateCfmSer, CsrBtMcapDeactivateCfmDes, CsrBtMcapDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_MCAP_DEACTIVATE_REQ
    { CSR_BT_MCAP_DEACTIVATE_REQ, CsrBtMcapDeactivateReqSizeof, CsrBtMcapDeactivateReqSer, CsrBtMcapDeactivateReqDes, CsrBtMcapDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_CFM
    { CSR_BT_MCAP_DELETE_MDL_CFM, CsrBtMcapDeleteMdlCfmSizeof, CsrBtMcapDeleteMdlCfmSer, CsrBtMcapDeleteMdlCfmDes, CsrBtMcapDeleteMdlCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_CFM */
#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_IND
    { CSR_BT_MCAP_DELETE_MDL_IND, CsrBtMcapDeleteMdlIndSizeof, CsrBtMcapDeleteMdlIndSer, CsrBtMcapDeleteMdlIndDes, CsrBtMcapDeleteMdlIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_REQ
    { CSR_BT_MCAP_DELETE_MDL_REQ, CsrBtMcapDeleteMdlReqSizeof, CsrBtMcapDeleteMdlReqSer, CsrBtMcapDeleteMdlReqDes, CsrBtMcapDeleteMdlReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_RES
    { CSR_BT_MCAP_DELETE_MDL_RES, CsrBtMcapDeleteMdlResSizeof, CsrBtMcapDeleteMdlResSer, CsrBtMcapDeleteMdlResDes, CsrBtMcapDeleteMdlResSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_RES */
#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_CFM
    { CSR_BT_MCAP_DISCONNECT_MCL_CFM, CsrBtMcapDisconnectMclCfmSizeof, CsrBtMcapDisconnectMclCfmSer, CsrBtMcapDisconnectMclCfmDes, CsrBtMcapDisconnectMclCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_CFM */
#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_IND
    { CSR_BT_MCAP_DISCONNECT_MCL_IND, CsrBtMcapDisconnectMclIndSizeof, CsrBtMcapDisconnectMclIndSer, CsrBtMcapDisconnectMclIndDes, CsrBtMcapDisconnectMclIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_REQ
    { CSR_BT_MCAP_DISCONNECT_MCL_REQ, CsrBtMcapDisconnectMclReqSizeof, CsrBtMcapDisconnectMclReqSer, CsrBtMcapDisconnectMclReqDes, CsrBtMcapDisconnectMclReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_CFM
    { CSR_BT_MCAP_DISCONNECT_MDL_CFM, CsrBtMcapDisconnectMdlCfmSizeof, CsrBtMcapDisconnectMdlCfmSer, CsrBtMcapDisconnectMdlCfmDes, CsrBtMcapDisconnectMdlCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_CFM */
#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_IND
    { CSR_BT_MCAP_DISCONNECT_MDL_IND, CsrBtMcapDisconnectMdlIndSizeof, CsrBtMcapDisconnectMdlIndSer, CsrBtMcapDisconnectMdlIndDes, CsrBtMcapDisconnectMdlIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_REQ
    { CSR_BT_MCAP_DISCONNECT_MDL_REQ, CsrBtMcapDisconnectMdlReqSizeof, CsrBtMcapDisconnectMdlReqSer, CsrBtMcapDisconnectMdlReqDes, CsrBtMcapDisconnectMdlReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_CFM
    { CSR_BT_MCAP_RECONNECT_MDL_CFM, CsrBtMcapReconnectMdlCfmSizeof, CsrBtMcapReconnectMdlCfmSer, CsrBtMcapReconnectMdlCfmDes, CsrBtMcapReconnectMdlCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_CFM */
#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_IND
    { CSR_BT_MCAP_RECONNECT_MDL_IND, CsrBtMcapReconnectMdlIndSizeof, CsrBtMcapReconnectMdlIndSer, CsrBtMcapReconnectMdlIndDes, CsrBtMcapReconnectMdlIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_REQ
    { CSR_BT_MCAP_RECONNECT_MDL_REQ, CsrBtMcapReconnectMdlReqSizeof, CsrBtMcapReconnectMdlReqSer, CsrBtMcapReconnectMdlReqDes, CsrBtMcapReconnectMdlReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES
    { CSR_BT_MCAP_RECONNECT_MDL_RES, CsrBtMcapReconnectMdlResSizeof, CsrBtMcapReconnectMdlResSer, CsrBtMcapReconnectMdlResDes, CsrBtMcapReconnectMdlResSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES */
#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_CFM
    { CSR_BT_MCAP_SYNC_CAP_CFM, CsrBtMcapSyncCapCfmSizeof, CsrBtMcapSyncCapCfmSer, CsrBtMcapSyncCapCfmDes, CsrBtMcapSyncCapCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_CFM */
#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_IND
    { CSR_BT_MCAP_SYNC_CAP_IND, CsrBtMcapSyncCapIndSizeof, CsrBtMcapSyncCapIndSer, CsrBtMcapSyncCapIndDes, CsrBtMcapSyncCapIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_REQ
    { CSR_BT_MCAP_SYNC_CAP_REQ, CsrBtMcapSyncCapReqSizeof, CsrBtMcapSyncCapReqSer, CsrBtMcapSyncCapReqDes, CsrBtMcapSyncCapReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_RES
    { CSR_BT_MCAP_SYNC_CAP_RES, CsrBtMcapSyncCapResSizeof, CsrBtMcapSyncCapResSer, CsrBtMcapSyncCapResDes, CsrBtMcapSyncCapResSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_RES */
#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_INFO_IND
    { CSR_BT_MCAP_SYNC_INFO_IND, CsrBtMcapSyncInfoIndSizeof, CsrBtMcapSyncInfoIndSer, CsrBtMcapSyncInfoIndDes, CsrBtMcapSyncInfoIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_INFO_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_INFO_REQ
    { CSR_BT_MCAP_SYNC_INFO_REQ, CsrBtMcapSyncInfoReqSizeof, CsrBtMcapSyncInfoReqSer, CsrBtMcapSyncInfoReqDes, CsrBtMcapSyncInfoReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_INFO_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_CFM
    { CSR_BT_MCAP_SYNC_SET_CFM, CsrBtMcapSyncSetCfmSizeof, CsrBtMcapSyncSetCfmSer, CsrBtMcapSyncSetCfmDes, CsrBtMcapSyncSetCfmSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_CFM */
#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_IND
    { CSR_BT_MCAP_SYNC_SET_IND, CsrBtMcapSyncSetIndSizeof, CsrBtMcapSyncSetIndSer, CsrBtMcapSyncSetIndDes, CsrBtMcapSyncSetIndSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_IND */
#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_REQ
    { CSR_BT_MCAP_SYNC_SET_REQ, CsrBtMcapSyncSetReqSizeof, CsrBtMcapSyncSetReqSer, CsrBtMcapSyncSetReqDes, CsrBtMcapSyncSetReqSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_REQ */
#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_RES
    { CSR_BT_MCAP_SYNC_SET_RES, CsrBtMcapSyncSetResSizeof, CsrBtMcapSyncSetResSer, CsrBtMcapSyncSetResDes, CsrBtMcapSyncSetResSerFree },
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtMcapConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_MCAP_PRIM, csr_bt_mcap_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_mcap_conv_info = {
    CSR_BT_MCAP_PRIM,
    "CSR_BT_MCAP_PRIM",
    csr_bt_mcap_conv_lut
};

CsrLogPrimitiveInformation* CsrBtMcapTechInfoGet(void)
{
    return &csr_bt_mcap_conv_info;
}
#endif
#endif

