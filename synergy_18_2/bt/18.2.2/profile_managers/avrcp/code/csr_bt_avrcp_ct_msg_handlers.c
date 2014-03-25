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

void CsrBtAvrcpCtPassThroughCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 opId, CsrUint8 state)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_PT_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_RCP);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               connInst->control.ctTLabel,  /* Transaction label */
                               AVRCP_DATA_AVCTP_CR_CMD);    /* Command or response */

    CsrBtAvrcpDataInsertAvcCommonHeader(txData, AVRCP_DATA_AVC_CTYPE_CONTROL);
    CsrBtAvrcpDataInsertAvcPassThroughHeader(txData, state, opId);
    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_PT_SIZE, txData);
}

#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
void CsrBtAvrcpCtGroupNavigationCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 operation, CsrUint8 state)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_PT_GN_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTC);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               connInst->control.ctTLabel,  /* Transaction label */
                               AVRCP_DATA_AVCTP_CR_CMD);    /* Command or response */

    CsrBtAvrcpDataInsertAvcCommonHeader(txData, AVRCP_DATA_AVC_CTYPE_CONTROL);
    CsrBtAvrcpDataInsertAvcGroupNavigationHeader(txData, state, operation);

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_PT_GN_SIZE, txData);
}

void CsrBtAvrcpCtGetCapabilitiesCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 capaType)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_CAP_CMD_HEADER_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTP);

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->control.ctTLabel,       /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_CMD,          /* Command or response */
                              AVRCP_DATA_AVC_CTYPE_STATUS,      /* Command/response type */
                              AVRCP_DATA_PDU_ID_GET_CAPABILITIES,/* PDU ID */
                              AVRCP_DATA_PDU_GET_CAP_CMD_HEADER_SIZE); /* MD parameter length */

    txData[AVRCP_DATA_PDU_GET_CAP_CMN_ID_INDEX] = capaType;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_CAP_CMD_HEADER_SIZE, txData);
}

void CsrBtAvrcpCtPasListAttCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo)
{
    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTP);

    CsrBtAvrcpDataSimpleVendorFrameSend(connInst,
                                   connInst->control.ctTLabel,
                                   AVRCP_DATA_AVCTP_CR_CMD,           /* Command/response */
                                   AVRCP_DATA_AVC_CTYPE_STATUS,       /* Command/response type */
                                   AVRCP_DATA_PDU_ID_LIST_PAS_ATTRIBUTES);   /* PDU ID */
}

void CsrBtAvrcpCtPasListValCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpPasAttId attId)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_LIST_VAL_CMD_HEADER_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTP);

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->control.ctTLabel,       /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_CMD,          /* Command or response */
                              AVRCP_DATA_AVC_CTYPE_STATUS,      /* Command/response type */
                              AVRCP_DATA_PDU_ID_LIST_PAS_VALUES,   /* PDU ID */
                              AVRCP_DATA_PDU_PAS_LIST_VAL_CMD_HEADER_SIZE); /* MD parameter length */

    txData[AVRCP_DATA_PDU_PAS_LIST_VAL_CMD_ATT_ID_INDEX] = attId;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_LIST_VAL_CMD_HEADER_SIZE, txData);
}

void CsrBtAvrcpCtPasGetCurrentValCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 attIdCount, CsrBtAvrcpPasAttId *attId)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_HEADER_SIZE + attIdCount);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTP);

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->control.ctTLabel,       /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_CMD,          /* Command or response */
                              AVRCP_DATA_AVC_CTYPE_STATUS,      /* Command/response type */
                              AVRCP_DATA_PDU_ID_GET_CUR_PAS_VALUES, /* PDU ID */
                              (CsrUint16)(AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_HEADER_SIZE + attIdCount)); /* MD parameter length */

    txData[AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_NUM_INDEX] = attIdCount;
    CsrMemCpy(&txData[AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_ATT_ID_INDEX], attId, attIdCount);

    CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_HEADER_SIZE + attIdCount), txData);
}

void CsrBtAvrcpCtPasSetValCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 changedPasCount, CsrBtAvrcpPasAttValPair *changedPas)
{
    CsrUint16 additionalSize = AVRCP_DATA_PDU_PAS_SET_VAL_CMD_HEADER_SIZE + changedPasCount * AVRCP_DATA_PDU_PAS_SET_VAL_CMD_PART_SIZE;
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + additionalSize);
    CsrUintFast8 i;

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTC);

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->control.ctTLabel,       /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_CMD,          /* Command or response */
                              AVRCP_DATA_AVC_CTYPE_CONTROL,     /* Command/response type */
                              AVRCP_DATA_PDU_ID_SET_PAS_VALUES,    /* PDU ID */
                              additionalSize);                  /* MD parameter length */

    txData[AVRCP_DATA_PDU_PAS_SET_VAL_CMD_NUM_INDEX] = changedPasCount;

    /* Insert the changed PAS att. IDs and value IDs */
    for (i = 0; i < changedPasCount; i++)
    {
        txData[AVRCP_DATA_PDU_PAS_SET_VAL_CMD_ATT_ID_INDEX + i * AVRCP_DATA_PDU_PAS_SET_VAL_CMD_PART_SIZE] = changedPas[i].attribId;
        txData[AVRCP_DATA_PDU_PAS_SET_VAL_CMD_VAL_ID_INDEX + i * AVRCP_DATA_PDU_PAS_SET_VAL_CMD_PART_SIZE] = changedPas[i].valueId;
    }

    CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + additionalSize), txData);
}

void CsrBtAvrcpCtPasGetAttTextCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 attIdCount, CsrBtAvrcpPasAttId *attId)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_HEADER_SIZE + attIdCount);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTP);

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->control.ctTLabel,       /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_CMD,          /* Command or response */
                              AVRCP_DATA_AVC_CTYPE_STATUS,      /* Command/response type */
                              AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT, /* PDU ID */
                              (CsrUint16)(AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_HEADER_SIZE + attIdCount)); /* MD parameter length */

    txData[AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_NUM_INDEX] = attIdCount;
    CsrMemCpy(&txData[AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_ATT_ID_INDEX], attId, attIdCount);

    CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_HEADER_SIZE + attIdCount), txData);
}

void CsrBtAvrcpCtPasGetValTextCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpPasAttId attId, CsrUint8 valIdCount, CsrBtAvrcpPasValId *valId)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_HEADER_SIZE + valIdCount);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTP);

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->control.ctTLabel,       /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_CMD,          /* Command or response */
                              AVRCP_DATA_AVC_CTYPE_STATUS,      /* Command/response type */
                              AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT, /* PDU ID */
                              (CsrUint16)(AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_HEADER_SIZE + valIdCount)); /* MD parameter length */

    txData[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_ATT_ID_INDEX] = attId;
    txData[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_NUM_INDEX] = valIdCount;
    CsrMemCpy(&txData[AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_VAL_ID_INDEX], valId, valIdCount);

    CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_HEADER_SIZE + valIdCount), txData);
}

void CsrBtAvrcpCtInformDispCharsetCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 charsetCount, CsrBtAvrcpCharSet *charset)
{
    CsrUint8 *txData;
    CsrUintFast8 i;
    CsrUint16 len = (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_DISP_CSET_CMD_HEADER_SIZE + CSR_BT_AVRCP_CHARACTER_SET_SIZE * charsetCount);

    txData = CsrPmemAlloc(len);
    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTC);

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->control.ctTLabel,       /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_CMD,          /* Command or response */
                              AVRCP_DATA_AVC_CTYPE_CONTROL,     /* Command/response type */
                              AVRCP_DATA_PDU_ID_INFO_DISP_CHAR_SET,/* PDU ID */
                              (CsrUint16)(len - AVRCP_DATA_MD_HEADER_SIZE)); /* MD parameter length */

    txData[AVRCP_DATA_PDU_DISP_CSET_CMD_NUM_INDEX] = charsetCount;

    for (i = 0; i < charsetCount; i++)
    {
        CSR_COPY_UINT16_TO_BIG_ENDIAN(charset[i], &txData[AVRCP_DATA_PDU_DISP_CSET_CMD_CSET_INDEX + i * CSR_BT_AVRCP_CHARACTER_SET_SIZE]);
    }

    CsrBtAvrcpControlDataSend(connInst, len, txData);
}

void CsrBtAvrcpCtGetElementAttributesCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint32 attributeMask)
{
    CsrUint8 attributeCount = CsrBitCountSparse(attributeMask);
    CsrUint16 additionalSize = AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_HEADER_SIZE + attributeCount * AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ATT_SIZE;
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + additionalSize);
    CsrUintFast8 i;

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTP);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               connInst->control.ctTLabel,          /* Transaction label */
                               AVRCP_DATA_AVCTP_CR_CMD);            /* Command or response */

    CsrBtAvrcpDataInsertAvcCommonHeader(txData, AVRCP_DATA_AVC_CTYPE_STATUS);

    CsrBtAvrcpDataInsertAvcVendorHeader(txData,
                                   AVRCP_DATA_PDU_ID_GET_ELEMENT_ATTRIBUTES,
                                   AVRCP_DATA_MD_PACKET_TYPE_SINGLE,
                                   additionalSize);

    CsrMemSet(&txData[AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ID_INDEX], 0, AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ID_SIZE);
    txData[AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_NUM_INDEX] = attributeCount;

    /* Insert the attribute IDs */
    for (i = 0; i < CSR_BT_AVRCP_ITEM_ATT_COUNT; i++)
    {
        if (CSR_BIT_IS_SET(attributeMask, i))
        {
            attributeCount--;
            CSR_COPY_UINT32_TO_BIG_ENDIAN(i + CSR_BT_AVRCP_ITEM_ATT_OFFSET_FROM_MASK,
                &txData[AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ATT_INDEX + attributeCount * AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ATT_SIZE]);
        }
    }

    CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_MD_HEADER_SIZE + additionalSize), txData);
}

void CsrBtAvrcpCtGetPlayStatusCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_PLAY_STATUS_CMD_HEADER_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTP);

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->control.ctTLabel,       /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_CMD,          /* Command or response */
                              AVRCP_DATA_AVC_CTYPE_STATUS,      /* Command/response type */
                              AVRCP_DATA_PDU_ID_GET_PLAY_STATUS,       /* PDU ID */
                              AVRCP_DATA_PDU_GET_PLAY_STATUS_CMD_HEADER_SIZE); /* MD parameter length */

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_GET_PLAY_STATUS_CMD_HEADER_SIZE, txData);
}

void CsrBtAvrcpCtRegisterNotificationCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 notificationId, CsrUint32 interval)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_CMD_HEADER_SIZE);

    if (pendingMsgInfo)
    {/* if pendingMsgInfo is a NULL pointer, then the ctTLabel has already a value */
        CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTP);
    }

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->control.ctTLabel,       /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_CMD,          /* Command or response */
                              AVRCP_DATA_AVC_CTYPE_NOTIFY,      /* Command/response type */
                              AVRCP_DATA_PDU_ID_REG_NOTI,       /* PDU ID */
                              AVRCP_DATA_PDU_REG_NOTI_CMD_HEADER_SIZE); /* MD parameter length */

    txData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX] = notificationId;

    CSR_COPY_UINT32_TO_BIG_ENDIAN(notificationId == CSR_BT_AVRCP_NOTI_ID_PLAYBACK_POS ? interval : 0,
                                   &txData[AVRCP_DATA_PDU_REG_NOTI_CMD_PLAY_INT_INDEX]);

    /* Store the TLabel for use when a response is received */
    connInst->ctLocal.notiList[notificationId - CSR_BT_AVRCP_NOTI_ID_OFFSET] = connInst->control.ctTLabel;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_CMD_HEADER_SIZE, txData);
    connInst->ctLocal.activeNotifications++;
}

void CsrBtAvrcpCtRequestAbortContinuationCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBool proceed, CsrUint8 pduId)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_CONTINUING_CMD_HEADER_SIZE);
    if (pendingMsgInfo)
    {
        CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTC);
        /* Update result code.... */
        switch(pduId)
        {
            case AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT:
            {
                CsrBtAvrcpCtPasAttTxtCfm *cfmMsg = (CsrBtAvrcpCtPasAttTxtCfm *)pendingMsgInfo->cfmMsg;
                cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_UNSPECIFIED_ERROR;
                cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
                break;
            }
            case AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT:
            {
                CsrBtAvrcpCtPasValTxtCfm *cfmMsg = (CsrBtAvrcpCtPasValTxtCfm *)pendingMsgInfo->cfmMsg;
                cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_UNSPECIFIED_ERROR;
                cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
                break;
            }
            case AVRCP_DATA_PDU_ID_GET_ELEMENT_ATTRIBUTES:
            {
                CsrBtAvrcpCtGetAttributesCfm *cfmMsg = (CsrBtAvrcpCtGetAttributesCfm *)pendingMsgInfo->cfmMsg;
                cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_UNSPECIFIED_ERROR;
                cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
                break;
            }
        }
    }

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->control.ctTLabel,       /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_CMD,          /* Command or response */
                              AVRCP_DATA_AVC_CTYPE_CONTROL,     /* Command/response type */
                              (CsrUint8)(proceed ? AVRCP_DATA_PDU_ID_REQ_CONTINUING_RES : AVRCP_DATA_PDU_ID_ABORT_CONTINUING_RES),/* PDU ID */
                              AVRCP_DATA_PDU_CONTINUING_CMD_HEADER_SIZE); /* MD parameter length */

    txData[AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ID_INDEX] = pduId; /* Either continue or response the handling of this PDU ID */

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_CONTINUING_CMD_HEADER_SIZE, txData);
}

void CsrBtAvrcpCtSetAbsoluteVolumeCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 volume)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_VOLUME_CMD_HEADER_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTC);

    CsrBtAvrcpDataInsertControlHeader(txData,
                            pendingMsgInfo->tLabel,
                            AVRCP_DATA_AVCTP_CR_CMD,
                            AVRCP_DATA_AVC_CTYPE_CONTROL,
                            AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL,
                            0x00, /* Subunit ID*/
                            AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT,
                            AVRCP_DATA_PDU_ID_SET_ABSOLUTE_VOLUME,
                            AVRCP_DATA_PDU_SET_VOLUME_CMD_HEADER_SIZE);

    txData[AVRCP_DATA_PDU_SET_VOLUME_CMD_INDEX] = volume;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_VOLUME_CMD_HEADER_SIZE, txData);
}
#define ARMD_SAMSUNG_PACKET                  (0xFE)

void CsrBtAvrcpCtSetAbsoluteVolumeSamsungCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 volume,CsrUint8 isMute)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_VOLUME_CMD_HEADER_SIZE+1);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTC);
#if 1
    CsrBtAvrcpDataInsertControlHeader(txData,
                            pendingMsgInfo->tLabel,
                            AVRCP_DATA_AVCTP_CR_CMD,
                            AVRCP_DATA_AVC_CTYPE_CONTROL, 
                            AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL,
                            0x00, /* Subunit ID*/
                            AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT,
                            AVRCP_DATA_PDU_ID_SET_ABSOLUTE_VOLUME,
                            AVRCP_DATA_PDU_SET_VOLUME_CMD_HEADER_SIZE+1);
#else
	CsrBtAvrcpDataInsertAvctpHeader(txData,
                               pendingMsgInfo->tLabel,                              /* Transaction label */
                               AVRCP_DATA_AVCTP_CR_CMD);                                 /* Command or response */

    txData[AVRCP_DATA_AVCTP_SINGLE_MI_INDEX]     = AVRCP_DATA_AVC_CTYPE_NOTIFY;
    txData[AVRCP_DATA_AVC_SUBUNIT_TYPE_INDEX]    = (CsrUint8)((CsrUint8) (AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL << 3) + 0x00);
    txData[AVRCP_DATA_AVC_OPCODE_INDEX]          = AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT;
    CSR_COPY_UINT24_TO_BIG_ENDIAN(AVRCP_DATA_MD_BT_COMPANY_ID, &txData[AVRCP_DATA_MD_BT_COMPANY_ID_INDEX]);
    txData[AVRCP_DATA_MD_PDU_ID_INDEX]           = AVRCP_DATA_PDU_ID_SET_ABSOLUTE_VOLUME;
    txData[AVRCP_DATA_MD_PACKET_TYPE_INDEX]      = 0x00;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(AVRCP_DATA_PDU_SET_VOLUME_CMD_HEADER_SIZE+1, &txData[AVRCP_DATA_MD_PARAM_LEN_INDEX]);
#endif	

    txData[AVRCP_DATA_PDU_SET_VOLUME_CMD_INDEX] = volume;
	txData[AVRCP_DATA_PDU_SET_VOLUME_CMD_INDEX+1] = isMute;

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_VOLUME_CMD_HEADER_SIZE+1, txData);
}



void CsrBtAvrcpCtSetAddressedPlayerCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 playerId)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_AP_CMD_HEADER_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTC);

    CsrBtAvrcpDataInsertControlHeader(txData,
                            pendingMsgInfo->tLabel,
                            AVRCP_DATA_AVCTP_CR_CMD,
                            AVRCP_DATA_AVC_CTYPE_CONTROL,
                            AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL,
                            0x00, /* Subunit ID*/
                            AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT,
                            AVRCP_DATA_PDU_ID_SET_ADDRESSED_PLAYER,
                            AVRCP_DATA_PDU_SET_AP_CMD_HEADER_SIZE);

    CSR_COPY_UINT16_TO_BIG_ENDIAN(playerId, &txData[AVRCP_DATA_PDU_SET_AP_CMD_PID_INDEX]);

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_AP_CMD_HEADER_SIZE, txData);
}

void CsrBtAvrcpCtSetBrowsedPlayerCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 playerId)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SET_BP_CMD_HEADER_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_BROWSING);

    CsrBtAvrcpDataInsertBrowsingHeader(txData,
                            pendingMsgInfo->tLabel,
                            AVRCP_DATA_AVCTP_CR_CMD,
                            AVRCP_DATA_PDU_ID_SET_BROWSED_PLAYER,
                            AVRCP_DATA_PDU_SET_BP_CMD_HEADER_SIZE);

    CSR_COPY_UINT16_TO_BIG_ENDIAN(playerId, &txData[AVRCP_DATA_PDU_SET_BP_CMD_PID_INDEX]);

    CsrBtAvrcpBrowsingDataSend(connInst, AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SET_BP_CMD_HEADER_SIZE, txData);
}

void CsrBtAvrcpCtGetFolderItemsCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpScope scope, CsrUint32 startItem, CsrUint32 endItem, CsrBtAvrcpItemAttMask attributeMask)
{
    CsrUint8 attributeCount  = CsrBitCountSparse(attributeMask) * sizeof(CsrBtAvrcpItemAttMask); /* Each attribute Id fills 4 bytes */
    CsrUint8 *txData         = CsrPmemAlloc(AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_GFI_CMD_MIN_HEADER_SIZE + attributeCount - 1);
    CsrUintFast8 i;
    CsrUint8 attributeIndex;

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_BROWSING);

    CsrBtAvrcpDataInsertBrowsingHeader(txData,
                            pendingMsgInfo->tLabel,
                            AVRCP_DATA_AVCTP_CR_CMD,
                            AVRCP_DATA_PDU_ID_GET_FOLDER_ITEMS,
                            (CsrUint16)(AVRCP_DATA_PDU_GFI_CMD_MIN_HEADER_SIZE + attributeCount - 1));

    txData[AVRCP_DATA_PDU_GFI_CMD_SCOPE_INDEX] = scope;
    txData[AVRCP_DATA_PDU_GFI_CMD_ATT_COUNT_INDEX] = attributeCount/sizeof(CsrBtAvrcpItemAttMask);
    CSR_COPY_UINT32_TO_BIG_ENDIAN(startItem, &txData[AVRCP_DATA_PDU_GFI_CMD_START_INDEX]);
    CSR_COPY_UINT32_TO_BIG_ENDIAN(endItem, &txData[AVRCP_DATA_PDU_GFI_CMD_END_INDEX]);

    /* Insert the attribute IDs */
    for (i = 0, attributeIndex = 0; attributeIndex < attributeCount; i++)
    {
        if (CSR_BIT_IS_SET(attributeMask, i))
        {
            CSR_COPY_UINT32_TO_BIG_ENDIAN(i + CSR_BT_AVRCP_ITEM_ATT_OFFSET_FROM_MASK, &txData[AVRCP_DATA_PDU_GFI_CMD_ATT_LIST_INDEX + attributeIndex]);
            attributeIndex += sizeof(CsrBtAvrcpItemAttMask);
        }
    }

    CsrBtAvrcpBrowsingDataSend(connInst, (CsrUint16)(AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_GFI_CMD_MIN_HEADER_SIZE + attributeCount - 1), txData);
}

void CsrBtAvrcpCtChangePathCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 uidCount, CsrBtAvrcpFolderDirection direction, CsrBtAvrcpUid *folderUid)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_CHANGE_PATH_CMD_HEADER_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_BROWSING);

    CsrBtAvrcpDataInsertBrowsingHeader(txData,
                            pendingMsgInfo->tLabel,
                            AVRCP_DATA_AVCTP_CR_CMD,
                            AVRCP_DATA_PDU_ID_CHANGE_PATH,
                            AVRCP_DATA_PDU_CHANGE_PATH_CMD_HEADER_SIZE);

    txData[AVRCP_DATA_PDU_CHANGE_PATH_CMD_DIRECTION_INDEX] = direction;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(uidCount, &txData[AVRCP_DATA_PDU_CHANGE_PATH_CMD_UID_COUNT_INDEX]);
    CSR_BT_AVRCP_UID_COPY(&txData[AVRCP_DATA_PDU_CHANGE_PATH_CMD_FOLDER_UID_INDEX], *folderUid);

    CsrBtAvrcpBrowsingDataSend(connInst, AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_CHANGE_PATH_CMD_HEADER_SIZE, txData);
}

void CsrBtAvrcpCtGetItemAttributesCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpScope scope, CsrUint16 uidCount, CsrBtAvrcpUid *itemUid, CsrBtAvrcpItemAttMask attributeMask)
{
    CsrUint8 attributeCount  = CsrBitCountSparse(attributeMask);
    CsrUint16 paramLen       = AVRCP_DATA_PDU_GIA_CMD_MIN_HEADER_SIZE + attributeCount*AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ATT_SIZE;
    CsrUint8 *txData         = CsrPmemAlloc(AVRCP_DATA_BROWSING_HEADER_SIZE + paramLen);
    CsrUintFast8 i;
    CsrUint8 attributeIndex;

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_BROWSING);

    CsrBtAvrcpDataInsertBrowsingHeader(txData,
                            pendingMsgInfo->tLabel,
                            AVRCP_DATA_AVCTP_CR_CMD,
                            AVRCP_DATA_PDU_ID_GET_ITEM_ATTRIBUTES,
                            paramLen);

    txData[AVRCP_DATA_PDU_GIA_CMD_SCOPE_INDEX] = scope;
    txData[AVRCP_DATA_PDU_GIA_CMD_ATTRIB_COUNT_INDEX] = attributeCount;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(uidCount, &txData[AVRCP_DATA_PDU_GIA_CMD_UID_COUNT_INDEX]);
    CSR_BT_AVRCP_UID_COPY(&txData[AVRCP_DATA_PDU_GIA_CMD_UID_INDEX], *itemUid);

    /* Insert the attribute IDs */
    for (i = 0, attributeIndex = 0; attributeIndex < attributeCount; i++)
    {
        if (CSR_BIT_IS_SET(attributeMask, i))
        {
            CSR_COPY_UINT32_TO_BIG_ENDIAN(i + CSR_BT_AVRCP_ITEM_ATT_OFFSET_FROM_MASK,
                &txData[AVRCP_DATA_PDU_GIA_CMD_ATTRIB_INDEX + attributeIndex * AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ATT_SIZE]);
            attributeIndex++;
        }
    }

    CsrBtAvrcpBrowsingDataSend(connInst, (CsrUint16)(AVRCP_DATA_BROWSING_HEADER_SIZE + paramLen), txData);
}

void CsrBtAvrcpCtPlayItemCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpScope scope, CsrUint16 uidCount, CsrBtAvrcpUid *itemUid)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PLAY_ITEM_CMD_HEADER_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTC);

    CsrBtAvrcpDataInsertControlHeader(txData,
                            pendingMsgInfo->tLabel,
                            AVRCP_DATA_AVCTP_CR_CMD,
                            AVRCP_DATA_AVC_CTYPE_CONTROL,
                            AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL,
                            0x00, /* Subunit ID*/
                            AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT,
                            AVRCP_DATA_PDU_ID_PLAY_ITEM,
                            AVRCP_DATA_PDU_PLAY_ITEM_CMD_HEADER_SIZE);

    txData[AVRCP_DATA_PDU_PLAY_ITEM_CMD_SCOPE_INDEX] = scope;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(uidCount, &txData[AVRCP_DATA_PDU_PLAY_ITEM_CMD_UID_COUNT_INDEX]);
    CsrMemCpy(&txData[AVRCP_DATA_PDU_PLAY_ITEM_CMD_ITEM_UID_INDEX], *itemUid, sizeof(CsrBtAvrcpUid));

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PLAY_ITEM_CMD_HEADER_SIZE, txData);
}

void CsrBtAvrcpCtSearchCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpCharSet charset, CsrUtf8String *text)
{
    CsrUint16 textLen = (CsrUint16)CsrStrLen((char*)text);
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SEARCH_CMD_MIN_HEADER_SIZE + textLen);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_BROWSING_SEARCH);

    CsrBtAvrcpDataInsertBrowsingHeader(txData,
                            pendingMsgInfo->tLabel,
                            AVRCP_DATA_AVCTP_CR_CMD,
                            AVRCP_DATA_PDU_ID_SEARCH,
                            (CsrUint16)(AVRCP_DATA_PDU_SEARCH_CMD_MIN_HEADER_SIZE + textLen));

    CSR_COPY_UINT16_TO_BIG_ENDIAN(charset, &txData[AVRCP_DATA_PDU_SEARCH_CMD_CHARSET_INDEX]);
    CSR_COPY_UINT16_TO_BIG_ENDIAN(textLen, &txData[AVRCP_DATA_PDU_SEARCH_CMD_LEN_INDEX]);
    CsrMemCpy(&txData[AVRCP_DATA_PDU_SEARCH_CMD_SEARCH_TXT_INDEX], text, textLen);
    CsrPmemFree(text);

    CsrBtAvrcpBrowsingDataSend(connInst, (CsrUint16)(AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SEARCH_CMD_MIN_HEADER_SIZE + textLen), txData);
}

void CsrBtAvrcpCtAddToNPLCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpScope scope, CsrUint16 uidCount, CsrBtAvrcpUid *itemUid)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_ADD_TO_NPL_CMD_HEADER_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTC);

    CsrBtAvrcpDataInsertControlHeader(txData,
                            pendingMsgInfo->tLabel,
                            AVRCP_DATA_AVCTP_CR_CMD,
                            AVRCP_DATA_AVC_CTYPE_CONTROL,
                            AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL,
                            0x00, /* Subunit ID*/
                            AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT,
                            AVRCP_DATA_PDU_ID_ADD_TO_NOW_PLAYING,
                            AVRCP_DATA_PDU_ADD_TO_NPL_CMD_HEADER_SIZE);

    txData[AVRCP_DATA_PDU_ADD_TO_NPL_CMD_SCOPE_INDEX] = scope;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(uidCount, &txData[AVRCP_DATA_PDU_ADD_TO_NPL_CMD_UID_COUNT_INDEX]);
    CsrMemCpy(&txData[AVRCP_DATA_PDU_ADD_TO_NPL_CMD_ITEM_UID_INDEX], *itemUid,sizeof(CsrBtAvrcpUid));

    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_ADD_TO_NPL_CMD_HEADER_SIZE, txData);
}

void CsrBtAvrcpCtInformBatterySend(AvrcpConnInstance_t *connInst,AvrcpCtPendingMsgInfo_t *pendingMsgInfo,CsrBtAvrcpBatteryStatus batStatus)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_BATT_HEADER_SIZE);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTC);

    CsrBtAvrcpDataVendorDataInsert(txData,                           /* Data to transmit */
                              connInst->control.ctTLabel,       /* Transaction label */
                              AVRCP_DATA_AVCTP_CR_CMD,          /* Command or response */
                              AVRCP_DATA_AVC_CTYPE_CONTROL,     /* Command/response type */
                              AVRCP_DATA_PDU_ID_INFO_BAT_STAT_OF_CT,/* PDU ID */
                              1); /* MD parameter length */
    txData[AVRCP_DATA_PDU_REG_NOTI_RES_BATT_LEVEL_INDEX] = batStatus;
    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_REG_NOTI_RES_BATT_HEADER_SIZE, txData);

}
#endif

void CsrBtAvrcpCtUnitInfoCmdSend(AvrcpConnInstance_t *connInst,AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 pDataLen, CsrUint8 *pData)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_AVC_UNIT_RES_SIZE + pDataLen);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_RCP);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               connInst->control.ctTLabel,
                               AVRCP_DATA_AVCTP_CR_CMD);            /* Command or response */

    txData[AVRCP_DATA_AVC_CRTYPE_INDEX]        = AVRCP_DATA_AVC_CTYPE_STATUS;
    txData[AVRCP_DATA_AVC_SUBUNIT_TYPE_INDEX]  = 0xFF; /* Type + ID */
    txData[AVRCP_DATA_AVC_OPCODE_INDEX]        = AVRCP_DATA_AVC_OPCODE_UNIT_INFO; /* 0x30 */

    CsrMemSet(&txData[AVRCP_DATA_AVC_OPERAND_INDEX], 0xFF, (CsrUint16)(AVRCP_DATA_MD_PARAM_LEN_INDEX - AVRCP_DATA_AVC_OPERAND_INDEX));
    CsrMemCpy(&txData[AVRCP_DATA_MD_PARAM_LEN_INDEX], pData, pDataLen);

    CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_AVC_UNIT_RES_SIZE + pDataLen), txData);
    CsrPmemFree(pData);

}

void CsrBtAvrcpCtSubUnitInfoCmdSend(AvrcpConnInstance_t *connInst,AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 pDataLen, CsrUint8 *pData)
{
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_AVC_SUBUNIT_RES_SIZE + pDataLen);

    CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_RCP);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               connInst->control.ctTLabel,
                               AVRCP_DATA_AVCTP_CR_CMD);            /* Command or response */

    txData[AVRCP_DATA_AVC_CRTYPE_INDEX]        = AVRCP_DATA_AVC_CTYPE_STATUS;
    txData[AVRCP_DATA_AVC_SUBUNIT_TYPE_INDEX]  = 0xFF; /* Type + ID */
    txData[AVRCP_DATA_AVC_OPCODE_INDEX]        = AVRCP_DATA_AVC_OPCODE_SUBUNIT_INFO; /* 0x31 */

    CsrMemSet(&txData[AVRCP_DATA_AVC_OPERAND_INDEX], 0xFF, (CsrUint16)(AVRCP_DATA_MD_PARAM_LEN_INDEX - AVRCP_DATA_AVC_OPERAND_INDEX));
    CsrMemCpy(&txData[AVRCP_DATA_MD_PARAM_LEN_INDEX], pData, pDataLen);

    CsrBtAvrcpControlDataSend(connInst, (CsrUint16)(AVRCP_DATA_AVC_SUBUNIT_RES_SIZE + pDataLen), txData);
    CsrPmemFree(pData);
}
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
static void csrBtAvrcpCtDispCharSetRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtInformDispCharsetCfm *cfmMsg = (CsrBtAvrcpCtInformDispCharsetCfm *)pendingMsgInfo->cfmMsg;

    CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
        &cfmMsg->resultCode, &cfmMsg->resultSupplier);
    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtInformBatteryStatusOfCtRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtInformBatteryStatusCfm *cfmMsg = (CsrBtAvrcpCtInformBatteryStatusCfm *)pendingMsgInfo->cfmMsg;

    CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
        &cfmMsg->resultCode, &cfmMsg->resultSupplier);
    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtGetCapRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{/* Only handled in relation to a CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ */
    CsrBtAvrcpCtNotiRegisterCfm *cfmMsg = (CsrBtAvrcpCtNotiRegisterCfm *)pendingMsgInfo->cfmMsg;

    if (rxData[AVRCP_DATA_PDU_GET_CAP_RES_CAPA_ID_INDEX] == AVRCP_DATA_PDU_GET_CAP_CMN_NOTI_SUP)
    {/* This is a response to the get supported notifications command */
        if (rxDataLen == AVRCP_DATA_PDU_GET_CAP_RES_CAPA_INDEX + rxData[AVRCP_DATA_PDU_GET_CAP_RES_COUNT_INDEX])
        {
            CsrUintFast8 i;

            connInst->ctLocal.tgSupportedNotifications = 0;

            for (i = 0; i < rxData[AVRCP_DATA_PDU_GET_CAP_RES_COUNT_INDEX]; i++)
            {/* Skip through all the announced notifications */
                if (rxData[AVRCP_DATA_PDU_GET_CAP_RES_CAPA_INDEX + i] <= CSR_BT_AVRCP_NOTI_ID_MAXIMUM)
                {/* Only handle known notifications */
                    /* Convert notification IDs to a bitmask */
                    CSR_BIT_SET(connInst->ctLocal.tgSupportedNotifications, rxData[AVRCP_DATA_PDU_GET_CAP_RES_CAPA_INDEX + i] - CSR_BT_AVRCP_NOTI_ID_OFFSET);
                }
            }

            /* Start registering for the notifications */
            if (CsrBtAvrcpCtRegisterNextNotification(connInst, pendingMsgInfo))
            {/* None of the requested notifications are supported */
                cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_SUCCESS; /* Still a success even if no notifications has been registered */
                CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
            }
            else
            {/* A RegisterNotification command has been sent - wait for response */
            }
        }
        else
        {
            cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
            cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
            CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
        }
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_UNSPECIFIED_ERROR;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
    }
}

static void csrBtAvrcpCtRegisterNotificationRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    if (pendingMsgInfo)
    {/* An CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ is being processed (multiple notifications being registered for) */
        CsrBtAvrcpCtNotiRegisterCfm *cfmMsg = (CsrBtAvrcpCtNotiRegisterCfm *)pendingMsgInfo->cfmMsg;

        if (AVRCP_DATA_AVC_CRTYPE_GET(rxData) == CSR_BT_AVRCP_DATA_AVC_RTYPE_INTERIM)
        {/* Registration succeeded - set corresponding flag */
            CSR_BIT_SET(cfmMsg->notiMask, pendingMsgInfo->tmpState - CSR_BT_AVRCP_NOTI_ID_OFFSET);
            CsrBtAvrcpCtNotiIndSend(connInst, rxDataLen, rxData);
        }

        if(CsrBtAvrcpCtRegisterNextNotification(connInst, pendingMsgInfo))
        {/* All requested notifications have been registered */
            cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_SUCCESS;
            cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
            CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
        }
        else
        {/* A RegisterNotification command has been sent - wait for response */
        }
    }
    else
    {/* No confirmation message is pending */
        CsrUint8 notiIndex = rxData[AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX];

        /* Check if TLabel matches the registered notification ID */
        if ((notiIndex <= CSR_BT_AVRCP_NOTI_ID_MAXIMUM) &&
            (connInst->ctLocal.notiList[notiIndex-CSR_BT_AVRCP_NOTI_ID_OFFSET] == AVRCP_TLABEL_GET(rxData)))
        {/* Notification has been previously registered */
            if (AVRCP_DATA_AVC_CRTYPE_GET(rxData) == CSR_BT_AVRCP_DATA_AVC_RTYPE_CHANGED)
            {
                /* Reregister for a new notification */
                if (CSR_BIT_IS_SET(connInst->ctLocal.ctRequestedNotifications, notiIndex-CSR_BT_AVRCP_NOTI_ID_OFFSET)
                    && !(connInst->ctLocal.notiConfig & CSR_BT_AVRCP_NOTI_REG_NON_PERSISTENT))
                {
                    /* Reuse the same TLabel for re-registration */
                     connInst->control.ctTLabel = connInst->ctLocal.notiList[notiIndex-CSR_BT_AVRCP_NOTI_ID_OFFSET];
                     CsrBtAvrcpCtRegisterNotificationCmdSend(connInst,
                                                       pendingMsgInfo,
                                                       notiIndex,                            /* Notification ID */
                                                       connInst->ctLocal.playbackInterval); /* Playback interval */
                }
                else
                {/* Invalidate used TLabel */
                    connInst->ctLocal.notiList[notiIndex-CSR_BT_AVRCP_NOTI_ID_OFFSET] = AVRCP_TLABEL_INVALID;
                    if (connInst->ctLocal.activeNotifications > 0)
                    {
                        connInst->ctLocal.activeNotifications--;
                    }
                }

                /* Send proper indication to application */
                CsrBtAvrcpCtNotiIndSend(connInst, rxDataLen, rxData);
            }/* Ignore INTERIM responses */
        }
    }
}

static void csrBtAvrcpCtPasListAttRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtPasAttIdCfm *cfmMsg = (CsrBtAvrcpCtPasAttIdCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen >= AVRCP_DATA_MD_HEADER_SIZE)
    {
        CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
            &cfmMsg->resultCode, &cfmMsg->resultSupplier);

        if (cfmMsg->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
            cfmMsg->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
        {
            cfmMsg->attIdCount   = rxData[AVRCP_DATA_PDU_PAS_LIST_ATT_RES_NUM_INDEX];
            cfmMsg->attId        = CsrMemDup(&rxData[AVRCP_DATA_PDU_PAS_LIST_ATT_RES_ATT_ID_INDEX], cfmMsg->attIdCount);
        }
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtPasListValRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtPasValIdCfm *cfmMsg = (CsrBtAvrcpCtPasValIdCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen >= AVRCP_DATA_MD_HEADER_SIZE)
    {
        CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
            &cfmMsg->resultCode, &cfmMsg->resultSupplier);

        if (cfmMsg->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
        {
            cfmMsg->valIdCount  = rxData[AVRCP_DATA_PDU_PAS_LIST_VAL_RES_NUM_INDEX];
            cfmMsg->valId       = CsrMemDup(&rxData[AVRCP_DATA_PDU_PAS_LIST_VAL_RES_VAL_ID_INDEX], cfmMsg->valIdCount);
        }
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtPasGetAttTextRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtPasAttTxtCfm *cfmMsg = (CsrBtAvrcpCtPasAttTxtCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen >= AVRCP_DATA_MD_HEADER_SIZE)
    {
        if ((AVRCP_DATA_MD_PACKET_TYPE_GET(rxData) == AVRCP_DATA_MD_PACKET_TYPE_SINGLE) ||
            (AVRCP_DATA_MD_PACKET_TYPE_GET(rxData) == AVRCP_DATA_MD_PACKET_TYPE_END))
        {/* Procedure is complete - send confirmation */
            CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
                &cfmMsg->resultCode, &cfmMsg->resultSupplier);

            if (cfmMsg->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
                cfmMsg->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
                cfmMsg->pasDataLen = rxDataLen;
                cfmMsg->pasData = CsrMemDup(rxData, rxDataLen);
            }

            CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
        }
        else
        {/* Start or continue packet */
            connInst->ctLocal.pendingMsgInfo = pendingMsgInfo;
            CsrBtAvrcpCtPasAttTxtIndSend(connInst->appConnId, rxDataLen, rxData, pendingMsgInfo->phandle);
        }
    }
    else
    {/* Message incorrectly formatted */
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
    }
}

static void csrBtAvrcpCtPasGetValTextRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtPasValTxtCfm *cfmMsg = (CsrBtAvrcpCtPasValTxtCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen >= AVRCP_DATA_MD_HEADER_SIZE)
    {
        if ((AVRCP_DATA_MD_PACKET_TYPE_GET(rxData) == AVRCP_DATA_MD_PACKET_TYPE_SINGLE) ||
            (AVRCP_DATA_MD_PACKET_TYPE_GET(rxData) == AVRCP_DATA_MD_PACKET_TYPE_END))
        {/* Procedure is complete - send confirmation */
            CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
                &cfmMsg->resultCode, &cfmMsg->resultSupplier);

            if (cfmMsg->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
                cfmMsg->pasDataLen = rxDataLen;
                cfmMsg->pasData = CsrMemDup(rxData, rxDataLen);
            }

            CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
        }
        else
        {/* Start or continue packet */
            connInst->ctLocal.pendingMsgInfo = pendingMsgInfo;
            CsrBtAvrcpCtPasValTxtIndSend(connInst->appConnId, cfmMsg->attId, rxDataLen, rxData, pendingMsgInfo->phandle);
        }
    }
    else
    {/* Message incorrectly formatted */
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;

        CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
    }
}

static void csrBtAvrcpCtPasGetCurrValRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtPasCurrentCfm *cfmMsg = (CsrBtAvrcpCtPasCurrentCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen >= AVRCP_DATA_MD_HEADER_SIZE)
    {
        CsrUintFast8 i;

        CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
            &cfmMsg->resultCode, &cfmMsg->resultSupplier);

        if (cfmMsg->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
        {
            cfmMsg->attValPairCount    = rxData[AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_NUM_INDEX];
            cfmMsg->attValPair         = CsrPmemAlloc(cfmMsg->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair));
        }

        /* Insert the attributes and corresponding values */
        for (i = 0; i < cfmMsg->attValPairCount; i++)
        {
            cfmMsg->attValPair[i].attribId = rxData[AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_ATT_ID_INDEX + i * AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_PART_SIZE];
            cfmMsg->attValPair[i].valueId = rxData[AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_VAL_ID_INDEX + i * AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_PART_SIZE];
        }
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtPasSetValueRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtPasSetCfm *cfmMsg = (CsrBtAvrcpCtPasSetCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen >= AVRCP_DATA_MD_HEADER_SIZE)
    {
        CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
            &cfmMsg->resultCode, &cfmMsg->resultSupplier);
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtGetElemAttRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtGetAttributesCfm *cfmMsg = (CsrBtAvrcpCtGetAttributesCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen >= AVRCP_DATA_MD_HEADER_SIZE)
    {
        CsrUint8 *pData = CsrPmemAlloc(rxDataLen);

        CsrMemCpy(pData,rxData,rxDataLen);

        if ((AVRCP_DATA_MD_PACKET_TYPE_GET(rxData) == AVRCP_DATA_MD_PACKET_TYPE_START) ||
            (AVRCP_DATA_MD_PACKET_TYPE_GET(rxData) == AVRCP_DATA_MD_PACKET_TYPE_CONTINUE))
        {
            CsrBtAvrcpCtGetAttributesIndSend(pendingMsgInfo->phandle, cfmMsg->connectionId, cfmMsg->scope, &cfmMsg->uid, rxData[AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ATT_COUNT_INDEX],
                                            rxDataLen, pData,AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ID_INDEX);
            connInst->ctLocal.pendingMsgInfo = pendingMsgInfo;
            CsrBtAvrcpCtResetPendingMsg(&connInst->control, pendingMsgInfo, CSR_BT_AVRCP_TIMER_MTC);
        }
        else
        {  /* Single frame or end fragment: just send confirmation */
            CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
                            &cfmMsg->resultCode, &cfmMsg->resultSupplier);
            cfmMsg->attributeCount     = rxData[AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ATT_COUNT_INDEX];
            cfmMsg->attribDataLen      = rxDataLen;
            cfmMsg->attribData         = pData;
            cfmMsg->attribDataPayloadOffset = AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ID_INDEX;
            CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
        }
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
    }

}

static void csrBtAvrcpCtGetPlayStatusRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtGetPlayStatusCfm *cfmMsg = (CsrBtAvrcpCtGetPlayStatusCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen >= AVRCP_DATA_MD_HEADER_SIZE)
    {
        CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
            &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        cfmMsg->playStatus      = rxData[AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_PS_INDEX];
        cfmMsg->songLength      = CSR_GET_UINT32_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_SL_INDEX]);
        cfmMsg->songPosition    = CSR_GET_UINT32_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_SP_INDEX]);
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}
#endif
static void csrBtAvrcpCtPassThroughRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtPassThroughCfm *cfmMsg = (CsrBtAvrcpCtPassThroughCfm *)pendingMsgInfo->cfmMsg;

    if ((CsrBtAvrcpUtilDataCheckPT(rxDataLen, rxData)) &&
        ((*(CsrBtAvrcpPrim *)cfmMsg) == CSR_BT_AVRCP_CT_PASS_THROUGH_CFM))
    {

        if (pendingMsgInfo->tmpState == AVRCP_STATE_PM_PT_RELEASE_PENDING)
        {
            pendingMsgInfo->tmpState = AVRCP_STATE_PM_IDLE;
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if ((cfmMsg->operationId == CSR_BT_AVRCP_PT_OP_ID_GROUP_NAV_NEXT) ||
                (cfmMsg->operationId == CSR_BT_AVRCP_PT_OP_ID_GROUP_NAV_PREV))
            {/* Group navigation */
                CsrBtAvrcpCtGroupNavigationCmdSend(connInst,
                                                   pendingMsgInfo,
                                                   (CsrUint16)(cfmMsg->operationId == CSR_BT_AVRCP_PT_OP_ID_GROUP_NAV_NEXT ? AVRCP_DATA_PT_GN_OPERATION_NEXT : AVRCP_DATA_PT_GN_OPERATION_PREV),
                                                   CSR_BT_AVRCP_PT_STATE_RELEASE);
            }
            else
#endif
            {/* Pass-through */
                CsrBtAvrcpCtPassThroughCmdSend(connInst,
                                          pendingMsgInfo,
                                          cfmMsg->operationId,
                                          CSR_BT_AVRCP_PT_STATE_RELEASE);
            }
        }
        else
        {
            CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
                &cfmMsg->resultCode, &cfmMsg->resultSupplier);
            CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
        }
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_INVALID_PARAMETER;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
    }
}
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14

static void csrBtAvrcpCtSetAbsoluteVolumeRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtSetVolumeCfm *cfmMsg = (CsrBtAvrcpCtSetVolumeCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen == (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_VOLUME_RES_HEADER_SIZE))
    {
        CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(rxData),
                            &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        if (cfmMsg->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
        {
            cfmMsg->volume  = rxData[AVRCP_DATA_MD_PARAM_INDEX];
        }
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtSetAddressedPlayerRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtSetAddressedPlayerCfm *cfmMsg = (CsrBtAvrcpCtSetAddressedPlayerCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen == (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_SET_AP_RES_HEADER_SIZE))
    {
        if (rxData[AVRCP_DATA_MD_PARAM_INDEX] == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_SUCCESS;
            cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        }
        else
        {
            cfmMsg->resultCode = rxData[AVRCP_DATA_MD_PARAM_INDEX];
            cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVCTP;
        }
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtSetBrowsedPlayerRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtSetBrowsedPlayerCfm *cfmMsg = (CsrBtAvrcpCtSetBrowsedPlayerCfm *)pendingMsgInfo->cfmMsg;

    if ((rxDataLen >= (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SET_BP_RES_MIN_HEADER_SIZE)))
    {
        CsrBtAvrcpCtUtilConvertBrowsingRspType(rxData[AVRCP_DATA_PDU_SET_BP_RES_STATUS_INDEX],
            &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        cfmMsg->itemsCount      = CSR_GET_UINT32_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PDU_SET_BP_RES_NUMBER_OF_ITEMS_INDEX]);
        cfmMsg->uidCounter      = CSR_GET_UINT16_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PDU_SET_BP_RES_UID_COUNT_INDEX]);
        cfmMsg->folderDepth     = rxData[AVRCP_DATA_PDU_SET_BP_RES_FOLDER_DEPTH_INDEX];
        if (cfmMsg->folderDepth > 0)
        {/* Convert to single directory path */
            cfmMsg->folderNamesLen  = CsrBtAvrcpCtSBPFolderNamesGet(&cfmMsg->folderNames,&rxData[AVRCP_DATA_PDU_SET_BP_RES_FOLDER_PAIRS_INDEX],cfmMsg->folderDepth);
        }
        else
        {
            cfmMsg->folderNamesLen = 0;
            cfmMsg->folderNames    = NULL;
        }
    }
    else
    {
        if ((rxDataLen == (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_STATUS_HEADER_SIZE)))
        {/* This is a normal rejection from the TG */
            CsrBtAvrcpCtUtilConvertBrowsingRspType(rxData[AVRCP_DATA_PDU_SET_BP_RES_STATUS_INDEX],
                &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        }
        else
        {
            cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
            cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        }
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtGetFolderItemsRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    CsrBtAvrcpCtGetFolderItemsCfm *cfmMsg = (CsrBtAvrcpCtGetFolderItemsCfm *)pendingMsgInfo->cfmMsg;

    if ((rxDataLen >= (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_GFI_RES_MIN_HEADER_SIZE)))
    {
        CsrBtAvrcpCtUtilConvertBrowsingRspType((*rxData)[AVRCP_DATA_PDU_GFI_RES_STATUS_INDEX],
            &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        cfmMsg->uidCounter      = CSR_GET_UINT16_FROM_BIG_ENDIAN(&(*rxData)[AVRCP_DATA_PDU_GFI_RES_UID_COUNT_INDEX]);
        cfmMsg->itemsCount      = CSR_GET_UINT16_FROM_BIG_ENDIAN(&(*rxData)[AVRCP_DATA_PDU_GFI_RES_NUM_OF_ITEMS_INDEX]);
        cfmMsg->itemsDataLen    = rxDataLen;
        cfmMsg->itemsData       = *rxData;
        *rxData = NULL;
    }
    else
    {
        if ((rxDataLen == (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_STATUS_HEADER_SIZE)))
        {/* This is a normal rejection from the TG */
            CsrBtAvrcpCtUtilConvertBrowsingRspType((*rxData)[AVRCP_DATA_PDU_SET_BP_RES_STATUS_INDEX],
                &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        }
        else
        {
            cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
            cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        }
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtChangePathRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtChangePathCfm *cfmMsg = (CsrBtAvrcpCtChangePathCfm *)pendingMsgInfo->cfmMsg;
    
    cfmMsg->itemsCount = 0;
    
    if ((rxDataLen == (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_CHANGE_PATH_RES_HEADER_SIZE)))
    {
        CsrBtAvrcpCtUtilConvertBrowsingRspType(rxData[AVRCP_DATA_PDU_CHANGE_PATH_RES_STATUS_INDEX],
            &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        cfmMsg->itemsCount      = CSR_GET_UINT32_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PDU_CHANGE_PATH_RES_NUM_OF_ITEMS_INDEX]);
    }
    else
    {
        if ((rxDataLen == (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_STATUS_HEADER_SIZE)))
        {/* This is a normal rejection from the TG */
            CsrBtAvrcpCtUtilConvertBrowsingRspType(rxData[AVRCP_DATA_PDU_SET_BP_RES_STATUS_INDEX],
                &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        }
        else
        {
            cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
            cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        }
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtGetItemAttributeRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    CsrBtAvrcpCtGetAttributesCfm *cfmMsg = (CsrBtAvrcpCtGetAttributesCfm *)pendingMsgInfo->cfmMsg;

    if ((rxDataLen >= (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_GIA_RES_MIN_HEADER_SIZE)))
    {
        CsrBtAvrcpCtUtilConvertBrowsingRspType((*rxData)[AVRCP_DATA_PDU_GIA_RES_STATUS_INDEX],
            &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        cfmMsg->attributeCount  = (*rxData)[AVRCP_DATA_PDU_GIA_RES_NOA_INDEX];
        cfmMsg->attribDataLen   = rxDataLen;
        cfmMsg->attribData      = *rxData;
        *rxData = NULL;
        cfmMsg->attribDataPayloadOffset = CSR_BT_AVRCP_LIB_GIA_HEADER_OFFSET;
    }
    else
    {
        if ((rxDataLen == (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_STATUS_HEADER_SIZE)))
        {/* This is a normal rejection from the TG */
            CsrBtAvrcpCtUtilConvertBrowsingRspType((*rxData)[AVRCP_DATA_PDU_SET_BP_RES_STATUS_INDEX],
                &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        }
        else
        {
            cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
            cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        }
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtPlayItemRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtPlayCfm *cfmMsg = (CsrBtAvrcpCtPlayCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen == (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_PLAY_ITEM_RES_HEADER_SIZE))
    {
        if (rxData[AVRCP_DATA_MD_PARAM_INDEX] == CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE)
        {
            cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_SUCCESS;
            cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        }
        else
        {
            cfmMsg->resultCode = rxData[AVRCP_DATA_MD_PARAM_INDEX];
            cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVCTP;
        }
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtSearchRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtSearchCfm *cfmMsg = (CsrBtAvrcpCtSearchCfm *)pendingMsgInfo->cfmMsg;

    if ((rxDataLen == (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_PDU_SEARCH_RES_HEADER_SIZE)))
    {
        CsrBtAvrcpCtUtilConvertBrowsingRspType(rxData[AVRCP_DATA_PDU_SEARCH_RES_STATUS_INDEX],
            &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        cfmMsg->numberOfItems   = CSR_GET_UINT32_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PDU_SEARCH_RES_ITEM_COUNT_INDEX]);
        cfmMsg->uidCounter      = CSR_GET_UINT16_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PDU_SEARCH_RES_UID_COUNT_INDEX]);
    }
    else
    {
        if ((rxDataLen == (AVRCP_DATA_BROWSING_HEADER_SIZE + AVRCP_DATA_STATUS_HEADER_SIZE)))
        {/* This is a normal rejection from the TG */
            CsrBtAvrcpCtUtilConvertBrowsingRspType(rxData[AVRCP_DATA_PDU_SET_BP_RES_STATUS_INDEX],
                &cfmMsg->resultCode, &cfmMsg->resultSupplier);
        }
        else
        {
            cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
            cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
        }
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtAddToNPLRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    CsrBtAvrcpCtAddToNowPlayingCfm *cfmMsg = (CsrBtAvrcpCtAddToNowPlayingCfm *)pendingMsgInfo->cfmMsg;

    if (rxDataLen == (AVRCP_DATA_MD_HEADER_SIZE + AVRCP_DATA_PDU_ADD_TO_NPL_RES_HEADER_SIZE))
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_SUCCESS;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
    }
    else
    {
        cfmMsg->resultCode = CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE;
        cfmMsg->resultSupplier = CSR_BT_SUPPLIER_AVRCP;
    }

    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
}

static void csrBtAvrcpCtVendorRspHandler(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    if (CsrBtAvrcpUtilDataCheckVendor(rxDataLen, rxData))
    {
        switch (rxData[AVRCP_DATA_MD_PDU_ID_INDEX])
        {
            case AVRCP_DATA_PDU_ID_GET_CAPABILITIES:
            {
                csrBtAvrcpCtGetCapRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_LIST_PAS_ATTRIBUTES:
            {
                csrBtAvrcpCtPasListAttRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_LIST_PAS_VALUES:
            {
                csrBtAvrcpCtPasListValRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_CUR_PAS_VALUES:
            {
                csrBtAvrcpCtPasGetCurrValRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_SET_PAS_VALUES:
            {
                csrBtAvrcpCtPasSetValueRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT:
            {
                csrBtAvrcpCtPasGetAttTextRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT:
            {
                csrBtAvrcpCtPasGetValTextRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_INFO_DISP_CHAR_SET:
            {
                csrBtAvrcpCtDispCharSetRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_INFO_BAT_STAT_OF_CT:
            {
                csrBtAvrcpCtInformBatteryStatusOfCtRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_ELEMENT_ATTRIBUTES:
            {
                csrBtAvrcpCtGetElemAttRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_GET_PLAY_STATUS:
            {
                csrBtAvrcpCtGetPlayStatusRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }

            case AVRCP_DATA_PDU_ID_REG_NOTI:
            {
                csrBtAvrcpCtRegisterNotificationRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }
            case AVRCP_DATA_PDU_ID_ABORT_CONTINUING_RES:
            {
                CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
                break;
            }
            case AVRCP_DATA_PDU_ID_SET_ADDRESSED_PLAYER:
            {
                csrBtAvrcpCtSetAddressedPlayerRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }
            case AVRCP_DATA_PDU_ID_SET_ABSOLUTE_VOLUME:
            {
                csrBtAvrcpCtSetAbsoluteVolumeRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }
            case AVRCP_DATA_PDU_ID_ADD_TO_NOW_PLAYING:
            {
                csrBtAvrcpCtAddToNPLRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }
            case AVRCP_DATA_PDU_ID_PLAY_ITEM:
            {
                csrBtAvrcpCtPlayItemRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else
    {/* AVRCP message incorrectly formatted */
        CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
    }
}

void CsrBtAvrcpCtRxBrowsingHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    AvrcpCtPendingMsgInfo_t *pendingMsgInfo;

    pendingMsgInfo = AVRCP_LIST_CT_PMSG_GET_TLABEL(&connInst->ctLocal.pendingMsgList, AVRCP_TLABEL_GET(*rxData));

    if (pendingMsgInfo)
    {
        if (CsrBtAvrcpUtilDataCheckBrowsing(rxDataLen, *rxData))
        {
            switch (AVRCP_DATA_BROWSING_PDU_ID_GET(*rxData))
            {
                case AVRCP_DATA_PDU_ID_SET_ABSOLUTE_VOLUME:
                {
                    csrBtAvrcpCtSetAbsoluteVolumeRspHandler(connInst, pendingMsgInfo, rxDataLen, *rxData);
                    break;
                }

                case AVRCP_DATA_PDU_ID_SET_BROWSED_PLAYER:
                {
                    csrBtAvrcpCtSetBrowsedPlayerRspHandler(connInst, pendingMsgInfo, rxDataLen, *rxData);
                    break;
                }

                case AVRCP_DATA_PDU_ID_GET_FOLDER_ITEMS:
                {
                    csrBtAvrcpCtGetFolderItemsRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                    break;
                }

                case AVRCP_DATA_PDU_ID_CHANGE_PATH:
                {
                    csrBtAvrcpCtChangePathRspHandler(connInst, pendingMsgInfo, rxDataLen, *rxData);
                    break;
                }

                case AVRCP_DATA_PDU_ID_GET_ITEM_ATTRIBUTES:
                {
                    csrBtAvrcpCtGetItemAttributeRspHandler(connInst, pendingMsgInfo, rxDataLen, rxData);
                    break;
                }

                case AVRCP_DATA_PDU_ID_SEARCH:
                {
                    csrBtAvrcpCtSearchRspHandler(connInst, pendingMsgInfo, rxDataLen, *rxData);
                    break;
                }

                case AVRCP_DATA_PDU_ID_GENERAL_REJECT:
                {/* Find out what message is pending and what message should be sent to the application */
                    CsrBtAvrcpCtPendingMsgUpdateResult(pendingMsgInfo,
                        (*rxData)[AVRCP_DATA_PDU_GENERAL_REJECT_RES_REASON_INDEX],
                        CSR_BT_SUPPLIER_AVCTP);
                    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
                    break;
                }

                default:
                {
                    CsrBtAvrcpCtPendingMsgUpdateResult(pendingMsgInfo,
                        CSR_BT_RESULT_CODE_AVRCP_UNACCEPTABLE_PARAMETER,
                        CSR_BT_SUPPLIER_AVRCP);
                    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
                    break;
                }
            }
        }
        else
        {
            CsrBtAvrcpCtPendingMsgUpdateResult(pendingMsgInfo,
                CSR_BT_RESULT_CODE_AVRCP_RSP_INCORRECT_SIZE,
                CSR_BT_SUPPLIER_AVRCP);
            CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
        }
    }

}
#endif
void CsrBtAvrcpCtRxControlHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData)
{
    AvrcpCtPendingMsgInfo_t *pendingMsgInfo;

    pendingMsgInfo = AVRCP_LIST_CT_PMSG_GET_TLABEL(&connInst->ctLocal.pendingMsgList, AVRCP_TLABEL_GET(*rxData));

    if (pendingMsgInfo)
    {
        if (CsrBtAvrcpUtilDataCheckAVC(rxDataLen, *rxData))
        {
            switch (AVRCP_DATA_AVC_OPCODE_GET(*rxData))
            {
                case AVRCP_DATA_AVC_OPCODE_PASS_THROUGH:
                {
                    csrBtAvrcpCtPassThroughRspHandler(connInst, pendingMsgInfo, rxDataLen, *rxData);
                    break;
                }
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                case AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT:
                {
                    csrBtAvrcpCtVendorRspHandler(connInst, pendingMsgInfo, rxDataLen, *rxData);
                    break;
                }
#endif
                case AVRCP_DATA_AVC_OPCODE_UNIT_INFO:
                {
                    CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(*rxData),
                        &(((CsrBtAvrcpCtUnitInfoCmdCfm *)(pendingMsgInfo->cfmMsg))->resultCode),
                        &(((CsrBtAvrcpCtUnitInfoCmdCfm *)(pendingMsgInfo->cfmMsg))->resultSupplier));

                    if (rxDataLen > AVRCP_DATA_AVC_OPCODE_INDEX)
                    {
                        CsrUint8 *pData = CsrPmemAlloc((CsrUint16)(rxDataLen-AVRCP_DATA_AVC_UNIT_RES_07_INDEX));
                        ((CsrBtAvrcpCtUnitInfoCmdCfm *)(pendingMsgInfo->cfmMsg))->pDataLen = (CsrUint16)(rxDataLen-AVRCP_DATA_AVC_UNIT_RES_07_INDEX);
                        CsrMemCpy(pData,&(*rxData)[AVRCP_DATA_AVC_UNIT_RES_07_INDEX],(CsrUint16)(rxDataLen-AVRCP_DATA_AVC_UNIT_RES_07_INDEX));
                        ((CsrBtAvrcpCtUnitInfoCmdCfm *)(pendingMsgInfo->cfmMsg))->pData    = pData;
                    }

                    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
                    break;
                }

                case AVRCP_DATA_AVC_OPCODE_SUBUNIT_INFO:
                {
                    CsrBtAvrcpCtUtilConvertAVCRspType(AVRCP_DATA_AVC_CRTYPE_GET(*rxData),
                        &(((CsrBtAvrcpCtSubUnitInfoCmdCfm *)(pendingMsgInfo->cfmMsg))->resultCode),
                        &(((CsrBtAvrcpCtSubUnitInfoCmdCfm *)(pendingMsgInfo->cfmMsg))->resultSupplier));

                    if (rxDataLen > AVRCP_DATA_AVC_OPCODE_INDEX)
                    {
                        CsrUint8 *pData = CsrPmemAlloc((CsrUint16)(rxDataLen-AVRCP_DATA_AVC_SUBUNIT_RES_PAGE_INDEX));
                        ((CsrBtAvrcpCtSubUnitInfoCmdCfm *)(pendingMsgInfo->cfmMsg))->pDataLen = (CsrUint16)(rxDataLen-AVRCP_DATA_AVC_SUBUNIT_RES_PAGE_INDEX);
                        CsrMemCpy(pData,&(*rxData)[AVRCP_DATA_AVC_SUBUNIT_RES_PAGE_INDEX],(CsrUint16)(rxDataLen-AVRCP_DATA_AVC_SUBUNIT_RES_PAGE_INDEX));
                        ((CsrBtAvrcpCtSubUnitInfoCmdCfm *)(pendingMsgInfo->cfmMsg))->pData    = pData;
                    }

                    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
                    break;
                }

                default:
                {/* Unknown response - ignore */
                    CsrBtAvrcpCtPendingMsgUpdateResult(pendingMsgInfo,
                        CSR_BT_RESULT_CODE_AVRCP_UNACCEPTABLE_PARAMETER,
                        CSR_BT_SUPPLIER_AVRCP);
                    CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
                    break;
                }
            }
        }
        else
        {/* AV/C frame is incorrectly formatted - ignore */
            CsrBtAvrcpCtPendingMsgUpdateResult(pendingMsgInfo,
                CSR_BT_RESULT_CODE_AVRCP_INVALID_PARAMETER,
                CSR_BT_SUPPLIER_AVRCP);
            CsrBtAvrcpCtCompletePendingMsg(pendingMsgInfo);
        }
    }
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
    else if ((CsrBtAvrcpUtilDataCheckVendor(rxDataLen, *rxData)) &&
             ((*rxData)[AVRCP_DATA_MD_PDU_ID_INDEX] == AVRCP_DATA_PDU_ID_REG_NOTI) &&
             ((AVRCP_DATA_AVC_CRTYPE_GET(*rxData) == CSR_BT_AVRCP_DATA_AVC_RTYPE_CHANGED) ||
             (AVRCP_DATA_AVC_CRTYPE_GET(*rxData) == CSR_BT_AVRCP_DATA_AVC_RTYPE_INTERIM)))
    {/* This is a notification */
        csrBtAvrcpCtRegisterNotificationRspHandler(connInst, NULL, rxDataLen, *rxData);
    }
#endif
}

#endif /* #ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE */

#endif

