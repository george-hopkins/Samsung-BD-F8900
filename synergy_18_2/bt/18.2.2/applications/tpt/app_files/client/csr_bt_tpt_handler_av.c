/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_cmd.h"
#include "csr_bt_tpt_app.h"
#include "csr_bt_tpt_handler.h"
#include "csr_bt_av_prim.h"
#include "csr_bt_demoapp.h"

static CsrUint32 tptReadAvSeqNo(CsrUint8 *dataPtr, CsrUint32 offset)
{
    CsrUint32  result;

    result =  *(dataPtr + offset)     * 0x1000000;
    result += *(dataPtr + offset + 1) * 0x10000;
    result += *(dataPtr + offset + 2) * 0x100;
    result += *(dataPtr + offset + 3);

    return result;
}

void tptInitHandleAvPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        default:
            {
                CsrBtAvFreeUpstreamMessageContents(CSR_BT_AV_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected AV prim type %u in state init\n", *primType);
                break;
            }
    }

    return;
}

void tptConnectingHandleAvPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        default:
            {
                CsrBtAvFreeUpstreamMessageContents(CSR_BT_AV_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected AV prim type %u in state connecting\n", *primType);
                break;
            }
    }

    return;
}

void tptConnectedHandleAvPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        default:
            {
                CsrBtAvFreeUpstreamMessageContents(CSR_BT_AV_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected AV prim type %u in state connected\n", *primType);
                break;
            }
    }

    return;
}

void tptConfiguringHandleAvPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_AV_ACTIVATE_CFM:
            {
                CsrBtAvActivateCfm *prim;

                csrBtTptDebugPrint("CSR_BT_AV_ACTIVATE_CFM\n");

                prim = (CsrBtAvActivateCfm *) TptData->recvMsgP;
                if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
                    prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
                {
                    csrBtTptDebugPrint("AV activated\n");
                    if (TptData->tests[TptData->curTest].testType & TPT_TESTDIR_CS)
                    {
                        CsrBtAvConnectReqSend(TptData->phandle, TptData->tptPeerAddr, CSR_BT_AV_AUDIO_SINK);
                    }
                    else
                    {
                        CsrBtAvConnectReqSend(TptData->phandle, TptData->tptPeerAddr, CSR_BT_AV_AUDIO_SOURCE);
                    }
                }
                else
                {
                    csrBtTptClientPrint("Error:  AV not activated\n");
                }
                break;
            }
        case CSR_BT_AV_CONNECT_CFM:
            {
                CsrBtAvConnectCfm *prim;

                csrBtTptDebugPrint("CSR_BT_AV_CONNECT_CFM\n");

                prim = (CsrBtAvConnectCfm *) TptData->recvMsgP;
                if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
                    prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
                {
                    csrBtTptDebugPrint("AV connected\n");
                    TptData->av_cid = prim->connectionId;
                    TptData->retries = 0;
                    CsrBtAvDiscoverReqSend(TptData->av_cid, TptData->av_tlabel);
                }
                else if(TptData->retries > CSR_BT_TPT_CONNECT_RETRIES)
                {
                    csrBtTptClientPrint("Error: AV not connected\n");
                }
                else
                {
                    csrBtTptClientPrint("AV connect did not succeed. Trying again...\n");
                    TptData->retries++;
                    if (TptData->tests[TptData->curTest].testType & TPT_TESTDIR_CS)
                    {
                        CsrBtAvConnectReqSend(TptData->phandle, TptData->tptPeerAddr, CSR_BT_AV_AUDIO_SINK);
                    }
                    else
                    {
                        CsrBtAvConnectReqSend(TptData->phandle, TptData->tptPeerAddr, CSR_BT_AV_AUDIO_SOURCE);
                    }
                }
                break;
            }
        case CSR_BT_AV_DISCOVER_CFM:
            {
                CsrBtAvDiscoverCfm *prim;

                csrBtTptDebugPrint("CSR_BT_AV_DISCOVER_CFM\n");

                prim = (CsrBtAvDiscoverCfm *) TptData->recvMsgP;
                if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
                    prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
                {

                    TptData->av_peersep = prim->seidInfo[0].acpSeid;
                    csrBtTptDebugPrint("Peer acceptor stream ID: %u\n", TptData->av_peersep);
                    CsrBtAvSetConfigReqSend(TptData->av_cid, TptData->av_tlabel,
                        TptData->av_peersep, 0, 0, NULL);
                    CsrPmemFree(prim->seidInfo);
                }
                else
                {
                    csrBtTptClientPrint("Error:  Unable to obtain list of AV acceptor endpoints.\n");
                }
                break;
            }
        case CSR_BT_AV_SET_CONFIGURATION_CFM:
            {
                CsrBtAvSetConfigurationCfm *prim;

                csrBtTptDebugPrint("CSR_BT_AV_SET_CONFIGURATION_CFM\n");

                prim = (CsrBtAvSetConfigurationCfm *) TptData->recvMsgP;
                if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
                    prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
                {
                    TptData->av_shandle = prim->shandle;
                    csrBtTptDebugPrint("AV stream configured, handle ID %u\n", TptData->av_shandle);
                    CsrBtAvOpenReqSend(TptData->av_shandle, TptData->av_tlabel);
                }
                else
                {
                    csrBtTptClientPrint("Error: AV stream not configured.\n");
                }
                break;
            }
        case CSR_BT_AV_OPEN_CFM:
            {
                CsrBtAvOpenCfm *prim;

                csrBtTptDebugPrint("CSR_BT_AV_OPEN_CFM\n");

                prim = (CsrBtAvOpenCfm *) TptData->recvMsgP;
                if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
                    prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
                {
                    csrBtTptDebugPrint("AV stream opened.\n");

                    TptData->av_open = TRUE;
                }
                else
                {
                    csrBtTptClientPrint("Error: AV stream not opened.\n");
                }
                break;
            }
        case CSR_BT_AV_STREAM_MTU_SIZE_IND:
            {
                CsrBtAvStreamMtuSizeInd *prim;

                csrBtTptDebugPrint("CSR_BT_AV_STREAM_MTU_SIZE_IND\n");

                prim = (CsrBtAvStreamMtuSizeInd *) TptData->recvMsgP;
                csrBtTptClientPrint("Remote AV MTU size: %u\n", prim->remoteMtuSize);

                /*
                 * Start test if client has already confirmed
                 * the test parameters, otherwise this is done
                 * from the command handler.
                 */

                csrBtTptDebugPrint("test_configured: %u\n", TptData->test_configured);
                if (TptData->test_configured)
                {
                    TptCommand_t *cmd;

                    cmd = CsrPmemZalloc(sizeof(*cmd));
                    cmd->cmdOpcode = TPT_CMD_TESTRUN_REQ;
                    cmd->testType = TptData->tests[TptData->curTest].testType;

                    tptCommandSend(TptData, cmd);
                    CsrPmemFree(cmd);
                    TptData->testRunCfmReceived = FALSE;
                }
                break;
            }
        default:
            {
                CsrBtAvFreeUpstreamMessageContents(CSR_BT_AV_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected AV prim type %u in state configuring\n", *primType);
                break;
            }
    }
    return;
}

void tptTestingHandleAvPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_AV_OPEN_CFM:
            {
                CsrBtAvOpenCfm *prim;

                csrBtTptDebugPrint("CSR_BT_AV_OPEN_CFM\n");

                prim = (CsrBtAvOpenCfm *) TptData->recvMsgP;
                if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
                    prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
                {
                    csrBtTptClientPrint("AV stream opened.\n");
                }
                else
                {
                   csrBtTptClientPrint("Error: AV stream not opened.\n");
                }
                break;
            }
        case CSR_BT_AV_STREAM_MTU_SIZE_IND:
            {
                CsrBtAvStreamMtuSizeInd *prim;

                csrBtTptDebugPrint("CSR_BT_AV_STREAM_MTU_SIZE_IND\n");

                prim = (CsrBtAvStreamMtuSizeInd *) TptData->recvMsgP;
                csrBtTptClientPrint("Remote AV MTU size: %u\n", prim->remoteMtuSize);
                break;
            }
        case CSR_BT_AV_START_CFM:
            {
                CsrBtAvStartCfm *prim;
                /*
                 * Because of a race, handle this here
                 * as well as below.
                 */
                csrBtTptDebugPrint("CSR_BT_AV_START_CFM\n");

                prim = (CsrBtAvStartCfm *) TptData->recvMsgP;

                if (prim->avResultSupplier != CSR_BT_SUPPLIER_AV ||
                    prim->avResultCode != CSR_BT_RESULT_CODE_AV_SUCCESS)
                {
                    csrBtTptClientPrint("Error:  AV Stream not started.\n");
                    return;
                }

                if (TptData->tests[TptData->curTest].testType & TPT_TESTDIR_CS)
                {
                    int i;

                    /*
                     * The ``+ 1'' is to ensure that we'll get an
                     * CSR_BT_AV_QOS_IND.  First packet goes directly to
                     * L2CAP.
                     */
                    for (i = 0; i < (CSR_BT_AV_MEDIA_BUFFER_SIZE + 1) &&
                        TptData->tests[TptData->curTest].packetsDone <
                        TptData->tests[TptData->curTest].packetCount;
                        i++,
                        TptData->tests[TptData->curTest].packetsDone = TptData->tests[TptData->curTest].packetsDone + 1)
                    {
                        CsrUint8 *buf;

                        csrBtTptDebugPrint("AV sent: %u\n", i);
                        csrBtTptUpdatePrint(TptData);

                        buf = CsrPmemAlloc(TptData->tests[TptData->curTest].packetSize);
                        CsrBtAvStreamDataReqSend(TptData->av_shandle, FALSE, FALSE, 0,
                            TptData->tests[TptData->curTest].packetsDone, /* abuse to carry sequence no. */
                            TptData->tests[TptData->curTest].packetSize, buf)
                    }

                    if (TptData->tests[TptData->curTest].packetsDone ==
                        TptData->tests[TptData->curTest].packetCount)
                    {
                        csrBtTptClientPrint("Waiting for test completion signal.\n");
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
                    csrBtTptDebugPrint("AV tx buffer full.\n");
                }
                else
                {
                    int i;

                    csrBtTptDebugPrint("AV tx buffer: %u.\n", prim->bufferStatus);

                    for (i = 0; i < (CSR_BT_AV_MEDIA_BUFFER_SIZE + 1) &&
                        TptData->tests[TptData->curTest].packetsDone < TptData->tests[TptData->curTest].packetCount;
                        i++,
                        TptData->tests[TptData->curTest].packetsDone = TptData->tests[TptData->curTest].packetsDone + 1)
                    {
                        CsrUint8 *buf;

                        csrBtTptDebugPrint("AV sent: %u\n", i);
                        csrBtTptUpdatePrint(TptData);

                        buf = CsrPmemAlloc(TptData->tests[TptData->curTest].packetSize);
                        CsrBtAvStreamDataReqSend(TptData->av_shandle, FALSE, FALSE, 0,
                            TptData->tests[TptData->curTest].packetsDone, /* abuse to carry sequence no. */
                            TptData->tests[TptData->curTest].packetSize, buf)
                    }
                }
                break;
            }
        case CSR_BT_AV_STREAM_DATA_IND:
            {
                CsrBtAvStreamDataInd    *prim;
                CsrTime                 currentTime;
                CsrUint32               seqNo;
                char                    buffer[CSR_BT_TPT_MAX_LINE_LENGTH];

                csrBtTptDebugPrint("CSR_BT_AV_STREAM_DATA_IND\n");

                currentTime = csrBtTptHpGetTime();

                csrBtTptUpdatePrint(TptData);
                prim = (CsrBtAvStreamDataInd *) TptData->recvMsgP;

                /* Extract sequence number from the media packet header `timestamp' field. */
                seqNo = tptReadAvSeqNo(prim->data, 4);

                csrBtTptDebugPrint("AV: Received %u bytes pkt %lu.\n", prim->length, seqNo);

                if (!(TptData->tests[TptData->curTest].packetsDone))
                {
                    TptData->previousTimeStamp = currentTime;
                }
                sprintf(buffer, "%lu,%lu,", (currentTime - TptData->previousTimeStamp), seqNo);
                CsrBtFwrite(buffer, 1, CsrStrLen(buffer), TptData->avScResultFilePtr);

                TptData->previousTimeStamp = currentTime;
                CsrPmemFree(prim->data);
                TptData->tests[TptData->curTest].packetsDone = TptData->tests[TptData->curTest].packetsDone + 1;

                if (TptData->tests[TptData->curTest].packetsDone ==
                TptData->tests[TptData->curTest].packetCount)
                {
                    csrBtTptDebugPrint("AV test completed.\n");
                    if(TptData->testRunCfmReceived)
                    {
                        CsrBtAvCloseReqSend(TptData->av_shandle, TptData->av_tlabel);
                    }
                    else
                    {
                        csrBtTptDebugPrint("Waiting for TPT_CMD_TESTRUN_CFM\n");
                    }
                }
                break;
            }
        case CSR_BT_AV_CLOSE_CFM:
            {
                csrBtTptDebugPrint("CSR_BT_AV_CLOSE_CFM\n");
                TptData->av_open = FALSE;
                CsrBtAvDisconnectReqSend(TptData->av_cid);
                break;
            }
        case CSR_BT_AV_DISCONNECT_IND:
            {
                csrBtTptDebugPrint("CSR_BT_AV_DISCONNECT_IND\n");
                if (TptData->tests[TptData->curTest].testType & TPT_TESTDIR_CS)
                {
                    CsrBtAvDeactivateReqSend(CSR_BT_AV_AUDIO_SOURCE);
                }
                else
                {
                    CsrBtAvDeactivateReqSend(CSR_BT_AV_AUDIO_SINK);
                }
                break;
            }
        case CSR_BT_AV_DEACTIVATE_CFM:
            {
                CsrBtAvDeactivateCfm *prim;

                csrBtTptDebugPrint("CSR_BT_AV_DEACTIVATE_CFM\n");

                prim = (CsrBtAvDeactivateCfm *) TptData->recvMsgP;

                if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
                    prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
                {
                    csrBtTptDebugPrint("AV deactivated.\n");
                    if (TptData->tests[TptData->curTest].testType == CSR_BT_TPT_TEST_AV_SC && TptData->avScResultFilePtr)
                    {
                        csrBtTptDebugPrint("closing AV result file\n");
                        CsrBtFclose(TptData->avScResultFilePtr);
                    }
                    tptScheduleRun(TptData, CSR_BT_TPT_STATE_REPORTING);
                }
                else
                {
                    csrBtTptDebugPrint("Error:  AV not deactivated.\n");
                }
                break;
            }
        default:
            {
                CsrBtAvFreeUpstreamMessageContents(CSR_BT_AV_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected AV prim type %u in state testing\n", *primType);
                break;
            }
    }
    return;
}

void tptReportingHandleAvPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        default:
            {
                CsrBtAvFreeUpstreamMessageContents(CSR_BT_AV_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected AV prim type %u in state reporting\n", *primType);
                break;
            }
    }

    return;
}

