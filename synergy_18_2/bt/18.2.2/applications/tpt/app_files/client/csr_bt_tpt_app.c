/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_platform.h"
#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_tasks.h"
#include "csr_bt_tpt_app.h"
#include "csr_bt_tpt_cmd.h"
#include "csr_bt_tpt_handler.h"
#include "csr_sched_init.h"
#include "csr_bt_demoapp.h"

static CsrSchedTid timerId;

static char *configFile = NULL;

CsrTime csrBtTptHpGetTime()
{
    CsrTime timeTemp;

    timeTemp = (CsrTime) (1000 * timeGetTime());
    return (timeTemp);
}


char* intToAscii(CsrUint32 integer)
{
    char  *string;
    CsrUint16 n = 0;

    string = CsrPmemZalloc(11); /* Max integer value is 4294967295. Thus CSRMAX 10 digits are needed */
    sprintf(string, "%lu", integer);

    return string;
}


void csrBtTptUpdatePrint(CsrBtTptClientInstanceDataType *TptData)
{
    char printChar;

    switch(TptData->updatePrintChar)
    {
        case '/':
            {
                printChar = '-';
                break;
            }
        case '-':
            {
                printChar = '\x5c';
                break;
            }
        case '\x5c':
            {
                printChar = '\x7c';
                break;
            }
        case '\x7c':
            {
                printChar = '/';
                break;
            }
        default:
            {
                printChar = '/';
                break;
            }
    }
    TptData->updatePrintChar = printChar;
    csrBtTptClientPrint("\b%c", printChar);
}


static void generateTestList(CsrBtTptClientInstanceDataType *TptData, CsrBtTptConfigValuesType configValues)
{
    tptTest     *testList;
    tptTest     tmpTest;
    CsrUint8     numOfTestTypes;
    CsrUint8     numOfTestPerType;
    CsrUint16    n = 0;
    CsrUint16    curTest = 0;

    /* Calcultate total number of tests */
    /* E.g. CSRMIN 100, CSRMAX 300, step 100: test 100, 200, 300 */
    /* (300 - 100) / 100 = 2 -- thus the ``+ 1'' */
    if (configValues.packetIncrement == 0)
    {
        numOfTestPerType = 1;
    }
    else
    {
        numOfTestPerType = (((configValues.maxPacketSize - configValues.minPacketSize) / configValues.packetIncrement) + 1);
        if ((configValues.maxPacketSize - configValues.minPacketSize) % configValues.packetIncrement)
        {
            numOfTestPerType++;
        }
    }
    numOfTestTypes = configValues.runSppSC + configValues.runSppCS + configValues.runAvSC + configValues.runAvCS;
    TptData->numTest = numOfTestTypes * numOfTestPerType;
    csrBtTptClientPrint("Total number of test to run: %d\n\n", TptData->numTest);

    testList = (tptTest*) CsrPmemAlloc(TptData->numTest * sizeof(tptTest));

    if(configValues.runSppSC)
    {
        char *fileExtStart;
        char tmpLogName[CSR_BT_TPT_MAX_LINE_LENGTH + 1];
        char *logNamePtr;
        char *tmpAsciiInt;

        for (n = 0; n < numOfTestPerType; n++)
        {
            tmpTest.testType = CSR_BT_TPT_TEST_SPP_SC;
            tmpTest.packetSize = configValues.minPacketSize + (n * configValues.packetIncrement);
            if (tmpTest.packetSize > configValues.maxPacketSize)
            {
                tmpTest.packetSize = configValues.maxPacketSize;
            }
            tmpTest.packetCount = configValues.maxNoToSend;
            tmpTest.packetsDone = 0;
            tmpTest.testFlags = TPT_TESTFLAG_NONE;
            if (configValues.runInquiry)
            {
                tmpTest.testFlags |= TPT_TESTFLAG_INQUIRY;
            }
            if (configValues.runSbcEncode)
            {
                tmpTest.testFlags |= TPT_TESTFLAG_DOSBC;
            }
            /* Remove file extention */
            strcpy(tmpLogName, TptData->sppScResultFileName);
            fileExtStart = strchr(tmpLogName, '\0');
            *(fileExtStart - 4) = '\0';

            strcat(tmpLogName, "_");

            tmpAsciiInt = intToAscii(tmpTest.packetSize);
            strcat(tmpLogName, tmpAsciiInt);
            CsrPmemFree(tmpAsciiInt);

            strcat(tmpLogName, "_");

            tmpAsciiInt = intToAscii(tmpTest.packetCount);
            strcat(tmpLogName, tmpAsciiInt);
            CsrPmemFree(tmpAsciiInt);

            strcat(tmpLogName, ".csv\0");

            logNamePtr = CsrStrDup(tmpLogName);
            tmpTest.logName = logNamePtr;
            testList[curTest] = tmpTest;
            curTest++;
        }
    }
    if(configValues.runSppCS)
    {
        char *fileExtStart;
        char tmpLogName[CSR_BT_TPT_MAX_LINE_LENGTH + 1];
        char *logNamePtr;
        char *tmpAsciiInt;

        for (n = 0; n < numOfTestPerType; n++)
        {
            tmpTest.testType = CSR_BT_TPT_TEST_SPP_CS;
            tmpTest.packetSize = configValues.minPacketSize + (n * configValues.packetIncrement);
            if (tmpTest.packetSize > configValues.maxPacketSize)
            {
                tmpTest.packetSize = configValues.maxPacketSize;
            }
            tmpTest.packetCount = configValues.maxNoToSend;
            tmpTest.packetsDone = 0;
            tmpTest.testFlags = TPT_TESTFLAG_NONE;
            if (configValues.runInquiry)
            {
                tmpTest.testFlags |= TPT_TESTFLAG_INQUIRY;
            }
            if (configValues.runSbcEncode)
            {
                tmpTest.testFlags |= TPT_TESTFLAG_DOSBC;
            }
            /* Remove file extention */
            strcpy(tmpLogName, TptData->sppCsResultFileName);
            fileExtStart = strchr(tmpLogName, '\0');
            *(fileExtStart - 4) = '\0';

            strcat(tmpLogName, "_");

            tmpAsciiInt = intToAscii(tmpTest.packetSize);
            strcat(tmpLogName, tmpAsciiInt);
            CsrPmemFree(tmpAsciiInt);

            strcat(tmpLogName, "_");

            tmpAsciiInt = intToAscii(tmpTest.packetCount);
            strcat(tmpLogName, tmpAsciiInt);
            CsrPmemFree(tmpAsciiInt);

            strcat(tmpLogName, ".csv\0");

            logNamePtr = CsrStrDup(tmpLogName);
            tmpTest.logName = logNamePtr;
            testList[curTest] = tmpTest;
            curTest++;
        }
    }
    if(configValues.runAvSC)
    {
        char *fileExtStart;
        char tmpLogName[CSR_BT_TPT_MAX_LINE_LENGTH + 1];
        char *logNamePtr;
        char *tmpAsciiInt;

        for (n = 0; n < numOfTestPerType; n++)
        {
            tmpTest.testType = CSR_BT_TPT_TEST_AV_SC;
            tmpTest.packetSize = configValues.minPacketSize + (n * configValues.packetIncrement);
            if (tmpTest.packetSize > configValues.maxPacketSize)
            {
                tmpTest.packetSize = configValues.maxPacketSize;
            }
            tmpTest.packetCount = configValues.maxNoToSend;
            tmpTest.packetsDone = 0;
            tmpTest.testFlags = TPT_TESTFLAG_NONE;
            if (configValues.runInquiry)
            {
                tmpTest.testFlags |= TPT_TESTFLAG_INQUIRY;
            }
            if (configValues.runSbcEncode)
            {
                tmpTest.testFlags |= TPT_TESTFLAG_DOSBC;
            }
            /* Remove file extention */
            strcpy(tmpLogName, TptData->avScResultFileName);
            fileExtStart = strchr(tmpLogName, '\0');
            *(fileExtStart - 4) = '\0';

            strcat(tmpLogName, "_");

            tmpAsciiInt = intToAscii(tmpTest.packetSize);
            strcat(tmpLogName, tmpAsciiInt);
            CsrPmemFree(tmpAsciiInt);

            strcat(tmpLogName, "_");

            tmpAsciiInt = intToAscii(tmpTest.packetCount);
            strcat(tmpLogName, tmpAsciiInt);
            CsrPmemFree(tmpAsciiInt);

            strcat(tmpLogName, ".csv\0");

            logNamePtr = CsrStrDup(tmpLogName);
            tmpTest.logName = logNamePtr;
            testList[curTest] = tmpTest;
            curTest++;
        }
    }
    if(configValues.runAvCS)
    {
        char *fileExtStart;
        char tmpLogName[CSR_BT_TPT_MAX_LINE_LENGTH + 1];
        char *logNamePtr;
        char *tmpAsciiInt;

        for (n = 0; n < numOfTestPerType; n++)
        {
            tmpTest.testType = CSR_BT_TPT_TEST_AV_CS;
            tmpTest.packetSize = configValues.minPacketSize + (n * configValues.packetIncrement);
            if (tmpTest.packetSize > configValues.maxPacketSize)
            {
                tmpTest.packetSize = configValues.maxPacketSize;
            }
            tmpTest.packetCount = configValues.maxNoToSend;
            tmpTest.packetsDone = 0;
            tmpTest.testFlags = TPT_TESTFLAG_NONE;
            if (configValues.runInquiry)
            {
                tmpTest.testFlags |= TPT_TESTFLAG_INQUIRY;
            }
            if (configValues.runSbcEncode)
            {
                tmpTest.testFlags |= TPT_TESTFLAG_DOSBC;
            }
            /* Remove file extention */
            strcpy(tmpLogName, TptData->avCsResultFileName);
            fileExtStart = strchr(tmpLogName, '\0');
            *(fileExtStart - 4) = '\0';

            strcat(tmpLogName, "_");

            tmpAsciiInt = intToAscii(tmpTest.packetSize);
            strcat(tmpLogName, tmpAsciiInt);
            CsrPmemFree(tmpAsciiInt);

            strcat(tmpLogName, "_");

            tmpAsciiInt = intToAscii(tmpTest.packetCount);
            strcat(tmpLogName, tmpAsciiInt);
            CsrPmemFree(tmpAsciiInt);

            strcat(tmpLogName, ".csv\0");

            logNamePtr = CsrStrDup(tmpLogName);
            tmpTest.logName = logNamePtr;
            testList[curTest] = tmpTest;
            curTest++;
        }
    }
    TptData->tests = testList;
}


static void readFileName(char *saveAddr, char *valueStart, char *valueStop)
{
    CsrUint8    offset;

    offset = 0;
    CsrMemCpy(saveAddr, valueStart, (valueStop - valueStart));
    offset += (valueStop - valueStart);
    if (!strstr(saveAddr, ".csv"))
    {
        CsrMemCpy(saveAddr + offset, ".csv", 4);
        offset += 4;
    }
    CsrMemCpy(saveAddr + offset, "\0", 1);
}


static void readServerAddress(CsrBtTptClientInstanceDataType *TptData, char *valueStart, char *valueStop)
{
    CsrUint8 n = 0;
    CsrUint8 *offset = NULL;
    char    tmpStr[CSR_BT_TPT_MAX_LINE_LENGTH + 1];

    offset = valueStart;
    CsrMemCpy(tmpStr, valueStart, 4); /* 4 is size of nap */
    TptData->tptPeerAddr.nap = (unsigned short)strtoul(tmpStr, NULL, 16);
    for (n = 0; n < 4; n++) /* 4 is size of nap */
    {
        tmpStr[n] = 0;
    }
    offset += 4 + 1; /* 4 is size of nap */
    CsrMemCpy(tmpStr, offset, 2); /* 2 is size of uap */
    TptData->tptPeerAddr.uap = (unsigned char)strtoul(tmpStr, NULL, 16);
    for (n = 0; n < 2; n++) /* 2 is size of uap */
    {
        tmpStr[n] = 0;
    }
    offset += 2 + 1; /* 2 is size of uap */
    CsrMemCpy(tmpStr, offset, 6); /* 6 is size of lap */
    TptData->tptPeerAddr.lap = strtoul(tmpStr, NULL, 16);
}


static CsrBool tptReadConfig(CsrBtTptClientInstanceDataType *TptData, const char *fName)
{
    char            line[CSR_BT_TPT_MAX_LINE_LENGTH + 1];
    char            tmpStr[CSR_BT_TPT_MAX_LINE_LENGTH + 1];
    char            *valueStart = NULL;
    char            *valueStop = NULL;
    CsrFileHandle            *f = NULL;
    CsrBtTptConfigValuesType configValues;

    configValues.maxPacketSize = 0;
    configValues.minPacketSize = 0;
    configValues.packetIncrement = 0;
    configValues.maxNoToSend = 0;
    configValues.runSppSC = FALSE;
    configValues.runSppCS = FALSE;
    configValues.runAvSC = FALSE;
    configValues.runAvCS = FALSE;
    configValues.runSbcEncode = FALSE;
    configValues.runInquiry = FALSE;

    f = CsrBtFopen(fName, "r");
    if (f == NULL)
    {
        csrBtTptClientPrint("\nConfiguration file %s could not be opened\n\n", fName);
        return FALSE;
    }
    else
    {
        csrBtTptClientPrint("\nConfiguration read:\n");
    }

    while(CsrBtFgets(line, CSR_BT_TPT_MAX_LINE_LENGTH + 1, f))
    {
        if(strstr(line, "SPP_SERVER_SEND_TEST:"))
        {
            configValues.runSppSC = strstr(line, "(Y)") ? TRUE : FALSE;
            if (configValues.runSppSC)
            {
                csrBtTptClientPrint("     * SPP SERVER SEND TEST should run\n");
            }
        }
        else if(strstr(line, "SPP_CLIENT_SEND_TEST:"))
        {
            configValues.runSppCS = strstr(line, "(Y)") ? TRUE : FALSE;
            if (configValues.runSppCS)
            {
                csrBtTptClientPrint("     * SPP CLIENT SEND TEST should run\n");
            }
        }
        else if(strstr(line, "AV_SERVER_SEND_TEST:"))
        {
            configValues.runAvSC = strstr(line, "(Y)") ? TRUE : FALSE;
            if (configValues.runAvSC)
            {
                csrBtTptClientPrint("     * AV SERVER SEND TEST should run\n");
            }
        }
        else if(strstr(line, "AV_CLIENT_SEND_TEST:"))
        {
            configValues.runAvCS = strstr(line, "(Y)") ? TRUE : FALSE;
            if (configValues.runAvCS)
            {
                csrBtTptClientPrint("     * AV CLIENT SEND TEST should run\n");
            }
        }
        else if(strstr(line, "RUN_SBC_ENCODE:"))
        {
            configValues.runSbcEncode = strstr(line, "(Y)") ? TRUE : FALSE;
            if (configValues.runSbcEncode)
            {
                csrBtTptClientPrint("     * SBC encoding runs on server while testing\n");
            }
        }
        else if(strstr(line, "RUN_INQUIRY:"))
        {
            configValues.runInquiry = strstr(line, "(Y)") ? TRUE : FALSE;
            if (configValues.runInquiry)
            {
                csrBtTptClientPrint("     * Inquiry scan runs on server while testing\n");
            }
        }
        else if(strstr(line, "MAX_PACKET_SIZE:"))
        {
            valueStart = strstr(line, "(") + 1;
            valueStop  = strstr(line, ")");

            if(valueStart && valueStop && (valueStart < valueStop))
            {
                CsrMemCpy(tmpStr, valueStart, (valueStop - valueStart));
                *(tmpStr + (valueStop - valueStart)) = '\0';
                configValues.maxPacketSize = atoi(tmpStr);
                csrBtTptClientPrint("     * Maximum packet size: %d\n", configValues.maxPacketSize);
            }
            else
            {
                csrBtTptClientPrint("\nCould not read MAX_PACKET_SIZE from configuration file\n\n");
                return FALSE;
            }
        }
        else if(strstr(line, "MIN_PACKET_SIZE:"))
        {
            valueStart = strstr(line, "(") + 1;
            valueStop  = strstr(line, ")");

            if(valueStart && valueStop && (valueStart < valueStop))
            {
                CsrMemCpy(tmpStr, valueStart, (valueStop - valueStart));
                *(tmpStr + (valueStop - valueStart)) = '\0';
                configValues.minPacketSize = atoi(tmpStr);
                csrBtTptClientPrint("     * Minimum packet size: %d\n", configValues.minPacketSize);
            }
            else
            {
                csrBtTptClientPrint("\nCould not read MIN_PACKET_SIZE from configuration file\n\n");
                return FALSE;
            }
        }
        else if(strstr(line, "PACKET_SIZE_INCREMENT:"))
        {
            valueStart = strstr(line, "(") + 1;
            valueStop  = strstr(line, ")");

            if(valueStart && valueStop && (valueStart < valueStop))
            {
                CsrMemCpy(tmpStr, valueStart, (valueStop - valueStart));
                *(tmpStr + (valueStop - valueStart)) = '\0';
                configValues.packetIncrement = atoi(tmpStr);
                csrBtTptClientPrint("     * Packet size increments: %d\n", configValues.packetIncrement);
            }
            else
            {
                csrBtTptClientPrint("\nCould not read PACKET_SIZE_INCREMENT from configuration file\n\n");
                return FALSE;
            }
        }
        else if(strstr(line, "MAX_NUMBER_TO_SEND:"))
        {
            valueStart = strstr(line, "(") + 1;
            valueStop  = strstr(line, ")");

            if(valueStart && valueStop && (valueStart < valueStop))
            {
                CsrMemCpy(tmpStr, valueStart, (valueStop - valueStart));
                *(tmpStr + (valueStop - valueStart)) = '\0';
                configValues.maxNoToSend = atoi(tmpStr);
                csrBtTptClientPrint("     * Maximum number of packets to send: %d\n", configValues.maxNoToSend);
            }
            else
            {
                csrBtTptClientPrint("\nCould not read MAX_NUMBER_TO_SEND from configuration file\n\n");
                return FALSE;
            }
        }
        else if(strstr(line, "SERVER_ADDRESS:"))
        {
            valueStart = strstr(line, "(") + 1;
            valueStop  = strstr(line, ")");

            if(valueStart && valueStop && (valueStart < valueStop))
            {
                readServerAddress(TptData, valueStart, valueStop);
                csrBtTptClientPrint("     * Peer Address: %04X:%02X:%06X\n", TptData->tptPeerAddr.nap,
                                                                        TptData->tptPeerAddr.uap,
                                                                        TptData->tptPeerAddr.lap);
            }
            else
            {
                csrBtTptClientPrint("\nCould not read SERVER_ADDRESS from configuration file\n\n");
                return FALSE;
            }
        }
        else if(strstr(line, "SPP_SERVER_SEND_FILE:"))
        {
            valueStart = strstr(line, "(") + 1;
            valueStop  = strstr(line, ")");

            if(valueStart && valueStop && (valueStart < valueStop) &&
              ((valueStop - valueStart) < CSR_BT_TPT_MAX_FILE_NAME_LENGTH ))
            {
                readFileName(TptData->sppScResultFileName, valueStart, valueStop);
                csrBtTptClientPrint("     * SPP Server Send file name: %s\n", TptData->sppScResultFileName);
            }
            else
            {
                csrBtTptClientPrint("\nCould not read SPP_SERVER_SEND_FILE from configuration file. File name may be too long.\n\n");
                return FALSE;
            }
        }
        else if(strstr(line, "SPP_CLIENT_SEND_FILE:"))
        {
            valueStart = strstr(line, "(") + 1;
            valueStop  = strstr(line, ")");

            if(valueStart && valueStop && (valueStart < valueStop) &&
              ((valueStop - valueStart) < CSR_BT_TPT_MAX_FILE_NAME_LENGTH ))
            {
                readFileName(TptData->sppCsResultFileName, valueStart, valueStop);
                csrBtTptClientPrint("     * SPP Client Send file name: %s\n", TptData->sppCsResultFileName);
            }
            else
            {
                csrBtTptClientPrint("\nCould not read SPP_CLIENT_SEND_FILE from configuration file. File name may be too long.\n\n");
                return FALSE;
            }
        }
        else if(strstr(line, "AV_SERVER_SEND_FILE:"))
        {
            valueStart = strstr(line, "(") + 1;
            valueStop  = strstr(line, ")");

            if(valueStart && valueStop && (valueStart < valueStop) &&
              ((valueStop - valueStart) < CSR_BT_TPT_MAX_FILE_NAME_LENGTH ))
            {
                readFileName(TptData->avScResultFileName, valueStart, valueStop);
                csrBtTptClientPrint("     * AV Server Send file name: %s\n", TptData->avScResultFileName);
            }
            else
            {
                csrBtTptClientPrint("\nCould not read AV_SERVER_SEND_FILE from configuration file. File name may be too long.\n\n");
                return FALSE;
            }
        }
        else if(strstr(line, "AV_CLIENT_SEND_FILE:"))
        {
            valueStart = strstr(line, "(") + 1;
            valueStop  = strstr(line, ")");

            if(valueStart && valueStop && (valueStart < valueStop) &&
              ((valueStop - valueStart) < CSR_BT_TPT_MAX_FILE_NAME_LENGTH ))
            {
                readFileName(TptData->avCsResultFileName, valueStart, valueStop);
                csrBtTptClientPrint("     * AV Client Send file name: %s\n", TptData->avCsResultFileName);
            }
            else
            {
                csrBtTptClientPrint("\nCould not read AV_CLIENT_SEND_FILE from configuration file. File name may be too long.\n\n");
                return FALSE;
            }
        }
        else if(strstr(line, "AV_SERVER_DELAY_FILE:"))
        {
            valueStart = strstr(line, "(") + 1;
            valueStop  = strstr(line, ")");

            if(valueStart && valueStop && (valueStart < valueStop) &&
              ((valueStop - valueStart) < CSR_BT_TPT_MAX_FILE_NAME_LENGTH ))
            {
                readFileName(TptData->avScDelayResultFileName, valueStart, valueStop);
                csrBtTptClientPrint("     * AV Server Send Delay file name: %s\n", TptData->avScDelayResultFileName);
            }
            else
            {
                csrBtTptClientPrint("\nCould not read AV_SERVER_DELAY_FILE from configuration file. File name may be too long.\n\n");
                return FALSE;
            }
        }
        else if(strstr(line, "AV_CLIENT_DELAY_FILE:"))
        {
            valueStart = strstr(line, "(") + 1;
            valueStop  = strstr(line, ")");

            if(valueStart && valueStop && (valueStart < valueStop) &&
              ((valueStop - valueStart) < CSR_BT_TPT_MAX_FILE_NAME_LENGTH ))
            {
                readFileName(TptData->avCsDelayResultFileName, valueStart, valueStop);
                csrBtTptClientPrint("     * AV Client Send Delay file name: %s\n", TptData->avCsDelayResultFileName);
            }
            else
            {
                csrBtTptClientPrint("\nCould not read AV_CLIENT_DELAY_FILE from configuration file. File name may be too long.\n\n");
                return FALSE;
            }
        }

    } /* End of while*/

    /* Sanity check the values read */
    if ((configValues.minPacketSize > configValues.maxPacketSize) ||
        (configValues.packetIncrement > (configValues.maxPacketSize - configValues.minPacketSize)) ||
        (configValues.minPacketSize < sizeof(TptTestDataStruct_t)))
    {
        csrBtTptClientPrint("\nData read from the configuration file are not valid.\n\n");
        return FALSE;
    }

    if ((configValues.packetIncrement == 0) && (configValues.maxPacketSize != configValues.minPacketSize))
    {
        csrBtTptClientPrint("\nData read from the configuration file are not valid - \n"
                       "PACKET_SIZE_INCREMENT = 0, i.e. MAX_PACKET_SIZE must equal MIN_PACKET_SIZE\n\n");
        return FALSE;
    }

    if ((configValues.packetIncrement != 0) && (configValues.maxPacketSize == configValues.minPacketSize))
    {
        csrBtTptClientPrint("\nData read from the configuration file are not valid - \n"
                       "MAX_PACKET_SIZE equals MIN_PACKET_SIZE, i.e. PACKET_SIZE_INCREMENT must be 0\n\n");
        return FALSE;
    }

    generateTestList(TptData, configValues);
    csrBtTptClientPrint("\nBonding with %04X:%02X:%06X...\n", TptData->tptPeerAddr.nap,
                                                         TptData->tptPeerAddr.uap,
                                                         TptData->tptPeerAddr.lap);
    CsrBtFclose(f);

    return TRUE;
}


static void csrBtTptInitInstanceData(CsrBtTptClientInstanceDataType *TptData)
{
    TptData->phandle    = TESTQUEUE;
    TptData->recvMsgP   = NULL;
    CsrMemSet(&TptData->sppResult, 0, sizeof(TptData->sppResult));

    TptData->lastTime   = 0;

    /* Check system timer resolution. */
    {
        CsrTime startTime, minTime, maxTime, tempTime, tempTime2, deltaTime;

        minTime = 0;
        maxTime = 0;
        tempTime = tempTime2 = startTime = csrBtTptHpGetTime();
        do{
            tempTime = csrBtTptHpGetTime();

            deltaTime = tempTime - tempTime2;
            if (deltaTime)
            {
                if (deltaTime > maxTime)    maxTime = deltaTime;
                if (deltaTime < minTime)    minTime = deltaTime;
            }
            tempTime2  = tempTime;

        }while ((startTime + 1000000) > tempTime);

        TptData->systemInfoClient.timerPrecision = minTime;
    }

    /* Read config file */
    if (configFile != NULL)
    {
        csrBtTptClientPrint("\nReading config file: %s...\n", configFile);
        TptData->configValid = tptReadConfig(TptData, configFile);
    }

    /* Set up test defaults */
    TptData->curTest    = 0;
}


/**************************************************************************************************
*
*    De-init function called by the scheduler upon deinit of the stack
*
**************************************************************************************************/
#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16    eventType=0;
    void        *prim=NULL;

    CsrBtTptClientInstanceDataType    *TptData;
    TptData = (CsrBtTptClientInstanceDataType *) *gash;

    csrBtTptClientPrint("Ending program, Please wait....\n");

    /* Get a message from the demoapplication message queue. The
     * message is returned in prim and the event type in eventType
     */
    while(CsrSchedMessageGet(&eventType, &prim) )
    {
        switch (eventType)
        {
            case CSR_BT_SPP_PRIM:
                CsrBtSppFreeUpstreamMessageContents(eventType, prim);
                break;
            case CSR_BT_AV_PRIM:
                CsrBtAvFreeUpstreamMessageContents(eventType, prim);
                break;
            case CSR_BT_SC_PRIM:
                CsrBtScFreeUpstreamMessageContents(eventType, prim);
                break;
            case CSR_BT_CM_PRIM:
                CsrBtCmFreeUpstreamMessageContents(eventType, prim);
                break;
        }
        CsrPmemFree(prim);
    }
    CsrPmemFree(TptData);

}
#endif


/**************************************************************************************************
*
*    Init function called by the scheduler upon initialisation. This function is used to boot
*    the TPT application.
*
**************************************************************************************************/
void CsrBtAppInit(void **gash)
{
    CsrBtTptClientInstanceDataType *TptData;
    system(CLS);
    csrBtTptClientPrint("Starting up, Please wait....\n");

    *gash = (void *) CsrPmemZalloc(sizeof(CsrBtTptClientInstanceDataType));
    TptData = (CsrBtTptClientInstanceDataType *) *gash;

    if (configFile)
    {
        csrBtTptInitInstanceData(TptData);

        if (!TptData->configValid)
        {
            csrBtTptClientPrint("Error:  TPT configuration not valid, exiting.\n");
            CsrSchedStop();
        }
        else
        {
            /*
             * Initialize SC, SPP, and SD setup.
             */
            tptScheduleRun(TptData, CSR_BT_TPT_STATE_INIT);
        }
    }
    else
    {
        csrBtTptClientPrint("Error:  No configuration file given, exiting.\n");
        CsrSchedStop();
    }
}

void applicationUsage(char *col, char *des, char *opt)
{
    strcat(col, "<-O <config>> ");
    sprintf(opt, "\t-O <config>\t\tThe configuration file to use.\n");
    strcat(des, opt);
}

char* applicationExtraOptions(void)
{
    return "o:O:";
}

char* applicationCmdLineParse(int ch, char *optionArgument)
{
    switch(ch)
    {
        case 'O':
        case 'o':
            {
                configFile = CsrStrDup(optionArgument);
                return NULL;
                break;
            }
        default:
            /*should not be possible*/
            return "Unknown option in applicationCmdLineParse()";
            break;
    }

    return NULL;
}

