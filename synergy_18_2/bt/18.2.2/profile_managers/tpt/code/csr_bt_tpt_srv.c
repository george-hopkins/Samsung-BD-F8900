/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_util.h"
#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_srv.h"

void csrBtTptAvSbcCoding(CsrUint16 dummy, CsrBtTptServerInstanceDataType *TptData)
{
    CsrUint8 *sampleData;
    CSR_UNUSED(dummy);

    sampleData = CsrPmemZalloc((CSR_BT_SBC_CODING_SAMPLE_DATA_SIZE_X + 1) * (CSR_BT_SBC_CODING_SAMPLE_DATA_SIZE_Y + 1) * sizeof(CsrUint16));

    CsrSbcEncode(TptData->sbcHdl,
               (const CsrInt16(*)[2])sampleData,
               TptData->sbcFramePtr,
               30);

    CsrPmemFree(sampleData);

    if (TptData->runSbcCoding)
    {
        TptData->timerId = CsrSchedTimerSet(CSR_BT_TPT_SBC_ENCODE_DELAY, (void (*) (CsrUint16, void *)) csrBtTptAvSbcCoding, 0, (void *) TptData);
    }
}

void csrBtTptPrepareSbc(CsrBtTptServerInstanceDataType *TptData)
{
    CsrUint8 bitPoolAlt;
    CsrUint8 togglePeriod;
    CsrUint8 bitPool;

    CsrSchedTimerCancel(TptData->timerId,NULL,NULL);

    TptData->sbcHdl = CsrSbcOpen();

    bitPool = CsrSbcCalcBitPool( &bitPoolAlt,
                               &togglePeriod,
                               CSR_SBC_JOINT_STEREO,
                               44100,
                               8,                  /* config->blocks,    */
                               8,                  /* config->subbands,  */
                               320);               /* targetBitrate);    */


    TptData->sbcFramesize = CsrSbcConfig(TptData->sbcHdl,
                                       (CsrSbcChannelMode)(CSR_SBC_JOINT_STEREO),   /* config->channel_mode,            */
                                       (CsrSbcAllocMethod)(CSR_SBC_METHOD_SNR),     /* config->alloc_method,            */
                                       (CsrUint16)(44100),                      /* config->sample_freq,             */
                                       (CsrUint8)(8),                           /* config->blocks,                  */
                                       (CsrUint8)(8),                           /* config->subbands,                */
                                       (CsrUint8)bitPool);                      /* config->bitPool                  */

    TptData->sbcFramePtr    = (CsrUint8*) CsrPmemAlloc(TptData->sbcFramesize);       /* The framesize for the bit pool   */
}


static void csrBtTptInitInstanceData(CsrBtTptServerInstanceDataType *TptData)
{
    TptData->phandle    = TESTQUEUE;
    TptData->avDeactivated          = TRUE;
    TptData->reActivateOnDisconnect = CSR_BT_TPT_REACTIVATE_SPP_ON_DISCONNECT;

    /* Check system timer resolution. */
    {
        CsrTime startTime, minTime, maxTime, tempTime, tempTime2, deltaTime;

        minTime = 0;
        maxTime = 0;
        tempTime = tempTime2 = startTime = CsrTimeGet(NULL);
        do{
            tempTime = CsrTimeGet(NULL);

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
}


/**************************************************************************************************
*
*    De-init function called by the scheduler upon deinit of the stack
*
**************************************************************************************************/
#ifdef ENABLE_SHUTDOWN
void CsrBtTptDeinit(void **gash)
{

    CsrBtTptServerInstanceDataType    *TptData;
    TptData = (CsrBtTptServerInstanceDataType *) *gash;

    CsrPmemFree(TptData->sbcFramePtr);
    CsrPmemFree(TptData->sbcHdl);
    CsrPmemFree(TptData->timestamps);

}
#endif


/**************************************************************************************************
*
*    Init function called by the scheduler upon initialisation. This function is used to boot
*    the TPT application.
*
**************************************************************************************************/
void CsrBtTptInit(void **gash)
{
    CsrBtTptServerInstanceDataType *TptData;

    *gash = (void *) CsrPmemZalloc(sizeof(CsrBtTptServerInstanceDataType));
    TptData = (CsrBtTptServerInstanceDataType *) *gash;
    csrBtTptInitInstanceData(TptData);

    /*
     * Initialize SC, SPP, and SD setup.
     */
    tptScheduleRun(TptData, CSR_BT_TPT_STATE_INIT);

}

void applicationUsage(char *col, char *des, char *opt)
{
}

char* applicationCmdLineParse(int ch, char *optionArgument)
{
    return NULL;
}

void applicationSetup(void)
{
}

char* applicationExtraOptions(void)
{
    return "";
}
