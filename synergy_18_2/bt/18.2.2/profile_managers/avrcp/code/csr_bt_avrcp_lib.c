/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_avrcp_lib.h"
#include "csr_msg_transport.h"

void CsrBtAvrcpMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_AVRCP_IFACEQUEUE, CSR_BT_AVRCP_PRIM, msg);
}

void CsrBtCopyBackwards(CsrBtAvrcpUid dst, CsrBtAvrcpUid src, CsrUint16 len)
{
    CsrUintFast16 i;

    for (i=0; i<len; i++)
    {
        dst[len-i-1] = src[i];
    }
}



void CsrBtAvrcpConfigRoleNoSupport(CsrBtAvrcpRoleDetails *details)
{
    details->roleSupported  = FALSE;
    details->roleConfig     = 0;
    details->srAvrcpVersion = CSR_BT_AVRCP_CONFIG_SR_VERSION_10;
    details->srFeatures     = CSR_BT_AVRCP_CONFIG_SR_FEAT_NONE;
    details->providerName   = NULL;
    details->serviceName    = NULL;
}

void CsrBtAvrcpConfigRoleSupport(CsrBtAvrcpRoleDetails          *details,
                            CsrBtAvrcpConfigRoleMask       roleConfig,
                            CsrBtAvrcpConfigSrVersion      srAvrcpVersion,
                            CsrBtAvrcpConfigSrFeatureMask  srFeatures,
                            CsrCharString                    *providerName,
                            CsrCharString                    *serviceName)
{
    details->roleSupported  = TRUE;
    details->roleConfig     = roleConfig;
    details->srAvrcpVersion = srAvrcpVersion;
    details->srFeatures     = srFeatures;
    details->providerName   = providerName;
    details->serviceName    = serviceName;
}

void CsrBtAvrcpUtilFreeRoleDetails(CsrBtAvrcpRoleDetails *ptr)
{
    if (ptr)
    {
        ptr->roleSupported = FALSE;
        CsrPmemFree(ptr->providerName);
        ptr->providerName = NULL;
        CsrPmemFree(ptr->serviceName);
        ptr->serviceName = NULL;
    }
}

void CsrBtAvrcpFreeRemoteFeaturesInd(CsrBtAvrcpRemoteFeaturesInd **msg)
{
    if (*msg)
    {
        CsrBtAvrcpUtilFreeRoleDetails(&(*msg)->tgFeatures);
        CsrBtAvrcpUtilFreeRoleDetails(&(*msg)->ctFeatures);
        CsrPmemFree(*msg);
        *msg = NULL;
    }
}

/******************************* CT-related functions **************************/
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
CsrBool CsrBtAvrcpCtLibPasAttribTxtGet(CsrUint16 pasLen, CsrUint8 *pas, CsrUint16 *index, CsrBtAvrcpPasAttId *attId, CsrBtAvrcpCharSet *charset, CsrUint8 *attTxtLen, CsrUint8 **attTxt)
{
    if (*index == 0)
    {
        *index = CSR_BT_AVRCP_CT_LIB_PAS_TXT_OFFSET;
    }

    if (pasLen >= (*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_MIN_HEADER_SIZE))
    {/* Correct size check passed: now get the text and update the index */
        *attId          = pas[*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_ID_INDEX];
        *charset        = CSR_GET_UINT16_FROM_BIG_ENDIAN(&pas[*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_CS_INDEX]);
        *attTxtLen      = pas[*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_STR_LEN_INDEX];
        *attTxt         = &pas[*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_STR_INDEX];

        if (pasLen < (*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_MIN_PART_SIZE + *attTxtLen))
        {/* Incomplete name is available */
            *attTxtLen = pasLen - (*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_MIN_PART_SIZE);
        }
        *index = *index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_MIN_PART_SIZE + *attTxtLen;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

CsrBool CsrBtAvrcpCtLibPasValueTxtGet(CsrUint16 pasLen, CsrUint8 *pas, CsrUint16 *index, CsrBtAvrcpPasValId *valId, CsrBtAvrcpCharSet *charset, CsrUint8 *valTxtLen, CsrUint8 **valTxt)
{
    if (*index == 0)
    {
        *index = CSR_BT_AVRCP_CT_LIB_PAS_TXT_VAL_OFFSET;
    }

    if (pasLen >= (*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_VAL_MIN_HEADER_SIZE))
    {
        *valId          = pas[*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_VAL_ID_INDEX];
        *charset        = CSR_GET_UINT16_FROM_BIG_ENDIAN(&pas[*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_VAL_CS_INDEX]);
        *valTxtLen      = pas[*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_VAL_STR_LEN_INDEX];
        *valTxt         = &pas[*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_VAL_STR_INDEX];

        if (pasLen < (*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_VAL_MIN_PART_SIZE + *valTxtLen))
        {/* Incomplete name is available */
            *valTxtLen = pasLen - (*index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_VAL_MIN_PART_SIZE);
        }
        *index = *index + CSR_BT_AVRCP_CT_LIB_PAS_TXT_VAL_MIN_PART_SIZE + *valTxtLen;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

CsrBool CsrBtAvrcpCtLibGfiNextGet(CsrUint16 *index,
                            CsrUint16 itemsLen,
                            CsrUint8 *items,
                            CsrBtAvrcpItemType *itemType)
{
    if (*index == 0)
    {
        *index = CSR_BT_AVRCP_LIB_GFI_HEADER_OFFSET;
    }
    else
    {
        *index += CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_ITEM_LENGTH_INDEX]) + CSR_BT_AVRCP_LIB_GFI_ITEM_HEADER_SIZE;
    }

    if (*index < itemsLen)
    {
        *itemType = items[*index + CSR_BT_AVRCP_LIB_GFI_ITEM_TYPE_INDEX];
        return TRUE;
    }
    else
    {
        *itemType = CSR_BT_AVRCP_ITEM_TYPE_INVALID;
        return FALSE;
    }
}


/* Deprecated: use "CsrBtAvrcpCtLibExtGfiMpGet" instead, as it also retrieves the media player ID */
CsrBool CsrBtAvrcpCtLibGfiMpGet(CsrUint16 *index,
                          CsrUint16 itemsLen,
                          CsrUint8 *items,
                          CsrBtAvrcpFolderType *majorType,
                          CsrBtAvrcpMpTypeSub *subType,
                          CsrBtAvrcpPlaybackStatus *playbackStatus,
                          CsrBtAvrcpMpFeatureMask *featureMask,
                          CsrBtAvrcpCharSet *charset,
                          CsrUint16 *playerNameLen,
                          CsrUint8 **playerName)
{
    if (itemsLen >= (*index + CSR_BT_AVRCP_LIB_GFI_MP_PART_SIZE))
    {
        CSR_BT_AVRCP_FEATURE_MASK_COPY(featureMask, &items[*index + CSR_BT_AVRCP_LIB_GFI_MP_FEATURE_MASK_INDEX]);
        *majorType      = items[*index + CSR_BT_AVRCP_LIB_GFI_MP_MAJOR_TYPE_INDEX];
        *subType        = CSR_GET_UINT32_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_MP_SUB_TYPE_INDEX]);
        *playbackStatus = items[*index + CSR_BT_AVRCP_LIB_GFI_MP_PLAY_STATUS_INDEX];
        *charset        = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_MP_CHARSET_INDEX]);
        *playerNameLen  = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_MP_NAME_LEN_INDEX]);
        *playerName     = &items[*index + CSR_BT_AVRCP_LIB_GFI_MP_NAME_INDEX];

        if (itemsLen < (*index + CSR_BT_AVRCP_LIB_GFI_MP_PART_SIZE + *playerNameLen))
        {/* Incomplete name is available */
            *playerNameLen = itemsLen - (*index + CSR_BT_AVRCP_LIB_GFI_MP_PART_SIZE);
        }
        *index = *index + CSR_BT_AVRCP_LIB_GFI_MP_PART_SIZE + *playerNameLen;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

CsrBool CsrBtAvrcpCtLibExtGfiMpGet(CsrUint16 *index,
                          CsrUint16 itemsLen,
                          CsrUint8 *items,
                          CsrUint16 *playerId,
                          CsrBtAvrcpFolderType *majorType,
                          CsrBtAvrcpMpTypeSub *subType,
                          CsrBtAvrcpPlaybackStatus *playbackStatus,
                          CsrBtAvrcpMpFeatureMask *featureMask,
                          CsrBtAvrcpCharSet *charset,
                          CsrUint16 *playerNameLen,
                          CsrUint8 **playerName)
{
    if (itemsLen >= (*index + CSR_BT_AVRCP_LIB_GFI_MP_PART_SIZE))
    {
        CSR_BT_AVRCP_FEATURE_MASK_COPY(featureMask, &items[*index + CSR_BT_AVRCP_LIB_GFI_MP_FEATURE_MASK_INDEX]);
        *playerId       = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_MP_PLAYER_ID_INDEX]);
        *majorType      = items[*index + CSR_BT_AVRCP_LIB_GFI_MP_MAJOR_TYPE_INDEX];
        *subType        = CSR_GET_UINT32_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_MP_SUB_TYPE_INDEX]);
        *playbackStatus = items[*index + CSR_BT_AVRCP_LIB_GFI_MP_PLAY_STATUS_INDEX];
        *charset        = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_MP_CHARSET_INDEX]);
        *playerNameLen  = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_MP_NAME_LEN_INDEX]);
        *playerName     = &items[*index + CSR_BT_AVRCP_LIB_GFI_MP_NAME_INDEX];

        if (itemsLen < (*index + CSR_BT_AVRCP_LIB_GFI_MP_PART_SIZE + *playerNameLen))
        {/* Incomplete name is available */
            *playerNameLen = itemsLen - (*index + CSR_BT_AVRCP_LIB_GFI_MP_PART_SIZE);
        }
        *index = *index + CSR_BT_AVRCP_LIB_GFI_MP_PART_SIZE + *playerNameLen;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

CsrBool CsrBtAvrcpCtLibGfiFolderGet(CsrUint16 *index,
                              CsrUint16 itemsLen,
                              CsrUint8 *items,
                              CsrBtAvrcpUid *folderUid,
                              CsrBtAvrcpFolderType *folderType,
                              CsrBtAvrcpFolderPlayableType *playableType,
                              CsrBtAvrcpCharSet *charset,
                              CsrUint16 *folderNameLen,
                              CsrUint8 **folderName)
{
    if (itemsLen >= (*index + CSR_BT_AVRCP_LIB_GFI_FOLDER_PART_SIZE))
    {
        CSR_BT_AVRCP_UID_COPY(*folderUid, &items[*index + CSR_BT_AVRCP_LIB_GFI_FOLDER_UID_INDEX]);
        *folderType     = items[*index + CSR_BT_AVRCP_LIB_GFI_FOLDER_TYPE_INDEX];
        *playableType   = items[*index + CSR_BT_AVRCP_LIB_GFI_FOLDER_PLAYABLE_INDEX];
        *charset        = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_FOLDER_CHARSET_INDEX]);
        *folderNameLen  = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_FOLDER_NAME_LEN_INDEX]);
        *folderName      = &items[*index + CSR_BT_AVRCP_LIB_GFI_FOLDER_NAME_INDEX];

        if (itemsLen < (*index + CSR_BT_AVRCP_LIB_GFI_FOLDER_PART_SIZE + *folderNameLen))
        {/* Incomplete name is available */
            *folderNameLen = itemsLen - (*index + CSR_BT_AVRCP_LIB_GFI_FOLDER_PART_SIZE);
        }
        *index = *index + CSR_BT_AVRCP_LIB_GFI_FOLDER_PART_SIZE + *folderNameLen;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

CsrBool CsrBtAvrcpCtLibGfiMediaGet(CsrUint16 *index,
                             CsrUint16 itemsLen,
                             CsrUint8 *items,
                             CsrBtAvrcpUid *mediaUid,
                             CsrBtAvrcpMediaType *mediaType,
                             CsrBtAvrcpCharSet *charset,
                             CsrUint16 *mediaNameLen,
                             CsrUint8 **mediaName,
                             CsrUint8 *attributeCount)
{
    if (itemsLen >= (*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_PART_SIZE))
    {
        CSR_BT_AVRCP_UID_COPY(*mediaUid, &items[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_UID_INDEX]);
        *mediaType      = items[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_TYPE_INDEX];
        *charset        = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_CHARSET_INDEX]);
        *mediaNameLen   = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_NAME_LEN_INDEX]);
        *mediaName       = &items[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_NAME_INDEX];
        *attributeCount = items[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_NAME_INDEX + *mediaNameLen];

        if (itemsLen < (*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_PART_SIZE + *mediaNameLen))
        {/* Incomplete name is available */
            *mediaNameLen = itemsLen - (*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_PART_SIZE);
        }
        *index = *index + CSR_BT_AVRCP_LIB_GFI_MEDIA_PART_SIZE + *mediaNameLen + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_COUNT_SIZE;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

CsrBool CsrBtAvrcpCtLibGfiMediaAttributeNextGet(CsrUint16 *mediaIndex,
                                          CsrUint16 *attIndex,
                                          CsrUint16 itemsLen,
                                          CsrUint8 *items,
                                          CsrBtAvrcpItemMediaAttributeId *attribId)
{
    if (*attIndex == 0)
    {/* Point at the first attribute */
        *attIndex = *mediaIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_PART_SIZE +
                CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*mediaIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_NAME_LEN_INDEX]) + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_COUNT_SIZE;

    }
    else
    {/* Skip to next attribute */
        *attIndex += CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE + CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_VAL_LEN_INDEX]);
    }

    if (*attIndex < itemsLen)
    {
        *attribId = CSR_GET_UINT32_FROM_BIG_ENDIAN(&items[*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_ID_INDEX]);
        return TRUE;
    }
    else
    {
        *attribId = CSR_BT_AVRCP_ITEM_ATT_INVALID;
        return FALSE;
    }
}

CsrBool CsrBtAvrcpCtLibGfiMediaAttributeGet(CsrUint16 maxData,
                                    CsrUint16 *attIndex,
                                    CsrUint16 itemsLen,
                                    CsrUint8 *items,
                                    CsrBtAvrcpItemMediaAttributeId *attribId,
                                    CsrBtAvrcpCharSet *charset,
                                    CsrUint16 *attLen,
                                    CsrUint8 **att)
{
    if (itemsLen >= (*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE))
    {
        *attribId   = CSR_GET_UINT32_FROM_BIG_ENDIAN(&items[*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_ID_INDEX]);
        *charset    = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_CHARSET_INDEX]);
        *attLen     = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_VAL_LEN_INDEX]);
        *att         = &items[*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_VAL_INDEX];

        if (itemsLen < (*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE + *attLen))
        {/* Incomplete name is available */
            *attLen = itemsLen - (*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE);
        }
        *attIndex = *attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE + *attLen;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


CsrBool CsrBtAvrcpCtLibItemsAttributeGet(CsrUint16 maxData,
                                         CsrUint16 *attIndex,
                                         CsrUint16 itemsLen,
                                         CsrUint8 *items,
                                         CsrBtAvrcpItemMediaAttributeId *attribId,
                                         CsrBtAvrcpCharSet *charset,
                                         CsrUint16 *attLen,
                                         CsrUint8 **att)
{
    if(*attIndex == 0)
    {
        /* Move the index to point to Attribute List by offsetting GIA mandatory
         * header
         */
        *attIndex = CSR_BT_AVRCP_LIB_GIA_HEADER_OFFSET;
    }

    if (itemsLen >= (*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE))
    {
        *attribId   = CSR_GET_UINT32_FROM_BIG_ENDIAN(&items[*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_ID_INDEX]);
        *charset    = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_CHARSET_INDEX]);
        *attLen     = CSR_GET_UINT16_FROM_BIG_ENDIAN(&items[*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_VAL_LEN_INDEX]);
        *att         = &items[*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_VAL_INDEX];

        if (itemsLen < (*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE + *attLen))
        {/* Incomplete name is available */
            *attLen = itemsLen - (*attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE);
        }
        *attIndex = *attIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE + *attLen;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif /* CSR_BT_INSTALL_AVRCP_CT_13_14 */ 
#endif



/******************************* TG-related functions **************************/
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
void CsrBtAvrcpTgNotiPlaybackStatusRes(CsrUint8 connId, CsrBtAvrcpStatus status, CsrUint32 msgId, CsrBtAvrcpPlaybackStatus playbackStatus)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    notiData[0] = playbackStatus;

    CsrBtAvrcpTgNotiResSend(connId, CSR_BT_AVRCP_NOTI_ID_PLAYBACK_STATUS, notiData, status, msgId);
}

void CsrBtAvrcpTgNotiTrackRes(CsrUint8 connId, CsrBtAvrcpStatus status, CsrUint32 msgId, CsrBtAvrcpUid uid)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    CSR_BT_AVRCP_UID_COPY(notiData, uid);
    CsrBtAvrcpTgNotiResSend(connId, CSR_BT_AVRCP_NOTI_ID_TRACK, notiData, status, msgId);
}

void CsrBtAvrcpTgNotiTrackStartEndRes(CsrUint8 connId, CsrBtAvrcpStatus status, CsrUint32 msgId, CsrBool start)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];
    notiData[0] = 0; /* Unused */
    CsrBtAvrcpTgNotiResSend(connId, (CsrUint8)(start ? CSR_BT_AVRCP_NOTI_ID_TRACK_START : CSR_BT_AVRCP_NOTI_ID_TRACK_END), notiData, status, msgId);
}

void CsrBtAvrcpTgNotiPlaybackPositionRes(CsrUint8 connId, CsrBtAvrcpStatus status, CsrUint32 msgId, CsrUint32 pos)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    CSR_COPY_UINT32_TO_BIG_ENDIAN(pos, notiData);
    CsrBtAvrcpTgNotiResSend(connId, CSR_BT_AVRCP_NOTI_ID_PLAYBACK_POS, notiData, status, msgId);
}

void CsrBtAvrcpTgNotiBatStatusRes(CsrUint8 connId, CsrBtAvrcpStatus status, CsrUint32 msgId, CsrBtAvrcpBatteryStatus batStatus)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    notiData[0] = batStatus;
    CsrBtAvrcpTgNotiResSend(connId, CSR_BT_AVRCP_NOTI_ID_BATT_STATUS, notiData, status, msgId);
}

void CsrBtAvrcpTgNotiSystemStatusRes(CsrUint8 connId, CsrBtAvrcpStatus status, CsrUint32 msgId, CsrBtAvrcpSystemStatus sysStatus)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    notiData[0] = sysStatus;
    CsrBtAvrcpTgNotiResSend(connId, CSR_BT_AVRCP_NOTI_ID_SYSTEM_STATUS, notiData, status, msgId);
}

void CsrBtAvrcpTgNotiNowPlayingRes(CsrUint8 connId, CsrBtAvrcpStatus status, CsrUint32 msgId)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];
    notiData[0] = 0; /* Unused */
    CsrBtAvrcpTgNotiResSend(connId, CSR_BT_AVRCP_NOTI_ID_NOW_PLAYING_CONTENT, notiData, status, msgId);
}

void CsrBtAvrcpTgNotiUidsRes(CsrUint8 connId, CsrBtAvrcpStatus status, CsrUint32 msgId, CsrUint16 uidCounter)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    CSR_COPY_UINT16_TO_BIG_ENDIAN(uidCounter, notiData);
    CsrBtAvrcpTgNotiResSend(connId, CSR_BT_AVRCP_NOTI_ID_UIDS, notiData, status, msgId);
}


void CsrBtAvrcpTgNotiVolumeRes(CsrUint8 connId, CsrBtAvrcpStatus status, CsrUint32 msgId, CsrUint8 volume)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    notiData[0] = volume;
    CsrBtAvrcpTgNotiResSend(connId, CSR_BT_AVRCP_NOTI_ID_VOLUME, notiData, status, msgId);

}

/*
     +----------+-----------+----------------+--------------+--------------+
     | AttId[1] | AttLen[2] | AttValCount[1] | AttTxtLen[1] | AttTxt[n]... | * AttCount
     +----------+-----------+----------------+--------------+--------------+

         +----------+--------------+--------------+
         | ValId[1] | ValTxtLen[1] | ValTxt[n]... | * AttValCount
         +----------+--------------+--------------+
*/

static CsrUint16 csrBtAvrcpTgLibPasReallocate(CsrUint16 *pasLen, CsrUint8 **pas, CsrUint16 requiredSpace)
{
    CsrUint16 newPasIndex = *pasLen;
    CsrUint16 remainder = ((*pasLen / CSR_BT_AVRCP_PAS_CHUNK_SIZE + (*pasLen % CSR_BT_AVRCP_PAS_CHUNK_SIZE ? 1 : 0)) * CSR_BT_AVRCP_PAS_CHUNK_SIZE) - *pasLen;

    if (requiredSpace > remainder)
    {/* Reallocation required */
        CsrUint8 requiredWholeChunks = requiredSpace / CSR_BT_AVRCP_PAS_CHUNK_SIZE;
        CsrUint8 *tmpPas = CsrPmemAlloc(*pasLen + remainder + (requiredWholeChunks + (requiredWholeChunks ? 0 : 1)) * CSR_BT_AVRCP_PAS_CHUNK_SIZE);

        /* Reinsert old data */
        CsrMemCpy(tmpPas, *pas, *pasLen);
        CsrPmemFree(*pas);
        *pas = tmpPas;
    }

    *pasLen += requiredSpace;
    return newPasIndex;
}

CsrUint16 CsrBtAvrcpTgLibPasAttribGet(CsrUint16 pasLen, CsrUint8 *pas, CsrUint8 attId)
{
    CsrUint16 attIndex = 0;

    do
    {
        if (pas[attIndex] == attId)
        {
            if ((pas[attIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_LEN_IDX] + attIndex) <= pasLen)
            {/* The attribute ID matches and the attribute is complete */
                return attIndex;
            }
        }
    } while (CsrBtAvrcpTgLibPasAttribNext(pasLen, pas, &attIndex));

    return CSR_BT_AVRCP_TG_LIB_PAS_INVALID_INDEX;
}

CsrUint16 CsrBtAvrcpTgLibPasValueGet(CsrUint16 pasLen, CsrUint8 *pas, CsrUint8 attId, CsrUint8 valId)
{
    CsrUint16 attIndex = CsrBtAvrcpTgLibPasAttribGet(pasLen, pas, attId);

    if (attIndex != CSR_BT_AVRCP_TG_LIB_PAS_INVALID_INDEX)
    {
        CsrUint16 valIndex = CsrBtAvrcpTgLibPasValueFirstGet(pasLen, pas, attIndex);

        do
        {
            if (pas[valIndex] == valId)
            {
                if ((pas[valIndex + CSR_BT_AVRCP_TG_LIB_PAS_VAL_TXT_LEN_IDX] + valIndex) <= pasLen)
                {/* The value ID matches and the value is complete */
                    return valIndex;
                }
            }
        } while (CsrBtAvrcpTgLibPasValueNext(pasLen, pas, attIndex, &valIndex));
    }

    return CSR_BT_AVRCP_TG_LIB_PAS_INVALID_INDEX;
}

CsrUint16 CsrBtAvrcpTgLibPasValueFirstGet(CsrUint16 pasLen, CsrUint8 *pas, CsrUint16 attIndex)
{
    return (attIndex + pas[attIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_TXT_LEN_IDX] + CSR_BT_AVRCP_TG_LIB_PAS_ATT_SIZE);
}

CsrBool CsrBtAvrcpTgLibPasAttribNext(CsrUint16 pasLen, CsrUint8 *pas, CsrUint16 *attIndex)
{
    if (pas[*attIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_LEN_IDX] < pasLen)
    {
        /* Skip to next attribute */
        CsrUint16 newVal = CSR_GET_UINT16_FROM_LITTLE_ENDIAN(&pas[*attIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_LEN_IDX]);
        if ((*attIndex + newVal) < pasLen)
        {
            *attIndex += newVal;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    /* No more attributes in PAS */
    return FALSE;
}

CsrBool CsrBtAvrcpTgLibPasValueNext(CsrUint16 pasLen, CsrUint8 *pas, CsrUint16 attIdIndex, CsrUint16 *valIndex)
{
    if (((pas[*valIndex + CSR_BT_AVRCP_TG_LIB_PAS_VAL_TXT_LEN_IDX] + CSR_BT_AVRCP_TG_LIB_PAS_VAL_SIZE ) < pasLen) &&
        ((pas[*valIndex + CSR_BT_AVRCP_TG_LIB_PAS_VAL_TXT_LEN_IDX] + CSR_BT_AVRCP_TG_LIB_PAS_VAL_SIZE ) < (pas[attIdIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_LEN_IDX])))
    {
        /* Skip to next value */
        *valIndex += pas[*valIndex + CSR_BT_AVRCP_TG_LIB_PAS_VAL_TXT_LEN_IDX] + CSR_BT_AVRCP_TG_LIB_PAS_VAL_SIZE;
        return TRUE;
    }

    /* No more values in attribute */
    return FALSE;
}


CsrUint8 CsrBtAvrcpTgLibPasAttribCount(CsrUint16 pasLen, CsrUint8 *pas)
{
    CsrUintFast16 pasIndex;
    CsrUint8 attCount = 0;

    for (pasIndex = 0;
         ((pasIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_LEN_IDX + 1) < pasLen) &&
         (CSR_GET_UINT16_FROM_LITTLE_ENDIAN(&pas[pasIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_LEN_IDX]) + pasIndex <= pasLen);
         pasIndex += CSR_GET_UINT16_FROM_LITTLE_ENDIAN(&pas[pasIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_LEN_IDX]))
    {
        attCount++;
    }

    return attCount;
}

CsrUint8 CsrBtAvrcpTgLibPasValueCount(CsrUint16 pasLen, CsrUint8 *pas, CsrUint8 attId)
{
    CsrUint16 attIndex = CsrBtAvrcpTgLibPasAttribGet(pasLen, pas, attId);

    if (attIndex != CSR_BT_AVRCP_TG_LIB_PAS_INVALID_INDEX)
    {
        return pas[attIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_VAL_COUNT_IDX];
    }
    else
    {
        return 0;
    }
}

void CsrBtAvrcpTgLibPasAttribAdd(CsrUint16 *pasLen, CsrUint8 **pas, CsrBtAvrcpPasAttId attId, CsrUtf8String *attTxt)
{
    CsrUint8 attTxtLen = (CsrUint8)CsrStrLen((char*)attTxt);
    CsrUint16 index = csrBtAvrcpTgLibPasReallocate(pasLen, pas, (CsrUint16)(attTxtLen + CSR_BT_AVRCP_TG_LIB_PAS_ATT_SIZE));

    (*pas)[index] = attId;
    (*pas)[index + CSR_BT_AVRCP_TG_LIB_PAS_ATT_TXT_LEN_IDX] = attTxtLen;
    (*pas)[index + CSR_BT_AVRCP_TG_LIB_PAS_ATT_VAL_COUNT_IDX] = 0;
    CsrMemCpy(&(*pas)[index + CSR_BT_AVRCP_TG_LIB_PAS_ATT_TXT_IDX], attTxt, attTxtLen);
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(CSR_BT_AVRCP_TG_LIB_PAS_ATT_SIZE + attTxtLen, &(*pas)[index + CSR_BT_AVRCP_TG_LIB_PAS_ATT_LEN_IDX]);
}

void CsrBtAvrcpTgLibPasValueAdd(CsrUint16 *pasLen, CsrUint8 **pas, CsrBtAvrcpPasAttId attId, CsrBtAvrcpPasValId valId, CsrUtf8String *valTxt)
{
    CsrUint8 valTxtLen = (CsrUint8)CsrStrLen((char*)valTxt);
    CsrUint16 index = csrBtAvrcpTgLibPasReallocate(pasLen, pas, (CsrUint16)(CSR_BT_AVRCP_TG_LIB_PAS_VAL_SIZE + valTxtLen));
    CsrUint16 attIndex = CsrBtAvrcpTgLibPasAttribGet(*pasLen, *pas, attId);

    /* Insert the value specific fields */
    (*pas)[index] = valId;
    (*pas)[index + CSR_BT_AVRCP_TG_LIB_PAS_VAL_TXT_LEN_IDX] = valTxtLen;
    CsrMemCpy(&(*pas)[index + CSR_BT_AVRCP_TG_LIB_PAS_VAL_TXT_IDX], valTxt, valTxtLen);

    /* Update the attribute specific fields */
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(CSR_BT_AVRCP_TG_LIB_PAS_VAL_SIZE + valTxtLen + CSR_GET_UINT16_FROM_LITTLE_ENDIAN(&(*pas)[attIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_LEN_IDX]),
                                      &(*pas)[attIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_LEN_IDX]);

    (*pas)[attIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_VAL_COUNT_IDX] += 1;
}

static void csrBtAvrcpLibGfiInit(CsrUint16 maxData, CsrUint16 *itemsLen, CsrUint8 **items)
{
    if (!*items)
    {
        *items = CsrPmemAlloc(maxData); /* Always allocate the maximum amount of data */
        CsrMemSet(*items, 0, maxData);
        *itemsLen = CSR_BT_AVRCP_LIB_GFI_HEADER_OFFSET; /* Header will be inserted by profile */
    }
}

static void csrBtAvrcpTgLibGfiItemLenUpdate(CsrUint16 itemIndex,
                                       CsrUint8 *item,
                                       CsrUint16 addItemLen)
{
    CsrUint16 currentItemLen = CSR_GET_UINT16_FROM_BIG_ENDIAN(&item[itemIndex + CSR_BT_AVRCP_LIB_GFI_ITEM_LENGTH_INDEX]);
    CSR_COPY_UINT16_TO_BIG_ENDIAN(currentItemLen + addItemLen, &item[itemIndex + CSR_BT_AVRCP_LIB_GFI_ITEM_LENGTH_INDEX]);
}

CsrBool CsrBtAvrcpTgLibGfiFolderAdd(CsrUint16 maxData,
                            CsrUint16 *itemsLen,
                            CsrUint8 **items,
                            CsrBtAvrcpUid *folderUid,
                            CsrBtAvrcpFolderType folderType,
                            CsrBtAvrcpFolderPlayableType playableType,
                            CsrBtAvrcpCharSet charset,
                            CsrCharString *folderName)
{
    CsrUint16 folderNameLen = (CsrUint16)CsrStrLen((char*)folderName);
    CsrUint16 totalItemLen  = (CSR_BT_AVRCP_LIB_GFI_FOLDER_PART_SIZE + folderNameLen);

    csrBtAvrcpLibGfiInit(maxData, itemsLen, items);

    if ((maxData - *itemsLen) >= totalItemLen)
    {/* There is room for the folder */
        /* Item type and item length */
        (*items)[CSR_BT_AVRCP_LIB_GFI_ITEM_TYPE_INDEX + *itemsLen] = CSR_BT_AVRCP_ITEM_TYPE_FOLDER;
        CSR_COPY_UINT16_TO_BIG_ENDIAN(totalItemLen - CSR_BT_AVRCP_LIB_GFI_ITEM_HEADER_SIZE, &(*items)[CSR_BT_AVRCP_LIB_GFI_ITEM_LENGTH_INDEX + *itemsLen]);

        /* Folder details */
        CSR_BT_AVRCP_UID_COPY(&(*items)[CSR_BT_AVRCP_LIB_GFI_FOLDER_UID_INDEX + *itemsLen], *folderUid);
        (*items)[CSR_BT_AVRCP_LIB_GFI_FOLDER_TYPE_INDEX + *itemsLen] = folderType;
        (*items)[CSR_BT_AVRCP_LIB_GFI_FOLDER_PLAYABLE_INDEX + *itemsLen] = playableType;
        CSR_COPY_UINT16_TO_BIG_ENDIAN(charset, &(*items)[CSR_BT_AVRCP_LIB_GFI_FOLDER_CHARSET_INDEX + *itemsLen]);
        CSR_COPY_UINT16_TO_BIG_ENDIAN(folderNameLen, &(*items)[CSR_BT_AVRCP_LIB_GFI_FOLDER_NAME_LEN_INDEX + *itemsLen]);
        CsrMemCpy(&(*items)[CSR_BT_AVRCP_LIB_GFI_FOLDER_NAME_INDEX + *itemsLen], folderName, folderNameLen);

        *itemsLen += totalItemLen;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

CsrBool CsrBtAvrcpTgLibGfiMediaAdd(CsrUint16 maxData,
                           CsrUint16 *index,
                           CsrUint8 **data,
                           CsrUint16 *mediaIndex,
                           CsrBtAvrcpUid *mediaUid,
                           CsrBtAvrcpMediaType mediaType,
                           CsrBtAvrcpCharSet charset,
                           CsrCharString *mediaName)
{
    CsrUint16 mediaNameLen  = (CsrUint16)CsrStrLen((char*)mediaName);
    CsrUint16 itemLen  = (CSR_BT_AVRCP_LIB_GFI_MEDIA_PART_SIZE + mediaNameLen + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_COUNT_SIZE);

    csrBtAvrcpLibGfiInit(maxData, index, data);

    if ((itemLen + *index) <= maxData)
    {/* There is room for the media item */
        *mediaIndex = *index;
        /* Item type and item length */
        (*data)[*index + CSR_BT_AVRCP_LIB_GFI_ITEM_TYPE_INDEX] = CSR_BT_AVRCP_ITEM_TYPE_MEDIA_ELEMENT;
        csrBtAvrcpTgLibGfiItemLenUpdate(*mediaIndex, *data, (CsrUint16)(itemLen - CSR_BT_AVRCP_LIB_GFI_ITEM_HEADER_SIZE));

        /* Media details */
        CSR_BT_AVRCP_UID_COPY(&(*data)[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_UID_INDEX], *mediaUid);
        (*data)[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_TYPE_INDEX] = mediaType;
        CSR_COPY_UINT16_TO_BIG_ENDIAN(charset, &(*data)[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_CHARSET_INDEX]);
        CSR_COPY_UINT16_TO_BIG_ENDIAN(mediaNameLen, &(*data)[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_NAME_LEN_INDEX]);
        CsrMemCpy(&(*data)[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_NAME_INDEX], mediaName, mediaNameLen);
        (*data)[*index + CSR_BT_AVRCP_LIB_GFI_MEDIA_NAME_INDEX + mediaNameLen] = 0;  /* Attribute count is initialized to 0 */

        *index += itemLen;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

CsrBool CsrBtAvrcpTgLibGfiMediaAttributeAdd(CsrUint16 maxData,
                                      CsrUint16 *itemsLen,
                                      CsrUint8 **items,
                                      CsrUint16 mediaIndex,
                                      CsrBtAvrcpItemMediaAttributeId attribId,
                                      CsrBtAvrcpCharSet charset,
                                      CsrUint16 attLen,
                                      CsrUint8 *att)
{
    CsrUint16 totalAttribLen = (CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE + attLen);

    csrBtAvrcpLibGfiInit(maxData, itemsLen, items);

    if ((maxData - *itemsLen) >= totalAttribLen)
    {/* There is room for the media item */
        CsrUint16 mediaNameLen = CSR_GET_UINT16_FROM_BIG_ENDIAN(&(*items)[mediaIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_NAME_LEN_INDEX]);

        /* Update media item length */
        csrBtAvrcpTgLibGfiItemLenUpdate(mediaIndex, *items, totalAttribLen);

        /* Update media item attribute count */
        (*items)[mediaIndex + CSR_BT_AVRCP_LIB_GFI_MEDIA_NAME_INDEX + mediaNameLen]++;

        /* Attribute details */
        CSR_COPY_UINT32_TO_BIG_ENDIAN(attribId, &(*items)[*itemsLen + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_ID_INDEX]);
        CSR_COPY_UINT16_TO_BIG_ENDIAN(charset , &(*items)[*itemsLen + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_CHARSET_INDEX]);
        CSR_COPY_UINT16_TO_BIG_ENDIAN(attLen  , &(*items)[*itemsLen + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_VAL_LEN_INDEX]);
        CsrMemCpy(&(*items)[CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_VAL_INDEX + *itemsLen], att, attLen);

        *itemsLen += totalAttribLen;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void CsrBtAvrcpTgNotiPlaybackStatusReq(CsrUint32 playerId, CsrUint8 playbackStatus)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    notiData[0] = playbackStatus;
    CsrBtAvrcpTgNotiReqSend(playerId, CSR_BT_AVRCP_NOTI_ID_PLAYBACK_STATUS, notiData);
}

void CsrBtAvrcpTgNotiTrackReq(CsrUint32 playerId, CsrBtAvrcpUid uid)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    CSR_BT_AVRCP_UID_COPY(notiData, uid);
    CsrBtAvrcpTgNotiReqSend(playerId, CSR_BT_AVRCP_NOTI_ID_TRACK, notiData);
}

void CsrBtAvrcpTgNotiTrackStartEndReq(CsrUint32 playerId, CsrBool start)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];
    notiData[0] = 0; /* Unused */
    CsrBtAvrcpTgNotiReqSend(playerId, (CsrUint8)(start ? CSR_BT_AVRCP_NOTI_ID_TRACK_START : CSR_BT_AVRCP_NOTI_ID_TRACK_END), notiData);
}

void CsrBtAvrcpTgNotiPlaybackPositionReq(CsrUint32 playerId, CsrUint32 pos)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    CSR_COPY_UINT32_TO_BIG_ENDIAN(pos, notiData);
    CsrBtAvrcpTgNotiReqSend(playerId, CSR_BT_AVRCP_NOTI_ID_PLAYBACK_POS, notiData);
}

void CsrBtAvrcpTgNotiBatStatusReq(CsrUint32 playerId, CsrBtAvrcpBatteryStatus batStatus)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    notiData[0] = batStatus;
    CsrBtAvrcpTgNotiReqSend(playerId, CSR_BT_AVRCP_NOTI_ID_BATT_STATUS, notiData);
}

void CsrBtAvrcpTgNotiSystemStatusReq(CsrUint32 playerId, CsrBtAvrcpSystemStatus sysStatus)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    notiData[0] = sysStatus;
    CsrBtAvrcpTgNotiReqSend(playerId, CSR_BT_AVRCP_NOTI_ID_SYSTEM_STATUS, notiData);
}

void CsrBtAvrcpTgNotiNowPlayingReq(CsrUint32 playerId)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];
    notiData[0] = 0; /* Unused */
    CsrBtAvrcpTgNotiReqSend(playerId, CSR_BT_AVRCP_NOTI_ID_NOW_PLAYING_CONTENT, notiData);
}

void CsrBtAvrcpTgNotiUidsReq(CsrUint32 playerId, CsrUint16 uidCounter)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    CSR_COPY_UINT16_TO_BIG_ENDIAN(uidCounter, notiData);
    CsrBtAvrcpTgNotiReqSend(playerId, CSR_BT_AVRCP_NOTI_ID_UIDS, notiData);
}


void CsrBtAvrcpTgNotiVolumeReq(CsrUint32 playerId, CsrUint8 volume)
{
    CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

    notiData[0] = volume;
    CsrBtAvrcpTgNotiReqSend(playerId, CSR_BT_AVRCP_NOTI_ID_VOLUME, notiData);
}
#endif
#endif /* TG-related functions */


