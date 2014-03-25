/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ppp_hdlc.h"
#include "csr_bt_lsl_prim.h"
#include "csr_bt_lsl_to_prim_converter.h"

CsrBool PppHdlcReceive(PppInstanceData * instanceData)
{
    CsrBtLslDataInd *lslDataInd;

    if (instanceData->pppDataInd == NULL)
    {
        instanceData->pppDataInd = (CsrBtPppDataInd *) CsrPmemAlloc(sizeof(CsrBtPppDataInd));
        instanceData->pppDataInd->payload = (CsrUint8 *) CsrPmemAlloc(instanceData->mru + 2);    /* reserve room for the FCS */
        instanceData->pppDataInd->payloadLength = 0;
    }

    lslDataInd = (CsrBtLslDataInd*) instanceData->receivedMessage;

    for(; instanceData->currentPayloadIndex < lslDataInd->length; instanceData->currentPayloadIndex++)
    {
        if ((lslDataInd->payload[instanceData->currentPayloadIndex] < 0x20) &&
            (instanceData->rxAsynchronousControlCharacterMap[lslDataInd->payload[instanceData->currentPayloadIndex]]))
        {
            ;    /* skip! */
        }
        else
        {
            switch (instanceData->rxHdlcState)
            {
                case HDLC_STATE_SEARCHING_FOR_FRAME_DELIMITER:
                    if (lslDataInd->payload[instanceData->currentPayloadIndex] == HDLC_FRAME_DELIMITER)
                    {
                        instanceData->rxHdlcState = HDLC_STATE_HEADER_1;
                    }
                    break;

                case HDLC_STATE_HEADER_1:
                    switch (lslDataInd->payload[instanceData->currentPayloadIndex])
                    {
                        case HDLC_FRAME_DELIMITER:
                            /* just eat it at this point */
                            break;

                        case HDLC_ESCAPE_CHARACTER:
                            instanceData->rxHdlcEscapeFound = TRUE;
                            break;

                        default:
                            if (instanceData->rxHdlcEscapeFound)
                            {
                                lslDataInd->payload[instanceData->currentPayloadIndex] = UN_ESCAPE_BYTE(lslDataInd->payload[instanceData->currentPayloadIndex]);
                                instanceData->rxHdlcEscapeFound = FALSE;
                            }
                            if (lslDataInd->payload[instanceData->currentPayloadIndex] == 0xFF)
                            {
                                instanceData->rxHdlcState = HDLC_STATE_HEADER_2;
                            }
                            else
                            {
                                instanceData->rxHdlcState = HDLC_STATE_SEARCHING_FOR_FRAME_DELIMITER;
                            }
                            break;
                    }
                    break;

                case HDLC_STATE_HEADER_2:
                    switch (lslDataInd->payload[instanceData->currentPayloadIndex])
                    {
                        case HDLC_ESCAPE_CHARACTER:
                            instanceData->rxHdlcEscapeFound = TRUE;
                            break;

                        default:
                            if (instanceData->rxHdlcEscapeFound)
                            {
                                lslDataInd->payload[instanceData->currentPayloadIndex] = UN_ESCAPE_BYTE(lslDataInd->payload[instanceData->currentPayloadIndex]);
                                instanceData->rxHdlcEscapeFound = FALSE;
                            }
                            if (lslDataInd->payload[instanceData->currentPayloadIndex] == 0x03)
                            {
                                instanceData->rxHdlcState = HDLC_STATE_PROTOCOL_1;
                            }
                            else
                            {
                                instanceData->rxHdlcState = HDLC_STATE_SEARCHING_FOR_FRAME_DELIMITER;
                            }
                            break;
                    }
                    break;

                case HDLC_STATE_PROTOCOL_1:
                    switch (lslDataInd->payload[instanceData->currentPayloadIndex])
                    {
                        case HDLC_ESCAPE_CHARACTER:
                            instanceData->rxHdlcEscapeFound = TRUE;
                            break;

                        case HDLC_FRAME_DELIMITER:
                            instanceData->rxHdlcState = HDLC_STATE_SEARCHING_FOR_FRAME_DELIMITER;
                            break;

                        default:
                            if (instanceData->rxHdlcEscapeFound)
                            {
                                lslDataInd->payload[instanceData->currentPayloadIndex] = UN_ESCAPE_BYTE(lslDataInd->payload[instanceData->currentPayloadIndex]);
                                instanceData->rxHdlcEscapeFound = FALSE;
                            }
                            instanceData->hdlcAndPppHeader[2] = lslDataInd->payload[instanceData->currentPayloadIndex];
                            instanceData->rxHdlcState = HDLC_STATE_PROTOCOL_2;
                            break;
                    }
                    break;

                case HDLC_STATE_PROTOCOL_2:
                    switch (lslDataInd->payload[instanceData->currentPayloadIndex])
                    {
                        case HDLC_ESCAPE_CHARACTER:
                            instanceData->rxHdlcEscapeFound = TRUE;
                            break;

                        case HDLC_FRAME_DELIMITER:
                            instanceData->rxHdlcState = HDLC_STATE_SEARCHING_FOR_FRAME_DELIMITER;
                            break;

                        default:
                            if (instanceData->rxHdlcEscapeFound)
                            {
                                lslDataInd->payload[instanceData->currentPayloadIndex] = UN_ESCAPE_BYTE(lslDataInd->payload[instanceData->currentPayloadIndex]);
                                instanceData->rxHdlcEscapeFound = FALSE;
                            }
                            instanceData->hdlcAndPppHeader[3] = lslDataInd->payload[instanceData->currentPayloadIndex];
                            instanceData->rxHdlcState = HDLC_STATE_SEARCHING_FOR_END_FRAME_DELIMITER;
                            break;
                    }
                    break;

                case HDLC_STATE_SEARCHING_FOR_END_FRAME_DELIMITER:
                    switch (lslDataInd->payload[instanceData->currentPayloadIndex])
                    {
                        case HDLC_ESCAPE_CHARACTER:
                            instanceData->rxHdlcEscapeFound = TRUE;
                            break;

                        case HDLC_FRAME_DELIMITER:
                            instanceData->rxHdlcState = HDLC_STATE_HEADER_1;
                            if (instanceData->rxHdlcEscapeFound)
                            {
                                /* silently discard */
                                instanceData->pppDataInd->payloadLength = 0;
                            }
                            else
                            {
                                instanceData->currentPayloadIndex++;
                                return TRUE;
                            }
                            break;

                        default:
                            if (instanceData->rxHdlcEscapeFound)
                            {
                                lslDataInd->payload[instanceData->currentPayloadIndex] = UN_ESCAPE_BYTE(lslDataInd->payload[instanceData->currentPayloadIndex]);
                                instanceData->rxHdlcEscapeFound = FALSE;
                            }
                            if (instanceData->pppDataInd->payloadLength < instanceData->mru + 2)
                            {
                                instanceData->pppDataInd->payload[instanceData->pppDataInd->payloadLength++] = lslDataInd->payload[instanceData->currentPayloadIndex];
                            }
                            else
                            {
                                /* too big (> mru), so silently discard */
                                instanceData->pppDataInd->payloadLength = 0;
                                instanceData->rxHdlcState = HDLC_STATE_SEARCHING_FOR_END_FRAME_DELIMITER;
                            }
                            break;
                    }
                    break;
            }
        }
    }

    return FALSE;
}

/*
  PppHdlcTransmit() will (start or continue to) transmit the frame contained in the
  instanceData->pppDataReq variable or pop the next from the queue.

  So, when a new request comes in, it is up to the code that calls PppHdlcTransmit()
  to either push the request to the instanceData->dataReqQueue or put it into the
  instanceData->pppDataReq variable.

  In addition, if the request is put into the instanceData->pppDataReq variable, it
  is up to the calling code to set instanceData->txHdlcState = HDLC_STATE_TRANSMIT_START_DELIMITER.

  Call this function upon reception of requests from above and upon data confirmations from below.
*/
void PppHdlcTransmit(PppInstanceData * instanceData)
{
    if (instanceData->transmissionAllowed && (instanceData->pppDataReq || instanceData->dataReqQueue || instanceData->numberOnQueue))
    {
        CsrUint16 maxPayloadSize;
        CsrUint16 payloadLength;
        CsrUint8 * payload;
        CsrBool packetComplete = FALSE;

        maxPayloadSize = instanceData->maxPayloadLength < instanceData->mtu ? instanceData->maxPayloadLength : instanceData->mtu;

        if (instanceData->pppDataReq == NULL)
        {
            if (instanceData->dataReqQueue)
            {
                CsrUint16 dummyEvent;

                CsrMessageQueuePop(&instanceData->dataReqQueue, &dummyEvent, (void **) &instanceData->pppDataReq);
            }
            else
            {
                instanceData->pppDataReq = instanceData->pppDataReqQueue[instanceData->queueTail++];
                if (instanceData->queueTail >= CSR_BT_PPP_DATA_REQ_QUEUE_SIZE)
                {
                    instanceData->queueTail = 0;
                }
                instanceData->numberOnQueue--;
            }

            instanceData->txHdlcEscapeSent = FALSE;
            instanceData->txHdlcState = HDLC_STATE_TRANSMIT_START_DELIMITER;
        }

        payload = (CsrUint8 *) CsrPmemAlloc(maxPayloadSize);

        for (payloadLength = 0; !packetComplete && (payloadLength < maxPayloadSize); payloadLength++)
        {
            switch (instanceData->txHdlcState)
            {
                case HDLC_STATE_TRANSMIT_START_DELIMITER:
                    payload[payloadLength] = HDLC_FRAME_DELIMITER;
                    instanceData->txHdlcState = HDLC_STATE_TRANSMIT_HEADER_1;
                    break;

                case HDLC_STATE_TRANSMIT_HEADER_1:
                    payload[payloadLength] = 0xFF;
                    instanceData->txHdlcState = HDLC_STATE_TRANSMIT_HEADER_2;
                    break;

                case HDLC_STATE_TRANSMIT_HEADER_2:
                    if (!instanceData->txHdlcEscapeSent && instanceData->txAsynchronousControlCharacterMap[0x03])
                    {
                        payload[payloadLength] = HDLC_ESCAPE_CHARACTER;
                        instanceData->txHdlcEscapeSent = TRUE;
                    }
                    else
                    {
                        if (instanceData->txHdlcEscapeSent)
                        {
                            payload[payloadLength] = ESCAPE_BYTE(0x03);
                            instanceData->txHdlcEscapeSent = FALSE;
                        }
                        else
                        {
                            payload[payloadLength] = 0x03;
                        }
                        instanceData->txHdlcState = HDLC_STATE_TRANSMIT_PROTOCOL_1;
                    }
                    break;

                case HDLC_STATE_TRANSMIT_PROTOCOL_1:
                    {
                        CsrUint8 protocolHighByte;

                        protocolHighByte = ((CsrUint8) ((CSR_BT_LAN_WORD(instanceData->pppDataReq->protocol) >> 8) & 0x00FF));
                        if (!instanceData->txHdlcEscapeSent && (protocolHighByte < 0x20) && instanceData->txAsynchronousControlCharacterMap[protocolHighByte])
                        {
                            payload[payloadLength] = HDLC_ESCAPE_CHARACTER;
                            instanceData->txHdlcEscapeSent = TRUE;
                        }
                        else
                        {
                            if (instanceData->txHdlcEscapeSent)
                            {
                                payload[payloadLength] = ESCAPE_BYTE(protocolHighByte);
                                instanceData->txHdlcEscapeSent = FALSE;
                            }
                            else
                            {
                                payload[payloadLength] = protocolHighByte;
                            }
                            instanceData->txHdlcState = HDLC_STATE_TRANSMIT_PROTOCOL_2;
                        }
                        break;
                    }

                case HDLC_STATE_TRANSMIT_PROTOCOL_2:
                    {
                        CsrUint8 protocolLowByte;

                        protocolLowByte = ((CsrUint8) (CSR_BT_LAN_WORD(instanceData->pppDataReq->protocol) & 0x00FF));
                        if (!instanceData->txHdlcEscapeSent && (protocolLowByte < 0x20) && instanceData->txAsynchronousControlCharacterMap[protocolLowByte])
                        {
                            payload[payloadLength] = HDLC_ESCAPE_CHARACTER;
                            instanceData->txHdlcEscapeSent = TRUE;
                        }
                        else
                        {
                            if (instanceData->txHdlcEscapeSent)
                            {
                                payload[payloadLength] = ESCAPE_BYTE(protocolLowByte);
                                instanceData->txHdlcEscapeSent = FALSE;
                            }
                            else
                            {
                                payload[payloadLength] = protocolLowByte;
                            }
                            instanceData->txCurrentPayloadIndex = 0;
                            instanceData->txHdlcState = HDLC_STATE_TRANSMIT_DATA;
                        }
                        break;
                    }

                case HDLC_STATE_TRANSMIT_DATA:
                    if (!instanceData->txHdlcEscapeSent && (instanceData->pppDataReq->payload[instanceData->txCurrentPayloadIndex] < 0x20) && instanceData->txAsynchronousControlCharacterMap[instanceData->pppDataReq->payload[instanceData->txCurrentPayloadIndex]])
                    {
                        payload[payloadLength] = HDLC_ESCAPE_CHARACTER;
                        instanceData->txHdlcEscapeSent = TRUE;
                    }
                    else
                    {
                        if (instanceData->txHdlcEscapeSent)
                        {
                            payload[payloadLength] = ESCAPE_BYTE(instanceData->pppDataReq->payload[instanceData->txCurrentPayloadIndex++]);
                            instanceData->txHdlcEscapeSent = FALSE;
                        }
                        else
                        {
                            payload[payloadLength] = instanceData->pppDataReq->payload[instanceData->txCurrentPayloadIndex++];
                        }

                        if (instanceData->txCurrentPayloadIndex >= instanceData->pppDataReq->payloadLength)
                        {
                            CsrUint16 fcs;

                            fcs = CSR_BT_LAN_WORD(CalculateHdlcFrameFcs16(instanceData->pppDataReq->protocol, instanceData->pppDataReq->payloadLength, instanceData->pppDataReq->payload));
                            instanceData->txFcs[0] = ((CsrUint8) ((fcs >> 8) & 0x00FF));
                            instanceData->txFcs[1] = ((CsrUint8) (fcs & 0x00FF));
                            instanceData->txHdlcState = HDLC_STATE_TRANSMIT_FCS_1;
                        }
                    }
                    break;

                case HDLC_STATE_TRANSMIT_FCS_1:
                    {
                        if (!instanceData->txHdlcEscapeSent && (instanceData->txFcs[0] < 0x20) && instanceData->txAsynchronousControlCharacterMap[instanceData->txFcs[0]])
                        {
                            payload[payloadLength] = HDLC_ESCAPE_CHARACTER;
                            instanceData->txHdlcEscapeSent = TRUE;
                        }
                        else
                        {
                            if (instanceData->txHdlcEscapeSent)
                            {
                                payload[payloadLength] = ESCAPE_BYTE(instanceData->txFcs[0]);
                                instanceData->txHdlcEscapeSent = FALSE;
                            }
                            else
                            {
                                payload[payloadLength] = instanceData->txFcs[0];
                            }
                            instanceData->txHdlcState = HDLC_STATE_TRANSMIT_FCS_2;
                        }
                        break;
                    }

                case HDLC_STATE_TRANSMIT_FCS_2:
                    {
                        if (!instanceData->txHdlcEscapeSent && (instanceData->txFcs[1] < 0x20) && instanceData->txAsynchronousControlCharacterMap[instanceData->txFcs[1]])
                        {
                            payload[payloadLength] = HDLC_ESCAPE_CHARACTER;
                            instanceData->txHdlcEscapeSent = TRUE;
                        }
                        else
                        {
                            if (instanceData->txHdlcEscapeSent)
                            {
                                payload[payloadLength] = ESCAPE_BYTE(instanceData->txFcs[1]);
                                instanceData->txHdlcEscapeSent = FALSE;
                            }
                            else
                            {
                                payload[payloadLength] = instanceData->txFcs[1];
                            }
                            instanceData->txHdlcState = HDLC_STATE_TRANSMIT_END_DELIMITER;
                        }
                        break;
                    }

                case HDLC_STATE_TRANSMIT_END_DELIMITER:
                    payload[payloadLength] = HDLC_FRAME_DELIMITER;
                    packetComplete = TRUE;

                    CsrPmemFree(instanceData->pppDataReq->payload);
                    CsrPmemFree(instanceData->pppDataReq);
                    instanceData->pppDataReq = NULL;

                    instanceData->txHdlcState = HDLC_STATE_TRANSMIT_START_DELIMITER;
                    break;

            }
        }
        LslDataReqSend(payloadLength, payload);
        instanceData->transmissionAllowed = FALSE;
    }
}
