/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE

#include "csr_bt_util.h"
#include "csr_bt_avrcp_main.h"
#include "csr_bt_avrcp_prim.h"
#include "csr_bt_avrcp_lib.h"

void CsrBtAvrcpCtCompletePendingMsg(AvrcpCtPendingMsgInfo_t *pendingMsgInfo)
{
    CsrBtAvrcpMessagePut(pendingMsgInfo->phandle, pendingMsgInfo->cfmMsg);
    pendingMsgInfo->cfmMsg = NULL;
    AVRCP_LIST_CT_PMSG_REMOVE(&pendingMsgInfo->connInst->ctLocal.pendingMsgList, pendingMsgInfo);
}

void CsrBtAvrcpCtUtilConvertAVCRspType(CsrUint8 rspType,
    CsrBtResultCode *resultCode, CsrBtSupplier *resultSupplier)
{
    switch (rspType)
    {
        case CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED: /* Intentional fall-through */
        case CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE:
        {
            *resultCode = CSR_BT_RESULT_CODE_AVRCP_SUCCESS;
            *resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        }
        break;

        case CSR_BT_AVRCP_DATA_AVC_RTYPE_NOT_IMP: /* Intentional fall-through */
        case CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED:
        {
            *resultCode = rspType;
            *resultSupplier = CSR_BT_SUPPLIER_AVC;
        }
        break;

        default:
        {
            *resultCode = CSR_BT_RESULT_CODE_AVRCP_UNSPECIFIED_ERROR;
            *resultSupplier = CSR_BT_SUPPLIER_AVRCP; /* Sanity checking */
        }
    }
}

#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
void CsrBtAvrcpCtUtilConvertBrowsingRspType(CsrUint8 rspType,
    CsrBtResultCode *resultCode, CsrBtSupplier *resultSupplier)
{
    if (rspType == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
    {
        *resultCode = CSR_BT_RESULT_CODE_AVRCP_SUCCESS;
        *resultSupplier = CSR_BT_SUPPLIER_AVRCP;
    }
    else
    {
        *resultCode = rspType;
        *resultSupplier = CSR_BT_SUPPLIER_AVCTP;
    }
}
#endif
void CsrBtAvrcpCtPendingMsgTimeout(CsrUint16 dummy, void *pendingMsg)
{
    AvrcpCtPendingMsgInfo_t *pendingMsgInfo = (AvrcpCtPendingMsgInfo_t *)pendingMsg;
    pendingMsgInfo->timerId = 0;
    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static const CsrUint8 avrcpCtUtilResultCodeOffsets[CSR_BT_AVRCP_CT_PRIM_CFM_HIGHEST - CSR_BT_AVRCP_CT_PRIM_UPSTREAM_LOWEST + 1] =
{/* Order MUST correspond to the numbering of the primitives */
    offsetof(CsrBtAvrcpCtInformDispCharsetCfm, resultCode),
    offsetof(CsrBtAvrcpCtNotiRegisterCfm, resultCode),
    offsetof(CsrBtAvrcpCtGetPlayStatusCfm, resultCode),
    offsetof(CsrBtAvrcpCtPasAttIdCfm, resultCode),
    offsetof(CsrBtAvrcpCtPasValIdCfm, resultCode),
    offsetof(CsrBtAvrcpCtPasAttTxtCfm, resultCode),
    offsetof(CsrBtAvrcpCtPasValTxtCfm, resultCode),
    offsetof(CsrBtAvrcpCtPasCurrentCfm, resultCode),
    offsetof(CsrBtAvrcpCtPasSetCfm, resultCode),
    offsetof(CsrBtAvrcpCtPassThroughCfm, resultCode),
    offsetof(CsrBtAvrcpCtGetFolderItemsCfm, resultCode),
    offsetof(CsrBtAvrcpCtPlayCfm, resultCode),
    offsetof(CsrBtAvrcpCtSearchCfm, resultCode),
    offsetof(CsrBtAvrcpCtGetAttributesCfm, resultCode),
    offsetof(CsrBtAvrcpCtChangePathCfm, resultCode),
    offsetof(CsrBtAvrcpCtSetVolumeCfm, resultCode),
    offsetof(CsrBtAvrcpCtSetAddressedPlayerCfm, resultCode),
    offsetof(CsrBtAvrcpCtSetBrowsedPlayerCfm, resultCode),
    offsetof(CsrBtAvrcpCtAddToNowPlayingCfm, resultCode),
    offsetof(CsrBtAvrcpCtInformBatteryStatusCfm, resultCode),
    offsetof(CsrBtAvrcpCtUnitInfoCmdCfm, resultCode),
    offsetof(CsrBtAvrcpCtSubUnitInfoCmdCfm, resultCode)
};

static const CsrUint8 avrcpCtUtilResultSupplierOffsets[CSR_BT_AVRCP_CT_PRIM_CFM_HIGHEST - CSR_BT_AVRCP_CT_PRIM_UPSTREAM_LOWEST + 1] =
{/* Order MUST correspond to the numbering of the primitives */
    offsetof(CsrBtAvrcpCtInformDispCharsetCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtNotiRegisterCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtGetPlayStatusCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtPasAttIdCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtPasValIdCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtPasAttTxtCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtPasValTxtCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtPasCurrentCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtPasSetCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtPassThroughCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtGetFolderItemsCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtPlayCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtSearchCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtGetAttributesCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtChangePathCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtSetVolumeCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtSetAddressedPlayerCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtSetBrowsedPlayerCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtAddToNowPlayingCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtInformBatteryStatusCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtUnitInfoCmdCfm, resultSupplier),
    offsetof(CsrBtAvrcpCtSubUnitInfoCmdCfm, resultSupplier)
};

void CsrBtAvrcpCtPendingMsgUpdateResult(AvrcpCtPendingMsgInfo_t *pendingMsg,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{/* Use a look-up table instead of a huge switch to reduce code size */
    CsrBtAvrcpPrim primType = *(CsrBtAvrcpPrim *)pendingMsg->cfmMsg;

    if ((primType >= CSR_BT_AVRCP_CT_PRIM_UPSTREAM_LOWEST) && (primType <= CSR_BT_AVRCP_CT_PRIM_CFM_HIGHEST))
    {
        *((CsrBtResultCode *)((CsrUint8 *)pendingMsg->cfmMsg +
            avrcpCtUtilResultCodeOffsets[primType - CSR_BT_AVRCP_CT_PRIM_UPSTREAM_LOWEST])) = resultCode;
        *((CsrBtSupplier *)((CsrUint8 *)pendingMsg->cfmMsg +
            avrcpCtUtilResultSupplierOffsets[primType - CSR_BT_AVRCP_CT_PRIM_UPSTREAM_LOWEST])) = resultSupplier;
    }
}

void CsrBtAvrcpCtUtilPendingMsgCompleteFromPsm(AvrcpConnInstance_t *connInst,
    CsrUint16 psm, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    AvrcpCtPendingMsgInfo_t *pendingMsg;

    for (pendingMsg = AVRCP_LIST_CT_PMSG_GET_FIRST(&connInst->ctLocal.pendingMsgList); pendingMsg; )
    {
        AvrcpCtPendingMsgInfo_t *tmpPendingMsg = pendingMsg->next;
        if (pendingMsg->psm == psm)
        {
            CsrBtAvrcpCtPendingMsgUpdateResult(pendingMsg, resultCode,
                resultSupplier);
            CsrBtAvrcpCtCompletePendingMsg(pendingMsg);
        }
        pendingMsg = tmpPendingMsg;
    }
}

#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
CsrBool CsrBtAvrcpCtRegisterNextNotification(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo)
{
    CsrBtAvrcpCtNotiRegisterCfm *cfmMsg = (CsrBtAvrcpCtNotiRegisterCfm *)pendingMsgInfo->cfmMsg;

    while (pendingMsgInfo->tmpState < CSR_BT_AVRCP_NOTI_ID_MAXIMUM)
    {
        pendingMsgInfo->tmpState++; /* Skip to next notification ID */

        if (CSR_BIT_IS_SET((connInst->ctLocal.tgSupportedNotifications & connInst->ctLocal.ctRequestedNotifications), pendingMsgInfo->tmpState - CSR_BT_AVRCP_NOTI_ID_OFFSET))
        {/* Register for the next notification that is supported by both the target and controller */
            CsrBtAvrcpCtRegisterNotificationCmdSend(connInst,
                                               pendingMsgInfo,
                                               pendingMsgInfo->tmpState,
                                               cfmMsg->playbackInterval);

            return FALSE; /* A command was sent */
        }
    }

    return TRUE; /* All supported/requested notifications have been registered */
}
#endif

void CsrBtAvrcpCtUtilGetAvailableTLabel(AvrcpConnInstance_t *connInst, CsrBtAvrcpConnDetails *conn)
{/* Assign an unique TLabel */
    if (&connInst->control == conn)
    {/* Control channel */
        AVRCP_TLABEL_ASSIGN(connInst->control.ctTLabel);

#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
        if (connInst->ctLocal.activeNotifications)
        {/* At least one notification is already registered and the requested TLabel is for a notification */
            CsrUintFast8 i;

            for (i = 0; i < CSR_BT_AVRCP_NOTI_ID_COUNT; i++)
            {
                if (connInst->control.ctTLabel == connInst->ctLocal.notiList[i])
                {/* TLabel is already used for a notification - skip to next and restart */
                    AVRCP_TLABEL_ASSIGN(connInst->control.ctTLabel);
                    i = 0;
                }
            }
        }
#endif        
    }
    else
    {/* Browsing channel */
        AVRCP_TLABEL_ASSIGN(connInst->browsing.ctTLabel);
    }
}

void CsrBtAvrcpCtResetPendingMsg(CsrBtAvrcpConnDetails *conn, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrTime time)
{
    CsrBtAvrcpCtUtilGetAvailableTLabel(pendingMsgInfo->connInst, conn);
    pendingMsgInfo->tLabel = conn->ctTLabel;
    CsrSchedTimerCancel(pendingMsgInfo->timerId, NULL, NULL);
    pendingMsgInfo->timerId = CsrSchedTimerSet(time + CSR_BT_AVRCP_CT_TIMER_AIR_OVERHEAD, CsrBtAvrcpCtPendingMsgTimeout, 0, pendingMsgInfo);
}

CsrBtResultCode CsrBtAvrcpCtUtilCheckRemoteRdy(AvrcpConnInstance_t *connInst, CsrUint16 avrcpVersion, CsrBool control)
{
    if (connInst)
    {
        if (connInst->ctLocal.tgSdpAvrcpVersion >= avrcpVersion)
        {
            if (connInst->control.state == AVRCP_STATE_CONN_CONNECTED)
            {
                if (control)
                {
                    return CSR_BT_RESULT_CODE_AVRCP_SUCCESS;
                }
                else
                {
                    if (connInst->browsing.state == AVRCP_STATE_CONN_CONNECTED)
                    {
                        return CSR_BT_RESULT_CODE_AVRCP_SUCCESS;
                    }
                    else if (CSR_MASK_IS_SET(connInst->remoteFeatures, CSR_BT_AVRCP_CONFIG_SR_FEAT_BROWSING))
                    {/* Browsing is supported but not yet connected - start connecting */
                        CsrBtAvrcpUtilConnect(connInst);
                        return CSR_BT_RESULT_CODE_AVRCP_SUCCESS;
                    }
                    else
                    {/* Local or remote does not support browsing */
                        return CSR_BT_RESULT_CODE_AVRCP_INVALID_VERSION;
                    }
                }
            }
            else
            {
                return CSR_BT_RESULT_CODE_AVRCP_CHANNEL_NOT_CONNECTED;
            }
        }
        else
        {
            return CSR_BT_RESULT_CODE_AVRCP_INVALID_VERSION;
        }
    }
    else
    {
        return CSR_BT_RESULT_CODE_AVRCP_DEVICE_NOT_CONNECTED;
    }
}

AvrcpCtPendingMsgInfo_t *CsrBtAvrcpCtUtilMsgQueueAdd(AvrcpConnInstance_t *connInst, void *cfmMsg, CsrSchedQid phandle, CsrUint16 psm)
{
    AvrcpCtPendingMsgInfo_t *newPendingMsgInfo = AVRCP_LIST_CT_PMSG_ADD_FIRST(&connInst->ctLocal.pendingMsgList);

    newPendingMsgInfo->cfmMsg   = cfmMsg;
    newPendingMsgInfo->tLabel   = AVRCP_TLABEL_INVALID;
    newPendingMsgInfo->tmpState = AVRCP_STATE_PM_IDLE;
    newPendingMsgInfo->timerId  = 0;
    newPendingMsgInfo->connInst = connInst;
    newPendingMsgInfo->phandle  = phandle;
    newPendingMsgInfo->psm      = psm;
    return newPendingMsgInfo;
}

void CsrBtAvrcpCtUtilMsgQueueRemoveCB(CsrCmnListElm_t *elem)
{
    AvrcpCtPendingMsgInfo_t *pendingMsgInfo = (AvrcpCtPendingMsgInfo_t *)elem;

    CsrSchedTimerCancel(pendingMsgInfo->timerId, NULL, NULL);

    if (pendingMsgInfo->cfmMsg)
    {
        /* Complete pending message */
        CsrBtAvrcpCtPendingMsgUpdateResult(pendingMsgInfo,
            CSR_BT_RESULT_CODE_AVRCP_DEVICE_NOT_CONNECTED,
            CSR_BT_SUPPLIER_AVRCP);
        CsrBtAvrcpMessagePut(pendingMsgInfo->phandle, pendingMsgInfo->cfmMsg);
    }
}

void CsrBtAvrcpCtUtilInitConnLocal(CsrBtAvrcpCtConnInfo *connInfo)
{
    /* SDP */
    connInfo->tgSdpAvrcpVersion         = CSR_BT_AVRCP_CONFIG_SR_VERSION_10;
    connInfo->tgSdpSupportedFeatures    = 0;

    /* Notifications */
    connInfo->activeNotifications       = 0;
    connInfo->ctRequestedNotifications  = 0;
    connInfo->notiConfig                = CSR_BT_AVRCP_NOTI_REG_STANDARD;
    connInfo->playbackInterval          = 0;
    connInfo->tgSupportedNotifications  = 0;
    CsrMemSet(connInfo->notiList, 0, CSR_BT_AVRCP_NOTI_ID_COUNT);

    /* Misc */
    connInfo->pendingMsgInfo            = NULL;
    connInfo->mpAddressedId             = CSR_BT_AVRCP_MP_ID_INVALID;
    connInfo->mpBrowsedId               = 0;
    CsrCmnListInit(&connInfo->pendingMsgList, 0, NULL, CsrBtAvrcpCtUtilMsgQueueRemoveCB);
}

CsrUint16 CsrBtAvrcpCtSBPFolderNamesGet(CsrUint8 **folderPathName, CsrUint8 *inputData, CsrUint8 depth)
{
    CsrUint16 folderNamesLen = 1; /* Space for the first '/' and NULL terminator */
    CsrUint8 *tmpData = inputData;
    CsrUintFast8 i;
    CsrUint16 tmp, idx;

    for (i=0; i < depth; i++)
    {/* First two bytes contain the length of the first directory name */
        tmp = (CsrUint16)(CSR_GET_UINT16_FROM_BIG_ENDIAN(tmpData));

        folderNamesLen += (tmp + 1); /* Accumulative data length including place for separator */
        /* Move pointer to the next folder name length */
        tmpData += (tmp + sizeof(CsrUint16));
    }

    *folderPathName = CsrPmemAlloc(folderNamesLen);

    tmpData = inputData;
    idx = 0;
    (*folderPathName)[idx] = '/';
    idx++;

    for (i=0; i < depth; i++)
    {
        tmp = (CsrUint16)(CSR_GET_UINT16_FROM_BIG_ENDIAN(tmpData));
        tmpData += sizeof(CsrUint16);
        CsrMemCpy(&(*folderPathName)[idx],tmpData,tmp);
        tmpData += tmp;
        idx += tmp;
        (*folderPathName)[idx] = '/';
        idx++;
    }
    (*folderPathName)[folderNamesLen-1] = 0;
    return folderNamesLen;
}

#endif /* #ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE */

#endif

