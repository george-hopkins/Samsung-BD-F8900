/****************************************************************************

(c) Cambridge Silicon Radio Limited 2010

All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_MCAP_MODULE

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_mcap_handler.h"
#include "csr_bt_mcap_sef.h"
#include "csr_bt_mcap_util.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

extern CsrUint8 *buildMdErrorRsp(void);
extern CsrUint8 *buildMdCreateMdlReq(McapMdlListEntry *currentMdlPtr);
extern CsrUint8 *buildMdCreateMdlRes(CsrUint8 responseCode, CsrUint16 mdlId, CsrUint8 resPar);
extern CsrUint8 *buildMdDeleteMdlReq(CsrUint16 mdlId);
extern CsrUint8 *buildMdDeleteMdlRes(CsrUint8 responseCode, CsrUint16 mdlId);
extern CsrUint8 *buildMdAbortMdlReq(CsrUint16 mdlId);
extern CsrUint8 *buildMdAbortMdlRes(CsrUint8 responseCode, CsrUint16 mdlId);
extern CsrUint8 *buildMdReconnectMdlReq(McapMdlListEntry *currentMdlPtr);
extern CsrUint8 *buildMdReconnectMdlRes(CsrUint8 responseCode, CsrUint16 mdlId);
extern CsrUint8 *buildMdSyncCapReq(CsrUint16 timeStampReqAcc);
extern CsrUint8 *buildMdSyncCapRes(CsrUint8  responseCode,
                                   CsrUint8  btClkAccRes,
                                   CsrUint16 syncLeadTime,
                                   CsrUint16 timeStampNatRes,
                                   CsrUint16 timeStampNatAcc);
extern CsrUint8 *buildMdSyncSetReq(CsrUint8  timeStampUpdateInfo,
                                   CsrUint32 btClkSyncTime,
                                   CsrUint32 timeStampSyncTimeLsb,
                                   CsrUint32 timeStampSyncTimeMsb);
extern CsrUint8 *buildMdSyncSetRes(CsrUint8  responseCode,
                                   CsrUint32 btClkSyncTime,
                                   CsrUint32 timeStampSyncTimeLsb,
                                   CsrUint32 timeStampSyncTimeMsb,
                                   CsrUint16 timeStampSampleAccuracy);
extern CsrUint8 *buildMdSyncInfoInd(CsrUint32 btClkSyncTime,
                                    CsrUint32 timeStampSyncTimeLsb,
                                    CsrUint32 timeStampSyncTimeMsb,
                                    CsrUint16 timeStampSampleAccuracy);


MclMdlPtr *CsrBtMcapFindMclAndMdlFromCid(McapInstanceData *pInst, CsrBtConnId btConnId);
MclMdlPtr *CsrBtMcapFindFromBdAddrAndMdlState(McapInstanceData *pInst, CsrBtDeviceAddr bd_addr, CsrUint8 mdlState);
void handleMcapMsg(McapInstanceData *pInst, CsrUint16 len, CsrUint8 *msg, McapMclListEntry *mclPtr);
static void csrBtMcapDeleteAllMdlEntries(McapInstanceData *pInst, McapMclListEntry *mclPtr);
static void csrBtMcapDisconnectAllMdlEntries(McapInstanceData *pInst, McapMclListEntry *mclPtr);
static CsrBool csrBtMcapCheckMdlIdCreate(CsrUint16 mdlId, CsrBool ffffAllowed);
static void csrBtMcapSetGlobalSniffTimeout(McapInstanceData *pInst);
static void csrBtMcapTimerHandler(CsrUint16 dummy, void * inst);
static void csrBtMcapModeChangeReq(McapInstanceData *pInst, CsrUint8 newMode);
static void csrBtMcapInvalidOpCodeSend(McapMclListEntry *mclPtr);

/********************************************************/
/*                                                      */
/*    Upstream primitive send functions                 */
/*                                                      */
/********************************************************/
static void csrBtMcapMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_MCAP_PRIM, msg);
}

static void csrBtMcapActivateCfmSend(CsrSchedQid appHandle, psm_t psm, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_MCAP_ACTIVATE_CFM message to the application */
    CsrBtMcapActivateCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapActivateCfm));

    pMsg->type = CSR_BT_MCAP_ACTIVATE_CFM;
    pMsg->psm = psm;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapDeactivateCfmSend(CsrSchedQid appHandle, psm_t psm, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_MCAP_DEACTIVATE_CFM message to the application */
    CsrBtMcapDeactivateCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapDeactivateCfm));

    pMsg->type = CSR_BT_MCAP_DEACTIVATE_CFM;
    pMsg->psm = psm;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapConnectMclIndSend(CsrSchedQid appHandle, CsrBtDeviceAddr bd_addr, CsrUint16 mclId, psm_t localPsm, psm_t remotePsm, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_MCAP_CONNECT_MCL_IND message to the application */
    CsrBtMcapConnectMclInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapConnectMclInd));

    pMsg->type = CSR_BT_MCAP_CONNECT_MCL_IND;
    pMsg->bd_addr = bd_addr;
    pMsg->mclId = mclId;
    pMsg->localPsm = localPsm;
    pMsg->remotePsm = remotePsm;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapConnectMclCfmSend(CsrSchedQid appHandle, CsrBtDeviceAddr bd_addr, CsrUint16 mclId, psm_t psm, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_MCAP_CONNECT_MCL_CFM message to the application */
    CsrBtMcapConnectMclCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapConnectMclCfm));

    pMsg->type = CSR_BT_MCAP_CONNECT_MCL_CFM;
    pMsg->bd_addr = bd_addr;
    pMsg->mclId = mclId;
    pMsg->psm = psm;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapDisconnectMclIndSend(CsrSchedQid appHandle, CsrBtDeviceAddr bd_addr, CsrUint16 mclId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a MCAP_DISCONNECT_IND_CFM message to the application */
    CsrBtMcapDisconnectMclInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapDisconnectMclInd));

    pMsg->type = CSR_BT_MCAP_DISCONNECT_MCL_IND;
    pMsg->bd_addr = bd_addr;
    pMsg->mclId = mclId;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapDisconnectMclCfmSend(CsrSchedQid appHandle, CsrBtDeviceAddr bd_addr, CsrUint16 mclId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_MCAP_DISCONNECT_MCL_CFM message to the application */
    CsrBtMcapDisconnectMclCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapDisconnectMclCfm));

    pMsg->type = CSR_BT_MCAP_DISCONNECT_MCL_CFM;
    pMsg->bd_addr = bd_addr;
    pMsg->mclId = mclId;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapDataSendIndSend(CsrSchedQid appHandle, CsrUint16 mclId, CsrUint16 mdlId, CsrUint16 length, CsrUint8 *dataPtr)
{
    /* Send a CSR_BT_MCAP_DATA_SEND_IND message to the application */
    CsrBtMcapDataSendInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapDataSendInd));

    pMsg->type = CSR_BT_MCAP_DATA_SEND_IND;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    pMsg->length = length;
    pMsg->payload = dataPtr;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapDataSendCfmSend(CsrSchedQid appHandle, CsrUint16 mclId, CsrUint16 mdlId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_MCAP_DATA_SEND_CFM message to the application */
    CsrBtMcapDataSendCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapDataSendCfm));

    pMsg->type = CSR_BT_MCAP_DATA_SEND_CFM;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapCreateMdlIndSend(CsrSchedQid appHandle, CsrUint16 mclId, CsrUint16 mdlId,CsrUint8 mdepId, CsrUint8 conf)
{
    /* Send a CSR_BT_MCAP_CREATE_MDL_IND message to the application */
    CsrBtMcapCreateMdlInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapCreateMdlInd));

    pMsg->type = CSR_BT_MCAP_CREATE_MDL_IND;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    pMsg->mdepId = mdepId;
    pMsg->configuration = conf;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapConnectMdlIndSend(CsrSchedQid appHandle,
                                       CsrUint16 mclId,
                                       CsrUint16 mdlId,
                                       psm_t localPsm,
                                       psm_t remotePsm,
                                       l2ca_mtu_t localMtu,
                                       l2ca_mtu_t remoteMtu,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_MCAP_CONNECT_MDL_IND message to the application */
    CsrBtMcapConnectMdlInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapConnectMdlInd));

    pMsg->type = CSR_BT_MCAP_CONNECT_MDL_IND;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    pMsg->localPsm = localPsm;
    pMsg->remotePsm = remotePsm;
    pMsg->localMtu = localMtu;
    pMsg->remoteMtu = remoteMtu;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapConnectMdlCfmSend(CsrSchedQid appHandle,
                                       CsrUint16 mclId,
                                       CsrUint16 mdlId,
                                       psm_t localPsm,
                                       psm_t remotePsm,
                                       l2ca_mtu_t localMtu,
                                       l2ca_mtu_t remoteMtu,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_MCAP_CONNECT_MDL_CFM message to the application */
    CsrBtMcapConnectMdlCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapConnectMdlCfm));

    pMsg->type = CSR_BT_MCAP_CONNECT_MDL_CFM;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    pMsg->localPsm = localPsm;
    pMsg->remotePsm = remotePsm;
    pMsg->localMtu = localMtu;
    pMsg->remoteMtu = remoteMtu;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapDeleteMdlIndSend(CsrSchedQid appHandle, CsrUint16 mclId, CsrUint16 mdlId)
{
    /* Send a CSR_BT_MCAP_DELETE_MDL_IND message to the application */
    CsrBtMcapDeleteMdlInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapDeleteMdlInd));

    pMsg->type = CSR_BT_MCAP_DELETE_MDL_IND;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapDeleteMdlCfmSend(CsrSchedQid appHandle, CsrUint16 mclId, CsrUint16 mdlId, CsrUint8 result)
{
    /* Send a CSR_BT_MCAP_DELETE_MDL_CFM message to the application */
    CsrBtMcapDeleteMdlCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapDeleteMdlCfm));

    pMsg->type = CSR_BT_MCAP_DELETE_MDL_CFM;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    pMsg->mcapResponse = (McapResponseType)result;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapAbortMdlIndSend(CsrSchedQid appHandle, CsrUint16 mclId, CsrUint16 mdlId)
{
    /* Send a CSR_BT_MCAP_ABORT_MDL_IND message to the application */
    CsrBtMcapAbortMdlInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapAbortMdlInd));

    pMsg->type = CSR_BT_MCAP_ABORT_MDL_IND;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapDisconnectMdlIndSend(CsrSchedQid appHandle, CsrUint16 mclId, CsrUint16 mdlId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_MCAP_DISCONNECT_MDL_IND message to the application */
    CsrBtMcapDisconnectMdlInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapDisconnectMdlInd));

    pMsg->type = CSR_BT_MCAP_DISCONNECT_MDL_IND;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapDisconnectMdlCfmSend(CsrSchedQid appHandle, CsrUint16 mclId, CsrUint16 mdlId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_MCAP_DISCONNECT_MDL_CFM message to the application */
    CsrBtMcapDisconnectMdlCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapDisconnectMdlCfm));

    pMsg->type = CSR_BT_MCAP_DISCONNECT_MDL_CFM;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapReconnectMdlIndSend(CsrSchedQid appHandle,
                                         CsrUint16 mclId,
                                         CsrUint16 mdlId,
                                         psm_t localPsm,
                                         psm_t remotePsm,
                                         l2ca_mtu_t localMtu,
                                         l2ca_mtu_t remoteMtu,
                                         CsrBtDeviceAddr bd_addr)
{
    /* Send a CSR_BT_MCAP_RECONNECT_MDL_IND message to the application */
    CsrBtMcapReconnectMdlInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapReconnectMdlInd));

    pMsg->type = CSR_BT_MCAP_RECONNECT_MDL_IND;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    pMsg->localPsm = localPsm;
    pMsg->remotePsm = remotePsm;
    pMsg->localMtu = localMtu;
    pMsg->remoteMtu = remoteMtu;
    pMsg->bd_addr = bd_addr;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapReconnectMdlCfmSend(CsrSchedQid appHandle, CsrUint16 mclId, CsrUint16 mdlId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_MCAP_RECONNECT_MDL_CFM message to the application */
    CsrBtMcapReconnectMdlCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapReconnectMdlCfm));

    pMsg->type = CSR_BT_MCAP_RECONNECT_MDL_CFM;
    pMsg->mclId = mclId;
    pMsg->mdlId = mdlId;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapSyncCapIndSend(CsrSchedQid appHandle, CsrUint16 mclId, CsrUint16 timeStampRequiredAccuracy)
{
    /* Send a CSR_BT_MCAP_SYNC_CAP_IND message to the application */
    CsrBtMcapSyncCapInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapSyncCapInd));

    pMsg->type = CSR_BT_MCAP_SYNC_CAP_IND;
    pMsg->mclId = mclId;
    pMsg->timeStampRequiredAccuracy = timeStampRequiredAccuracy;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapSyncCapCfmSend(CsrSchedQid        appHandle,
                                    CsrUint16         mclId,
                                    McapResponseType mcapResponse,
                                    CsrUint8          btClkAccessRes,
                                    CsrUint16         syncLeadTime,
                                    CsrUint16         timeStampNativeRes,
                                    CsrUint16         timeStampNativeAccuracy)
{
    /* Send a CSR_BT_MCAP_SYNC_CAP_CFM message to the application */
    CsrBtMcapSyncCapCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapSyncCapCfm));

    pMsg->type = CSR_BT_MCAP_SYNC_CAP_CFM;
    pMsg->mclId = mclId;
    pMsg->mcapResponse = mcapResponse;
    pMsg->btClkAccessRes = btClkAccessRes;
    pMsg->syncLeadTime = syncLeadTime;
    pMsg->timeStampNativeRes = timeStampNativeRes;
    pMsg->timeStampNativeAccuracy = timeStampNativeAccuracy;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapSyncSetIndSend(CsrSchedQid    appHandle,
                                    CsrUint16     mclId,
                                    CsrUint8      tsUpdateInfo,
                                    CsrUint32     btClkSyncTime,
                                    CsrUint32     tsSyncTimeLsb,
                                    CsrUint32     tsSyncTimeMsb)
{
    /* Send a CSR_BT_MCAP_SYNC_SET_IND message to the application */
    CsrBtMcapSyncSetInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapSyncSetInd));

    pMsg->type = CSR_BT_MCAP_SYNC_SET_IND;
    pMsg->mclId = mclId;
    pMsg->timeStampUpdateInfo = tsUpdateInfo;
    pMsg->btClkSyncTime = btClkSyncTime;
    pMsg->timeStampSyncTimeLsb = tsSyncTimeLsb;
    pMsg->timeStampSyncTimeMsb = tsSyncTimeMsb;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapSyncSetCfmSend(CsrSchedQid        appHandle,
                                    CsrUint16         mclId,
                                    McapResponseType responseCode,
                                    CsrUint32         btClkSyncTime,
                                    CsrUint32         timeStampSyncTimeLsb,
                                    CsrUint32         timeStampSyncTimeMsb,
                                    CsrUint16         timeStampSampleAccuracy)
{
    /* Send a CSR_BT_MCAP_SYNC_SET_CFM message to the application */
    CsrBtMcapSyncSetCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapSyncSetCfm));

    pMsg->type = CSR_BT_MCAP_SYNC_SET_CFM;
    pMsg->mclId = mclId;
    pMsg->mcapResponse = responseCode;
    pMsg->btClkSyncTime = btClkSyncTime;
    pMsg->timeStampSyncTimeLsb = timeStampSyncTimeLsb;
    pMsg->timeStampSyncTimeMsb = timeStampSyncTimeLsb;
    pMsg->timeStampSampleAccuracy = timeStampSampleAccuracy;
    csrBtMcapMessagePut(appHandle, pMsg);
}

static void csrBtMcapSyncInfoIndSend(CsrSchedQid        appHandle,
                                     CsrUint16         mclId,
                                     CsrUint32         btClkSyncTime,
                                     CsrUint32         timeStampSyncTimeLsb,
                                     CsrUint32         timeStampSyncTimeMsb,
                                     CsrUint16         timeStampSampleAccuracy)
{
    /* Send a CSR_BT_MCAP_SYNC_INFO_IND message to the application */
    CsrBtMcapSyncInfoInd *pMsg = CsrPmemAlloc(sizeof(CsrBtMcapSyncInfoInd));

    pMsg->type = CSR_BT_MCAP_SYNC_INFO_IND;
    pMsg->mclId = mclId;
    pMsg->btClkSyncTime = btClkSyncTime;
    pMsg->timeStampSyncTimeLsb = timeStampSyncTimeLsb;
    pMsg->timeStampSyncTimeMsb = timeStampSyncTimeLsb;
    pMsg->timeStampSampleAccuracy = timeStampSampleAccuracy;
    csrBtMcapMessagePut(appHandle, pMsg);
}



/********************************************************/
/*                                                      */
/*    MCAP primitive handlers                           */
/*                                                      */
/********************************************************/

void CsrBtMcapActivateReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapActivateReq    *msg;
    McapMclListEntry        *currentMclPtr = NULL;
    McapPsmListEntry        *currentPsmPtr = NULL;
    L2CA_FLOW_T             *flow = CsrPmemAlloc(sizeof(L2CA_FLOW_T));

    msg = (CsrBtMcapActivateReq *)pInst->recvMsgP;

    currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->psm));
    if (currentPsmPtr != NULL)
    {
        currentPsmPtr->allowedMcls += msg->noOfConnections;
    }
    else
    {
        currentPsmPtr = MCAP_PSM_ADD_ENTRY(pInst->psmList);
        currentPsmPtr->psm = msg->psm;
        currentPsmPtr->allowedMcls = msg->noOfConnections;
    }
    currentMclPtr = MCAP_MCL_ADD_ENTRY(pInst->mclList);
    currentMclPtr->sniffTimeout = msg->sniffTimeout;
    currentMclPtr->localControlPsm = msg->psm;
    currentMclPtr->classOfDevice = msg->classOfDevice;
    currentMclPtr->secLevel = msg->secLevel;
    currentMclPtr->mtu = msg->mtu;
    currentMclPtr->uuid = msg->uuid;
    currentMclPtr->phandle = msg->phandle;
    currentMclPtr->mclRole = MCAP_SERVER;
    currentMclPtr->deleteRole = MCAP_RESPONDER;
    currentMclPtr->flow->mode = L2CA_FLOW_MODE_ENHANCED_RETRANS;
    currentMclPtr->flow->tx_window = 1;
    currentMclPtr->flow->max_retransmit = MCAP_CONTROL_CHANNEL_MAX_RETRANS;
    currentMclPtr->flow->retrans_timeout = MCAP_CONTROL_CHANNEL_RETRANS_TIMEOUT;
    currentMclPtr->flow->monitor_timeout = MCAP_CONTROL_CHANNEL_MONITOR_TIMEOUT;
    currentMclPtr->flow->maximum_pdu = msg->mtu;
    CsrMemCpy(flow, currentMclPtr->flow, sizeof(L2CA_FLOW_T));
    CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_NULL_STATE);

    CsrBtCml2caFecConnectAcceptReqSendExt( CSR_BT_MCAP_IFACEQUEUE,
                                            msg->psm,
                                            msg->classOfDevice,
                                            msg->secLevel,
                                            msg->mtu,
                                            L2CA_FLUSH_TO_DEFAULT,
                                            NULL,                /* L2CA_QOS_T serviceQuality */
                                            flow,
                                            FALSE,              /* CsrBool fallbackBasicMode */
                                            msg->uuid);

    csrBtMcapActivateCfmSend(msg->phandle,
                            msg->psm,
                            CSR_BT_RESULT_CODE_MCAP_SUCCESS,
                            CSR_BT_SUPPLIER_MCAP);
}

void CsrBtMcapDeactivateReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapDeactivateReq   *msg;
    McapPsmListEntry    *currentPsmPtr = NULL;
    McapMclListEntry    *currentMclPtr = NULL;
    McapMdlListEntry    *currentMdlPtr = NULL;

    msg = (CsrBtMcapDeactivateReq *)pInst->recvMsgP;

    if (pInst->mclList.first)
    {
        currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_CONTROL_PSM(pInst->mclList, &(msg->psm));
    }

    if (currentMclPtr == NULL) /* if psm is not in use */
    {
        csrBtMcapDeactivateCfmSend(msg->phandle,
                                    msg->psm,
                                    CSR_BT_RESULT_CODE_MCAP_INVALID_PARAMETER_VALUE,
                                    CSR_BT_SUPPLIER_MCAP);
        currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->psm));
        if (currentPsmPtr != NULL)
        {
            MCAP_PSM_REMOVE_ENTRY(pInst->psmList, currentPsmPtr);
        }
    }
    else if (currentMclPtr->mclState == MCAP_MCL_DEACTIVATING_STATE)
    {
        csrBtMcapDeactivateCfmSend(msg->phandle,
                                    msg->psm,
                                    CSR_BT_RESULT_CODE_MCAP_INVALID_OPERATION,
                                    CSR_BT_SUPPLIER_MCAP);
        currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->psm));
        if (currentPsmPtr != NULL)
        {
            MCAP_PSM_REMOVE_ENTRY(pInst->psmList, currentPsmPtr);
        }
    }
    else if (currentMclPtr->mclState == MCAP_MCL_CONNECTED_STATE)
    {
        CsrUint8 searchState;

        CsrBtCml2caDisconnectReqSend(currentMclPtr->btConnId);
        CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
        CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_DEACTIVATING_STATE);

        searchState = MCAP_MDL_CONNECTING_STATE;
        currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
        if (currentMdlPtr == NULL)
        {
            searchState = MCAP_MDL_RECONNECTING_STATE;
            currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
        }
        if (currentMdlPtr != NULL)
        {
            CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_MCAP_IFACEQUEUE, currentMclPtr->localDataPsm);
            CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_DEACTIVATING_STATE);
        }
    }
    else if (currentMclPtr->mclState == MCAP_MCL_NULL_STATE)
    {
        currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->psm));
        if (currentPsmPtr != NULL)
        {
            if (currentPsmPtr->allowedMcls > currentPsmPtr->currentMcls)
            {
                CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_MCAP_IFACEQUEUE, msg->psm);
                CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_DEACTIVATING_STATE);
            }
            else
            {
                csrBtMcapDeactivateCfmSend(msg->phandle,
                                            msg->psm,
                                            CSR_BT_RESULT_CODE_MCAP_SUCCESS,
                                            CSR_BT_SUPPLIER_MCAP);
                MCAP_PSM_REMOVE_ENTRY(pInst->psmList, currentPsmPtr);
            }
        }
    }
    else
    {
        if (currentMclPtr->mdlList.first)
        {
            CsrBool  loop = TRUE;
            CsrUint8 searchState;

            searchState = MCAP_MDL_CONNECTING_STATE;
            currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
            if (currentMdlPtr == NULL)
            {
                searchState = MCAP_MDL_RECONNECTING_STATE;
                currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
            }
            if (currentMdlPtr != NULL)
            {
                CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_MCAP_IFACEQUEUE, currentMclPtr->localDataPsm);
            }
            else
            {
                currentMdlPtr = (McapMdlListEntry *)(currentMclPtr->mdlList.first);
                while (loop && currentMdlPtr != NULL)
                {
                    if (currentMdlPtr->mdlState == MCAP_MDL_ACTIVE_STATE)
                    {
                        CsrBtCml2caDisconnectReqSend(currentMdlPtr->btConnId);
                        CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_DISCONNECTING_STATE);
                        loop = FALSE;
                    }
                    else
                    {
                        currentMdlPtr = currentMdlPtr->next;
                    }
                }
            }
            if (currentMdlPtr == NULL)  /* No active MDLs on this MCL */
            {
                CsrBtCml2caDisconnectReqSend(currentMclPtr->btConnId);
                CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
            }
        }
        else
        {
            currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->psm));
            if (currentPsmPtr != NULL)
            {
                if (currentPsmPtr->allowedMcls > currentPsmPtr->currentMcls)
                {
                    CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_MCAP_IFACEQUEUE, msg->psm);
                }
                else
                {
                    CsrBtCml2caDisconnectReqSend(currentMclPtr->btConnId);
                    CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
                }
            }
        }
        CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_DEACTIVATING_STATE);
        currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->psm));
        if (currentPsmPtr != NULL)
        {
            currentPsmPtr->allowedMcls = 0;
        }
    }

}

void CsrBtMcapConnectMclReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapConnectMclReq   *msg;
    McapMclListEntry    *currentMclPtr = NULL;
    L2CA_FLOW_T      *flow = NULL;

    msg = (CsrBtMcapConnectMclReq *)pInst->recvMsgP;

    if (pInst->mclList.first)
    {
        currentMclPtr = MCAP_MCL_FIND_FROM_BDADDR(pInst->mclList, &(msg->bd_addr));
    }

    if (currentMclPtr == NULL) /* if BD address is not in use */
    {
        currentMclPtr = MCAP_MCL_ADD_ENTRY(pInst->mclList);
    }
    if (currentMclPtr->localControlPsm == msg->localPsm && bd_addr_eq(&(currentMclPtr->bd_addr), &(msg->bd_addr)) &&
        (currentMclPtr->mclState == MCAP_MCL_CONNECTED_STATE))
    { /* Only one contol connection is allowed to the same PSM on the same device */
        csrBtMcapConnectMclCfmSend(currentMclPtr->phandle, msg->bd_addr,
                                    currentMclPtr->mclId, msg->localPsm, CSR_BT_RESULT_CODE_MCAP_PROTOCOL_RESOURCE_UNAVAILABLE,
                                    CSR_BT_SUPPLIER_MCAP_PROTOCOL);
    }
    else
    {
        currentMclPtr->bd_addr = msg->bd_addr;
        currentMclPtr->localControlPsm = msg->localPsm;
        currentMclPtr->remoteControlPsm = msg->remotePsm;
        currentMclPtr->phandle = msg->phandle;
        currentMclPtr->secLevel = msg->secLevel;
        currentMclPtr->mtu = MCAP_CONTROL_MTU;
        currentMclPtr->sniffTimeout = msg->sniffTimeout;
        currentMclPtr->mclRole = MCAP_CLIENT;
        CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_CONNECTING_STATE);

        flow = CsrPmemAlloc(sizeof(L2CA_FLOW_T));
        flow->mode =            L2CA_FLOW_MODE_ENHANCED_RETRANS;
        flow->tx_window =       1;
        flow->max_retransmit =  MCAP_CONTROL_CHANNEL_MAX_RETRANS;
        flow->retrans_timeout = MCAP_CONTROL_CHANNEL_RETRANS_TIMEOUT;
        flow->monitor_timeout = MCAP_CONTROL_CHANNEL_MONITOR_TIMEOUT;
        flow->maximum_pdu =     currentMclPtr->mtu;

        CsrMemCpy(currentMclPtr->flow, flow, sizeof(L2CA_FLOW_T));
        
        CsrBtCml2caFecConnectReqSendExt(CSR_BT_MCAP_IFACEQUEUE,
                                        currentMclPtr->bd_addr,
                                        currentMclPtr->localControlPsm,
                                        msg->remotePsm,
                                        currentMclPtr->mtu,
                                        L2CA_FLUSH_TO_DEFAULT,
                                        NULL,                      /* L2CA_QOS_T serviceQuality */
                                        flow,
                                        FALSE,                  /* CsrBool fallbackBasicMode */
                                        currentMclPtr->secLevel);
    }
}

void CsrBtMcapDisconnectMclReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapDisconnectMclReq    *msg;
    McapMclListEntry        *currentMclPtr = NULL;

    msg = (CsrBtMcapDisconnectMclReq *)pInst->recvMsgP;

    if (pInst->mclList.first)
    {
        currentMclPtr = MCAP_MCL_FIND_FROM_BDADDR(pInst->mclList, &(msg->bd_addr));
        if (currentMclPtr == NULL)
        {
            currentMclPtr = MCAP_MCL_FIND_FROM_REMOTE_CONTROL_PSM(pInst->mclList, &(msg->psm));
        }
    }

    if (currentMclPtr != NULL) /* if BD address is in use */
    {
        if (currentMclPtr->mdlList.count == 0)
        {
            if (currentMclPtr->mclState == MCAP_MCL_CONNECTING_STATE)
            {
                CsrBtCml2caCancelConnectReqSend(CSR_BT_MCAP_IFACEQUEUE,
                                                currentMclPtr->bd_addr,
                                                currentMclPtr->localControlPsm);
                CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
                CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_DISCONNECTING_STATE);
            }
            else
            {
                CsrBtCml2caDisconnectReqSend(currentMclPtr->btConnId);
                CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
                CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_DISCONNECTING_STATE);
            }
        }
        else
        {
            CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
            CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_DISCONNECTING_STATE);
            csrBtMcapDisconnectAllMdlEntries(pInst, currentMclPtr);
        }
    }
    else
    {
        csrBtMcapDisconnectMclCfmSend(msg->phandle,
                                        msg->bd_addr,
                                        0,
                                        CSR_BT_RESULT_CODE_MCAP_INVALID_PARAMETER_VALUE,
                                        CSR_BT_SUPPLIER_MCAP);
    }
}

void CsrBtMcapInvalidDisconnectMclReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapDisconnectMclReq    *msg;

    msg = (CsrBtMcapDisconnectMclReq *)pInst->recvMsgP;

    csrBtMcapDisconnectMclCfmSend(msg->phandle,
                                    msg->bd_addr,
                                    0,
                                    CSR_BT_RESULT_CODE_MCAP_INVALID_OPERATION,
                                    CSR_BT_SUPPLIER_MCAP);
}

void CsrBtMcapConnectMdlReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapConnectMdlReq   *msg;
    McapMclListEntry    *currentMclPtr = NULL;
    McapMdlListEntry    *currentMdlPtr = NULL;

    msg = (CsrBtMcapConnectMdlReq *)pInst->recvMsgP;

    if (pInst->mclList.first)
    {
        currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
        if (currentMclPtr == NULL)
        {
            currentMclPtr = MCAP_MCL_FIND_FROM_REMOTE_CONTROL_PSM(pInst->mclList, &(msg->remotePsm));
        }
    }

    if (currentMclPtr != NULL)
    {
        if (!csrBtMcapCheckMdlIdCreate(msg->mdlId, FALSE))
        {
            csrBtMcapConnectMdlCfmSend(currentMclPtr->phandle,
                                       currentMclPtr->mclId,
                                       msg->mdlId,
                                       0,
                                       0,
                                       0,
                                       0,
                                       CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_MDL,
                                       CSR_BT_SUPPLIER_MCAP_PROTOCOL);
            currentMclPtr->awaitingCfm = FALSE;
        }
        else
        {
            CsrUint8 *reqPtr;

            currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(currentMclPtr->mdlList, &(msg->mdlId));

            if (currentMdlPtr != NULL) /* If the MDL ID exists, it must be replaced */
            {
                if (currentMdlPtr->mdlState == MCAP_MDL_ACTIVE_STATE)
                {
                    CsrBtCml2caDisconnectReqSend(currentMdlPtr->btConnId);
                }
                MCAP_MDL_REMOVE_ENTRY(currentMclPtr->mdlList, currentMdlPtr);
                currentMdlPtr = NULL;
            }
            currentMdlPtr = MCAP_MDL_ADD_ENTRY(currentMclPtr->mdlList);
            CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_CREATING_STATE);
            currentMdlPtr->phandle = currentMclPtr->phandle;
            currentMdlPtr->mdlId = msg->mdlId;
            currentMdlPtr->mdepId = msg->mdepId;
            currentMdlPtr->configuration = msg->configuration;

            currentMclPtr->localDataPsm = msg->localPsm;

            currentMclPtr->remoteDataPsm = msg->remotePsm;

            currentMdlPtr->secLevel = msg->secLevel;
            currentMdlPtr->localMtu = msg->mtu;
            CsrMemCpy(currentMdlPtr->flow, msg->flow, sizeof(L2CA_FLOW_T));
            CsrPmemFree(msg->flow);

            if (currentMclPtr->mclRole == MCAP_CLIENT ||
                currentMclPtr->awaitingRes == FALSE)
            {
                reqPtr = buildMdCreateMdlReq(currentMdlPtr);
                CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_CREATE_MDL_REQ_LEN, reqPtr);
                currentMdlPtr->createRole = MCAP_REQUESTER;
                CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_CREATING_STATE);
                currentMclPtr->awaitingCfm = TRUE;
                currentMclPtr->awaitingRes = FALSE;
                currentMclPtr->reqOpCode = MD_CREATE_MDL_REQ;
                currentMclPtr->reqMdlId = currentMdlPtr->mdlId;
            }
            else
            {
                csrBtMcapConnectMdlCfmSend(currentMclPtr->phandle,
                                            currentMclPtr->mclId,
                                            msg->mdlId,
                                            0,
                                            0,
                                            0,
                                            0,
                                            CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                            CSR_BT_SUPPLIER_MCAP_PROTOCOL);
                currentMclPtr->awaitingCfm = FALSE;
            }
        }
    }
    else
    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrStateEventException("MCAP (CsrBtMcapConnectMdlReqHandler - no MCL found)",
                                CSR_BT_CM_PRIM,
                               msg->type,
                               0);
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
        CsrPmemFree(msg);
    }
}

void CsrBtMcapInvalidConnectMdlReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapConnectMdlReq   *msg;
    McapMclListEntry    *currentMclPtr = NULL;

    msg = (CsrBtMcapConnectMdlReq *)pInst->recvMsgP;

    if (pInst->mclList.first)
    {
        currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
        if (currentMclPtr == NULL)
        {
            currentMclPtr = MCAP_MCL_FIND_FROM_REMOTE_CONTROL_PSM(pInst->mclList, &(msg->remotePsm));
        }
    }

    if (currentMclPtr != NULL)
    {
        csrBtMcapConnectMdlCfmSend(currentMclPtr->phandle,
                                    msg->mclId,
                                    msg->mdlId,
                                    0,
                                    0,
                                    0,
                                    0,
                                    CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                    CSR_BT_SUPPLIER_MCAP_PROTOCOL);
        currentMclPtr->awaitingCfm = FALSE;
    }
}

void CsrBtMcapCreateMdlResHandler(McapInstanceData *pInst)
{
    CsrBtMcapCreateMdlRes    *msg;
    McapMclListEntry    *currentMclPtr = NULL;

    msg = (CsrBtMcapCreateMdlRes *)pInst->recvMsgP;

    if (pInst->mclList.first)
    {
        currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
    }

    if (currentMclPtr == NULL) /* if MCL is unknown */
    {
        /* Ignore - we can not do anything */
        CsrPmemFree(msg->flow);
    }
    else if (currentMclPtr->awaitingRes == FALSE &&
        currentMclPtr->awaitingCfm == TRUE &&
        currentMclPtr->mclRole == MCAP_CLIENT)
    {
        /* Ignore this response. Another request has priority now */
        CsrPmemFree(msg->flow);
    }
    else
    {
        McapMdlListEntry    *currentMdlPtr = NULL;
        CsrUint8             *resPtr;
        currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(currentMclPtr->mdlList, &(msg->mdlId));

        if (currentMdlPtr != NULL) /* If the MDL ID exists, it must be replaced */
        {
            if (currentMdlPtr->mdlState == MCAP_MDL_ACTIVE_STATE)
            {
                CsrBtCml2caDisconnectReqSend(currentMdlPtr->btConnId);
            }
            MCAP_MDL_REMOVE_ENTRY(currentMclPtr->mdlList, currentMdlPtr);
            currentMdlPtr = NULL;
        }
        currentMdlPtr = MCAP_MDL_ADD_ENTRY(currentMclPtr->mdlList);
        currentMdlPtr->phandle = currentMclPtr->phandle;
        currentMdlPtr->mdlId = msg->mdlId;

        currentMclPtr->localDataPsm = msg->psm; /* Is this the right psm !!!*/
        
        currentMdlPtr->secLevel = msg->secLevel;
        currentMdlPtr->localMtu = msg->mtu;
        currentMdlPtr->configuration = msg->configResponse;
        CsrMemCpy(currentMdlPtr->flow, msg->flow, sizeof(L2CA_FLOW_T));

        csrBtMcapSetGlobalSniffTimeout(pInst);

        if (msg->mcapResponse == CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS)
        {
            CsrBtCml2caFecConnectAcceptReqSendExt( CSR_BT_MCAP_IFACEQUEUE,
                                                    currentMclPtr->localDataPsm,
                                                    currentMclPtr->classOfDevice,
                                                    currentMdlPtr->secLevel,
                                                    currentMdlPtr->localMtu,
                                                    L2CA_FLUSH_TO_DEFAULT,
                                                    NULL,                /* L2CA_QOS_T serviceQuality */
                                                    msg->flow,
                                                    FALSE,              /* CsrBool fallbackBasicMode */
                                                    currentMclPtr->uuid);
            CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_CONNECTING_STATE);
        }
        else
        {
            CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_CONNECTED_STATE);
            CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_NULL_STATE);
            CsrPmemFree(msg->flow);
        }

        if (currentMclPtr->awaitingRes == TRUE)
        {
            resPtr = buildMdCreateMdlRes(msg->mcapResponse, msg->mdlId, msg->configResponse);
            CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_CREATE_MDL_RSP_LEN, resPtr);
            currentMclPtr->awaitingRes = FALSE;
        }
    }
}

void CsrBtMcapInvalidCreateMdlResHandler(McapInstanceData *pInst)
{
    CsrBtMcapFreeUpstreamMessageContents(pInst->recvMsgP);
    CsrBtMcapPrivateFreeUpstreamMessageContents(CSR_BT_MCAP_PRIVATE_PRIM,pInst->recvMsgP);

}

void CsrBtMcapDataSendReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapDataSendReq     *msg;
    McapMclListEntry    *currentMclPtr = NULL;
    McapMdlListEntry    *currentMdlPtr = NULL;

    msg = (CsrBtMcapDataSendReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
    if (currentMclPtr != NULL)
    {
        currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(currentMclPtr->mdlList, &(msg->mdlId));
    }
    if (currentMdlPtr != NULL)
    {
        csrBtMcapModeChangeReq(pInst, CSR_BT_ACTIVE_MODE);

        CsrBtCml2caDataReqSend(currentMdlPtr->btConnId, msg->length, msg->payload);
    }
}

void CsrBtMcapInvalidDataSendReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapDataSendReq     *msg;
    McapMclListEntry    *currentMclPtr = NULL;

    msg = (CsrBtMcapDataSendReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
    if (currentMclPtr != NULL)
    {
        csrBtMcapDataSendCfmSend(currentMclPtr->phandle,
                                currentMclPtr->mclId,
                                msg->mdlId,
                                CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                CSR_BT_SUPPLIER_MCAP_PROTOCOL);
    }
}

void CsrBtMcapDeleteMdlReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapDeleteMdlReq    *msg;
    McapMclListEntry    *currentMclPtr = NULL;
    CsrUint8             *reqPtr;

    msg = (CsrBtMcapDeleteMdlReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));

    if (currentMclPtr)
    {
        if (msg->mdlId != 0xFFFF)
        {
            McapMdlListEntry    *currentMdlPtr = NULL;

            currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(currentMclPtr->mdlList, &(msg->mdlId));
            if (currentMdlPtr == NULL)
            {
                csrBtMcapDeleteMdlCfmSend(currentMclPtr->phandle,
                                        msg->mclId,
                                        msg->mdlId,
                                        CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_MDL);
                currentMclPtr->deletingMdlId = 0;
                currentMclPtr->awaitingCfm = FALSE;
            }
            else
            {
                if (currentMdlPtr->mdlState == MCAP_MDL_CONNECTING_STATE ||
                    currentMdlPtr->mdlState == MCAP_MDL_ACTIVE_STATE)
                {
                    currentMclPtr->deleteRole = MCAP_REQUESTER;
                    CsrBtCml2caDisconnectReqSend(currentMdlPtr->btConnId);
                    CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_DELETING_STATE);
                }
                else if (currentMclPtr->mclRole == MCAP_CLIENT ||
                         currentMclPtr->awaitingRes == FALSE)
                {
                    reqPtr = buildMdDeleteMdlReq(msg->mdlId);
                    CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_DELETE_MDL_REQ_LEN, reqPtr);
                    currentMclPtr->awaitingCfm = TRUE;
                    currentMclPtr->awaitingRes = FALSE;
                    currentMclPtr->reqOpCode = MD_DELETE_MDL_REQ;
                    currentMclPtr->reqMdlId = currentMdlPtr->mdlId;
                }
                else
                {
                    csrBtMcapDeleteMdlCfmSend(currentMclPtr->phandle,
                                              msg->mclId,
                                              msg->mdlId,
                                              CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION);
                    currentMclPtr->deletingMdlId = 0;
                    currentMclPtr->awaitingCfm = FALSE;
                }
            }
        }
        else
        {
            currentMclPtr->deleteRole = MCAP_REQUESTER;
            csrBtMcapDeleteAllMdlEntries(pInst, currentMclPtr);
        }
    }
}

void CsrBtMcapInvalidDeleteMdlReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapDeleteMdlReq    *msg;
    McapMclListEntry    *currentMclPtr = NULL;

    msg = (CsrBtMcapDeleteMdlReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));

    if (currentMclPtr)
    {
        csrBtMcapDeleteMdlCfmSend(currentMclPtr->phandle,
                                    msg->mclId,
                                    msg->mdlId,
                                    CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION);
        currentMclPtr->deletingMdlId = 0;
        currentMclPtr->awaitingCfm = FALSE;
    }
}

void CsrBtMcapDeleteMdlResHandler(McapInstanceData *pInst)
{
    CsrBtMcapDeleteMdlRes    *msg;
    McapMclListEntry    *currentMclPtr = NULL;
    McapMdlListEntry    *currentMdlPtr = NULL;
    CsrUint8             *reqPtr;

    msg = (CsrBtMcapDeleteMdlRes *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
    
    if (currentMclPtr)
    {
        currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(currentMclPtr->mdlList, &(msg->mdlId));

        if (msg->mcapResponse == CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS)
        {
            currentMclPtr->deleteRole = MCAP_RESPONDER;
            if (msg->mdlId == 0xFFFF)
            {

                csrBtMcapDeleteAllMdlEntries(pInst, currentMclPtr);
            }
            else if (currentMdlPtr)
            {
                if (currentMdlPtr->mdlState == MCAP_MDL_NULL_STATE)
                {
                    if (currentMclPtr->awaitingRes == TRUE)
                    {
                        MCAP_MDL_REMOVE_ENTRY(currentMclPtr->mdlList, currentMdlPtr);
                        if (currentMclPtr->mdlList.count == 0)
                        {
                            CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_CONNECTED_STATE);
                        }
                        reqPtr = buildMdDeleteMdlRes(msg->mcapResponse, msg->mdlId);
                        CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_DELETE_MDL_RSP_LEN, reqPtr);
                        currentMclPtr->awaitingRes = FALSE;
                        csrBtMcapSetGlobalSniffTimeout(pInst);
                    }
                }
                else
                {
                    CsrBtCml2caDisconnectReqSend(currentMdlPtr->btConnId);
                    CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_DELETING_STATE);
                }
            }
        }
        else
        {
            if (currentMclPtr->awaitingRes == TRUE)
            {
                reqPtr = buildMdDeleteMdlRes(msg->mcapResponse, msg->mdlId);
                CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_DELETE_MDL_RSP_LEN, reqPtr);
                currentMclPtr->awaitingRes = FALSE;
            }
        }
    }
}

void CsrBtMcapInvalidDeleteMdlResHandler(McapInstanceData *pInst)
{
    CsrBtMcapFreeUpstreamMessageContents(pInst->recvMsgP);
    CsrBtMcapPrivateFreeUpstreamMessageContents(CSR_BT_MCAP_PRIVATE_PRIM,pInst->recvMsgP);
}

void CsrBtMcapAbortMdlResHandler(McapInstanceData *pInst)
{
    CsrBtMcapAbortMdlRes     *msg;
    McapMclListEntry    *currentMclPtr = NULL;
    McapMdlListEntry    *currentMdlPtr = NULL;
    CsrUint8             *reqPtr;

    msg = (CsrBtMcapAbortMdlRes *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
    if (currentMclPtr != NULL)
    {
        currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(currentMclPtr->mdlList, &(msg->mdlId));
    
        if (currentMdlPtr == NULL)
        {
            if (currentMclPtr->awaitingRes == TRUE)
            {
                reqPtr = buildMdAbortMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_MDL, msg->mdlId);
                CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_ABORT_MDL_RSP_LEN, reqPtr);
                currentMclPtr->awaitingRes = FALSE;
            }
        }
        else if (currentMclPtr->awaitingRes == TRUE)
        {
            if (msg->mcapResponse == CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS)
            {
                CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_MCAP_IFACEQUEUE, currentMclPtr->localDataPsm);
                CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_ABORTING_STATE);
            }
            else
            {
                CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_CONNECTING_STATE);
            }
            reqPtr = buildMdAbortMdlRes(msg->mcapResponse, msg->mdlId);
            CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_ABORT_MDL_RSP_LEN, reqPtr);
            currentMclPtr->awaitingRes = FALSE;
        }
        else
        {
            /* We are not allowed to send a response in this case */
        }
    }
   else
    {
        /* We are not allowed to send a response in this case */
    }
}

void CsrBtMcapInvalidAbortMdlResHandler(McapInstanceData *pInst)
{
    CsrBtMcapFreeUpstreamMessageContents(pInst->recvMsgP);
    CsrBtMcapPrivateFreeUpstreamMessageContents(CSR_BT_MCAP_PRIVATE_PRIM,pInst->recvMsgP);
}

void CsrBtMcapDisconnectMdlReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapDisconnectMdlReq    *msg;
    McapMclListEntry        *currentMclPtr = NULL;
    McapMdlListEntry        *currentMdlPtr = NULL;

    msg = (CsrBtMcapDisconnectMdlReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
    if (currentMclPtr)
    {
        currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(currentMclPtr->mdlList, &(msg->mdlId));
    }

    if (currentMclPtr == NULL || currentMdlPtr == NULL)
    {
        /* No nothing - just ignore */
    }
    else
    {
        currentMdlPtr->disconnectRole = MCAP_REQUESTER;
        if (currentMdlPtr->mdlState == MCAP_MDL_CONNECTING_STATE)
        {
            if (currentMclPtr->mclRole == MCAP_CLIENT ||
                currentMclPtr->awaitingRes == FALSE)
            {
                CsrUint8             *reqPtr;

                reqPtr = buildMdAbortMdlReq(currentMdlPtr->mdlId);
                CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_ABORT_MDL_REQ_LEN, reqPtr);
                CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_ABORTING_STATE);
                currentMclPtr->awaitingCfm = TRUE;
                currentMclPtr->awaitingRes = FALSE;
                currentMclPtr->reqOpCode = MD_ABORT_MDL_REQ;
                currentMclPtr->reqMdlId = currentMdlPtr->mdlId;
            }
            else
            {
                csrBtMcapDisconnectMdlCfmSend(currentMdlPtr->phandle,
                                            currentMclPtr->mclId,
                                            currentMdlPtr->mdlId,
                                            CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                            CSR_BT_SUPPLIER_MCAP_PROTOCOL);
                currentMclPtr->awaitingCfm = FALSE;
            }
        }
        else if (currentMdlPtr->mdlState == MCAP_MDL_CREATING_STATE)
        {
            CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_DISCONNECTING_STATE);
        }
        else
        {
            CsrBtCml2caDisconnectReqSend(currentMdlPtr->btConnId);
            CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_DISCONNECTING_STATE);
        }
    }
}

void CsrBtMcapInvalidDisconnectMdlReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapDisconnectMdlReq    *msg;
    McapMclListEntry        *currentMclPtr = NULL;
    McapMdlListEntry        *currentMdlPtr = NULL;

    msg = (CsrBtMcapDisconnectMdlReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
    if (currentMclPtr)
    {
        currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(currentMclPtr->mdlList, &(msg->mdlId));
        if (currentMdlPtr)
        {
            csrBtMcapDisconnectMdlCfmSend(currentMdlPtr->phandle,
                                        currentMclPtr->mclId,
                                        currentMdlPtr->mdlId,
                                        CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                        CSR_BT_SUPPLIER_MCAP_PROTOCOL);
        }
        currentMclPtr->awaitingCfm = FALSE;
    }
}

void CsrBtMcapReconnectMdlReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapReconnectMdlReq *msg;
    McapMclListEntry    *currentMclPtr = NULL;
    McapMdlListEntry    *currentMdlPtr = NULL;
    CsrUint8             *reqPtr;

    msg = (CsrBtMcapReconnectMdlReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
    if (currentMclPtr != NULL)
    {
        currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(currentMclPtr->mdlList, &(msg->mdlId));

        if (currentMdlPtr != NULL)
        {
            if (currentMclPtr->mclRole == MCAP_CLIENT ||
                currentMclPtr->awaitingRes == FALSE)
            {
                reqPtr = buildMdReconnectMdlReq(currentMdlPtr);
                CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_RECONNECT_MDL_REQ_LEN, reqPtr);
                CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_RECONNECTING_STATE);
                currentMdlPtr->reconnectRole = MCAP_REQUESTER;
                currentMclPtr->localDataPsm = msg->localPsm;
                currentMclPtr->remoteDataPsm = msg->remotePsm;
                currentMclPtr->awaitingCfm = TRUE;
                currentMclPtr->awaitingRes = FALSE;
                currentMclPtr->reqOpCode = MD_RECONNECT_MDL_REQ;
                currentMclPtr->reqMdlId = currentMdlPtr->mdlId;
            }
            else
            {
                csrBtMcapReconnectMdlCfmSend(currentMclPtr->phandle,
                                             msg->mclId, 
                                             msg->mdlId,
                                             CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                             CSR_BT_SUPPLIER_MCAP_PROTOCOL);
                currentMclPtr->awaitingCfm = FALSE;
            }
        }
        else
        {
            csrBtMcapReconnectMdlCfmSend(currentMclPtr->phandle,
                                         msg->mclId,
                                         msg->mdlId,
                                         CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_MDL,
                                         CSR_BT_SUPPLIER_MCAP_PROTOCOL);
            currentMclPtr->awaitingCfm = FALSE;
        }
    }
    /* else do nothing: we don't have the appHandle to answer to.... 
       ... And this should never happen!! */
}

void CsrBtMcapInvalidReconnectMdlReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapReconnectMdlReq *msg;
    McapMclListEntry    *currentMclPtr = NULL;
    McapMdlListEntry    *currentMdlPtr = NULL;

    msg = (CsrBtMcapReconnectMdlReq *)pInst->recvMsgP;

    if (pInst->mclList.first)
    {
        currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
        if (currentMclPtr != NULL)
        {
            currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(currentMclPtr->mdlList, &(msg->mdlId));

            csrBtMcapReconnectMdlCfmSend(currentMclPtr->phandle,
                                     msg->mclId,
                                     msg->mdlId,
                                     CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                     CSR_BT_SUPPLIER_MCAP_PROTOCOL);
            if (currentMdlPtr != NULL)
            {
                currentMdlPtr->reconnectRole = MCAP_RESPONDER;
            }
        }
    }
}


void CsrBtMcapReconnectMdlResHandler(McapInstanceData *pInst)
{
    CsrBtMcapReconnectMdlRes *msg;
    McapMclListEntry    *currentMclPtr = NULL;
    McapMdlListEntry    *currentMdlPtr = NULL;

    msg = (CsrBtMcapReconnectMdlRes *)pInst->recvMsgP;

    if (pInst->mclList.first)
    {
        currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
        if (currentMclPtr)
        {
            currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(currentMclPtr->mdlList, &(msg->mdlId));
        }
    }

    if (currentMclPtr == NULL || currentMdlPtr == NULL)
    {
        /* Ignore - we can not do anything */
    }
    else if (currentMclPtr->awaitingRes == TRUE)
    {
        CsrUint8             *resPtr;

        currentMdlPtr->phandle = currentMclPtr->phandle ;
        currentMdlPtr->mdlId = msg->mdlId;
        currentMclPtr->localDataPsm = msg->psm;
        currentMdlPtr->secLevel = msg->secLevel;
        currentMdlPtr->localMtu = msg->mtu;
        if (msg->flow != NULL)
        {
            CsrMemCpy(currentMdlPtr->flow, msg->flow, sizeof(L2CA_FLOW_T));
        }

        if (msg->mcapResponse == CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS)
        {
            CsrBtCml2caFecConnectAcceptReqSendExt( CSR_BT_MCAP_IFACEQUEUE,
                                                    currentMclPtr->localDataPsm,
                                                    currentMclPtr->classOfDevice,
                                                    currentMdlPtr->secLevel,
                                                    currentMdlPtr->localMtu,
                                                    L2CA_FLUSH_TO_DEFAULT,
                                                    NULL,                /* L2CA_QOS_T serviceQuality */
                                                    msg->flow,
                                                    FALSE,              /* CsrBool fallbackBasicMode */
                                                    currentMclPtr->uuid);
            CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_RECONNECTING_STATE);
        }
        else
        {
            if (currentMclPtr->mdlList.count == 0)
            {
                CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_CONNECTED_STATE);
            }
            CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_NULL_STATE);
            CsrPmemFree(msg->flow);
        }
        resPtr = buildMdReconnectMdlRes(msg->mcapResponse, msg->mdlId);
        CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_RECONNECT_MDL_RSP_LEN, resPtr);
        currentMclPtr->awaitingRes = FALSE;
    }
    else
    {
        /* We are not allowed to send any response in this case */
    }
}

void CsrBtMcapInvalidReconnectMdlResHandler(McapInstanceData *pInst)
{
    CsrBtMcapFreeUpstreamMessageContents(pInst->recvMsgP);
    CsrBtMcapPrivateFreeUpstreamMessageContents(CSR_BT_MCAP_PRIVATE_PRIM,pInst->recvMsgP);
}

void CsrBtMcapSyncCapReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapSyncCapReq      *msg;
    McapMclListEntry    *currentMclPtr = NULL;
    CsrUint8             *reqPtr;

    msg = (CsrBtMcapSyncCapReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
    if (currentMclPtr == NULL)
    {
        /* Do nothing - just ignore */
    }
    else
    {
        reqPtr = buildMdSyncCapReq(msg->timeStampRequiredAccuracy);
        CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_SYNC_CAP_REQ_LEN, reqPtr);
        currentMclPtr->awaitingSyncRes = TRUE;
    }
}

void CsrBtMcapInvalidSyncCapReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapSyncCapReq      *msg;
    McapMclListEntry    *currentMclPtr = NULL;

    msg = (CsrBtMcapSyncCapReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));

    if (currentMclPtr != NULL)
    {
        csrBtMcapSyncCapCfmSend(currentMclPtr->phandle,
                                currentMclPtr->mclId,
                                CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                0,
                                0,
                                0,
                                0);
    }
}

void CsrBtMcapSyncCapResHandler(McapInstanceData *pInst)
{
    CsrBtMcapSyncCapRes      *msg;
    McapMclListEntry    *currentMclPtr = NULL;

    msg = (CsrBtMcapSyncCapRes *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));

    if (currentMclPtr)
    {
        CsrUint8  *reqPtr;
        if(msg->mcapResponse == CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS)
        {
            reqPtr = buildMdSyncCapRes(msg->mcapResponse,
                                        msg->btClkAccessRes,
                                        msg->syncLeadTime,
                                        msg->timeStampNativeRes,
                                        msg->timeStampNativeAccuracy);
        }
        else
        {
            reqPtr = buildMdSyncCapRes(msg->mcapResponse,
                                        0x00,
                                        0x00,
                                        0x00,
                                        0x00);
        }
        CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_SYNC_CAP_RSP_LEN, reqPtr);
    }
}

void CsrBtMcapInvalidSyncCapResHandler(McapInstanceData *pInst)
{
    /* Do nothing - just ignore */
}

void CsrBtMcapSyncSetReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapSyncSetReq      *msg;
    McapMclListEntry    *currentMclPtr = NULL;
    CsrUint8             *reqPtr;

    msg = (CsrBtMcapSyncSetReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));

    if (currentMclPtr == NULL)
    {
        /* Do nothing - just ignore */
    }
    else
    {
        reqPtr = buildMdSyncSetReq(msg->timeStampUpdateInfo,
                                    msg->btClkSyncTime,
                                    msg->timeStampSyncTimeLsb,
                                    msg->timeStampSyncTimeMsb);
        CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_SYNC_SET_REQ_LEN, reqPtr);
        currentMclPtr->awaitingSyncRes = TRUE;
    }
}

void CsrBtMcapInvalidSyncSetReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapSyncSetReq      *msg;
    McapMclListEntry    *currentMclPtr = NULL;

    msg = (CsrBtMcapSyncSetReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));

    if (currentMclPtr != NULL)
    {
        csrBtMcapSyncSetCfmSend(currentMclPtr->phandle,
                                currentMclPtr->mclId,
                                CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                0,
                                0,
                                0,
                                0);
    }
}

void CsrBtMcapSyncSetResHandler(McapInstanceData *pInst)
{
    CsrBtMcapSyncSetRes      *msg;
    McapMclListEntry    *currentMclPtr = NULL;

    msg = (CsrBtMcapSyncSetRes *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));

    if (currentMclPtr)
    {
        CsrUint8             *reqPtr;

        if(msg->mcapResponse == CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS)
        {
            reqPtr = buildMdSyncSetRes(msg->mcapResponse,
                                        msg->btClkSyncTime,
                                        msg->timeStampSyncTimeLsb,
                                        msg->timeStampSyncTimeMsb,
                                        msg->timeStampSampleAccuracy);
        }
        else
        {
            reqPtr = buildMdSyncSetRes(msg->mcapResponse,
                                        0x00000000,
                                        0x00000000,
                                        0x00000000,
                                        0x0000);
        }
        CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_SYNC_SET_RSP_LEN, reqPtr);
    }
}

void CsrBtMcapInvalidSyncSetResHandler(McapInstanceData *pInst)
{
    /* Do nothing - just ignore */
}

void CsrBtMcapSyncInfoReqHandler(McapInstanceData *pInst)
{
    CsrBtMcapSyncInfoReq     *msg;
    McapMclListEntry    *currentMclPtr = NULL;

    msg = (CsrBtMcapSyncInfoReq *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));

    if (currentMclPtr)
    {
        CsrUint8  *reqPtr;
        reqPtr = buildMdSyncInfoInd(msg->btClkSyncTime,
                                    msg->timeStampSyncTimeLsb,
                                    msg->timeStampSyncTimeMsb,
                                    msg->timeStampSampleAccuracy);
        CsrBtCml2caDataReqSend(currentMclPtr->btConnId, MD_SYNC_INFO_IND_LEN, reqPtr);
    }
}

void CsrBtMcapInvalidSyncInfoReqHandler(McapInstanceData *pInst)
{
    /* Do nothing - just ignore */
}


/********************************************************/
/*                                                      */
/*    CM primitive handlers                             */
/*                                                      */
/********************************************************/

static void csrBtMcapMdlDisconnectHandler(McapInstanceData *pInst, CsrBtCmL2caDisconnectInd *msg, MclMdlPtr *commonPtr)
{
    CsrUint8 *reqPtr;

    if (CsrBtMcapIsDisconnectAbnormal (msg->reasonCode, msg->reasonSupplier) &&
        commonPtr->mdlPtr->mdlState != MCAP_MDL_ABORTING_STATE)
    {
        if (commonPtr->mdlPtr->mdlId == 0xFFFF) /* We are in the process of deleting all MDLs */
        {
            csrBtMcapDisconnectMdlIndSend(commonPtr->mdlPtr->phandle,
                                          commonPtr->mclPtr->mclId,
                                          commonPtr->mclPtr->deletingMdlId,
                                          msg->reasonCode,
                                          msg->reasonSupplier);
            commonPtr->mclPtr->deletingMdlId = 0;
            MCAP_MDL_REMOVE_ENTRY(commonPtr->mclPtr->mdlList, commonPtr->mdlPtr);
            csrBtMcapSetGlobalSniffTimeout(pInst);
            csrBtMcapDeleteAllMdlEntries(pInst, commonPtr->mclPtr);
        }
        else
        {
            csrBtMcapDisconnectMdlIndSend(commonPtr->mdlPtr->phandle,
                                          commonPtr->mclPtr->mclId,
                                          commonPtr->mdlPtr->mdlId,
                                          msg->reasonCode,
                                          msg->reasonSupplier);
            if (commonPtr->mdlPtr->mdlState == MCAP_MDL_DELETING_STATE)
            {
                csrBtMcapDeleteMdlCfmSend(commonPtr->mclPtr->phandle,
                                          commonPtr->mclPtr->mclId,
                                          commonPtr->mdlPtr->mdlId,
                                          CSR_BT_RESULT_CODE_MCAP_PROTOCOL_UNSPECIFIED_ERROR);
                commonPtr->mclPtr->deletingMdlId = 0;
                commonPtr->mclPtr->awaitingCfm = FALSE;
            }
            if (commonPtr->mclPtr->mclState == MCAP_MCL_DEACTIVATING_STATE)
            {
                McapMdlListEntry    *mdlPtr  = NULL;
                McapPsmListEntry    *currentPsmPtr  = NULL;
                CsrUint16            mdlCount = 0;
                CsrBool              activeMdlsFound = FALSE;

                CSR_BT_MCAP_STATE_CHANGE(commonPtr->mdlPtr->mdlState, MCAP_MDL_NULL_STATE);
                mdlPtr = (McapMdlListEntry *) (commonPtr->mclPtr->mdlList.first);
                for (mdlCount = 0; mdlCount < commonPtr->mclPtr->mdlList.count; mdlCount++)
                {
                    if (mdlPtr->mdlState != MCAP_MDL_NULL_STATE)
                    {
                        activeMdlsFound = TRUE;
                    }
                    mdlPtr = mdlPtr->next;
                }
                if (!activeMdlsFound)
                {
                    csrBtMcapDeactivateCfmSend(commonPtr->mclPtr->phandle,
                                                commonPtr->mclPtr->localControlPsm,
                                                CSR_BT_RESULT_CODE_MCAP_SUCCESS,
                                                CSR_BT_SUPPLIER_MCAP);
                    CSR_BT_MCAP_STATE_CHANGE(commonPtr->mclPtr->mclState, MCAP_MCL_NULL_STATE);
                    currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(commonPtr->mclPtr->localControlPsm));
                    if (currentPsmPtr != NULL)
                    {
                        MCAP_PSM_REMOVE_ENTRY(pInst->psmList, currentPsmPtr);
                    }
                    MCAP_MCL_REMOVE_ENTRY(pInst->mclList, commonPtr->mclPtr);
                }
            }
            else
            {
                CSR_BT_MCAP_STATE_CHANGE(commonPtr->mdlPtr->mdlState, MCAP_MDL_NULL_STATE);
            }
        }
    }
    else if (commonPtr->mdlPtr->mdlState == MCAP_MDL_ABORTING_STATE)
    {
        /* Do nothing - just ignore */
    }
    else if (commonPtr->mclPtr->mclState == MCAP_MCL_DISCONNECTING_STATE)
    {
        csrBtMcapDisconnectMdlIndSend(commonPtr->mdlPtr->phandle,
                                        commonPtr->mclPtr->mclId,
                                        commonPtr->mdlPtr->mdlId,
                                        msg->reasonCode,
                                        msg->reasonSupplier);
        CSR_BT_MCAP_STATE_CHANGE(commonPtr->mdlPtr->mdlState, MCAP_MDL_NULL_STATE);
        csrBtMcapDisconnectAllMdlEntries(pInst, commonPtr->mclPtr);
    }
    else if (commonPtr->mclPtr->mclState == MCAP_MCL_DEACTIVATING_STATE)
    {
        CsrBool loop = TRUE;

        csrBtMcapDisconnectMdlIndSend(commonPtr->mdlPtr->phandle,
                                        commonPtr->mclPtr->mclId,
                                        commonPtr->mdlPtr->mdlId,
                                        msg->reasonCode,
                                        msg->reasonSupplier);
        if (commonPtr->mdlPtr->mdlState == MCAP_MDL_DELETING_STATE &&
            commonPtr->mclPtr->awaitingRes == TRUE)
        {
            CsrUint8 *resPtr;

            resPtr = buildMdDeleteMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS, commonPtr->mdlPtr->mdlId);
            CsrBtCml2caDataReqSend(commonPtr->mclPtr->btConnId, MD_DELETE_MDL_RSP_LEN, resPtr);
            commonPtr->mclPtr->awaitingRes = FALSE;
        }
        MCAP_MDL_REMOVE_ENTRY(commonPtr->mclPtr->mdlList, commonPtr->mdlPtr);
        commonPtr->mdlPtr = (McapMdlListEntry *)(commonPtr->mclPtr->mdlList.first);
        while (loop && commonPtr->mdlPtr != NULL)
        {
            if (commonPtr->mdlPtr->mdlState == MCAP_MDL_ACTIVE_STATE)
            {
                CsrBtCml2caDisconnectReqSend(commonPtr->mdlPtr->btConnId);
                loop = FALSE;
            }
            else
            {
                commonPtr->mdlPtr = commonPtr->mdlPtr->next;
            }
        }
        if (commonPtr->mdlPtr == NULL && commonPtr->mclPtr->mclSubState != MCAP_MCL_DISCONNECTING_STATE)  /* No more active MDLs on this MCL */
        {
            CsrBtCml2caDisconnectReqSend(commonPtr->mclPtr->btConnId);
            CSR_BT_MCAP_STATE_CHANGE(commonPtr->mclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
        }
    }
    else if (msg->localTerminated)
    {
        if (commonPtr->mdlPtr->mdlId == 0xFFFF) /* We are in the process of deleting all MDLs */
        {
            csrBtMcapDisconnectMdlIndSend(commonPtr->mdlPtr->phandle,
                                          commonPtr->mclPtr->mclId,
                                          commonPtr->mclPtr->deletingMdlId,
                                          msg->reasonCode,
                                          msg->reasonSupplier);
            commonPtr->mclPtr->deletingMdlId = 0;
            MCAP_MDL_REMOVE_ENTRY(commonPtr->mclPtr->mdlList, commonPtr->mdlPtr);
            csrBtMcapSetGlobalSniffTimeout(pInst);
            csrBtMcapDeleteAllMdlEntries(pInst, commonPtr->mclPtr);
        }
        else if (commonPtr->mdlPtr->mdlState == MCAP_MDL_DELETING_STATE) /* We are deleting an MDL */
        {
            csrBtMcapDisconnectMdlIndSend(commonPtr->mdlPtr->phandle,
                                          commonPtr->mclPtr->mclId,
                                          commonPtr->mdlPtr->mdlId,
                                          msg->reasonCode,
                                          msg->reasonSupplier);
            if (commonPtr->mclPtr->deleteRole == MCAP_REQUESTER)
            {
                if (commonPtr->mclPtr->mclRole == MCAP_CLIENT ||
                    commonPtr->mclPtr->awaitingRes == FALSE)
                {
                    if(commonPtr->mclPtr->mclState != MCAP_MCL_NULL_STATE)
                    {
                        reqPtr = buildMdDeleteMdlReq(commonPtr->mdlPtr->mdlId);
                        CsrBtCml2caDataReqSend(commonPtr->mclPtr->btConnId, MD_DELETE_MDL_REQ_LEN, reqPtr);
                        commonPtr->mclPtr->awaitingCfm = TRUE;
                        commonPtr->mclPtr->awaitingRes = FALSE;
                        commonPtr->mclPtr->reqOpCode = MD_DELETE_MDL_REQ;
                        commonPtr->mclPtr->reqMdlId = commonPtr->mdlPtr->mdlId;
                    }
                }
                else 
                {
                    csrBtMcapDeleteMdlCfmSend(commonPtr->mclPtr->phandle,
                                              commonPtr->mclPtr->mclId,
                                              commonPtr->mdlPtr->mdlId,
                                              CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION);
                    CSR_BT_MCAP_STATE_CHANGE(commonPtr->mdlPtr->mdlState, MCAP_MDL_NULL_STATE);
                    commonPtr->mclPtr->awaitingCfm = FALSE;
                }
            }
            else
            {
                if (commonPtr->mclPtr->awaitingRes == TRUE)
                {
                    reqPtr = buildMdDeleteMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS, commonPtr->mdlPtr->mdlId);
                    CsrBtCml2caDataReqSend(commonPtr->mclPtr->btConnId, MD_DELETE_MDL_RSP_LEN, reqPtr);
                    commonPtr->mclPtr->awaitingRes = FALSE;
                    MCAP_MDL_REMOVE_ENTRY(commonPtr->mclPtr->mdlList, commonPtr->mdlPtr);
                }
                if (commonPtr->mclPtr->mdlList.count == 0)
                {
                    CSR_BT_MCAP_STATE_CHANGE(commonPtr->mclPtr->mclState, MCAP_MCL_CONNECTED_STATE);
                }
                csrBtMcapSetGlobalSniffTimeout(pInst);
            }
        }
        else
        {
            csrBtMcapDisconnectMdlCfmSend(commonPtr->mdlPtr->phandle,
                                          commonPtr->mclPtr->mclId,
                                          commonPtr->mdlPtr->mdlId,
                                          msg->reasonCode,
                                          msg->reasonSupplier);
            CSR_BT_MCAP_STATE_CHANGE(commonPtr->mdlPtr->mdlState, MCAP_MDL_NULL_STATE);
            commonPtr->mdlPtr->btConnId = 0;
            commonPtr->mclPtr->awaitingCfm = FALSE;
            if (commonPtr->mclPtr->mdlList.count == 0)
            {
                CSR_BT_MCAP_STATE_CHANGE(commonPtr->mclPtr->mclState, MCAP_MCL_CONNECTED_STATE);
            }
        }
    }
    else /* Peer device disconnected */
    {
        csrBtMcapDisconnectMdlIndSend(commonPtr->mdlPtr->phandle,
                                      commonPtr->mclPtr->mclId,
                                      commonPtr->mdlPtr->mdlId,
                                      msg->reasonCode,
                                      msg->reasonSupplier);
        CSR_BT_MCAP_STATE_CHANGE(commonPtr->mdlPtr->mdlState, MCAP_MDL_NULL_STATE);
        commonPtr->mdlPtr->btConnId = 0;
    }
}


void CsrBtMcapCmL2caConnectAcceptCfmHandler(McapInstanceData *pInst)
{
    CsrBtCmL2caConnectAcceptCfm *msg;
    McapPsmListEntry            *currentPsmPtr = NULL;
    McapMclListEntry            *currentMclPtr = NULL;
    McapMdlListEntry            *currentMdlPtr = NULL;
    CsrUint8                    searchState;
    PsmStatePtr                 *searchData;
    CsrUint32                   i;
    CsrBool                     validCfm = TRUE;

    msg = (CsrBtCmL2caConnectAcceptCfm *)pInst->recvMsgP;

    if (msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && msg->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        currentMclPtr = (McapMclListEntry *)(pInst->mclList.first);
        for (i = 0; i < pInst->mclList.count; i++)
        {
            if (currentMclPtr->localControlPsm == msg->localPsm &&
                currentMclPtr->mclState == MCAP_MCL_CONNECTING_STATE &&
                bd_addr_eq(&(currentMclPtr->bd_addr), &(msg->deviceAddr)))
            {
                CsrBtCml2caCancelConnectReqSend(CSR_BT_MCAP_IFACEQUEUE,
                                                currentMclPtr->bd_addr,
                                                msg->localPsm);
            }
            else if (currentMclPtr->localControlPsm == msg->localPsm &&
                currentMclPtr->mclState != MCAP_MCL_NULL_STATE &&
                bd_addr_eq(&(currentMclPtr->bd_addr), &(msg->deviceAddr)))
            {
                CsrBtCml2caDisconnectReqSend(msg->btConnId);
                currentMclPtr->mclId = (CsrUint16)msg->btConnId;
                currentMclPtr->btConnId = msg->btConnId;
                validCfm = FALSE;
            }
            currentMclPtr = currentMclPtr->next;
        }
        if (validCfm)
        {
            searchData = (PsmStatePtr *)CsrPmemAlloc(sizeof(PsmStatePtr));
            searchData->psm = msg->localPsm;
            currentMclPtr = NULL;
            searchData->state = MCAP_MCL_NULL_STATE;
            currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_CONTROL_PSM_AND_MCL_STATE(pInst->mclList, (void *) searchData);
            CsrPmemFree(searchData);

            if(currentMclPtr != NULL)
            {
                CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, msg->deviceAddr, msg->btConnId);

                currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->localPsm));
                if (currentPsmPtr != NULL)
                {
                    currentPsmPtr->currentMcls++;
                    if (currentPsmPtr->allowedMcls > currentPsmPtr->currentMcls &&
                        !bd_addr_eq(&(currentMclPtr->bd_addr), &(msg->deviceAddr)))
                    {
                        McapMclListEntry *newMclPtr = NULL;
                        L2CA_FLOW_T  *flow = CsrPmemAlloc(sizeof(L2CA_FLOW_T));

                        newMclPtr = MCAP_MCL_ADD_ENTRY(pInst->mclList);
                        newMclPtr->sniffTimeout = currentMclPtr->sniffTimeout;
                        newMclPtr->localControlPsm = msg->localPsm; /* MCL PSM overwrite */
                        newMclPtr->classOfDevice = currentMclPtr->classOfDevice;
                        newMclPtr->secLevel = currentMclPtr->secLevel;
                        newMclPtr->mtu = currentMclPtr->mtu;
                        newMclPtr->uuid = currentMclPtr->uuid;
                        newMclPtr->phandle = currentMclPtr->phandle;
                        newMclPtr->mclRole = MCAP_SERVER;
                        newMclPtr->deleteRole = MCAP_RESPONDER;
                        newMclPtr->flow->mode = L2CA_FLOW_MODE_ENHANCED_RETRANS;
                        newMclPtr->flow->tx_window = 1;
                        newMclPtr->flow->max_retransmit = MCAP_CONTROL_CHANNEL_MAX_RETRANS;
                        newMclPtr->flow->retrans_timeout = MCAP_CONTROL_CHANNEL_RETRANS_TIMEOUT;
                        newMclPtr->flow->monitor_timeout = MCAP_CONTROL_CHANNEL_MONITOR_TIMEOUT;
                        newMclPtr->flow->maximum_pdu = msg->mtu;
                        CsrMemCpy(flow, newMclPtr->flow, sizeof(L2CA_FLOW_T));
                        CSR_BT_MCAP_STATE_CHANGE(newMclPtr->mclState, MCAP_MCL_NULL_STATE);

                        CsrBtCml2caFecConnectAcceptReqSendExt( CSR_BT_MCAP_IFACEQUEUE,
                                                                msg->localPsm,
                                                                newMclPtr->classOfDevice,
                                                                newMclPtr->secLevel,
                                                                newMclPtr->mtu,
                                                                L2CA_FLUSH_TO_DEFAULT,
                                                                NULL,                /* L2CA_QOS_T serviceQuality */
                                                                flow,
                                                                FALSE,              /* CsrBool fallbackBasicMode */
                                                                newMclPtr->uuid);
                        currentPsmPtr->currentMcls++;
                    }
                }

                currentMclPtr->bd_addr = msg->deviceAddr;
                currentMclPtr->mclId = (CsrUint16)msg->btConnId; /* mclId and btConnId the same? */
                currentMclPtr->btConnId = msg->btConnId;
                CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_CONNECTED_STATE);
                csrBtMcapConnectMclIndSend(currentMclPtr->phandle,
                                            currentMclPtr->bd_addr,
                                            currentMclPtr->mclId,
                                            msg->localPsm,
                                            currentMclPtr->remoteControlPsm,
                                            CSR_BT_RESULT_CODE_MCAP_SUCCESS,
                                            CSR_BT_SUPPLIER_MCAP);


                csrBtMcapSetGlobalSniffTimeout(pInst);
                currentMclPtr->mclRole = MCAP_SERVER;
            }
            else /* This must be an MDL connection */
            {
                PsmBdAddrPtr    *searchDataPtr;

                CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL, msg->deviceAddr, msg->btConnId);

                searchDataPtr = (PsmBdAddrPtr *)CsrPmemAlloc(sizeof(PsmBdAddrPtr));
                searchDataPtr->psm = msg->localPsm;
                searchDataPtr->bdAddr = &(msg->deviceAddr);
                currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_DATA_PSM_AND_BDADDR(pInst->mclList, (void *) searchDataPtr);
                CsrPmemFree(searchDataPtr);
                if (currentMclPtr == NULL) /* This must be a connection we don't expect, so disconnect it */
                {
                    CsrBtCml2caDisconnectReqSend(msg->btConnId);
                }
                else
                {
                    searchState = MCAP_MDL_CONNECTING_STATE;
                    currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
                    if (currentMdlPtr == NULL)
                    {
                        searchState = MCAP_MDL_RECONNECTING_STATE;
                        currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
                    }

                    if (currentMdlPtr != NULL && currentMclPtr->mclState != MCAP_MCL_DEACTIVATING_STATE)
                    {
                        currentMdlPtr->btConnId = msg->btConnId;

                        currentMclPtr->localDataPsm = msg->localPsm;

                        CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_ACTIVE_STATE);
                        CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_ACTIVE_STATE);
                        csrBtMcapConnectMdlIndSend(currentMdlPtr->phandle,
                                                    currentMclPtr->mclId,
                                                    currentMdlPtr->mdlId,
                                                    msg->localPsm,
                                                    currentMclPtr->remoteDataPsm,
                                                    msg->localMtu,
                                                    msg->mtu,
                                                    CSR_BT_RESULT_CODE_MCAP_SUCCESS,
                                                    CSR_BT_SUPPLIER_MCAP);
                        currentMdlPtr->localMtu = msg->localMtu;
                        currentMdlPtr->remoteMtu = msg->mtu;
                    }
                    else /* This must be a connection we don't expect, so disconnect it */
                    {
                        CsrBtCml2caDisconnectReqSend(msg->btConnId);
                    }
                }
            }
        }
    }
    else /* msg->result != CSR_BT_RESULT_CODE_MCAP_SUCCESS */
    {
        currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_CONTROL_PSM(pInst->mclList, (void *) &(msg->localPsm));
        if (currentMclPtr != NULL)
        {
            csrBtMcapConnectMclIndSend(currentMclPtr->phandle,
                                        currentMclPtr->bd_addr,
                                        currentMclPtr->mclId,
                                        msg->localPsm,
                                        msg->localPsm,
                                        msg->resultCode,
                                        msg->resultSupplier);
            CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_NULL_STATE);
            CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclSubState, MCAP_MCL_NULL_STATE);
            currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->localPsm));
            if (currentPsmPtr != NULL)
            {
                currentPsmPtr->allowedMcls--;
            }
        }
        else
        {
            currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_DATA_PSM(pInst->mclList, (void *) &(msg->localPsm));
            if (currentMclPtr != NULL)
            {
                searchState = MCAP_MDL_CONNECTING_STATE;
                currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
                if (currentMdlPtr == NULL)
                {
                    searchState = MCAP_MDL_RECONNECTING_STATE;
                    currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
                }
            }
            if (currentMdlPtr != NULL)
            {
                csrBtMcapConnectMdlIndSend(currentMdlPtr->phandle,
                                            currentMclPtr->mclId,
                                            currentMdlPtr->mdlId,
                                            msg->localPsm,
                                            msg->localPsm,
                                            msg->localMtu,
                                            msg->mtu,
                                            msg->resultCode,
                                            msg->resultSupplier);
                CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_NULL_STATE);
            }
        }
    }
}

void CsrBtMcapCmL2caCancelConnectAcceptCfmHandler(McapInstanceData *pInst)
{
    CsrBtCmL2caCancelConnectAcceptCfm   *msg;
    McapPsmListEntry                    *currentPsmPtr = NULL;
    McapMclListEntry                    *currentMclPtr = NULL;
    CsrBool                              dataChannel = FALSE;

    msg = (CsrBtCmL2caCancelConnectAcceptCfm *)pInst->recvMsgP;

    currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_CONTROL_PSM(pInst->mclList, (void *) &(msg->localPsm));
    if(currentMclPtr == NULL)
    {
        currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_DATA_PSM(pInst->mclList, (void *) &(msg->localPsm));
        dataChannel = TRUE;
    }

    if(currentMclPtr == NULL)
    {
        /* Do nothing - just ignore */
    }
    else if (dataChannel == FALSE)
    {
        switch(currentMclPtr->mclState)
        {
        case MCAP_MCL_DEACTIVATING_STATE:
            {
                if ((msg->resultCode == CSR_BT_RESULT_CODE_CM_NOTHING_TO_CANCEL || msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) &&
                    msg->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    if (currentMclPtr->mclSubState == MCAP_MCL_NULL_STATE)
                    {
                        if (pInst->mclList.count)
                        {
                            /* Find out if more MCLs uses the same PSM */
                            CsrSchedQid   tempPhandle = currentMclPtr->phandle;
                            CsrBool      loop = TRUE;

                            while (loop)
                            {
                                currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_CONTROL_PSM(pInst->mclList, (void *) &(msg->localPsm));
                                if(currentMclPtr == NULL)
                                {
                                    csrBtMcapDeactivateCfmSend(tempPhandle,
                                                            msg->localPsm,
                                                            CSR_BT_RESULT_CODE_MCAP_SUCCESS,
                                                            CSR_BT_SUPPLIER_MCAP);
                                    currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->localPsm));
                                    if (currentPsmPtr != NULL)
                                    {
                                        MCAP_PSM_REMOVE_ENTRY(pInst->psmList, currentPsmPtr);
                                    }
                                    loop = FALSE;
                                }
                                else if (currentMclPtr->mclSubState == MCAP_MCL_NULL_STATE)
                                {
                                    MCAP_MCL_REMOVE_ENTRY(pInst->mclList, currentMclPtr);
                                }
                                else
                                {
                                    CsrBtCml2caDisconnectReqSend(currentMclPtr->btConnId);
                                    CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
                                    loop = FALSE;
                                }
                            }
                        }
                        else
                        {
                            csrBtMcapDeactivateCfmSend(currentMclPtr->phandle,
                                                        currentMclPtr->localControlPsm,
                                                        CSR_BT_RESULT_CODE_MCAP_SUCCESS,
                                                        CSR_BT_SUPPLIER_MCAP);
                            currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->localPsm));
                            if (currentPsmPtr != NULL)
                            {
                                MCAP_PSM_REMOVE_ENTRY(pInst->psmList, currentPsmPtr);
                            }
                        }
                    }
                    else
                    {
                        CsrBtCml2caDisconnectReqSend(currentMclPtr->btConnId);
                        CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
                        currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->localPsm));
                        if (currentPsmPtr != NULL)
                        {
                            currentPsmPtr->allowedMcls = 0;
                        }
                    }
                }
                else
                {
                    csrBtMcapDeactivateCfmSend(currentMclPtr->phandle,
                        currentMclPtr->localControlPsm,
                        CSR_BT_RESULT_CODE_MCAP_UNSPECIFIED_ERROR,
                        CSR_BT_SUPPLIER_MCAP);
                    currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->localPsm));
                    if (currentPsmPtr != NULL)
                    {
                        MCAP_PSM_REMOVE_ENTRY(pInst->psmList, currentPsmPtr);
                    }
                }
                break;
            }
        default:
            {
                /* Do noting - just ignore */
            }
        }
    }
    else /* if (dataChannel == TRUE) */
    {

        if ((msg->resultCode == CSR_BT_RESULT_CODE_CM_NOTHING_TO_CANCEL || msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) &&
            msg->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            CsrUint8             searchState = MCAP_MDL_DEACTIVATING_STATE;
            McapPsmListEntry    *currentPsmPtr = NULL;
            McapMdlListEntry    *currentMdlPtr = NULL;
            CsrBool              abort = FALSE;

            currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
            if (currentMdlPtr != NULL)
            {
                if (currentMclPtr->mclState == MCAP_MCL_DEACTIVATING_STATE)
                {
                    csrBtMcapDeactivateCfmSend(currentMclPtr->phandle,
                                                currentMclPtr->localControlPsm,
                                                CSR_BT_RESULT_CODE_MCAP_SUCCESS,
                                                CSR_BT_SUPPLIER_MCAP);
                    currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->localPsm));
                    if (currentPsmPtr != NULL)
                    {
                        MCAP_PSM_REMOVE_ENTRY(pInst->psmList, currentPsmPtr);
                    }
                    MCAP_MCL_REMOVE_ENTRY(pInst->mclList, currentMclPtr);
                    csrBtMcapSetGlobalSniffTimeout(pInst);
                }
                else
                {
                    CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_NULL_STATE);
                }
            }
            else
            {
                searchState = MCAP_MDL_NULL_STATE;
                currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
                if (currentMdlPtr != NULL)
                {
                    /* Ignore, mdl accept is cancelled as mcl is disconnected */
                }
                else
                {
                    searchState = MCAP_MDL_CONNECTING_STATE;
                    currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
                    if (currentMdlPtr == NULL)
                    {
                        searchState = MCAP_MDL_ABORTING_STATE;
                        currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(currentMclPtr->mdlList, &searchState);
                        if (currentMdlPtr != NULL)
                        {
                            abort = TRUE;
                        }
                    }
                    if (currentMdlPtr != NULL)
                    {
                        CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_NULL_STATE);
                    }
                    if (abort)
                    {
                        if (currentMdlPtr->disconnectRole == MCAP_REQUESTER)
                        {
                            if (msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                                msg->resultSupplier == CSR_BT_SUPPLIER_CM)
                            {
                                csrBtMcapDisconnectMdlCfmSend(currentMdlPtr->phandle,
                                                                currentMclPtr->mclId,
                                                                currentMdlPtr->mdlId,
                                                                CSR_BT_RESULT_CODE_MCAP_SUCCESS,
                                                                CSR_BT_SUPPLIER_MCAP);
                            }
                            else
                            {
                                csrBtMcapDisconnectMdlCfmSend(currentMdlPtr->phandle,
                                                            currentMclPtr->mclId,
                                                            currentMdlPtr->mdlId,
                                                            msg->resultCode,
                                                            msg->resultSupplier);
                            }
                        }
                        else
                        {
                            /* No need to do anything here */
                        }
                    }
                    else
                    {
                        csrBtMcapDeleteAllMdlEntries(pInst, currentMclPtr);
                    }
                }
            }
        }
        else
        {
            /* Do nothing - just ignore */
        }
    }
}

void CsrBtMcapCmL2caConnectCfmConnectingMclHandler(McapInstanceData *pInst)
{
    CsrBtCmL2caConnectCfm   *msg;
    McapPsmListEntry        *currentPsmPtr = NULL;
    McapMclListEntry        *currentMclPtr = NULL;
    PsmBdAddrPtr            *searchData;
    CsrUint32                i;
    CsrBool                  validCfm = TRUE;

    msg = (CsrBtCmL2caConnectCfm *)pInst->recvMsgP;

    currentMclPtr = (McapMclListEntry *)(pInst->mclList.first);
    for (i = 0; i < pInst->mclList.count; i++)
    {
        if (currentMclPtr->localControlPsm == msg->localPsm &&
            currentMclPtr->mclState != MCAP_MCL_CONNECTING_STATE &&
            bd_addr_eq(&(currentMclPtr->bd_addr), &(msg->deviceAddr)))
        {
            McapMclListEntry    *tempMclPtr = NULL;
            CsrUint16            searchState = MCAP_MCL_CONNECTING_STATE;

            validCfm = FALSE;
            tempMclPtr = MCAP_MCL_FIND_FROM_MCL_STATE(pInst->mclList, &searchState);

            if (tempMclPtr)
            {
                if (msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                    msg->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    CsrBtCml2caDisconnectReqSend(msg->btConnId);
                    csrBtMcapConnectMclCfmSend(tempMclPtr->phandle,
                                                msg->deviceAddr,
                                                (CsrUint16)msg->btConnId,
                                                msg->localPsm,
                                                CSR_BT_RESULT_CODE_MCAP_CONNECT_ATTEMPT_FAILED,
                                                CSR_BT_SUPPLIER_MCAP);
                }
                else
                {
                    csrBtMcapConnectMclCfmSend(tempMclPtr->phandle,
                                                msg->deviceAddr,
                                                (CsrUint16)msg->btConnId,
                                                msg->localPsm,
                                                msg->resultCode,
                                                msg->resultSupplier);
                }
                MCAP_MCL_REMOVE_ENTRY(pInst->mclList, tempMclPtr);
            }
        }
        currentMclPtr = currentMclPtr->next;
    }

    if (validCfm)
    {
        searchData = CsrPmemAlloc(sizeof(PsmBdAddrPtr));
        searchData->psm = msg->localPsm;
        searchData->bdAddr = &(msg->deviceAddr);
        currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_CONTROL_PSM_AND_BDADDR(pInst->mclList, searchData);

        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, msg->deviceAddr, msg->btConnId);

        if (currentMclPtr == NULL)
        {
            /* We can not do anything - so just ignore */
        }
        else if (msg->resultCode != CSR_BT_RESULT_CODE_CM_SUCCESS ||
            msg->resultSupplier != CSR_BT_SUPPLIER_CM)
        {
            csrBtMcapConnectMclCfmSend(currentMclPtr->phandle,
                                        currentMclPtr->bd_addr,
                                        currentMclPtr->mclId,
                                        msg->localPsm,
                                        msg->resultCode,
                                        msg->resultSupplier);
            MCAP_MCL_REMOVE_ENTRY(pInst->mclList, currentMclPtr);
        }
        else
        {
            currentMclPtr->mclId = (CsrUint16)msg->btConnId;
            currentMclPtr->btConnId = msg->btConnId;
            CSR_BT_MCAP_STATE_CHANGE(currentMclPtr->mclState, MCAP_MCL_CONNECTED_STATE);

            csrBtMcapConnectMclCfmSend(currentMclPtr->phandle,
                                        currentMclPtr->bd_addr,
                                        currentMclPtr->mclId,
                                        msg->localPsm,
                                        CSR_BT_RESULT_CODE_MCAP_SUCCESS,
                                        CSR_BT_SUPPLIER_MCAP);
            csrBtMcapSetGlobalSniffTimeout(pInst);
            currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(msg->localPsm));
            if (currentPsmPtr != NULL)
            {
                currentPsmPtr->currentMcls++;
            }
        }
        CsrPmemFree(searchData);
    }
}

void CsrBtMcapCmL2caConnectCfmDisconnectingMclHandler(McapInstanceData *pInst)
{
    CsrBtCmL2caConnectCfm   *msg;
    McapMclListEntry        *currentMclPtr = NULL;
    PsmBdAddrPtr            *searchData;

    msg = (CsrBtCmL2caConnectCfm *)pInst->recvMsgP;
    searchData = CsrPmemAlloc(sizeof(PsmBdAddrPtr));

    searchData->psm = msg->localPsm;
    searchData->bdAddr = &(msg->deviceAddr);

    currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_CONTROL_PSM_AND_BDADDR(pInst->mclList, searchData);
    CsrPmemFree(searchData);

    if (msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        msg->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if (currentMclPtr)
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, msg->deviceAddr, msg->btConnId);
        }
        else
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL, msg->deviceAddr, msg->btConnId);
        }
        CsrBtCml2caDisconnectReqSend(msg->btConnId);
    }
    if (currentMclPtr == NULL)
    {
        /* We can not do anything - so just ignore */
    }
    else
    {
        csrBtMcapConnectMclCfmSend(currentMclPtr->phandle,
                                    currentMclPtr->bd_addr,
                                    currentMclPtr->mclId,
                                    currentMclPtr->localControlPsm,
                                    CSR_BT_RESULT_CODE_MCAP_CANCELLED_CONNECT_ATTEMPT,
                                    CSR_BT_SUPPLIER_MCAP);
        MCAP_MCL_REMOVE_ENTRY(pInst->mclList, currentMclPtr);
        csrBtMcapSetGlobalSniffTimeout(pInst);
    }
}

void CsrBtMcapCmL2caConnectCfmHandler(McapInstanceData *pInst)
{
    CsrBtCmL2caConnectCfm  *msg;
    CsrUint8                searchState = MCAP_MDL_ABORTING_STATE;
    MclMdlPtr              *mclMdlPtr;
    CsrBool                 reconn = FALSE;
    CsrBool                 validCfm = TRUE;

    msg = (CsrBtCmL2caConnectCfm *)pInst->recvMsgP;

    if (pInst->mclList.count)
    {
        McapMclListEntry    *currentMclPtr = NULL;
        CsrUint32            i;

        currentMclPtr = (McapMclListEntry *)(pInst->mclList.first);
        for (i = 0; i < pInst->mclList.count; i++)
        {
            if (currentMclPtr->localControlPsm == msg->localPsm &&
                bd_addr_eq(&(currentMclPtr->bd_addr), &(msg->deviceAddr)))
            {
                /* This is a confirm for an MCL being cancelled */
                csrBtMcapConnectMclCfmSend(currentMclPtr->phandle,
                                            currentMclPtr->bd_addr,
                                            currentMclPtr->mclId,
                                            currentMclPtr->localControlPsm,
                                            CSR_BT_RESULT_CODE_MCAP_CANCELLED_CONNECT_ATTEMPT,
                                            CSR_BT_SUPPLIER_MCAP);
                MCAP_MCL_REMOVE_ENTRY(pInst->mclList, currentMclPtr);
                csrBtMcapSetGlobalSniffTimeout(pInst);
                validCfm = FALSE;
            }
            currentMclPtr = currentMclPtr->next;
        }
    }
    if (validCfm)
    {
        mclMdlPtr = CsrBtMcapFindFromBdAddrAndMdlState(pInst, msg->deviceAddr, searchState);

        if (mclMdlPtr->mdlPtr != NULL)
        {
            if (msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                msg->resultSupplier == CSR_BT_SUPPLIER_CM)
            {
                CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL, msg->deviceAddr, msg->btConnId);

                csrBtMcapConnectMdlCfmSend(mclMdlPtr->mclPtr->phandle,
                                            mclMdlPtr->mclPtr->mclId,
                                            mclMdlPtr->mdlPtr->mdlId,
                                            0,
                                            0,
                                            0,
                                            0,
                                            CSR_BT_RESULT_CODE_MCAP_CANCELLED_CONNECT_ATTEMPT,
                                            CSR_BT_SUPPLIER_MCAP);
                mclMdlPtr->mdlPtr->createRole = MCAP_RESPONDER;
                mclMdlPtr->mdlPtr->btConnId = msg->btConnId;
                mclMdlPtr->mclPtr->awaitingCfm = FALSE;
                CsrBtCml2caDisconnectReqSend(msg->btConnId);
            }
            else
            {
                csrBtMcapConnectMdlCfmSend(mclMdlPtr->mclPtr->phandle,
                                            mclMdlPtr->mclPtr->mclId,
                                            mclMdlPtr->mdlPtr->mdlId,
                                            0,
                                            0,
                                            0,
                                            0,
                                            msg->resultCode,
                                            msg->resultSupplier);
                mclMdlPtr->mdlPtr->createRole = MCAP_RESPONDER;
                CSR_BT_MCAP_STATE_CHANGE(mclMdlPtr->mdlPtr->mdlState, MCAP_MDL_NULL_STATE);
                mclMdlPtr->mclPtr->awaitingCfm = FALSE;
            }
        }
        else
        {
            searchState = MCAP_MDL_CONNECTING_STATE;
            CsrPmemFree(mclMdlPtr);
            mclMdlPtr = CsrBtMcapFindFromBdAddrAndMdlState(pInst, msg->deviceAddr, searchState);

            if (mclMdlPtr->mdlPtr == NULL)
            {
                searchState = MCAP_MDL_RECONNECTING_STATE;
                CsrPmemFree(mclMdlPtr);
                mclMdlPtr = CsrBtMcapFindFromBdAddrAndMdlState(pInst, msg->deviceAddr, searchState);
                if (mclMdlPtr->mdlPtr != NULL)
                {
                    reconn = TRUE;
                }
            }

            if (mclMdlPtr->mclPtr == NULL) /* If the MCL is not found */
            {
                /* We are not able to do anything as we can not find any MDL or MCL for the bd_addr */
            }
            else
            {
                CsrBtResultCode     msgResult   = (msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) ? CSR_BT_RESULT_CODE_MCAP_SUCCESS : msg->resultCode;
                CsrBtSupplier msgSupplier = (msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) ? CSR_BT_SUPPLIER_MCAP : msg->resultSupplier;

                if (msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && 
                    msg->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL, msg->deviceAddr, msg->btConnId);
                }

                if (reconn == FALSE)
                {
                    if (mclMdlPtr->mdlPtr)
                    {
                        csrBtMcapConnectMdlCfmSend(mclMdlPtr->mdlPtr->phandle,
                                              mclMdlPtr->mclPtr->mclId,
                                              mclMdlPtr->mdlPtr->mdlId,
                                              msg->localPsm,
                                              mclMdlPtr->mclPtr->remoteDataPsm,
                                              msg->localMtu,
                                              msg->mtu,
                                              msgResult,
                                              msgSupplier);
                        mclMdlPtr->mdlPtr->createRole = MCAP_RESPONDER;
                        mclMdlPtr->mdlPtr->localMtu = msg->localMtu;
                        mclMdlPtr->mdlPtr->remoteMtu = msg->mtu;
                    }
                    mclMdlPtr->mclPtr->awaitingCfm = FALSE;
                    mclMdlPtr->mclPtr->localDataPsm = msg->localPsm;
                }
                else
                {
                    csrBtMcapReconnectMdlCfmSend(mclMdlPtr->mdlPtr->phandle,
                                                mclMdlPtr->mclPtr->mclId,
                                                mclMdlPtr->mdlPtr->mdlId,
                                                msgResult,
                                                msgSupplier);
                    mclMdlPtr->mclPtr->awaitingCfm = FALSE;
                    mclMdlPtr->mdlPtr->reconnectRole = MCAP_RESPONDER;
                }

                if (msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                    msg->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    if (mclMdlPtr->mdlPtr)
                    {
                        CSR_BT_MCAP_STATE_CHANGE(mclMdlPtr->mdlPtr->mdlState, MCAP_MDL_ACTIVE_STATE);
                    }
                    CSR_BT_MCAP_STATE_CHANGE(mclMdlPtr->mclPtr->mclState, MCAP_MCL_ACTIVE_STATE);
                }
                else
                {
                    if (mclMdlPtr->mdlPtr)
                    {
                        CSR_BT_MCAP_STATE_CHANGE(mclMdlPtr->mdlPtr->mdlState, MCAP_MDL_NULL_STATE);
                    }
                }
                if (mclMdlPtr->mdlPtr)
                {
                    mclMdlPtr->mdlPtr->btConnId = msg->btConnId;
                }
                csrBtMcapSetGlobalSniffTimeout(pInst);
            }
        }
        CsrPmemFree(mclMdlPtr);
    }
}

void CsrBtMcapCmL2caDisconnectIndHandler(McapInstanceData *pInst)
{
    CsrBtCmL2caDisconnectInd    *msg;
    McapPsmListEntry            *currentPsmPtr = NULL;
    MclMdlPtr                   *commonPtr;

    msg = (CsrBtCmL2caDisconnectInd *)pInst->recvMsgP;

    commonPtr = CsrBtMcapFindMclAndMdlFromCid(pInst, msg->btConnId);

    if (commonPtr->mclPtr)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL, commonPtr->mclPtr->bd_addr, msg->btConnId);
    
        if (commonPtr->mdlPtr != NULL) 
        {
            csrBtMcapMdlDisconnectHandler(pInst, msg, commonPtr);
        }
        else
        {
            commonPtr->mclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->btConnId));
            if (commonPtr->mclPtr == NULL)
            {
                /* Do nothing - just ignore */
            }
            else if (commonPtr->mclPtr->mclState == MCAP_MCL_NULL_STATE)
            {
                if (commonPtr->mclPtr->mclRole == MCAP_SERVER)
                {
                    currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(commonPtr->mclPtr->localControlPsm));
                    if (currentPsmPtr != NULL)
                    {
                        if (currentPsmPtr->allowedMcls > currentPsmPtr->currentMcls)
                        {
                            L2CA_FLOW_T  *flow = CsrPmemAlloc(sizeof(L2CA_FLOW_T));

                            CsrMemCpy(flow, commonPtr->mclPtr->flow, sizeof(L2CA_FLOW_T));
                            CsrBtCml2caFecConnectAcceptReqSendExt(CSR_BT_MCAP_IFACEQUEUE,
                                                                    commonPtr->mclPtr->localControlPsm,
                                                                    commonPtr->mclPtr->classOfDevice,
                                                                    commonPtr->mclPtr->secLevel,
                                                                    commonPtr->mclPtr->mtu,
                                                                    L2CA_FLUSH_TO_DEFAULT,
                                                                    NULL,                /* L2CA_QOS_T serviceQuality */
                                                                    flow,
                                                                    FALSE,              /* CsrBool fallbackBasicMode */
                                                                    commonPtr->mclPtr->uuid);
                        }
                    }
                }
                else
                {
                    /* Do nothing - just ignore */
                }
            }
            else if (commonPtr->mclPtr->mclState == MCAP_MCL_DEACTIVATING_STATE) /* Local device disconnected */
            {
                if (CsrBtMcapIsDisconnectAbnormal(msg->reasonCode, msg->reasonSupplier))
                {
                    csrBtMcapDisconnectAllMdlEntries(pInst, commonPtr->mclPtr);
                    csrBtMcapDisconnectMclIndSend(commonPtr->mclPtr->phandle,
                                                commonPtr->mclPtr->bd_addr,
                                                commonPtr->mclPtr->mclId,
                                                msg->reasonCode,
                                                msg->reasonSupplier);
                    CSR_BT_MCAP_STATE_CHANGE(commonPtr->mclPtr->mclSubState, MCAP_MCL_NULL_STATE);
                }
                else
                {
                    csrBtMcapDisconnectMclIndSend(commonPtr->mclPtr->phandle,
                                                commonPtr->mclPtr->bd_addr,
                                                commonPtr->mclPtr->mclId,
                                                msg->reasonCode,
                                                msg->reasonSupplier);
                    csrBtMcapDeactivateCfmSend(commonPtr->mclPtr->phandle,
                                                commonPtr->mclPtr->localControlPsm,
                                                CSR_BT_RESULT_CODE_MCAP_SUCCESS,
                                                CSR_BT_SUPPLIER_MCAP);
                    currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(commonPtr->mclPtr->localControlPsm));
                    if (currentPsmPtr != NULL)
                    {
                        MCAP_PSM_REMOVE_ENTRY(pInst->psmList, currentPsmPtr);
                    }
                    MCAP_MCL_REMOVE_ENTRY(pInst->mclList, commonPtr->mclPtr);
                    csrBtMcapSetGlobalSniffTimeout(pInst);
                }
            }
            else
            {
                if (commonPtr->mclPtr->mclState == MCAP_MCL_DISCONNECTING_STATE) /* Local device disconnected */
                {
                    csrBtMcapDisconnectMclCfmSend(commonPtr->mclPtr->phandle,
                                                    commonPtr->mclPtr->bd_addr,
                                                    commonPtr->mclPtr->mclId,
                                                    msg->reasonCode,
                                                    msg->reasonSupplier);
                    CSR_BT_MCAP_STATE_CHANGE(commonPtr->mclPtr->mclSubState, MCAP_MCL_NULL_STATE);
                    currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(commonPtr->mclPtr->localControlPsm));
                    if (currentPsmPtr != NULL)
                    {
                        currentPsmPtr->currentMcls--;
                    }
                }
                else if (CsrBtMcapIsDisconnectAbnormal(msg->reasonCode, msg->reasonSupplier))
                {
                    McapMdlListEntry    *mdlPtr  = NULL;
                    CsrUint16            mdlCount = 0;

                    mdlPtr = (McapMdlListEntry *) (commonPtr->mclPtr->mdlList.first);
                    for (mdlCount = 0; mdlCount < commonPtr->mclPtr->mdlList.count; mdlCount++)
                    {
                        if ((mdlPtr->mdlState == MCAP_MDL_CREATING_STATE ||
                            mdlPtr->mdlState == MCAP_MDL_CONNECTING_STATE) &&
                            mdlPtr->createRole == MCAP_REQUESTER)
                        {
                            csrBtMcapConnectMdlCfmSend(commonPtr->mclPtr->phandle,
                                                        commonPtr->mclPtr->mclId,
                                                        mdlPtr->mdlId,
                                                        0,
                                                        0,
                                                        0,
                                                        0,
                                                        CSR_BT_RESULT_CODE_MCAP_CONNECT_ATTEMPT_FAILED,
                                                        CSR_BT_SUPPLIER_MCAP);
                            mdlPtr->createRole = MCAP_RESPONDER;
                            CSR_BT_MCAP_STATE_CHANGE(mdlPtr->mdlState, MCAP_MDL_NULL_STATE);
                            commonPtr->mclPtr->awaitingCfm = FALSE;
                        }
                        else if (mdlPtr->mdlState == MCAP_MDL_ABORTING_STATE)
                        {
                            csrBtMcapConnectMdlCfmSend(commonPtr->mclPtr->phandle,
                                                        commonPtr->mclPtr->mclId,
                                                        mdlPtr->mdlId,
                                                        0,
                                                        0,
                                                        0,
                                                        0,
                                                        CSR_BT_RESULT_CODE_MCAP_CANCELLED_CONNECT_ATTEMPT,
                                                        CSR_BT_SUPPLIER_MCAP);
                            CSR_BT_MCAP_STATE_CHANGE(mdlPtr->mdlState, MCAP_MDL_NULL_STATE);
                            commonPtr->mclPtr->awaitingCfm = FALSE;
                        }
                        else if (mdlPtr->mdlState == MCAP_MDL_DELETING_STATE)
                        {
                            csrBtMcapDeleteMdlCfmSend(commonPtr->mclPtr->phandle,
                                                    commonPtr->mclPtr->mclId,
                                                    mdlPtr->mdlId,
                                                    CSR_BT_RESULT_CODE_MCAP_PROTOCOL_UNSPECIFIED_ERROR);
                            CSR_BT_MCAP_STATE_CHANGE(mdlPtr->mdlState, MCAP_MDL_DISCONNECTING_STATE);
                            commonPtr->mclPtr->deletingMdlId = 0;
                            commonPtr->mclPtr->awaitingCfm = FALSE;
                        }
                        else if (mdlPtr->mdlState == MCAP_MDL_RECONNECTING_STATE &&
                            mdlPtr->reconnectRole == MCAP_REQUESTER)
                        {
                            csrBtMcapReconnectMdlCfmSend(commonPtr->mclPtr->phandle,
                                                        commonPtr->mclPtr->mclId,
                                                        mdlPtr->mdlId,
                                                        msg->reasonCode,
                                                        msg->reasonSupplier);
                            mdlPtr->reconnectRole = MCAP_RESPONDER;
                            commonPtr->mclPtr->awaitingCfm = FALSE;
                        }
                        mdlPtr = mdlPtr->next;
                    }
                    if (commonPtr->mclPtr->deletingMdlId == 0xFFFF)
                    {
                        csrBtMcapDeleteMdlCfmSend(commonPtr->mclPtr->phandle,
                                                    commonPtr->mclPtr->mclId,
                                                    0xFFFF,
                                                    CSR_BT_RESULT_CODE_MCAP_PROTOCOL_UNSPECIFIED_ERROR);
                        commonPtr->mclPtr->deletingMdlId = 0;
                        commonPtr->mclPtr->awaitingCfm = FALSE;
                    }
                    csrBtMcapDisconnectMclIndSend(commonPtr->mclPtr->phandle,
                                                    commonPtr->mclPtr->bd_addr,
                                                    commonPtr->mclPtr->mclId,
                                                    msg->reasonCode,
                                                    msg->reasonSupplier);
                    currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(commonPtr->mclPtr->localControlPsm));
                    if (currentPsmPtr != NULL)
                    {
                        currentPsmPtr->currentMcls--;
                    }
                    CSR_BT_MCAP_STATE_CHANGE(commonPtr->mclPtr->mclSubState, MCAP_MCL_NULL_STATE);
                }
                else /* Peer device disconnected */
                {
                    csrBtMcapDisconnectMclIndSend(commonPtr->mclPtr->phandle,
                                                commonPtr->mclPtr->bd_addr,
                                                commonPtr->mclPtr->mclId,
                                                msg->reasonCode,
                                                msg->reasonSupplier);
                    currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(commonPtr->mclPtr->localControlPsm));
                    if (currentPsmPtr != NULL)
                    {
                        currentPsmPtr->currentMcls--;
                    }
                    CSR_BT_MCAP_STATE_CHANGE(commonPtr->mclPtr->mclSubState, MCAP_MCL_NULL_STATE);
                    csrBtMcapDisconnectAllMdlEntries(pInst, commonPtr->mclPtr);
                }

                if (commonPtr->mclPtr->mclRole == MCAP_SERVER &&
                    commonPtr->mclPtr->mclState != MCAP_MCL_DEACTIVATING_STATE)
                {
                    L2CA_FLOW_T  *flow = CsrPmemAlloc(sizeof(L2CA_FLOW_T));
                    CsrMemCpy(flow, commonPtr->mclPtr->flow, sizeof(L2CA_FLOW_T));

                    CsrBtCml2caFecConnectAcceptReqSendExt( CSR_BT_MCAP_IFACEQUEUE,
                                                            commonPtr->mclPtr->localControlPsm,
                                                            commonPtr->mclPtr->classOfDevice,
                                                            commonPtr->mclPtr->secLevel,
                                                            commonPtr->mclPtr->mtu,
                                                            L2CA_FLUSH_TO_DEFAULT,
                                                            NULL,                /* L2CA_QOS_T serviceQuality */
                                                            flow,
                                                            FALSE,              /* CsrBool fallbackBasicMode */
                                                            commonPtr->mclPtr->uuid);
                }
                CSR_BT_MCAP_STATE_CHANGE(commonPtr->mclPtr->mclState, MCAP_MCL_NULL_STATE);
            }
        }
    }
    CsrPmemFree(commonPtr);
}

void CsrBtMcapCmL2caDataCfmHandler(McapInstanceData *pInst)
{
    CsrBtCmL2caDataCfm  *msg;
    MclMdlPtr           *commonPtr;
    CsrBtResultCode     msgResult;
    CsrBtSupplier msgSupplier;


    msg = (CsrBtCmL2caDataCfm *)pInst->recvMsgP;
    msgResult   = (msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) ? CSR_BT_RESULT_CODE_MCAP_SUCCESS : msg->resultCode;
    msgSupplier = (msg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) ? CSR_BT_SUPPLIER_MCAP : msg->resultSupplier;

    commonPtr = CsrBtMcapFindMclAndMdlFromCid(pInst, msg->btConnId);

    if (commonPtr->mclPtr == NULL && commonPtr->mdlPtr == NULL)
    {
        CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInst->recvMsgP);
    }
    else if (commonPtr->mclPtr != NULL)
    {
        if (commonPtr->mdlPtr == NULL)
        {
            /* This is a confirm of a control signal. The app should not receive anything in this case */
            /* Do nothing in this case */
            CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInst->recvMsgP);
        }
        else /* This is a MDL data confirm */
        {
            csrBtMcapDataSendCfmSend(commonPtr->mclPtr->phandle,
                                commonPtr->mclPtr->mclId,
                                commonPtr->mdlPtr->mdlId,
                                msgResult,
                                msgSupplier);
        }
    }

    CsrPmemFree(commonPtr);
}

void CsrBtMcapCmL2caDataIndIgnore(McapInstanceData *pInst)
{
    /* Ignore the data, but we must send the response */
    CsrBtCmL2caDataInd  *msg;
    msg = (CsrBtCmL2caDataInd *)pInst->recvMsgP;
    CsrBtCmL2caDataResSend(msg->btConnId);
    CsrPmemFree(msg->payload);
}

void CsrBtMcapCmL2caDataIndHandler(McapInstanceData *pInst)
{
    CsrBtCmL2caDataInd  *msg;
    MclMdlPtr           *commonPtr;

    msg = (CsrBtCmL2caDataInd *)pInst->recvMsgP;

    CsrBtCmL2caDataResSend(msg->btConnId);
    commonPtr = CsrBtMcapFindMclAndMdlFromCid(pInst, msg->btConnId);

    if (commonPtr->mclPtr == NULL && commonPtr->mdlPtr == NULL)
    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrStateEventException("MCAP (CsrBtMcapCmL2caDataIndHandler 1)",
                                CSR_BT_CM_PRIM,
                                msg->type,
                                0);
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
        CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInst->recvMsgP);
    }
    else
    {
        CsrUint8 *dataPtr;

        dataPtr = (CsrUint8 *)CsrPmemAlloc(msg->length);
        CsrMemCpy(dataPtr, msg->payload, msg->length);

        if (commonPtr->mdlPtr != NULL && commonPtr->mclPtr != NULL) /* This is data for the application */
        {
            csrBtMcapModeChangeReq(pInst, CSR_BT_ACTIVE_MODE);

            csrBtMcapDataSendIndSend(commonPtr->mclPtr->phandle, commonPtr->mclPtr->mclId, commonPtr->mdlPtr->mdlId, msg->length, dataPtr);
        }
        else if (commonPtr->mdlPtr == NULL && commonPtr->mclPtr != NULL) /* This is data sent on the control channel */
        {
            handleMcapMsg(pInst, msg->length, dataPtr, commonPtr->mclPtr);
        }
        else
        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("MCAP (CsrBtMcapCmL2caDataIndHandler 2)",
                                    CSR_BT_CM_PRIM,
                                    msg->type,
                                    0);
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
            CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInst->recvMsgP);
            CsrPmemFree(dataPtr);
        }
        CsrPmemFree(msg->payload);
    }
    CsrPmemFree(commonPtr);
}

void CsrBtMcapCmL2caModeChangeIndHandler(McapInstanceData *pInst)
{
    CsrBtCmL2caModeChangeInd  *msg;

    msg = (CsrBtCmL2caModeChangeInd *)pInst->recvMsgP;

    pInst->currentLocalMode = msg->mode;
    if (msg->mode == CSR_BT_ACTIVE_MODE)
    {
        if (pInst->timerId != 0)
        {
            CsrSchedTimerCancel(pInst->timerId,NULL,NULL);
            pInst->timerId = 0;
        }
        if (pInst->sniffTimeout)
        {
            pInst->timerId = CsrSchedTimerSet(pInst->sniffTimeout * 1000, csrBtMcapTimerHandler, 0, (void *) pInst);
        }
    }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
    else if (msg->mode == CSR_BT_PARK_MODE)
    {
        CsrBtCml2caModeChangeReqSend(msg->btConnId, CSR_BT_ACTIVE_MODE);
        pInst->requestedMode = CSR_BT_ACTIVE_MODE;
    }
#endif    
    else if (msg->mode == CSR_BT_SNIFF_MODE && pInst->timerId != 0)
    {
        CsrSchedTimerCancel(pInst->timerId,NULL,NULL);
        pInst->timerId = 0;
    }
}

void CsrBtMcapCmIgnoreMsg(McapInstanceData *pInst)
{
    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInst->recvMsgP);
}


/********************************************************/
/*                                                      */
/*    Other functions                                   */
/*                                                      */
/********************************************************/
static void csrBtMcapModeChangeReq(McapInstanceData *pInst, CsrUint8 newMode)
{
    CsrUint16            mclCount = 0;
    CsrUint16            mdlCount = 0;
    McapMclListEntry    *mclPtr  = NULL;
    McapMdlListEntry    *mdlPtr  = NULL;

    if (pInst->requestedMode != newMode)
    {
        mclPtr = (McapMclListEntry *) (pInst->mclList.first);
        for (mclCount = 0; mclCount < pInst->mclList.count; mclCount++)
        {
            if (mclPtr->mclState != MCAP_MCL_NULL_STATE &&
                mclPtr->mclState != MCAP_MCL_CONNECTING_STATE)
            {
                CsrBtCml2caModeChangeReqSend(mclPtr->btConnId, newMode);
            }
            mdlPtr = (McapMdlListEntry *) (mclPtr->mdlList.first);
            for (mdlCount = 0; mdlCount < mclPtr->mdlList.count; mdlCount++)
            {
                if (mdlPtr->mdlState != MCAP_MDL_NULL_STATE &&
                    mdlPtr->mdlState != MCAP_MDL_CREATING_STATE &&
                    mdlPtr->mdlState != MCAP_MDL_CONNECTING_STATE)
                {
                    CsrBtCml2caModeChangeReqSend(mdlPtr->btConnId, newMode);
                }
                mdlPtr = mdlPtr->next;
            }
            mclPtr = mclPtr->next;
        }
        pInst->requestedMode = newMode;
    }
    if (newMode == CSR_BT_ACTIVE_MODE)
    {
        if (pInst->timerId != 0)
        {
            CsrSchedTimerCancel(pInst->timerId,NULL,NULL);
            pInst->timerId = 0;
        }
        if (pInst->sniffTimeout)
        {
            pInst->timerId = CsrSchedTimerSet(pInst->sniffTimeout * 1000, csrBtMcapTimerHandler, 0, (void *) pInst);
        }
    }
}

static void csrBtMcapTimerHandler(CsrUint16 dummy, void *inst)
{
    McapInstanceData *pInst;
    CSR_UNUSED(dummy);

    pInst = (McapInstanceData *) inst;

    pInst->timerId = 0;
    if (pInst->requestedMode != CSR_BT_SNIFF_MODE)
    {
        csrBtMcapModeChangeReq(pInst, CSR_BT_SNIFF_MODE);
        pInst->requestedMode = CSR_BT_SNIFF_MODE;
    }
}

static void csrBtMcapSetGlobalSniffTimeout(McapInstanceData *pInst)
{
    CsrUint16            mclCount = 0;
    McapMclListEntry    *mclPtr  = NULL;

    mclPtr = (McapMclListEntry *)pInst->mclList.first;

    if (mclPtr)
    {
        pInst->sniffTimeout = 1; /* Sniff timeout is set to the lowest possible value. Zero means that sniff is never requested */

        for (mclCount = 0; mclCount < pInst->mclList.count; mclCount++)
        {
            if (mclPtr->mclState != MCAP_MCL_NULL_STATE && pInst->sniffTimeout != 0)
            {
                if (mclPtr->sniffTimeout == 0)
                {
                    pInst->sniffTimeout = 0;
                }
                else
                {
                    pInst->sniffTimeout =
                            (pInst->sniffTimeout >= mclPtr->sniffTimeout) ? pInst->sniffTimeout : mclPtr->sniffTimeout;
                }
            }
            mclPtr = mclPtr->next;
        }
    }
    else
    {
        pInst->sniffTimeout = 0;
    }

    if (pInst->timerId != 0)
    {
        CsrSchedTimerCancel(pInst->timerId, NULL, NULL);
        pInst->timerId = 0;
    }
    if (pInst->sniffTimeout)
    {
        pInst->timerId = CsrSchedTimerSet(pInst->sniffTimeout * 1000, csrBtMcapTimerHandler, 0, (void *) pInst);
    }
}

static CsrBool csrBtMcapCheckMdlId(McapMclListEntry *mclPtr, CsrUint16 mdlId, CsrBool ffffAllowed)
{
    McapMdlListEntry    *mdlPtr = NULL;

    if (mdlId == 0 || mdlId > 0xFEFF)
    {
        if (mdlId == 0xFFFF && ffffAllowed)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        mdlPtr = MCAP_MDL_FIND_FROM_MDLID(mclPtr->mdlList, &mdlId);
        if (mdlPtr == NULL)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
}

static CsrBool csrBtMcapCheckMdlIdCreate(CsrUint16 mdlId, CsrBool ffffAllowed)
{
    if (mdlId == 0 || mdlId > 0xFEFF)
    {
        if (mdlId == 0xFFFF && ffffAllowed)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return TRUE;
    }
}

static CsrBool csrBtMcapCheckMdepId(CsrUint8 mdepId)
{
    if (mdepId < 0x80)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void csrBtMcapDeleteAllMdlEntries(McapInstanceData *pInst, McapMclListEntry *mclPtr)
{
    McapMdlListEntry    *firstMdlEntryPtr;
    McapPsmListEntry    *currentPsmPtr = NULL;
    CsrBool              loop = TRUE;

    firstMdlEntryPtr = MCAP_MDL_GET_FIRST(mclPtr->mdlList);

    while (loop && (mclPtr->mdlList.count > 0) && firstMdlEntryPtr)
    {
        if (firstMdlEntryPtr->mdlState == MCAP_MDL_CONNECTING_STATE ||
            firstMdlEntryPtr->mdlState == MCAP_MDL_ACTIVE_STATE)
        {
            CsrBtCml2caDisconnectReqSend(firstMdlEntryPtr->btConnId);
            mclPtr->deletingMdlId = firstMdlEntryPtr->mdlId;
            firstMdlEntryPtr->mdlId = 0xFFFF;
            loop = FALSE;
        }
        else
        {
            McapMdlListEntry    *nextMdlEntryPtr = firstMdlEntryPtr->next;
            MCAP_MDL_REMOVE_ENTRY(mclPtr->mdlList, firstMdlEntryPtr);
            loop = TRUE;
            firstMdlEntryPtr = nextMdlEntryPtr;
        }
    }
    csrBtMcapSetGlobalSniffTimeout(pInst);
    if (mclPtr->mdlList.count == 0)
    {
        CsrUint8 *resPtr;

        if (mclPtr->mclState == MCAP_MCL_DEACTIVATING_STATE)
        {
            currentPsmPtr = MCAP_PSM_FIND(pInst->psmList, &(mclPtr->localControlPsm));
            if (currentPsmPtr != NULL)
            {
                if (currentPsmPtr->allowedMcls > currentPsmPtr->currentMcls)
                {
                    CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_MCAP_IFACEQUEUE, mclPtr->localControlPsm);
                }
                else if (mclPtr->mclSubState != MCAP_MCL_DISCONNECTING_STATE)
                {
                    CsrBtCml2caDisconnectReqSend(mclPtr->btConnId);
                    CSR_BT_MCAP_STATE_CHANGE(mclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
                }
            }
        }
        else if (mclPtr->deleteRole == MCAP_REQUESTER)
        {
            if ((mclPtr->mclState != MCAP_MCL_NULL_STATE) && 
                (mclPtr->mclRole == MCAP_CLIENT ||
                 mclPtr->awaitingRes == FALSE) )
            {
                resPtr = buildMdDeleteMdlReq(0xFFFF);
                CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_DELETE_MDL_REQ_LEN, resPtr);
                mclPtr->deletingMdlId = 0xFFFF;
                mclPtr->awaitingCfm = TRUE;
                mclPtr->awaitingRes = FALSE;
                mclPtr->reqOpCode = MD_DELETE_MDL_REQ;
                mclPtr->reqMdlId = 0xFFFF;
            }
            else
            {
                csrBtMcapDeleteMdlCfmSend(mclPtr->phandle,
                    mclPtr->mclId,
                    0xFFFF,
                    CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION);
                mclPtr->deletingMdlId = 0;
                mclPtr->awaitingCfm = FALSE;
            }
        }
        else if (mclPtr->awaitingRes == TRUE)
        {
            resPtr = buildMdDeleteMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS, 0xFFFF);
            CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_DELETE_MDL_RSP_LEN, resPtr);
            mclPtr->awaitingRes = FALSE;
        }
        else
        {
            /* We are not allowed to send any response in this case */
        }
    }
}

static void csrBtMcapDisconnectAllMdlEntries(McapInstanceData *pInst, McapMclListEntry *mclPtr)
{
    McapMdlListEntry    *currentMdlPtr;
    CsrBool              activeMdlFound = FALSE;
    CsrUint16            count;

    currentMdlPtr = (McapMdlListEntry *)mclPtr->mdlList.first;
    for (count = 0; count < mclPtr->mdlList.count; count++)
    {
        if (currentMdlPtr->mdlState == MCAP_MDL_ACTIVE_STATE)
        {
            CsrBtCml2caDisconnectReqSend(currentMdlPtr->btConnId);
            CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_DISCONNECTING_STATE);
            count = (CsrUint16) mclPtr->mdlList.count; /* This will stop the for-loop */
            activeMdlFound = TRUE;
        }
        else if (currentMdlPtr->mdlState == MCAP_MDL_CONNECTING_STATE && currentMdlPtr->createRole == MCAP_RESPONDER)
        {
            CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_NULL_STATE);
            CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_MCAP_IFACEQUEUE, mclPtr->localDataPsm);
        }
        else
        {
            currentMdlPtr = currentMdlPtr->next;
        }
    }

    if (!activeMdlFound && mclPtr->mclState != MCAP_MCL_NULL_STATE && mclPtr->mclSubState != MCAP_MCL_NULL_STATE)
    {
        CsrBtCml2caDisconnectReqSend(mclPtr->btConnId);
        CSR_BT_MCAP_STATE_CHANGE(mclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
    }
}

CsrUint8 getMclState(McapInstanceData *pInst, CsrUint16 primType)
{
    McapMclListEntry    *currentMclPtr = NULL;
    MclMdlPtr           *commonPtr = NULL;
    CsrUint8            *msgPtr;
    CsrPrim             *type;

    msgPtr = (CsrUint8 *)pInst->recvMsgP;
    type = (CsrPrim *)msgPtr;

    if (*type == CSR_BT_CM_L2CA_DATA_CFM)
    {
        CsrBtCmL2caDataCfm  *msg;

        msg = (CsrBtCmL2caDataCfm *)msgPtr;
        commonPtr = CsrBtMcapFindMclAndMdlFromCid(pInst, msg->btConnId);
        currentMclPtr = commonPtr->mclPtr;
    }
    else if (*type == CSR_BT_CM_L2CA_DATA_IND)
    {
        CsrBtCmL2caDataInd  *msg;

        msg = (CsrBtCmL2caDataInd *)msgPtr;
        commonPtr = CsrBtMcapFindMclAndMdlFromCid(pInst, msg->btConnId);
        currentMclPtr = commonPtr->mclPtr;
    }
    else if (*type == CSR_BT_CM_L2CA_CONNECT_CFM)
    {
        CsrBtCmL2caConnectCfm  *msg;
        PsmBdAddrPtr           *searchData;

        msg = (CsrBtCmL2caConnectCfm *)msgPtr;
        searchData = CsrPmemAlloc(sizeof(PsmBdAddrPtr));

        searchData->psm = msg->localPsm;
        searchData->bdAddr = &(msg->deviceAddr);

        currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_CONTROL_PSM_AND_BDADDR(pInst->mclList, searchData);
        if (currentMclPtr == NULL)
        {
            currentMclPtr = MCAP_MCL_FIND_FROM_LOCAL_DATA_PSM_AND_BDADDR(pInst->mclList, searchData);
        }
        CsrPmemFree(searchData);
    }
    else if (primType == CSR_BT_MCAP_PRIM && *type >= CSR_BT_MCAP_CREATE_MDL_RES && *type <= CSR_BT_MCAP_PRIM_DOWNSTREAM_HIGHEST)
    {
        CsrBtMcapCreateMdlRes  *msg;            /* The message received may not be a CSR_BT_MCAP_CREATE_MDL_RES, */
        msg = (CsrBtMcapCreateMdlRes *)msgPtr;  /* but the structure for the first two parameters is the same */

        currentMclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &(msg->mclId));
    }
    else
    {
        if (primType == CSR_BT_MCAP_PRIM)
        {
            CsrBtMcapDisconnectMclReq  *msg;            /* The message received may not be a CSR_BT_MCAP_DISCONNECT_MCL_REQ, */
            msg = (CsrBtMcapDisconnectMclReq *)msgPtr;  /* but the structure for the first two parameters is the same */

            currentMclPtr = MCAP_MCL_FIND_FROM_BDADDR(pInst->mclList, &(msg->bd_addr));
        }
        else if (primType == CSR_BT_CM_PRIM)
        {
            CsrBtCmL2caConnectAcceptCfm  *msg;            /* The message received may not be a CSR_BT_CM_CONNECT_ACCEPT_CFM, */
            msg = (CsrBtCmL2caConnectAcceptCfm *)msgPtr;  /* but the structure for the first parameters is the same */

            currentMclPtr = MCAP_MCL_FIND_FROM_BDADDR(pInst->mclList, &(msg->deviceAddr));
        }
    }

    CsrPmemFree(commonPtr);

    if (currentMclPtr != NULL)
    {
        return currentMclPtr->mclState;
    }
    else
    {
        return MCAP_UNDEFINED_STATE;
    }
}

MclMdlPtr *CsrBtMcapFindMclAndMdlFromCid(McapInstanceData *pInst, CsrBtConnId btConnId)
{
    MclMdlPtr   *returnPtr;
    CsrBool      i;
    
    returnPtr = CsrPmemAlloc(sizeof(MclMdlPtr));
    returnPtr->mclPtr = NULL;
    returnPtr->mdlPtr = NULL;

    if (pInst->mclList.count)
    {
        returnPtr->mclPtr = MCAP_MCL_FIND_FROM_MCLID(pInst->mclList, &btConnId);

        if (returnPtr->mclPtr == NULL) /* Search all MDLs in all MCLs */
        {
            returnPtr->mclPtr = (McapMclListEntry *)pInst->mclList.first;
            returnPtr->mdlPtr = NULL;
            for (i = TRUE; i; returnPtr->mclPtr = (McapMclListEntry *)(returnPtr->mclPtr->next))
            {
                returnPtr->mdlPtr = MCAP_MDL_FIND_FROM_CID(returnPtr->mclPtr->mdlList, &btConnId);
                if (returnPtr->mdlPtr != NULL || returnPtr->mclPtr->next == NULL)
                {
                    return returnPtr;
                }
            }
        }
    }
    return returnPtr;
}

MclMdlPtr *CsrBtMcapFindFromBdAddrAndMdlState(McapInstanceData *pInst, CsrBtDeviceAddr bd_addr, CsrUint8 mdlState)
{
    MclMdlPtr   *returnPtr;
    returnPtr = CsrPmemAlloc(sizeof(MclMdlPtr));
    returnPtr->mclPtr = NULL;
    returnPtr->mdlPtr = NULL;

    returnPtr->mclPtr = MCAP_MCL_FIND_FROM_BDADDR(pInst->mclList, &bd_addr);

    if (returnPtr->mclPtr)
    {
        returnPtr->mdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(returnPtr->mclPtr->mdlList, &mdlState);
    }
    return returnPtr;
}


CsrBool CsrBtMcapIsInPendingState(McapMclListEntry *mclPtr)
{
    CsrUint8 state = MCAP_MDL_CONNECTING_STATE;

    if (MCAP_MDL_FIND_FROM_MDL_STATE(mclPtr->mdlList, &state) != NULL)
    {
        return TRUE;
    }

    state = MCAP_MDL_RECONNECTING_STATE;
    if (MCAP_MDL_FIND_FROM_MDL_STATE(mclPtr->mdlList, &state) != NULL)
    {
        return TRUE;
    }

    return FALSE;
}

void handleMcapMsg(McapInstanceData *pInst, CsrUint16 len, CsrUint8 *msg, McapMclListEntry *mclPtr)
{
    switch (msg[0]) /* First byte is always the opcode */
    {
        case MD_ERROR_RSP:
            {
                if (len != MD_ERROR_RSP_LEN)
                {
                    CsrUint8 *resPtr;

                    resPtr = buildMdErrorRsp();
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_ERROR_RSP_LEN, resPtr);
                }
                break;
            }
        case MD_CREATE_MDL_REQ:
            {
                CsrUint16    mdlId  = 0;
                CsrUint8     mdepId = 0;
                CsrUint8     config = 0;

                if (len == MD_CREATE_MDL_REQ_LEN)
                {
                    mdlId = (msg[1] * 0x100) + msg[2];
                    mdepId = msg[3];
                    config = msg[4];
                }
                else
                {
                    CsrUint8             *resPtr;

                    if (len >= 3)
                    {
                        mdlId = (msg[1] * 0x100) + msg[2];

                        resPtr = buildMdCreateMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_PARAMETER_VALUE, mdlId, config);
                        CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_CREATE_MDL_RSP_LEN, resPtr);
                        break;
                    }
                }

                if (mclPtr->awaitingCfm && mclPtr->mclRole == MCAP_CLIENT)
                {
                    /* Message must be ignored according to MCAP specification */
                }
                else if (CsrBtMcapIsInPendingState(mclPtr) == TRUE)
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdCreateMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION, mdlId, config);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_CREATE_MDL_RSP_LEN, resPtr);
                }
                else if (len != MD_CREATE_MDL_REQ_LEN)
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdCreateMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_PARAMETER_VALUE, mdlId, config);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_CREATE_MDL_RSP_LEN, resPtr);
                }
                else if (!csrBtMcapCheckMdlIdCreate(mdlId, FALSE))
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdCreateMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_MDL, mdlId, config);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_CREATE_MDL_RSP_LEN, resPtr);
                }
                else if (!csrBtMcapCheckMdepId(mdepId))
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdCreateMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_MDEP, mdlId, config);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_CREATE_MDL_RSP_LEN, resPtr);
                }
                else
                {
                    if (mclPtr->awaitingCfm && mclPtr->mclRole == MCAP_SERVER)
                    {
                        csrBtMcapInvalidOpCodeSend(mclPtr);
                    }
                    csrBtMcapCreateMdlIndSend(mclPtr->phandle, mclPtr->mclId, mdlId, mdepId, config);
                    mclPtr->awaitingRes = TRUE;
                }
                break;
            }
        case MD_CREATE_MDL_RSP:
            {
                McapMdlListEntry    *currentMdlPtr    = NULL;
                CsrUint8             responseCode      = 0;
                CsrUint16            mdlId             = 0;
                CsrUint8             responseParameter = 0;

                if (len == MD_CREATE_MDL_RSP_LEN)
                {
                    responseCode = msg[1];
                    mdlId = (msg[2] * 0x100) + msg[3];
                    responseParameter = msg[4];
                }
                else if (len == MD_CREATE_MDL_RSP_LEN - 1)
                {
                    responseCode = msg[1];
                    mdlId = (msg[2] * 0x100) + msg[3];
                }
                else
                {
                    /* Ignore */
                }

                currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(mclPtr->mdlList, &mdlId);
                if (currentMdlPtr == NULL)
                {
                    /* MDL ID is wrong - so we can not anything - just ignore */
                }
                else if (mclPtr->awaitingCfm == FALSE)
                {
                    /* Not expecting this response, ignore */
                }
                else if (currentMdlPtr->mdlState == MCAP_MDL_DISCONNECTING_STATE)
                {
                    if (responseCode == CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS ||
                        responseCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS)
                    {
                        responseCode = CSR_BT_RESULT_CODE_MCAP_CANCELLED_CONNECT_ATTEMPT;
                    }
                    csrBtMcapConnectMdlCfmSend(mclPtr->phandle,
                                               mclPtr->mclId,
                                               mdlId,
                                               0,
                                               0,
                                               0,
                                               0,
                                               responseCode,
                                               CSR_BT_SUPPLIER_MCAP);
                    currentMdlPtr->createRole = MCAP_RESPONDER;
                    mclPtr->awaitingCfm = FALSE;
                }
                else if (responseCode != CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS)
                {
                    csrBtMcapConnectMdlCfmSend(mclPtr->phandle,
                                               mclPtr->mclId,
                                               mdlId,
                                               0,
                                               0,
                                               0,
                                               0,
                                               responseCode,
                                               CSR_BT_SUPPLIER_MCAP_PROTOCOL);
                    currentMdlPtr->createRole = MCAP_RESPONDER;
                    mclPtr->awaitingCfm = FALSE;
                }
                else
                {
                    CsrUint8         searchState;
                    L2CA_FLOW_T  *flow = NULL;

                    searchState = MCAP_MDL_CREATING_STATE;
                    currentMdlPtr = MCAP_MDL_FIND_FROM_MDL_STATE(mclPtr->mdlList, &searchState);
                    if (currentMdlPtr != NULL) 
                    {
                        if(currentMdlPtr->configuration == 0)
                        {
                            currentMdlPtr->configuration = responseParameter;
                        }
                        flow = CsrPmemAlloc(sizeof(L2CA_FLOW_T));
                        CsrMemCpy(flow, currentMdlPtr->flow, sizeof(L2CA_FLOW_T));
                    
                    
                        CsrBtCml2caFecConnectReqSendExt(CSR_BT_MCAP_IFACEQUEUE,
                                                mclPtr->bd_addr,
                                                mclPtr->localDataPsm,
                                                mclPtr->remoteDataPsm,
                                                currentMdlPtr->localMtu,
                                                L2CA_FLUSH_TO_DEFAULT,
                                                NULL,                      /* L2CA_QOS_T serviceQuality */
                                                flow,
                                                FALSE,                  /* CsrBool fallbackBasicMode */
                                                currentMdlPtr->secLevel);
                        CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_CONNECTING_STATE);
                    }
                }
                mclPtr->awaitingCfm = FALSE;
                break;
            }
        case MD_RECONNECT_MDL_REQ:
            {
                CsrUint16    mdlId = 0;

                if (len == MD_RECONNECT_MDL_REQ_LEN)
                {
                    mdlId = (msg[1] * 0x100) + msg[2];
                }
                else
                {
                    CsrUint8             *resPtr;

                    if (len >= 3)
                    {
                        mdlId = (msg[1] * 0x100) + msg[2];

                        resPtr = buildMdReconnectMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_PARAMETER_VALUE, mdlId);
                        CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_RECONNECT_MDL_RSP_LEN, resPtr);
                        break;
                    }
                }

                if (mclPtr->awaitingCfm && mclPtr->mclRole == MCAP_CLIENT)
                {
                    /* Message must be ignored according to MCAP specification */
                }
                else if (CsrBtMcapIsInPendingState(mclPtr) == TRUE)
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdReconnectMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION, mdlId);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_RECONNECT_MDL_RSP_LEN, resPtr);
                }
                else if (len != MD_RECONNECT_MDL_REQ_LEN)
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdReconnectMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_PARAMETER_VALUE, mdlId);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_RECONNECT_MDL_RSP_LEN, resPtr);
                }
                else if (!csrBtMcapCheckMdlId(mclPtr, mdlId, FALSE))
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdReconnectMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_MDL, mdlId);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_RECONNECT_MDL_RSP_LEN, resPtr);
                }
                else
                {
                    McapMdlListEntry    *currentMdlPtr = NULL;

                    currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(mclPtr->mdlList, &mdlId);
                    if (currentMdlPtr != NULL)
                    {
                        if (mclPtr->awaitingCfm && mclPtr->mclRole == MCAP_SERVER)
                        {
                            csrBtMcapInvalidOpCodeSend(mclPtr);
                        }
                        csrBtMcapReconnectMdlIndSend(mclPtr->phandle,
                                                     mclPtr->mclId,
                                                     mdlId,
                                                     mclPtr->localDataPsm,
                                                     mclPtr->remoteDataPsm,
                                                     currentMdlPtr->localMtu,
                                                     currentMdlPtr->remoteMtu,
                                                     mclPtr->bd_addr);
                        currentMdlPtr->reconnectRole = MCAP_RESPONDER;
                        mclPtr->awaitingRes = TRUE;
                    }
                }
                break;
            }
        case MD_RECONNECT_MDL_RSP:
            {
                McapMdlListEntry *currentMdlPtr = NULL;
                CsrUint8 responseCode            = 0;
                CsrUint16 mdlId                  = 0;
                
                if (len == MD_RECONNECT_MDL_RSP_LEN)
                {
                    responseCode = msg[1];
                    mdlId = (msg[2] * 0x100) + msg[3];
                }
                else
                {
                    /* Ignore */
                }

                currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(mclPtr->mdlList, &mdlId);
                if (currentMdlPtr == NULL)
                {
                    /* MDL ID is wrong - so we can not anything - just ignore */
                }
                else if (mclPtr->awaitingCfm == FALSE)
                {
                    /* Not expecting this response, ignore */
                }
                else if (responseCode != CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS &&
                         responseCode != CSR_BT_RESULT_CODE_MCAP_SUCCESS)
                {
                    csrBtMcapReconnectMdlCfmSend(mclPtr->phandle,
                                                 mclPtr->mclId,
                                                 mdlId,
                                                 responseCode,
                                                 CSR_BT_SUPPLIER_MCAP_PROTOCOL);
                    CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_NULL_STATE);
                    currentMdlPtr->reconnectRole = MCAP_RESPONDER;
                    mclPtr->awaitingCfm = FALSE;
                }
                else
                {
                    if ((currentMdlPtr->configuration == 1 && currentMdlPtr->flow->mode == L2CA_FLOW_MODE_ENHANCED_RETRANS) ||
                        (currentMdlPtr->configuration == 2 && currentMdlPtr->flow->mode == L2CA_FLOW_MODE_STREAMING))
                    {
                        L2CA_FLOW_T      *flow = CsrPmemAlloc(sizeof(L2CA_FLOW_T));

                        CsrMemCpy(flow, currentMdlPtr->flow, sizeof(L2CA_FLOW_T));
                        CsrBtCml2caFecConnectReqSendExt(CSR_BT_MCAP_IFACEQUEUE,
                                                mclPtr->bd_addr,
                                                mclPtr->localDataPsm,
                                                mclPtr->remoteDataPsm,
                                                currentMdlPtr->localMtu,
                                                L2CA_FLUSH_TO_DEFAULT,
                                                NULL,                      /* L2CA_QOS_T serviceQuality */
                                                flow,
                                                FALSE,                  /* CsrBool fallbackBasicMode */
                                                currentMdlPtr->secLevel);
                        CSR_BT_MCAP_STATE_CHANGE(currentMdlPtr->mdlState, MCAP_MDL_RECONNECTING_STATE);
                    }
                    else
                    {
                        csrBtMcapReconnectMdlCfmSend(mclPtr->phandle,
                                                     mclPtr->mclId,
                                                     mdlId,
                                                     CSR_BT_RESULT_CODE_MCAP_PROTOCOL_UNSPECIFIED_ERROR,
                                                     CSR_BT_SUPPLIER_MCAP_PROTOCOL);
                        currentMdlPtr->reconnectRole = MCAP_RESPONDER;
                        mclPtr->awaitingCfm = FALSE;
                    }
                }
                mclPtr->awaitingCfm = FALSE;
                break;
            }
        case MD_ABORT_MDL_REQ:
            {
                McapMdlListEntry    *currentMdlPtr = NULL;
                CsrUint16            mdlId;
                CsrUint8             *resPtr;

                if (len == MD_ABORT_MDL_REQ_LEN)
                {
                    mdlId = (msg[1] * 0x100) + msg[2];
                }
                else
                {
                    CsrUint8             *resPtr;

                    if (len >= 3)
                    {
                        mdlId = (msg[1] * 0x100) + msg[2];

                        resPtr = buildMdAbortMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_PARAMETER_VALUE, mdlId);
                        CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_ABORT_MDL_RSP_LEN, resPtr);
                        break;
                    }
                }

                currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(mclPtr->mdlList, &mdlId);
                if (mclPtr->awaitingCfm && mclPtr->mclRole == MCAP_CLIENT)
                {
                    /* Message must be ignored according to MCAP specification */
                }
                else if (len != MD_ABORT_MDL_REQ_LEN || CsrBtMcapIsInPendingState(mclPtr) == FALSE)
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdAbortMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION, mdlId);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_ABORT_MDL_RSP_LEN, resPtr);
                }
                else if ((!csrBtMcapCheckMdlId(mclPtr, mdlId, FALSE)) || currentMdlPtr == NULL)
                {
                    resPtr = buildMdAbortMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_MDL, mdlId);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_ABORT_MDL_RSP_LEN, resPtr);
                }
                else if (currentMdlPtr->mdlState != MCAP_MDL_CONNECTING_STATE)
                {
                    resPtr = buildMdAbortMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION, mdlId);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_ABORT_MDL_RSP_LEN, resPtr);
                }
                else
                {
                    if (mclPtr->awaitingCfm && mclPtr->mclRole == MCAP_SERVER)
                    {
                        csrBtMcapInvalidOpCodeSend(mclPtr);
                    }
                    csrBtMcapAbortMdlIndSend(mclPtr->phandle, mclPtr->mclId, mdlId);
                    mclPtr->awaitingRes = TRUE;
                }
                break;
            }
        case MD_ABORT_MDL_RSP:
            {
                McapMdlListEntry    *currentMdlPtr = NULL;
                CsrUint8             responseCode   = 0;
                CsrUint16            mdlId          = 0;

                if (len == MD_ABORT_MDL_RSP_LEN)
                {
                    responseCode = msg[1];
                    mdlId = (msg[2] * 0x100) + msg[3];
                }
                else
                {
                    /* Ignore */
                }

                currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(mclPtr->mdlList, &mdlId);
                if (currentMdlPtr == NULL)
                {
                    /* Ignore this response as mdl is unknown*/
                }
                else if (mclPtr->awaitingCfm == FALSE)
                {
                    /* Not expecting this response, ignore */
                }
                else if (currentMdlPtr->mdlState == MCAP_MDL_ABORTING_STATE &&
                         responseCode == CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS)
                {
                    if (currentMdlPtr->createRole == MCAP_REQUESTER)
                    {
                        CsrBtCml2caCancelConnectReqSend(CSR_BT_MCAP_IFACEQUEUE,
                                                   mclPtr->bd_addr,
                                                   mclPtr->localDataPsm);
                    }
                    else
                    {
                        CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_MCAP_IFACEQUEUE, mclPtr->localDataPsm);
                    }
                }
                mclPtr->awaitingCfm = FALSE;
                break;
            }
        case MD_DELETE_MDL_REQ:
            {
                CsrUint16    mdlId = 0;

                if (len == MD_DELETE_MDL_REQ_LEN)
                {
                    mdlId = (msg[1] * 0x100) + msg[2];
                }
                else
                {
                    CsrUint8             *resPtr;

                    if (len >= 3)
                    {
                        mdlId = (msg[1] * 0x100) + msg[2];

                        resPtr = buildMdDeleteMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_PARAMETER_VALUE, mdlId);
                        CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_DELETE_MDL_RSP_LEN, resPtr);
                        break;
                    }
                }

                if (mclPtr->awaitingCfm && mclPtr->mclRole == MCAP_CLIENT)
                {
                    /* Message must be ignored according to MCAP specification */
                }
                else if (CsrBtMcapIsInPendingState(mclPtr) == TRUE)
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdDeleteMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION, mdlId);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_DELETE_MDL_RSP_LEN, resPtr);
                }
                else if (len != MD_DELETE_MDL_REQ_LEN)
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdDeleteMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_PARAMETER_VALUE, mdlId);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_DELETE_MDL_RSP_LEN, resPtr);
                }
                else if (!csrBtMcapCheckMdlId(mclPtr, mdlId, TRUE))
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdDeleteMdlRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_MDL, mdlId);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_DELETE_MDL_RSP_LEN, resPtr);
                }
                else
                {
                    if (mclPtr->awaitingCfm && mclPtr->mclRole == MCAP_SERVER)
                    {
                        csrBtMcapInvalidOpCodeSend(mclPtr);
                    }
                    csrBtMcapDeleteMdlIndSend(mclPtr->phandle, mclPtr->mclId, mdlId);
                    mclPtr->awaitingRes = TRUE;
                }
                break;
            }
        case MD_DELETE_MDL_RSP:
            {
                McapMdlListEntry    *currentMdlPtr = NULL;
                CsrUint8             responseCode   = 0;
                CsrUint16            mdlId          = 0;

                if (len == MD_DELETE_MDL_RSP_LEN)
                {
                    responseCode = msg[1];
                    mdlId = (msg[2] * 0x100) + msg[3];
                }
                else
                {
                    /* Ignore */
                }

                currentMdlPtr = MCAP_MDL_FIND_FROM_MDLID(mclPtr->mdlList, &mdlId);
                if (currentMdlPtr == NULL && mdlId != 0xFFFF)
                {
                    /* Ignore this response as mdl is unknown*/
                    break;
                }
                else if (mclPtr->awaitingCfm == FALSE)
                {
                    /* Not expecting this response, ignore */
                    break;
                }

                if (responseCode == CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS)
                {
                    if (mdlId != 0xFFFF) /* If mdlId = 0xFFFF, all MDLs are disconnected and removed at this stage */
                    {
                        MCAP_MDL_REMOVE_ENTRY(mclPtr->mdlList, currentMdlPtr);
                        if (mclPtr->mdlList.count == 0)
                        {
                            CSR_BT_MCAP_STATE_CHANGE(mclPtr->mclState, MCAP_MCL_CONNECTED_STATE);
                        }
                        csrBtMcapSetGlobalSniffTimeout(pInst);
                    }

                    if (mclPtr->mclState == MCAP_MCL_DISCONNECTING_STATE) /* if MCL is being disconnected */
                    {
                        CsrBtCml2caDisconnectReqSend(mclPtr->btConnId);
                        CSR_BT_MCAP_STATE_CHANGE(mclPtr->mclSubState, MCAP_MCL_DISCONNECTING_STATE);
                    }
                    else
                    {
                        if (mclPtr->mdlList.count == 0)
                        {
                            CSR_BT_MCAP_STATE_CHANGE(mclPtr->mclState, MCAP_MCL_CONNECTED_STATE);
                        }
                    }
                }

                csrBtMcapDeleteMdlCfmSend(mclPtr->phandle,
                                          mclPtr->mclId,
                                          mdlId,
                                          responseCode);
                mclPtr->awaitingCfm = FALSE;
                break;
            }
        case MD_SYNC_CAP_REQ:
            {
                if (len != MD_SYNC_CAP_REQ_LEN)
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdSyncCapRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_PARAMETER_VALUE, 0, 0, 0, 0);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_SYNC_CAP_RSP_LEN, resPtr);
                }
                else
                {
                    CsrUint16    timeStampRequiredAccuracy = (msg[1] * 0x100) + msg[2];

                    csrBtMcapSyncCapIndSend(mclPtr->phandle, mclPtr->mclId, timeStampRequiredAccuracy);
                }
                break;
            }
        case MD_SYNC_CAP_RSP:
            {
                if (len == MD_SYNC_CAP_RSP_LEN)
                {
                    CsrUint8     responseCode = msg[1];
                    CsrUint8     btClkAccessRes = msg[2];
                    CsrUint16    syncLeadTime = (msg[3] * 0x100) + msg[4];
                    CsrUint16    timeStampNativeRes = (msg[5] * 0x100) + msg[6];
                    CsrUint16    timeStampNativeAccuracy = (msg[7] * 0x100) + msg[8];

                    if (mclPtr->awaitingSyncRes == TRUE)
                    {
                        csrBtMcapSyncCapCfmSend(mclPtr->phandle,
                                           mclPtr->mclId,
                                           responseCode,
                                           btClkAccessRes,
                                           syncLeadTime,
                                           timeStampNativeRes,
                                           timeStampNativeAccuracy);
                        mclPtr->awaitingSyncRes = FALSE;
                    }
                }
                else
                {
                    /* Ignore */
                }
                break;
            }
        case MD_SYNC_SET_REQ:
            {
                if (len != MD_SYNC_SET_REQ_LEN)
                {
                    CsrUint8             *resPtr;

                    resPtr = buildMdSyncSetRes(CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_PARAMETER_VALUE, 0, 0, 0, 0);
                    CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_SYNC_SET_RSP_LEN, resPtr);
                }
                else
                {
                    CsrBool      tsUpdateInfo = (CsrBool)msg[1];
                    CsrUint32    btClkSyncTime = (msg[2] * 0x1000000) + (msg[3] * 0x10000) + (msg[4] * 0x100) + msg[5];
                    CsrUint32    tsSyncTimeLsb = (msg[6] * 0x1000000) + (msg[7] * 0x10000) + (msg[8] * 0x100) + msg[9];
                    CsrUint32    tsSyncTimeMsb = (msg[10] * 0x1000000) + (msg[11] * 0x10000) + (msg[12] * 0x100) + msg[13];

                    csrBtMcapSyncSetIndSend(mclPtr->phandle,
                                       mclPtr->mclId,
                                       tsUpdateInfo,
                                       btClkSyncTime,
                                       tsSyncTimeLsb,
                                       tsSyncTimeMsb);
                }
                break;
            }
        case MD_SYNC_SET_RSP:
            {
                if (len == MD_SYNC_SET_RSP_LEN)
                {
                    CsrUint8     responseCode = msg[1];
                    CsrUint32    btClkSyncTime = (msg[2] * 0x1000000) + (msg[3] * 0x10000) + (msg[4] * 0x100) + msg[5];
                    CsrUint32    tsSyncTimeLsb = (msg[6] * 0x1000000) + (msg[7] * 0x10000) + (msg[8] * 0x100) + msg[9];
                    CsrUint32    tsSyncTimeMsb = (msg[10] * 0x1000000) + (msg[11] * 0x10000) + (msg[12] * 0x100) + msg[13];
                    CsrUint16    timeStampSampleAcc = (msg[14] * 0x100) + msg[15];

                    if (mclPtr->awaitingSyncRes == TRUE)
                    {
                        csrBtMcapSyncSetCfmSend(mclPtr->phandle,
                                           mclPtr->mclId,
                                           responseCode,
                                           btClkSyncTime,
                                           tsSyncTimeLsb,
                                           tsSyncTimeMsb,
                                           timeStampSampleAcc);
                        mclPtr->awaitingSyncRes = FALSE;
                    }
                }
                else
                {
                    /* Ignore */
                }

                break;
            }
        case MD_SYNC_INFO_IND:
            {
                if (len == MD_SYNC_INFO_IND_LEN)
                {
                    CsrUint32    btClkSyncTime = (msg[1] * 0x1000000) + (msg[2] * 0x10000) + (msg[3] * 0x100) + msg[4];
                    CsrUint32    tsSyncTimeLsb = (msg[5] * 0x1000000) + (msg[6] * 0x10000) + (msg[7] * 0x100) + msg[8];
                    CsrUint32    tsSyncTimeMsb = (msg[9] * 0x1000000) + (msg[10] * 0x10000) + (msg[11] * 0x100) + msg[12];
                    CsrUint16    timeStampSampleAcc = (msg[13] * 0x100) + msg[14];

                    csrBtMcapSyncInfoIndSend(mclPtr->phandle,
                                        mclPtr->mclId,
                                        btClkSyncTime,
                                        tsSyncTimeLsb,
                                        tsSyncTimeMsb,
                                        timeStampSampleAcc);
                }
                else
                {
                    /* Ignore */
                }
                break;
            }
        default:
            {
                CsrUint8 *resPtr;

                resPtr = buildMdErrorRsp();
                CsrBtCml2caDataReqSend(mclPtr->btConnId, MD_ERROR_RSP_LEN, resPtr);
                break;
            }
    } /* End of Switch */
    CsrPmemFree(msg);
}

void csrBtMcapInvalidOpCodeSend(McapMclListEntry *mclPtr)
{

    switch (mclPtr->reqOpCode)
    {
    case MD_CREATE_MDL_REQ:
        {
            csrBtMcapConnectMdlCfmSend(mclPtr->phandle,
                                        mclPtr->mclId,
                                        mclPtr->reqMdlId,
                                        0,
                                        0,
                                        0,
                                        0,
                                        CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                        CSR_BT_SUPPLIER_MCAP_PROTOCOL);
            break;
        }
    case MD_RECONNECT_MDL_REQ:
        {
            csrBtMcapReconnectMdlCfmSend(mclPtr->phandle,
                                        mclPtr->mclId,
                                        mclPtr->reqMdlId,
                                        CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                        CSR_BT_SUPPLIER_MCAP_PROTOCOL);
            break;
        }
    case MD_ABORT_MDL_REQ:
        {
            /* No ABORT_MDL_CFM signal exists - so ignore */
            break;
        }
    case MD_DELETE_MDL_REQ:
        {
            csrBtMcapDeleteMdlCfmSend(mclPtr->phandle,
                                        mclPtr->mclId,
                                        mclPtr->reqMdlId,
                                        CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION);
            break;
        }
    case MD_SYNC_CAP_REQ:
        {
            csrBtMcapSyncCapCfmSend(mclPtr->phandle,
                                    mclPtr->mclId,
                                    CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                    0,
                                    0,
                                    0,
                                    0);
            break;
        }
    case MD_SYNC_SET_REQ:
        {
            csrBtMcapSyncSetCfmSend(mclPtr->phandle,
                                    mclPtr->mclId,
                                    CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPERATION,
                                    0,
                                    0,
                                    0,
                                    0);
            break;
        }
    default:
        {
            /* Do nothing - just ignore */
        }
    }
    mclPtr->awaitingCfm = FALSE;
}


#ifdef CSR_LOG_ENABLE
void CsrBtMcapChangeState(const CsrCharString *file, CsrUint32 line,
                          const CsrCharString *fsmName,
                          CsrUint8 *state, CsrUint8 newState, const CsrCharString *newStateName)
{
    CsrLogStateTransition((CSR_LOG_STATE_TRANSITION_MASK_FSM_NAME |
                            CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE |
                            CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE_STR |
                            CSR_LOG_STATE_TRANSITION_MASK_PREV_STATE),
                            0,
                            fsmName,
                            *state,
                            NULL,
                            0,
                            NULL,
                            newState,
                            newStateName,
                            line,
                            file);

    *state = newState;
}
#endif

#endif

