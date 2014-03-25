/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #13 $
****************************************************************************/

#include "csr_synergy.h"
#ifdef CSR_USE_DSPM
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_dspm_lib.h"
#include "csr_dspm_prim.h"

#include "csr_bt_hf_main.h"
#include "csr_bt_hf_prim.h"
#include "csr_bt_hf_util.h"
#include "csr_bt_hf_dspm.h"


/*initialises the DSP instance data for this HFInstance*/
void CsrHfDspmInit(HfMainInstanceData_t *inst)
{
    if ((inst->dspmHfCallBack == NULL) ||
        (inst->dspmHfCallBack == CsrHfDspmActivateRspHandler) ||
        (inst->dspmHfCallBack == CsrHfDspmEssCfm) )
    {
        HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(inst->linkData[inst->index]);
        HfDspm_t *dspInstData = (HfDspm_t *)&(linkPtr->dspmInst);
        CsrMemSet(dspInstData,0x00, sizeof(HfDspm_t));

        dspInstData->scoSourceHandle        = inst->dspmInst.scoSourceHandle;
        dspInstData->scoSinkHandle          = inst->dspmInst.scoSinkHandle;
        dspInstData->sourceHandle           = inst->dspmInst.sourceHandle;
        dspInstData->sinkHandle             = inst->dspmInst.sinkHandle;
        dspInstData->operatorEncoder        = inst->dspmInst.operatorEncoder;
        dspInstData->operatorDecoder        = inst->dspmInst.operatorDecoder;
        dspInstData->nextOperatorEncoder    = inst->dspmInst.nextOperatorEncoder;
        dspInstData->nextOperatorDecoder    = inst->dspmInst.nextOperatorDecoder;
        dspInstData->connIdAudioDsp         = inst->dspmInst.connIdAudioDsp;
        dspInstData->connIdDspSco           = inst->dspmInst.connIdDspSco;
        dspInstData->connIdScoDsp           = inst->dspmInst.connIdScoDsp;
        dspInstData->connIdDspAudio         = inst->dspmInst.connIdDspAudio;

        dspInstData->sourceType             = inst->dspmInst.sourceType;
        dspInstData->sinkType               = inst->dspmInst.sinkType;

        inst->hfDspmOpPending               = DSPM_NOP;
    }
}

void CsrHfDspmActivateRspHandler(HfMainInstanceData_t *instData)
{
    CsrDspmStreamScoEnableReqSend(CSR_BT_HF_IFACEQUEUE, TRUE);
    instData->dspmHfCallBack = CsrHfDspmEssCfm;
}

/* Call back function for DSPM message Enable SCO streams.  */
void CsrHfDspmEssCfm(HfMainInstanceData_t *inst)
{
    CsrDspmStreamScoEnableCfm *prim = (CsrDspmStreamScoEnableCfm *)inst->recvMsgP;
    /* if Enable SCO streams fails, DSPM manager is not supported - fall back to Audio Manager */
    if(prim->result != CSR_RESULT_SUCCESS)
    {
        /* You are using the wrong chip, or something is very wrong... panic? */
    }
    inst->dspmHfCallBack = NULL;
}

/* This is the function to create operators for HF - either NB passthrough with PLC on the received SCO, or WBS Encoder and Decoder*/
void CsrBtHfDspmSendCreateOperators(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrDspmOperatorInfo *operatorInfo = CsrPmemZalloc(sizeof(CsrDspmOperatorInfo) * 2);

    operatorInfo[0].opid = CSR_DSPM_OPID_INVALID;
    operatorInfo[1].opid = CSR_DSPM_OPID_INVALID;
    /* for WB and NB we have 2 operators - in NB there is PLC too, make this manditory (should we make it possible to disable?)*/
    if (linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC)
    {
        operatorInfo[0].capability = CSR_DSPM_CAPABILITY_WBS_ENCODER;
        operatorInfo[1].capability = CSR_DSPM_CAPABILITY_WBS_DECODER;
    }
    else if (linkPtr->codecToUse == CSR_BT_WBS_CVSD_CODEC)
    {
        operatorInfo[0].capability = CSR_DSPM_CAPABILITY_SCO_SEND;
        operatorInfo[1].capability = CSR_DSPM_CAPABILITY_SCO_RECEIVE;
    }
    CsrDspmOperatorCreateReqSend(CSR_BT_HF_IFACEQUEUE, 2, operatorInfo);
    instData->dspmHfCallBack = CsrBtHfDspmCreateOperatorsRspHandler;
}

void CsrBtHfDspmCreateOperatorsRspHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrDspmOperatorCreateCfm *prim = (CsrDspmOperatorCreateCfm *)instData->recvMsgP;
    instData->dspmHfCallBack = NULL;
    if(prim->result == CSR_RESULT_SUCCESS)
    {
        /* check we have the right operator, put it in the dsp instance*/
        if(((linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC) && (prim->operatorInfo[0].capability == CSR_DSPM_CAPABILITY_WBS_ENCODER)) || 
            ((linkPtr->codecToUse == CSR_BT_WBS_CVSD_CODEC) && (prim->operatorInfo[0].capability == CSR_DSPM_CAPABILITY_SCO_SEND)))
        {
            if (dspmInst->operatorEncoder == HF_DSPM_DEFAULT_HANDLE)
            {
                dspmInst->operatorEncoder = prim->operatorInfo[0].opid;
            }
            else
            {
                dspmInst->nextOperatorEncoder = prim->operatorInfo[0].opid;
            }
        }
        else
        {
            /* - TO BE DONE - */
            /* things came in in the wrong order?  What do we do?*/
        }
        if(((linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC) && (prim->operatorInfo[1].capability == CSR_DSPM_CAPABILITY_WBS_DECODER)) || 
            ((linkPtr->codecToUse == CSR_BT_WBS_CVSD_CODEC) && (prim->operatorInfo[1].capability == CSR_DSPM_CAPABILITY_SCO_RECEIVE)))
        {
            if (dspmInst->operatorDecoder == HF_DSPM_DEFAULT_HANDLE)
            {
                dspmInst->operatorDecoder = prim->operatorInfo[1].opid;
            }
            else
            {
                dspmInst->nextOperatorDecoder = prim->operatorInfo[1].opid;
            }
        }
        else
        {
            /* - TO BE DONE - */
            /* things came in in the wrong order?  What do we do?*/
        }
        if (dspmInst->opSwap)
        {
            CsrBtHfDspmSendDestroyOldOperators(instData);
        }
        
        if ((linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION) &&
            (instData->localSupportedFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION) &&
            dspmInst->codecNegOngoing)
        {/*Now send BCS - */
            CsrBtHfSendAtBcs(instData);
        }
        else
        {
            CsrHfDspmAudioConnect(instData);
        }
    }
    else
    {
        /* The Create Operators failed - if we are attempting WBS, change the supported codecs to only CVSD, and send to AG */
        if(linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC)
        {
            instData->supportedCodecsMask = CSR_BT_WBS_CVSD_CODEC_MASK;
            sendCodecSupport(instData);
        }
        /* If we are Narrow Band, it would be worth attempting to connect without DSP, but we should probably alert the app that something is wrong */
        if(linkPtr->codecToUse == CSR_BT_WBS_CVSD_CODEC)
        {
            
        }
    }
    
    dspmInst->opSwap = FALSE;
    CsrPmemFree(prim->operatorInfo);
}


/* This is the function called to kick off an audio connection in the DSP manager
   From here we need to send at least 6 message:
   Stream Source/Sink Open Request, with SCO and audio type sources/sinks
   Stream Configure Request, with either default data or that recieved by App, for all sources and sinks (at once)
   Create Operators - either WBS operators or CVSD operators
   Create Connection
   Start Operator
   */

/*  Start the DSP Manager Stream Connection sequence.  Should only be send if using DSP manager, and SCO has started
    Start by building and sending Source/Sink Open Request */
void CsrHfDspmAudioConnect(HfMainInstanceData_t *instData)
{

    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
/* build list of sources - SCO and audio source (eg PCM)*/
    CsrDspmCpInfo *cpInfo = CsrPmemZalloc(sizeof(CsrDspmCpInfo) * 4);

    CsrUint16 dspmPcmMapping = 0;
    /* If PCM slot is selected and not set to DON'T CARE we need to give the PCM slot in the form 0x8n00, where n is the desired PCM channel*/
    if((linkPtr->pcmSlot != 0) && (linkPtr->pcmSlot != 0xFF))
    {
        dspmPcmMapping = (CsrUint16)linkPtr->pcmSlot;
        dspmPcmMapping = (dspmPcmMapping << 8) | 0x8000;
    }

    cpInfo[0].cpid = CSR_DSPM_CPID_SOURCE;
    cpInfo[0].cpType = CSR_DSPM_CP_TYPE_SCO;
    cpInfo[0].instance = linkPtr->scoHandle;
    cpInfo[0].channel = 0;
    cpInfo[1].cpid = CSR_DSPM_CPID_SINK;
    cpInfo[1].cpType = CSR_DSPM_CP_TYPE_SCO;
    cpInfo[1].instance = linkPtr->scoHandle;
    cpInfo[1].channel = 0;

/* for different CP types, there are different numbers of instances and channels
Eg, Codec typically has 1 instance, 2 channels, 
    I2S can have 2 instances with 2 channels
    PCM can have more than 1 instance, and has n channels
    Need to know which instance to use - must be application configureable, but we use defaults of first instance and channel - 
    will not work for multi-HF, up to app to manage in that case */

    cpInfo[2].cpid = CSR_DSPM_CPID_SOURCE;
    cpInfo[2].instance = dspmInst->sourceInstance;
    cpInfo[2].cpType = dspmInst->sourceType;
    /* For PCM we need to give the PCM Slot in the form 0xFn00, where n is the PCM slot*/
    if(dspmInst->sourceType == CSR_BT_HFP_DSPM_CP_TYPE_PCM)
    {
        cpInfo[2].channel = dspmPcmMapping;
    }
    else
    {
        cpInfo[2].channel = dspmInst->sourceChannel;
    }

    cpInfo[3].cpid = CSR_DSPM_CPID_SINK;
    cpInfo[3].instance = dspmInst->sinkInstance;
    cpInfo[3].cpType = dspmInst->sinkType;
    if(dspmInst->sinkType == CSR_BT_HFP_DSPM_CP_TYPE_PCM)
    {
        cpInfo[3].channel = dspmPcmMapping;
    }
    else
    {
        cpInfo[3].channel = dspmInst->sinkChannel;
    }
    CsrDspmStreamCpOpenReqSend(CSR_BT_HF_IFACEQUEUE, 4, cpInfo);
    instData->dspmHfCallBack = CsrBtHfDspmStreamOpenRspHandle;
}

void CsrBtHfDspmStreamOpenRspHandle(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrDspmStreamCpOpenCfm *prim = (CsrDspmStreamCpOpenCfm *)instData->recvMsgP;
    CsrUint8 numParams= 0;
    instData->dspmHfCallBack = NULL;
    
/* Error handling */
    if(prim->result == CSR_DSPM_RESULT_UNSUPPORTED)
    {
        /* TO BE COMPLETED*/
        /*Wrong version of chip!  
        Error handler */
        instData->hfDspmOpPending = DSPM_NOP;
    }
    else if(prim->result == CSR_DSPM_RESULT_UNAVAILABLE)
    {
        /* TO BE COMPLETED*/
        /*Error getting source/sink handles
        unavailable for whatever reason (Could be any BCCMD error above VARID_NOT_SUPPORTED).  
        Could do something clever here, at least indicate whether it was the SCO or the other CP that failed
        If it is SCO, then SCO has been disconnected or Handle is wrong - something went wrong, disconnect SCO 
        If it is PCM, then hardware is in use, or some other issue - up to Application (or HFP?) to select alternate routing, or close SCO? 
        */
        instData->hfDspmOpPending = DSPM_NOP;
        
    }
    else if(prim->result == CSR_RESULT_SUCCESS)
    {
        /* Put the handles into the DSPM structure */
        /* The DSPM prim contains all the information sent about each CP from the original message, 
           while they are returned in the same order as they are send, check they are correct before writing them into DSP instance*/
        CsrUint32 rateVal = 8000; /* 8 Khz per default: NB */
        CsrBtHfStreamConfigParam *sourceConfigParams = NULL;
        CsrBtHfStreamConfigParam *sinkConfigParams = NULL;
        CsrUint16 numSourceConfigParams = 0;
        CsrUint16 numSinkConfigParams = 0;

        if (linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC)
        {/* Wide-band */
            rateVal = 16000; /* 16KHz */
        }

        dspmInst->scoSourceHandle   =  prim->cpInfo[0].cpid;
        dspmInst->scoSinkHandle     =  prim->cpInfo[1].cpid;
        dspmInst->sourceHandle      =  prim->cpInfo[2].cpid;
        dspmInst->sinkHandle        =  prim->cpInfo[3].cpid;

        if (((linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC) && (dspmInst->sourceWbsConfigParams == NULL)) ||
            ((linkPtr->codecToUse == CSR_BT_WBS_CVSD_CODEC) && (dspmInst->sourceNbConfigParams == NULL)))
        {/* No configuration data received from the application: ensure that default configuration takes place */
            if (dspmInst->sourceType == CSR_BT_HFP_DSPM_CP_TYPE_PCM)
            {
                    numSourceConfigParams = 3;

                    sourceConfigParams = CsrPmemZalloc(sizeof(CsrBtHfStreamConfigParam) * numSourceConfigParams);
                    sourceConfigParams[0].CsrBtHfConfigParamId  = 0x0101; /* PCM_MASTER_CLOCK_RATE */
                    sourceConfigParams[0].CsrBtHfConfigParamVal = 2304000; /* 48000 */
                    sourceConfigParams[1].CsrBtHfConfigParamId  = 0x0100; /* PCM_SYNC_RATE */
                    sourceConfigParams[1].CsrBtHfConfigParamVal = rateVal;
                    sourceConfigParams[2].CsrBtHfConfigParamId  = 0x0102; /* PCM_SYNC_RATE */
                    sourceConfigParams[2].CsrBtHfConfigParamVal = 0x01;   /* master */
            }
            else if (dspmInst->sourceType == CSR_BT_HFP_DSPM_CP_TYPE_CODEC)
            {
                numSourceConfigParams = 2;
                sourceConfigParams = CsrPmemZalloc(sizeof(CsrBtHfStreamConfigParam) * numSourceConfigParams);
                sourceConfigParams[0].CsrBtHfConfigParamId  = 0x0300; /* CODEC_ADC_RATE */
                sourceConfigParams[0].CsrBtHfConfigParamVal = rateVal;
                sourceConfigParams[1].CsrBtHfConfigParamId  = 0x0302; /* CODEC_ADC_GAIN */
                sourceConfigParams[1].CsrBtHfConfigParamVal = 0x0A;
            }
            else if (dspmInst->sourceType == CSR_BT_HFP_DSPM_CP_TYPE_I2S)
            {
                numSourceConfigParams = 3;
                sourceConfigParams = CsrPmemZalloc(sizeof(CsrBtHfStreamConfigParam) * numSourceConfigParams);
                sourceConfigParams[0].CsrBtHfConfigParamId   = 0x0201; /* I2S_MASTER_CLOCK_RATE */
                sourceConfigParams[0].CsrBtHfConfigParamVal  = 2304000; /* 48000 */
                sourceConfigParams[1].CsrBtHfConfigParamId   = 0x0200; /* I2S_SYNC_RATE */
                sourceConfigParams[1].CsrBtHfConfigParamVal  = rateVal;
                sourceConfigParams[2].CsrBtHfConfigParamId   = 0x0202; /* I2S_MASTER_MODE */
                sourceConfigParams[2].CsrBtHfConfigParamVal  = 0x01;   /* master */
            }
        }
        else if (linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC)
        {/* WBS */
            numSourceConfigParams = dspmInst->numSourceWbsConfigParams;
            sourceConfigParams    = dspmInst->sourceWbsConfigParams;
        }
        else
        {/* NB */
            numSourceConfigParams = dspmInst->numSourceNbConfigParams;
            sourceConfigParams    = dspmInst->sourceNbConfigParams;
        }

        if (((linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC) && (dspmInst->sinkWbsConfigParams == NULL)) ||
            ((linkPtr->codecToUse == CSR_BT_WBS_CVSD_CODEC) && (dspmInst->sinkNbConfigParams == NULL)))
        {
            if (dspmInst->sinkType == CSR_BT_HFP_DSPM_CP_TYPE_PCM)
            {
                numSinkConfigParams   = 3;
                sinkConfigParams = CsrPmemZalloc(sizeof(CsrBtHfStreamConfigParam) * numSinkConfigParams);
                sinkConfigParams[0].CsrBtHfConfigParamId  = 0x0101; /* PCM_MASTER_CLOCK_RATE */
                sinkConfigParams[0].CsrBtHfConfigParamVal = 2304000; /* 48000 */
                sinkConfigParams[1].CsrBtHfConfigParamId  = 0x0100; /* PCM_SYNC_RATE */
                sinkConfigParams[1].CsrBtHfConfigParamVal = rateVal;
                sinkConfigParams[2].CsrBtHfConfigParamId  = 0x0102; /* PCM_MASTER_MODE */
                sinkConfigParams[2].CsrBtHfConfigParamVal = 0x01;   /* master */
            }
            else if (dspmInst->sinkType == CSR_BT_HFP_DSPM_CP_TYPE_CODEC)
            {
                numSinkConfigParams = 2;
                sinkConfigParams = CsrPmemZalloc(sizeof(CsrBtHfStreamConfigParam) * numSinkConfigParams);
                sinkConfigParams[0].CsrBtHfConfigParamId  = 0x0301; /* CODEC_DAC_RATE */
                sinkConfigParams[0].CsrBtHfConfigParamVal = rateVal; 
                sinkConfigParams[1].CsrBtHfConfigParamId  = 0x0303; /* CODEC_DAC_GAIN */
                sinkConfigParams[1].CsrBtHfConfigParamVal = 0x0A;
            }
            else if (dspmInst->sinkType == CSR_BT_HFP_DSPM_CP_TYPE_I2S)
            {
                numSinkConfigParams = 3;
                sinkConfigParams = CsrPmemZalloc(sizeof(CsrBtHfStreamConfigParam) * numSinkConfigParams);
                sinkConfigParams[0].CsrBtHfConfigParamId  = 0x0201; /* I2S_MASTER_CLOCK_RATE */
                sinkConfigParams[0].CsrBtHfConfigParamVal = 2304000; /* 48000 */
                sinkConfigParams[1].CsrBtHfConfigParamId  = 0x0200; /* I2S_SYNC_RATE */
                sinkConfigParams[1].CsrBtHfConfigParamVal = rateVal;
                sinkConfigParams[2].CsrBtHfConfigParamId  = 0x0202; /* I2S_MASTER_MODE */
                sinkConfigParams[2].CsrBtHfConfigParamVal = 0x01;   /* master */
            }
        }
        else if (linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC)
        {/* WBS */
            numSinkConfigParams = dspmInst->numSinkWbsConfigParams;
            sinkConfigParams    = dspmInst->sinkWbsConfigParams;
        }
        else
        {/* NB */
            numSinkConfigParams = dspmInst->numSinkNbConfigParams;
            sinkConfigParams    = dspmInst->sinkNbConfigParams;
        }
        
        /* Now we have the CP handles, send config parameters, if any*/
        numParams = dspmInst->numScoSourceConfigParams + dspmInst->numScoSinkConfigParams + numSourceConfigParams + numSinkConfigParams;
        if (numParams != 0)
        {
            CsrUint16 i = 0;
            CsrDspmConfigInfo *configInfo = CsrPmemZalloc(sizeof(CsrDspmConfigInfo) * numParams);
            for(i=0; i<numParams; i++)
            {
                CsrUint16 j;
                CsrUint16 offset = 0;
                if((i >= 0) && (i < dspmInst->numScoSourceConfigParams) && dspmInst->scoSourceConfigParams)
                {
                    configInfo[i].cpid      = dspmInst->scoSourceHandle;
                    configInfo[i].feature   = dspmInst->scoSourceConfigParams[i].CsrBtHfConfigParamId;
                    configInfo[i].value     = dspmInst->scoSourceConfigParams[i].CsrBtHfConfigParamVal;
                }
                
                offset += dspmInst->numScoSourceConfigParams;
                
                if((i >= offset) && (i < (offset + dspmInst->numScoSinkConfigParams)) && dspmInst->scoSinkConfigParams)
                {
                    j = i - offset;
                    configInfo[i].cpid      = dspmInst->scoSinkHandle;
                    configInfo[i].feature   = dspmInst->scoSinkConfigParams[j].CsrBtHfConfigParamId;
                    configInfo[i].value     = dspmInst->scoSinkConfigParams[j].CsrBtHfConfigParamVal;
                }
                
                offset += dspmInst->numScoSinkConfigParams;
                
                if((i >= offset) && (i < (offset + numSourceConfigParams))
                     && sourceConfigParams)
                {
                    j = i - offset;
                    configInfo[i].cpid      = dspmInst->sourceHandle;
                    configInfo[i].feature   = sourceConfigParams[j].CsrBtHfConfigParamId;
                    configInfo[i].value     = sourceConfigParams[j].CsrBtHfConfigParamVal;
                }
                
                offset += numSourceConfigParams;
                
                if ((i >= offset) && (i < (offset + numSinkConfigParams))
                     && sinkConfigParams)
                {
                    j = i - offset;
                    configInfo[i].cpid      = dspmInst->sinkHandle;
                    configInfo[i].feature   = sinkConfigParams[j].CsrBtHfConfigParamId;
                    configInfo[i].value     = sinkConfigParams[j].CsrBtHfConfigParamVal;
                }
            }
            CsrDspmStreamConfigureReqSend(CSR_BT_HF_IFACEQUEUE, numParams, configInfo);
            instData->dspmHfCallBack = CsrBtHfDspmConfigStreamRspHandler;
            if ((sinkConfigParams != dspmInst->sinkWbsConfigParams) && (sinkConfigParams != dspmInst->sinkNbConfigParams))
            {
                CsrPmemFree(sinkConfigParams);
            }
            if ((sourceConfigParams != dspmInst->sourceWbsConfigParams) && (sourceConfigParams != dspmInst->sourceNbConfigParams))
            {
                CsrPmemFree(sourceConfigParams);
            }
        }
        else
        {
            /* No config parameters to send, the operators are created, connect the streams!*/
            CsrBtHfDspmSendConnectStreams(instData);
        }
    }
            /* Other Result IDs not handled here */
    CsrPmemFree(prim->cpInfo);
}


void CsrBtHfDspmConfigStreamRspHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrDspmStreamConfigureCfm *prim = (CsrDspmStreamConfigureCfm *)instData->recvMsgP;
    instData->dspmHfCallBack = NULL;

    /*Check the config was successful, then Create Operators */
    if (prim->result == CSR_RESULT_SUCCESS)
    {
        /*We have CP, all is configured, connect streams!*/
        CsrBtHfDspmSendConnectStreams(instData);
    }
    else
    {
        /* - TO BE DONE - */
        /*Config failed - this is Application supplied configuration, we need to inform the app, and send new parameters before continuing - or what?*/
        instData->hfDspmOpPending = DSPM_NOP;
    }
    CsrPmemFree(prim->configInfo);
}

void CsrBtHfDspmSendSingleConfigStream(HfMainInstanceData_t *instData, CsrBtHfAudioDspStreamSingleConfig *configData)
{
    CsrDspmConfigInfo *configInfo = CsrPmemZalloc(sizeof(CsrDspmConfigInfo));
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;

    configInfo->cpid      = dspmInst->sinkHandle;
    configInfo->feature   = configData->configPair.CsrBtHfConfigParamId;
    configInfo->value     = configData->configPair.CsrBtHfConfigParamVal;
    
    CsrDspmStreamConfigureReqSend(CSR_BT_HF_IFACEQUEUE, 1, configInfo);
    instData->dspmHfCallBack = CsrBtHfDspmSingleConfigStreamRspHandler;
}

void CsrBtHfDspmSingleConfigStreamRspHandler(HfMainInstanceData_t *instData)
{
    CsrDspmStreamConfigureCfm *prim = (CsrDspmStreamConfigureCfm *)instData->recvMsgP;
    instData->dspmHfCallBack = NULL;
    CsrPmemFree(prim->configInfo);
}

void CsrBtHfDspmSendConnectStreams(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrUint8 hfDspmNumConnections = 4;
    CsrDspmConnectionInfo *connectionInfo;
    /* Check if we have some operators for the DSP - ie that Create Operators was successful */
    if(dspmInst->operatorDecoder != HF_DSPM_DEFAULT_HANDLE)
    {
        /* for SCO there will always be 4 connections as long as using DSP is default (it is, as of writing this) */
        connectionInfo = CsrPmemZalloc(sizeof(CsrDspmConnectionInfo) * hfDspmNumConnections);

        /* SCO data received to "decoder" sink*/
        connectionInfo[2].cid        = CSR_DSPM_CID_INVALID;
        connectionInfo[2].cpidSource = dspmInst->scoSourceHandle;
        connectionInfo[2].cpidSink   = CSR_DSPM_CPID_OPERATOR_SINK(dspmInst->operatorDecoder,0);
        /* Decoder output (source) to the selected CP Sink (default PCM) */
        connectionInfo[0].cid        = CSR_DSPM_CID_INVALID;
        connectionInfo[0].cpidSource = CSR_DSPM_CPID_OPERATOR_SOURCE(dspmInst->operatorDecoder,0);
        connectionInfo[0].cpidSink   = dspmInst->sinkHandle;
        /* selected CP Source (default PCM) to Encoder sink */
        connectionInfo[1].cid        = CSR_DSPM_CID_INVALID;
        connectionInfo[1].cpidSource = dspmInst->sourceHandle;
        connectionInfo[1].cpidSink   = CSR_DSPM_CPID_OPERATOR_SINK(dspmInst->operatorEncoder,0); 
        /* Emcoder source to the SCO transmit */
        connectionInfo[3].cid        = CSR_DSPM_CID_INVALID;
        connectionInfo[3].cpidSource = CSR_DSPM_CPID_OPERATOR_SOURCE(dspmInst->operatorEncoder,0);
        connectionInfo[3].cpidSink   = dspmInst->scoSinkHandle;
    }
    /* Create Operator failed, so connect SCO direct to CPs */
    else 
    {
        if(linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC)
        {/*TO BE COMPLETED*/
         /* This is bad - somehow we have WBS and no Operator */
        }
        else
        {/* SCO data received to CP sink*/
            hfDspmNumConnections = 2;
            connectionInfo = CsrPmemZalloc(sizeof(CsrDspmConnectionInfo) * hfDspmNumConnections);
        
            connectionInfo[0].cid        = CSR_DSPM_CID_INVALID;
            connectionInfo[0].cpidSource = dspmInst->scoSourceHandle;
            connectionInfo[0].cpidSink = dspmInst->sinkHandle;
            /* CP data to SCO transmit*/
            connectionInfo[1].cid        = CSR_DSPM_CID_INVALID;
            connectionInfo[1].cpidSource = dspmInst->sourceHandle;
            connectionInfo[1].cpidSink = dspmInst->scoSinkHandle;
        }
    }
    CsrDspmConnectionCreateReqSend(CSR_BT_HF_IFACEQUEUE, hfDspmNumConnections, connectionInfo);
    instData->dspmHfCallBack = CsrBtHfDspmConnectionCreateRspHandler;
}

void CsrBtHfDspmConnectionCreateRspHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrDspmConnectionCreateCfm *prim = (CsrDspmConnectionCreateCfm *)instData->recvMsgP;
    instData->dspmHfCallBack = NULL;

    if((prim->result == CSR_RESULT_SUCCESS)&&(prim->connectionInfoCount == 4))
    {
        dspmInst->connIdDspAudio = prim->connectionInfo[0].cid;
        dspmInst->connIdAudioDsp = prim->connectionInfo[1].cid;
        dspmInst->connIdScoDsp   = prim->connectionInfo[2].cid;
        dspmInst->connIdDspSco   = prim->connectionInfo[3].cid;
        /* Now start the operators if any */
        if ((dspmInst->operatorEncoder != HF_DSPM_DEFAULT_HANDLE) || (dspmInst->operatorDecoder != HF_DSPM_DEFAULT_HANDLE))
        {
            CsrDspmOpid *opId = CsrPmemZalloc(sizeof(CsrDspmOpid) * 2);
            opId[0] = dspmInst->operatorEncoder;
            opId[1] = dspmInst->operatorDecoder;
            CsrDspmOperatorStartReqSend(CSR_BT_HF_IFACEQUEUE, 2,opId);
            instData->dspmHfCallBack = CsrDspmOperatorStartRspHandler;
        }
    }
    else
    {
        instData->hfDspmOpPending = DSPM_NOP;
    }
    
    CsrPmemFree(prim->connectionInfo);
}

void CsrDspmOperatorStartRspHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrDspmOperatorStartCfm *prim = (CsrDspmOperatorStartCfm *)instData->recvMsgP;
    instData->dspmHfCallBack = NULL;
    CsrPmemFree(prim->opid);
    instData->hfDspmOpPending = DSPM_NOP;
    /* No point in checking the result code? What to do if the allocated and connected operator cannot be started??? */
}

/* DSPM audio disconnection helper functions */
void CsrBtHfDspmSendDisconnectStreams(HfMainInstanceData_t *instData)
{
    /* This means the following steps:
    1. Stop operators 
    2. Disconect streams 
    3. Close stream CPs
    4. Keep the used operators or destroy operators and keep the new ones if these exist
    */
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrUint8 hfCount = 2;

    if ((dspmInst->operatorEncoder != HF_DSPM_DEFAULT_HANDLE) || (dspmInst->operatorDecoder != HF_DSPM_DEFAULT_HANDLE))
    {/* Stop operators */
        CsrDspmOpid *opId = CsrPmemZalloc(hfCount * sizeof(CsrDspmOpid));
        opId[0] = dspmInst->operatorEncoder;
        opId[1] = dspmInst->operatorDecoder;
        CsrDspmOperatorStopReqSend(CSR_BT_HF_IFACEQUEUE, hfCount,opId);
        instData->dspmHfCallBack = CsrBtHfDspmOperatorStopRspHandler;
    }
    else if ((dspmInst->connIdScoDsp != HF_DSPM_DEFAULT_HANDLE) || (dspmInst->connIdDspSco != HF_DSPM_DEFAULT_HANDLE))
    {/* Disconnect streams */
        CsrDspmCid *cid;
        hfCount = 4;
        cid = CsrPmemZalloc(hfCount * sizeof(CsrDspmCid));
        cid[0] = dspmInst->connIdDspAudio;
        cid[1] = dspmInst->connIdAudioDsp;
        cid[2] = dspmInst->connIdScoDsp;
        cid[3] = dspmInst->connIdDspSco;
        CsrDspmConnectionDestroyReqSend(CSR_BT_HF_IFACEQUEUE, hfCount, cid);
        instData->dspmHfCallBack = CsrBtHfDspmConnectionDestroyRspHandler;
    }
    else 
    {/* There was no audo connection and no operators at all; just de-allocate the connection points allocated... if any! */
        if ((dspmInst->sourceHandle != HF_DSPM_DEFAULT_HANDLE) || (dspmInst->sinkHandle != HF_DSPM_DEFAULT_HANDLE))
        {/* source allocated: de-allocate it */
            CsrDspmCid *cid = CsrPmemZalloc(hfCount * sizeof(CsrDspmCid));
            cid[0] = dspmInst->sourceHandle;
            cid[1] = dspmInst->sinkHandle;
            CsrDspmStreamCpCloseReqSend(CSR_BT_HF_IFACEQUEUE, hfCount, cid);
            instData->dspmHfCallBack = CsrBtHfDspmStreamCpCloseRspHandler;
        }
    }
}

void CsrBtHfDspmOperatorStopRspHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrDspmOperatorStopCfm *prim = (CsrDspmOperatorStopCfm *)instData->recvMsgP;
    CsrUint8 hfCount = 2;
    instData->dspmHfCallBack = NULL;
    
    
    /*if(prim->result == CSR_RESULT_SUCCESS)*/
    /* No need to check for the result... if the stop operation failed, the operators will be automatically stopped 
       when the connection is released */
    {/* Now disconnect the streams, before destroying the operators if needed */
        if ((dspmInst->connIdScoDsp != HF_DSPM_DEFAULT_HANDLE) || (dspmInst->connIdDspSco != HF_DSPM_DEFAULT_HANDLE))
        {
            CsrDspmCid *cid;
            hfCount = 4;
            cid = CsrPmemZalloc(hfCount * sizeof(CsrDspmCid));
            cid[0] = dspmInst->connIdDspAudio;
            cid[1] = dspmInst->connIdAudioDsp;
            cid[2] = dspmInst->connIdScoDsp;
            cid[3] = dspmInst->connIdDspSco;
            CsrDspmConnectionDestroyReqSend(CSR_BT_HF_IFACEQUEUE, hfCount, cid);
            instData->dspmHfCallBack = CsrBtHfDspmConnectionDestroyRspHandler;
        }
        else if ((dspmInst->sourceHandle != HF_DSPM_DEFAULT_HANDLE) || (dspmInst->sinkHandle != HF_DSPM_DEFAULT_HANDLE))
        {/* No streams existed: just destroy the operators */
            CsrDspmCid *cid = CsrPmemZalloc(hfCount * sizeof(CsrDspmCid));
            /* Close the stream connection points */
            cid[0] = dspmInst->sourceHandle;
            cid[1] = dspmInst->sinkHandle;
            CsrDspmStreamCpCloseReqSend(CSR_BT_HF_IFACEQUEUE, hfCount, cid);
            instData->dspmHfCallBack = CsrBtHfDspmStreamCpCloseRspHandler;
        }
    }

    CsrPmemFree(prim->opid);
}

void CsrBtHfDspmConnectionDestroyRspHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrDspmConnectionDestroyCfm *prim = (CsrDspmConnectionDestroyCfm *)instData->recvMsgP;
    CsrUint8 hfCount = 2;
    CsrDspmCid *cid = CsrPmemZalloc(hfCount * sizeof(CsrDspmCid));

    instData->dspmHfCallBack = NULL;
    CsrPmemFree(prim->cid);

    dspmInst->connIdScoDsp    = HF_DSPM_DEFAULT_HANDLE;
    dspmInst->connIdDspSco    = HF_DSPM_DEFAULT_HANDLE;
    dspmInst->scoSourceHandle = HF_DSPM_DEFAULT_HANDLE;
    dspmInst->scoSinkHandle   = HF_DSPM_DEFAULT_HANDLE;

    /* Close the stream connection points */
    cid[0] = dspmInst->sourceHandle;
    cid[1] = dspmInst->sinkHandle;
    CsrDspmStreamCpCloseReqSend(CSR_BT_HF_IFACEQUEUE, hfCount, cid);
    instData->dspmHfCallBack = CsrBtHfDspmStreamCpCloseRspHandler;
}

void CsrBtHfDspmOperatorDestroyRspHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrDspmOperatorDestroyCfm *prim = (CsrDspmOperatorDestroyCfm *)instData->recvMsgP;
    
    CsrPmemFree(prim->opid);
    
    if ((dspmInst->nextOperatorEncoder != HF_DSPM_DEFAULT_HANDLE) && (dspmInst->nextOperatorDecoder != HF_DSPM_DEFAULT_HANDLE))
    {
        dspmInst->operatorEncoder = dspmInst->nextOperatorEncoder;
        dspmInst->operatorDecoder = dspmInst->nextOperatorDecoder;
        dspmInst->nextOperatorEncoder = HF_DSPM_DEFAULT_HANDLE;
        dspmInst->nextOperatorDecoder = HF_DSPM_DEFAULT_HANDLE;
    }
    else
    {
        dspmInst->operatorEncoder = HF_DSPM_DEFAULT_HANDLE;
        dspmInst->operatorDecoder = HF_DSPM_DEFAULT_HANDLE;
    }

    instData->dspmHfCallBack = NULL;
}

void CsrBtHfDspmSendDestroyOldOperators(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrUint8 hfCount = 2;
    CsrDspmOpid *opId = CsrPmemZalloc(hfCount * sizeof(CsrDspmOpid));

    opId[0] = dspmInst->operatorEncoder;
    opId[1] = dspmInst->operatorDecoder;
    CsrDspmOperatorDestroyReqSend(CSR_BT_HF_IFACEQUEUE, hfCount,opId);
    instData->dspmHfCallBack = CsrBtHfDspmOperatorDestroyRspHandler;
}

void CsrBtHfDspmStreamCpCloseRspHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrDspmStreamCpCloseCfm *prim = (CsrDspmStreamCpCloseCfm *)instData->recvMsgP;
    instData->dspmHfCallBack = NULL;
    instData->hfDspmOpPending = DSPM_NOP;
    CsrPmemFree(prim->cpid);
    dspmInst->sourceHandle = HF_DSPM_DEFAULT_HANDLE;
    dspmInst->sinkHandle   = HF_DSPM_DEFAULT_HANDLE;
    
    if (dspmInst->cleanUp)
    {/* This is due to a disconnection of the SLC: make sure to remove the operators! 
        The callback function must be set to NULL already so when the answer to the operator de-allocation
        arrives, the message will just be freed and discarded.*/
        CsrBtHfDspmCleanUpOperators(linkPtr);
    }
    else
    {/* Normal behavior: just make sure to set up the needed parameters to allow for a furtur eSCO on this very SLC. */
        if ((dspmInst->nextOperatorEncoder != HF_DSPM_DEFAULT_HANDLE) && (dspmInst->nextOperatorDecoder != HF_DSPM_DEFAULT_HANDLE))
        {
            CsrUint8 hfCount = 2;
            CsrDspmOpid *opId = CsrPmemZalloc(hfCount * sizeof(CsrDspmOpid));
            opId[0] = dspmInst->operatorEncoder;
            opId[1] = dspmInst->operatorDecoder;
            CsrDspmOperatorDestroyReqSend(CSR_BT_HF_IFACEQUEUE, hfCount,opId);
            instData->dspmHfCallBack = CsrBtHfDspmOperatorDestroyRspHandler;
        }
        if (linkPtr->state == Connected_s || linkPtr->state == LpEnabled_s)
        {/* Make sure to accept incoming (e)SCO with the proper settings...unless the link is released! */
            CsrBtHfAcceptIncomingSco(instData);
        }
    }
}


void CsrBtHfDspmCleanUpOperators(HfInstanceData_t *linkPtr)
{
    HfDspm_t *dspmInst = (HfDspm_t *)&linkPtr->dspmInst;
    CsrUint8 hfCount = 0;
    CsrBool nextOpExist = FALSE;
    CsrDspmOpid *opId = NULL;

    if (dspmInst->operatorEncoder != HF_DSPM_DEFAULT_HANDLE) 
    {
        hfCount += 2;
    }
    if (dspmInst->nextOperatorEncoder != HF_DSPM_DEFAULT_HANDLE)
    {
        hfCount += 2;
        nextOpExist = TRUE;
    }

    if (hfCount != 0)
    {
        opId = CsrPmemZalloc(hfCount * sizeof(CsrDspmOpid));
        if (hfCount == 2) 
        {
            if (nextOpExist == FALSE)
            {
                opId[0] = dspmInst->operatorEncoder;
                opId[1] = dspmInst->operatorDecoder;
            }
            else
            {
                opId[0] = dspmInst->nextOperatorEncoder;
                opId[1] = dspmInst->nextOperatorDecoder;
            }
        }
        else
        {/* Need to free 4 operators! */
            opId[0] = dspmInst->operatorEncoder;
            opId[1] = dspmInst->operatorDecoder;
            opId[2] = dspmInst->nextOperatorEncoder;
            opId[3] = dspmInst->nextOperatorDecoder;
        }
        CsrDspmOperatorDestroyReqSend(CSR_BT_HF_IFACEQUEUE, hfCount,opId);
    }
}
#endif /*CSR_USE_DSPM*/
