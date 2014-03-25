/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2011. All rights reserved.

REVISION:      $Revision: #6 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE

#include "csr_bt_util.h"
#include "csr_bt_avrcp_main.h"
#include "csr_bt_avrcp_prim.h"
#include "csr_bt_avrcp_lib.h"

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
/* The variable sizes of the different notification responses */
static const CsrUint8 avrcpTgMsgResponseSizes[] =
{
    1,  /* AVRCP_NOTI_ID_PLAYBACK_STATUS */
    8,  /* AVRCP_NOTI_ID_TRACK */
    0,  /* AVRCP_NOTI_ID_TRACK_END */
    0,  /* AVRCP_NOTI_ID_TRACK_START */
    4,  /* AVRCP_NOTI_ID_PLAYBACK_POS */
    1,  /* AVRCP_NOTI_ID_BATT_STATUS */
    1,  /* AVRCP_NOTI_ID_SYSTEM_STATUS */
    0,  /* AVRCP_NOTI_ID_PAS */
    0,  /* AVRCP_NOTI_ID_NOW_PLAYING_CONTENT */
    0,  /* AVRCP_NOTI_ID_AVAILABLE_PLAYERS */
    4,  /* AVRCP_NOTI_ID_ADDRESSED_PLAYER - Player ID + UID counter */
    2,  /* AVRCP_NOTI_ID_UIDS - UID counter */
    1,  /* AVRCP_NOTI_ID_VOLUME */
};

static CsrBool validAttCmd(CsrUint8 cmdAttId)
{
    if ( ((cmdAttId >= CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID) && (cmdAttId <= CSR_BT_AVRCP_PAS_SCAN_ATT_ID)) ||
         (cmdAttId >= CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN) )
    {
        return TRUE;
    }

    return FALSE;
}

static CsrBool validAttCmdList(CsrUint8 cmdAttCount, CsrUint8 *cmdAttList)
{
    CsrUintFast8 i;

    for (i=0; i<cmdAttCount; i++)
    {
        if (!validAttCmd(cmdAttList[i]))
        {
            return FALSE;
        }
    }

    return TRUE;
}

static CsrBool validGetAttTextData(CsrUint8 cmdAttId, CsrUint8 cmdValId)
{/* Check that the Id received is valid */
  switch (cmdAttId)
  {
    case CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID:
        {
            if ((cmdValId == CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF) || (cmdValId == CSR_BT_AVRCP_PAS_EQUALIZER_VAL_ON))
            {
                return TRUE;
            }
            return FALSE;
        }
    case CSR_BT_AVRCP_PAS_REPEAT_ATT_ID:
        {
            if ((cmdValId >= CSR_BT_AVRCP_PAS_REPEAT_VAL_OFF) && (cmdValId <= CSR_BT_AVRCP_PAS_REPEAT_VAL_GROUP))
            {
                return TRUE;
            }
            return FALSE;
        }
    case CSR_BT_AVRCP_PAS_SHUFFLE_ATT_ID:
        {
            if ((cmdValId >= CSR_BT_AVRCP_PAS_SHUFFLE_VAL_OFF) && (cmdValId <= CSR_BT_AVRCP_PAS_SHUFFLE_VAL_GROUP))
            {
                return TRUE;
            }
            return FALSE;
        }
    case CSR_BT_AVRCP_PAS_SCAN_ATT_ID:
        {
            if ((cmdValId >= CSR_BT_AVRCP_PAS_SCAN_VAL_OFF) && (cmdValId <= CSR_BT_AVRCP_PAS_SCAN_VAL_GROUP))
            {
                return TRUE;
            }
            return FALSE;
        }
    default:
        {
            if (cmdAttId >= CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
  }
}
#endif

/* Reject a pass-through AV/C command */
void CsrBtAvrcpTgRejectAvcSend(AvrcpConnInstance_t *connInst, CsrUint8 *rxData, CsrUint8 rType)
{
    CsrUint8 *txData, opIdVendorLen = 0;
    
    if( (AVRCP_DATA_AVC_OPCODE_GET(rxData) == AVRCP_DATA_AVC_OPCODE_PASS_THROUGH)
        &&  (AVRCP_DATA_PT_OPID_GET(rxData) == CSR_BT_AVRCP_PT_OP_ID_VENDOR_DEP) )
    {
        opIdVendorLen = AVRCP_DATA_MD_BT_COMPANY_ID_SIZE;
    }

    txData = CsrPmemAlloc(AVRCP_DATA_PT_SIZE + opIdVendorLen);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               AVRCP_TLABEL_GET(rxData),            /* Transaction label */
                               AVRCP_DATA_AVCTP_CR_RES);            /* Command or response */

    CsrBtAvrcpDataInsertAvcCommonHeader(txData, rType); /* Will be REJECTED or NOT_IMPLEMENTED */

    CsrBtAvrcpDataInsertAvcPassThroughHeader(txData,
                                        AVRCP_DATA_PT_STATE_GET(rxData),
                                        AVRCP_DATA_PT_OPID_GET(rxData));

    /* add vendor id if operation id is 'vendor unique' in the command */
    txData[AVRCP_DATA_PT_OPID_LEN_INDEX]  = opIdVendorLen;
    CsrMemCpy(&txData[AVRCP_DATA_PT_GN_COMPANY_ID_INDEX], &rxData[AVRCP_DATA_PT_GN_COMPANY_ID_INDEX], opIdVendorLen);
 
    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_PT_SIZE, txData);
}

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
/* Reject an vendor specific AV/C command */
void CsrBtAvrcpTgRejectAvcVendorSend(AvrcpConnInstance_t *connInst, CsrUint8 *rxData, CsrUint8 rType, CsrUint8 status)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_STATUS_HEADER_SIZE);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               AVRCP_TLABEL_GET(rxData),            /* Transaction label */
                               AVRCP_DATA_AVCTP_CR_RES);            /* Command or response */

    CsrBtAvrcpDataInsertAvcCommonHeader(txData, rType);

    CsrBtAvrcpDataInsertAvcVendorHeader(txData,
                                   rxData[AVRCP_DATA_MD_PDU_ID_INDEX],
                                   AVRCP_DATA_MD_PACKET_TYPE_SINGLE,/* MD packet type */
                                   AVRCP_DATA_STATUS_HEADER_SIZE);  /* Parameter length */

    txData[AVRCP_DATA_STATUS_AVC_INDEX] = status;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_STATUS_HEADER_SIZE, txData);
}

void CsrBtAvrcpTgGeneralRejectBrowsingSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint8 status)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_GENERAL_REJECT_RES_HEADER_SIZE);

    CsrBtAvrcpDataInsertBrowsingHeader(txData,
                                  tLabel,
                                  AVRCP_DATA_AVCTP_CR_RES,
                                  AVRCP_DATA_PDU_ID_GENERAL_REJECT,
                                  AVRCP_DATA_PDU_GENERAL_REJECT_RES_HEADER_SIZE);

    txData[AVRCP_DATA_PDU_GENERAL_REJECT_RES_REASON_INDEX] = status;

    CsrBtAvrcpBrowsingDataSend(connInst, AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_GENERAL_REJECT_RES_HEADER_SIZE, txData);
}

void CsrBtAvrcpTgNormalRejectBrowsingSend(AvrcpConnInstance_t *connInst, CsrUint8 *rxData, CsrUint8 status)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_STATUS_HEADER_SIZE);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               AVRCP_TLABEL_GET(rxData),            /* Transaction label */
                               AVRCP_DATA_AVCTP_CR_RES);            /* Command or response */

    CSR_COPY_UINT16_TO_BIG_ENDIAN(AVRCP_DATA_STATUS_HEADER_SIZE, &txData[AVRCP_DATA_BROWSING_PARAM_LEN_INDEX]);
    txData[AVRCP_DATA_BROWSING_PDU_ID_INDEX] = AVRCP_DATA_BROWSING_PDU_ID_GET(rxData);
    txData[AVRCP_DATA_BROWSING_PARAM_INDEX] = status;

    CsrBtAvrcpBrowsingDataSend(connInst, AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_STATUS_HEADER_SIZE, txData);
}

void CsrBtAvrcpTgRegisterNotificationRspSend(AvrcpConnInstance_t *connInst, CsrUint8 notiId, CsrUint8 *notiData, CsrUint8 rspType,CsrBool ApChanged)
{
    CsrUint8 rspSize;
    CsrUint8 *txData;
    if (ApChanged)
    {
        rspSize = 1;
    }
    else
    {
        rspSize =  avrcpTgMsgResponseSizes[notiId - CSR_BT_AVRCP_NOTI_ID_OFFSET] + 1; /* +1 to include noti type */
    }

    txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + rspSize);

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->tgLocal.notiList[notiId - CSR_BT_AVRCP_NOTI_ID_OFFSET], /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_RES,          /* Command or response */
                              rspType,                          /* Command/response type */
                              AVRCP_DATA_PDU_ID_REG_NOTI,       /* PDU ID */
                              rspSize);/* MD parameter length */
    if (ApChanged)
    {
        txData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX] = CSR_BT_AVRCP_STATUS_ADDR_PLAYER_CHANGED;
    }
    else
    {
        txData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX] = notiId;
        CsrMemCpy(&txData[AVRCP_DATA_PDU_REG_NOTI_RES_VAL_INDEX], notiData, rspSize - 1);
    }

    CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + rspSize), txData);
}

void CsrBtAvrcpTgRegisterNotificationPasRspSend(AvrcpConnInstance_t *connInst, CsrUint8 rType, CsrUint8 pairsCount, CsrBtAvrcpPasAttValPair *pairs)
{
    CsrUint16 additionalSize = AVRCP_DATA_PDU_REG_NOTI_RES_PAS_HEADER_SIZE + pairsCount * AVRCP_DATA_PDU_REG_NOTI_RES_PAS_PART_SIZE;
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + additionalSize);
    CsrUintFast8 i;

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->tgLocal.notiList[CSR_BT_AVRCP_NOTI_ID_PAS - CSR_BT_AVRCP_NOTI_ID_OFFSET], /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_RES,          /* Command or response */
                              rType,      /* Command/response type */
                              AVRCP_DATA_PDU_ID_REG_NOTI,       /* PDU ID */
                              additionalSize); /* MD parameter length */

    txData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX] = CSR_BT_AVRCP_NOTI_ID_PAS;
    txData[AVRCP_DATA_PDU_REG_NOTI_RES_PAS_NUM_INDEX] = pairsCount;

    for (i = 0; i < pairsCount; i++)
    {
        txData[AVRCP_DATA_PDU_REG_NOTI_RES_PAS_ATT_INDEX + i * AVRCP_DATA_PDU_REG_NOTI_RES_PAS_PART_SIZE] = pairs[i].attribId;
        txData[AVRCP_DATA_PDU_REG_NOTI_RES_PAS_VAL_INDEX + i * AVRCP_DATA_PDU_REG_NOTI_RES_PAS_PART_SIZE] = pairs[i].valueId;
    }

    CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + additionalSize), txData);
}

void CsrBtAvrcpTgGetElementAttributesRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel)
{
    CsrBtAvrcpTgGetAttributesRes *resMsg = connInst->tgLocal.getAttribResPrim;
    CsrUint16 txAttribIndex;
    CsrUint16 attribDataToCopy = 0;
    CsrUint16 txDataLen = 0, paramLen = 0;
    CsrUint8 *txData = NULL;

    if ( resMsg->attribDataLen > (AVRCP_DATA_AVCTP_HEADER_SIZE_SINGLE + AVRCP_DATA_AVC_MAX_SIZE) )
    {/* AV/C fragmentation is required */
        switch (connInst->tgLocal.packetType)
        {
            case AVRCP_DATA_MD_PACKET_TYPE_SINGLE: /* Initial state */
            {
                connInst->tgLocal.packetType = AVRCP_DATA_MD_PACKET_TYPE_START;
                connInst->tgLocal.getAttribIndex = AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ID_INDEX;

                txDataLen           = AVRCP_DATA_AVC_MAX_SIZE;
                txData              = CsrPmemAlloc(AVRCP_DATA_AVC_MAX_SIZE);
                paramLen            = AVRCP_DATA_AVC_MAX_SIZE - AVRCP_DATA_MD_HEADER_SIZE;
                attribDataToCopy    = paramLen - 1; /* -1 for Item count field */
                break;
            }

            case AVRCP_DATA_MD_PACKET_TYPE_START: /* Fall-through */
            case AVRCP_DATA_MD_PACKET_TYPE_CONTINUE:
            {/* Continue or end frame? */
                if ( AVRCP_DATA_AVC_MAX_SIZE < (resMsg->attribDataLen - connInst->tgLocal.getAttribIndex + AVRCP_DATA_MD_HEADER_SIZE))
                {/* There is NOT room for the rest of the data in this frame */
                    connInst->tgLocal.packetType = AVRCP_DATA_MD_PACKET_TYPE_CONTINUE;

                    txDataLen           = AVRCP_DATA_AVC_MAX_SIZE;
                    txData              = CsrPmemAlloc(AVRCP_DATA_AVC_MAX_SIZE);
                    attribDataToCopy    = AVRCP_DATA_AVC_MAX_SIZE - AVRCP_DATA_MD_HEADER_SIZE;
                    paramLen            = attribDataToCopy;
                }
                else
                {
                    connInst->tgLocal.packetType = AVRCP_DATA_MD_PACKET_TYPE_END;

                    txDataLen           = AVRCP_DATA_MD_HEADER_SIZE + resMsg->attribDataLen - connInst->tgLocal.getAttribIndex - 1;
                    txData              = CsrPmemAlloc(txDataLen);
                    paramLen            = txDataLen - AVRCP_DATA_MD_HEADER_SIZE;
                    attribDataToCopy    = paramLen;
                }
                break;
            }

            /* case AVRCP_DATA_MD_PACKET_TYPE_END */
            default:
            {/* Should not occur */
                return;
            }

        }
    }
    else
    {/* The data can be sent in a single frame */
        connInst->tgLocal.packetType = AVRCP_DATA_MD_PACKET_TYPE_SINGLE;

        txDataLen           = resMsg->attribDataLen;
        txData              = resMsg->attribData;
        resMsg->attribData  = NULL;
        paramLen            = resMsg->attribDataLen - AVRCP_DATA_MD_HEADER_SIZE;
    }

    /* Insert the standard AVCTP and AV/C headers */
    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               tLabel,
                               AVRCP_DATA_AVCTP_CR_RES);

    CsrBtAvrcpDataInsertAvcCommonHeader(txData, CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE);

    CsrBtAvrcpDataInsertAvcVendorHeader(txData,
                                   AVRCP_DATA_PDU_ID_GET_ELEMENT_ATTRIBUTES,
                                   connInst->tgLocal.packetType,
                                   paramLen);

    if ((connInst->tgLocal.packetType == AVRCP_DATA_MD_PACKET_TYPE_SINGLE) ||
        (connInst->tgLocal.packetType == AVRCP_DATA_MD_PACKET_TYPE_START))
    {
        txData[AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ATT_COUNT_INDEX] = resMsg->attribCount;
        txAttribIndex = AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ID_INDEX;
    }
    else
    {
        txAttribIndex = AVRCP_DATA_MD_PARAM_INDEX; /* 'Attribute count' field is only present in single/start packets */
    }

    if (connInst->tgLocal.packetType != AVRCP_DATA_MD_PACKET_TYPE_SINGLE)
    {
        CsrMemCpy(&txData[txAttribIndex], resMsg->attribData + connInst->tgLocal.getAttribIndex, attribDataToCopy);

        /* Update getAttribIndex */
        connInst->tgLocal.getAttribIndex += attribDataToCopy;
    }

    if ((connInst->tgLocal.packetType == AVRCP_DATA_MD_PACKET_TYPE_END) ||
        (connInst->tgLocal.packetType == AVRCP_DATA_MD_PACKET_TYPE_SINGLE))
    {/* The fragmentation is complete */
        CsrBtAvrcpTgUtilContinuingRspReset(connInst, TRUE);
    }

    CsrBtAvrcpControlDataSend(connInst, txDataLen, txData);
}

void CsrBtAvrcpTgGetItemAttributesRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint16 txDataLen, CsrUint8 *txData, CsrUint8 attribCount, CsrUint8 status)
{/* *txData has been prepared in advance by the application */
    CsrBtAvrcpDataInsertBrowsingHeader(txData,
                            tLabel,
                            AVRCP_DATA_AVCTP_CR_RES,
                            AVRCP_DATA_PDU_ID_GET_ITEM_ATTRIBUTES,
                            (CsrUint16)(txDataLen - AVRCP_DATA_BROWSING_HEADER_SIZE));

    txData[AVRCP_DATA_PDU_GIA_RES_STATUS_INDEX] = status;
    txData[AVRCP_DATA_PDU_GIA_RES_NOA_INDEX]    = attribCount;

    /* All of the items are already preinserted - just send the response */
    CsrBtAvrcpBrowsingDataSend(connInst, (CsrUint16)txDataLen, txData);
}

void CsrBtAvrcpTgGetPlayStatusRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint32 songLength, CsrBtAvrcpPlaybackPos songPosition, CsrBtAvrcpPlaybackStatus playStatus)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_HEADER_SIZE);

    CsrBtAvrcpDataVendorDataInsert(txData,                                           /* Data to transmit */
                              tLabel,                                           /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_RES,                          /* Command or response */
                              CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE,                      /* Command/response type */
                              AVRCP_DATA_PDU_ID_GET_PLAY_STATUS,                /* PDU ID */
                              AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_HEADER_SIZE);  /* MD parameter length */

    CSR_COPY_UINT32_TO_BIG_ENDIAN(songLength, &txData[AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_SL_INDEX]);
    CSR_COPY_UINT32_TO_BIG_ENDIAN(songPosition, &txData[AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_SP_INDEX]);
    txData[AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_PS_INDEX] = playStatus;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_HEADER_SIZE, txData);
}

static void csrBtAvrcpTgPasGetAttTxtRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint8 cmdAttIdCount, CsrUint8 *cmdAttId)
{
    CsrUint8 *pas = connInst->tgLocal.mpAddressed->pas;
    CsrUint16 pasLen = connInst->tgLocal.mpAddressed->pasLen;
    CsrUintFast16 cmdAttIndex, tgAttIndex;
    CsrUint16 rspSize = 0, rspIndex = 0, rspPartSize;
    CsrUint8 rspAttCount = 0;
    CsrUint8 *txData;
    CsrBool fragmented = FALSE;

    /* Determine total length of message */
    for (cmdAttIndex = 0; cmdAttIndex < cmdAttIdCount; cmdAttIndex++)
    {
        tgAttIndex = CsrBtAvrcpTgLibPasAttribGet(pasLen, pas, cmdAttId[cmdAttIndex]);

        if (tgAttIndex != CSR_BT_AVRCP_TG_LIB_PAS_INVALID_INDEX)
        {
            rspPartSize = AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_PART_SIZE + pas[tgAttIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_TXT_LEN_IDX];

            /* Determine if there is room for the next attribute - only send complete attributes */
            if ((rspSize + rspPartSize + AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_HEADER_SIZE) > AVRCP_DATA_AVC_MAX_SIZE)
            {/* Response will become too large to fit in a single AV/C frame - fragmentation is required */
                if (connInst->tgLocal.pduId == AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT)
                {/* We are already processing a fragmented AV/C sequence */
                    connInst->tgLocal.packetType = AVRCP_DATA_MD_PACKET_TYPE_CONTINUE;
                }
                else
                {/* This is the first AV/C fragment */
                    connInst->tgLocal.pduId = AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT;
                    connInst->tgLocal.packetType = AVRCP_DATA_MD_PACKET_TYPE_START;
                }

                CsrBtAvrcpTgUtilContinuingRspReset(connInst, FALSE);

                connInst->tgLocal.attIdCount = cmdAttIdCount - rspAttCount; /* Number of attribute IDs remaining */
                connInst->tgLocal.attId = CsrMemDup(&cmdAttId[rspAttCount], connInst->tgLocal.attIdCount);
                fragmented = TRUE;
                break;
            }
            else
            {/* There is room for the attribute in the response */
                rspAttCount++;
                rspSize += rspPartSize;
            }
        }
    }

    if (!fragmented && (connInst->tgLocal.pduId == AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT))
    {/* This is the last AV/C fragment and we are already handling a fragmented frame */
        connInst->tgLocal.packetType = AVRCP_DATA_MD_PACKET_TYPE_END;
    }

    /* Prepare response */
    txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_HEADER_SIZE + rspSize);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               tLabel,
                               AVRCP_DATA_AVCTP_CR_RES);

    CsrBtAvrcpDataInsertAvcCommonHeader(txData, CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE);

    CsrBtAvrcpDataInsertAvcVendorHeader(txData,
                                   AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT,
                                   connInst->tgLocal.packetType,
                                   (CsrUint16)(AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_HEADER_SIZE + rspSize));

    txData[AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_NUM_INDEX] = rspAttCount;

    /* Insert the attribute text parts */
    for (cmdAttIndex = 0; cmdAttIndex < rspAttCount; cmdAttIndex++)
    {
        tgAttIndex = CsrBtAvrcpTgLibPasAttribGet(pasLen, pas, cmdAttId[cmdAttIndex]);

        if (tgAttIndex != CSR_BT_AVRCP_TG_LIB_PAS_INVALID_INDEX)
        {
            /* Attribute index */
            txData[AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_ATT_ID_INDEX + rspIndex] = pas[tgAttIndex];

            /* Character set */
            CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &txData[AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_CSET_INDEX + rspIndex]);

            /* Insert attribute text length + text */
            txData[AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_TXT_LEN_INDEX + rspIndex] = pas[tgAttIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_TXT_LEN_IDX];
            CsrMemCpy(&txData[AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_TXT_INDEX + rspIndex],
                   &pas[tgAttIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_TXT_IDX],
                   pas[tgAttIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_TXT_LEN_IDX]);

            rspIndex += AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_PART_SIZE + pas[tgAttIndex + CSR_BT_AVRCP_TG_LIB_PAS_ATT_TXT_LEN_IDX];
        }
    }

    CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_HEADER_SIZE + rspSize), txData);
}

static CsrBool csrBtAvrcpTgPasGetValTxtRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint8 cmdAttId, CsrUint8 cmdValIdCount, CsrUint8 *cmdValId)
{
    CsrUint8 *pas = connInst->tgLocal.mpAddressed->pas;
    CsrUint16 pasLen = connInst->tgLocal.mpAddressed->pasLen;

    if (CsrBtAvrcpTgLibPasAttribGet(pasLen, pas, cmdAttId) != CSR_BT_AVRCP_TG_LIB_PAS_INVALID_INDEX)
    {
        CsrUintFast16 cmdValIndex, tgValIndex;
        CsrUint16 rspSize = 0, rspIndex = 0, rspPartSize;
        CsrUint8 rspValCount = 0;
        CsrUint8 *txData;
        CsrBool fragmented = FALSE;

        /* Determine total length of message */
        for (cmdValIndex = 0; cmdValIndex < cmdValIdCount; cmdValIndex++)
        {
            tgValIndex = CsrBtAvrcpTgLibPasValueGet(pasLen, pas, cmdAttId, cmdValId[cmdValIndex]);

            if (tgValIndex != CSR_BT_AVRCP_TG_LIB_PAS_INVALID_INDEX)
            {
                rspPartSize = AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_PART_SIZE + pas[tgValIndex + CSR_BT_AVRCP_TG_LIB_PAS_VAL_TXT_LEN_IDX];

                /* Determine if there is room for the next value - only send complete values */
                if ((rspSize + rspPartSize + AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_HEADER_SIZE) > AVRCP_DATA_AVC_MAX_SIZE)
                {/* Response will become too large to fit in a single AV/C frame - fragmentation is required */
                    if (connInst->tgLocal.pduId == AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT)
                    {/* We are already processing a fragmented AV/C sequence */
                        connInst->tgLocal.packetType = AVRCP_DATA_MD_PACKET_TYPE_CONTINUE;
                    }
                    else
                    {/* This is the first AV/C fragment */
                        connInst->tgLocal.pduId      = AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT;
                        connInst->tgLocal.packetType = AVRCP_DATA_MD_PACKET_TYPE_START;
                    }

                    CsrBtAvrcpTgUtilContinuingRspReset(connInst, FALSE);

                    connInst->tgLocal.valIdFromAttId    = cmdAttId;
                    connInst->tgLocal.valIdCount        = cmdValIdCount - rspValCount; /* Number fo attribute IDs remaining */
                    connInst->tgLocal.valId             = CsrMemDup(&cmdValId[rspValCount], connInst->tgLocal.valIdCount);
                    fragmented = TRUE;
                    break;
                }
                else
                {/* There is room for the value in the response */
                    rspValCount++;
                    rspSize += rspPartSize;
                }
            }
        }

        if (!fragmented && (connInst->tgLocal.pduId == AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT))
        {/* This is the last AV/C fragment */
            connInst->tgLocal.packetType = AVRCP_DATA_MD_PACKET_TYPE_END;
        }

        /* Prepare response */
        txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_HEADER_SIZE + rspSize);

        CsrBtAvrcpDataInsertAvctpHeader(txData,
                                   tLabel,
                                   AVRCP_DATA_AVCTP_CR_RES);

        CsrBtAvrcpDataInsertAvcCommonHeader(txData, CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE);

        CsrBtAvrcpDataInsertAvcVendorHeader(txData,
                                       AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT,
                                       connInst->tgLocal.packetType,
                                       (CsrUint16)(AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_HEADER_SIZE + rspSize));

        txData[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_NUM_INDEX] = rspValCount;

        /* Insert the attribute text parts */
        for (cmdValIndex = 0; cmdValIndex < rspValCount; cmdValIndex++)
        {
            tgValIndex = CsrBtAvrcpTgLibPasValueGet(pasLen, pas, cmdAttId, cmdValId[cmdValIndex]);

            if (tgValIndex != CSR_BT_AVRCP_TG_LIB_PAS_INVALID_INDEX)
            {
                /* Value index */
                txData[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_VAL_ID_INDEX + rspIndex] = pas[tgValIndex];

                /* Character set */
                CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &txData[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_CSET_INDEX + rspIndex]);

                /* Insert attribute text length + text */
                txData[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_TXT_LEN_INDEX + rspIndex] = pas[tgValIndex + CSR_BT_AVRCP_TG_LIB_PAS_VAL_TXT_LEN_IDX];
                CsrMemCpy(&txData[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_TXT_INDEX + rspIndex],
                       &pas[tgValIndex + CSR_BT_AVRCP_TG_LIB_PAS_VAL_TXT_IDX],
                       pas[tgValIndex + CSR_BT_AVRCP_TG_LIB_PAS_VAL_TXT_LEN_IDX]);

                /*  */
                rspIndex += AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_PART_SIZE + pas[tgValIndex + CSR_BT_AVRCP_TG_LIB_PAS_VAL_TXT_LEN_IDX];
            }
        }

        CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_HEADER_SIZE + rspSize), txData);
        return TRUE;
    }

    return FALSE;
}

void CsrBtAvrcpTgSetVolumeRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint8 volume)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_VOLUME_RES_HEADER_SIZE);

    CsrBtAvrcpDataInsertControlHeader(txData,
                            tLabel,
                            AVRCP_DATA_AVCTP_CR_RES,
                            CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED,
                            AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL,
                            0x00, /* Subunit ID*/
                            AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT,
                            AVRCP_DATA_PDU_ID_SET_ABSOLUTE_VOLUME,
                            AVRCP_DATA_PDU_SET_VOLUME_RES_HEADER_SIZE);

    txData[AVRCP_DATA_PDU_SET_VOLUME_RES_INDEX] = volume;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_VOLUME_RES_HEADER_SIZE, txData);
}

void CsrBtAvrcpTgSetAddressedPlayerRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status, CsrUint8 cType)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_AP_RES_HEADER_SIZE);

    CsrBtAvrcpDataInsertControlHeader(txData,
                            tLabel,
                            AVRCP_DATA_AVCTP_CR_RES,
                            cType,
                            AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL,
                            0x00, /* Subunit ID*/
                            AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT,
                            AVRCP_DATA_PDU_ID_SET_ADDRESSED_PLAYER,
                            AVRCP_DATA_PDU_SET_AP_RES_HEADER_SIZE);

    txData[AVRCP_DATA_PDU_SET_AP_RES_STATUS_INDEX] = status;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_AP_RES_HEADER_SIZE, txData);
}

void CsrBtAvrcpTgSetBrowsedPlayerRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status, CsrUint16 uidCounter, CsrUint32 itemsCount, CsrUint8 folderDepth, CsrUint16 foldersNameLen, CsrUint8 *folderNames)
{
    CsrUint8 *txData;
    CsrUint16 txLength = (CsrUint16)(AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SET_BP_RES_MIN_HEADER_SIZE + foldersNameLen);
    txData = CsrPmemAlloc(txLength);

    CsrBtAvrcpDataInsertBrowsingHeader(txData,
                            tLabel,
                            AVRCP_DATA_AVCTP_CR_RES,
                            AVRCP_DATA_PDU_ID_SET_BROWSED_PLAYER,
                            (CsrUint16)(txLength - AVRCP_DATA_BROWSING_HEADER_SIZE));

    txData[AVRCP_DATA_PDU_SET_BP_RES_STATUS_INDEX] = status;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(uidCounter, &txData[AVRCP_DATA_PDU_SET_BP_RES_UID_COUNT_INDEX]);
    CSR_COPY_UINT32_TO_BIG_ENDIAN(itemsCount, &txData[AVRCP_DATA_PDU_SET_BP_RES_NUMBER_OF_ITEMS_INDEX]);
    CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &txData[AVRCP_DATA_PDU_SET_BP_RES_CHARSET_INDEX]);
    txData[AVRCP_DATA_PDU_SET_BP_RES_FOLDER_DEPTH_INDEX] = folderDepth;
    CsrMemCpy(&txData[AVRCP_DATA_PDU_SET_BP_RES_FOLDER_PAIRS_INDEX], folderNames, foldersNameLen);

    CsrBtAvrcpBrowsingDataSend(connInst, txLength, txData);
}

void CsrBtAvrcpTgGetFolderItemsRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint16 txDataLen, CsrUint8 *txData, CsrUint16 itemCount, CsrUint16 uidCounter)
{/* *txData is preallocated from application to avoid allocation/moving data overhead */
    CsrBtAvrcpDataInsertBrowsingHeader(txData,
                            tLabel,
                            AVRCP_DATA_AVCTP_CR_RES,
                            AVRCP_DATA_PDU_ID_GET_FOLDER_ITEMS,
                            (CsrUint16)(txDataLen - AVRCP_DATA_BROWSING_HEADER_SIZE));

    txData[AVRCP_DATA_PDU_GFI_RES_STATUS_INDEX] = CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(uidCounter, &txData[AVRCP_DATA_PDU_GFI_RES_UID_COUNT_INDEX]);
    CSR_COPY_UINT16_TO_BIG_ENDIAN(itemCount, &txData[AVRCP_DATA_PDU_GFI_RES_NUM_OF_ITEMS_INDEX]);

    /* The items are preinserted - just send the response */
    CsrBtAvrcpBrowsingDataSend(connInst, txDataLen, txData);
}

void CsrBtAvrcpTgChangePathRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status, CsrUint32 itemCount)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_CHANGE_PATH_RES_HEADER_SIZE);

    CsrBtAvrcpDataInsertBrowsingHeader(txData,
                            tLabel,
                            AVRCP_DATA_AVCTP_CR_RES,
                            AVRCP_DATA_PDU_ID_CHANGE_PATH,
                            AVRCP_DATA_PDU_CHANGE_PATH_RES_HEADER_SIZE);

    txData[AVRCP_DATA_PDU_CHANGE_PATH_RES_STATUS_INDEX] = status;
    CSR_COPY_UINT32_TO_BIG_ENDIAN(itemCount, &txData[AVRCP_DATA_PDU_CHANGE_PATH_RES_NUM_OF_ITEMS_INDEX]);

    CsrBtAvrcpBrowsingDataSend(connInst, AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_CHANGE_PATH_RES_HEADER_SIZE, txData);
}

void CsrBtAvrcpTgPlayRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PLAY_ITEM_RES_HEADER_SIZE);

    CsrBtAvrcpDataInsertControlHeader(txData,
                            tLabel,
                            AVRCP_DATA_AVCTP_CR_RES,
                            CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED,
                            AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL,
                            0x00, /* Subunit ID*/
                            AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT,
                            AVRCP_DATA_PDU_ID_PLAY_ITEM,
                            AVRCP_DATA_PDU_PLAY_ITEM_RES_HEADER_SIZE);

    txData[AVRCP_DATA_PDU_PLAY_ITEM_RES_STATUS_INDEX] = status;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PLAY_ITEM_RES_HEADER_SIZE, txData);
}

void CsrBtAvrcpTgSearchRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status, CsrUint16 uidCount, CsrUint32 itemCount)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SEARCH_RES_HEADER_SIZE);

    CsrBtAvrcpDataInsertBrowsingHeader(txData,
                            tLabel,
                            AVRCP_DATA_AVCTP_CR_RES,
                            AVRCP_DATA_PDU_ID_SEARCH,
                            AVRCP_DATA_PDU_SEARCH_RES_HEADER_SIZE);

    txData[AVRCP_DATA_PDU_SEARCH_RES_STATUS_INDEX] = status;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(uidCount, &txData[AVRCP_DATA_PDU_SEARCH_RES_UID_COUNT_INDEX]);
    CSR_COPY_UINT32_TO_BIG_ENDIAN(itemCount, &txData[AVRCP_DATA_PDU_SEARCH_RES_ITEM_COUNT_INDEX]);

    CsrBtAvrcpBrowsingDataSend(connInst, AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SEARCH_RES_HEADER_SIZE, txData);
}

void CsrBtAvrcpTgAddToNPLRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_ADD_TO_NPL_RES_HEADER_SIZE);

    CsrBtAvrcpDataInsertControlHeader(txData,
                            tLabel,
                            AVRCP_DATA_AVCTP_CR_RES,
                            CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED,
                            AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL,
                            0x00, /* Subunit ID*/
                            AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT,
                            AVRCP_DATA_PDU_ID_ADD_TO_NOW_PLAYING,
                            AVRCP_DATA_PDU_ADD_TO_NPL_RES_HEADER_SIZE);

    txData[AVRCP_DATA_PDU_ADD_TO_NPL_RES_STATUS_INDEX] = status;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_ADD_TO_NPL_RES_HEADER_SIZE, txData);
}

static void csrBtAvrcpTgGetCapCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_CAP_CMD_HEADER_SIZE)) && (connInst->tgLocal.mpAddressed))
    {
        if ((*rxData)[AVRCP_DATA_MD_PARAM_INDEX] == AVRCP_DATA_PDU_GET_CAP_CMN_COMP_ID)
        {
            CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_CAP_RES_HEADER_SIZE + AVRCP_DATA_MD_BT_COMPANY_ID_SIZE);

            CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                                      AVRCP_TLABEL_GET((*rxData)),         /* Transaction label */
                                      AVRCP_DATA_AVCTP_CR_RES,          /* Command or response */
                                      CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE,      /* Command/response type */
                                      AVRCP_DATA_PDU_ID_GET_CAPABILITIES,/* PDU ID */
                                      AVRCP_DATA_PDU_GET_CAP_RES_HEADER_SIZE + AVRCP_DATA_MD_BT_COMPANY_ID_SIZE); /* MD parameter length */

            txData[AVRCP_DATA_PDU_GET_CAP_CMN_ID_INDEX] = AVRCP_DATA_PDU_GET_CAP_CMN_COMP_ID;

            /* Insert the BT company ID */
            txData[AVRCP_DATA_PDU_GET_CAP_RES_COUNT_INDEX]     = 0x01; /* CapabilityCount */
            CSR_COPY_UINT24_TO_BIG_ENDIAN(AVRCP_DATA_MD_BT_COMPANY_ID, &txData[AVRCP_DATA_PDU_GET_CAP_RES_CAPA_INDEX]);

            CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_CAP_RES_HEADER_SIZE + AVRCP_DATA_MD_BT_COMPANY_ID_SIZE, txData);
        }
        else if ((*rxData)[AVRCP_DATA_MD_PARAM_INDEX] == AVRCP_DATA_PDU_GET_CAP_CMN_NOTI_SUP)
        {
            /* Reply with the supported notifications */
            CsrUint32 notificationMask = connInst->tgLocal.mpAddressed->notificationMask;
            CsrUint8 notificationCount = CsrBitCountSparse(notificationMask);
            CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_CAP_RES_HEADER_SIZE + notificationCount);
            CsrUint8 notificationFlag, notificationIndex = 0;

            CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                                      AVRCP_TLABEL_GET((*rxData)),         /* Transaction label */
                                      AVRCP_DATA_AVCTP_CR_RES,          /* Command or response */
                                      CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE,      /* Command/response type */
                                      AVRCP_DATA_PDU_ID_GET_CAPABILITIES,/* PDU ID */
                                      (CsrUint16)(AVRCP_DATA_PDU_GET_CAP_RES_HEADER_SIZE + notificationCount)); /* MD parameter length */

            txData[AVRCP_DATA_PDU_GET_CAP_CMN_ID_INDEX]    = AVRCP_DATA_PDU_GET_CAP_CMN_NOTI_SUP;
            txData[AVRCP_DATA_PDU_GET_CAP_RES_COUNT_INDEX] = notificationCount;

            for (notificationFlag = 0; notificationFlag < CSR_BT_AVRCP_NOTI_ID_MAXIMUM; notificationFlag++)
            {
                if (CSR_BIT_IS_SET(notificationMask, notificationFlag))
                {
                    txData[AVRCP_DATA_PDU_GET_CAP_RES_CAPA_INDEX + notificationIndex++] = notificationFlag + CSR_BT_AVRCP_NOTI_ID_OFFSET;
                }
            }

            CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_CAP_RES_HEADER_SIZE + notificationCount), txData);
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
        }
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgRegisterNotificationCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (((rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_CMD_HEADER_SIZE))) && (connInst->tgLocal.mpAddressed))
    {
        if (((*rxData)[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX] <= CSR_BT_AVRCP_NOTI_ID_MAXIMUM) &&
            (CSR_BIT_IS_SET(connInst->tgLocal.mpAddressed->notificationMask, (*rxData)[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX] - CSR_BT_AVRCP_NOTI_ID_OFFSET)))
        {/* Notification is supported by the addressed player - it doesn't matter if it is already registered */
            AvrcpTgPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                                                                             CSR_BT_AVCTP_PSM,
                                                                             CSR_BT_AVRCP_TG_NOTI_IND,
                                                                             CSR_BT_AVRCP_TIMER_MTP,
                                                                             rxDataLen,
                                                                             rxData);

            switch (pendingMsgInfo->rxData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX])
            {
                case CSR_BT_AVRCP_NOTI_ID_PAS:
                {
                    /* Use a special indication for PAS */
                    CsrUint16 pasLen = connInst->tgLocal.mpAddressed->pasLen;
                    CsrUint8 *pas    = connInst->tgLocal.mpAddressed->pas;

                    if (pasLen && pas)
                    {/* PAS is supported by media player */
                        CsrUint8         attIdCount  = CsrBtAvrcpTgLibPasAttribCount(pasLen, pas);
                        CsrBtAvrcpPasAttId *attId      = CsrPmemAlloc(attIdCount * sizeof(CsrBtAvrcpPasAttId));
                        CsrUint16        attIndex    = 0;
                        CsrUintFast8     i;

                        for (i = 0; i < attIdCount; i++)
                        {
                            attId[i] = pas[attIndex];
                            CsrBtAvrcpTgLibPasAttribNext(pasLen, pas, &attIndex);
                        }

                        CsrBtAvrcpTgPasCurrentIndSend(pendingMsgInfo, attIdCount, attId);
                    }
                    else
                    {
                        CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
                        AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
                    }
                    break;
                }

                case CSR_BT_AVRCP_NOTI_ID_ADDRESSED_PLAYER:
                {
                    if (connInst->tgLocal.mpAddressed)
                    {
                        CsrUint8 notiData[sizeof(CsrUint32)];
                        /* Generate response: default or chosen media player. */
                        CSR_COPY_UINT16_TO_BIG_ENDIAN((CsrUint16)connInst->tgLocal.mpAddressed->mpId, notiData);
                        CSR_COPY_UINT16_TO_BIG_ENDIAN(0, notiData + sizeof(CsrUint16));

                        CSR_BIT_SET(connInst->tgLocal.notificationsActive, CSR_BT_AVRCP_NOTI_ID_ADDRESSED_PLAYER - CSR_BT_AVRCP_NOTI_ID_OFFSET);
                        connInst->tgLocal.notiList[CSR_BT_AVRCP_NOTI_ID_ADDRESSED_PLAYER - CSR_BT_AVRCP_NOTI_ID_OFFSET] = AVRCP_TLABEL_GET(pendingMsgInfo->rxData);

                        CsrBtAvrcpTgRegisterNotificationRspSend(connInst,
                                                                CSR_BT_AVRCP_NOTI_ID_ADDRESSED_PLAYER,
                                                                notiData,
                                                                CSR_BT_AVRCP_DATA_AVC_RTYPE_INTERIM,
                                                                FALSE);
                    }
                    else
                    {
                        CsrBtAvrcpTgRejectAvcVendorSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
                    }
                    AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
                    break;
                }

                default:
                {/* Use a common TG indication for all other notifications */
                    CsrBtAvrcpTgNotiIndSend(pendingMsgInfo,
                                            pendingMsgInfo->rxData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX],
                                            CSR_GET_UINT32_FROM_BIG_ENDIAN(&(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_REG_NOTI_CMD_PLAY_INT_INDEX]));
                    /* Send automatic response whenever possible...*/
                    if (pendingMsgInfo->rxData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX] == CSR_BT_AVRCP_NOTI_ID_AVAILABLE_PLAYERS)
                    {
                        CsrUint8 notiData[CSR_BT_AVRCP_TG_NOTI_MAX_SIZE];

                        CsrMemSet(notiData,0,CSR_BT_AVRCP_TG_NOTI_MAX_SIZE);

                        CsrBtAvrcpTgNotiResSend(pendingMsgInfo->connInst->appConnId,
                                    CSR_BT_AVRCP_NOTI_ID_AVAILABLE_PLAYERS,
                                    notiData,
                                    CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE,
                                    pendingMsgInfo->msgId);
                    }
                    break;
                }
            }
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
        }
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_COMMAND);
    }
}

static void csrBtAvrcpTgGetElemAttCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_HEADER_SIZE + (*rxData)[AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_NUM_INDEX]*AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ATT_SIZE))
        && (connInst->tgLocal.mpAddressed))
    {
        CsrUint32 attribId, attribMask = 0;
        if ((*rxData)[AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_NUM_INDEX] == 0)
        {/* NumAttributes = 0; ignore attributeId field(s) and return all attribute information */
            attribMask = CSR_BT_AVRCP_ITEM_ATT_MASK_ALL;
        }
        else
        {
            CsrUint8 i;
            /* Generate attributeMask */
            for (i = 0; i < (*rxData)[AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_NUM_INDEX]; i++)
            {
                attribId = CSR_GET_UINT32_FROM_BIG_ENDIAN(&(*rxData)[AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ATT_INDEX + i * AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ATT_SIZE]);

                if ((attribId >= CSR_BT_AVRCP_ITEM_ATT_MINIMUM) && (attribId <= CSR_BT_AVRCP_ITEM_ATT_COUNT))
                {/* The attribute is in the allowed range */
                    CSR_BIT_SET(attribMask, attribId - CSR_BT_AVRCP_ITEM_ATT_OFFSET_FROM_MASK);
                }
            }
        }

        if ((attribMask <= CSR_BT_AVRCP_ITEM_ATT_MASK_ALL) && (attribMask > 0))
        {
            CsrBtAvrcpUid AvrcpUid;

            /* Prepare and send indication to application */
            AvrcpTgPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                                         CSR_BT_AVCTP_PSM,
                                         CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES,
                                         CSR_BT_AVRCP_TIMER_MTP,
                                         rxDataLen,
                                         rxData);
            CsrMemCpy(AvrcpUid, &(pendingMsgInfo->rxData)[AVRCP_DATA_MD_PARAM_INDEX], sizeof(CsrBtAvrcpUid));
            CsrBtAvrcpTgGetAttributesIndSend(pendingMsgInfo,
                                        connInst->tgLocal.mpAddressed,
                                        attribMask,
                                        0xFFF0, /* maxData, virtually no upper limit because of fragmentation */
                                        CSR_BT_AVRCP_SCOPE_NPL,
                                        AvrcpUid,
                                        (CsrUint16)(CSR_GET_UINT16_FROM_BIG_ENDIAN(&(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_GIA_CMD_UID_COUNT_INDEX])));
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
        }
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgGetPlayStatusCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_PLAY_STATUS_CMD_HEADER_SIZE)) && (connInst->tgLocal.mpAddressed))
    {
        AvrcpTgPendingMsgInfo_t *pendingMsgInfo;

        pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                               CSR_BT_AVCTP_PSM,
                               CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES,/* Pending message type */
                               CSR_BT_AVRCP_TIMER_MTP,        /* Time */
                               rxDataLen,
                               rxData);                     /* Received command for later use */
        if (connInst->tgLocal.mpAddressed == NULL)
        {/* No current player chosen yet....*/
            CsrBtAvrcpTgGetPlayStatusRspSend(connInst,
                                            AVRCP_TLABEL_GET(pendingMsgInfo->rxData),
                                            0xFFFFFFFF,
                                            0xFFFFFFFF,
                                            CSR_BT_AVRCP_PLAYBACK_STATUS_ERROR);
            AVRCP_LIST_TG_PMSG_REMOVE(&pendingMsgInfo->connInst->tgLocal.pendingMsgList, pendingMsgInfo);
        }
        else
        {
            CsrBtAvrcpTgGetPlayStatusIndSend(pendingMsgInfo, connInst->tgLocal.mpAddressed);
        }
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgDispCharSetCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (((rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_DISP_CSET_CMD_HEADER_SIZE + (*rxData)[AVRCP_DATA_PDU_DISP_CSET_CMD_NUM_INDEX]*CSR_BT_AVRCP_CHARACTER_SET_SIZE)))
        && (connInst->tgLocal.mpAddressed))
    {
        CsrUint8 i;
        CsrBtAvrcpCharSet *charSet = CsrPmemAlloc(sizeof(CsrBtAvrcpCharSet) * (*rxData)[AVRCP_DATA_PDU_DISP_CSET_CMD_NUM_INDEX]);

        for (i = 0; i < (*rxData)[AVRCP_DATA_PDU_DISP_CSET_CMD_NUM_INDEX]; i++)
        {/* Get the character sets supported */
            charSet[i] = CSR_GET_UINT16_FROM_BIG_ENDIAN(&(*rxData)[AVRCP_DATA_PDU_DISP_CSET_CMD_CSET_INDEX + i*CSR_BT_AVRCP_CHARACTER_SET_SIZE]);
        }


        CsrBtAvrcpTgInformDispCharsetIndSend(connInst->appConnId, connInst->tgLocal.mpAddressed->mpId, connInst->tgLocal.mpAddressed->mpHandle,
                                            (*rxData)[AVRCP_DATA_PDU_DISP_CSET_CMD_NUM_INDEX], charSet);

        CsrBtAvrcpDataSimpleVendorFrameSend(connInst,
                           AVRCP_TLABEL_GET((*rxData)),
                           AVRCP_DATA_AVCTP_CR_RES,                 /* Command/response */
                           CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED,           /* Command/response type */
                           AVRCP_DATA_PDU_ID_INFO_DISP_CHAR_SET);   /* PDU ID */
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgPasListAttCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (connInst->tgLocal.mpAddressed && connInst->tgLocal.mpAddressed->pas)
    {
        CsrUint8 *pas        = connInst->tgLocal.mpAddressed->pas;
        CsrUint16 pasLen     = connInst->tgLocal.mpAddressed->pasLen;
        CsrUint8 attIdCount  = CsrBtAvrcpTgLibPasAttribCount(pasLen, pas);
        CsrUint8 *txData     = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_LIST_ATT_RES_HEADER_SIZE + attIdCount);
        CsrUint16 attIndex   = 0;
        CsrUint8 i;

        CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                                  AVRCP_TLABEL_GET((*rxData)),         /* Transaction label */
                                  AVRCP_DATA_AVCTP_CR_RES,          /* Command or response */
                                  CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE,      /* Command/response type */
                                  AVRCP_DATA_PDU_ID_LIST_PAS_ATTRIBUTES,   /* PDU ID */
                                  (CsrUint16)(AVRCP_DATA_PDU_PAS_LIST_ATT_RES_HEADER_SIZE + attIdCount)); /* MD parameter length */

        txData[AVRCP_DATA_PDU_PAS_LIST_ATT_RES_NUM_INDEX] = attIdCount;

        for (i = 0; i < attIdCount; i++)
        {
            txData[AVRCP_DATA_PDU_PAS_LIST_ATT_RES_ATT_ID_INDEX + i] = pas[attIndex];
            CsrBtAvrcpTgLibPasAttribNext(pasLen, pas, &attIndex);
        }

        CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_LIST_ATT_RES_HEADER_SIZE + attIdCount), txData);
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgPasListValCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_LIST_VAL_CMD_HEADER_SIZE)) &&
        (connInst->tgLocal.mpAddressed && connInst->tgLocal.mpAddressed->pas) &&
        validAttCmd((*rxData)[AVRCP_DATA_PDU_PAS_LIST_VAL_CMD_ATT_ID_INDEX]))
    {
        CsrUint8 *pas        = connInst->tgLocal.mpAddressed->pas;
        CsrUint16 pasLen     = connInst->tgLocal.mpAddressed->pasLen;
        CsrUint8 valIdCount  = CsrBtAvrcpTgLibPasValueCount(pasLen, pas, (*rxData)[AVRCP_DATA_PDU_PAS_LIST_VAL_CMD_ATT_ID_INDEX]);
        CsrUint8 *txData     = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_LIST_VAL_RES_HEADER_SIZE + valIdCount);
        CsrUint16 attIndex   = CsrBtAvrcpTgLibPasAttribGet(pasLen, pas, (*rxData)[AVRCP_DATA_PDU_PAS_LIST_VAL_CMD_ATT_ID_INDEX]);
        CsrUint16 valIndex   = CsrBtAvrcpTgLibPasValueFirstGet(pasLen, pas, attIndex);
        CsrUint8 i;

        CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                                  AVRCP_TLABEL_GET((*rxData)),         /* Transaction label */
                                  AVRCP_DATA_AVCTP_CR_RES,          /* Command or response */
                                  CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE,      /* Command/response type */
                                  AVRCP_DATA_PDU_ID_LIST_PAS_VALUES,   /* PDU ID */
                                  (CsrUint16)(AVRCP_DATA_PDU_PAS_LIST_VAL_RES_HEADER_SIZE + valIdCount)); /* MD parameter length */

        txData[AVRCP_DATA_PDU_PAS_LIST_VAL_RES_NUM_INDEX] = valIdCount;

        for (i = 0; i < valIdCount; i++)
        {
            txData[AVRCP_DATA_PDU_PAS_LIST_ATT_RES_ATT_ID_INDEX + i] = pas[valIndex];
            CsrBtAvrcpTgLibPasValueNext(pasLen, pas, attIndex, &valIndex);
        }

        CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_LIST_VAL_RES_HEADER_SIZE + valIdCount), txData);
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgPasGetCurrValCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_MIN_SIZE)) && /* To make sure at least the number of attributes is present */
        (connInst->tgLocal.mpAddressed && connInst->tgLocal.mpAddressed->pas) &&
        validAttCmd((*rxData)[AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_ATT_ID_INDEX]))
    {
        CsrUint8 i;
        CsrUint8  attIdCount  = (*rxData)[AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_NUM_INDEX];
        CsrBtAvrcpPasAttId *attId    = CsrPmemAlloc(attIdCount * sizeof(CsrBtAvrcpPasAttId));
        AvrcpTgPendingMsgInfo_t *pendingMsgInfo;

        /* Send indication to app */
        pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                                   CSR_BT_AVCTP_PSM,
                                   CSR_BT_AVRCP_TG_PAS_CURRENT_RES,    /* Pending message type */
                                   CSR_BT_AVRCP_TIMER_MTP,             /* Time */
                                   rxDataLen,
                                   rxData);                     /* Received command for later use */
        for (i = 0; i < attIdCount; i++)
        {
            attId[i] = pendingMsgInfo->rxData[AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_ATT_ID_INDEX+i];
        }

        CsrBtAvrcpTgPasCurrentIndSend(pendingMsgInfo, attIdCount, attId);
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgPasSetValueCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_SET_VAL_CMD_MIN_SIZE)) &&
        (connInst->tgLocal.mpAddressed && connInst->tgLocal.mpAddressed->pas) &&
        validGetAttTextData((*rxData)[AVRCP_DATA_PDU_PAS_SET_VAL_CMD_ATT_ID_INDEX],(*rxData)[AVRCP_DATA_PDU_PAS_SET_VAL_CMD_VAL_ID_INDEX]))
    {
        AvrcpTgPendingMsgInfo_t *pendingMsgInfo;
        CsrUint8  valSetCount  = (*rxData)[AVRCP_DATA_PDU_PAS_SET_VAL_CMD_NUM_INDEX];
        CsrBtAvrcpPasAttValPair *valPtr = CsrPmemAlloc(valSetCount * sizeof(CsrBtAvrcpPasAttValPair));

        /* Send indication to app */
        pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                                   CSR_BT_AVCTP_PSM,
                                   CSR_BT_AVRCP_TG_PAS_SET_RES,    /* Pending message type */
                                   CSR_BT_AVRCP_TIMER_MTC,             /* Time */
                                   rxDataLen,
                                   rxData);                     /* Received command for later use */

        CsrMemCpy(valPtr,&(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_PAS_SET_VAL_CMD_ATT_ID_INDEX],valSetCount * sizeof(CsrBtAvrcpPasAttValPair));

        CsrBtAvrcpTgPasSetIndSend(pendingMsgInfo,valSetCount,valPtr);
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgPasGetAttTextCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_MIN_SIZE)) && /* To make sure at least the number of attributes is present */
        (connInst->tgLocal.mpAddressed && connInst->tgLocal.mpAddressed->pas) &&
        validAttCmdList((*rxData)[AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_NUM_INDEX],&(*rxData)[AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_ATT_ID_INDEX]))
    {
        csrBtAvrcpTgPasGetAttTxtRspSend(connInst,
                                   AVRCP_TLABEL_GET((*rxData)),
                                   (*rxData)[AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_NUM_INDEX],
                                   &(*rxData)[AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_ATT_ID_INDEX]);
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgPasGetValTextCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_MIN_SIZE)) && /* To make sure at least the number of values is present */
        (connInst->tgLocal.mpAddressed && connInst->tgLocal.mpAddressed->pas) &&
        (validGetAttTextData((*rxData)[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_ATT_ID_INDEX],(*rxData)[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_VAL_ID_INDEX])))
    {
        if (csrBtAvrcpTgPasGetValTxtRspSend(connInst,
                                   AVRCP_TLABEL_GET((*rxData)),
                                   (*rxData)[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_ATT_ID_INDEX],
                                   (*rxData)[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_NUM_INDEX],
                                   &(*rxData)[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_VAL_ID_INDEX]) == FALSE)
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
        }
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}
#endif

static CsrBool csrBtOpIdSupported(AvrcpConnInstance_t *connInst, CsrBtAvrcpPTOpId recvOpId)
{
    CsrBool retVal = TRUE;

    if (connInst->tgLocal.mpAddressed)
    {/* Only accept pass through commands if acting as TG and addressed media player appointed */
        CsrUint8 idx = 0xFF;
        CsrUint32 mask = 0xFFFFFFFF;

        if (recvOpId <= CSR_BT_AVRCP_PT_OP_ID_INPUT_SEL)
        {
            idx = 0;
        }
        else if (recvOpId <= CSR_BT_AVRCP_PT_OP_ID_VENDOR_DEP)
        {
            idx = 1;
        }

        switch (recvOpId)
        {
            case CSR_BT_AVRCP_PT_OP_ID_SELECT:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_SELECT;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_OP:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_UP;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_DOWN:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_DOWN;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_LEFT:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_LEFT;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_RIGHT:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_RIGHT;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_RIGHT_UP:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_RIGHT_UP;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_RIGHT_DOWN:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_RIGHT_DOWN;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_LEFT_UP:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_LEFT_UP;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_LEFT_DOWN:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_LEFT_DOWN;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_ROOT_MENU:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_ROOT_MENU;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_SETUP_MENU:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_SETUP_MENU;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_CONTENTS_MENU:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_CONTENTS_MENU;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_FAVOURITE_MENU:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_FAVORITE_MENU;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_EXIT:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_EXIT;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_0:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_0;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_1:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_1;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_2:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_2;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_3:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_3;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_4:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_4;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_5:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_5;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_6:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_6;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_7:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_7;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_8:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_8;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_9:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_9;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_DOT:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_DOT;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_ENTER:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_ENTER;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_CLEAR:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_CLEAR;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_CHANNEL_UP:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_CH_UP;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_CHANNEL_DOWN:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_CH_DOWN;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_PREV_CHANNEL:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_CH_PREV;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_SOUND_SEL:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_SOUND_SELECT;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_INPUT_SEL:
                mask = CSR_BT_AVRCP_FEATURE_MASK_0_INPUT_SELECT;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_DISP_INFO:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_DISP_INFO;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_HELP:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_HELP;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_PAGE_UP:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_PAGE_UP;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_PAGE_DOWN:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_PAGE_DOWN;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_POWER:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_POWER;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_VOLUME_UP:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_VOLUME_UP;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_VOLUME_DOWN:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_VOLUME_DOWN;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_MUTE:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_MUTE;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_PLAY:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_PLAY;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_STOP:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_STOP;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_PAUSE:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_PAUSE;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_RECORD:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_RECORD;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_REWIND:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_REWIND;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_FFORWARD:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_FFORWARD;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_EJECT:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_EJECT;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_FORWARD:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_FORWARD;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_BACKWARD:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_BACKWARD;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_ANGLE:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_ANGLE;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_SUBPICTURE:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_SUBPICTURE;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_F1:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_F1;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_F2:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_F2;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_F3:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_F3;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_F4:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_F4;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_F5:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_F5;
                break;
            case CSR_BT_AVRCP_PT_OP_ID_VENDOR_DEP:
                mask = CSR_BT_AVRCP_FEATURE_MASK_1_VENDOR_UNIQUE;
                break;
          }


        if ((idx == 0xFF) || (mask == 0xFFFFFFFF))
        {
            retVal = FALSE;
        }
        else
        {
            retVal = ( (connInst->tgLocal.mpAddressed->featureMask[idx] & mask) > 0) ? TRUE : FALSE;
        }

    }
    else
    {
        retVal = FALSE;
    }

    return retVal;
}

static void csrBtAvrcpTgPassThroughCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (CsrBtAvrcpUtilDataCheckPT(rxDataLen, *rxData))
    {/* Determine if operation ID is supported */
        CsrBtAvrcpPTOpId recvOpId = AVRCP_DATA_PT_OPID_GET(*rxData);

        if ( ((recvOpId <= CSR_BT_AVRCP_PT_OP_ID_EXIT) ||
             ( (recvOpId >= CSR_BT_AVRCP_PT_OP_ID_0) && (recvOpId <= CSR_BT_AVRCP_PT_OP_ID_CLEAR) ) ||
             ( (recvOpId >= CSR_BT_AVRCP_PT_OP_ID_CHANNEL_UP) && (recvOpId <= CSR_BT_AVRCP_PT_OP_ID_PAGE_DOWN) ) ||
             ( (recvOpId >= CSR_BT_AVRCP_PT_OP_ID_POWER) && (recvOpId <= CSR_BT_AVRCP_PT_OP_ID_BACKWARD) ) ||
             (recvOpId == CSR_BT_AVRCP_PT_OP_ID_ANGLE ) || (recvOpId == CSR_BT_AVRCP_PT_OP_ID_SUBPICTURE)  ||
             ( (recvOpId >= CSR_BT_AVRCP_PT_OP_ID_F1) && (recvOpId <= CSR_BT_AVRCP_PT_OP_ID_F5) ) ||
             (recvOpId == CSR_BT_AVRCP_PT_OP_ID_VENDOR_DEP)) &&
             csrBtOpIdSupported( connInst, recvOpId) )
        {
            AvrcpTgPendingMsgInfo_t *pendingMsgInfo;

            /* Send indication to app */
            pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                                   CSR_BT_AVCTP_PSM,
                                   CSR_BT_AVRCP_TG_PASS_THROUGH_RES,   /* Pending message type */
                                   CSR_BT_AVRCP_TIMER_MTC,             /* Time */
                                   rxDataLen,
                                   rxData);                     /* Received command for later use */

            if (AVRCP_DATA_PT_OPID_GET(pendingMsgInfo->rxData) == CSR_BT_AVRCP_PT_OP_ID_VENDOR_DEP)
            {/* Group navigation */
                CsrBtAvrcpTgPassThroughIndSend(pendingMsgInfo,
                                          (CsrUint8)(AVRCP_DATA_PT_GN_OPERATION_GET((pendingMsgInfo->rxData)) == AVRCP_DATA_PT_GN_OPERATION_NEXT ? CSR_BT_AVRCP_PT_OP_ID_GROUP_NAV_NEXT : CSR_BT_AVRCP_PT_OP_ID_GROUP_NAV_PREV),
                                          AVRCP_DATA_PT_STATE_GET((pendingMsgInfo->rxData)));
            }
            else
            {/* Ordinary pass-through */
                CsrBtAvrcpTgPassThroughIndSend(pendingMsgInfo,
                                          AVRCP_DATA_PT_OPID_GET((pendingMsgInfo->rxData)),
                                          AVRCP_DATA_PT_STATE_GET((pendingMsgInfo->rxData)));
            }
        }
        else
        {/* Reply with not implemented */
            CsrBtAvrcpTgRejectAvcSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_NOT_IMP);
        }
    }
    else
    {
        /* Reply with not implemented */
        CsrBtAvrcpTgRejectAvcSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_NOT_IMP);
    }
}

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
static void csrBtAvrcpTgReqContinuingRspHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (rxDataLen >= AVRCP_DATA_PDU_CONTINUING_CMD_HEADER_SIZE)
    {
        if ((connInst->tgLocal.packetType == AVRCP_DATA_MD_PACKET_TYPE_START) ||
            (connInst->tgLocal.packetType == AVRCP_DATA_MD_PACKET_TYPE_CONTINUE))
        {
            switch ((*rxData)[AVRCP_DATA_PDU_CONTINUING_CMD_PDU_ID_INDEX])
            {
                case AVRCP_DATA_PDU_ID_GET_ELEMENT_ATTRIBUTES:
                {
                    CsrBtAvrcpTgGetElementAttributesRspSend(connInst, AVRCP_TLABEL_GET((*rxData)));
                    break;
                }

                case AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT:
                {
                    csrBtAvrcpTgPasGetAttTxtRspSend(connInst,
                                               AVRCP_TLABEL_GET((*rxData)),
                                               connInst->tgLocal.attIdCount,
                                               connInst->tgLocal.attId);
                    break;
                }

                case AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT:
                {
                    csrBtAvrcpTgPasGetValTxtRspSend(connInst,
                                               AVRCP_TLABEL_GET((*rxData)),
                                               connInst->tgLocal.valIdFromAttId,
                                               connInst->tgLocal.valIdCount,
                                               connInst->tgLocal.valId);
                    break;
                }

                default:
                {
                    CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_COMMAND);
                    break;
                }
            }
        }
        else
        {
            CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
        }
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgAbortContinuingRspHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_CONTINUING_RES_HEADER_SIZE);

    /* Always clean everything */
    CsrBtAvrcpTgUtilContinuingRspReset(connInst, TRUE);

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              AVRCP_TLABEL_GET((*rxData)),         /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_RES,          /* Command or response */
                              CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED,    /* Command/response type */
                              AVRCP_DATA_PDU_ID_ABORT_CONTINUING_RES,/* PDU ID */
                              AVRCP_DATA_PDU_CONTINUING_RES_HEADER_SIZE); /* MD parameter length */

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_CONTINUING_RES_HEADER_SIZE, txData);
}

static void csrBtAvrcpTgInformBatteryStatusOfCTHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen >= (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_BATT_HEADER_SIZE)) &&
        ((*rxData)[rxDataLen-1] <= CSR_BT_AVRCP_BATTERY_STATUS_FULL_CHARGE) &&
        ((connInst->instData->tgDetails->roleConfig & CSR_BT_AVRCP_CONFIG_ROLE_TG_BATT_SUPPORT) != 0) 
        && connInst->tgLocal.mpAddressed  && connInst->tgLocal.mpBrowsed)
    {/* Inform the corresponding media player and send response to the remote device */
       CsrBtAvrcpTgBatteryStatusOfCtIndSend(connInst->appConnId,connInst->tgLocal.mpAddressed->mpHandle,
                                            connInst->tgLocal.mpBrowsed->mpId,
                                            (*rxData)[AVRCP_DATA_PDU_REG_NOTI_RES_BATT_LEVEL_INDEX]);
       CsrBtAvrcpDataSimpleVendorFrameSend(connInst,
                   AVRCP_TLABEL_GET((*rxData)),
                   AVRCP_DATA_AVCTP_CR_RES,                 /* Command/response */
                   CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED,           /* Command/response type */
                   AVRCP_DATA_PDU_ID_INFO_BAT_STAT_OF_CT);   /* PDU ID */

    }
    else
    {
        CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, *rxData, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgSetAbsoluteVolumeCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (rxDataLen == (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_VOLUME_CMD_HEADER_SIZE))
    {
        AvrcpTgPendingMsgInfo_t *pendingMsgInfo;

        pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                               CSR_BT_AVCTP_PSM,
                               CSR_BT_AVRCP_TG_SET_VOLUME_RES,/* Pending message type */
                               CSR_BT_AVRCP_TIMER_MTC,        /* Time */
                               rxDataLen,
                               rxData);                     /* Received command for later use */

        CsrBtAvrcpTgSetVolumeIndSend(pendingMsgInfo, (pendingMsgInfo->rxData)[AVRCP_DATA_PDU_SET_VOLUME_CMD_INDEX]);
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, (*rxData), CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgSetAddressedPlayerCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (rxDataLen >= (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SET_AP_CMD_HEADER_SIZE))
    {
        /* Find player */
        CsrUint32 playerId = CSR_GET_UINT16_FROM_BIG_ENDIAN(&(*rxData)[AVRCP_DATA_PDU_SET_AP_CMD_PID_INDEX]);
        CsrBtAvrcpTgMp *mp = AVRCP_LIST_TG_MP_GET_ID(&connInst->instData->tgLocal.mpList, playerId);

        if (mp)
        {
            AvrcpTgPendingMsgInfo_t *pendingMsgInfo;

            pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                                   CSR_BT_AVCTP_PSM,
                                   CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES,/* Pending message type */
                                   CSR_BT_AVRCP_TIMER_MTC,        /* Time */
                                   rxDataLen,
                                   rxData);                     /* Received command for later use */

            CsrBtAvrcpTgSetAddressedPlayerIndSend(pendingMsgInfo, mp);
        }
        else
        {
            CsrBtAvrcpTgSetAddressedPlayerRspSend(connInst, AVRCP_TLABEL_GET(*rxData), CSR_BT_AVRCP_STATUS_INVALID_PLAYER_ID, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED);
        }
    }
    else
    {
        CsrBtAvrcpTgSetAddressedPlayerRspSend(connInst, AVRCP_TLABEL_GET(*rxData), CSR_BT_AVRCP_STATUS_INVALID_PARAMETER, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED);
    }
}

static void csrBtAvrcpTgSetBrowsedPlayerCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (rxDataLen >= (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SET_BP_CMD_HEADER_SIZE))
    {
        /* Find player */
        CsrUint32 playerId = CSR_GET_UINT16_FROM_BIG_ENDIAN(&(*rxData)[AVRCP_DATA_PDU_SET_BP_CMD_PID_INDEX]);
        CsrBtAvrcpTgMp *mp = AVRCP_LIST_TG_MP_GET_ID(&connInst->instData->tgLocal.mpList, playerId);

        if (mp)
        {
            AvrcpTgPendingMsgInfo_t *pendingMsgInfo;

            pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                                   CSR_BT_AVCTP_BROWSING_PSM,
                                   CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES,/* Pending message type */
                                   CSR_BT_AVRCP_TIMER_BROWSING,        /* Time */
                                   rxDataLen,
                                   rxData);                     /* Received command for later use */

            CsrBtAvrcpTgSetBrowsedPlayerIndSend(pendingMsgInfo, mp);
        }
        else
        {
            CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, *rxData, CSR_BT_AVRCP_STATUS_INVALID_PLAYER_ID);
        }
    }
    else
    {
        CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, *rxData, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgGetFolderItemsCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    CsrUint16 dataLenExpected = AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_GFI_CMD_MIN_HEADER_SIZE -1; /* If no attribute count (value 0xFF or 0), no attribute list! */

    if (((*rxData)[AVRCP_DATA_PDU_GFI_CMD_ATT_COUNT_INDEX] > 0) && ((*rxData)[AVRCP_DATA_PDU_GFI_CMD_ATT_COUNT_INDEX] < 0xFF))
    {
        dataLenExpected = AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_GFI_CMD_MIN_HEADER_SIZE + (*rxData)[AVRCP_DATA_PDU_GFI_CMD_ATT_COUNT_INDEX];
    }
    if ((rxDataLen >= dataLenExpected)  && connInst->tgLocal.mpBrowsed)
    {
        AvrcpTgPendingMsgInfo_t *pendingMsgInfo;
        CsrUint32 startIdx = CSR_GET_UINT32_FROM_BIG_ENDIAN(&(*rxData)[AVRCP_DATA_PDU_GFI_CMD_START_INDEX]);
        CsrUint32 endIdx = CSR_GET_UINT32_FROM_BIG_ENDIAN(&(*rxData)[AVRCP_DATA_PDU_GFI_CMD_END_INDEX]);

        pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                               CSR_BT_AVCTP_BROWSING_PSM,
                               CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES,/* Pending message type */
                               CSR_BT_AVRCP_TIMER_BROWSING,        /* Time */
                               rxDataLen,
                               rxData);                     /* Received command for later use */

        if (pendingMsgInfo->rxData[AVRCP_DATA_PDU_GFI_CMD_SCOPE_INDEX] == CSR_BT_AVRCP_SCOPE_MP_LIST)
        {
            if (endIdx >= startIdx)
            {/* Inquire the application about the play status of the media players requested - begin with first player in the request */
                CsrBtAvrcpTgMp  *tmpPlayer = AVRCP_LIST_TG_MP_GET_FIRST(&connInst->instData->tgLocal.mpList);
                CsrUint16 i;
                for (i = 0; i < startIdx; i++)
                {
                    if (tmpPlayer != NULL)
                    {
                        tmpPlayer = tmpPlayer->next;
                    }
                }
                connInst->tgLocal.currentPlayer = tmpPlayer; 
                connInst->tgLocal.itemCnt       = endIdx - startIdx + 1;
                if ((AVRCP_LIST_TG_MP_GET_COUNT(&connInst->instData->tgLocal.mpList) - startIdx) < connInst->tgLocal.itemCnt)
                {
                    connInst->tgLocal.itemCnt = AVRCP_LIST_TG_MP_GET_COUNT(&connInst->instData->tgLocal.mpList) - startIdx;
                }

                if (connInst->tgLocal.currentPlayer)
                {
                    CsrBtAvrcpTgGetPlayStatusIndSend(pendingMsgInfo, connInst->tgLocal.currentPlayer);
                }
                else
                {
                    CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_STATUS_RANGE_OOB);
                }
            }
            else
            {
                CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_STATUS_RANGE_OOB);
            }
        }
        else
        {/* Verify parameters in command */
            if ((pendingMsgInfo->rxData[AVRCP_DATA_PDU_GFI_CMD_SCOPE_INDEX] <= CSR_BT_AVRCP_SCOPE_NPL) &&
                (endIdx >= startIdx))
            {
                CsrUint32 attribMask = 0;

                /* Generate attribute mask */
                if (pendingMsgInfo->rxData[AVRCP_DATA_PDU_GFI_CMD_ATT_COUNT_INDEX] == 0)
                {/* All attributes requested */
                    attribMask = CSR_BT_AVRCP_ITEM_ATT_MASK_ALL;
                }
                else if (pendingMsgInfo->rxData[AVRCP_DATA_PDU_GFI_CMD_ATT_COUNT_INDEX] == 0xFF)
                {/* No attributes requested for each item */
                    attribMask = CSR_BT_AVRCP_ITEM_ATT_MASK_NONE;
                }
                else
                {/* Find out which attributes are requested for each item */
                    CsrUint32 attribRead = 0;
                    CsrUint8 i;

                    for (i = 0; i < pendingMsgInfo->rxData[AVRCP_DATA_PDU_GFI_CMD_ATT_COUNT_INDEX]; i++)
                    {
                        attribRead = CSR_GET_UINT32_FROM_BIG_ENDIAN(&(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_GFI_CMD_ATT_LIST_INDEX + (i * sizeof(CsrUint32))]);
                        CSR_BIT_SET(attribMask, attribRead - CSR_BT_AVRCP_ITEM_ATT_OFFSET_FROM_MASK);
                    }
                }

                CsrBtAvrcpTgGetFolderItemsIndSend(pendingMsgInfo,
                                             pendingMsgInfo->rxData[AVRCP_DATA_PDU_GFI_CMD_SCOPE_INDEX],
                                             startIdx, endIdx, attribMask, connInst->browsing.mtu - AVRCP_DATA_BROWSING_HEADER_SIZE);
            }
            else
            {
                CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, pendingMsgInfo->rxData, CSR_BT_AVRCP_STATUS_RANGE_OOB);
            }
        }
    }
    else
    {
        CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, *rxData, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgChangePathCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen >= (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_CHANGE_PATH_CMD_HEADER_SIZE))  && connInst->tgLocal.mpBrowsed)
    {
        AvrcpTgPendingMsgInfo_t *pendingMsgInfo;

        pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                               CSR_BT_AVCTP_BROWSING_PSM,
                               CSR_BT_AVRCP_TG_CHANGE_PATH_RES,/* Pending message type */
                               CSR_BT_AVRCP_TIMER_BROWSING,        /* Time */
                               rxDataLen,
                               rxData);                     /* Received command for later use */

        CsrBtAvrcpTgChangePathIndSend(pendingMsgInfo,
                                 pendingMsgInfo->rxData[AVRCP_DATA_PDU_CHANGE_PATH_CMD_DIRECTION_INDEX],
                                 &(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_CHANGE_PATH_CMD_FOLDER_UID_INDEX]);

    }
    else
    {
        CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, *rxData, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgPlayItemCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PLAY_ITEM_CMD_HEADER_SIZE))
    {
        AvrcpTgPendingMsgInfo_t *pendingMsgInfo;

        pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                               CSR_BT_AVCTP_PSM,
                               CSR_BT_AVRCP_TG_PLAY_RES,/* Pending message type */
                               CSR_BT_AVRCP_TIMER_MTC,        /* Time */
                               rxDataLen,
                               rxData);                     /* Received command for later use */

        CsrBtAvrcpTgPlayIndSend(pendingMsgInfo,
                           (pendingMsgInfo->rxData)[AVRCP_DATA_PDU_PLAY_ITEM_CMD_SCOPE_INDEX],
                           (CsrUint16)CSR_GET_UINT16_FROM_BIG_ENDIAN(&(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_PLAY_ITEM_CMD_UID_COUNT_INDEX]),
                           &(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_PLAY_ITEM_CMD_ITEM_UID_INDEX]);
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgSearchCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    CsrUint16 rxPayloadLength = CSR_GET_UINT16_FROM_BIG_ENDIAN(&(*rxData)[AVRCP_DATA_PDU_SEARCH_CMD_LEN_INDEX]);

    if (rxDataLen >= (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SEARCH_CMD_MIN_HEADER_SIZE + rxPayloadLength))
    {
        AvrcpTgPendingMsgInfo_t *pendingMsgInfo;
        CsrUint16 len = (CsrUint16)(CSR_GET_UINT16_FROM_BIG_ENDIAN(&(*rxData)[AVRCP_DATA_PDU_SEARCH_CMD_LEN_INDEX]) + 1);
        CsrCharString *text = CsrPmemAlloc(len);
        CsrMemSet(text,0x00,len); /* Make sure that the text contains a NULL terminator. */
        CsrMemCpy(text,&(*rxData)[AVRCP_DATA_PDU_SEARCH_CMD_SEARCH_TXT_INDEX],len-1);

        pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                               CSR_BT_AVCTP_BROWSING_PSM, /*CSR_BT_AVCTP_PSM,*/
                               CSR_BT_AVRCP_TG_SEARCH_RES,         /* Pending message type */
                               CSR_BT_AVRCP_TIMER_BROWSING_SEARCH, /* Time */
                               rxDataLen,
                               rxData);                     /* Received command for later use */

        CsrBtAvrcpTgSearchIndSend(pendingMsgInfo, text);
    }
    else
    {
        CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, *rxData, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgAddToNPLCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen >= (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_ADD_TO_NPL_CMD_HEADER_SIZE))  && connInst->tgLocal.mpBrowsed)
    {
        AvrcpTgPendingMsgInfo_t *pendingMsgInfo;

        pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                               CSR_BT_AVCTP_PSM, /*CSR_BT_AVCTP_PSM,*/
                               CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES,/* Pending message type */
                               CSR_BT_AVRCP_TIMER_MTC,        /* Time */
                               rxDataLen,
                               rxData);                     /* Received command for later use */

        CsrBtAvrcpTgAddToNPLIndSend(pendingMsgInfo,
                               (pendingMsgInfo->rxData)[AVRCP_DATA_PDU_ADD_TO_NPL_CMD_SCOPE_INDEX],
                               (CsrUint16)CSR_GET_UINT16_FROM_BIG_ENDIAN(&(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_ADD_TO_NPL_CMD_UID_COUNT_INDEX]),
                               &(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_ADD_TO_NPL_CMD_ITEM_UID_INDEX]);
    }
    else
    {
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgGetItemAttCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if ((rxDataLen  >= (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_GIA_CMD_MIN_HEADER_SIZE)) && connInst->tgLocal.mpBrowsed)
    {
        CsrUint32 attribId, attribMask = 0;
        CsrBtAvrcpTgMp *mp = connInst->tgLocal.mpBrowsed;
        CsrBtAvrcpScope scope = (*rxData)[AVRCP_DATA_PDU_GIA_CMD_SCOPE_INDEX];

        if ((*rxData)[AVRCP_DATA_PDU_GIA_CMD_ATTRIB_COUNT_INDEX] == 0)
        {/* NumAttributes = 0; ignore attributeId field(s) and return all attribute information */
            attribMask = CSR_BT_AVRCP_ITEM_ATT_MASK_ALL;
        }
        else
        {
            CsrUint8 i;
            /* Generate attributeMask */
            for (i = 0; i < (*rxData)[AVRCP_DATA_PDU_GIA_CMD_ATTRIB_COUNT_INDEX]; i++)
            {
                attribId = CSR_GET_UINT32_FROM_BIG_ENDIAN(&(*rxData)[AVRCP_DATA_PDU_GIA_CMD_ATTRIB_INDEX + i * AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ATT_SIZE]);

                if ((attribId >= CSR_BT_AVRCP_ITEM_ATT_MINIMUM) && (attribId <= CSR_BT_AVRCP_ITEM_ATT_COUNT))
                {/* The attribute is in the allowed range */
                    CSR_BIT_SET(attribMask, attribId - CSR_BT_AVRCP_ITEM_ATT_OFFSET_FROM_MASK);
                }
            }
        }

        if ((attribMask <= CSR_BT_AVRCP_ITEM_ATT_MASK_ALL) && (attribMask > 0))
        {
            CsrBtAvrcpUid AvrcpUid;
            /* Prepare and send indication to application */
            AvrcpTgPendingMsgInfo_t *pendingMsgInfo = CsrBtAvrcpTgUtilMsgQueueAdd(connInst,
                                         CSR_BT_AVCTP_BROWSING_PSM,
                                         CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES,
                                         CSR_BT_AVRCP_TIMER_BROWSING,
                                         rxDataLen,
                                         rxData);

            CsrMemCpy(AvrcpUid, &(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_GIA_CMD_UID_INDEX], sizeof(CsrBtAvrcpUid));

            CsrBtAvrcpTgGetAttributesIndSend(pendingMsgInfo,
                                        mp,
                                        attribMask,
                                        connInst->browsing.mtu - AVRCP_DATA_BROWSING_HEADER_SIZE,
                                        scope,
                                        AvrcpUid,
                                        (CsrUint16)(CSR_GET_UINT16_FROM_BIG_ENDIAN(&(pendingMsgInfo->rxData)[AVRCP_DATA_PDU_GIA_CMD_UID_COUNT_INDEX])));
        }
        else
        {
            CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, *rxData, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
        }
    }
    else
    {
        CsrBtAvrcpTgNormalRejectBrowsingSend(connInst, *rxData, CSR_BT_AVRCP_STATUS_INVALID_PARAMETER);
    }
}

static void csrBtAvrcpTgVendorCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (CsrBtAvrcpUtilDataCheckVendor(rxDataLen, *rxData))
    {
        switch ((*rxData)[AVRCP_DATA_MD_PDU_ID_INDEX])
        {
            case AVRCP_DATA_PDU_ID_GET_CAPABILITIES:
            {
                csrBtAvrcpTgGetCapCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_LIST_PAS_ATTRIBUTES:
            {
                csrBtAvrcpTgPasListAttCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_LIST_PAS_VALUES:
            {
                csrBtAvrcpTgPasListValCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_CUR_PAS_VALUES:
            {
                csrBtAvrcpTgPasGetCurrValCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_SET_PAS_VALUES:
            {
                csrBtAvrcpTgPasSetValueCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT:
            {
                csrBtAvrcpTgPasGetAttTextCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT:
            {
                csrBtAvrcpTgPasGetValTextCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_INFO_DISP_CHAR_SET:
            {
                csrBtAvrcpTgDispCharSetCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_INFO_BAT_STAT_OF_CT:
            {
                csrBtAvrcpTgInformBatteryStatusOfCTHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_ELEMENT_ATTRIBUTES:
            {
                csrBtAvrcpTgGetElemAttCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_PLAY_STATUS:
            {
                csrBtAvrcpTgGetPlayStatusCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_REG_NOTI:
            {
                csrBtAvrcpTgRegisterNotificationCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_REQ_CONTINUING_RES:
            {
                csrBtAvrcpTgReqContinuingRspHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_ABORT_CONTINUING_RES:
            {
                csrBtAvrcpTgAbortContinuingRspHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_SET_ABSOLUTE_VOLUME:
            {
                csrBtAvrcpTgSetAbsoluteVolumeCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_SET_ADDRESSED_PLAYER:
            {
                csrBtAvrcpTgSetAddressedPlayerCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_ADD_TO_NOW_PLAYING:
            {
                csrBtAvrcpTgAddToNPLCmdHandler(connInst, rxDataLen, rxData);
                break;
            }
            case AVRCP_DATA_PDU_ID_PLAY_ITEM:
            {
                csrBtAvrcpTgPlayItemCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            default:
            {
                CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_COMMAND);
                break;
            }
        }
    }
    else
    {/* AVRCP message incorrectly formatted */
        CsrBtAvrcpTgRejectAvcVendorSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED, CSR_BT_AVRCP_STATUS_INVALID_COMMAND);
    }
}
#endif

static void csrBtAvrcpTgUnitInfoCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    /* Note: 11 = minimum unit info cmd */
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_AVC_UNIT_RES_SIZE);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               AVRCP_TLABEL_GET(rxData),    /* Transaction label */
                               AVRCP_DATA_AVCTP_CR_RES);    /* Command or response */

    /* Insert "hardcoded" answer */
    txData[AVRCP_DATA_AVC_CRTYPE_INDEX]                 = CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE;
    txData[AVRCP_DATA_AVC_OPCODE_INDEX]                 = AVRCP_DATA_AVC_OPCODE_UNIT_INFO;
    txData[AVRCP_DATA_AVC_UNIT_RES_07_INDEX]            = 0x07;
    txData[AVRCP_DATA_AVC_UNIT_RES_UNIT_TYPE_INDEX]     = (AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL << AVRCP_DATA_AVC_UNIT_RES_UNIT_TYPE_SHIFT);
    txData[AVRCP_DATA_AVC_UNIT_RES_UNIT_INDEX]          = 0xff;
    txData[AVRCP_DATA_AVC_UNIT_RES_COMP_ID_INDEX]       = 0xFF;
    txData[AVRCP_DATA_AVC_UNIT_RES_COMP_ID_INDEX + 1]   = 0xFF;
    txData[AVRCP_DATA_AVC_UNIT_RES_COMP_ID_INDEX + 2]   = 0xFF;


    CsrBtAvrcpControlDataSend(connInst,
        (CsrUint16)(AVRCP_DATA_AVC_UNIT_RES_SIZE),
        txData);
}

static void csrBtAvrcpTgSubUnitInfoCmdHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_AVC_SUBUNIT_RES_SIZE);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               AVRCP_TLABEL_GET(rxData),            /* Transaction label */
                               AVRCP_DATA_AVCTP_CR_RES);            /* Command or response */

    /* Insert "hardcoded" answer */
    txData[AVRCP_DATA_AVC_CRTYPE_INDEX]                 = CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE;
    txData[AVRCP_DATA_AVC_OPCODE_INDEX]                 = AVRCP_DATA_AVC_OPCODE_SUBUNIT_INFO;
    txData[AVRCP_DATA_AVC_SUBUNIT_RES_EXT_INDEX]        = 0x07; /* 'Page' and reserved fields are set to 0... */
    txData[AVRCP_DATA_AVC_SUBUNIT_RES_SU_TYPE_INDEX]    = (AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL << AVRCP_DATA_AVC_SUBUNIT_RES_SU_TYPE_SHIFT);
    txData[AVRCP_DATA_AVC_SUBUNIT_RES_PD_INDEX]         = 0xFF;
    txData[AVRCP_DATA_AVC_SUBUNIT_RES_PD_INDEX + 1]     = 0xFF;
    txData[AVRCP_DATA_AVC_SUBUNIT_RES_PD_INDEX + 2]     = 0xFF;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_AVC_SUBUNIT_RES_SIZE, txData);
}

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
void CsrBtAvrcpTgRxBrowsingHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (CsrBtAvrcpUtilDataCheckBrowsing(rxDataLen, *rxData))
    {
        switch (AVRCP_DATA_BROWSING_PDU_ID_GET(*rxData))
        {
            case AVRCP_DATA_PDU_ID_SET_BROWSED_PLAYER:
            {
                csrBtAvrcpTgSetBrowsedPlayerCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_FOLDER_ITEMS:
            {
                csrBtAvrcpTgGetFolderItemsCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_CHANGE_PATH:
            {
                csrBtAvrcpTgChangePathCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_SEARCH:
            {
                csrBtAvrcpTgSearchCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_ITEM_ATTRIBUTES:
            {
                csrBtAvrcpTgGetItemAttCmdHandler(connInst, rxDataLen, rxData);
                break;
            }

            default:
            {
                CsrBtAvrcpTgGeneralRejectBrowsingSend(connInst, AVRCP_TLABEL_GET(*rxData), CSR_BT_AVRCP_STATUS_INVALID_COMMAND);
                break;
            }
        }
    }
    else
    {
        CsrBtAvrcpTgGeneralRejectBrowsingSend(connInst, AVRCP_TLABEL_GET(*rxData), CSR_BT_AVRCP_STATUS_INVALID_COMMAND);
    }
}
#endif

void CsrBtAvrcpTgRxControlHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    if (CsrBtAvrcpUtilDataCheckAVC(rxDataLen, *rxData))
    {
        if ((*rxData)[AVRCP_DATA_AVCTP_SINGLE_MI_INDEX] == AVRCP_DATA_AVC_CTYPE_NOTIFY)
        {
            switch ((*rxData)[AVRCP_DATA_MD_PDU_ID_INDEX])
            {
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
                case AVRCP_DATA_PDU_ID_REG_NOTI:
                {
                    csrBtAvrcpTgRegisterNotificationCmdHandler(connInst, rxDataLen, rxData);
                    break;
                }
#endif
                default:
                {/* Unknown command - reject */
                    CsrBtAvrcpTgRejectAvcSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_NOT_IMP);
                    break;
                }
            }
        }
        else
        {
            switch ((*rxData)[AVRCP_DATA_AVC_OPCODE_INDEX])
            {
                case AVRCP_DATA_AVC_OPCODE_PASS_THROUGH:
                {
                    csrBtAvrcpTgPassThroughCmdHandler(connInst, rxDataLen, rxData);
                    break;
                }
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
                case AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT:
                {
                    csrBtAvrcpTgVendorCmdHandler(connInst, rxDataLen, rxData);
                    break;
                }
#endif
                case AVRCP_DATA_AVC_OPCODE_UNIT_INFO:
                {
                    csrBtAvrcpTgUnitInfoCmdHandler(connInst, rxDataLen, *rxData);
                    break;
                }

                case AVRCP_DATA_AVC_OPCODE_SUBUNIT_INFO:
                {
                    csrBtAvrcpTgSubUnitInfoCmdHandler(connInst, rxDataLen, *rxData);
                    break;
                }

                default:
                {/* Unknown command - reject */
                    CsrBtAvrcpTgRejectAvcSend(connInst, *rxData, CSR_BT_AVRCP_DATA_AVC_RTYPE_NOT_IMP);
                    break;
                }
            }
        }
    }
}

#endif /* #ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE */
#endif

