/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_util.h"
#include "csr_bt_avrcp_main.h"
#include "csr_bt_avrcp_prim.h"
#include "csr_bt_avrcp_lib.h"

#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
void CsrBtAvrcpCtInformDispCharsetReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtInformDispCharsetReq *reqMsg = (CsrBtAvrcpCtInformDispCharsetReq *)instData->recvMsgP;
    CsrBtAvrcpCtInformDispCharsetCfm *cfmMsg = CsrBtAvrcpCtInformDispCharsetCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (reqMsg->charset == NULL)
    {
        reqMsg->charset = CsrPmemAlloc(sizeof(CsrBtAvrcpCharSet));
        *reqMsg->charset = CSR_BT_AVRCP_CHARACTER_SET_UTF_8;
    }

    if (reqMsg->charsetCount == 0)
    {
        reqMsg->charsetCount = 1;
    }

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtInformDispCharsetCmdSend(connInst, pendingMsgInfo, reqMsg->charsetCount, reqMsg->charset);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtNotiRegisterReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtNotiRegisterReq *reqMsg    = (CsrBtAvrcpCtNotiRegisterReq *)instData->recvMsgP;
    CsrBtAvrcpCtNotiRegisterCfm *cfmMsg    = CsrBtAvrcpCtNotiRegisterCfmBuild(reqMsg->connectionId, reqMsg->playbackInterval, reqMsg->notiMask);
    AvrcpConnInstance_t         *connInst  = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode             resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);

        connInst->ctLocal.ctRequestedNotifications  = reqMsg->notiMask;
        connInst->ctLocal.playbackInterval          = reqMsg->playbackInterval;
        connInst->ctLocal.notiConfig                = reqMsg->config;
        /* Start by determining which notifications the remote device supports */
        CsrBtAvrcpCtGetCapabilitiesCmdSend(connInst, pendingMsgInfo, AVRCP_DATA_PDU_GET_CAP_CMN_NOTI_SUP);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}
#endif
void CsrBtAvrcpCtPassThroughReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtPassThroughReq *reqMsg = (CsrBtAvrcpCtPassThroughReq *)instData->recvMsgP;
    CsrBtAvrcpCtPassThroughCfm *cfmMsg = CsrBtAvrcpCtPassThroughCfmBuild(reqMsg->connectionId, reqMsg->operationId, reqMsg->state);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_10, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
        CsrUint8 state;

        if (reqMsg->state == CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE)
        {
            state = CSR_BT_AVRCP_PT_STATE_PRESS;
            pendingMsgInfo->tmpState = AVRCP_STATE_PM_PT_RELEASE_PENDING;
        }
        else
        {
            state = reqMsg->state;
        }

        if ((reqMsg->operationId == CSR_BT_AVRCP_PT_OP_ID_GROUP_NAV_NEXT) ||
            (reqMsg->operationId == CSR_BT_AVRCP_PT_OP_ID_GROUP_NAV_PREV))
        {
            CsrBtAvrcpCtGroupNavigationCmdSend(connInst,
                                               pendingMsgInfo,
                                               (CsrUint16)(reqMsg->operationId == CSR_BT_AVRCP_PT_OP_ID_GROUP_NAV_NEXT ? AVRCP_DATA_PT_GN_OPERATION_NEXT : AVRCP_DATA_PT_GN_OPERATION_PREV),
                                               state);
        }
        else
#else
        if (reqMsg->state == CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE)
        {
            pendingMsgInfo->tmpState = AVRCP_STATE_PM_PT_RELEASE_PENDING;
        }
#endif
        {
            CsrBtAvrcpCtPassThroughCmdSend(connInst, pendingMsgInfo, reqMsg->operationId, reqMsg->state);
        }
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
void CsrBtAvrcpCtPasAttIdReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtPasAttIdReq *reqMsg = (CsrBtAvrcpCtPasAttIdReq *)instData->recvMsgP;
    CsrBtAvrcpCtPasAttIdCfm *cfmMsg = CsrBtAvrcpCtPasAttIdCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtPasListAttCmdSend(connInst, pendingMsgInfo);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtPasValIdReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtPasValIdReq *reqMsg = (CsrBtAvrcpCtPasValIdReq *)instData->recvMsgP;
    CsrBtAvrcpCtPasValIdCfm *cfmMsg = CsrBtAvrcpCtPasValIdCfmBuild(reqMsg->connectionId, reqMsg->attId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtPasListValCmdSend(connInst, pendingMsgInfo, reqMsg->attId);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtPasAttTxtReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtPasAttTxtReq *reqMsg = (CsrBtAvrcpCtPasAttTxtReq *)instData->recvMsgP;
    CsrBtAvrcpCtPasAttTxtCfm *cfmMsg = CsrBtAvrcpCtPasAttTxtCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtPasGetAttTextCmdSend(connInst, pendingMsgInfo, reqMsg->attIdCount, reqMsg->attId);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtPasAttTxtResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtPasAttTxtRes *reqMsg = (CsrBtAvrcpCtPasAttTxtRes *)instData->recvMsgP;
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        CsrBtAvrcpCtRequestAbortContinuationCmdSend(connInst, connInst->ctLocal.pendingMsgInfo, reqMsg->proceed, AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT);
    }
    else
    {/* Remote device have been disconnected - confirmation already sent */
    }
}

void CsrBtAvrcpCtPasValTxtReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtPasValTxtReq *reqMsg = (CsrBtAvrcpCtPasValTxtReq *)instData->recvMsgP;
    CsrBtAvrcpCtPasValTxtCfm *cfmMsg = CsrBtAvrcpCtPasValTxtCfmBuild(reqMsg->connectionId, reqMsg->attId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtPasGetValTextCmdSend(connInst, pendingMsgInfo, reqMsg->attId, reqMsg->valIdCount, reqMsg->valId);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtPasValTxtResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtPasValTxtRes *reqMsg = (CsrBtAvrcpCtPasValTxtRes *)instData->recvMsgP;
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        CsrBtAvrcpCtRequestAbortContinuationCmdSend(connInst, connInst->ctLocal.pendingMsgInfo, reqMsg->proceed, AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT);
    }
    else
    {/* Remote device have been disconnected - confirmation already sent */
    }
}

void CsrBtAvrcpCtPasCurrentReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtPasCurrentReq *reqMsg = (CsrBtAvrcpCtPasCurrentReq *)instData->recvMsgP;
    CsrBtAvrcpCtPasCurrentCfm *cfmMsg = CsrBtAvrcpCtPasCurrentCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtPasGetCurrentValCmdSend(connInst, pendingMsgInfo, reqMsg->attIdCount, reqMsg->attId);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtPasSetReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtPasSetReq *reqMsg = (CsrBtAvrcpCtPasSetReq *)instData->recvMsgP;
    CsrBtAvrcpCtPasSetCfm *cfmMsg = CsrBtAvrcpCtPasSetCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtPasSetValCmdSend(connInst, pendingMsgInfo, reqMsg->attValPairCount, reqMsg->attValPair);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtGetPlayStatusReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtGetPlayStatusReq *reqMsg = (CsrBtAvrcpCtGetPlayStatusReq *)instData->recvMsgP;
    CsrBtAvrcpCtGetPlayStatusCfm *cfmMsg = CsrBtAvrcpCtGetPlayStatusCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);

    /* XXX I HATE THIS API */
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtGetPlayStatusCmdSend(connInst, pendingMsgInfo);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtGetAttributesReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtGetAttributesReq *reqMsg = (CsrBtAvrcpCtGetAttributesReq *)instData->recvMsgP;
    CsrBtAvrcpCtGetAttributesCfm *cfmMsg = CsrBtAvrcpCtGetAttributesCfmBuild(reqMsg->connectionId, reqMsg->scope, reqMsg->uid);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);

    if (connInst)
    {
        /* Determine if GetElementAttributes or GetItemAttributes should be used */
        if ((instData->ctLocal.srAvrcpVersion >= CSR_BT_AVRCP_CONFIG_SR_VERSION_14) &&
            (connInst->ctLocal.tgSdpAvrcpVersion >= CSR_BT_AVRCP_CONFIG_SR_VERSION_14) &&
            CSR_MASK_IS_SET(connInst->ctLocal.tgSdpSupportedFeatures, CSR_BT_AVRCP_CONFIG_SR_FEAT_BROWSING))
        {/* GetItemAttributes */
            CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_14, FALSE);

            if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
                AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
                CsrBtAvrcpCtGetItemAttributesCmdSend(connInst,
                                                     pendingMsgInfo,
                                                     reqMsg->scope,
                                                     reqMsg->uidCounter,
                                                     &reqMsg->uid,
                                                     reqMsg->attributeMask);
            }
            else
            {
                cfmMsg->resultCode = resultCode;
                cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

                CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
            }
        }
        else
        {/* GetElementAttributes */
            CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);
            if (resultCode == CSR_BT_RESULT_CODE_AVRCP_INVALID_VERSION)
            {
                if ((CSR_MASK_IS_SET(connInst->ctLocal.tgSdpSupportedFeatures, CSR_BT_AVRCP_CONFIG_SR_FEAT_BROWSING) == FALSE) &&
                    (connInst->ctLocal.tgSdpAvrcpVersion >= CSR_BT_AVRCP_CONFIG_SR_VERSION_14))
                {/* The remote device claims support for rev 1.4 of the AVRCP spec, but no support for the browsing feature:
                    Issue the GetElementAttributes instead of the GetItemAttributes */
                    resultCode = CSR_BT_RESULT_CODE_AVRCP_SUCCESS;
                }
            }

            if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
                AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
                CsrBtAvrcpCtGetElementAttributesCmdSend(connInst, pendingMsgInfo, reqMsg->attributeMask);
            }
            else
            {
                cfmMsg->resultCode = resultCode;
                cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

                CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
            }
        }
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_DEVICE_NOT_CONNECTED;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtGetAttributesResHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtGetAttributesRes *resMsg = (CsrBtAvrcpCtGetAttributesRes *)instData->recvMsgP;
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, resMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        CsrBtAvrcpCtRequestAbortContinuationCmdSend(connInst, connInst->ctLocal.pendingMsgInfo, resMsg->proceed, AVRCP_DATA_PDU_ID_GET_ELEMENT_ATTRIBUTES);
    }
    else
    {/* Remote device have been disconnected - confirmation already sent */
    }
}

void CsrBtAvrcpCtGetFolderItemsReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtGetFolderItemsReq *reqMsg = (CsrBtAvrcpCtGetFolderItemsReq *)instData->recvMsgP;
    CsrBtAvrcpCtGetFolderItemsCfm *cfmMsg = CsrBtAvrcpCtGetFolderItemsCfmBuild(reqMsg->connectionId, reqMsg->scope,reqMsg->startItem,reqMsg->endItem);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_14, FALSE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_BROWSING_PSM);
        CsrBtAvrcpCtGetFolderItemsCmdSend(connInst, pendingMsgInfo, reqMsg->scope, reqMsg->startItem, reqMsg->endItem, reqMsg->attributeMask);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtPlayReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtPlayReq *reqMsg = (CsrBtAvrcpCtPlayReq *)instData->recvMsgP;
    CsrBtAvrcpCtPlayCfm *cfmMsg = CsrBtAvrcpCtPlayCfmBuild(reqMsg->connectionId, reqMsg->scope, &reqMsg->uid);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_14, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        CsrBtAvrcpUid uid;
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);

        CSR_BT_AVRCP_UID_COPY(uid, reqMsg->uid);
        CsrBtAvrcpCtPlayItemCmdSend(connInst, pendingMsgInfo, reqMsg->scope, reqMsg->uidCounter, &uid);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtSearchReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtSearchReq *reqMsg = (CsrBtAvrcpCtSearchReq *)instData->recvMsgP;
    CsrBtAvrcpCtSearchCfm *cfmMsg = CsrBtAvrcpCtSearchCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_14, FALSE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_BROWSING_PSM);
        CsrBtAvrcpCtSearchCmdSend(connInst, pendingMsgInfo, CSR_BT_AVRCP_CHARACTER_SET_UTF_8, reqMsg->text);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        CsrPmemFree(reqMsg->text);
        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtChangePathReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtChangePathReq *reqMsg = (CsrBtAvrcpCtChangePathReq *)instData->recvMsgP;
    CsrBtAvrcpCtChangePathCfm *cfmMsg = CsrBtAvrcpCtChangePathCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_14, FALSE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        CsrBtAvrcpUid uid;
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_BROWSING_PSM);
        if (reqMsg->folderDir == CSR_BT_AVRCP_CHANGE_PATH_UP)
        {/* According to 1.4 spec, if folder direction is "UP" then the folderUid is reserved; that is all zeros */
            CsrMemSet(uid,0,sizeof(CsrBtAvrcpUid));
        }
        else
        {
            CSR_BT_AVRCP_UID_COPY(uid, reqMsg->folderUid);
        }
        CsrBtAvrcpCtChangePathCmdSend(connInst, pendingMsgInfo, reqMsg->uidCounter, reqMsg->folderDir, &reqMsg->folderUid);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtSetVolumeReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtSetVolumeReq *reqMsg = (CsrBtAvrcpCtSetVolumeReq *)instData->recvMsgP;
    CsrBtAvrcpCtSetVolumeCfm *cfmMsg = CsrBtAvrcpCtSetVolumeCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_14, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtSetAbsoluteVolumeCmdSend(connInst, pendingMsgInfo, reqMsg->volume);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtSetSSVolumeReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtSetSSVolumeReq *reqMsg = (CsrBtAvrcpCtSetSSVolumeReq *)instData->recvMsgP;
    CsrBtAvrcpCtSetVolumeCfm *cfmMsg = CsrBtAvrcpCtSetVolumeCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_13, TRUE);
    printf("CsrBtAvrcpCtSetSSVolumeReqHandler %x\n",resultCode);
    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtSetAbsoluteVolumeSamsungCmdSend(connInst, pendingMsgInfo, reqMsg->volume,reqMsg->isMute);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}


void CsrBtAvrcpCtSetAddressedPlayerReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtSetAddressedPlayerReq *reqMsg = (CsrBtAvrcpCtSetAddressedPlayerReq *)instData->recvMsgP;
    CsrBtAvrcpCtSetAddressedPlayerCfm *cfmMsg = CsrBtAvrcpCtSetAddressedPlayerCfmBuild(reqMsg->connectionId, reqMsg->playerId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_14, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtSetAddressedPlayerCmdSend(connInst, pendingMsgInfo, (CsrUint16)reqMsg->playerId);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtSetBrowsedPlayerReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtSetBrowsedPlayerReq *reqMsg = (CsrBtAvrcpCtSetBrowsedPlayerReq *)instData->recvMsgP;
    CsrBtAvrcpCtSetBrowsedPlayerCfm *cfmMsg = CsrBtAvrcpCtSetBrowsedPlayerCfmBuild(reqMsg->connectionId, reqMsg->playerId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_14, FALSE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_BROWSING_PSM);
        CsrBtAvrcpCtSetBrowsedPlayerCmdSend(connInst, pendingMsgInfo, (CsrUint16)reqMsg->playerId);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtAddToNowPlayingReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtAddToNowPlayingReq *reqMsg = (CsrBtAvrcpCtAddToNowPlayingReq *)instData->recvMsgP;
    CsrBtAvrcpCtAddToNowPlayingCfm *cfmMsg = CsrBtAvrcpCtAddToNowPlayingCfmBuild(reqMsg->connectionId, reqMsg->scope, &reqMsg->uid);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_14, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        CsrBtAvrcpUid uid;
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);

        CSR_BT_AVRCP_UID_COPY(uid, reqMsg->uid);
        CsrBtAvrcpCtAddToNPLCmdSend(connInst, pendingMsgInfo, reqMsg->scope, reqMsg->uidCounter, &uid);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtInformBatteryStatusReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtInformBatteryStatusReq *reqMsg = (CsrBtAvrcpCtInformBatteryStatusReq *)instData->recvMsgP;
    CsrBtAvrcpCtInformBatteryStatusCfm *cfmMsg = CsrBtAvrcpCtInformBatteryStatusCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_14, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);
        CsrBtAvrcpCtInformBatterySend(connInst,pendingMsgInfo,reqMsg->batStatus);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}
#endif
void CsrBtAvrcpCtUnitInfoCmdReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtUnitInfoCmdReq *reqMsg = (CsrBtAvrcpCtUnitInfoCmdReq *)instData->recvMsgP;
    CsrBtAvrcpCtUnitInfoCmdCfm *cfmMsg = CsrBtAvrcpCtUnitInfoCmdCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_10, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {/* Build and send UNIT INFO message */
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);

        CsrBtAvrcpCtUnitInfoCmdSend(connInst,pendingMsgInfo, reqMsg->pDataLen, reqMsg->pData);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

void CsrBtAvrcpCtSubUnitInfoCmdReqHandler(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpCtSubUnitInfoCmdReq *reqMsg = (CsrBtAvrcpCtSubUnitInfoCmdReq *)instData->recvMsgP;
    CsrBtAvrcpCtSubUnitInfoCmdCfm *cfmMsg = CsrBtAvrcpCtSubUnitInfoCmdCfmBuild(reqMsg->connectionId);
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_AID(&instData->connList, reqMsg->connectionId);
    CsrBtResultCode resultCode = CsrBtAvrcpCtUtilCheckRemoteRdy(connInst, CSR_BT_AVRCP_CONFIG_SR_VERSION_10, TRUE);

    if (resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
    {/* Build and send SUB UNIT INFO message */
        AvrcpCtPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpCtUtilMsgQueueAdd(connInst, cfmMsg, reqMsg->phandle, CSR_BT_AVCTP_PSM);

        CsrBtAvrcpCtSubUnitInfoCmdSend(connInst,pendingMsgInfo, reqMsg->pDataLen, reqMsg->pData);
    }
    else
    {
        cfmMsg->resultCode = resultCode;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpMessagePut(reqMsg->phandle, cfmMsg);
    }
}

/******************************************************************************
******************* Building of primitives for application ********************
******************************************************************************/
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
CsrBtAvrcpCtInformDispCharsetCfm *CsrBtAvrcpCtInformDispCharsetCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtInformDispCharsetCfm *cfmMsg = pnew(CsrBtAvrcpCtInformDispCharsetCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtNotiRegisterCfm *CsrBtAvrcpCtNotiRegisterCfmBuild(CsrUint8 connId, CsrUint32 playbackInterval,CsrBtAvrcpNotiMask notiMask)
{
    CsrBtAvrcpCtNotiRegisterCfm *cfmMsg = pnew(CsrBtAvrcpCtNotiRegisterCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->notiMask        = notiMask;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;
    cfmMsg->playbackInterval = playbackInterval;

    return cfmMsg;
}

CsrBtAvrcpCtPasAttIdCfm *CsrBtAvrcpCtPasAttIdCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtPasAttIdCfm *cfmMsg = pnew(CsrBtAvrcpCtPasAttIdCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->attIdCount   = 0;
    cfmMsg->attId        = NULL;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;
    return cfmMsg;
}

CsrBtAvrcpCtPasValIdCfm *CsrBtAvrcpCtPasValIdCfmBuild(CsrUint8 connId, CsrBtAvrcpPasAttId attId)
{
    CsrBtAvrcpCtPasValIdCfm *cfmMsg = pnew(CsrBtAvrcpCtPasValIdCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->attId        = attId;
    cfmMsg->valIdCount      = 0;
    cfmMsg->valId           = NULL;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtPasAttTxtCfm *CsrBtAvrcpCtPasAttTxtCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtPasAttTxtCfm *cfmMsg = pnew(CsrBtAvrcpCtPasAttTxtCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->pasDataLen      = 0;
    cfmMsg->pasData         = NULL;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

void CsrBtAvrcpCtPasAttTxtIndSend(CsrUint8 connId, CsrUint16 pasDataLen, CsrUint8 *pasData, CsrSchedQid phandle)
{
    CsrBtAvrcpCtPasAttTxtInd *indMsg = pnew(CsrBtAvrcpCtPasAttTxtInd);

    indMsg->type            = CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND;
    indMsg->connectionId    = connId;
    indMsg->pasDataLen      = pasDataLen;
    indMsg->pasData         = pasData;
    CsrBtAvrcpMessagePut(phandle, indMsg);
}

CsrBtAvrcpCtPasValTxtCfm *CsrBtAvrcpCtPasValTxtCfmBuild(CsrUint8 connId, CsrBtAvrcpPasAttId attId)
{
    CsrBtAvrcpCtPasValTxtCfm *cfmMsg = pnew(CsrBtAvrcpCtPasValTxtCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->attId           = attId;
    cfmMsg->pasDataLen      = 0;
    cfmMsg->pasData         = NULL;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

void CsrBtAvrcpCtPasValTxtIndSend(CsrUint8 connId, CsrBtAvrcpPasAttId attId, CsrUint16 pasDataLen, CsrUint8 *pasData, CsrSchedQid phandle)
{
    CsrBtAvrcpCtPasValTxtInd *indMsg = pnew(CsrBtAvrcpCtPasValTxtInd);

    indMsg->type            = CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND;
    indMsg->connectionId    = connId;
    indMsg->attId        = attId;
    indMsg->pasDataLen      = pasDataLen;
    indMsg->pasData         = pasData;
    CsrBtAvrcpMessagePut(phandle, indMsg);
}

CsrBtAvrcpCtPasCurrentCfm *CsrBtAvrcpCtPasCurrentCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtPasCurrentCfm *cfmMsg = pnew(CsrBtAvrcpCtPasCurrentCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_PAS_CURRENT_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->attValPairCount        = 0;
    cfmMsg->attValPair             = NULL;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtPasSetCfm *CsrBtAvrcpCtPasSetCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtPasSetCfm *cfmMsg = pnew(CsrBtAvrcpCtPasSetCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_PAS_SET_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}
#endif
CsrBtAvrcpCtPassThroughCfm *CsrBtAvrcpCtPassThroughCfmBuild(CsrUint8 appConnId, CsrUint8 opId, CsrUint8 state)
{
    CsrBtAvrcpCtPassThroughCfm *cfmMsg = pnew(CsrBtAvrcpCtPassThroughCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_PASS_THROUGH_CFM;
    cfmMsg->connectionId    = appConnId;
    cfmMsg->operationId     = opId;
    cfmMsg->state           = state;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
void CsrBtAvrcpCtNotiIndSend(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    switch (rxData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX])
    {
        case CSR_BT_AVRCP_NOTI_ID_PLAYBACK_STATUS:
        {
            if (rxDataLen == AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_PS_HEADER_SIZE)
            {
                CsrBtAvrcpCtNotiPlaybackStatusInd *indMsg = pnew(CsrBtAvrcpCtNotiPlaybackStatusInd);

                indMsg->type            = CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND;
                indMsg->connectionId    = connInst->appConnId;
                indMsg->playbackStatus  = rxData[AVRCP_DATA_PDU_REG_NOTI_RES_VAL_INDEX];
                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_TRACK:
        {
            if (rxDataLen == AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_TC_HEADER_SIZE)
            {
                CsrBtAvrcpCtNotiTrackChangedInd *indMsg = pnew(CsrBtAvrcpCtNotiTrackChangedInd);

                indMsg->type            = CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND;
                indMsg->connectionId    = connInst->appConnId;
                CSR_BT_AVRCP_UID_COPY(indMsg->trackUid, &rxData[AVRCP_DATA_PDU_REG_NOTI_RES_VAL_INDEX]);
                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_TRACK_END:
        {
            if (rxDataLen == AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_TE_HEADER_SIZE)
            {
                CsrBtAvrcpCtNotiTrackEndInd *indMsg = pnew(CsrBtAvrcpCtNotiTrackEndInd);

                indMsg->type            = CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND;
                indMsg->connectionId    = connInst->appConnId;
                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_TRACK_START:
        {
            if (rxDataLen == AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_TS_HEADER_SIZE)
            {
                CsrBtAvrcpCtNotiTrackStartInd *indMsg = pnew(CsrBtAvrcpCtNotiTrackStartInd);

                indMsg->type            = CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND;
                indMsg->connectionId    = connInst->appConnId;
                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_PLAYBACK_POS:
        {
            if (rxDataLen == AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_PBP_HEADER_SIZE)
            {
                CsrBtAvrcpCtNotiPlaybackPosInd *indMsg = pnew(CsrBtAvrcpCtNotiPlaybackPosInd);

                indMsg->type            = CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND;
                indMsg->connectionId    = connInst->appConnId;
                indMsg->playbackPos     = CSR_GET_UINT32_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PDU_REG_NOTI_RES_VAL_INDEX]);
                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_BATT_STATUS:
        {
            if (rxDataLen == AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_BATT_HEADER_SIZE)
            {
                CsrBtAvrcpCtNotiBatteryStatusInd *indMsg = pnew(CsrBtAvrcpCtNotiBatteryStatusInd);

                indMsg->type            = CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND;
                indMsg->connectionId    = connInst->appConnId;
                indMsg->batteryStatus   = rxData[AVRCP_DATA_PDU_REG_NOTI_RES_VAL_INDEX];
                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_SYSTEM_STATUS:
        {
            if (rxDataLen == AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_SYS_HEADER_SIZE)
            {
                CsrBtAvrcpCtNotiSystemStatusInd *indMsg = pnew(CsrBtAvrcpCtNotiSystemStatusInd);

                indMsg->type            = CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND;
                indMsg->connectionId    = connInst->appConnId;
                indMsg->systemStatus    = rxData[AVRCP_DATA_PDU_REG_NOTI_RES_VAL_INDEX];
                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_PAS:
        {
            if ((rxDataLen >= AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_PAS_MIN_HEADER_SIZE) &&
                (rxDataLen == (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_PAS_HEADER_SIZE + rxData[AVRCP_DATA_PDU_REG_NOTI_RES_PAS_NUM_INDEX] * AVRCP_DATA_PDU_REG_NOTI_RES_PAS_PART_SIZE)))
            {
                CsrBtAvrcpCtPasSetInd *indMsg = pnew(CsrBtAvrcpCtPasSetInd);
                CsrUintFast8 i;

                indMsg->type            = CSR_BT_AVRCP_CT_PAS_SET_IND;
                indMsg->connectionId    = connInst->appConnId;
                indMsg->attValPairCount = rxData[AVRCP_DATA_PDU_REG_NOTI_RES_PAS_NUM_INDEX];
                indMsg->attValPair      = CsrPmemAlloc(indMsg->attValPairCount * AVRCP_DATA_PDU_REG_NOTI_RES_PAS_PART_SIZE);

                for (i = 0; i < indMsg->attValPairCount; i++)
                {
                    indMsg->attValPair[i].attribId = rxData[AVRCP_DATA_PDU_REG_NOTI_RES_PAS_ATT_INDEX + i * AVRCP_DATA_PDU_REG_NOTI_RES_PAS_PART_SIZE];
                    indMsg->attValPair[i].valueId = rxData[AVRCP_DATA_PDU_REG_NOTI_RES_PAS_VAL_INDEX + i * AVRCP_DATA_PDU_REG_NOTI_RES_PAS_PART_SIZE];
                }

                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_NOW_PLAYING_CONTENT:
        {
            if (rxDataLen == AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_NPL_HEADER_SIZE)
            {
                CsrBtAvrcpCtNotiNowPlayingInd *indMsg = pnew(CsrBtAvrcpCtNotiNowPlayingInd);

                indMsg->type            = CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND;
                indMsg->connectionId    = connInst->appConnId;

                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_AVAILABLE_PLAYERS:
        {
            if (rxDataLen == AVRCP_DATA_MD_HEADER_SIZE + 1)
            {
                CsrBtAvrcpCtNotiAvailablePlayersInd *indMsg = pnew(CsrBtAvrcpCtNotiAvailablePlayersInd);

                indMsg->type            = CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND;
                indMsg->connectionId    = connInst->appConnId;

                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_ADDRESSED_PLAYER:
        {
            if (rxDataLen >= AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_SYS_HEADER_SIZE)
            {
                CsrBtAvrcpCtSetAddressedPlayerInd *indMsg = pnew(CsrBtAvrcpCtSetAddressedPlayerInd);

                indMsg->type            = CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND;
                indMsg->connectionId    = connInst->appConnId;
                indMsg->playerId        = CSR_GET_UINT16_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PDU_REG_NOTI_RES_AP_PID_INDEX]);
                indMsg->uidCounter      = CSR_GET_UINT16_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PDU_REG_NOTI_RES_AP_UIDC_INDEX]);

                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_UIDS:
        {
            if (rxDataLen == AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_UIDS_HEADER_SIZE)
            {
                CsrBtAvrcpCtNotiUidsInd *indMsg = pnew(CsrBtAvrcpCtNotiUidsInd);

                indMsg->type            = CSR_BT_AVRCP_CT_NOTI_UIDS_IND;
                indMsg->connectionId    = connInst->appConnId;
                indMsg->uidCounter      = CSR_GET_UINT16_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PDU_REG_NOTI_RES_UIDS_UIDC_INDEX]);

                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        case CSR_BT_AVRCP_NOTI_ID_VOLUME:
        {
            if (rxDataLen == AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_VOL_HEADER_SIZE)
            {
                CsrBtAvrcpCtNotiVolumeInd *indMsg = pnew(CsrBtAvrcpCtNotiVolumeInd);

                indMsg->type            = CSR_BT_AVRCP_CT_NOTI_VOLUME_IND;
                indMsg->connectionId    = connInst->appConnId;
                indMsg->volume          = rxData[AVRCP_DATA_PDU_REG_NOTI_RES_VOL_INDEX];

                CsrBtAvrcpMessagePut(connInst->instData->ctrlHandle, indMsg);
            }
            break;
        }

        default:
        {
            break;
        }
    }
}



CsrBtAvrcpCtGetPlayStatusCfm *CsrBtAvrcpCtGetPlayStatusCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtGetPlayStatusCfm *cfmMsg = pnew(CsrBtAvrcpCtGetPlayStatusCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->songLength      = 0;
    cfmMsg->songPosition    = 0;
    cfmMsg->playStatus      = CSR_BT_AVRCP_PLAYBACK_STATUS_ERROR;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtGetAttributesCfm *CsrBtAvrcpCtGetAttributesCfmBuild(CsrUint8 connId, CsrBtAvrcpScope scope, CsrBtAvrcpUid uid)
{
    CsrBtAvrcpCtGetAttributesCfm *cfmMsg = pnew(CsrBtAvrcpCtGetAttributesCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->scope           = scope;
    cfmMsg->attributeCount  = 0;
    cfmMsg->attribDataLen   = 0;
    cfmMsg->attribData      = NULL;
    CsrMemCpy(cfmMsg->uid, uid, sizeof(CsrBtAvrcpUid));
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;
    cfmMsg->attribDataPayloadOffset = 0;

    return cfmMsg;
}

void CsrBtAvrcpCtGetAttributesIndSend(CsrSchedQid phandle, CsrUint8 connId, CsrBtAvrcpScope scope, CsrBtAvrcpUid *uid, CsrUint8 attribCount, CsrUint16 attribDataLen, CsrUint8 *attribData, CsrUint16 offset)
{
    CsrBtAvrcpCtGetAttributesInd *indMsg = pnew(CsrBtAvrcpCtGetAttributesInd);

    indMsg->type            = CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND;
    indMsg->connectionId    = connId;
    indMsg->scope           = scope;
    indMsg->attributeCount  = attribCount;
    indMsg->attribDataLen   = attribDataLen;
    indMsg->attribData      = attribData;
    CSR_BT_AVRCP_UID_COPY(indMsg->uid, *uid);
    indMsg->attribDataPayloadOffset = offset;
    CsrBtAvrcpMessagePut(phandle, indMsg);
}

CsrBtAvrcpCtGetFolderItemsCfm *CsrBtAvrcpCtGetFolderItemsCfmBuild(CsrUint8 connId, CsrBtAvrcpScope scope, CsrUint32 startItem, CsrUint32 endItem)
{
    CsrBtAvrcpCtGetFolderItemsCfm *cfmMsg = pnew(CsrBtAvrcpCtGetFolderItemsCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->scope           = scope;
    cfmMsg->uidCounter      = 0;
    cfmMsg->startItem       = startItem;
    cfmMsg->endItem         = endItem;
    cfmMsg->itemsCount      = 0;
    cfmMsg->itemsDataLen    = 0;
    cfmMsg->itemsData       = NULL;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtPlayCfm *CsrBtAvrcpCtPlayCfmBuild(CsrUint8 connId, CsrBtAvrcpScope scope, CsrBtAvrcpUid *uid)
{
    CsrBtAvrcpCtPlayCfm *cfmMsg = pnew(CsrBtAvrcpCtPlayCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_PLAY_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->scope           = scope;
    CsrMemCpy(cfmMsg->uid, *uid, sizeof(CsrBtAvrcpUid));
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtSearchCfm *CsrBtAvrcpCtSearchCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtSearchCfm *cfmMsg = pnew(CsrBtAvrcpCtSearchCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_SEARCH_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->numberOfItems   = 0;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtChangePathCfm *CsrBtAvrcpCtChangePathCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtChangePathCfm *cfmMsg = pnew(CsrBtAvrcpCtChangePathCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_CHANGE_PATH_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtSetVolumeCfm *CsrBtAvrcpCtSetVolumeCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtSetVolumeCfm *cfmMsg = pnew(CsrBtAvrcpCtSetVolumeCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_SET_VOLUME_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->volume          = 0;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtSetAddressedPlayerCfm *CsrBtAvrcpCtSetAddressedPlayerCfmBuild(CsrUint8 connId, CsrUint32 playerId)
{
    CsrBtAvrcpCtSetAddressedPlayerCfm *cfmMsg = pnew(CsrBtAvrcpCtSetAddressedPlayerCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->playerId        = playerId;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtSetBrowsedPlayerCfm *CsrBtAvrcpCtSetBrowsedPlayerCfmBuild(CsrUint8 connId, CsrUint32 playerId)
{
    CsrBtAvrcpCtSetBrowsedPlayerCfm *cfmMsg = pnew(CsrBtAvrcpCtSetBrowsedPlayerCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->playerId        = playerId;
    cfmMsg->uidCounter      = 0;
    cfmMsg->itemsCount      = 0;
    cfmMsg->folderDepth     = 0;
    cfmMsg->folderNamesLen  = 0;
    cfmMsg->folderNames     = NULL;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtAddToNowPlayingCfm *CsrBtAvrcpCtAddToNowPlayingCfmBuild(CsrUint8 connId, CsrBtAvrcpScope scope, CsrBtAvrcpUid *uid)
{
    CsrBtAvrcpCtAddToNowPlayingCfm *cfmMsg = pnew(CsrBtAvrcpCtAddToNowPlayingCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->scope           = scope;
    CsrMemCpy(cfmMsg->uid, *uid, sizeof(CsrBtAvrcpUid));
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}

CsrBtAvrcpCtInformBatteryStatusCfm *CsrBtAvrcpCtInformBatteryStatusCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtInformBatteryStatusCfm *cfmMsg = pnew(CsrBtAvrcpCtInformBatteryStatusCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;

    return cfmMsg;
}
#endif
CsrBtAvrcpCtUnitInfoCmdCfm *CsrBtAvrcpCtUnitInfoCmdCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtUnitInfoCmdCfm *cfmMsg = pnew(CsrBtAvrcpCtUnitInfoCmdCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;
    cfmMsg->pDataLen        = 0;
    cfmMsg->pData           = NULL;

    return cfmMsg;
}

CsrBtAvrcpCtSubUnitInfoCmdCfm *CsrBtAvrcpCtSubUnitInfoCmdCfmBuild(CsrUint8 connId)
{
    CsrBtAvrcpCtSubUnitInfoCmdCfm *cfmMsg = pnew(CsrBtAvrcpCtSubUnitInfoCmdCfm);

    cfmMsg->type            = CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM;
    cfmMsg->connectionId    = connId;
    cfmMsg->resultCode      = CSR_BT_RESULT_CODE_AVRCP_TIMEOUT;
    cfmMsg->resultSupplier  = CSR_BT_SUPPLIER_AVRCP;
    cfmMsg->pDataLen        = 0;
    cfmMsg->pData           = NULL;

    return cfmMsg;
}
#endif /* #ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE */
#endif /* #ifndef EXCLUDE_CSR_BT_AVRCP_MODULE */

