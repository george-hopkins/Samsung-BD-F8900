/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE

#include "csr_bt_util.h"
#include "csr_bt_avrcp_main.h"
#include "csr_bt_avrcp_prim.h"
#include "csr_bt_avrcp_lib.h"

void CsrBtAvrcpTgMpRegisterReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgMpRegisterReq *prim = (CsrBtAvrcpTgMpRegisterReq *)instData->recvMsgP;
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    CsrBtAvrcpTgMp *mp = CsrBtAvrcpTgUtilMpListAdd(instData, prim);
    if (!instData->tgLocal.mpDefault ||
        CSR_MASK_IS_SET(prim->configMask, CSR_BT_AVRCP_TG_MP_REGISTER_CONFIG_SET_DEFAULT))
    {/* No default player is set or this one should be set instead of the current one */
        instData->tgLocal.mpDefault = mp;
    }

    CsrBtAvrcpTgUtilAvailableMPChanged(instData);
#else
    CsrBtAvrcpTgMp *mp = (CsrBtAvrcpTgMp *) CsrPmemAlloc(sizeof(CsrBtAvrcpTgMp));
    mp->mpHandle            = prim->playerHandle;
    mp->mpId                = 0;
    mp->majorType           = prim->majorType;
    mp->subType             = prim->subType;
    mp->playerName          = prim->playerName;
    CsrMemCpy(mp->featureMask, prim->featureMask, sizeof(prim->featureMask));

    instData->tgLocal.mpDefault = mp;
#endif

    CsrBtAvrcpTgMpRegisterCfmSend(mp->mpId, prim->playerHandle,
        CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
        CSR_BT_SUPPLIER_AVRCP);
}

void CsrBtAvrcpTgMpUnregisterReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgMpUnregisterReq *prim = (CsrBtAvrcpTgMpUnregisterReq *)instData->recvMsgP;
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    CsrBtAvrcpTgMp *mp = AVRCP_LIST_TG_MP_GET_ID(&instData->tgLocal.mpList, prim->playerId);
#else
    CsrBtAvrcpTgMp *mp = instData->tgLocal.mpDefault;
#endif
    if (mp)
    {/* The currently addressed player must not be removed - check if is addressed by a remote device */
        AvrcpConnInstance_t *connInst;

        for (connInst = AVRCP_LIST_CONN_GET_FIRST(&instData->connList); connInst; connInst = connInst->next)
        {
            if (connInst->tgLocal.mpAddressed == mp)
            {
                CsrBtAvrcpTgMpUnregisterCfmSend(prim->playerId,
                    prim->phandle,
                    CSR_BT_RESULT_CODE_AVRCP_INVALID_PARAMETER,
                    CSR_BT_SUPPLIER_AVRCP);
                return;
            }
        }

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
        CsrBtAvrcpTgUtilAvailableMPChanged(instData);
#endif
        CsrBtAvrcpTgMpUnregisterCfmSend(prim->playerId,
            prim->phandle,
            CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
            CSR_BT_SUPPLIER_AVRCP);

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
        AVRCP_LIST_TG_MP_REMOVE(&instData->tgLocal.mpList, mp);

        if (instData->tgLocal.mpDefault == mp)
        {/* The default player is being removed - set the first player as default */
            instData->tgLocal.mpDefault = AVRCP_LIST_TG_MP_GET_FIRST(&instData->tgLocal.mpList);
        }
#else
        CsrPmemFree(mp->playerName);
        mp->playerName = NULL;
        CsrPmemFree(instData->tgLocal.mpDefault);
        instData->tgLocal.mpDefault=NULL;
#endif
    }
    else
    {
        CsrBtAvrcpTgMpUnregisterCfmSend(prim->playerId,
            prim->phandle,
            CSR_BT_RESULT_CODE_AVRCP_INVALID_PARAMETER,
            CSR_BT_SUPPLIER_AVRCP);
    }
}

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
void CsrBtAvrcpTgNotiReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgNotiReq *reqMsg = (CsrBtAvrcpTgNotiReq *)instData->recvMsgP;
    AvrcpConnInstance_t *connInst;
    CsrBtAvrcpTgMp *mp = AVRCP_LIST_TG_MP_GET_ID(&instData->tgLocal.mpList, reqMsg->playerId);
    
    if (reqMsg->notiId == CSR_BT_AVRCP_NOTI_ID_UIDS)
    {
        instData->tgLocal.uidCount = CSR_GET_UINT16_FROM_BIG_ENDIAN(reqMsg->notiData);
    }
    
    if (mp)
    {
        for (connInst = AVRCP_LIST_CONN_GET_FIRST(&instData->connList); connInst; connInst = connInst->next)
        {
            if (CSR_BIT_IS_SET(connInst->tgLocal.notificationsActive, reqMsg->notiId - CSR_BT_AVRCP_NOTI_ID_OFFSET) &&
                (connInst->control.state == AVRCP_STATE_CONN_CONNECTED))
            {/* The remote controller has requested a changed event for the notification */
                CsrBtAvrcpTgRegisterNotificationRspSend(connInst,
                                                   reqMsg->notiId,
                                                   reqMsg->notiData,
                                                   CSR_BT_AVRCP_DATA_AVC_RTYPE_CHANGED, FALSE);

                CSR_BIT_UNSET(connInst->tgLocal.notificationsActive, reqMsg->notiId - CSR_BT_AVRCP_NOTI_ID_OFFSET);
            }
        }

        CsrBtAvrcpTgNotiCfmSend(reqMsg->playerId, mp->mpHandle, reqMsg->notiId,
                CSR_BT_RESULT_CODE_AVRCP_SUCCESS, CSR_BT_SUPPLIER_AVRCP);
    }
}

void CsrBtAvrcpTgPasSetReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgPasSetReq *reqMsg = (CsrBtAvrcpTgPasSetReq *)instData->recvMsgP;
    AvrcpConnInstance_t *connInst, *currentConnInst = NULL;
    CsrUint32 playerId = reqMsg->playerId;

    for (connInst = AVRCP_LIST_CONN_GET_FIRST(&instData->connList); connInst; connInst = connInst->next)
    {
        if (CSR_MASK_IS_SET(connInst->tgLocal.notificationsActive, CSR_BT_AVRCP_NOTI_FLAG_PAS) &&
            (connInst->control.state == AVRCP_STATE_CONN_CONNECTED))
        {/* The remote controller requests a changed event for the PAS */
            currentConnInst = connInst;
            CsrBtAvrcpTgRegisterNotificationPasRspSend(connInst, CSR_BT_AVRCP_DATA_AVC_RTYPE_CHANGED, reqMsg->attValPairCount, reqMsg->attValPair);
            CSR_MASK_UNSET(connInst->tgLocal.notificationsActive, CSR_BT_AVRCP_NOTI_FLAG_PAS);
        }
    }

    if (currentConnInst != NULL)
    {
        playerId = currentConnInst->appConnId;
    }
    CsrBtAvrcpTgPasSetCfmSend(playerId,reqMsg->phandle,
        CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
        CSR_BT_SUPPLIER_AVRCP);
    CsrPmemFree(reqMsg->attValPair);
}

void CsrBtAvrcpTgSetAddressedPlayerReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgSetAddressedPlayerReq *reqMsg = (CsrBtAvrcpTgSetAddressedPlayerReq *)instData->recvMsgP;
    CsrBtAvrcpTgMp *mp = AVRCP_LIST_TG_MP_GET_ID(&instData->tgLocal.mpList, reqMsg->playerId);
 
    instData->tgLocal.uidCount = reqMsg->uidCounter;
    
    if (mp)
    {
        CsrBtAvrcpTgUtilMpListUpdateAddressed(instData, mp, reqMsg->uidCounter);
        CsrBtAvrcpTgSetAddressedPlayerCfmSend(mp->mpId, reqMsg->phandle,
            CSR_BT_RESULT_CODE_AVRCP_SUCCESS,
            CSR_BT_SUPPLIER_AVRCP);
    }
    else
    {
        CsrBtAvrcpTgSetAddressedPlayerCfmSend(reqMsg->playerId, reqMsg->phandle,
            CSR_BT_RESULT_CODE_AVRCP_INVALID_PARAMETER,
            CSR_BT_SUPPLIER_AVRCP);
    }
}

void CsrBtAvrcpTgNotiResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgNotiRes     *resMsg         = (CsrBtAvrcpTgNotiRes *)instData->recvMsgP;
    AvrcpConnInstance_t     *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    if (resMsg->notiId == CSR_BT_AVRCP_NOTI_ID_UIDS)
    {
        instData->tgLocal.uidCount = CSR_GET_UINT16_FROM_BIG_ENDIAN(resMsg->notiData);
    }

    if (pendingMsgInfo)
    {
        if (resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            CsrUint8 notiIndex = pendingMsgInfo->rxData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX] - CSR_BT_AVRCP_NOTI_ID_OFFSET;

            CSR_BIT_SET(connInst->tgLocal.notificationsActive, notiIndex);
            connInst->tgLocal.notiList[notiIndex] = AVRCP_TLABEL_GET(pendingMsgInfo->rxData);

            CsrBtAvrcpTgRegisterNotificationRspSend(connInst,
                                               pendingMsgInfo->rxData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX],
                                               &resMsg->notiData[0],
                                               CSR_BT_AVRCP_DATA_AVC_RTYPE_INTERIM, FALSE);
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, resMsg->status);
        }

        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
    else if (resMsg->status == CSR_BT_AVRCP_STATUS_ADDR_PLAYER_CHANGED)
    {
        CsrBtAvrcpTgRegisterNotificationRspSend(connInst,
                                                resMsg->notiId,
                                                resMsg->notiData,
                                                CSR_BT_AVRCP_DATA_AVC_RTYPE_CHANGED, FALSE);
    }
}

void CsrBtAvrcpTgPlayResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgPlayRes     *resMsg         = (CsrBtAvrcpTgPlayRes *)instData->recvMsgP;
    AvrcpConnInstance_t     *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    if (pendingMsgInfo)
    {
        if (resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            CsrBtAvrcpTgPlayRspSend(connInst, AVRCP_TLABEL_GET(pendingMsgInfo->rxData), resMsg->status);
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, resMsg->status);
        }

        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
}

void CsrBtAvrcpTgSearchResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgSearchRes   *resMsg         = (CsrBtAvrcpTgSearchRes *)instData->recvMsgP;
    AvrcpConnInstance_t     *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    instData->tgLocal.uidCount = resMsg->uidCounter;

    if (pendingMsgInfo)
    {
        if (resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            CsrBtAvrcpTgSearchRspSend(connInst, AVRCP_TLABEL_GET(pendingMsgInfo->rxData), resMsg->status, resMsg->uidCounter, resMsg->numberOfItems);
        }
        else
        {
            CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, pendingMsgInfo->rxData, resMsg->status);
        }

        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
}

void CsrBtAvrcpTgChangePathResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgChangePathRes  *resMsg         = (CsrBtAvrcpTgChangePathRes *)instData->recvMsgP;
    AvrcpConnInstance_t         *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t     *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    if (pendingMsgInfo)
    {
        if (resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            CsrBtAvrcpTgChangePathRspSend(connInst, AVRCP_TLABEL_GET(pendingMsgInfo->rxData), resMsg->status, resMsg->itemsCount);
        }
        else
        {
            CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, pendingMsgInfo->rxData, resMsg->status);
        }

        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
}
#endif

void CsrBtAvrcpTgPassThroughResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgPassThroughRes  *resMsg         = (CsrBtAvrcpTgPassThroughRes *)instData->recvMsgP;
    AvrcpConnInstance_t         *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t     *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    if (pendingMsgInfo)
    {
        if ((pendingMsgInfo->msgType == CSR_BT_AVRCP_TG_PASS_THROUGH_RES) && (resMsg->status == CSR_BT_AVRCP_PT_STATUS_ACCEPT))
        {
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
            if (AVRCP_DATA_PT_OPID_GET(pendingMsgInfo->rxData) == CSR_BT_AVRCP_PT_OP_ID_VENDOR_DEP)
            {/* Group navigation */
                CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_PT_GN_SIZE);

                CsrBtAvrcpDataInsertAvctpHeader(txData,
                                           AVRCP_TLABEL_GET(pendingMsgInfo->rxData),
                                           AVRCP_DATA_AVCTP_CR_RES);

                CsrBtAvrcpDataInsertAvcCommonHeader(txData, CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED);

                CsrBtAvrcpDataInsertAvcGroupNavigationHeader(txData,
                                                             AVRCP_DATA_PT_STATE_GET(pendingMsgInfo->rxData),
                                                             (CsrUint16)AVRCP_DATA_PT_GN_OPERATION_GET(pendingMsgInfo->rxData));

                CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_PT_GN_SIZE, txData);
            }
            else
#endif
            {/* Ordinary pass-through */
                CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_PT_SIZE);

                CsrBtAvrcpDataInsertAvctpHeader(txData,
                                           AVRCP_TLABEL_GET(pendingMsgInfo->rxData),
                                           AVRCP_DATA_AVCTP_CR_RES);

                CsrBtAvrcpDataInsertAvcCommonHeader(txData, CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED);

                CsrBtAvrcpDataInsertAvcPassThroughHeader(txData,
                                                    AVRCP_DATA_PT_STATE_GET(pendingMsgInfo->rxData),
                                                    AVRCP_DATA_PT_OPID_GET(pendingMsgInfo->rxData));

                CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_PT_SIZE, txData);
            }
        }
        else
        {
            if (resMsg->status == CSR_BT_AVRCP_PT_STATUS_NOT_IMPL)
            {
                CsrBtAvrcpTgRejectAvcSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_NOT_IMP);
            }
            else
            {
                CsrBtAvrcpTgRejectAvcSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED);
            }
        }
        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
}

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
void CsrBtAvrcpTgPasSetResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgPasSetRes  *resMsg         = (CsrBtAvrcpTgPasSetRes *)instData->recvMsgP;
    AvrcpConnInstance_t     *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    if (pendingMsgInfo)
    {
        if (resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            CsrBtAvrcpDataSimpleVendorFrameSend(connInst,
                                           AVRCP_TLABEL_GET(pendingMsgInfo->rxData),
                                           AVRCP_DATA_AVCTP_CR_RES,
                                           CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED,
                                           AVRCP_DATA_PDU_ID_SET_PAS_VALUES);
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, resMsg->status);
        }

        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
}

void CsrBtAvrcpTgPasCurrentResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgPasCurrentRes  *resMsg         = (CsrBtAvrcpTgPasCurrentRes *)instData->recvMsgP;
    AvrcpConnInstance_t         *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t     *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    if (pendingMsgInfo)
    {
        if (pendingMsgInfo->rxData[AVRCP_DATA_MD_PDU_ID_INDEX] == AVRCP_DATA_PDU_ID_REG_NOTI)
        {
            CsrUint8 notiIndex = pendingMsgInfo->rxData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX] - CSR_BT_AVRCP_NOTI_ID_OFFSET;

            CSR_BIT_SET(connInst->tgLocal.notificationsActive, notiIndex);
            connInst->tgLocal.notiList[notiIndex] = AVRCP_TLABEL_GET(pendingMsgInfo->rxData);

            CsrBtAvrcpTgRegisterNotificationPasRspSend(connInst, CSR_BT_AVRCP_DATA_AVC_RTYPE_INTERIM, resMsg->attValPairCount, resMsg->attValPair);
        }
        else if (pendingMsgInfo->rxData[AVRCP_DATA_MD_PDU_ID_INDEX] == AVRCP_DATA_PDU_ID_GET_CUR_PAS_VALUES)
        {/* Send response */
            CsrUint8 *txData   = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_HEADER_SIZE + (resMsg->attValPairCount*sizeof(CsrBtAvrcpPasAttValPair)));

            CsrBtAvrcpDataVendorDataInsert(txData,                              /* Data to transmit */
                                      AVRCP_TLABEL_GET(pendingMsgInfo->rxData), /* Transaction label */
                                      AVRCP_DATA_AVCTP_CR_RES,                  /* Command or response */
                                      CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE,       /* Command/response type */
                                      AVRCP_DATA_PDU_ID_GET_CUR_PAS_VALUES,     /* PDU ID */
                                      (CsrUint16)(AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_HEADER_SIZE + (resMsg->attValPairCount*sizeof(CsrBtAvrcpPasAttValPair)))); /* MD parameter length */

            txData[AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_NUM_INDEX] = resMsg->attValPairCount;
            CsrMemCpy(&txData[AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_ATT_ID_INDEX],resMsg->attValPair,resMsg->attValPairCount*sizeof(CsrBtAvrcpPasAttValPair));

            CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_HEADER_SIZE + (resMsg->attValPairCount*sizeof(CsrBtAvrcpPasAttValPair))), txData);
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, resMsg->status);
        }
        /* In any case, an answer has been issued, so remove the message from the pending list to avoid a second response */
        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }

    CsrPmemFree(resMsg->attValPair);
}

void CsrBtAvrcpTgSetVolumeResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgSetVolumeRes   *resMsg         = (CsrBtAvrcpTgSetVolumeRes *)instData->recvMsgP;
    AvrcpConnInstance_t         *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t     *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    if (pendingMsgInfo)
    {
        if (resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            CsrBtAvrcpTgSetVolumeRspSend(connInst, AVRCP_TLABEL_GET(pendingMsgInfo->rxData), resMsg->volume);
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, resMsg->status);
        }

        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
}

void CsrBtAvrcpTgGetFolderItemsResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgGetFolderItemsRes *resMsg         = (CsrBtAvrcpTgGetFolderItemsRes *)instData->recvMsgP;
    AvrcpConnInstance_t             *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t         *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    instData->tgLocal.uidCount = resMsg->uidCounter;
    
    if (pendingMsgInfo)
    {
        if (resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {/* resMsg->items* is already preformatted as a single packet AVCTP frame - insert remaining fields */
            CsrBtAvrcpTgGetFolderItemsRspSend(connInst,
                                         AVRCP_TLABEL_GET(pendingMsgInfo->rxData),
                                         resMsg->itemsLen,
                                         resMsg->items,
                                         resMsg->itemsCount,
                                         resMsg->uidCounter);
        }
        else
        {
            CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, pendingMsgInfo->rxData, resMsg->status);
            CsrPmemFree(resMsg->items);
        }

        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
    else
    {
        CsrPmemFree(resMsg->items);
    }
}

void CsrBtAvrcpTgGetAttributesResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgGetAttributesRes   *resMsg         = (CsrBtAvrcpTgGetAttributesRes *)instData->recvMsgP;
    AvrcpConnInstance_t             *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t         *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    if (pendingMsgInfo)
    {
        if (((resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE) ||
            (resMsg->status == CSR_BT_AVRCP_STATUS_UID_CHANGED)) && (resMsg->attribDataLen != 0))
        {
            CsrUint8 *pData;
            
            if (pendingMsgInfo->psm == CSR_BT_AVCTP_PSM)
            {/* GetElementAttributes (v1.3) */
                CsrUint8 headerSize = AVRCP_DATA_MD_HEADER_SIZE - 1;
                CsrBtAvrcpTgUtilContinuingRspReset(connInst, TRUE); /* Make sure continuation info is completely reset */
                /* The application does not allocate room for headers; make sure to do it here */
                resMsg->attribDataLen += headerSize; 
                pData = CsrPmemAlloc(resMsg->attribDataLen);
                CsrMemCpy(&pData[headerSize],resMsg->attribData,(resMsg->attribDataLen - headerSize));
                CsrPmemFree(resMsg->attribData);
                resMsg->attribData = pData; 

                connInst->tgLocal.pduId             = AVRCP_DATA_PDU_ID_GET_ELEMENT_ATTRIBUTES;
                connInst->tgLocal.getAttribResPrim  = resMsg;
                instData->recvMsgP                  = NULL;

                CsrBtAvrcpTgGetElementAttributesRspSend(connInst, AVRCP_TLABEL_GET(pendingMsgInfo->rxData));
            }
            else
            {/* GetItemAttributes (v1.4+) - is never fragmented */
                resMsg->attribDataLen += AVRCP_DATA_BROWSING_HEADER_SIZE;
                pData = CsrPmemAlloc(resMsg->attribDataLen);

                CsrMemCpy(&pData[AVRCP_DATA_BROWSING_HEADER_SIZE],resMsg->attribData,(resMsg->attribDataLen - AVRCP_DATA_BROWSING_HEADER_SIZE));

                CsrBtAvrcpTgGetItemAttributesRspSend(connInst,
                                                AVRCP_TLABEL_GET(pendingMsgInfo->rxData),
                                                resMsg->attribDataLen,
                                                pData,
                                                resMsg->attribCount,
                                                resMsg->status);
                CsrPmemFree(resMsg->attribData);
            }
        }
        else
        {
            if (pendingMsgInfo->psm == CSR_BT_AVCTP_PSM)
            {/* GetElementAttributes (v1.3) */
                CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, resMsg->status);
            }
            else
            {/* GetItemAttributes (v1.4+) */
                CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, pendingMsgInfo->rxData, resMsg->status);
            }
            CsrPmemFree(resMsg->attribData);
        }

        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
    else
    {
        CsrPmemFree(resMsg->attribData);
    }
}

void CsrBtAvrcpTgGetPlayStatusResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgGetPlayStatusRes  *resMsg         = (CsrBtAvrcpTgGetPlayStatusRes *)instData->recvMsgP;
    AvrcpConnInstance_t             *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t         *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    if (pendingMsgInfo)
    {
        CsrBool removePmsg = TRUE;

        if (resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            /* Determine if the response is for a GetPlayStatus or actually a GetFolderItems (Media Players) */
            if (connInst->tgLocal.currentPlayer)
            {/* GetFolderItems */
                connInst->tgLocal.currentPlayer->playStatus = resMsg->playStatus;

                /* Skip to next media player */
                connInst->tgLocal.currentPlayer = connInst->tgLocal.currentPlayer->next;
                connInst->tgLocal.itemCnt--;

                if ((connInst->tgLocal.currentPlayer) && (connInst->tgLocal.itemCnt > 0))
                {
                    removePmsg = FALSE;
                    CsrBtAvrcpTgUtilPendingMsgUpdate(pendingMsgInfo);
                    CsrBtAvrcpTgGetPlayStatusIndSend(pendingMsgInfo, connInst->tgLocal.currentPlayer);
                }
                else
                {/* Status has been retrieved for all players - send GetFolderItems response */
                    CsrUint8 *txData;
                    CsrUint16 txDataLen;
                    CsrUint32 startIdx = CSR_GET_UINT32_FROM_BIG_ENDIAN(&(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_GFI_CMD_START_INDEX]);
                    CsrUint32 endIdx = CSR_GET_UINT32_FROM_BIG_ENDIAN(&(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_GFI_CMD_END_INDEX]);
                    CsrUint32 totalCnt = (CsrUint32)(endIdx - startIdx + 1);

                    if ( (AVRCP_LIST_TG_MP_GET_COUNT(&connInst->instData->tgLocal.mpList) - startIdx) <= totalCnt )
                    {
                        totalCnt = (CsrUint16)(AVRCP_LIST_TG_MP_GET_COUNT(&connInst->instData->tgLocal.mpList) - startIdx);
                    }

                    CsrBtAvrcpTgUtilGetFolderItemsMPListBuild(instData, &txDataLen, &txData, startIdx, endIdx);
                    CsrBtAvrcpTgGetFolderItemsRspSend(connInst,
                                                 AVRCP_TLABEL_GET(pendingMsgInfo->rxData),
                                                 txDataLen,
                                                 txData,
                                                 (CsrUint16) totalCnt,
                                                 instData->tgLocal.uidCount);
                    connInst->tgLocal.currentPlayer = NULL;
                }
            }
            else
            {/* GetPlayStatus */
                CsrBtAvrcpTgGetPlayStatusRspSend(connInst,
                                            AVRCP_TLABEL_GET(pendingMsgInfo->rxData),
                                            resMsg->songLength,
                                            resMsg->songPosition,
                                            resMsg->playStatus);
            }
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, resMsg->status);
        }

        if (removePmsg)
        {
            AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
        }
    }
}

void CsrBtAvrcpTgSetAddressedPlayerResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgSetAddressedPlayerRes   *resMsg         = (CsrBtAvrcpTgSetAddressedPlayerRes *)instData->recvMsgP;
    AvrcpConnInstance_t                 *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t             *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;
 
    instData->tgLocal.uidCount = resMsg->uidCounter;
    
    if (pendingMsgInfo)
    {
        if (resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            CsrBtAvrcpTgMp *mp = AVRCP_LIST_TG_MP_GET_ID(&instData->tgLocal.mpList, resMsg->playerId);

            if (mp)
            {
                CsrBtAvrcpTgUtilMpListUpdateAddressed(instData, mp, resMsg->uidCounter);
                CsrBtAvrcpTgSetAddressedPlayerRspSend(connInst, AVRCP_TLABEL_GET(pendingMsgInfo->rxData), resMsg->status, CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED);
            }
            else
            {
                CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INTERNAL_ERROR);
            }
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, resMsg->status);
        }

        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
}

void CsrBtAvrcpTgSetBrowsedPlayerResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgSetBrowsedPlayerRes *resMsg         = (CsrBtAvrcpTgSetBrowsedPlayerRes *)instData->recvMsgP;
    AvrcpConnInstance_t             *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t         *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    instData->tgLocal.uidCount = resMsg->uidCounter;
    if (pendingMsgInfo)
    {
        if (resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            CsrBtAvrcpTgMp *mp = AVRCP_LIST_TG_MP_GET_ID(&instData->tgLocal.mpList, resMsg->playerId);

            if (mp)
            {
                CsrUint8 *data;
                CsrUint16 dataLen;

                CsrBtAvrcpTgUtilMpListUpdateBrowsed(connInst, mp);

                resMsg->folderDepth = CsrBtAvrcpTgUtilSBPFolderNamesAdd(&data,&dataLen,(CsrCharString *)(resMsg->folderNames));

                CsrBtAvrcpTgSetBrowsedPlayerRspSend(connInst,
                                               AVRCP_TLABEL_GET(pendingMsgInfo->rxData),
                                               resMsg->status,
                                               resMsg->uidCounter,
                                               resMsg->itemsCount,
                                               resMsg->folderDepth,
                                               dataLen,
                                               data);
                CsrPmemFree(data);
            }
            else
            {
                CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_STATUS_INTERNAL_ERROR);
            }
        }
        else
        {
            CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, pendingMsgInfo->rxData, resMsg->status);
        }

        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
    CsrPmemFree(resMsg->folderNames);

}

void CsrBtAvrcpTgAddToNowPlayingResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgAddToNowPlayingRes  *resMsg         = (CsrBtAvrcpTgAddToNowPlayingRes *)instData->recvMsgP;
    AvrcpConnInstance_t             *connInst       = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    AvrcpTgPendingMsgInfo_t         *pendingMsgInfo = connInst ? AVRCP_LIST_TG_PMSG_GET_MSGID(&connInst->tgLocal.pendingMsgList, resMsg->msgId) : NULL;

    if (pendingMsgInfo)
    {
        if (resMsg->status == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            CsrBtAvrcpTgAddToNPLRspSend(connInst, AVRCP_TLABEL_GET(pendingMsgInfo->rxData), resMsg->status);
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, resMsg->status);
        }

        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
    }
}
#endif



/***********

************/

void CsrBtAvrcpTgMpRegisterCfmSend(CsrUint32 playerId, CsrSchedQid phandle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtAvrcpTgMpRegisterCfm *prim = pnew(CsrBtAvrcpTgMpRegisterCfm);

    prim->type     = CSR_BT_AVRCP_TG_MP_REGISTER_CFM;
    prim->playerId = playerId;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtAvrcpMessagePut(phandle, prim);
}

void CsrBtAvrcpTgMpUnregisterCfmSend(CsrUint32 playerId, CsrSchedQid phandle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtAvrcpTgMpUnregisterCfm *prim = pnew(CsrBtAvrcpTgMpUnregisterCfm);

    prim->type     = CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM;
    prim->playerId = playerId;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtAvrcpMessagePut(phandle, prim);
}

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
void CsrBtAvrcpTgSetAddressedPlayerCfmSend(CsrUint32 playerId, CsrSchedQid phandle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtAvrcpTgSetAddressedPlayerCfm *prim = pnew(CsrBtAvrcpTgSetAddressedPlayerCfm);

    prim->type     = CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM;
    prim->playerId = playerId;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtAvrcpMessagePut(phandle, prim);
}

void CsrBtAvrcpTgNotiCfmSend(CsrUint32 playerId, CsrSchedQid phandle,
    CsrBtAvrcpNotiId notiType, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtAvrcpTgNotiCfm *prim = pnew(CsrBtAvrcpTgNotiCfm);

    prim->type          = CSR_BT_AVRCP_TG_NOTI_CFM;
    prim->playerId      = playerId;
    prim->notiId        = notiType;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtAvrcpMessagePut(phandle, prim);
}

void CsrBtAvrcpTgNotiIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 notiType, CsrUint32 playbackInterval)
{
    CsrBtAvrcpTgNotiInd *prim = pnew(CsrBtAvrcpTgNotiInd);

    prim->type          = CSR_BT_AVRCP_TG_NOTI_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = pendingMsgInfo->connInst->tgLocal.mpAddressed->mpId;
    prim->notiId        = notiType;
    prim->playbackInterval = playbackInterval;
    prim->msgId         = pendingMsgInfo->msgId;
    CsrBtAvrcpMessagePut(pendingMsgInfo->connInst->tgLocal.mpAddressed->mpHandle, prim);
}

void CsrBtAvrcpTgPasSetCfmSend(CsrUint32 playerId, CsrSchedQid phandle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtAvrcpTgPasSetCfm *prim = pnew(CsrBtAvrcpTgPasSetCfm);

    prim->type          = CSR_BT_AVRCP_TG_PAS_SET_CFM;
    prim->playerId      = playerId;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtAvrcpMessagePut(phandle, prim);
}

void CsrBtAvrcpTgPlayIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpScope scope, CsrUint16 uidCounter, CsrUint8 *uid)
{
    CsrBtAvrcpTgPlayInd *prim = pnew(CsrBtAvrcpTgPlayInd);

    prim->type          = CSR_BT_AVRCP_TG_PLAY_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = pendingMsgInfo->connInst->tgLocal.mpAddressed->mpId;
    prim->scope         = scope;
    prim->uidCounter    = uidCounter;
    prim->msgId         = pendingMsgInfo->msgId;
    CSR_BT_AVRCP_UID_COPY(prim->uid, uid);
    CsrBtAvrcpMessagePut(pendingMsgInfo->connInst->tgLocal.mpAddressed->mpHandle, prim);
}

void CsrBtAvrcpTgSearchIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrCharString *text)
{
    CsrBtAvrcpTgSearchInd *prim = pnew(CsrBtAvrcpTgSearchInd);

    prim->type          = CSR_BT_AVRCP_TG_SEARCH_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = pendingMsgInfo->connInst->tgLocal.mpAddressed->mpId;
    prim->text          = text;
    prim->msgId         = pendingMsgInfo->msgId;
    CsrBtAvrcpMessagePut(pendingMsgInfo->connInst->tgLocal.mpAddressed->mpHandle, prim);
}

void CsrBtAvrcpTgPasCurrentIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 attIdCount, CsrBtAvrcpPasAttId *attId)
{
    CsrBtAvrcpTgPasCurrentInd *prim = pnew(CsrBtAvrcpTgPasCurrentInd);

    prim->type          = CSR_BT_AVRCP_TG_PAS_CURRENT_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = pendingMsgInfo->connInst->tgLocal.mpAddressed->mpId;
    prim->attIdCount    = attIdCount;
    prim->attId         = attId;
    prim->msgId         = pendingMsgInfo->msgId;
    CsrBtAvrcpMessagePut(pendingMsgInfo->connInst->tgLocal.mpAddressed->mpHandle, prim);
}

void CsrBtAvrcpTgChangePathIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpFolderDirection folderDir, CsrUint8 *folderUid)
{
    CsrBtAvrcpTgChangePathInd *prim = pnew(CsrBtAvrcpTgChangePathInd);

    prim->type          = CSR_BT_AVRCP_TG_CHANGE_PATH_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = pendingMsgInfo->connInst->tgLocal.mpBrowsed->mpId;
    prim->folderDir     = folderDir;
    prim->msgId         = pendingMsgInfo->msgId;
    CSR_BT_AVRCP_UID_COPY(prim->folderUid, folderUid);
    CsrBtAvrcpMessagePut(pendingMsgInfo->connInst->tgLocal.mpAddressed->mpHandle, prim);
}
#endif

void CsrBtAvrcpTgPassThroughIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 opId, CsrUint8 state)
{
    CsrBtAvrcpTgPassThroughInd *prim = pnew(CsrBtAvrcpTgPassThroughInd);

    prim->type          = CSR_BT_AVRCP_TG_PASS_THROUGH_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = pendingMsgInfo->connInst->tgLocal.mpAddressed->mpId;
    prim->operationId   = opId;
    prim->state         = state;
    prim->msgId         = pendingMsgInfo->msgId;
    CsrBtAvrcpMessagePut(pendingMsgInfo->connInst->tgLocal.mpAddressed->mpHandle, prim);
}

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
void CsrBtAvrcpTgPasSetIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 attValPairCount, CsrBtAvrcpPasAttValPair* changedPas)
{
    CsrBtAvrcpTgPasSetInd *prim = pnew(CsrBtAvrcpTgPasSetInd);

    prim->type              = CSR_BT_AVRCP_TG_PAS_SET_IND;
    prim->connectionId      = pendingMsgInfo->connInst->appConnId;
    prim->playerId          = pendingMsgInfo->connInst->tgLocal.mpAddressed->mpId;
    prim->attValPairCount   = attValPairCount;
    prim->attValPair        = changedPas;
    prim->msgId             = pendingMsgInfo->msgId;
    CsrBtAvrcpMessagePut(pendingMsgInfo->connInst->tgLocal.mpAddressed->mpHandle, prim);
}

void CsrBtAvrcpTgSetVolumeIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 volume)
{
    CsrBtAvrcpTgSetVolumeInd *prim = pnew(CsrBtAvrcpTgSetVolumeInd);

    prim->type          = CSR_BT_AVRCP_TG_SET_VOLUME_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = pendingMsgInfo->connInst->tgLocal.mpAddressed->mpId;
    prim->volume        = volume;
    prim->msgId         = pendingMsgInfo->msgId;
    CsrBtAvrcpMessagePut(pendingMsgInfo->connInst->tgLocal.mpAddressed->mpHandle, prim);
}

void CsrBtAvrcpTgInformDispCharsetIndSend(CsrUint8 conId, CsrUint32 playerId, CsrSchedQid mpHandle, CsrUint8 charsetCount, CsrBtAvrcpCharSet *charset)
{
    CsrBtAvrcpTgInformDispCharsetInd *prim = pnew(CsrBtAvrcpTgInformDispCharsetInd);

    prim->type          = CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND;
    prim->connectionId  = conId;
    prim->playerId      = playerId;
    prim->charsetCount  = charsetCount;
    prim->charset       = charset;

    CsrBtAvrcpMessagePut(mpHandle, prim);
}

void CsrBtAvrcpTgBatteryStatusOfCtIndSend(CsrUint8 conId, CsrSchedQid mpHandle, CsrUint32 playerId, CsrUint8 battLevel)
{
    CsrBtAvrcpTgInformBatteryStatusInd *prim = pnew(CsrBtAvrcpTgInformBatteryStatusInd);

    prim->type          = CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND;
    prim->connectionId  = conId;
    prim->playerId      = playerId;
    prim->batStatus     = battLevel;

    CsrBtAvrcpMessagePut(mpHandle, prim);
}

void CsrBtAvrcpTgGetFolderItemsIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 scope, CsrUint32 startItem, CsrUint32 endItem, CsrUint32 attribMask, CsrUint32 maxData)
{
    CsrBtAvrcpTgGetFolderItemsInd *prim = pnew(CsrBtAvrcpTgGetFolderItemsInd);

    prim->type          = CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = pendingMsgInfo->connInst->tgLocal.mpBrowsed->mpId;
    prim->msgId         = pendingMsgInfo->msgId;
    prim->scope         = scope;
    prim->startItem     = startItem;
    prim->endItem       = endItem;
    prim->attributeMask = attribMask;
    prim->maxData       = maxData;
    CsrBtAvrcpMessagePut(pendingMsgInfo->connInst->tgLocal.mpBrowsed->mpHandle, prim);
}

void CsrBtAvrcpTgGetPlayStatusIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpTgMp *mp)
{
    CsrBtAvrcpTgGetPlayStatusInd *prim = pnew(CsrBtAvrcpTgGetPlayStatusInd);

    prim->type          = CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = mp->mpId;
    prim->msgId         = pendingMsgInfo->msgId;
    CsrBtAvrcpMessagePut(mp->mpHandle, prim);
}

void CsrBtAvrcpTgSetAddressedPlayerIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpTgMp *mp)
{
    CsrBtAvrcpTgSetAddressedPlayerInd *prim = pnew(CsrBtAvrcpTgSetAddressedPlayerInd);

    prim->type          = CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = mp->mpId;
    prim->msgId         = pendingMsgInfo->msgId;
    CsrBtAvrcpMessagePut(mp->mpHandle, prim);
}

void CsrBtAvrcpTgSetBrowsedPlayerIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpTgMp *mp)
{
    CsrBtAvrcpTgSetBrowsedPlayerInd *prim = pnew(CsrBtAvrcpTgSetBrowsedPlayerInd);

    prim->type          = CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = mp->mpId;
    prim->msgId         = pendingMsgInfo->msgId;
    prim->phandle       = mp->mpHandle;
    CsrBtAvrcpMessagePut(mp->mpHandle, prim);
}

void CsrBtAvrcpTgGetAttributesIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpTgMp *mp, CsrBtAvrcpItemAttMask attribMask, CsrUint32 maxData, CsrBtAvrcpScope scope, CsrBtAvrcpUid uid, CsrUint16 uidCounter)
{
    CsrBtAvrcpTgGetAttributesInd *prim = pnew(CsrBtAvrcpTgGetAttributesInd);

    prim->type          = CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = mp->mpId;
    prim->msgId         = pendingMsgInfo->msgId;
    prim->attributeMask = attribMask;
    prim->maxData       = maxData;
    prim->scope         = scope;
    prim->uidCounter    = uidCounter;
    CSR_BT_AVRCP_UID_COPY(prim->uid, uid);
    CsrBtAvrcpMessagePut(mp->mpHandle, prim);
}

void CsrBtAvrcpTgAddToNPLIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 scope, CsrUint16 uidCounter, CsrUint8 *uid)
{
    CsrBtAvrcpTgAddToNowPlayingInd *prim = pnew(CsrBtAvrcpTgAddToNowPlayingInd);

    prim->type          = CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND;
    prim->connectionId  = pendingMsgInfo->connInst->appConnId;
    prim->playerId      = pendingMsgInfo->connInst->tgLocal.mpBrowsed->mpId;
    prim->msgId         = pendingMsgInfo->msgId;
    prim->scope         = scope;
    prim->uidCounter    = uidCounter;
    CSR_BT_AVRCP_UID_COPY(prim->uid, uid);
    CsrBtAvrcpMessagePut(pendingMsgInfo->connInst->tgLocal.mpBrowsed->mpHandle, prim);
}

#endif /* CSR_BT_INSTALL_AVRCP_TG_13_14 */
#endif /* #ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE */
#endif

