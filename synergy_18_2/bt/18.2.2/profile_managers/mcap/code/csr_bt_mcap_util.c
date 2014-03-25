/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_MCAP_MODULE

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_mcap_handler.h"
#include "csr_bt_mcap_sef.h"
#include "csr_bt_mcap_util.h"

/********************************************************/
/*                                                      */
/*    Linked List functions                             */
/*                                                      */
/********************************************************/

void CsrBtMcapUtilCmnListPsmAdd(CsrCmnListElm_t *elem)
{
    ((McapPsmListEntry *)elem)->psm                 = 0;
    ((McapPsmListEntry *)elem)->allowedMcls         = 0;
    ((McapPsmListEntry *)elem)->currentMcls         = 0;
}

void CsrBtMcapUtilCmnListPsmRemove(CsrCmnListElm_t *elem)
{
    /* No special needs */
}

CsrBool CsrBtMcapUtilComparePsm(CsrCmnListElm_t *elem, void *data)
{
    return (((McapPsmListEntry *)elem)->psm == *((CsrUint16 *)data));
}


void CsrBtMcapUtilCmnListMclAdd(CsrCmnListElm_t *elem)
{
    CsrCmnListInit(&(((McapMclListEntry *)elem)->mdlList), 0, CsrBtMcapUtilCmnListMdlAdd, CsrBtMcapUtilCmnListMdlRemove);

    bd_addr_zero(&((McapMclListEntry *)elem)->bd_addr);
    ((McapMclListEntry *)elem)->phandle             = 0;
    ((McapMclListEntry *)elem)->mclState            = MCAP_MCL_NULL_STATE;
    ((McapMclListEntry *)elem)->mclSubState         = MCAP_MCL_NULL_STATE;
    ((McapMclListEntry *)elem)->sniffTimeout        = 0;
    ((McapMclListEntry *)elem)->mclId               = 0;
    ((McapMclListEntry *)elem)->localControlPsm     = 0;
    ((McapMclListEntry *)elem)->localDataPsm        = 0;
    ((McapMclListEntry *)elem)->remoteControlPsm    = 0;
    ((McapMclListEntry *)elem)->remoteDataPsm       = 0;
    ((McapMclListEntry *)elem)->classOfDevice       = 0;
    ((McapMclListEntry *)elem)->uuid                = 0;
    ((McapMclListEntry *)elem)->deletingMdlId       = 0;
    ((McapMclListEntry *)elem)->flow                = CsrPmemAlloc(sizeof(L2CA_FLOW_T));
    ((McapMclListEntry *)elem)->awaitingRes         = FALSE;
    ((McapMclListEntry *)elem)->awaitingCfm         = FALSE;
    ((McapMclListEntry *)elem)->awaitingSyncRes     = FALSE;
}

void CsrBtMcapUtilCmnListMclRemove(CsrCmnListElm_t *elem)
{
    CsrUint32 i, mdlCount;
    McapMdlListEntry *nodeToRemove = MCAP_MDL_GET_FIRST(((McapMclListEntry *)elem)->mdlList);
    
    CsrPmemFree(((McapMclListEntry *)elem)->flow);

    mdlCount = ((McapMclListEntry *)elem)->mdlList.count;
    for (i = 0; ((i < mdlCount) && nodeToRemove); i++)
    {
        McapMdlListEntry *nextNode = nodeToRemove->next;
        MCAP_MDL_REMOVE_ENTRY(((McapMclListEntry *)elem)->mdlList, nodeToRemove);
        nodeToRemove = nextNode;
    }
}

void CsrBtMcapUtilCmnListMdlAdd(CsrCmnListElm_t *elem)
{
    ((McapMdlListEntry *)elem)->mdlId          = 0;
    ((McapMdlListEntry *)elem)->mdlState       = MCAP_MDL_NULL_STATE;
    ((McapMdlListEntry *)elem)->btConnId       = 0;
    ((McapMdlListEntry *)elem)->localMtu       = 0;
    ((McapMdlListEntry *)elem)->remoteMtu      = 0;
    ((McapMdlListEntry *)elem)->flow           = CsrPmemAlloc(sizeof(L2CA_FLOW_T));
    ((McapMdlListEntry *)elem)->createRole     = MCAP_RESPONDER;
    ((McapMdlListEntry *)elem)->reconnectRole  = MCAP_RESPONDER;
    ((McapMdlListEntry *)elem)->disconnectRole = MCAP_RESPONDER;
}

void CsrBtMcapUtilCmnListMdlRemove(CsrCmnListElm_t *elem)
{
    CsrPmemFree(((McapMdlListEntry *)elem)->flow);
}

CsrBool CsrBtMcapUtilCompareBdAddr(CsrCmnListElm_t *elem, void *data)
{
    return CsrBtBdAddrEq(&((McapMclListEntry *)elem)->bd_addr, (BD_ADDR_T *)data);
}

CsrBool CsrBtMcapUtilCompareMdlId(CsrCmnListElm_t *elem, void *data)
{
    return (((McapMdlListEntry *)elem)->mdlId == *((CsrUint16 *)data));
}

CsrBool CsrBtMcapUtilCompareLocalControlPsm(CsrCmnListElm_t *elem, void *data)
{
    return (((McapMclListEntry *)elem)->localControlPsm == *((CsrUint16 *)data));
}

CsrBool CsrBtMcapUtilCompareLocalControlPsmAndMclState(CsrCmnListElm_t *elem, void *data)
{
    PsmStatePtr *dataPtr = (PsmStatePtr *)data;

    return (((McapMclListEntry *)elem)->localControlPsm == (CsrUint16)dataPtr->psm &&
            ((McapMclListEntry *)elem)->mclState == (CsrUint8)dataPtr->state);
}

CsrBool CsrBtMcapUtilCompareLocalDataPsm(CsrCmnListElm_t *elem, void *data)
{
    return (((McapMclListEntry *)elem)->localDataPsm == *((CsrUint16 *)data));
}

CsrBool CsrBtMcapUtilCompareLocalControlPsmAndBdAddr(CsrCmnListElm_t *elem, void *data)
{
    PsmBdAddrPtr    *dataPtr = (PsmBdAddrPtr *)data;

    return (((McapMclListEntry *)elem)->localControlPsm == (CsrUint16)dataPtr->psm &&
            CsrBtBdAddrEq(&((McapMclListEntry *)elem)->bd_addr, (BD_ADDR_T *)dataPtr->bdAddr));
}

CsrBool CsrBtMcapUtilCompareLocalDataPsmAndBdAddr(CsrCmnListElm_t *elem, void *data)
{
    PsmBdAddrPtr    *dataPtr = (PsmBdAddrPtr *)data;

    return (((McapMclListEntry *)elem)->localDataPsm == (CsrUint16)dataPtr->psm &&
            CsrBtBdAddrEq(&((McapMclListEntry *)elem)->bd_addr, (BD_ADDR_T *)dataPtr->bdAddr));
}

CsrBool CsrBtMcapUtilCompareRemoteControlPsm(CsrCmnListElm_t *elem, void *data)
{
    return (((McapMclListEntry *)elem)->remoteControlPsm == *((CsrUint16 *)data));
}

CsrBool CsrBtMcapUtilCompareRemoteDataPsm(CsrCmnListElm_t *elem, void *data)
{
    return (((McapMclListEntry *)elem)->remoteDataPsm == *((CsrUint16 *)data));
}

CsrBool CsrBtMcapUtilCompareMclState(CsrCmnListElm_t *elem, void *data)
{
    return (((McapMclListEntry *)elem)->mclState == *((CsrUint16 *)data));
}

CsrBool CsrBtMcapUtilCompareMclId(CsrCmnListElm_t *elem, void *data)
{
    return (((McapMclListEntry *)elem)->mclId == *((CsrUint16 *)data));
}

CsrBool CsrBtMcapUtilCompareMdlCid(CsrCmnListElm_t *elem, void *data)
{
    return (((McapMdlListEntry *)elem)->btConnId == *((CsrBtConnId *)data));
}

CsrBool CsrBtMcapUtilCompareMdlState(CsrCmnListElm_t *elem, void *data)
{
    return (((McapMdlListEntry *)elem)->mdlState == *((CsrUint8 *)data));
}



/********************************************************/
/*                                                      */
/*    MCAP message builder functions                    */
/*                                                      */
/********************************************************/

CsrUint8 *buildMdErrorRsp(void)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_ERROR_RSP_LEN);

    returnPtr[0] = MD_ERROR_RSP;
    returnPtr[1] = CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_OPCODE;
    returnPtr[2] = 0x00;
    returnPtr[3] = 0x00;

    return returnPtr;
}

CsrUint8 *buildMdCreateMdlReq(McapMdlListEntry *currentMdlPtr)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_CREATE_MDL_REQ_LEN);

    returnPtr[0] = MD_CREATE_MDL_REQ;
    returnPtr[1] = (CsrUint8)(currentMdlPtr->mdlId / 0x100);
    returnPtr[2] = (CsrUint8)(currentMdlPtr->mdlId - (returnPtr[1] * 0x100));
    returnPtr[3] = currentMdlPtr->mdepId;
    returnPtr[4] = currentMdlPtr->configuration;

    return returnPtr;
}

CsrUint8 *buildMdCreateMdlRes(CsrUint8 responseCode, CsrUint16 mdlId, CsrUint8 resPar)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_CREATE_MDL_RSP_LEN);

    returnPtr[0] = MD_CREATE_MDL_RSP;
    returnPtr[1] = responseCode;
    returnPtr[2] = (CsrUint8)(mdlId / 0x100);
    returnPtr[3] = (CsrUint8)(mdlId - (returnPtr[2] * 0x100));
    returnPtr[4] = resPar;

    return returnPtr;
}

CsrUint8 *buildMdDeleteMdlReq(CsrUint16 mdlId)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_DELETE_MDL_REQ_LEN);

    returnPtr[0] = MD_DELETE_MDL_REQ;
    returnPtr[1] = (CsrUint8)(mdlId / 0x100);
    returnPtr[2] = (CsrUint8)(mdlId - (returnPtr[1] * 0x100));

    return returnPtr;
}

CsrUint8 *buildMdDeleteMdlRes(CsrUint8 responseCode, CsrUint16 mdlId)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_DELETE_MDL_RSP_LEN);

    returnPtr[0] = MD_DELETE_MDL_RSP;
    returnPtr[1] = responseCode;
    returnPtr[2] = (CsrUint8)(mdlId / 0x100);
    returnPtr[3] = (CsrUint8)(mdlId - (returnPtr[1] * 0x100));

    return returnPtr;
}

CsrUint8 *buildMdAbortMdlReq(CsrUint16 mdlId)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_ABORT_MDL_REQ_LEN);

    returnPtr[0] = MD_ABORT_MDL_REQ;
    returnPtr[1] = (CsrUint8)(mdlId / 0x100);
    returnPtr[2] = (CsrUint8)(mdlId - (returnPtr[1] * 0x100));

    return returnPtr;
}

CsrUint8 *buildMdAbortMdlRes(CsrUint8 responseCode, CsrUint16 mdlId)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_ABORT_MDL_RSP_LEN);

    returnPtr[0] = MD_ABORT_MDL_RSP;
    returnPtr[1] = responseCode;
    returnPtr[2] = (CsrUint8)(mdlId / 0x100);
    returnPtr[3] = (CsrUint8)(mdlId - (returnPtr[1] * 0x100));

    return returnPtr;
}

CsrUint8 *buildMdReconnectMdlReq(McapMdlListEntry *currentMdlPtr)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_RECONNECT_MDL_REQ_LEN);

    returnPtr[0] = MD_RECONNECT_MDL_REQ;
    returnPtr[1] = (CsrUint8)(currentMdlPtr->mdlId / 0x100);
    returnPtr[2] = (CsrUint8)(currentMdlPtr->mdlId - (returnPtr[1] * 0x100));

    return returnPtr;
}

CsrUint8 *buildMdReconnectMdlRes(CsrUint8 responseCode, CsrUint16 mdlId)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_RECONNECT_MDL_RSP_LEN);

    returnPtr[0] = MD_RECONNECT_MDL_RSP;
    returnPtr[1] = responseCode;
    returnPtr[2] = (CsrUint8)(mdlId / 0x100);
    returnPtr[3] = (CsrUint8)(mdlId - (returnPtr[1] * 0x100));

    return returnPtr;
}

CsrUint8 *buildMdSyncCapReq(CsrUint16 timeStampReqAcc)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_SYNC_CAP_REQ_LEN);

    returnPtr[0] = MD_SYNC_CAP_REQ;
    returnPtr[1] = (CsrUint8)(timeStampReqAcc / 0x100);
    returnPtr[2] = (CsrUint8)(timeStampReqAcc - (returnPtr[1] * 0x100));

    return returnPtr;
}

CsrUint8 *buildMdSyncCapRes(CsrUint8    responseCode,
                         CsrUint8    btClkAccRes,
                         CsrUint16   syncLeadTime,
                         CsrUint16   timeStampNatRes,
                         CsrUint16   timeStampNatAcc)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_SYNC_CAP_RSP_LEN);

    returnPtr[0] = MD_SYNC_CAP_RSP;
    returnPtr[1] = responseCode;
    returnPtr[2] = btClkAccRes;
    returnPtr[3] = (CsrUint8)(syncLeadTime / 0x100);
    returnPtr[4] = (CsrUint8)(syncLeadTime - (returnPtr[3] * 0x100));
    returnPtr[5] = (CsrUint8)(timeStampNatRes / 0x100);
    returnPtr[6] = (CsrUint8)(timeStampNatRes - (returnPtr[5] * 0x100));
    returnPtr[7] = (CsrUint8)(timeStampNatAcc / 0x100);
    returnPtr[8] = (CsrUint8)(timeStampNatAcc - (returnPtr[7] * 0x100));

    return returnPtr;
}

CsrUint8 *buildMdSyncSetReq(CsrBool     timeStampUpdateInfo,
                         CsrUint32   btClkSyncTime,
                         CsrUint32   timeStampSyncTimeLsb,
                         CsrUint32   timeStampSyncTimeMsb)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_SYNC_SET_REQ_LEN);

    returnPtr[0]  = MD_SYNC_SET_REQ;
    returnPtr[1]  = (CsrUint8)timeStampUpdateInfo;
    returnPtr[2]  = (CsrUint8)(btClkSyncTime / 0x1000000);
    returnPtr[3]  = (CsrUint8)((btClkSyncTime - (returnPtr[2] * 0x1000000)) / 0x10000);
    returnPtr[4]  = (CsrUint8)((btClkSyncTime - (returnPtr[2] * 0x1000000) - (returnPtr[3] * 0x10000)) / 0x100);
    returnPtr[5]  = (CsrUint8)((btClkSyncTime - (returnPtr[2] * 0x1000000) - (returnPtr[3] * 0x10000) - (returnPtr[4] * 0x100)));
    returnPtr[6]  = (CsrUint8)(timeStampSyncTimeLsb / 0x1000000);
    returnPtr[7]  = (CsrUint8)((timeStampSyncTimeLsb - (returnPtr[6] * 0x1000000)) / 0x10000);
    returnPtr[8]  = (CsrUint8)((timeStampSyncTimeLsb - (returnPtr[6] * 0x1000000) - (returnPtr[7] * 0x10000)) / 0x100);
    returnPtr[9]  = (CsrUint8)((timeStampSyncTimeLsb - (returnPtr[6] * 0x1000000) - (returnPtr[7] * 0x10000) - (returnPtr[8] * 0x100)));
    returnPtr[10] = (CsrUint8)(timeStampSyncTimeMsb / 0x1000000);
    returnPtr[11] = (CsrUint8)((timeStampSyncTimeMsb - (returnPtr[10] * 0x1000000)) / 0x10000);
    returnPtr[12] = (CsrUint8)((timeStampSyncTimeMsb - (returnPtr[10] * 0x1000000) - (returnPtr[11] * 0x10000)) / 0x100);
    returnPtr[13] = (CsrUint8)((timeStampSyncTimeMsb - (returnPtr[10] * 0x1000000) - (returnPtr[11] * 0x10000) - (returnPtr[12] * 0x100)));

    return returnPtr;
}

CsrUint8 *buildMdSyncSetRes(CsrUint8  responseCode,
                         CsrUint32 btClkSyncTime,
                         CsrUint32 timeStampSyncTimeLsb,
                         CsrUint32 timeStampSyncTimeMsb,
                         CsrUint16 timeStampSampleAccuracy)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_SYNC_SET_RSP_LEN);

    returnPtr[0] = MD_SYNC_SET_RSP;
    returnPtr[1] = responseCode;

    returnPtr[2]  = (CsrUint8)(btClkSyncTime / 0x1000000);
    returnPtr[3]  = (CsrUint8)((btClkSyncTime - (returnPtr[2] * 0x1000000)) / 0x10000);
    returnPtr[4]  = (CsrUint8)((btClkSyncTime - (returnPtr[2] * 0x1000000) - (returnPtr[3] * 0x10000)) / 0x100);
    returnPtr[5]  = (CsrUint8)((btClkSyncTime - (returnPtr[2] * 0x1000000) - (returnPtr[3] * 0x10000) - (returnPtr[4] * 0x100)));
    returnPtr[6]  = (CsrUint8)(timeStampSyncTimeLsb / 0x1000000);
    returnPtr[7]  = (CsrUint8)((timeStampSyncTimeLsb - (returnPtr[6] * 0x1000000)) / 0x10000);
    returnPtr[8]  = (CsrUint8)((timeStampSyncTimeLsb - (returnPtr[6] * 0x1000000) - (returnPtr[7] * 0x10000)) / 0x100);
    returnPtr[9]  = (CsrUint8)((timeStampSyncTimeLsb - (returnPtr[6] * 0x1000000) - (returnPtr[7] * 0x10000) - (returnPtr[8] * 0x100)));
    returnPtr[10] = (CsrUint8)(timeStampSyncTimeMsb / 0x1000000);
    returnPtr[11] = (CsrUint8)((timeStampSyncTimeMsb - (returnPtr[10] * 0x1000000)) / 0x10000);
    returnPtr[12] = (CsrUint8)((timeStampSyncTimeMsb - (returnPtr[10] * 0x1000000) - (returnPtr[11] * 0x10000)) / 0x100);
    returnPtr[13] = (CsrUint8)((timeStampSyncTimeMsb - (returnPtr[10] * 0x1000000) - (returnPtr[11] * 0x10000) - (returnPtr[12] * 0x100)));
    returnPtr[14] = (CsrUint8)(timeStampSampleAccuracy / 0x100);
    returnPtr[15] = (CsrUint8)(timeStampSampleAccuracy - (returnPtr[14] * 0x100));

    return returnPtr;
}

CsrUint8 *buildMdSyncInfoInd(CsrUint32 btClkSyncTime,
                          CsrUint32 timeStampSyncTimeLsb,
                          CsrUint32 timeStampSyncTimeMsb,
                          CsrUint16 timeStampSampleAccuracy)
{
    CsrUint8 *returnPtr = (CsrUint8 *)CsrPmemAlloc(MD_SYNC_INFO_IND_LEN);

    returnPtr[0] = MD_SYNC_INFO_IND;
    returnPtr[1]  = (CsrUint8)(btClkSyncTime / 0x1000000);
    returnPtr[2]  = (CsrUint8)((btClkSyncTime - (returnPtr[1] * 0x1000000)) / 0x10000);
    returnPtr[3]  = (CsrUint8)((btClkSyncTime - (returnPtr[1] * 0x1000000) - (returnPtr[2] * 0x10000)) / 0x100);
    returnPtr[4]  = (CsrUint8)((btClkSyncTime - (returnPtr[1] * 0x1000000) - (returnPtr[2] * 0x10000) - (returnPtr[3] * 0x100)));
    returnPtr[5]  = (CsrUint8)(timeStampSyncTimeLsb / 0x1000000);
    returnPtr[6]  = (CsrUint8)((timeStampSyncTimeLsb - (returnPtr[5] * 0x1000000)) / 0x10000);
    returnPtr[7]  = (CsrUint8)((timeStampSyncTimeLsb - (returnPtr[5] * 0x1000000) - (returnPtr[6] * 0x10000)) / 0x100);
    returnPtr[8]  = (CsrUint8)((timeStampSyncTimeLsb - (returnPtr[5] * 0x1000000) - (returnPtr[6] * 0x10000) - (returnPtr[7] * 0x100)));
    returnPtr[9]  = (CsrUint8)(timeStampSyncTimeMsb / 0x1000000);
    returnPtr[10] = (CsrUint8)((timeStampSyncTimeMsb - (returnPtr[9] * 0x1000000)) / 0x10000);
    returnPtr[11] = (CsrUint8)((timeStampSyncTimeMsb - (returnPtr[9] * 0x1000000) - (returnPtr[10] * 0x10000)) / 0x100);
    returnPtr[12] = (CsrUint8)((timeStampSyncTimeMsb - (returnPtr[9] * 0x1000000) - (returnPtr[10] * 0x10000) - (returnPtr[11] * 0x100)));
    returnPtr[13] = (CsrUint8)(timeStampSampleAccuracy / 0x100);
    returnPtr[14] = (CsrUint8)(timeStampSampleAccuracy - (returnPtr[13] * 0x100));

    return returnPtr;
}

/********************************************************/
/*                                                      */
/*    Other functions                                   */
/*                                                      */
/********************************************************/
void CsrBtMcapFreeUpstreamMessageContents(void *msg_data)
{
    McapPrimType *type = msg_data;

    switch (*type)
    {
        case CSR_BT_MCAP_CREATE_MDL_RES:
            {
                CsrBtMcapCreateMdlRes *prim = msg_data;
                CsrPmemFree(prim->qos);
                CsrPmemFree(prim->flow);
                break;
            }
        case CSR_BT_MCAP_RECONNECT_MDL_RES:
            {
                CsrBtMcapReconnectMdlRes *prim = msg_data;
                CsrPmemFree(prim->qos);
                CsrPmemFree(prim->flow);
                break;
            }
        case CSR_BT_MCAP_CONNECT_MDL_REQ:
            {
                CsrBtMcapConnectMdlReq *prim = msg_data;
                CsrPmemFree(prim->qos);
                CsrPmemFree(prim->flow);
                break;
            }
        case CSR_BT_MCAP_DATA_SEND_REQ:
            {
                CsrBtMcapDataSendReq *prim = msg_data;
                CsrPmemFree(prim->payload);
                break;
            }
    }
}

CsrBool CsrBtMcapIsDisconnectAbnormal(CsrBtResultCode resultCode, CsrBtSupplier supplier)
{
    CsrBool result = FALSE;

    if (supplier == CSR_BT_SUPPLIER_HCI)
    {
        result = TRUE;
    }
    if (supplier == CSR_BT_SUPPLIER_L2CAP_DISCONNECT &&
        resultCode == L2CA_RESULT_LINK_LOSS)
    {
        result = TRUE;
    }

    return result;
}

#endif

