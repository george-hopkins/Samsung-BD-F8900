/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_spp_prim.h"
#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_app.h"
#include "csr_bt_tpt_cmd.h"
#include "csr_bt_tpt_cmd_serialize.h"
#include "csr_bt_tpt_handler.h"
#include "csr_bt_demoapp.h"

void tptInitHandleSppPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_SPP_GET_INSTANCES_QID_CFM:
            {
                CsrBtSppGetInstancesQidCfm *prim;

                prim = (CsrBtSppGetInstancesQidCfm *) TptData->recvMsgP;

                if (prim->phandlesListSize)
                {
                    TptData->spp_qid = prim->phandlesList[0];

                    csrBtTptDebugPrint("get instances cfm CsrSchedQid: %u\n",
                        TptData->spp_qid);
                    CsrPmemFree(prim->phandlesList);
                }
                break;
            }
        default:
            {
                CsrBtSppFreeUpstreamMessageContents(CSR_BT_SPP_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected SPP prim type %u in state init\n", *primType);
                break;
            }
    }

    return;
}

void tptConnectingHandleSppPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_SPP_SERVICE_NAME_IND:
            {
                CsrBtSppServiceNameInd *prim;

                csrBtTptDebugPrint("CSR_BT_SPP_SERVICE_NAME_IND\n");

                prim = (CsrBtSppServiceNameInd *) TptData->recvMsgP;
                if (prim->serviceNameListSize)
                {
                    CsrBtSppServiceName *dst;

                    dst = &prim->serviceNameList[0];
                    csrBtTptDebugPrint("Connecting to %s.\n", dst->serviceName);
                    CsrBtSppServiceNameResSend(TptData->spp_qid, TRUE, dst->serviceHandle);

                    CsrPmemFree(prim->serviceNameList);
                }
                else
                {
                    csrBtTptClientPrint("Error: Wrong CSR_BT_SPP_SERVICE_NAME_IND received<n");
                }
                break;
            }
        case CSR_BT_SPP_CONNECT_IND:
            {
                CsrBtSppConnectInd *prim;

                csrBtTptDebugPrint("CSR_BT_SPP_CONNECT_IND\n");

                prim = (CsrBtSppConnectInd *) TptData->recvMsgP;
                if (prim->resultCode == CSR_BT_RESULT_CODE_SPP_SUCCESS &&
                    prim->resultSupplier == CSR_BT_SUPPLIER_SPP)
                {
                    TptData->spp_sc = prim->serverChannel;
                    tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTED);
                    csrBtTptClientPrint("SPP Connection established.\n\n");
                    csrBtTptClientPrint("Starting tests...");
                    TptData->retries = 0;
                }
                else if(TptData->retries > CSR_BT_TPT_CONNECT_RETRIES)
                {
                    csrBtTptClientPrint("SPP Connection establishment failed.\n");
                }
                else
                {
                    csrBtTptClientPrint("SPP Connect did not succeed. Trying again...\n");
                    TptData->retries++;
                    CsrBtSppConnectReqSend(TptData->spp_qid, TptData->phandle, TptData->tptPeerAddr,
                                      FALSE, NULL, CSR_BT_SPP_CLIENT_ROLE);
                }
                break;
            }
        default:
            {
                CsrBtSppFreeUpstreamMessageContents(CSR_BT_SPP_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected SPP prim type %u in state connecting\n", *primType);
                break;
            }
    }
    return;
}

void tptConnectedHandleSppPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_SPP_STATUS_IND:
            {
                csrBtTptDebugPrint("CSR_BT_SPP_STATUS_IND\n");
                break;
            }
        case CSR_BT_SPP_DATA_CFM:
            {
                csrBtTptDebugPrint("CSR_BT_SPP_DATA_CFM\n");
                /*
                 * Client received setup command.
                 */
                tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONFIGURING);
                break;
            }
        case CSR_BT_SPP_DISCONNECT_IND:
            {
                CsrBtSppDisconnectInd *prim;

                csrBtTptDebugPrint("CSR_BT_SPP_DISCONNECT_IND\n");

                prim = (CsrBtSppDisconnectInd *) TptData->recvMsgP;
                if (prim->localTerminated)
                {
                    csrBtTptClientPrint("SPP Disconnected.\n");
                    exit(0);
                }
                else
                {
                    csrBtTptClientPrint("Error: SPP Disconnect failed.\n");
                    exit(1);
                }
                break;
            }
        default:
            {
                CsrBtSppFreeUpstreamMessageContents(CSR_BT_SPP_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected SPP prim type %u in state connected\n", *primType);
                break;
            }
    }
    return;
}

void tptConfiguringHandleSppPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_SPP_DATA_CFM:
            {
                csrBtTptDebugPrint("CSR_BT_SPP_DATA_CFM\n");

                /* Client confirmed ready to test. */
                tptScheduleRun(TptData, CSR_BT_TPT_STATE_TESTING);
                break;
            }
        case CSR_BT_SPP_DATA_IND:
            {
                CsrBtSppDataInd  *prim;
                TptCommand_t    *cmd;

                prim = (CsrBtSppDataInd *) TptData->recvMsgP;

                csrBtTptDebugPrint("CSR_BT_SPP_DATA_IND\n");

                cmd = deserializeTptCommand(prim->payload);
                CsrPmemFree(prim->payload);

                tptHandleCommand(TptData, cmd);

                CsrBtSppDataResSend(TptData->spp_qid, TptData->spp_sc);
                break;
            }
        default:
            {
                CsrBtSppFreeUpstreamMessageContents(CSR_BT_SPP_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected SPP prim type %u in state configuring\n", *primType);
                break;
            }
    }
    return;
}

void tptTestingHandleSppPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_SPP_DATA_IND:
            {
                CsrBtSppDataInd  *prim;

                csrBtTptDebugPrint("CSR_BT_SPP_DATA_IND\n");

                prim = (CsrBtSppDataInd *) TptData->recvMsgP;

                if (TptData->tests[TptData->curTest].packetsDone == TptData->tests[TptData->curTest].packetCount ||
                   (TptData->tests[TptData->curTest].testType & TPT_TEST_AV))
                {
                    TptCommand_t *cmd;
                    /*
                     * Must be TPT_CMD_TESTRUN_CFM.
                     */
                    cmd = deserializeTptCommand(prim->payload);

                    tptHandleCommand(TptData, cmd);
                }
                else
                {
                    /*
                     * Just testing.
                     */
                    csrBtTptDebugPrint("SPP testing: received %u bytes\n", prim->payloadLength);
                    csrBtTptUpdatePrint(TptData);

                    if (TptData->tests[TptData->curTest].packetsDone == 0)
                    {
                        TptData->previousTimeStamp = csrBtTptHpGetTime();
                    }
                    else
                    {
                        CsrTime currentTime;
                        char    buffer[CSR_BT_TPT_MAX_LINE_LENGTH];

                        currentTime = csrBtTptHpGetTime();
                        sprintf(buffer, "%lu,", (currentTime - TptData->previousTimeStamp));
                        CsrBtFwrite(buffer, 1, CsrStrLen(buffer), TptData->sppScResultFilePtr);
                        TptData->previousTimeStamp = currentTime;
                    }
                    TptData->tests[TptData->curTest].packetsDone = TptData->tests[TptData->curTest].packetsDone + 1;
                }

                CsrPmemFree(prim->payload);
                CsrBtSppDataResSend(TptData->spp_qid, TptData->spp_sc);
                break;
            }
        case CSR_BT_SPP_DATA_CFM:
            {
                CsrUint8         *buf;

                csrBtTptDebugPrint("CSR_BT_SPP_DATA_CFM\n");

                if (TptData->tests[TptData->curTest].packetsDone == TptData->tests[TptData->curTest].packetCount)
                {
                    csrBtTptDebugPrint("SPP testing complete.\n");
                }
                else
                {
                    csrBtTptUpdatePrint(TptData);
                    buf = CsrPmemAlloc(TptData->tests[TptData->curTest].packetSize);
                    CsrBtSppDataReqSend(TptData->spp_qid, TptData->spp_sc, TptData->tests[TptData->curTest].packetSize, buf);
                    TptData->tests[TptData->curTest].packetsDone = TptData->tests[TptData->curTest].packetsDone + 1;
                }
                break;
            }
        default:
            {
                CsrBtSppFreeUpstreamMessageContents(CSR_BT_SPP_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected SPP prim type %u in state testing\n", *primType);
                break;
            }
    }

    return;
}

void tptReportingHandleSppPrim(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_SPP_DATA_CFM:
            {
                csrBtTptDebugPrint("CSR_BT_SPP_DATA_CFM\n");
                break;
            }
        case CSR_BT_SPP_DATA_IND:
            {
                CsrBtSppDataInd  *prim;
                TptCommand_t    *cmd;

                prim = (CsrBtSppDataInd *) TptData->recvMsgP;

                csrBtTptDebugPrint("CSR_BT_SPP_DATA_IND\n");

                cmd = deserializeTptCommand(prim->payload);
                CsrPmemFree(prim->payload);

                tptHandleCommand(TptData, cmd);

                CsrBtSppDataResSend(TptData->spp_qid, TptData->spp_sc);
                break;
            }
        default:
            {
                CsrBtSppFreeUpstreamMessageContents(CSR_BT_SPP_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected SPP prim type %u in state reporting\n", *primType);
                break;
            }
    }
    return;
}

