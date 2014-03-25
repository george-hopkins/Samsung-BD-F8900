/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #6 $
****************************************************************************/
#include "csr_synergy.h"
#ifdef CSR_USE_DSPM

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_dspm_lib.h"
#include "csr_dspm_prim.h"

#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_consts.h"
#include "csr_bt_hfg_proto.h"


/*initialises the DSP instance data for this HFInstance*/
void CsrHfgDspmInit(HfgInstance_t *inst)
{
    HfgDspm_t *dspInstData     = (HfgDspm_t *)&(inst->dspmInst);
    HfgDspm_t *mainDspInstData = (HfgDspm_t *)&(CsrBtHfgGetMainInstance(inst)->dspmInst);
    CsrMemSet(dspInstData,0x00, sizeof(HfgDspm_t));

    dspInstData->scoSourceHandle    = mainDspInstData->scoSourceHandle;
    dspInstData->scoSinkHandle      = mainDspInstData->scoSinkHandle;
    dspInstData->sourceHandle       = mainDspInstData->sourceHandle;
    dspInstData->sinkHandle         = mainDspInstData->sinkHandle;
    dspInstData->operatorEncoder    = mainDspInstData->operatorEncoder;
    dspInstData->operatorDecoder    = mainDspInstData->operatorDecoder;
    dspInstData->connIdAudioDsp     = mainDspInstData->connIdAudioDsp;
    dspInstData->connIdDspSco       = mainDspInstData->connIdDspSco;
    dspInstData->connIdScoDsp       = mainDspInstData->connIdScoDsp;
    dspInstData->connIdDspAudio     = mainDspInstData->connIdDspAudio;

    dspInstData->sourceType         = mainDspInstData->sourceType;
    dspInstData->sinkType           = mainDspInstData->sinkType;
    
    CsrBtHfgGetMainInstance(inst)->hfgDspmOpPending = DSPM_NOP;
    inst->lastCodecUsed             = CSR_BT_WBS_INVALID_CODEC;
}

void CsrHfgDspmActivateRspHandler(HfgInstance_t *instData)
{
    CsrDspmStreamScoEnableReqSend(CSR_BT_HFG_IFACEQUEUE, TRUE);
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrHfgDspmEssCfm;
}

/* Call back function for DSPM message Enable SCO streams.  */
void CsrHfgDspmEssCfm(HfgInstance_t *inst)
{
    CsrDspmStreamScoEnableCfm *prim = (CsrDspmStreamScoEnableCfm *)((CsrBtHfgGetMainInstance(inst))->msg);
    /* if Enable SCO streams fails, DSPM manager is not supported - fall back to Audio Manager */
    if(prim->result != CSR_RESULT_SUCCESS)
    {
        /* You are using the wrong chip, or something is very wrong... panic? */
    }
    CsrBtHfgGetMainInstance(inst)->dspmHfgCallBack = NULL;
}

/* This is the function to create operators for HF - either NB passthrough with PLC on the received SCO, or WBS Encoder and Decoder*/
void CsrBtHfgDspmSendCreateOperators(HfgInstance_t *instData)
{
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrDspmOperatorInfo *operatorInfo = CsrPmemZalloc(sizeof(CsrDspmOperatorInfo) * 2);

    operatorInfo[0].opid = CSR_DSPM_OPID_INVALID;
    operatorInfo[1].opid = CSR_DSPM_OPID_INVALID;
    
    if (((instData->remSupFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION) == 0) ||
        ((CsrBtHfgGetMainInstance(instData)->locSupFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION) == 0))
    {/* In this case, codec negotiation has not taken place: make sure that a sensible codec is used! */
        instData->lastCodecUsed = CSR_BT_WBS_CVSD_CODEC;
    }

    /* for WB and NB we have 2 operators - in NB there is PLC too, make this manditory (should we make it possible to disable?)*/
    if (instData->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC)
    {
        operatorInfo[0].capability = CSR_DSPM_CAPABILITY_WBS_ENCODER;
        operatorInfo[1].capability = CSR_DSPM_CAPABILITY_WBS_DECODER;
    }
    else if (instData->lastCodecUsed == CSR_BT_WBS_CVSD_CODEC)
    {
        operatorInfo[0].capability = CSR_DSPM_CAPABILITY_SCO_SEND;
        operatorInfo[1].capability = CSR_DSPM_CAPABILITY_SCO_RECEIVE;
    }
    CsrDspmOperatorCreateReqSend(CSR_BT_HFG_IFACEQUEUE, 2, operatorInfo);
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack  = CsrBtHfgDspmCreateOperatorsRspHandler;
    CsrBtHfgGetMainInstance(instData)->hfgDspmOpPending = instData->index;
}

void CsrBtHfgDspmCreateOperatorsRspHandler(HfgInstance_t *instData)
{
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrDspmOperatorCreateCfm *prim = (CsrDspmOperatorCreateCfm *)((CsrBtHfgGetMainInstance(instData))->msg);
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = NULL;
    if(prim->result == CSR_RESULT_SUCCESS)
    {
        /* check we have the right operator, put it in the dsp instance*/
        if(((instData->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC) && (prim->operatorInfo[0].capability == CSR_DSPM_CAPABILITY_WBS_ENCODER)) || 
            ((instData->lastCodecUsed == CSR_BT_WBS_CVSD_CODEC) && (prim->operatorInfo[0].capability == CSR_DSPM_CAPABILITY_SCO_SEND)))
        {
            dspmInst->operatorEncoder = prim->operatorInfo[0].opid;
        }
        else
        {
            /* - TO BE DONE - */
            /* things came in in the wrong order?  What do we do?*/
        }
        if(((instData->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC) && (prim->operatorInfo[1].capability == CSR_DSPM_CAPABILITY_WBS_DECODER)) || 
            ((instData->lastCodecUsed == CSR_BT_WBS_CVSD_CODEC) && (prim->operatorInfo[1].capability == CSR_DSPM_CAPABILITY_SCO_RECEIVE)))
        {
            dspmInst->operatorDecoder = prim->operatorInfo[1].opid;
        }
        else
        {
            /* - TO BE DONE - */
            /* things came in in the wrong order?  What do we do?*/
        }

        if ((instData->remSupFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION) &&
            (CsrBtHfgGetMainInstance(instData)->locSupFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION) &&
            instData->pendingCodecNegotiation)
        {/* Always create operators before codec negotiation in case codec negotiation is supported */
            CsrBtHfgSendCodecNegMsg(instData);
        }
        else if (instData->scoHandle != CSR_SCHED_QID_INVALID)
        {/* Operators created and codec negotiation not supported: now connect */
            CsrHfgDspmAudioConnect(instData);
        }
    }
    else
    {
        CsrBtHfgGetMainInstance(instData)->hfgDspmOpPending = DSPM_NOP;
        /* The Create Operators failed - if we are attempting WBS, change the supported codecs to only CVSD, and send to AG */
        if(instData->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC)
        {
            instData->remSupportedCodecs = CSR_BT_WBS_CVSD_CODEC_MASK;
            CsrBtHfgSendCodecNegMsg(instData);
        }
        /* If we are Narrow Band, it would be worth attempting to connect without DSP, but we should probably alert the app that something is wrong */
        if(instData->lastCodecUsed == CSR_BT_WBS_CVSD_CODEC)
        {
            
        }
    }

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
void CsrHfgDspmAudioConnect(HfgInstance_t *instData)
{

    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
/* build list of sources - SCO and audio source (eg codec)*/
    CsrDspmCpInfo *cpInfo = CsrPmemZalloc(sizeof(CsrDspmCpInfo) * 4);

    CsrUint16 dspmPcmMapping = 0;
    /* If PCM slot is selected we need to give the PCM slot in the form 0x8n00, where n is the desired PCM channel*/
    if((instData->scoPcmSlot) && (instData->scoPcmSlot != 0xFF))
    {
        dspmPcmMapping = (CsrUint16)instData->scoPcmSlot;
        dspmPcmMapping = (dspmPcmMapping << 8) | 0x8000;
    }

    cpInfo[0].cpid = CSR_DSPM_CPID_SOURCE;
    cpInfo[0].cpType = CSR_DSPM_CP_TYPE_SCO;
    cpInfo[0].instance = instData->scoHandle;
    cpInfo[0].channel = 0;
    cpInfo[1].cpid = CSR_DSPM_CPID_SINK;
    cpInfo[1].cpType = CSR_DSPM_CP_TYPE_SCO;
    cpInfo[1].instance = instData->scoHandle;
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
    CsrDspmStreamCpOpenReqSend(CSR_BT_HFG_IFACEQUEUE, 4, cpInfo);
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmStreamOpenRspHandle;
}

void CsrBtHfgDspmStreamOpenRspHandle(HfgInstance_t *instData)
{
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrDspmStreamCpOpenCfm *prim = (CsrDspmStreamCpOpenCfm *)((CsrBtHfgGetMainInstance(instData))->msg);
    CsrUint8 numParams= 0;
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = NULL;
    
/* Error handling */
    if(prim->result == CSR_DSPM_RESULT_UNSUPPORTED)
    {
        /* TO BE COMPLETED*/
        /*Wrong version of chip!  
        Error handler */
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
        
    }
    else if(prim->result == CSR_RESULT_SUCCESS)
    {
        /* Put the handles into the DSPM structure */
        /* The DSPM prim contains all the information sent about each CP from the original message, 
           while they are returned in the same order as they are send, check they are correct before writing them into DSP instance*/
        CsrUint32 rateVal = 8000; /* 8 Khz per default: NB */
        CsrBtHfgStreamConfigParam *sourceConfigParams = NULL;
        CsrBtHfgStreamConfigParam *sinkConfigParams = NULL;
        CsrUint16 numSourceConfigParams = 0;
        CsrUint16 numSinkConfigParams = 0;

        if (instData->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC)
        {/* Wide-band */
            rateVal = 16000; /* 16KHz */
        }

        dspmInst->scoSourceHandle   =  prim->cpInfo[0].cpid;
        dspmInst->scoSinkHandle     =  prim->cpInfo[1].cpid;
        dspmInst->sourceHandle      =  prim->cpInfo[2].cpid;
        dspmInst->sinkHandle        =  prim->cpInfo[3].cpid;
        if (((instData->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC) && (dspmInst->sourceWbsConfigParams == NULL)) ||
            ((instData->lastCodecUsed == CSR_BT_WBS_CVSD_CODEC) && (dspmInst->sourceNbConfigParams == NULL)))
        {/* No configuration data received from the application: ensure that default configuration takes place */

            if (dspmInst->sourceType == CSR_BT_HFP_DSPM_CP_TYPE_PCM)
            {
                numSourceConfigParams = 3;
                sourceConfigParams = CsrPmemZalloc(sizeof(CsrBtHfgStreamConfigParam) * numSourceConfigParams);
                sourceConfigParams[0].CsrBtHfgConfigParamId  = 0x0101; /* PCM_MASTER_CLOCK_RATE */
                sourceConfigParams[0].CsrBtHfgConfigParamVal = 2304000; /* 48000 */
                sourceConfigParams[1].CsrBtHfgConfigParamId  = 0x0100; /* PCM_SYNC_RATE */
                sourceConfigParams[1].CsrBtHfgConfigParamVal = rateVal;
                sourceConfigParams[2].CsrBtHfgConfigParamId  = 0x0102; /* PCM_MASTER_MODE */
                sourceConfigParams[2].CsrBtHfgConfigParamVal = 0x01;   /* master */
            }
            else if (dspmInst->sourceType == CSR_BT_HFP_DSPM_CP_TYPE_CODEC)
            {
                numSourceConfigParams = 2;
                sourceConfigParams = CsrPmemZalloc(sizeof(CsrBtHfgStreamConfigParam) * numSourceConfigParams);
                sourceConfigParams[0].CsrBtHfgConfigParamId  = 0x0300; /* CODEC_ADC_RATE */
                sourceConfigParams[0].CsrBtHfgConfigParamVal = rateVal;
                sourceConfigParams[1].CsrBtHfgConfigParamId  = 0x0302; /* CODEC_ADC_GAIN */
                sourceConfigParams[1].CsrBtHfgConfigParamVal = 0x0A;
            }
            else if (dspmInst->sourceType == CSR_BT_HFP_DSPM_CP_TYPE_I2S)
            {
                numSourceConfigParams = 3;
                sourceConfigParams = CsrPmemZalloc(sizeof(CsrBtHfgStreamConfigParam) * numSourceConfigParams);
                sourceConfigParams[0].CsrBtHfgConfigParamId  = 0x0201; /* I2S_MASTER_CLOCK_RATE */
                sourceConfigParams[0].CsrBtHfgConfigParamVal = 2304000; /* 48000 */
                sourceConfigParams[1].CsrBtHfgConfigParamId  = 0x0200; /* I2S_SYNC_RATE */
                sourceConfigParams[1].CsrBtHfgConfigParamVal = rateVal;
                sourceConfigParams[2].CsrBtHfgConfigParamId  = 0x0202; /* I2S_MASTER_MODE */
                sourceConfigParams[2].CsrBtHfgConfigParamVal = 0x01;   /* master */
            }
        }
        else if (instData->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC)
        {/* WBS */
            numSourceConfigParams = dspmInst->numSourceWbsConfigParams;
            sourceConfigParams    = dspmInst->sourceWbsConfigParams;
        }
        else
        {/* NB */
            numSourceConfigParams = dspmInst->numSourceNbConfigParams;
            sourceConfigParams    = dspmInst->sourceNbConfigParams;
        }

        if (((instData->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC) && (dspmInst->sinkWbsConfigParams == NULL)) ||
            ((instData->lastCodecUsed == CSR_BT_WBS_CVSD_CODEC) && (dspmInst->sinkNbConfigParams == NULL)))
        {/* No configuration data received from the application: ensure that default configuration takes place */
            if (dspmInst->sinkType == CSR_BT_HFP_DSPM_CP_TYPE_PCM)
            {
                numSinkConfigParams   = 3;
                sinkConfigParams = CsrPmemZalloc(sizeof(CsrBtHfgStreamConfigParam) * numSinkConfigParams);
                sinkConfigParams[0].CsrBtHfgConfigParamId  = 0x0101; /* PCM_MASTER_CLOCK_RATE */
                sinkConfigParams[0].CsrBtHfgConfigParamVal = 2304000; /* 48000 */
                sinkConfigParams[1].CsrBtHfgConfigParamId  = 0x0100; /* PCM_SYNC_RATE */
                sinkConfigParams[1].CsrBtHfgConfigParamVal = rateVal;
                sinkConfigParams[2].CsrBtHfgConfigParamId  = 0x0102; /* PCM_MASTER_MODE */
                sinkConfigParams[2].CsrBtHfgConfigParamVal = 0x01;   /* master */
            }
            else if (dspmInst->sinkType == CSR_BT_HFP_DSPM_CP_TYPE_CODEC)
            {
                numSinkConfigParams   = 2;
                sinkConfigParams = CsrPmemZalloc(sizeof(CsrBtHfgStreamConfigParam) * numSinkConfigParams);
                sinkConfigParams[0].CsrBtHfgConfigParamId  = 0x0301; /* CODEC_DAC_RATE */
                sinkConfigParams[0].CsrBtHfgConfigParamVal = rateVal; 
                sinkConfigParams[1].CsrBtHfgConfigParamId  = 0x0303; /* CODEC_DAC_GAIN */
                sinkConfigParams[1].CsrBtHfgConfigParamVal = 0x0A;
            }
            else if (dspmInst->sinkType == CSR_BT_HFP_DSPM_CP_TYPE_I2S)
            {
                numSinkConfigParams = 3;
                sinkConfigParams = CsrPmemZalloc(sizeof(CsrBtHfgStreamConfigParam) * numSinkConfigParams);
                sinkConfigParams[0].CsrBtHfgConfigParamId  = 0x0201; /* I2S_MASTER_CLOCK_RATE */
                sinkConfigParams[0].CsrBtHfgConfigParamVal = 2304000; /* 48000 */
                sinkConfigParams[1].CsrBtHfgConfigParamId  = 0x0200; /* I2S_SYNC_RATE */
                sinkConfigParams[1].CsrBtHfgConfigParamVal = rateVal;
                sinkConfigParams[2].CsrBtHfgConfigParamId  = 0x0202; /* I2S_MASTER_MODE */
                sinkConfigParams[2].CsrBtHfgConfigParamVal = 0x01;   /* master */
            }
        }
        else if (instData->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC)
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
                    configInfo[i].cpid = dspmInst->scoSourceHandle;
                    configInfo[i].feature = dspmInst->scoSourceConfigParams[i].CsrBtHfgConfigParamId;
                    configInfo[i].value = dspmInst->scoSourceConfigParams[i].CsrBtHfgConfigParamVal;
                }
                
                offset += dspmInst->numScoSourceConfigParams;
                
                if((i >= offset) && (i < (offset + dspmInst->numScoSinkConfigParams)) && dspmInst->scoSinkConfigParams)
                {
                    j = i - offset;
                    configInfo[i].cpid = dspmInst->scoSinkHandle;
                    configInfo[i].feature = dspmInst->scoSinkConfigParams[j].CsrBtHfgConfigParamId;
                    configInfo[i].value = dspmInst->scoSinkConfigParams[j].CsrBtHfgConfigParamVal;
                }
                
                offset += dspmInst->numScoSinkConfigParams;
                
                if((i >= offset) && (i < (offset + numSourceConfigParams))
                     && sourceConfigParams)
                {
                    j = i - offset;
                    configInfo[i].cpid    = dspmInst->sourceHandle;
                    configInfo[i].feature = sourceConfigParams[j].CsrBtHfgConfigParamId;
                    configInfo[i].value   = sourceConfigParams[j].CsrBtHfgConfigParamVal;
                }
                
                offset += numSourceConfigParams;
                
                if ((i >= offset) && (i < (offset + numSinkConfigParams))
                     && sinkConfigParams)
                {
                    j = i - offset;
                    configInfo[i].cpid = dspmInst->sinkHandle;
                    configInfo[i].feature = sinkConfigParams[j].CsrBtHfgConfigParamId;
                    configInfo[i].value   = sinkConfigParams[j].CsrBtHfgConfigParamVal;
                }
            }
            CsrDspmStreamConfigureReqSend(CSR_BT_HFG_IFACEQUEUE, numParams, configInfo);
            CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmConfigStreamRspHandler;
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
            CsrBtHfgDspmSendConnectStreams(instData);
        }
    }
            /* Other Result IDs not handled here */
}

void CsrBtHfgDspmSendSingleConfigStream(HfgInstance_t *instData, CsrBtHfgAudioDspStreamSingleConfig *configData)
{
    CsrDspmConfigInfo *configInfo = CsrPmemZalloc(sizeof(CsrDspmConfigInfo));
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    configInfo->cpid      = dspmInst->sinkHandle;
    configInfo->feature   = configData->configPair.CsrBtHfgConfigParamId;
    configInfo->value     = configData->configPair.CsrBtHfgConfigParamVal;
    CsrDspmStreamConfigureReqSend(CSR_BT_HFG_IFACEQUEUE, 1, configInfo);
    
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmSingleConfigStreamRspHandler;
    CsrBtHfgGetMainInstance(instData)->hfgDspmOpPending = instData->index;
}

void CsrBtHfgDspmSingleConfigStreamRspHandler(HfgInstance_t *instData)
{
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = NULL;
    CsrBtHfgGetMainInstance(instData)->hfgDspmOpPending = DSPM_NOP;
}

void CsrBtHfgDspmConfigStreamRspHandler(HfgInstance_t *instData)
{
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrDspmStreamConfigureCfm *prim = (CsrDspmStreamConfigureCfm *)((CsrBtHfgGetMainInstance(instData))->msg);
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = NULL;

    /*Check the config was successful, then Create Operators */
    /*if (prim->result == CSR_RESULT_SUCCESS)
    {*/
        /*We have CP, all is configured, connect streams!*/
        CsrBtHfgDspmSendConnectStreams(instData);
    /*}
    else
    {*/
        /* - TO BE DONE - */
        /*Config failed - this is Application supplied configuration, we need to inform the app, and send new parameters before continuing - or what?*/
    /*}*/

    CsrPmemFree(dspmInst->scoSourceConfigParams);
    dspmInst->scoSourceConfigParams     = NULL;
    dspmInst->numScoSourceConfigParams  = 0;
    CsrPmemFree(dspmInst->scoSinkConfigParams);
    dspmInst->scoSinkConfigParams       = NULL;
    dspmInst->numScoSinkConfigParams    = 0;
    CsrPmemFree(dspmInst->sourceWbsConfigParams);
    dspmInst->sourceWbsConfigParams        = NULL;
    dspmInst->numSourceWbsConfigParams     = 0;
    CsrPmemFree(dspmInst->sourceNbConfigParams);
    dspmInst->sourceNbConfigParams         = NULL;
    dspmInst->numSourceNbConfigParams      = 0;
    CsrPmemFree(dspmInst->sinkWbsConfigParams);
    dspmInst->sinkWbsConfigParams          = NULL;
    dspmInst->numSinkWbsConfigParams       = 0;
    CsrPmemFree(dspmInst->sinkNbConfigParams);
    dspmInst->sinkNbConfigParams           = NULL;
    dspmInst->numSinkNbConfigParams        = 0;
}

void CsrBtHfgDspmSendConnectStreams(HfgInstance_t *instData)
{
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrUint8 hfDspmNumConnections = 4;
    CsrDspmConnectionInfo *connectionInfo;
    /* Check if we have some operators for the DSP - ie that Create Operators was successful */
    if(dspmInst->operatorDecoder != HFG_DSPM_DEFAULT_HANDLE)
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
        if(instData->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC)
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
    CsrDspmConnectionCreateReqSend(CSR_BT_HFG_IFACEQUEUE, hfDspmNumConnections, connectionInfo);
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmConnectionCreateRspHandler;
}

void CsrBtHfgDspmConnectionCreateRspHandler(HfgInstance_t *instData)
{
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrDspmConnectionCreateCfm *prim = (CsrDspmConnectionCreateCfm *)((CsrBtHfgGetMainInstance(instData))->msg);
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = NULL;

    if((prim->result == CSR_RESULT_SUCCESS)&&(prim->connectionInfoCount == 4))
    {
        dspmInst->connIdDspAudio = prim->connectionInfo[0].cid;
        dspmInst->connIdAudioDsp = prim->connectionInfo[1].cid;
        dspmInst->connIdScoDsp   = prim->connectionInfo[2].cid;
        dspmInst->connIdDspSco   = prim->connectionInfo[3].cid;
        /* Now start the operators if any */
        if ((dspmInst->operatorEncoder != HFG_DSPM_DEFAULT_HANDLE) || (dspmInst->operatorDecoder != HFG_DSPM_DEFAULT_HANDLE))
        {
            CsrDspmOpid *opId = CsrPmemZalloc(sizeof(CsrDspmOpid) * 2);
            opId[0] = dspmInst->operatorEncoder;
            opId[1] = dspmInst->operatorDecoder;
            CsrDspmOperatorStartReqSend(CSR_BT_HFG_IFACEQUEUE, 2,opId);
            CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmOperatorStartRspHandler;
        }
        else
        {
            CsrBtHfgGetMainInstance(instData)->hfgDspmOpPending = DSPM_NOP;
        }
    }
    else
    {
        CsrBtHfgGetMainInstance(instData)->hfgDspmOpPending = DSPM_NOP;
    }
    
}

void CsrBtHfgDspmOperatorStartRspHandler(HfgInstance_t *instData)
{
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrDspmOperatorStartCfm *prim = (CsrDspmOperatorStartCfm *)((CsrBtHfgGetMainInstance(instData))->msg);
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = NULL;
    CsrBtHfgGetMainInstance(instData)->hfgDspmOpPending = DSPM_NOP;
    /* No point in checking the result code? What to do if the allocated and connected operator cannot be started??? */
}

/* DSPM audio disconnection helper functions */
void CsrBtHfgDspmSendDisconnectStreams(HfgInstance_t *instData)
{
    /* This means the following steps:
    1. Stop operators 
    2. Disconect streams 
    3. Destroy operators
    4. Close stream CPs
    */
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrUint8 hfCount = 2;

    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = NULL;

    if ((dspmInst->operatorEncoder != HFG_DSPM_DEFAULT_HANDLE) || (dspmInst->operatorDecoder != HFG_DSPM_DEFAULT_HANDLE))
    {/* Stop operators */
        CsrDspmOpid *opId = CsrPmemZalloc(hfCount * sizeof(CsrDspmOpid));
        opId[0] = dspmInst->operatorEncoder;
        opId[1] = dspmInst->operatorDecoder;
        CsrDspmOperatorStopReqSend(CSR_BT_HFG_IFACEQUEUE, hfCount,opId);
        CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmOperatorStopRspHandler;
    }
    else if ((dspmInst->connIdScoDsp != HFG_DSPM_DEFAULT_HANDLE) || (dspmInst->connIdDspSco != HFG_DSPM_DEFAULT_HANDLE))
    {/* Disconnect streams */
        CsrDspmCid *cid;
        hfCount = 4;
        cid = CsrPmemZalloc(hfCount * sizeof(CsrDspmCid));
        cid[0] = dspmInst->connIdDspAudio;
        cid[1] = dspmInst->connIdAudioDsp;
        cid[2] = dspmInst->connIdScoDsp;
        cid[3] = dspmInst->connIdDspSco;
        CsrDspmConnectionDestroyReqSend(CSR_BT_HFG_IFACEQUEUE, hfCount, cid);
        CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmConnectionDestroyRspHandler;
    }
    else 
    {/* There was no audio connection and no operators at all; just de-allocate the connection points allocated... if any! */
        if ((dspmInst->sourceHandle != HFG_DSPM_DEFAULT_HANDLE) || (dspmInst->sinkHandle != HFG_DSPM_DEFAULT_HANDLE))
        {/* source allocated: de-allocate it */
            CsrDspmCid *cid = CsrPmemZalloc(hfCount * sizeof(CsrDspmCid));
            cid[0] = dspmInst->sourceHandle;
            cid[1] = dspmInst->sinkHandle;
            CsrDspmStreamCpCloseReqSend(CSR_BT_HFG_IFACEQUEUE, hfCount, cid);
            CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmStreamCpCloseRspHandler;
        }
    }

    if (CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack != NULL)
    {
        CsrBtHfgGetMainInstance(instData)->hfgDspmOpPending = instData->index;
    }
}

void CsrBtHfgDspmOperatorStopRspHandler(HfgInstance_t *instData)
{
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrDspmOperatorStopCfm *prim = (CsrDspmOperatorStopCfm *)((CsrBtHfgGetMainInstance(instData))->msg);
    CsrUint8 hfCount = 2;
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = NULL;
    
    if ((dspmInst->connIdScoDsp != HFG_DSPM_DEFAULT_HANDLE) || (dspmInst->connIdDspSco != HFG_DSPM_DEFAULT_HANDLE))
    {
        CsrDspmCid *cid;
        hfCount = 4;
        cid = CsrPmemZalloc(hfCount * sizeof(CsrDspmCid));
        cid[0] = dspmInst->connIdDspAudio;
        cid[1] = dspmInst->connIdAudioDsp;
        cid[2] = dspmInst->connIdScoDsp;
        cid[3] = dspmInst->connIdDspSco;
        CsrDspmConnectionDestroyReqSend(CSR_BT_HFG_IFACEQUEUE, hfCount, cid);
        CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmConnectionDestroyRspHandler;
    }
    else
    {/* No streams existed: just destroy the operators */
        CsrDspmOpid *opId = CsrPmemZalloc(hfCount * sizeof(CsrDspmOpid));
        opId[0] = dspmInst->operatorEncoder;
        opId[1] = dspmInst->operatorDecoder;
        CsrDspmOperatorDestroyReqSend(CSR_BT_HFG_IFACEQUEUE, hfCount,opId);
        CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmOperatorDestroyRspHandler;
    }
}

void CsrBtHfgDspmConnectionDestroyRspHandler(HfgInstance_t *instData)
{
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrDspmConnectionDestroyCfm *prim = (CsrDspmConnectionDestroyCfm *)((CsrBtHfgGetMainInstance(instData))->msg);
    CsrUint8 hfCount = 2;
    
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = NULL;

    dspmInst->connIdScoDsp    = HFG_DSPM_DEFAULT_HANDLE;
    dspmInst->connIdDspSco    = HFG_DSPM_DEFAULT_HANDLE;
    dspmInst->scoSourceHandle = HFG_DSPM_DEFAULT_HANDLE;
    dspmInst->scoSinkHandle   = HFG_DSPM_DEFAULT_HANDLE;

    if ((dspmInst->operatorEncoder != HFG_DSPM_DEFAULT_HANDLE) || (dspmInst->operatorDecoder != HFG_DSPM_DEFAULT_HANDLE))
    {
        CsrDspmOpid *opId = CsrPmemZalloc(hfCount * sizeof(CsrDspmOpid));
        opId[0] = dspmInst->operatorEncoder;
        opId[1] = dspmInst->operatorDecoder;
        CsrDspmOperatorDestroyReqSend(CSR_BT_HFG_IFACEQUEUE, hfCount,opId);
        CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmOperatorDestroyRspHandler;
    }
    else
    {/* There were no operators: just close the stream connection points */
        CsrDspmCid *cid = CsrPmemZalloc(hfCount * sizeof(CsrDspmCid));
        cid[0] = dspmInst->sourceHandle;
        cid[1] = dspmInst->sinkHandle;
        CsrDspmStreamCpCloseReqSend(CSR_BT_HFG_IFACEQUEUE, hfCount, cid);
        CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmStreamCpCloseRspHandler;
    }
}


void CsrBtHfgDspmOperatorDestroyRspHandler(HfgInstance_t *instData)
{
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrDspmOperatorDestroyCfm *prim = (CsrDspmOperatorDestroyCfm *)((CsrBtHfgGetMainInstance(instData))->msg);
    CsrUint8 hfCount = 2;
    CsrDspmCid *cid = CsrPmemZalloc(hfCount * sizeof(CsrDspmCid));
    
    dspmInst->operatorEncoder = HFG_DSPM_DEFAULT_HANDLE;
    dspmInst->operatorDecoder = HFG_DSPM_DEFAULT_HANDLE;
    
    cid[0] = dspmInst->sourceHandle;
    cid[1] = dspmInst->sinkHandle;
    CsrDspmStreamCpCloseReqSend(CSR_BT_HFG_IFACEQUEUE, hfCount, cid);
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = CsrBtHfgDspmStreamCpCloseRspHandler;
}

void CsrBtHfgDspmStreamCpCloseRspHandler(HfgInstance_t *instData)
{
    HfgDspm_t *dspmInst = (HfgDspm_t *)&instData->dspmInst;
    CsrDspmStreamCpCloseCfm *prim = (CsrDspmStreamCpCloseCfm *)((CsrBtHfgGetMainInstance(instData))->msg);
    CsrBtHfgGetMainInstance(instData)->dspmHfgCallBack = NULL;
    CsrBtHfgGetMainInstance(instData)->hfgDspmOpPending = DSPM_NOP;
    dspmInst->sourceHandle = HFG_DSPM_DEFAULT_HANDLE;
    dspmInst->sinkHandle   = HFG_DSPM_DEFAULT_HANDLE;
}

#endif /*CSR_USE_DSPM*/
