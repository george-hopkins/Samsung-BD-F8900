/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched_init.h"
#include "csr_types.h"
#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_srv.h"
#include "csr_bt_tpt_cmd_serialize.h"
#include "csr_bt_tpt_handler_av.h"
#include "csr_bt_tpt_handler_cm.h"
#include "csr_bt_tpt_handler_sc.h"
#include "csr_bt_tpt_handler_spp.h"

void TptCloseTimerHandler(CsrUint16 dummy1, void *dummy2)
{
    CSR_UNUSED(dummy1);
    CSR_UNUSED(dummy2);

    CsrSchedStop();
}


void tptCommandSend(CsrBtTptServerInstanceDataType *TptData, TptCommand_t *cmd)
{
    CsrUint8 *buf;
    CsrUint16  bufLen;

    buf = serializeTptCommand(cmd, &bufLen);

    CsrBtSppDataReqSend(TptData->spp_qid, TptData->spp_sc, bufLen, buf);
}


void tptDelayedReportTransition(CsrUint16 unused, void *inst)
{
    CsrBtTptServerInstanceDataType *TptData;

    TptData = (CsrBtTptServerInstanceDataType *) inst;

    tptScheduleRun(TptData, CSR_BT_TPT_STATE_REPORTING);
}


void tptSendResultData(CsrBtTptServerInstanceDataType *TptData)
{
    TptCommand_t    *cmd;
    tptReportResult *dataPtr;
    CsrUint32        dataStampSize = 0;
    CsrUint32        maxData, dataStampsInPacket, n;

    cmd = CsrPmemZalloc(sizeof(*cmd));

    dataStampSize = sizeof(dataPtr->timeStamp[0]) + sizeof(dataPtr->seqno[0]);
    if(TptData->sendingResults == FALSE)
    {
        TptData->remainData = (dataStampSize * (TptData->curTest.packetCount - 1));
        TptData->sendingResults = TRUE;
    }

    if(!TptData->remainData)
    {
        /* We don't have more data to send - send cfm signal */
        cmd->cmdOpcode = TPT_CMD_RESULT_CFM;
        cmd->testType  = TptData->curTest.testType;
        cmd->paramLen  = 0;
        cmd->paramPtr  = NULL;
        tptCommandSend(TptData, cmd);
        TptData->dataReported = 0;
        TptData->sendingResults = FALSE;
        if (TptData->curTest.testType & TPT_TEST_AV)
        {
            TptData->avResultsSent = TRUE;
        }
        else
        {
            TptData->sppResultsSent = TRUE;
        }
        CsrPmemFree(TptData->timestamps);
        CsrPmemFree(TptData->seqnos);
        CsrPmemFree(cmd);
        TptData->timestamps = NULL;
        TptData->seqnos = NULL;
    }
    else
    {
        cmd->cmdOpcode = TPT_CMD_RESULT_IND;
        cmd->testType  = TptData->curTest.testType;

        if (TptData->remainData > TPT_CMD_MAX_PAYLOAD_LEN)
        {
            maxData = TPT_CMD_MAX_PAYLOAD_LEN;
        }
        else
        {
            maxData = TptData->remainData;
        }
        dataStampsInPacket = maxData / dataStampSize;
        dataPtr = (tptReportResult *)CsrPmemAlloc(sizeof(tptReportResult));
        dataPtr->timeStamp = CsrPmemAlloc(dataStampsInPacket * sizeof(dataPtr->timeStamp[0]));
        dataPtr->seqno = CsrPmemAlloc(dataStampsInPacket * sizeof(dataPtr->seqno[0]));
        cmd->paramLen = dataStampsInPacket * dataStampSize;

        for (n = 0; n < dataStampsInPacket; n++)
        {
            dataPtr->timeStamp[n] = TptData->timestamps[TptData->dataReported];
            dataPtr->seqno[n] = 0;
            if (TptData->curTest.testType & TPT_TEST_AV)
            {
                dataPtr->seqno[n] = TptData->seqnos[TptData->dataReported];
            }
            TptData->dataReported++;
        }
        cmd->paramPtr = dataPtr;
        tptCommandSend(TptData, cmd);
        TptData->remainData -= cmd->paramLen;

        if(TptData->remainData < dataStampSize)
        {
            TptData->remainData = 0;
        }
        CsrPmemFree(dataPtr->timeStamp);
        CsrPmemFree(dataPtr->seqno);
        CsrPmemFree(dataPtr);
        CsrPmemFree(cmd);
    }
}

void tptHandleCommand(CsrBtTptServerInstanceDataType *TptData, TptCommand_t *cmd)
{
    switch (cmd->cmdOpcode)
    {
        case TPT_CMD_SETUP_REQ:
            {
                TptData->curTest.testType = cmd->testType;

                if (cmd->paramPtr)
                {
                    if (cmd->testType & TPT_TEST_SPP)
                    {
                        tptSppTestParams *par;

                        par = (tptSppTestParams *) cmd->paramPtr;

                        TptData->curTest.packetCount = par->packetCount;
                        TptData->curTest.packetsDone = 0;
                        TptData->curTest.packetSize = par->packetSize;
                        TptData->curTest.testFlags = par->sppFlags;

                    }
                    else if (cmd->testType & TPT_TEST_AV)
                    {
                        tptAvTestParams *par;

                        par = (tptAvTestParams *) cmd->paramPtr;

                        TptData->curTest.packetCount = par->packetCount;
                        TptData->curTest.packetsDone = 0;
                        TptData->curTest.packetSize = par->packetSize;
                        TptData->curTest.testFlags = par->avFlags;

                    }
                }

                TptData->test_started = FALSE;

                if (TptData->curTest.testType & TPT_TESTDIR_CS)
                {
                    /*
                     * We record dT's between packets, thus packetCount - 1 values.
                     */
                    TptData->timestamps = CsrPmemAlloc(sizeof(TptData->timestamps[0]) * (TptData->curTest.packetCount - 1));
                    TptData->seqnos = CsrPmemAlloc(sizeof(TptData->seqnos[0]) * TptData->curTest.packetCount);
                }

                tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONFIGURING);
                break;
            }
        case TPT_CMD_TESTRUN_REQ:
            {
                TptData->test_started   = TRUE;
                TptData->sppResultsSent = FALSE;
                TptData->avResultsSent  = FALSE;

                if (TptData->curTest.testType & TPT_TESTDIR_CS)
                {
                    /*
                     * Client is going to send to us so
                     * don't do anything.
                     */
                }
                else if (TptData->curTest.testType & TPT_TEST_SPP)
                {
                    CsrUint8 *buf;

                    buf = CsrPmemAlloc(TptData->curTest.packetSize);
                    CsrBtSppDataReqSend(TptData->spp_qid, TptData->spp_sc, TptData->curTest.packetSize, buf);
                }
                break;
            }
        case TPT_CMD_TESTRUN_CFM:
            {
                break;
            }
        case TPT_CMD_RESULT_REQ:
            {
                tptSendResultData(TptData);
                break;
            }
        default:
            {
                break;
            }
    }

    CsrPmemFree(cmd->paramPtr);
    CsrPmemFree(cmd);
}



void tptInitHandleTptPrim(CsrBtTptServerInstanceDataType *TptData)
{
    if (TptData->closingProgram)
    {
        CsrSchedTimerSet(1000000, TptCloseTimerHandler, 0, NULL);
    }
    else
    {
        CsrBtScActivateReqSend(TptData->phandle);
        CsrBtAvSetQosIntervalReq(0);
        CsrBtCmWriteScanEnableReqSend(TptData->phandle, FALSE, FALSE);
        CsrBtSppGetInstancesQidReqSend(TptData->phandle);
    }
}


void tptConnectableHandleTptPrim(CsrBtTptServerInstanceDataType *TptData)
{
    /* Do nothing */
}


void tptConnectedHandleTptPrim(CsrBtTptServerInstanceDataType *TptData)
{
    /* Do nothing */
}


void tptConfiguringHandleTptPrim(CsrBtTptServerInstanceDataType *TptData)
{
    /* Start inquiry if requested */
    if (TptData->curTest.testFlags & TPT_TESTFLAG_INQUIRY)
    {
        CsrBtCmWriteScanEnableReqSend(TptData->phandle, FALSE, FALSE); /* Visible .*/
    }
    /* Start SBC encoding if requested */
    if (TptData->curTest.testFlags & TPT_TESTFLAG_DOSBC)
    {
        TptData->runSbcCoding = TRUE;
        csrBtTptPrepareSbc(TptData);
        csrBtTptAvSbcCoding(0, TptData);
    }
    /* Start setting up AV stuff if needed */
    if (TptData->curTest.testType == CSR_BT_TPT_TEST_AV_CS)
    {
        CsrBtAvActivateReqSend(TptData->phandle, CSR_BT_AV_AUDIO_SINK);
    }
    else if (TptData->curTest.testType == CSR_BT_TPT_TEST_AV_SC)
    {
        CsrBtAvActivateReqSend(TptData->phandle, CSR_BT_AV_AUDIO_SOURCE);
    }
    else
    {
        /* Setting up SPP means do nothing -- go to */
        /* testing right away.                      */
        tptScheduleRun(TptData, CSR_BT_TPT_STATE_TESTING);
    }
}


void tptTestingHandleTptPrim(CsrBtTptServerInstanceDataType *TptData)
{
    TptCommand_t *cmd;

    /*
     * Let client know that we have
     * completed the test setup.
     */

    cmd = CsrPmemZalloc(sizeof(*cmd));
    cmd->cmdOpcode = TPT_CMD_SETUP_CFM;
    cmd->testType = TptData->curTest.testType;
    tptCommandSend(TptData, cmd);
    CsrPmemFree(cmd);
}


void tptReportingHandleTptPrim(CsrBtTptServerInstanceDataType *TptData)
{
    TptCommand_t *cmd;

    /* Stop inquiry - it may be running */
    CsrBtCmWriteScanEnableReqSend(TptData->phandle, TRUE, FALSE); /* Not visible .*/

    /* Stop SBC encoding - it may be running */
    if (TptData->runSbcCoding == TRUE)
    {
        TptData->runSbcCoding = FALSE;
        CsrSchedTimerCancel(TptData->timerId,NULL,NULL);
        CsrSbcClose(&TptData->sbcHdl);
        if(TptData->sbcFramePtr)
        {
            CsrPmemFree(TptData->sbcFramePtr);
            TptData->sbcFramePtr = NULL;
        }
    }

    /* Send TPT_CMD_TESTRUN_CFM */
    cmd = CsrPmemZalloc(sizeof(*cmd));
    cmd->cmdOpcode = TPT_CMD_TESTRUN_CFM;
    cmd->testType = TptData->curTest.testType;
    tptCommandSend(TptData, cmd);
    CsrPmemFree(cmd);
}


void tptInitStateHandler(CsrBtTptServerInstanceDataType *TptData, CsrUint16 eventType)
{
    switch (eventType)
    {
        case TPT_PRIM:
            {
                tptInitHandleTptPrim(TptData);
                break;
            }
        case CSR_BT_CM_PRIM:
            {
                tptCmHandler(TptData);
                break;
            }
        case CSR_BT_SPP_PRIM:
            {
                tptInitHandleSppPrim(TptData);
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                tptScHandler(TptData);
                break;
            }
        default:
            {
                break;
            }
    }
    CsrPmemFree(TptData->recvMsgP);
    TptData->recvMsgP = NULL;
}


void tptConnectableStateHandler(CsrBtTptServerInstanceDataType *TptData, CsrUint16 eventType)
{
    switch (eventType)
    {
        case TPT_PRIM:
            {
                tptConnectableHandleTptPrim(TptData);
                break;
            }
        case CSR_BT_CM_PRIM:
            {
                tptCmHandler(TptData);
                break;
            }
        case CSR_BT_SPP_PRIM:
            {
                tptConnectableHandleSppPrim(TptData);
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                tptScHandler(TptData);
                break;
            }
        default:
            {
                break;
            }
    }
    CsrPmemFree(TptData->recvMsgP);
    TptData->recvMsgP = NULL;
}


void tptConnectedStateHandler(CsrBtTptServerInstanceDataType *TptData, CsrUint16 eventType)
{
    switch (eventType)
    {
        case TPT_PRIM:
            {
                tptConnectedHandleTptPrim(TptData);
                break;
            }
        case CSR_BT_AV_PRIM:
            {
                tptConnectedHandleAvPrim(TptData);
                break;
            }
        case CSR_BT_CM_PRIM:
            {
                tptCmHandler(TptData);
                break;
            }
        case CSR_BT_SPP_PRIM:
            {
                tptConnectedHandleSppPrim(TptData);
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                tptScHandler(TptData);
                break;
            }
        default:
            {
                break;
            }
    }
    CsrPmemFree(TptData->recvMsgP);
    TptData->recvMsgP = NULL;
}


void tptConfiguringStateHandler(CsrBtTptServerInstanceDataType *TptData, CsrUint16 eventType)
{
    switch (eventType)
    {
        case TPT_PRIM:
            {
                tptConfiguringHandleTptPrim(TptData);
                break;
            }
        case CSR_BT_AV_PRIM:
            {
                tptConfiguringHandleAvPrim(TptData);
                break;
            }
        case CSR_BT_CM_PRIM:
            {
                tptCmHandler(TptData);
                break;
            }
        case CSR_BT_SPP_PRIM:
            {
                tptConfiguringHandleSppPrim(TptData);
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                tptScHandler(TptData);
                break;
            }
        default:
            {
                break;
            }
    }
    CsrPmemFree(TptData->recvMsgP);
    TptData->recvMsgP = NULL;
}


void tptTestingStateHandler(CsrBtTptServerInstanceDataType *TptData, CsrUint16 eventType)
{
    switch (eventType)
    {
        case TPT_PRIM:
            {
                tptTestingHandleTptPrim(TptData);
                break;
            }
        case CSR_BT_AV_PRIM:
            {
                tptTestingHandleAvPrim(TptData);
                break;
            }
        case CSR_BT_CM_PRIM:
            {
                tptCmHandler(TptData);
                break;
            }
        case CSR_BT_SPP_PRIM:
            {
                tptTestingHandleSppPrim(TptData);
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                tptScHandler(TptData);
                break;
            }
        default:
            {
                break;
            }
    }
    CsrPmemFree(TptData->recvMsgP);
    TptData->recvMsgP = NULL;
}


void tptReportingStateHandler(CsrBtTptServerInstanceDataType *TptData, CsrUint16 eventType)
{
    switch (eventType)
    {
        case TPT_PRIM:
            {
                tptReportingHandleTptPrim(TptData);
                break;
            }
        case CSR_BT_AV_PRIM:
            {
                tptReportingHandleAvPrim(TptData);
                break;
            }
        case CSR_BT_CM_PRIM:
            {
                tptCmHandler(TptData);
                break;
            }
        case CSR_BT_SPP_PRIM:
            {
                tptReportingHandleSppPrim(TptData);
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                tptScHandler(TptData);
                break;
            }
        default:
            {
                break;
            }
    }
    CsrPmemFree(TptData->recvMsgP);
    TptData->recvMsgP = NULL;
}


/**************************************************************************************************
*
*    This is the demo application handler function. All primitives sent to the demo application will
*    be received in here. The scheduler ensures that the function is activated when a signal is put
*    on the demo application signal queue.
*
**************************************************************************************************/
void CsrBtTptHandler(void **gash)
{
    CsrBtTptServerInstanceDataType *TptData;
    CsrUint16    eventType=0;
    void        *msg=NULL;

    TptData = (CsrBtTptServerInstanceDataType *) (*gash);
    CsrSchedMessageGet(&eventType, &msg);

    TptData->recvMsgP = msg;

    switch (TptData->state)
    {
        case CSR_BT_TPT_STATE_INIT:
            {
                tptInitStateHandler(TptData, eventType);
                break;
            }
        case CSR_BT_TPT_STATE_CONNECTABLE:
            {
                tptConnectableStateHandler(TptData, eventType);
                break;
            }
        case CSR_BT_TPT_STATE_CONNECTED:
            {
                tptConnectedStateHandler(TptData, eventType);
                break;
            }
        case CSR_BT_TPT_STATE_CONFIGURING:
            {
                tptConfiguringStateHandler(TptData, eventType);
                break;
            }
        case CSR_BT_TPT_STATE_TESTING:
            {
                tptTestingStateHandler(TptData, eventType);
                break;
            }
        case CSR_BT_TPT_STATE_REPORTING:
            {
                tptReportingStateHandler(TptData, eventType);
                break;
            }
        default:
            {
                break;
            }
    }
}
