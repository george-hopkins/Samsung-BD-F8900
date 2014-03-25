/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_srv.h"
#include "csr_bt_tpt_handler.h"

static CsrUint32 tptReadAvSeqNo(CsrUint8 *dataPtr, CsrUint32 offset)
{
    CsrUint32  result;

    result =  *(dataPtr + offset)     * 0x1000000;
    result += *(dataPtr + offset + 1) * 0x10000;
    result += *(dataPtr + offset + 2) * 0x100;
    result += *(dataPtr + offset + 3);

    return result;
}

void tptInitHandleAvPrim(CsrBtTptServerInstanceDataType *TptData)
{
    return;
}

void tptConnectableHandleAvPrim(CsrBtTptServerInstanceDataType *TptData)
{
    return;
}

void tptConnectedHandleAvPrim(CsrBtTptServerInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_AV_CLOSE_IND:
            {
                CsrBtAvCloseResAcpSend(TptData->av_shandle, TptData->av_tlabel);
                break;
            }
        case CSR_BT_AV_DISCONNECT_IND:
            {
                if (TptData->curTest.testType & TPT_TESTDIR_CS)
                {
                    CsrBtAvDeactivateReqSend(CSR_BT_AV_AUDIO_SINK);
                }
                else
                {
                    CsrBtAvDeactivateReqSend(CSR_BT_AV_AUDIO_SOURCE);
                }
                break;
            }
        case CSR_BT_AV_DEACTIVATE_CFM:
            {
                /* Do nothing */
                break;
            }
        default:
            {
                break;
            }
    }
    return;
}

void tptConfiguringHandleAvPrim(CsrBtTptServerInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_AV_ACTIVATE_CFM:
            {
                /* Do nothing */
                break;
            }
        case CSR_BT_AV_CONNECT_IND:
            {
                /* Do nothing */
                break;
            }
        case CSR_BT_AV_DISCOVER_IND:
            {
                CsrBtAvSeidInfo  *seidinfo = CsrPmemAlloc(sizeof(CsrBtAvSeidInfo));

                seidinfo->acpSeid   = 0;
                seidinfo->inUse     = FALSE;
                seidinfo->mediaType = CSR_BT_AV_AUDIO;
                if (TptData->curTest.testType & TPT_TESTDIR_CS)
                {
                    seidinfo->sepType   = CSR_BT_AV_SINK;
                }
                else
                {
                    seidinfo->sepType   = CSR_BT_AV_SOURCE;
                }

                CsrBtAvDiscoverResAcpSend(TptData->av_cid, TptData->av_tlabel, 1, seidinfo);
                break;
            }
        case CSR_BT_AV_SET_CONFIGURATION_IND:
            {
                CsrBtAvSetConfigurationInd *prim;

                prim = (CsrBtAvSetConfigurationInd *) TptData->recvMsgP;
                TptData->av_shandle = prim->shandle;

                CsrBtAvSetConfigResAcpSend(TptData->av_shandle, TptData->av_tlabel);
                CsrPmemFree(prim->servCapData);
                break;
            }
        case CSR_BT_AV_OPEN_IND:
            {
                CsrBtAvOpenResAcpSend(TptData->av_shandle, TptData->av_tlabel);
                break;
            }
        case CSR_BT_AV_STREAM_MTU_SIZE_IND:
            {
                tptScheduleRun(TptData, CSR_BT_TPT_STATE_TESTING);
                break;
            }
        default:
            {
                break;
            }
    }
    return;
}

void tptTestingHandleAvPrim(CsrBtTptServerInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_AV_START_IND:
            {
                CsrBtAvStartInd *prim;

                prim = (CsrBtAvStartInd *) TptData->recvMsgP;

                /*
                 * Accept stream start.
                 */
                CsrBtAvStartResAcpSend(TptData->av_tlabel, prim->listLength, prim->list);

                if (TptData->curTest.testType & TPT_TESTDIR_SC)
                {
                    int i;

                    /*
                     * The ``+ 1'' is to ensure that we'll get an
                     * CSR_BT_AV_QOS_IND.  First packet goes directly to
                     * L2CAP.
                     */
                    for (i = 0; i < (CSR_BT_AV_MEDIA_BUFFER_SIZE + 1) &&
                        TptData->curTest.packetsDone < TptData->curTest.packetCount;
                        i++, TptData->curTest.packetsDone = TptData->curTest.packetsDone + 1)
                    {
                        CsrUint8 *buf;

                        buf = CsrPmemAlloc(TptData->curTest.packetSize);
                        CsrBtAvStreamDataReqSend(TptData->av_shandle, FALSE, FALSE, 0,
                            TptData->curTest.packetsDone, /* abuse to carry sequence no. */
                            TptData->curTest.packetSize, buf)
                    }

                    if (TptData->curTest.packetsDone == TptData->curTest.packetCount)
                    {
                        CsrTime delay;

                        delay = CSR_BT_AV_MEDIA_BUFFER_SIZE / 10 * 1000000 / 5;
                        CsrSchedTimerSet(delay, tptDelayedReportTransition, 0, TptData);
                    }
                }
                break;
            }
        case CSR_BT_AV_QOS_IND:
            {
                CsrBtAvQosInd *prim;

                prim = (CsrBtAvQosInd *) TptData->recvMsgP;

                if (prim->bufferStatus == 10)
                {
                    /* AV tx buffer is full */
                }
                else
                {
                    int i;

                    for (i = 0; i < (CSR_BT_AV_MEDIA_BUFFER_SIZE + 1) &&
                        TptData->curTest.packetsDone < TptData->curTest.packetCount;
                        i++, TptData->curTest.packetsDone = TptData->curTest.packetsDone + 1)
                    {
                        CsrUint8 *buf;

                        buf = CsrPmemAlloc(TptData->curTest.packetSize);
                        CsrBtAvStreamDataReqSend(TptData->av_shandle, FALSE, FALSE, 0,
                            TptData->curTest.packetsDone, /* abuse to carry sequence no. */
                            TptData->curTest.packetSize, buf)
                    }

                    if (TptData->curTest.packetsDone == TptData->curTest.packetCount)
                    {
                        CsrTime delay;

                        delay = CSR_BT_AV_MEDIA_BUFFER_SIZE / 10 * 1000000 / 5;

                        CsrSchedTimerSet(delay, tptDelayedReportTransition, 0, TptData);
                    }
                }
                break;
            }
        case CSR_BT_AV_STREAM_DATA_IND:
            {
                CsrBtAvStreamDataInd *prim;
                CsrTime now;

                prim = (CsrBtAvStreamDataInd *) TptData->recvMsgP;

                /*
                 * Get timestamp ASAP.
                 */
                now = CsrTimeGet(NULL);

                /* Extract sequence number from the media packet header `timestamp' field. */
                TptData->seqnos[TptData->curTest.packetsDone] = tptReadAvSeqNo(prim->data, 4);

                CsrPmemFree(prim->data);

                if (TptData->curTest.packetsDone != 0)
                {
                    /*
                     * Don't get dT of first packet -- doesn't make sense.
                     */
                    TptData->timestamps[TptData->curTest.packetsDone - 1] = now - TptData->lastTime;
                }
                TptData->lastTime = now;
                TptData->curTest.packetsDone = TptData->curTest.packetsDone + 1;

                if (TptData->curTest.packetsDone == TptData->curTest.packetCount)
                {
                    CsrTime delay;

                    delay = CSR_BT_AV_MEDIA_BUFFER_SIZE / 10 * 1000000 / 5;

                    CsrSchedTimerSet(delay, tptDelayedReportTransition, 0, TptData);
                }
                break;
            }
        default:
            {
                break;
            }
    }
    return;
}

void tptReportingHandleAvPrim(CsrBtTptServerInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_AV_CLOSE_IND:
            {
                CsrBtAvCloseResAcpSend(TptData->av_shandle, TptData->av_tlabel);
                break;
            }
        case CSR_BT_AV_DISCONNECT_IND:
            {
                /* Do nothing */
                break;
            }
        case CSR_BT_AV_DEACTIVATE_CFM:
            {
                CsrBtAvDeactivateCfm *prim;

                prim = (CsrBtAvDeactivateCfm *) TptData->recvMsgP;

                if (prim->avResultCode != CSR_BT_AV_ACCEPT ||
                    prim->avResultSupplier != CSR_BT_SUPPLIER_AV)
                {
                    /* Error:  AV not deactivated */
                }
                else
                {
                    TptData->avDeactivated = TRUE;
                }

                if (TptData->curTest.testType == CSR_BT_TPT_TEST_AV_SC)
                {
                    tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTED);
                }
                else
                {
                    if (!TptData->sendingResults && TptData->avResultsSent)
                    {
                        tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTED);
                    }
                    else
                    {
                        /* Do nothing. We are waiting for a TPT_CMD_RESULT_REQ */
                    }
                }
                break;
            }
        default:
            {
                break;
            }
    }
    return;
}

