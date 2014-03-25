/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2011. All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE

#include "csr_bt_avrcp_main.h"
#include "csr_bt_avrcp_prim.h"

CsrBool CsrBtAvrcpUtilDataCheckAvctp(CsrUint16 rxDataLen, CsrUint8 *rxData)
{
    if (rxDataLen > AVRCP_DATA_AVCTP_HEADER_SIZE_MIN)
    {
        CsrUint8 headerSize;

        switch (AVRCP_DATA_AVCTP_PACKET_TYPE_GET(rxData))
        {
            case AVRCP_DATA_AVCTP_PACKET_TYPE_SINGLE:
            {
                if (!AVRCP_DATA_AVCTP_IPID_GET(rxData) &&
                    (CSR_GET_UINT16_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_AVCTP_SINGLE_PID_INDEX]) == CSR_BT_AV_REMOTE_CONTROL_UUID))
                {
                    headerSize = AVRCP_DATA_AVCTP_HEADER_SIZE_SINGLE;
                }
                else
                {
                    return FALSE;
                }
                break;
            }

            case AVRCP_DATA_AVCTP_PACKET_TYPE_START:
            {
                if (!AVRCP_DATA_AVCTP_IPID_GET(rxData) &&
                    (CSR_GET_UINT16_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_AVCTP_START_PID_INDEX]) == CSR_BT_AV_REMOTE_CONTROL_UUID))
                {
                    headerSize = AVRCP_DATA_AVCTP_HEADER_SIZE_START;
                }
                else
                {
                    return FALSE;
                }
                break;
            }

            case AVRCP_DATA_AVCTP_PACKET_TYPE_CONTINUE:
            {
                headerSize = AVRCP_DATA_AVCTP_HEADER_SIZE_CONTINUE;
                break;
            }

            case AVRCP_DATA_AVCTP_PACKET_TYPE_END:
            {
                headerSize = AVRCP_DATA_AVCTP_HEADER_SIZE_END;
                break;
            }

            default:
            {/* Can never end here since the packet type is only 2 bits == maximum 4 different packet types, which are all handled */
                headerSize = 0; /* To shut up Lint */
                break;
            }
        }

        if (rxDataLen > headerSize) /* Reject "empty" AVCTP packets */
        {/* AVCTP packet is valid */
            return TRUE;
        }
    }

    /* AVCTP packet is invalid */
    return FALSE;
}

CsrBool CsrBtAvrcpUtilDataCheckAVC(CsrUint16 rxDataLen, CsrUint8 *rxData)
{/* AVCTP must be checked in advance */
    if (rxDataLen >= AVRCP_DATA_AVC_MIN_SIZE)
    {
        if (AVRCP_DATA_AVCTP_CR_GET(rxData) == AVRCP_DATA_AVCTP_CR_RES)
        {/* Response - check response types */
            if ((AVRCP_DATA_AVC_CRTYPE_GET(rxData) == CSR_BT_AVRCP_DATA_AVC_RTYPE_ACCEPTED) ||
                (AVRCP_DATA_AVC_CRTYPE_GET(rxData) == CSR_BT_AVRCP_DATA_AVC_RTYPE_STABLE) ||
                (AVRCP_DATA_AVC_CRTYPE_GET(rxData) == CSR_BT_AVRCP_DATA_AVC_RTYPE_INTERIM) ||
                (AVRCP_DATA_AVC_CRTYPE_GET(rxData) == CSR_BT_AVRCP_DATA_AVC_RTYPE_CHANGED) ||
                (AVRCP_DATA_AVC_CRTYPE_GET(rxData) == CSR_BT_AVRCP_DATA_AVC_RTYPE_REJECTED) ||
                (AVRCP_DATA_AVC_CRTYPE_GET(rxData) == CSR_BT_AVRCP_DATA_AVC_RTYPE_NOT_IMP))
            {/* Response is valid */
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {/* Command - check command types */
            if ((AVRCP_DATA_AVC_CRTYPE_GET(rxData) == AVRCP_DATA_AVC_CTYPE_CONTROL) ||
                (AVRCP_DATA_AVC_CRTYPE_GET(rxData) == AVRCP_DATA_AVC_CTYPE_STATUS) ||
                (AVRCP_DATA_AVC_CRTYPE_GET(rxData) == AVRCP_DATA_AVC_CTYPE_NOTIFY))
            {/* Command is valid */
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    else
    {
        return FALSE;
    }
}

#if defined(CSR_BT_INSTALL_AVRCP_CT_13_14) || defined(CSR_BT_INSTALL_AVRCP_TG_13_14)
CsrBool CsrBtAvrcpUtilDataCheckVendor(CsrUint16 rxDataLen, CsrUint8 *rxData)
{/* AVCTP + AV/C must be checked in advance */
    if ((rxDataLen >= AVRCP_DATA_MD_HEADER_SIZE) &&
        (rxDataLen == (CSR_GET_UINT16_FROM_BIG_ENDIAN(rxData + AVRCP_DATA_MD_PARAM_LEN_INDEX) + AVRCP_DATA_MD_HEADER_SIZE)) &&
        (CSR_GET_UINT24_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_MD_BT_COMPANY_ID_INDEX]) == AVRCP_DATA_MD_BT_COMPANY_ID))
    {/* Size + company ID is valid */
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

CsrBool CsrBtAvrcpUtilDataCheckPT(CsrUint16 rxDataLen, CsrUint8 *rxData)
{/* AVCTP + AV/C must be checked in advance */
    if (rxDataLen >= AVRCP_DATA_PT_SIZE)
    {
        if (AVRCP_DATA_PT_OPID_GET(rxData) == CSR_BT_AVRCP_PT_OP_ID_VENDOR_DEP)
        {/* Group navigation */
            if ((rxDataLen == AVRCP_DATA_PT_GN_SIZE) &&
                (CSR_GET_UINT24_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PT_GN_COMPANY_ID_INDEX]) == AVRCP_DATA_MD_BT_COMPANY_ID) &&
                (rxData[AVRCP_DATA_PT_OPID_LEN_INDEX] == AVRCP_DATA_PT_GN_DATA_FIELD_LENGTH))
            {
                return TRUE;
            }
        }
        else
        {/* Ordinary pass-through */
            if ((rxDataLen == AVRCP_DATA_PT_SIZE) &&
                (rxData[AVRCP_DATA_PT_OPID_LEN_INDEX] == 0x00))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

#if defined(CSR_BT_INSTALL_AVRCP_TG_13_14) || defined(CSR_BT_INSTALL_AVRCP_CT_13_14)
CsrBool CsrBtAvrcpUtilDataCheckBrowsing(CsrUint16 rxDataLen, CsrUint8 *rxData)
{/* AVCTP must be checked in advance */
    if ((rxDataLen >= AVRCP_DATA_BROWSING_HEADER_SIZE) &&
        (rxDataLen == (AVRCP_DATA_BROWSING_PARAM_LEN_GET(rxData) + AVRCP_DATA_BROWSING_HEADER_SIZE)))
    {/* Size is correct */
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

void CsrBtAvrcpControlDataSend(AvrcpConnInstance_t *connInst, CsrUint16 dataLen, CsrUint8 *data)
{/* All data is prepared as single-packet AVCTP frames */
    CsrBtAvrcpConnDetails *connDetails = &connInst->control;

    if ( (dataLen > connDetails->mtu) && (dataLen > AVRCP_DATA_AVCTP_SINGLE_MI_INDEX) )
    {/* Frame should be fragmented - will only occur very rarely: Remote MTU < 515 bytes and corresponding large frames is being sent. Can only happen for control channel */
        CsrUint8 *txData;
        CsrUint16 remainder = dataLen % (connDetails->mtu - 1);
        CsrIntFast8 i;
        CsrUint8 numberOfPackets = dataLen / (connDetails->mtu - 1) + (remainder ? 1 : 0);
        CsrUint32 dataIndex = AVRCP_DATA_AVCTP_SINGLE_MI_INDEX;

        /* Add 'start' packet */
        txData = CsrPmemAlloc(connDetails->mtu);
        txData[0] = data[0]; /* TLabel and C/R */
        txData[AVRCP_DATA_AVCTP_PACKET_TYPE_INDEX] |= AVRCP_DATA_AVCTP_PACKET_TYPE_START << AVRCP_DATA_AVCTP_PACKET_TYPE_SHIFT;
        txData[AVRCP_DATA_AVCTP_NUM_OF_PACKETS_INDEX] = numberOfPackets;
        CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AV_REMOTE_CONTROL_UUID, &txData[AVRCP_DATA_AVCTP_START_PID_INDEX]);
        CsrMemCpy(txData + AVRCP_DATA_AVCTP_START_MI_INDEX, data + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX, connDetails->mtu - AVRCP_DATA_AVCTP_START_MI_INDEX);
        dataIndex += (connDetails->mtu - AVRCP_DATA_AVCTP_START_MI_INDEX);
        
        if (connDetails->dataSendAllowed)
        {/* Send the data immediately */
            CsrBtCml2caDataReqSend(connDetails->btConnId, connDetails->mtu, txData);
            connDetails->dataSendAllowed = FALSE;
        }
        else
        {/* Save the data for later transmission */
            CsrBtAvrcpUtilPendingDataAddLast(connDetails, connDetails->mtu, txData);
        }

        /* Add 'continue' packets - only if 3 or more packets in total (numberOfPackets > 2) */
        for (i = 1; i < (numberOfPackets - 1); i++)
        {
            txData = CsrPmemAlloc(connDetails->mtu);
            txData[0] = data[0]; /* TLabel and C/R */
            txData[AVRCP_DATA_AVCTP_PACKET_TYPE_INDEX] |= AVRCP_DATA_AVCTP_PACKET_TYPE_CONTINUE << AVRCP_DATA_AVCTP_PACKET_TYPE_SHIFT;
            CsrMemCpy(txData + AVRCP_DATA_AVCTP_CONTINUE_MI_INDEX, data + dataIndex, connDetails->mtu - AVRCP_DATA_AVCTP_CONTINUE_MI_INDEX);
            dataIndex += (connDetails->mtu - AVRCP_DATA_AVCTP_CONTINUE_MI_INDEX);
            CsrBtAvrcpUtilPendingDataAddLast(connDetails, connDetails->mtu, txData);
        }

        /* Add 'end' packet */
        if (!remainder)
        {/* There is no remainder - re-adjust parameter */
            remainder = connDetails->mtu - AVRCP_DATA_AVCTP_END_MI_INDEX;
        }

        txData = CsrPmemAlloc(remainder + AVRCP_DATA_AVCTP_END_MI_INDEX);
        txData[0] = data[0]; /* TLabel and C/R */
        txData[AVRCP_DATA_AVCTP_PACKET_TYPE_INDEX] |= AVRCP_DATA_AVCTP_PACKET_TYPE_END  << AVRCP_DATA_AVCTP_PACKET_TYPE_SHIFT;
        CsrMemCpy(txData + AVRCP_DATA_AVCTP_END_MI_INDEX, data + dataIndex, remainder);
        CsrBtAvrcpUtilPendingDataAddLast(connDetails, (CsrUint16) (remainder + AVRCP_DATA_AVCTP_END_MI_INDEX), txData);
        CsrPmemFree(data);
    }
    else
    {/* Frame is not fragmented - this is the normal path in almost all situations */
        if (connDetails->dataSendAllowed)
        {/* Send the data momentarily */
            CsrBtCml2caDataReqSend(connDetails->btConnId, dataLen, data);
            connDetails->dataSendAllowed = FALSE;
        }
        else
        {/* Save the data for later transmission */
            CsrBtAvrcpUtilPendingDataAddLast(connDetails, dataLen, data);
        }
    }
}

#if defined(CSR_BT_INSTALL_AVRCP_CT_13_14) || defined(CSR_BT_INSTALL_AVRCP_TG_13_14)
void CsrBtAvrcpBrowsingDataSend(AvrcpConnInstance_t *connInst, CsrUint16 dataLen, CsrUint8 *data)
{
    CsrBtAvrcpConnDetails *connDetails = &connInst->browsing;

    if (connInst->instData->lpMode != CSR_BT_LINK_STATUS_CONNECTED)
    {/* Change to active mode immediately and stop the lp timer if it is running */
        CsrBtCml2caModeChangeReqSend(connDetails->btConnId,CSR_BT_LINK_STATUS_CONNECTED);
    }

    /* Re-start timer if it was already running, to avoid going back to sniff during data exchange */
    CsrSchedTimerCancel(connInst->instData->lpTimer,NULL,NULL);
    /* Remember: Always use the control btConnId when the timer is re-started */
    connInst->instData->lpTimer = CsrSchedTimerSet(CSR_BT_AVRCP_LP_TIME, CsrBtAvrcpLpTimeout, (CsrUint16)connInst->control.btConnId,(void *)connInst->instData);


    if (connDetails->dataSendAllowed)
    {/* Send the data momentarily */
        CsrBtCml2caDataReqSend(connDetails->btConnId, dataLen, data);
        connDetails->dataSendAllowed = FALSE;
    }
    else
    {/* Save the data for later transmission */
        CsrBtAvrcpUtilPendingDataAddLast(connDetails, dataLen, data);
    }
}
#endif

#if defined(CSR_BT_INSTALL_AVRCP_CT_13_14) || defined(CSR_BT_INSTALL_AVRCP_TG_13_14)
void CsrBtAvrcpDataInsertControlHeader(CsrUint8 *txData,
                                       CsrUint8 tLabel,
                                       CsrUint8 cr,
                                       CsrUint8 cType,
                                       CsrUint8 subunitType,
                                       CsrUint8 subunitId,
                                       CsrUint8 opcode,
                                       CsrUint8 pduId,
                                       CsrUint16 paramLen)
{
    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               tLabel,                              /* Transaction label */
                               cr);                                 /* Command or response */

    txData[AVRCP_DATA_AVCTP_SINGLE_MI_INDEX]     = cType;
    txData[AVRCP_DATA_AVC_SUBUNIT_TYPE_INDEX]    = (CsrUint8)((CsrUint8) (subunitType << 3) + subunitId);
    txData[AVRCP_DATA_AVC_OPCODE_INDEX]          = opcode;
    CSR_COPY_UINT24_TO_BIG_ENDIAN(AVRCP_DATA_MD_BT_COMPANY_ID, &txData[AVRCP_DATA_MD_BT_COMPANY_ID_INDEX]);
    txData[AVRCP_DATA_MD_PDU_ID_INDEX]           = pduId;
    txData[AVRCP_DATA_MD_PACKET_TYPE_INDEX]      = 0x00;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(paramLen, &txData[AVRCP_DATA_MD_PARAM_LEN_INDEX]);
}

void CsrBtAvrcpDataInsertBrowsingHeader(CsrUint8 *txData, CsrUint8 tLabel, CsrUint8 cr, CsrUint8 pduId, CsrUint16 paramLen)
{
    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               tLabel,                              /* Transaction label */
                               cr);                                 /* Command or response */

    txData[AVRCP_DATA_BROWSING_PDU_ID_INDEX] = pduId;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(paramLen, &txData[AVRCP_DATA_BROWSING_PARAM_LEN_INDEX]);
}

void CsrBtAvrcpDataVendorDataInsert(CsrUint8 *txData, CsrUint8 tLabel, CsrUint8 cr, CsrUint8 crType, CsrUint8 pduId, CsrUint16 paramLen)
{
    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               tLabel,                              /* Transaction label */
                               cr);                                 /* Command or response */

    CsrBtAvrcpDataInsertAvcCommonHeader(txData, crType);

    CsrBtAvrcpDataInsertAvcVendorHeader(txData,
                                   pduId,
                                   AVRCP_DATA_MD_PACKET_TYPE_SINGLE,
                                   paramLen);
}

void CsrBtAvrcpDataSimpleVendorFrameSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint8 cr, CsrUint8 crType, CsrUint8 pduId)
{/* Can be used for sending vendor dependent AV/C frames without any payload besides PDU ID */
    CsrUint8 *txData = CsrPmemAlloc(AVRCP_DATA_MD_HEADER_SIZE);

    CsrBtAvrcpDataInsertAvctpHeader(txData,
                               tLabel,  /* Transaction label */
                               cr);     /* Command or response */

    CsrBtAvrcpDataInsertAvcCommonHeader(txData, crType);
    CsrBtAvrcpDataInsertAvcVendorHeader(txData, pduId, AVRCP_DATA_MD_PACKET_TYPE_SINGLE, 0);
    CsrBtAvrcpControlDataSend(connInst, AVRCP_DATA_MD_HEADER_SIZE, txData);
}
#endif

void CsrBtAvrcpDataInsertAvctpHeader(CsrUint8 *txData, CsrUint8 tLabel, CsrUint8 cr)
{/* Prepare a single packet AVCTP frame */
    txData[AVRCP_DATA_AVCTP_IPID_INDEX]        = 0x0 & AVRCP_DATA_AVCTP_IPID_MASK;
    txData[AVRCP_DATA_AVCTP_CR_INDEX]          |= (cr & AVRCP_DATA_AVCTP_CR_MASK) << AVRCP_DATA_AVCTP_CR_SHIFT;
    txData[AVRCP_DATA_AVCTP_PACKET_TYPE_INDEX] |= (AVRCP_DATA_AVCTP_PACKET_TYPE_SINGLE & AVRCP_DATA_AVCTP_PACKET_TYPE_MASK) << AVRCP_DATA_AVCTP_PACKET_TYPE_SHIFT;
    txData[AVRCP_DATA_AVCTP_TLABEL_INDEX]      |= (tLabel & AVRCP_DATA_AVCTP_TLABEL_MASK) << AVRCP_DATA_AVCTP_TLABEL_SHIFT;
    CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AV_REMOTE_CONTROL_UUID, &txData[AVRCP_DATA_AVCTP_SINGLE_PID_INDEX]);
}

void CsrBtAvrcpDataInsertAvcCommonHeader(CsrUint8 *txData, CsrUint8 crType)
{
    txData[AVRCP_DATA_AVC_CRTYPE_INDEX]        = crType;
    txData[AVRCP_DATA_AVC_SUBUNIT_TYPE_INDEX]  = (AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL << AVRCP_DATA_AVC_SUBUNIT_TYPE_SHIFT); /* SUBUNIT_ID is set to 0x0 */
}

#if defined(CSR_BT_INSTALL_AVRCP_CT_13_14) || defined(CSR_BT_INSTALL_AVRCP_TG_13_14)
void CsrBtAvrcpDataInsertAvcVendorHeader(CsrUint8 *txData, CsrUint8 pduId, CsrUint8 packetType, CsrUint16 paramLen)
{
    txData[AVRCP_DATA_AVC_OPCODE_INDEX]           = AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT;
    txData[AVRCP_DATA_MD_PACKET_TYPE_INDEX]       = packetType;
    txData[AVRCP_DATA_MD_PDU_ID_INDEX]            = pduId;
    CSR_COPY_UINT24_TO_BIG_ENDIAN(AVRCP_DATA_MD_BT_COMPANY_ID, &txData[AVRCP_DATA_MD_BT_COMPANY_ID_INDEX]);
    CSR_COPY_UINT16_TO_BIG_ENDIAN(paramLen, &txData[AVRCP_DATA_MD_PARAM_LEN_INDEX]);
}
#endif

void CsrBtAvrcpDataInsertAvcPassThroughHeader(CsrUint8 *txData, CsrUint8 state, CsrUint8 opId)
{
    txData[AVRCP_DATA_AVC_OPCODE_INDEX]   = AVRCP_DATA_AVC_OPCODE_PASS_THROUGH;
    txData[AVRCP_DATA_PT_STATE_INDEX]     = (state << AVRCP_DATA_PT_STATE_SHIFT) & AVRCP_DATA_PT_STATE_MASK;
    txData[AVRCP_DATA_PT_OPID_INDEX]      |= (opId & AVRCP_DATA_PT_OPID_MASK);
    txData[AVRCP_DATA_PT_OPID_LEN_INDEX]  = 0x00;
}

#if defined(CSR_BT_INSTALL_AVRCP_TG_13_14) || defined(CSR_BT_INSTALL_AVRCP_CT_13_14)
void CsrBtAvrcpDataInsertAvcGroupNavigationHeader(CsrUint8 *txData, CsrUint8 state, CsrUint16 operation)
{
    txData[AVRCP_DATA_AVC_OPCODE_INDEX]   = AVRCP_DATA_AVC_OPCODE_PASS_THROUGH;
    txData[AVRCP_DATA_PT_STATE_INDEX]     = (state << AVRCP_DATA_PT_STATE_SHIFT) & AVRCP_DATA_PT_STATE_MASK;
    txData[AVRCP_DATA_PT_OPID_INDEX]      |= (CSR_BT_AVRCP_PT_OP_ID_VENDOR_DEP & AVRCP_DATA_PT_OPID_MASK);
    txData[AVRCP_DATA_PT_OPID_LEN_INDEX]  = AVRCP_DATA_PT_GN_DATA_FIELD_LENGTH;
    CSR_COPY_UINT24_TO_BIG_ENDIAN(AVRCP_DATA_MD_BT_COMPANY_ID, &txData[AVRCP_DATA_PT_GN_COMPANY_ID_INDEX]);
    CSR_COPY_UINT16_TO_BIG_ENDIAN(operation, &txData[AVRCP_DATA_PT_GN_OPERATION_INDEX]);
}
#endif

void CsrBtAvrcpUtilDataFragRxFree(AvrcpConnInstance_t *connInst)
{
    connInst->pendingRxDataBufferLen = 0;
    CsrPmemFree(connInst->pendingRxDataBuffer);
    connInst->pendingRxDataBuffer = NULL;
}

CsrBool CsrBtAvrcpUtilDataFragRxHandle(AvrcpConnInstance_t *connInst, CsrUint16 *rxDataLen, CsrUint8 **rxData)
{/* Returns TRUE if *rxData can be treated as a complete AVCTP packet */
    if (*rxDataLen <= (AVRCP_DATA_AVC_MAX_SIZE + AVRCP_DATA_AVCTP_HEADER_SIZE_SINGLE))
    {
        switch (AVRCP_DATA_AVCTP_PACKET_TYPE_GET(*rxData))
        {
            case AVRCP_DATA_AVCTP_PACKET_TYPE_SINGLE:
            {/* Data is not fragmented - make sure any previous fragments are cleaned */
                CsrBtAvrcpUtilDataFragRxFree(connInst);
                return TRUE; /* Packet is complete */
            }

            case AVRCP_DATA_AVCTP_PACKET_TYPE_START:
            {
                /* Make sure to free any previously unhandled fragments */
                CsrBtAvrcpUtilDataFragRxFree(connInst);

                /* Allocate buffer for incoming AVCTP data - always use maximum allowed size */
                connInst->pendingRxDataBuffer = CsrPmemAlloc(AVRCP_DATA_AVC_MAX_SIZE + AVRCP_DATA_AVCTP_HEADER_SIZE_SINGLE);

                /* Insert header for single packet in order to simplify later processing */
                CsrBtAvrcpDataInsertAvctpHeader(connInst->pendingRxDataBuffer,
                                           AVRCP_TLABEL_GET(*rxData),
                                           AVRCP_DATA_AVCTP_CR_GET(*rxData));

                CsrMemCpy(connInst->pendingRxDataBuffer + AVRCP_DATA_AVCTP_HEADER_SIZE_SINGLE,
                       *rxData + AVRCP_DATA_AVCTP_HEADER_SIZE_START,
                       *rxDataLen - AVRCP_DATA_AVCTP_HEADER_SIZE_START);

                connInst->pendingRxDataBufferLen += (*rxDataLen - AVRCP_DATA_AVCTP_HEADER_SIZE_START + AVRCP_DATA_AVCTP_HEADER_SIZE_SINGLE);
                break;
            }

            case AVRCP_DATA_AVCTP_PACKET_TYPE_CONTINUE:
            {
                if (connInst->pendingRxDataBuffer &&
                    (connInst->pendingRxDataBufferLen + *rxDataLen - AVRCP_DATA_AVCTP_HEADER_SIZE_CONTINUE) <=
                    (AVRCP_DATA_AVC_MAX_SIZE + AVRCP_DATA_AVCTP_HEADER_SIZE_SINGLE))
                {/* The buffer is present and there is room for all the payload from the 'continue' packet */
                    CsrMemCpy(connInst->pendingRxDataBuffer + connInst->pendingRxDataBufferLen,
                           *rxData + AVRCP_DATA_AVCTP_HEADER_SIZE_CONTINUE,
                           *rxDataLen - AVRCP_DATA_AVCTP_HEADER_SIZE_CONTINUE);

                    connInst->pendingRxDataBufferLen += (*rxDataLen - AVRCP_DATA_AVCTP_HEADER_SIZE_CONTINUE);
                }
                else
                {
                    CsrBtAvrcpUtilDataFragRxFree(connInst);
                }
                break;
            }

            case AVRCP_DATA_AVCTP_PACKET_TYPE_END:
            {
                if (connInst->pendingRxDataBuffer &&
                    (connInst->pendingRxDataBufferLen + *rxDataLen - AVRCP_DATA_AVCTP_HEADER_SIZE_END) <=
                    (AVRCP_DATA_AVC_MAX_SIZE + AVRCP_DATA_AVCTP_HEADER_SIZE_SINGLE))
                {/* The buffer is present and there is room for all the payload from the 'end' packet */
                    CsrMemCpy(connInst->pendingRxDataBuffer + connInst->pendingRxDataBufferLen,
                           *rxData + AVRCP_DATA_AVCTP_HEADER_SIZE_END,
                           *rxDataLen - AVRCP_DATA_AVCTP_HEADER_SIZE_END);

                    CsrPmemFree(*rxData);

                    connInst->pendingRxDataBufferLen += (*rxDataLen - AVRCP_DATA_AVCTP_HEADER_SIZE_END);

                    /* Redirect the rxData to the conmplete AVCTP frame */
                    *rxDataLen = connInst->pendingRxDataBufferLen;
                    *rxData = connInst->pendingRxDataBuffer;
                    connInst->pendingRxDataBufferLen = 0;
                    connInst->pendingRxDataBuffer = NULL;
                    return TRUE; /* A complete AVCTP frame has been assembled */
                }
                else
                {
                    CsrBtAvrcpUtilDataFragRxFree(connInst);
                }
                break;
            }

            default:
            {/* Can never end here since the packet type is only 2 bits == maximum 4 different packet types, which are all handled */
                break;
            }
        }
    }

    return FALSE;
}


#endif

