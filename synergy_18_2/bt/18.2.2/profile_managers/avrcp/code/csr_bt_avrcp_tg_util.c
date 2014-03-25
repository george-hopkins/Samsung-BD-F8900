/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE

#include "csr_bt_util.h"
#include "csr_bt_avrcp_main.h"
#include "csr_bt_avrcp_prim.h"
#include "csr_bt_avrcp_lib.h"

void CsrBtAvrcpTgPendingMsgTimeout(CsrUint16 dummy, void *pendingMsg)
{
    AvrcpTgPendingMsgInfo_t *pendingMsgInfo = (AvrcpTgPendingMsgInfo_t *)pendingMsg;
    pendingMsgInfo->timerId = 0;

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    /* Send proper response with error */
    if (pendingMsgInfo->psm == CSR_BT_AVCTP_PSM)
    {/* Control */
        if (AVRCP_DATA_AVC_OPCODE_GET(pendingMsgInfo->rxData) == AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT)
        {/* Metadata */
            CsrBtAvrcpTgRejectAvcVendorSend(pendingMsgInfo->connInst,
                                       pendingMsgInfo->rxData,
                                       CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED,
                                       CSR_BT_AVRCP_STATUS_INTERNAL_ERROR);
        }
        else
#endif
        {/* Pass-through */
            CsrBtAvrcpTgRejectAvcSend(pendingMsgInfo->connInst,
                                 pendingMsgInfo->rxData,
                                 CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED);
        }
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    }
    else
    {/* Browsing */
        CsrBtAvrcpTgNormalRejectBrowsingSend(pendingMsgInfo->connInst,
                                  pendingMsgInfo->rxData,
                                  CSR_BT_AVRCP_STATUS_INTERNAL_ERROR);
    }
#endif

    AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
}

AvrcpTgPendingMsgInfo_t *CsrBtAvrcpTgUtilMsgQueueAdd(AvrcpConnInstance_t *connInst, psm_t psm, CsrBtAvrcpPrim msgType, CsrTime delay, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    AvrcpTgPendingMsgInfo_t *pendingMsgInfo = AVRCP_LIST_TG_PMSG_ADD_FIRST(&connInst->tgLocal.pendingMsgList);

    pendingMsgInfo->msgType = msgType;
    pendingMsgInfo->msgId   = AVRCP_MSG_ID_ASSIGN(connInst->instData->tgLocal.tgMsgId);
    pendingMsgInfo->timerId = CsrSchedTimerSet(delay, CsrBtAvrcpTgPendingMsgTimeout, 0, (void *)pendingMsgInfo);
    pendingMsgInfo->rxData  = *rxData;
    pendingMsgInfo->psm     = psm;
    pendingMsgInfo->connInst = connInst;

    *rxData = NULL;

    return pendingMsgInfo;
}

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
void CsrBtAvrcpTgUtilPendingMsgUpdate(AvrcpTgPendingMsgInfo_t *pendingMsgInfo)
{
    pendingMsgInfo->msgId = AVRCP_MSG_ID_ASSIGN(pendingMsgInfo->connInst->instData->tgLocal.tgMsgId);
}
#endif

void CsrBtAvrcpTgUtilMsgQueueRemoveCB(CsrCmnListElm_t *elem)
{
    AvrcpTgPendingMsgInfo_t *pendingMsgInfo = (AvrcpTgPendingMsgInfo_t *)elem;
    CsrSchedTimerCancel(pendingMsgInfo->timerId,NULL,NULL);
    CsrPmemFree(pendingMsgInfo->rxData);
}

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
static CsrUint16 csrBtAvrcpTgUtilGetMpId(AvrcpInstanceData_t *instData)
{
    CsrBtAvrcpTgMp *mp      = AVRCP_LIST_TG_MP_GET_FIRST(&instData->tgLocal.mpList);
    CsrUint8     currMpId = 0;

    while (mp)
    {
        if (mp->mpId == currMpId)
        {/* MP ID already in use - skip to next number and restart */
            currMpId++;
            mp = AVRCP_LIST_TG_MP_GET_FIRST(&instData->tgLocal.mpList);
        }
        else
        {
            mp = mp->next;
        }
    }

    /* Lowest possible connection ID found */
    return currMpId;
}

static void csrBtAvrcpTgUtilNotiCompleteAPChanged(AvrcpConnInstance_t *connInst, CsrBtAvrcpNotiId notiId)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE] = {CSR_BT_AVRCP_STATUS_ADDR_PLAYER_CHANGED,0,0,0,0,0,0,0};


    CsrBtAvrcpTgRegisterNotificationRspSend(connInst, notiId, notiData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, TRUE);
    CSR_BIT_UNSET(connInst->tgLocal.notificationsActive, notiId - CSR_BT_AVRCP_NOTI_ID_OFFSET);
}

static void csrBtAvrcpTgUtilNotiComplete(AvrcpConnInstance_t *connInst, CsrBtAvrcpNotiId notiId)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE] = {0,0,0,0,0,0,0,0};

    if (notiId == CSR_BT_AVRCP_NOTI_ID_PAS)
    {
        CsrBtAvrcpTgRegisterNotificationPasRspSend(connInst, CSR_BT_AVRCP_DATA_AVC_RTYPE_CHANGED, 0, NULL);
    }
    else
    {
        CsrBtAvrcpTgRegisterNotificationRspSend(connInst, notiId, notiData, CSR_BT_AVRCP_DATA_AVC_RTYPE_CHANGED, FALSE);
    }

    CSR_BIT_UNSET(connInst->tgLocal.notificationsActive, notiId - CSR_BT_AVRCP_NOTI_ID_OFFSET);
}

void CsrBtAvrcpTgUtilAvailableMPChanged(AvrcpInstanceData_t *instData)
{
    AvrcpConnInstance_t *connInst;

    for (connInst = AVRCP_LIST_CONN_GET_FIRST(&instData->connList); connInst; connInst = connInst->next)
    {
        if (CSR_BIT_IS_SET(connInst->tgLocal.notificationsActive, CSR_BT_AVRCP_NOTI_ID_AVAILABLE_PLAYERS - CSR_BT_AVRCP_NOTI_ID_OFFSET) &&
                (connInst->control.state == AVRCP_STATE_CONN_CONNECTED))
        {
            csrBtAvrcpTgUtilNotiComplete(connInst, CSR_BT_AVRCP_NOTI_ID_AVAILABLE_PLAYERS);
        }
    }
}

void CsrBtAvrcpTgUtilMpListUpdateAddressed(AvrcpInstanceData_t *instData, CsrBtAvrcpTgMp *mp, CsrUint16 uidCounter)
{
    AvrcpConnInstance_t *connInst;
    CsrBtAvrcpNotiMask notiToComplete;
    CsrUint8 notiId;
    CsrUint8 notiData[sizeof(CsrUint32)];

    /* Complete pending notifications */
    for (connInst = AVRCP_LIST_CONN_GET_FIRST(&instData->connList); connInst; connInst = connInst->next)
    {
        if (connInst->tgLocal.mpAddressed != mp)
        {/* The addressed player is changed for the specific connection */
            connInst->tgLocal.mpAddressed = mp;
            notiToComplete = connInst->tgLocal.notificationsActive & AVRCP_NOTI_FLAG_CHANGE_ADDRESSED;
            
            if (CSR_MASK_IS_SET(connInst->tgLocal.notificationsActive, CSR_BT_AVRCP_NOTI_FLAG_ADDRESSED_PLAYER))
            {
                /* Send addressed player changed noti */
                CSR_COPY_UINT16_TO_BIG_ENDIAN((CsrUint16)mp->mpId, notiData);
                CSR_COPY_UINT16_TO_BIG_ENDIAN(uidCounter, notiData + sizeof(CsrUint16));

                CsrBtAvrcpTgRegisterNotificationRspSend(connInst,
                                                        CSR_BT_AVRCP_NOTI_ID_ADDRESSED_PLAYER,
                                                        notiData,
                                                        CSR_BT_AVRCP_DATA_AVC_RTYPE_CHANGED, FALSE);
            }

            if (notiToComplete)
            {/* The remote controller has registered for notifications that needs to be completed */
                for (notiId = CSR_BT_AVRCP_NOTI_ID_OFFSET; notiId <= CSR_BT_AVRCP_NOTI_ID_MAXIMUM; notiId++)
                {
                    if (CSR_BIT_IS_SET(notiToComplete, notiId - CSR_BT_AVRCP_NOTI_ID_OFFSET))
                    {/* The specific notification should be completed */
                        csrBtAvrcpTgUtilNotiCompleteAPChanged(connInst, notiId);
                    }
                }
            }

        }
    }
}

void CsrBtAvrcpTgUtilMpListUpdateBrowsed(AvrcpConnInstance_t *connInst, CsrBtAvrcpTgMp *mp)
{
    if (connInst->tgLocal.mpBrowsed != mp)
    {/* The browsed player is changed for the specific connection */
        CsrBtAvrcpNotiMask notiToComplete = connInst->tgLocal.notificationsActive & AVRCP_NOTI_FLAG_CHANGE_BROWSED;

        connInst->tgLocal.mpBrowsed = mp;

        if (notiToComplete)
        {/* The remote controller has registered for notifications that needs to be completed */
            CsrUint8 notiId;

            for (notiId = CSR_BT_AVRCP_NOTI_ID_OFFSET; notiId <= CSR_BT_AVRCP_NOTI_ID_MAXIMUM; notiId++)
            {
                if (CSR_BIT_IS_SET(notiToComplete, notiId - CSR_BT_AVRCP_NOTI_ID_OFFSET))
                {/* The specific notification should be completed */
                    csrBtAvrcpTgUtilNotiComplete(connInst, notiId);
                }
            }
        }
    }
}

void CsrBtAvrcpTgUtilMpListAddCB(CsrCmnListElm_t *elem)
{
    CsrBtAvrcpTgMp *mp = (CsrBtAvrcpTgMp *)elem;
    mp->mpId        = 0xFFFF; /* To avoid UMR when a ID is assigned later */
    mp->pas         = NULL;
    mp->playerName  = NULL;
}

void CsrBtAvrcpTgUtilMpListRemoveCB(CsrCmnListElm_t *elem)
{
    CsrBtAvrcpTgMp *mp = (CsrBtAvrcpTgMp *)elem;
    CsrPmemFree(mp->pas);
    CsrPmemFree(mp->playerName);
}

CsrBtAvrcpTgMp *CsrBtAvrcpTgUtilMpListAdd(AvrcpInstanceData_t *instData, CsrBtAvrcpTgMpRegisterReq *req)
{
    CsrBtAvrcpTgMp *mp = AVRCP_LIST_TG_MP_ADD_LAST(&instData->tgLocal.mpList);

    mp->configMask          = req->configMask;
    mp->mpHandle            = req->playerHandle;
    mp->mpId                = csrBtAvrcpTgUtilGetMpId(instData);
    mp->notificationMask    = req->notificationMask;
    mp->pas                 = req->pas;
    mp->majorType           = req->majorType;
    mp->subType             = req->subType;
    mp->pasLen              = req->pasLen;
    mp->pas                 = req->pas;
    mp->playerName          = req->playerName;
    CsrMemCpy(mp->featureMask, req->featureMask, sizeof(req->featureMask));
    return mp;
}

void CsrBtAvrcpTgUtilContinuingRspReset(AvrcpConnInstance_t *connInst, CsrBool completeReset)
{
    /* PAS */
    CsrPmemFree(connInst->tgLocal.attId);
    CsrPmemFree(connInst->tgLocal.valId);
    connInst->tgLocal.attMask       = 0;
    connInst->tgLocal.attIdCount    = 0;
    connInst->tgLocal.attId         = NULL;
    connInst->tgLocal.valIdFromAttId = CSR_BT_AVRCP_PAS_ATT_ID_INVALID;
    connInst->tgLocal.valIdCount    = 0;
    connInst->tgLocal.valId         = NULL;

    /* GetElementAttributes */
    connInst->tgLocal.getAttribIndex = 0;

    if (connInst->tgLocal.getAttribResPrim)
    {
        CsrPmemFree(connInst->tgLocal.getAttribResPrim->attribData);
        CsrPmemFree(connInst->tgLocal.getAttribResPrim);
        connInst->tgLocal.getAttribResPrim = NULL;
    }

    if (completeReset)
    {
        connInst->tgLocal.pduId         = AVRCP_DATA_PDU_ID_INVALID;
        connInst->tgLocal.packetType    = AVRCP_DATA_MD_PACKET_TYPE_SINGLE;
    }
}
#endif

void CsrBtAvrcpTgUtilInitConnLocal(AvrcpInstanceData_t *instData, CsrBtAvrcpTgConnInfo *connInfo)
{
    /* SDP */
    connInfo->ctSdpAvrcpVersion         = CSR_BT_AVRCP_CONFIG_SR_VERSION_10;
    connInfo->ctSdpSupportedFeatures    = 0;

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    /* Notifications */
    connInfo->notificationsActive       = 0;
    connInfo->notificationsPBInterval   = CSR_BT_AVRCP_PLAYBACK_POS_INVALID;
    CsrMemSet(connInfo->notiList, AVRCP_TLABEL_INVALID, CSR_BT_AVRCP_NOTI_ID_COUNT);

    /* Continuing response */
    connInfo->pduId                     = AVRCP_DATA_PDU_ID_INVALID;
    connInfo->packetType                = AVRCP_DATA_MD_PACKET_TYPE_SINGLE;
    connInfo->attMask                   = 0;
    connInfo->attIdCount                = 0;
    connInfo->attId                     = NULL;
    connInfo->valIdFromAttId            = CSR_BT_AVRCP_PAS_ATT_ID_INVALID;
    connInfo->valIdCount                = 0;
    connInfo->valId                     = NULL;
    connInfo->getAttribResPrim = NULL;
    /* Misc */
#endif
    connInfo->mpAddressed               = instData->tgLocal.mpDefault;
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    connInfo->mpBrowsed                 = instData->tgLocal.mpDefault;
    connInfo->currentPlayer             = NULL;
    connInfo->itemCnt                   = 0;
#endif
    CsrCmnListInit(&connInfo->pendingMsgList, 0, NULL, CsrBtAvrcpTgUtilMsgQueueRemoveCB);
}

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
static CsrBtAvrcpTgMp *CsrBtAvrcpTgUtilGetMpIndex(CsrCmnList_t *MpList, CsrUint32 index)
{
    CsrUintFast32 i;
    CsrBtAvrcpTgMp *mp;
    
    mp = AVRCP_LIST_TG_MP_GET_FIRST(MpList);
    
    for(i=0; mp!=NULL && i<index; i++)
    {
        mp = mp->next;
    }
    
    return mp;
}

void CsrBtAvrcpTgUtilGetFolderItemsMPListBuild(AvrcpInstanceData_t *instData, CsrUint16 *txDataLen, CsrUint8 **txData, CsrUint32 startIdx, CsrUint32 endIdx)
{
    CsrBtAvrcpTgMp *mp;
    CsrUint16 index, nameLen;
    CsrUint32 mpCnt = endIdx - startIdx + 1;
    /* Determine total length of response */
    *txDataLen = AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_GFI_RES_MIN_HEADER_SIZE;

    if ((AVRCP_LIST_TG_MP_GET_COUNT(&instData->tgLocal.mpList) - startIdx) <= mpCnt)
    {
        mpCnt = AVRCP_LIST_TG_MP_GET_COUNT(&instData->tgLocal.mpList) - startIdx;
    }

    for (mp = CsrBtAvrcpTgUtilGetMpIndex(&instData->tgLocal.mpList,startIdx); (mp && (mpCnt > 0)); mp = mp->next)
    {
        *txDataLen += (CsrUint16)(CSR_BT_AVRCP_LIB_GFI_MP_PART_SIZE + CsrStrLen((char*)mp->playerName));
        mpCnt--;
    }

    *txData = CsrPmemAlloc(*txDataLen);
    mpCnt = endIdx - startIdx + 1;
    if ((AVRCP_LIST_TG_MP_GET_COUNT(&instData->tgLocal.mpList) - startIdx) <= mpCnt)
    {
        mpCnt = AVRCP_LIST_TG_MP_GET_COUNT(&instData->tgLocal.mpList) - startIdx;
    }
    /* Insert the common header */
    (*txData)[AVRCP_DATA_PDU_GFI_RES_STATUS_INDEX] = CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(0x0000, &(*txData)[AVRCP_DATA_PDU_GFI_RES_UID_COUNT_INDEX]);
    CSR_COPY_UINT16_TO_BIG_ENDIAN(mpCnt, &(*txData)[AVRCP_DATA_PDU_GFI_RES_NUM_OF_ITEMS_INDEX]);

    index = AVRCP_DATA_PDU_GFI_RES_ITEM_LIST_INDEX;
    /* Insert the MP information */

    for (mp = CsrBtAvrcpTgUtilGetMpIndex(&instData->tgLocal.mpList,startIdx); (mp && (mpCnt > 0)); mp = mp->next)
    {
        nameLen = (CsrUint16)CsrStrLen((char*)mp->playerName);

        (*txData)[CSR_BT_AVRCP_LIB_GFI_ITEM_TYPE_INDEX+index] = CSR_BT_AVRCP_ITEM_TYPE_MEDIA_PLAYER;
        CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_LIB_GFI_MP_PART_SIZE + nameLen - CSR_BT_AVRCP_LIB_GFI_ITEM_HEADER_SIZE, &(*txData)[CSR_BT_AVRCP_LIB_GFI_ITEM_LENGTH_INDEX + index]);
        CSR_COPY_UINT16_TO_BIG_ENDIAN(mp->mpId, &(*txData)[CSR_BT_AVRCP_LIB_GFI_MP_PLAYER_ID_INDEX + index]);

        /* Player types */
        (*txData)[CSR_BT_AVRCP_LIB_GFI_MP_MAJOR_TYPE_INDEX+index] = mp->majorType;
        CSR_COPY_UINT32_TO_BIG_ENDIAN(mp->subType, &(*txData)[CSR_BT_AVRCP_LIB_GFI_MP_SUB_TYPE_INDEX+index]);

        /* Play status */
        (*txData)[CSR_BT_AVRCP_LIB_GFI_MP_PLAY_STATUS_INDEX+index] = mp->playStatus;

        /* Feature mask */
        CSR_COPY_UINT32_TO_BIG_ENDIAN(mp->featureMask[0], &(*txData)[CSR_BT_AVRCP_LIB_GFI_MP_FEATURE_MASK_INDEX+index]);
        CSR_COPY_UINT32_TO_BIG_ENDIAN(mp->featureMask[1], &(*txData)[CSR_BT_AVRCP_LIB_GFI_MP_FEATURE_MASK_INDEX+4+index]);
        CSR_COPY_UINT32_TO_BIG_ENDIAN(mp->featureMask[2], &(*txData)[CSR_BT_AVRCP_LIB_GFI_MP_FEATURE_MASK_INDEX+8+index]);
        CSR_COPY_UINT32_TO_BIG_ENDIAN(mp->featureMask[3], &(*txData)[CSR_BT_AVRCP_LIB_GFI_MP_FEATURE_MASK_INDEX+12+index]);

        /* Character set */
        CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &(*txData)[CSR_BT_AVRCP_LIB_GFI_MP_CHARSET_INDEX+index]);

        /* Player name */
        CSR_COPY_UINT16_TO_BIG_ENDIAN(nameLen, &(*txData)[CSR_BT_AVRCP_LIB_GFI_MP_NAME_LEN_INDEX+index]);
        CsrMemCpy(&(*txData)[CSR_BT_AVRCP_LIB_GFI_MP_NAME_INDEX+index], mp->playerName, nameLen);

        index += CSR_BT_AVRCP_LIB_GFI_MP_PART_SIZE + nameLen;
        mpCnt--;
    }
}


/* Folder names must be separated by '/' or '\'- it does not matter whether the string starts or ends with '/' or '\' */
CsrUint8 CsrBtAvrcpTgUtilSBPFolderNamesAdd(CsrUint8 **data, CsrUint16 *dataLen, CsrCharString *folderNames)
{
    CsrUint16 folderNamesLength = (CsrUint16)(CsrStrLen((char *)folderNames));
    CsrIntFast16 folderNamesIdx, currentFolderLen;
    CsrUint8 folderCount;

    /* Count the number of folders */
    for (folderNamesIdx = 0, folderCount = 1; folderNamesIdx < folderNamesLength; folderNamesIdx++)
    {
        if ( ((folderNames[folderNamesIdx] == '/') || (folderNames[folderNamesIdx] == '\\')) &&
            ((folderNamesIdx != 0) && (folderNamesIdx != folderNamesLength - 1))) /* Ignore '/' or '\' in the beginning or at the end */
        {
            folderCount++;
        }
    }

    /* Copy the names in length/name pairs */
    *data = CsrPmemAlloc(folderNamesLength + folderCount);
    *dataLen = 0;
    folderNamesIdx = ((folderNames[0] == '/') || (folderNames[0] == '\\')) ? 1 : 0; /* Skip the first '/' */

    while (folderNamesIdx < folderNamesLength)
    {
        /* Determine the length of a single name */
        for (currentFolderLen = 0;
             (folderNames[folderNamesIdx + currentFolderLen] != '/') && (folderNames[folderNamesIdx + currentFolderLen] != '\\') &&(folderNames[folderNamesIdx + currentFolderLen] != '\0');
             currentFolderLen++)
        {
            ;
        }

        /* Insert length and name */
        CSR_COPY_UINT16_TO_BIG_ENDIAN(currentFolderLen, &(*data)[*dataLen]);
        CsrMemCpy(&(*data)[*dataLen + AVRCP_DATA_PDU_SET_BP_FOLDER_LENGTH_SIZE], &(folderNames[folderNamesIdx]), currentFolderLen);
        *dataLen += currentFolderLen + AVRCP_DATA_PDU_SET_BP_FOLDER_LENGTH_SIZE;
        folderNamesIdx += currentFolderLen + 1; /* +1 for '/' or '\0' */
    }

    return folderCount;
}
#endif /* CSR_BT_INSTALL_AVRCP_TG_13_14 */
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MODULE */
#endif

