/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>

#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_platform.h"
#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_tasks.h"
#include "csr_bt_tpt_app.h"
#include "csr_bt_tpt_cmd.h"
#include "csr_bt_tpt_cmd_serialize.h"
#include "csr_bt_av_prim.h"
#include "csr_bt_tpt_handler_av.h"
#include "csr_bt_tpt_handler_cm.h"
#include "csr_bt_tpt_handler_sc.h"
#include "csr_bt_tpt_handler_spp.h"
#include "csr_bt_demoapp.h"

static CsrSchedTid  timerId;
static char *configFile;


static void writeCurrentResults(FILE        *fh,
                                CsrFileHandle        *fhDelay,
                                void        *timeData,
                                void        *seqData,
                                CsrUint32    resultsNum,
                                CsrUint32    packetNum,
                                CsrUint32    packetSize)
{
    /* Throughput data will written to the file represented by fh in the */
    /* following order:                                                  */
    /*                                                                   */
    /*     Number of packets in test,                                    */
    /*     Packet size in bytes                                          */
    /*     Total send time in micro-seconds,                             */
    /*     Throughput in bytes/second                                    */
    /*     Total number of errors                                        */
    /*                                                                   */
    /* Latency data (applicable for AV tests only) will written to the   */
    /* file represented by fh in the following way:                      */
    /*                                                                   */
    /*     Number of packets in test,                                    */
    /*     Packet size in bytes                                          */
    /*     Delta time between first and second packet,                   */
    /*     Boolean, which is 1 if stream holes occured between first and */
    /*              second packet and 0 otherwise                        */
    /*     Delta time between second and third packet,                   */
    /*     Boolean, which is 1 if stream holes occured between second and*/
    /*              third packet and 0 otherwise                         */
    /*     Delta time between third and fourth packet,                   */
    /*     Boolean, which is 1 if stream holes occured between third and */
    /*              fourth packet and 0 otherwise                        */
    /*     ...                                                           */
    /*     ...                                                           */
    /*     etc                                                           */
    /*                                                                   */

    CsrTime      totalTime = 0;
    CsrTime      *timeDataPtr;
    CsrUint32    *seqDataPtr;
    CsrUint32    throughput = 0;
    CsrUint32    totalErrors = 0;
    CsrUint32    n = 0;
    char         buffer[CSR_BT_TPT_MAX_LINE_LENGTH];

    timeDataPtr = (CsrTime *) timeData;
    seqDataPtr  = (CsrUint32 *) seqData;

    for (n = 0; n < resultsNum; n++)
    {
        totalTime += timeDataPtr[n];
    }

    totalErrors = 0;
    if (seqDataPtr)
    {
        for (n = 1; n < resultsNum; n++)
        {
            if(seqDataPtr[n] != (n + seqDataPtr[0]))
            {
                totalErrors += (seqDataPtr[n] - (n + seqDataPtr[0]));
                n = seqDataPtr[n];
            }
        }
    }

    if (totalTime / 1000000) /* if total test time is more than one second */
    {
        throughput = (packetNum * packetSize) / (totalTime / 1000);
    }
    else
    {
        throughput = 0xffffffff; /* Indicate that test time is too low to get a valid result */
    }

    sprintf(buffer, "%u,%u,%lu,%lu,%lu\n",
                    packetNum,
                    packetSize,
                    totalTime,
                    throughput,
                    totalErrors);
    CsrBtFwrite(buffer, 1, CsrStrLen(buffer), fh);


    if (fhDelay && seqData)
    {
        CsrUint32 delayResultsNum;
        CsrUint32 i;

        delayResultsNum = resultsNum - totalErrors;

        sprintf(buffer, "%u,%u", packetNum, packetSize);
        CsrBtFwrite(buffer, 1, CsrStrLen(buffer), fhDelay);
        for (i = 0; i < delayResultsNum; i++)
        {
            sprintf(buffer, ",%lu", timeDataPtr[i]);
            CsrBtFwrite(buffer, 1, CsrStrLen(buffer), fhDelay);
        }
        CsrBtFwrite("\n", 1, CsrStrLen("\n"), fhDelay);
    }

}

static CsrUint32 readCurrentResults(FILE *fh, void *timeData, void *seqData, CsrUint32 dataSize)
{
    CsrTime      *td;
    CsrUint32    *ed;
    CsrUint32    n = 0;
    
    td = (CsrTime *) timeData;
    ed = (CsrUint32 *) seqData;

    CsrBtFseek(fh,0L,SEEK_SET);
    if (ed)
    {
        n = 0;
        while ((n < dataSize) && 
               (CsrBtFread(&td[n],sizeof(CsrTime),1,fh) == sizeof(CsrTime)) && 
               (CsrBtFread(&ed[n],sizeof(CsrUint32),1,fh) == sizeof(CsrUint32)) )
        {
            n++;
        }
    }
    else
    {
        n = 0;
        while((n < dataSize) && 
              (CsrBtFread(&td[n],sizeof(CsrTime),1,fh) == sizeof(CsrTime)) )
        {
            n++;
        }
    }

    return n;
}

static CsrBool makeFinalReports(CsrBtTptClientInstanceDataType *TptData)
{
    CsrFileHandle        *finalReportFh, *finalDelayReportFh, *currentReportFh;
    CsrBool      verdict = TRUE;
    CsrUint32    n;
    CsrBool      writeSppCsHeader = TRUE;
    CsrBool      writeSppScHeader = TRUE;
    CsrBool      writeAvCsHeader = TRUE;
    CsrBool      writeAvScHeader = TRUE;

    for (n = 0; n < TptData->numTest; n++)
    {
        switch (TptData->tests[n].testType)
        {
            case CSR_BT_TPT_TEST_SPP_CS:
                {
                    if (writeSppCsHeader)
                    {
                        finalReportFh = CsrBtFopen(TptData->sppCsResultFileName, "w");
                        if (!finalReportFh)
                        {
                            verdict = FALSE;
                        }
                        else
                        {
                            CsrBtFwrite("No of packets,packet size [bytes],total send time [micro-seconds],throughput [kbytes/second],total errors\n",
                                        1,
                                        CsrStrLen("No of packets,packet size [bytes],total send time [micro-seconds],throughput [kbytes/second],total errors\n"),
                                        finalReportFh);
                            writeSppCsHeader = FALSE;
                        }
                    }
                    else
                    {
                        finalReportFh = CsrBtFopen(TptData->sppCsResultFileName, "a");
                    }
                    break;
                }
            case CSR_BT_TPT_TEST_SPP_SC:
                {
                    if (writeSppScHeader)
                    {
                        finalReportFh = CsrBtFopen(TptData->sppScResultFileName, "w");
                        if (!finalReportFh)
                        {
                            verdict = FALSE;
                        }
                        else
                        {
                            CsrBtFwrite("No of packets,packet size [bytes],total send time [micro-seconds],throughput [kbytes/second],total errors\n",
                                        1,
                                        CsrStrLen("No of packets,packet size [bytes],total send time [micro-seconds],throughput [kbytes/second],total errors\n"),
                                        finalReportFh);
                            writeSppScHeader = FALSE;
                        }
                    }
                    else
                    {
                        finalReportFh = CsrBtFopen(TptData->sppScResultFileName, "a");
                    }
                    break;
                }
            case CSR_BT_TPT_TEST_AV_CS:
                {
                    if (writeAvCsHeader)
                    {
                        CsrUint32 idx;

                        finalReportFh = CsrBtFopen(TptData->avCsResultFileName, "w");
                        finalDelayReportFh = CsrBtFopen(TptData->avCsDelayResultFileName, "w");
                        if (!finalDelayReportFh || !finalReportFh)
                        {
                            verdict = FALSE;
                        }
                        else
                        {
                            char    buffer[CSR_BT_TPT_MAX_LINE_LENGTH];

                            CsrBtFwrite("No of packets,packet size [bytes],total send time [micro-seconds],throughput [kbytes/second],total errors\n",
                                        1,
                                        CsrStrLen("No of packets,packet size [bytes],total send time [micro-seconds],throughput [kbytes/second],total errors\n"),
                                        finalReportFh);
                            CsrBtFwrite("No of packets,packet size",
                                        1,
                                        CsrStrLen("No of packets,packet size"),
                                        finalDelayReportFh);
                            for (idx = 0; (idx + 1) < TptData->tests[n].packetCount; idx++)
                            {
                                sprintf(buffer, ",dt(%d-%d)", idx, idx + 1);
                                CsrBtFwrite(buffer, 1, CsrStrLen(buffer), finalDelayReportFh);
                            }
                            CsrBtFwrite("\n", 1, CsrStrLen("\n"), finalDelayReportFh);
                            writeAvCsHeader = FALSE;
                        }
                    }
                    else
                    {
                        finalReportFh = CsrBtFopen(TptData->avCsResultFileName, "a");
                        finalDelayReportFh = CsrBtFopen(TptData->avCsDelayResultFileName, "a");
                        if (!finalDelayReportFh || !finalReportFh)
                        {
                            verdict = FALSE;
                        }
                    }
                    break;
                }
            case CSR_BT_TPT_TEST_AV_SC:
                {
                    if (writeAvScHeader)
                    {
                        CsrUint32 idx;

                        finalReportFh = CsrBtFopen(TptData->avScResultFileName, "w");
                        finalDelayReportFh = CsrBtFopen(TptData->avScDelayResultFileName, "w");
                        if (!finalDelayReportFh || !finalReportFh)
                        {
                            verdict = FALSE;
                        }
                        else
                        {
                            char    buffer[CSR_BT_TPT_MAX_LINE_LENGTH];

                            CsrBtFwrite("No of packets,packet size [bytes],total send time [micro-seconds],throughput [kbytes/second],total errors\n",
                                        1,
                                        CsrStrLen("No of packets,packet size [bytes],total send time [micro-seconds],throughput [kbytes/second],total errors\n"),
                                        finalReportFh);
                            CsrBtFwrite("No of packets,packet size",
                                        1,
                                        CsrStrLen("No of packets,packet size"),
                                        finalDelayReportFh);
                            for (idx = 0; (idx + 1) < TptData->tests[n].packetCount; idx++)
                            {
                                sprintf(buffer, ",dt(%d-%d)", idx, idx + 1);
                                CsrBtFwrite(buffer, 1, CsrStrLen(buffer), finalDelayReportFh);
                            }
                            CsrBtFwrite("\n", 1, CsrStrLen("\n"), finalDelayReportFh);
                            writeAvScHeader = FALSE;
                        }
                    }
                    else
                    {
                        finalReportFh = CsrBtFopen(TptData->avScResultFileName, "a");
                        finalDelayReportFh = CsrBtFopen(TptData->avScDelayResultFileName, "a");
                        if (!finalDelayReportFh || !finalReportFh)
                        {
                            verdict = FALSE;
                        }
                    }
                    break;
                }
            default:
                {
                    csrBtTptClientPrint("ERROR: Unexpected test type when writing final report\n");
                    verdict = FALSE;
                    break;
                }
        }
        currentReportFh = CsrBtFopen(TptData->tests[n].logName, "r");

        if (!verdict || !finalReportFh || !currentReportFh ||
           (!finalDelayReportFh && (TptData->tests[n].testType & TPT_TEST_AV)))
        {
            csrBtTptClientPrint("ERROR: Could not open all necessary report files\n");
            verdict = FALSE;
        }
        else
        {
            CsrTime        *currentTimeData;
            CsrUint32    *currentSeqData;
            CsrUint32    numOfResults = 0;

            currentTimeData = CsrPmemAlloc(TptData->tests[n].packetCount * sizeof(CsrTime));
            currentSeqData  = CsrPmemAlloc(TptData->tests[n].packetCount * sizeof(CsrUint32));

            if (TptData->tests[n].testType & TPT_TEST_SPP)
            {
                numOfResults = readCurrentResults(currentReportFh, currentTimeData, NULL, TptData->tests[n].packetCount);
                writeCurrentResults(finalReportFh,
                                    NULL,
                                    currentTimeData,
                                    NULL,
                                    numOfResults,
                                    TptData->tests[n].packetCount,
                                    TptData->tests[n].packetSize);
            }
            else
            {
                numOfResults = readCurrentResults(currentReportFh, currentTimeData, currentSeqData, TptData->tests[n].packetCount);
                writeCurrentResults(finalReportFh,
                                    finalDelayReportFh,
                                    currentTimeData,
                                    currentSeqData,
                                    numOfResults,
                                    TptData->tests[n].packetCount,
                                    TptData->tests[n].packetSize);
            }
            CsrPmemFree(currentTimeData);
            CsrPmemFree(currentSeqData);

            CsrBtFclose(currentReportFh);
            CsrBtFclose(finalReportFh);
        }

    } /* End of for loop */

    return verdict;
}

static CsrBool tptWriteResultToFile(CsrBtTptClientInstanceDataType *TptData, CsrUint32 dataLen, void *dataPtr)
{
    CsrFileHandle            *fh = NULL;
    tptReportResult *data;
    CsrUint32        n = 0, numOfResults = 0;

    data = (tptReportResult *) dataPtr;
    numOfResults = data->resultCount;

    fh = CsrBtFopen(TptData->tests[TptData->curTest].logName, "a");
    if (!fh)
    {
        return FALSE;
    }

    if (TptData->tests[TptData->curTest].testType & TPT_TEST_AV)
    {
        char    buffer[CSR_BT_TPT_MAX_LINE_LENGTH];

        for (n = 0; n < numOfResults; n++)
        {
            sprintf(buffer, "%lu,%lu,", *(data[0].timeStamp + n), *(data[0].seqno + n));
            CsrBtFwrite(buffer, 1, CsrStrLen(buffer), fh);
        }
    }
    else
    {
        char    buffer[CSR_BT_TPT_MAX_LINE_LENGTH];

        for (n = 0; n < numOfResults; n++)
        {
            sprintf(buffer, "%lu,", *(data[0].timeStamp + n));
            CsrBtFwrite(buffer, 1, CsrStrLen(buffer), fh);
        }
    }
    CsrBtFclose(fh);

    return TRUE;
}

void tptCommandSend(CsrBtTptClientInstanceDataType *TptData, TptCommand_t *cmd)
{
    CsrUint8 *buf;
    CsrUint16  bufLen;

    csrBtTptDebugPrint("Sending command: %u.\n", cmd->cmdOpcode);

    buf = serializeTptCommand(cmd, &bufLen);

    CsrBtSppDataReqSend(TptData->spp_qid, TptData->spp_sc, bufLen, buf);
}

void tptHandleCommand(CsrBtTptClientInstanceDataType *TptData, TptCommand_t *cmd)
{
    csrBtTptDebugPrint("Handling command %u.\n", cmd->cmdOpcode);

    switch (cmd->cmdOpcode)
    {
        case TPT_CMD_SETUP_CFM:
            {
                csrBtTptDebugPrint("TPT_CMD_SETUP_CFM\n");
                if (TptData->tests[TptData->curTest].testType == CSR_BT_TPT_TEST_SPP_SC)
                {
                    csrBtTptDebugPrint("Opening SPP result file\n");
                    TptData->sppScResultFilePtr = CsrBtFopen(TptData->tests[TptData->curTest].logName, "w");
                    if (!TptData->sppScResultFilePtr)
                    {
                        csrBtTptDebugPrint("\nERROR: Could not open result file (%s). No results will be logged\n\n",
                                        TptData->tests[TptData->curTest].logName);
                    }
                }
                else if (TptData->tests[TptData->curTest].testType == CSR_BT_TPT_TEST_AV_SC)
                {
                    csrBtTptDebugPrint("Opening AV result file\n");
                    TptData->avScResultFilePtr = CsrBtFopen(TptData->tests[TptData->curTest].logName, "w");
                    if (!TptData->avScResultFilePtr)
                    {
                        csrBtTptDebugPrint("\nERROR: Could not open result file (%s). No results will be logged\n\n",
                                        TptData->tests[TptData->curTest].logName);
                    }
                }

                TptData->test_configured = TRUE;
                /*
                 * If AV is not connected yet, the CSR_BT_AV_OPEN_CFM handler
                 * will take care of this.
                 */
                if ((TptData->tests[TptData->curTest].testType & TPT_TEST_AV &&
                    TptData->av_open == TRUE) ||
                    TptData->tests[TptData->curTest].testType & TPT_TEST_SPP)
                {
                    TptCommand_t *startcmd;

                    csrBtTptDebugPrint("Starting test.\n");
                    TptData->testRunCfmReceived = FALSE;
                    startcmd = CsrPmemZalloc(sizeof(*startcmd));
                    startcmd->cmdOpcode = TPT_CMD_TESTRUN_REQ;
                    startcmd->testType = TptData->tests[TptData->curTest].testType;

                    tptCommandSend(TptData, startcmd);
                    CsrPmemFree(startcmd);
                }
                break;
            }
        case TPT_CMD_TESTRUN_CFM:
            {
                csrBtTptDebugPrint("TPT_CMD_TESTRUN_CFM\n");

                TptData->testRunCfmReceived = TRUE;
                if (TptData->tests[TptData->curTest].testType & TPT_TEST_AV)
                {
                    if (TptData->tests[TptData->curTest].packetsDone == TptData->tests[TptData->curTest].packetCount)
                    {
                        CsrBtAvCloseReqSend(TptData->av_shandle, TptData->av_tlabel);
                    }
                }
                else
                {
                    if (TptData->tests[TptData->curTest].testType == CSR_BT_TPT_TEST_SPP_SC && TptData->sppScResultFilePtr)
                    {
                        csrBtTptDebugPrint("closing SPP result file\n");
                        CsrBtFclose(TptData->sppScResultFilePtr);
                    }
                    tptScheduleRun(TptData, CSR_BT_TPT_STATE_REPORTING);
                }
                break;
            }
        case TPT_CMD_RESULT_IND:
            {
                tptReportResult *res;
                CsrBool result;

                csrBtTptDebugPrint("TPT_CMD_RESULT_IND\n");

                result = tptWriteResultToFile(TptData, cmd->paramLen, cmd->paramPtr);
                if (!result)
                {
                    csrBtTptClientPrint("Error writing test results to file\n");
                }

                /*
                 * Result indications contain pointers to external
                 * data in the paramPtr data that must be deallocated.
                 */

                res = (tptReportResult *) cmd->paramPtr;
                CsrPmemFree(res->timeStamp);
                CsrPmemFree(res->seqno);

                break;
            }
        case TPT_CMD_RESULT_CFM:
            {
                csrBtTptDebugPrint("TPT_CMD_RESULT_CFM\n");
                TptData->curTest = TptData->curTest + 1;
                tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTED);
                break;
            }
        default:
            {
                csrBtTptClientPrint("Error:  Unknown TPT command %u\n", cmd->cmdOpcode);
                break;
            }
    }

    /*
     * Deallocate command and parameters.
     */
    CsrPmemFree(cmd->paramPtr);
    CsrPmemFree(cmd);
}

void tptInitHandleTptPrim(CsrBtTptClientInstanceDataType *TptData)
{
    /*
     * Begin setting up things.
     */

    CsrBtScActivateReqSend(TptData->phandle);
    CsrBtAvSetQosIntervalReq(0);

    CsrBtCmWriteScanEnableReqSend(TptData->phandle, TRUE, FALSE); /* Set device "Not visible" again. */
    CsrBtSppGetInstancesQidReqSend(TptData->phandle);
    CsrBtScBondReqSend(TptData->phandle, TptData->tptPeerAddr);
}


void tptConnectingHandleTptPrim(CsrBtTptClientInstanceDataType *TptData)
{

    csrBtTptClientPrint("Connecting SPP...\n");
    CsrBtSppConnectReqSend(TptData->spp_qid, TptData->phandle, TptData->tptPeerAddr,
        FALSE, NULL, CSR_BT_SPP_CLIENT_ROLE);
}


void tptConnectedHandleTptPrim(CsrBtTptClientInstanceDataType *TptData)
{
    TptData->test_configured = FALSE;
    TptData->av_open = FALSE;

    /*
     * Getting a TPT primitive in this state means
     * run next test case or disconnect.
     */

    if (TptData->curTest < TptData->numTest)
    {
        TptCommand_t  *cmd = CsrPmemAlloc(sizeof(*cmd));

        csrBtTptDebugPrint("Configuring next test.\n");

        cmd->cmdOpcode = TPT_CMD_SETUP_REQ;
        cmd->testType = TptData->tests[TptData->curTest].testType;
        if (cmd->testType & TPT_TEST_SPP)
        {
            tptSppTestParams *par;

            cmd->paramLen = sizeof(*par);
            par = CsrPmemAlloc(sizeof(*par));
            cmd->paramPtr = par;

            par->packetCount    = TptData->tests[TptData->curTest].packetCount;
            par->packetSize     = TptData->tests[TptData->curTest].packetSize;
            par->sppFlags       = TptData->tests[TptData->curTest].testFlags;
        }
        else if (cmd->testType & TPT_TEST_AV)
        {
            tptAvTestParams *par;

            cmd->paramLen = sizeof(*par);
            par = CsrPmemAlloc(sizeof(*par));
            cmd->paramPtr = par;

            par->packetCount    = TptData->tests[TptData->curTest].packetCount;
            par->packetSize     = TptData->tests[TptData->curTest].packetSize;
            par->avFlags        = TptData->tests[TptData->curTest].testFlags;
        }
            csrBtTptClientPrint("\nCurrent test: %s sends %s data to %s\n",
                TptData->tests[TptData->curTest].testType & TPT_TESTDIR_CS ? "PC" : "Target platform",
                TptData->tests[TptData->curTest].testType & TPT_TEST_SPP ? "SPP" : "AV",
                TptData->tests[TptData->curTest].testType & TPT_TESTDIR_CS ? "Target platform" : "PC");
            csrBtTptClientPrint("Sending %u packets of %u bytes...\n",
                TptData->tests[TptData->curTest].packetCount,
                TptData->tests[TptData->curTest].packetSize);
            csrBtTptDebugPrint("Test flags are: %u\n", TptData->tests[TptData->curTest].testFlags);

        tptCommandSend(TptData, cmd);
        CsrPmemFree(cmd->paramPtr);
        CsrPmemFree(cmd);

    }
    else
    {
        CsrBool result = FALSE;

        csrBtTptClientPrint("\nAll tests completed.\n\n");
        CsrBtSppDisconnectReqSend(TptData->spp_qid, TptData->spp_sc);
        csrBtTptClientPrint("Doing final reporting...\n");
        result = makeFinalReports(TptData);
        if (!result)
        {
            csrBtTptClientPrint("*** ERROR: Final report writing did not succeed completely! ***\n\n");
        }
        else
        {
            csrBtTptClientPrint("Final report writing successful\n\n");
        }
    }
}


void tptConfiguringHandleTptPrim(CsrBtTptClientInstanceDataType *TptData)
{
    /* Command data received by server.
     *
     * Start setting up AV stuff if needed.
     */

    csrBtTptDebugPrint("Now configuring.\n");


    if (TptData->tests[TptData->curTest].testType == CSR_BT_TPT_TEST_AV_CS)
    {
        CsrBtAvActivateReqSend(TptData->phandle, CSR_BT_AV_AUDIO_SOURCE);
    }
    else if (TptData->tests[TptData->curTest].testType == CSR_BT_TPT_TEST_AV_SC)
    {
        CsrBtAvActivateReqSend(TptData->phandle, CSR_BT_AV_AUDIO_SINK);
    }
}


void tptTestingHandleTptPrim(CsrBtTptClientInstanceDataType *TptData)
{
    csrBtTptDebugPrint("Now testing.\n");

    if (TptData->tests[TptData->curTest].testType & TPT_TEST_AV)
    {
        CsrUint8 *slist = CsrPmemAlloc(sizeof(CsrUint8));

        /*
         * Allocate stream list for a single
         * stream handle element.
         */

        csrBtTptDebugPrint("Starting AV stream.\n");
        *slist = TptData->av_shandle;

        CsrBtAvStartReqSend(1, TptData->av_tlabel, slist);
    }
    else if (TptData->tests[TptData->curTest].testType == CSR_BT_TPT_TEST_SPP_CS)
    {
        CsrUint8 *buf;

        buf = CsrPmemAlloc(TptData->tests[TptData->curTest].packetSize);
        CsrBtSppDataReqSend(TptData->spp_qid, TptData->spp_sc, TptData->tests[TptData->curTest].packetSize, buf);

        TptData->tests[TptData->curTest].packetsDone = TptData->tests[TptData->curTest].packetsDone + 1;
    }

    if (TptData->tests[TptData->curTest].testType & TPT_TESTDIR_SC)
    {
        csrBtTptDebugPrint("Waiting for server.\n");
    }

}


void tptReportingHandleTptPrim(CsrBtTptClientInstanceDataType *TptData)
{
    if (TptData->tests[TptData->curTest].testType & TPT_TESTDIR_CS)
    {
        TptCommand_t  *cmd = CsrPmemAlloc(sizeof(*cmd));
        CsrFileHandle          *fh = NULL;

        /* Clear content of result file if it exists */
        fh = CsrBtFopen(TptData->tests[TptData->curTest].logName, "w");
        if (!fh)
        {
            csrBtTptDebugPrint("\nError clearing result file. Results may not be valid!\n\n");
        }
        else
        {
            CsrBtFclose(fh);
        }

        cmd->cmdOpcode = TPT_CMD_RESULT_REQ;
        cmd->testType  = TptData->tests[TptData->curTest].testType;
        cmd->paramLen  = 0;
        cmd->paramPtr  = NULL;
        tptCommandSend(TptData, cmd);
        CsrPmemFree(cmd);
    }
    else
    {
        TptData->curTest++;
        tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTED);
    }
}


void tptInitStateHandler(CsrBtTptClientInstanceDataType *TptData, CsrUint16 eventType)
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
                csrBtTptDebugPrint("Unexpected event (0x%04x) in CSR_BT_TPT_STATE_INIT\n", eventType);
                break;
            }
    }
    CsrPmemFree(TptData->recvMsgP);
    TptData->recvMsgP = NULL;
}


void tptConnectingStateHandler(CsrBtTptClientInstanceDataType *TptData, CsrUint16 eventType)
{
    switch (eventType)
    {
        case TPT_PRIM:
            {
                tptConnectingHandleTptPrim(TptData);
                break;
            }
        case CSR_BT_CM_PRIM:
            {
                tptCmHandler(TptData);
                break;
            }
        case CSR_BT_SPP_PRIM:
            {
                tptConnectingHandleSppPrim(TptData);
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                tptScHandler(TptData);
                break;
            }
        default:
            {
                csrBtTptDebugPrint("Unexpected event (0x%04x) in CSR_BT_TPT_STATE_CONNECTING\n", eventType);
                break;
            }
    }
    CsrPmemFree(TptData->recvMsgP);
    TptData->recvMsgP = NULL;
}


void tptConnectedStateHandler(CsrBtTptClientInstanceDataType *TptData, CsrUint16 eventType)
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
                csrBtTptDebugPrint("Unexpected event (0x%04x) in CSR_BT_TPT_STATE_CONNECTED\n", eventType);
                break;
            }
    }
    CsrPmemFree(TptData->recvMsgP);
    TptData->recvMsgP = NULL;
}


void tptConfiguringStateHandler(CsrBtTptClientInstanceDataType *TptData, CsrUint16 eventType)
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
                csrBtTptDebugPrint("Unexpected event (0x%04x) in CSR_BT_TPT_STATE_CONFIGURING\n", eventType);
                break;
            }
    }
    CsrPmemFree(TptData->recvMsgP);
    TptData->recvMsgP = NULL;
}


void tptTestingStateHandler(CsrBtTptClientInstanceDataType *TptData, CsrUint16 eventType)
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
                csrBtTptDebugPrint("Unexpected event (0x%04x) in TPT_RUNNING_STATE\n", eventType);
                break;
            }
    }
    CsrPmemFree(TptData->recvMsgP);
    TptData->recvMsgP = NULL;
}


void tptReportingStateHandler(CsrBtTptClientInstanceDataType *TptData, CsrUint16 eventType)
{
    switch (eventType)
    {
        case TPT_PRIM:
            {
                tptReportingHandleTptPrim(TptData);
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
                csrBtTptDebugPrint("Unexpected event (0x%04x) in CSR_BT_TPT_STATE_REPORTING\n", eventType);
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
void CsrBtAppHandler(void **gash)
{
    CsrBtTptClientInstanceDataType *TptData;
    CsrUint16    eventType=0;
    void        *msg=NULL;

    TptData = (CsrBtTptClientInstanceDataType *) (*gash);
    CsrSchedMessageGet(&eventType, &msg);

    TptData->recvMsgP = msg;

    switch (TptData->state)
    {
        case CSR_BT_TPT_STATE_INIT:
            {
                tptInitStateHandler(TptData, eventType);
                break;
            }
        case CSR_BT_TPT_STATE_CONNECTING:
            {
                tptConnectingStateHandler(TptData, eventType);
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
                csrBtTptClientPrint("Error:  TPT application went into an unexpected state (0x%02x)\n",TptData->state);
                break;
            }
    }
}
