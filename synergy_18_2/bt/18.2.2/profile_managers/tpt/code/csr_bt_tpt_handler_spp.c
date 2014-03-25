/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_srv.h"
#include "csr_bt_tpt_cmd_serialize.h"
#include "csr_bt_tpt_handler.h"

void tptInitHandleSppPrim(CsrBtTptServerInstanceDataType *TptData)
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
                    CsrCharString *serviceName;

                    serviceName = (CsrCharString *) CsrStrDup("TPTCTRL");

                    TptData->spp_qid = prim->phandlesList[0];

                    CsrBtSppActivateReqSend(TptData->spp_qid, TptData->phandle, 0, CSR_BT_SPP_SERVER_ROLE, serviceName);

                    CsrPmemFree(prim->phandlesList);
                }
                break;
            }
        case CSR_BT_SPP_ACTIVATE_CFM:
            {
                CsrBtSppActivateCfm *prim;

                prim = (CsrBtSppActivateCfm *) TptData->recvMsgP;

                if (prim->queueId == TptData->spp_qid)
                {
                    tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTABLE);
                }
                else
                {
                    /* Do nothing */
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

void tptConnectableHandleSppPrim(CsrBtTptServerInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_SPP_CONNECT_IND:
            {
                CsrBtSppConnectInd *prim;

                prim = (CsrBtSppConnectInd *) TptData->recvMsgP;
                if (prim->resultCode == CSR_BT_RESULT_CODE_SPP_SUCCESS &&
                    prim->resultSupplier == CSR_BT_SUPPLIER_SPP)
                {
                    TptData->spp_sc = prim->serverChannel;
                    tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTED);
                }
                else
                {
                    /* Error connecting - do nothing */
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

void tptConnectedHandleSppPrim(CsrBtTptServerInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_SPP_STATUS_IND:
            {
                /* Do nothing */
                break;
            }
        case CSR_BT_SPP_DATA_CFM:
            {
                /* Do nothing */
                break;
            }
        case CSR_BT_SPP_DATA_IND:
            {
                CsrBtSppDataInd  *prim;
                TptCommand_t    *cmd;

                prim = (CsrBtSppDataInd *) TptData->recvMsgP;

                cmd = deserializeTptCommand(prim->payload);
                CsrPmemFree(prim->payload);

                tptHandleCommand(TptData, cmd);

                CsrBtSppDataResSend(TptData->spp_qid, TptData->spp_sc);
                break;
            }
        case CSR_BT_SPP_DISCONNECT_IND:
            {
                /* Reactivate SPP or close program */
                TptData->closingProgram = !TptData->reActivateOnDisconnect;
                tptScheduleRun(TptData, CSR_BT_TPT_STATE_INIT);
                break;
            }
        default:
            {
                break;
            }
    }

    return;
}

void tptConfiguringHandleSppPrim(CsrBtTptServerInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_SPP_DATA_CFM:
            {
                /*
                 * This means the client got our setup ack.
                 */
                break;
            }
        case CSR_BT_SPP_DATA_IND:
            {
                CsrBtSppDataInd  *prim;
                TptCommand_t    *cmd;

                prim = (CsrBtSppDataInd *) TptData->recvMsgP;

                cmd = deserializeTptCommand(prim->payload);
                CsrPmemFree(prim->payload);

                tptHandleCommand(TptData, cmd);

                /*
                 * Delay SPP ack until we're actually in testing state.
                 */
                break;
            }
        default:
            {
                break;
            }
    }

    return;
}

void tptTestingHandleSppPrim(CsrBtTptServerInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_SPP_DATA_IND:
            {
                CsrBtSppDataInd  *prim;
                CsrTime now;

                /*
                 * Get timestamp ASAP.
                 */
                now = CsrTimeGet(NULL);

                prim = (CsrBtSppDataInd *) TptData->recvMsgP;

                CsrBtSppDataResSend(TptData->spp_qid, TptData->spp_sc);

                if (TptData->test_started == FALSE)
                {
                    /*
                     * Must be TPT_CMD_TESTRUN_REQ.
                     */
                    TptCommand_t *cmd;

                    cmd = deserializeTptCommand(prim->payload);
                    tptHandleCommand(TptData, cmd);

                    /*
                     * Just return here, the command
                     * handler will start things if needed.
                     */
                    CsrPmemFree(prim->payload);
                    return;
                }
                CsrPmemFree(prim->payload);

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
                    /*
                     * All done, start reporting.
                     */
                    tptScheduleRun(TptData, CSR_BT_TPT_STATE_REPORTING);
                }
                break;
            }
        case CSR_BT_SPP_DATA_CFM:
            {
                if (TptData->test_started == FALSE)
                {
                    /*
                     * Just the SPP ack for TPT_CMD_SETUP_CFM.
                     */
                    return;
                }

                /*
                 * If we get to here, it means we're actually
                 * testing, and we only get CSR_BT_SPP_DATA_CFM if
                 * we're the sending part.
                 */
                TptData->curTest.packetsDone = TptData->curTest.packetsDone + 1;
                if (TptData->curTest.packetsDone == TptData->curTest.packetCount)
                {
                    /*
                     * All done, start reporting.
                     */
                    tptScheduleRun(TptData, CSR_BT_TPT_STATE_REPORTING);
                }
                else
                {
                    if (TptData->curTest.testType & TPT_TESTDIR_SC)
                    {
                        CsrUint8 *buf;

                        buf = CsrPmemAlloc(TptData->curTest.packetSize);
                        CsrBtSppDataReqSend(TptData->spp_qid, TptData->spp_sc,
                                       TptData->curTest.packetSize, buf);
                    }
                    else
                    {
                        /* Do nothing */
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

void tptReportingHandleSppPrim(CsrBtTptServerInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_SPP_DATA_CFM:
            {
                if(TptData->curTest.testType & TPT_TEST_SPP)
                {
                    if (TptData->curTest.testType == CSR_BT_TPT_TEST_SPP_SC)
                    {
                        tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTED);
                    }
                    else
                    {
                        if(!TptData->sendingResults && TptData->sppResultsSent)
                        {
                            tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTED);
                        }
                        else if (!TptData->sendingResults && !TptData->sppResultsSent)
                        {
                            /* Do nothing */
                        }
                        else
                        {
                            tptSendResultData(TptData);
                        }
                    }
                }
                else
                {
                    if (TptData->curTest.testType == CSR_BT_TPT_TEST_AV_SC)
                    {
                        tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTED);
                    }
                    else
                    {
                        if (!TptData->sendingResults && TptData->avDeactivated && TptData->avResultsSent)
                        {
                            tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTED);
                        }
                        else if (!TptData->sendingResults && !TptData->avResultsSent)
                        {
                            /* Do nothing */
                        }
                        else
                        {
                            tptSendResultData(TptData);
                        }
                    }
                }
                break;
            }
        case CSR_BT_SPP_DATA_IND:
            {
                CsrBtSppDataInd  *prim;
                TptCommand_t    *cmd;

                prim = (CsrBtSppDataInd *) TptData->recvMsgP;

                cmd = deserializeTptCommand(prim->payload);
                CsrPmemFree(prim->payload);

                tptHandleCommand(TptData, cmd);

                CsrBtSppDataResSend(TptData->spp_qid, TptData->spp_sc);
                break;
            }
        default:
            {
                break;
            }
    }
    return;
}

