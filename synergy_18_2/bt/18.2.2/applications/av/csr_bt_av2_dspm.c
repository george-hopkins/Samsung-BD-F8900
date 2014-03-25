#ifdef CSR_USE_DSPM
/****************************************************************************

       Copyright Cambridge Silicon Radio Limited and its licensors 2012. 
               All rights reserved.

REVISION:      $Revision: #7 $
****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_dspm_lib.h"
#include "csr_dspm_prim.h"
#include "csr_bt_av2_dspm.h"


#define CSR_APTX_ATU_SIZE         448

void CsrAv2DspmActivateRspHandler(av2instance_t *instData)
{/* Registered to the DSPM */
    instData->av2DspmCallBack = NULL;
}


void CsrBtAv2DspmSendCreateOperator(av2instance_t *instData)
{
    CsrDspmOperatorInfo *operatorInfo = CsrPmemZalloc(sizeof(CsrDspmOperatorInfo));
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;

    dspmInst->operatorEncoder = CSR_BT_AV2_UNKNOWN_ID;
    operatorInfo->opid = CSR_DSPM_OPID_INVALID;
    operatorInfo->capability = dspmInst->encoderToUse;
    
    CsrDspmOperatorCreateReqSend(instData->CsrSchedQid, 1, operatorInfo);
    instData->av2DspmCallBack = CsrBtAv2DspmCreateOperatorsRspHandler;
}

void CsrBtAv2DspmCreateOperatorsRspHandler(av2instance_t *instData)
{
    CsrDspmOperatorCreateCfm *prim = (CsrDspmOperatorCreateCfm *)instData->recvMsgP;

    if (prim->result == CSR_RESULT_SUCCESS)
    {
        instData->avCon[instData->currentConnection].avDspmInst.operatorEncoder = prim->operatorInfo->opid;
        /* Now the encoder operator is created: if the needed information is available, create the connection! */
        if (instData->avCon[instData->currentConnection].avDspmInst.remoteCid != CSR_BT_AV2_UNKNOWN_ID)
        {
            CsrAv2DspmStreamConnect(instData);
        }
    }
}


/*  Start the DSP Manager Stream Connection sequence.  */
void CsrAv2DspmStreamConnect(av2instance_t *instData)
{/* If no encoder operator exists, allocate one
    Then, allocate shunt and PCM source connection points.
    Then configure the shunt and PCM endpoints allocated
    Finally connect the streams appropriately and start the operator
    when all connections are in place   */
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
    if (dspmInst->operatorEncoder == CSR_BT_AV2_UNKNOWN_ID)
    {
        CsrBtAv2DspmSendCreateOperator(instData);
    }
    else
    {/* The operator is already allocated: go on for the shunt and PCM connection points */
        CsrDspmCpInfo *cpInfo = CsrPmemZalloc(sizeof(CsrDspmCpInfo) * 3);

        cpInfo[0].cpid = CSR_DSPM_CPID_SINK;      /* SINK */
        cpInfo[0].cpType = CSR_DSPM_CP_TYPE_A2DP; /* SHUNT */
        cpInfo[0].instance = dspmInst->aclHandle; /* ACL ID */
        cpInfo[0].channel = dspmInst->remoteCid; /* Local CID of the connection */
    /*  
        for different CP types, there are different numbers of instances and channels
        Eg, Codec typically has 1 instance, 2 channels, 
        I2S can have 2 instances with 2 channels
        PCM can have more than 1 instance, and has n channels
    */
        if (dspmInst->endPointType == 0)
        {
            dspmInst->endPointType = instData->nextEndPointType;
        }

        /* Left side PCM */
        cpInfo[1].cpid = CSR_DSPM_CPID_SOURCE;
        cpInfo[1].cpType = dspmInst->endPointType; 
        cpInfo[1].instance = dspmInst->pcmInstance;
        cpInfo[1].channel = CSR_BT_AV2_DSPM_LEFT_CHANNEL;
        /* Right side PCM */
        cpInfo[2].cpid = CSR_DSPM_CPID_SOURCE;
        cpInfo[2].cpType = dspmInst->endPointType; 
        cpInfo[2].instance = dspmInst->pcmInstance;
        cpInfo[2].channel = CSR_BT_AV2_DSPM_RIGHT_CHANNEL;

        if (dspmInst->endPointType == CSR_DSPM_CP_TYPE_PCM)
        {/* If PCM is selected we need to give the PCM slot in the form 0x8n00, where n is the desired PCM channel*/
            cpInfo[1].channel = (CsrUint16)(CSR_BT_AV2_DSPM_LEFT_CHANNEL << 8) | 0x8000;
            cpInfo[2].channel = (CsrUint16)(CSR_BT_AV2_DSPM_RIGHT_CHANNEL << 8) | 0x8000;
        }

        CsrDspmStreamCpOpenReqSend(instData->CsrSchedQid, 3, cpInfo);
        instData->av2DspmCallBack = CsrBtAv2DspmStreamOpenRspHandle;
    }
    
}

void CsrBtAv2DspmStreamOpenRspHandle(av2instance_t *instData)
{/* Configuration always needed at least for the shunt endpoint when it is allocated!*/
    CsrDspmStreamCpOpenCfm *prim = (CsrDspmStreamCpOpenCfm *)instData->recvMsgP;
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
    instData->av2DspmCallBack = NULL;

    if(prim->result == CSR_RESULT_SUCCESS)
    {
        avrouter_instance_t *filter = (avrouter_instance_t *)(instData->filter_instance[instData->cur_filter]);
        CsrUint8 numConfigParams;
        CsrDspmConfigInfo *configInfo;


        if (dspmInst->endPointType   == CSR_DSPM_CP_TYPE_PCM)
        {
            if (dspmInst->pcmRightHandle == dspmInst->pcmLeftHandle)
            {
                numConfigParams = 4;
            }
            else
            {
                numConfigParams = 5;
            }
        }
        else
        {/* codec */
            numConfigParams = 7;
        }

        configInfo = CsrPmemZalloc(sizeof(CsrDspmConfigInfo) * numConfigParams);

        /* Put the handles into the DSPM structure */
        /* The DSPM prim contains all the information sent about each CP from the original message, 
           while they are returned in the same order as they are send, check they are correct before writing them into DSP instance*/
        dspmInst->shuntId            =  prim->cpInfo[0].cpid;
        dspmInst->pcmLeftHandle      =  prim->cpInfo[1].cpid;
        dspmInst->pcmRightHandle     =  prim->cpInfo[2].cpid;

        if (dspmInst->encoderToUse  == CSR_DSPM_CAPABILITY_SBC_ENCODER)
        {
            dspmInst->frame_size      = filter->framesize;
            dspmInst->framesPerPacket = (instData->maxFrameSize - CSR_BT_AV2_DSPM_RTP_HEADER_LENGTH) / dspmInst->frame_size;
        }
        else
        {/* APT-X */
            dspmInst->frame_size      = filter->framesize;
            dspmInst->framesPerPacket = 1;
        }
        
        dspmInst->num_tokens      = CSR_BT_AV2_DSPM_DEFAULT_NUM_TOKENS; 

        /* SHUNT configuration */
        configInfo[0].cpid      = dspmInst->shuntId;
        configInfo[0].feature   = CSR_BT_AV2_DSPM_ATU_SIZE;
        if (dspmInst->encoderToUse  == CSR_DSPM_CAPABILITY_SBC_ENCODER)
        {
            configInfo[0].value     = (dspmInst->framesPerPacket * dspmInst->frame_size) + CSR_BT_AV2_DSPM_RTP_HEADER_LENGTH;
        }
        else
        {
            configInfo[0].value     = CSR_APTX_ATU_SIZE;
        }
        
        configInfo[1].cpid      = dspmInst->shuntId;
        configInfo[1].feature   = CSR_BT_AV2_DSPM_AUTOFLUSH;
        configInfo[1].value     = 1; /* enabled */
        
        configInfo[2].cpid      = dspmInst->shuntId;
        configInfo[2].feature   = CSR_BT_AV2_DSPM_NUM_TOKENS;
        configInfo[2].value     = dspmInst->num_tokens;

        if (dspmInst->endPointType   == CSR_DSPM_CP_TYPE_CODEC)
        {/* right side */
            configInfo[3].cpid      = dspmInst->pcmRightHandle;
            configInfo[3].feature   = 0x300; /* ADC sample rate */
            configInfo[3].value     = filter->sample_freq; /* value */
        
            configInfo[4].cpid      = dspmInst->pcmRightHandle;
            configInfo[4].feature   = 0x302; /* ADC gain */
            configInfo[4].value     = 0x00f; /* value: must be between 0 and 0x16 */
        
            /* left side */
            configInfo[5].cpid      = dspmInst->pcmLeftHandle;
            configInfo[5].feature   = 0x300; /* ADC sample rate */
            configInfo[5].value     = filter->sample_freq; /* value */
        
            configInfo[6].cpid      = dspmInst->pcmLeftHandle;
            configInfo[6].feature   = 0x302; /* ADC gain */
            configInfo[6].value     = 0x00f; /* value: must be between 0 and 0x16 */
        }
        else
        {/* PCM */
            /* right side */
            configInfo[3].cpid      = dspmInst->pcmRightHandle;
            configInfo[3].feature   = 0x0100; /* PCM sync rate */
            configInfo[3].value     = filter->sample_freq; /* value */
            
            if (dspmInst->pcmRightHandle != dspmInst->pcmLeftHandle)
            {/* left side */
                configInfo[4].cpid      = dspmInst->pcmLeftHandle;
                configInfo[4].feature   = 0x0100; /* PCM sync rate */
                configInfo[4].value     = filter->sample_freq; /* value */
            }
        }

        CsrDspmStreamConfigureReqSend(instData->CsrSchedQid, numConfigParams, configInfo);
        instData->av2DspmCallBack = CsrBtAv2DspmConfigStreamRspHandler;
    }
    else
    {
        /* Error handling */
        if ((prim->result == CSR_DSPM_RESULT_UNSUPPORTED) || (prim->result == CSR_DSPM_RESULT_UNAVAILABLE))
        {
            /* TO BE COMPLETED*/
            /* Wrong version of chip!  or PCM slot already in use!
               Error handler */
        
        } 
    }
    /* Other Result IDs not handled here */
}


static CsrUint16 *CsrBtCreateSbcSettingsOpMsg(av2instance_t *instData)
{
    avrouter_instance_t *filter = (avrouter_instance_t *)(instData->filter_instance[instData->cur_filter]);
    CsrUint16 *message          = CsrPmemZalloc(7 * sizeof(CsrUint16));
    
    message[0]    = CSR_BT_AV2_DSPM_OP_MESSAGE_SBC_SETTINGS;
    message[1]    = filter->subbands;
    message[2]    = filter->blocks;
    message[3]    = filter->bitPool; 
    /*message[3]    = filter->maxBitpool;*/
    switch(filter->sample_freq)
    {
        case 48000:
            message[4]    = 3;
            break;
        case 44100:
            message[4]    = 2;
            break;
        case 32000:
            message[4]    = 1;
            break;
        case 16000:
        default:
            message[4]    = 0;
            break;
    }

    message[5]    = filter->channel_mode;
    message[6]    = filter->alloc_method;

    return message;

}

static CsrUint16 *CsrBtCreateATUSizeOpMsg(AvDspmType *dspmInst)
{
    CsrUint16 *message   = CsrPmemZalloc(2* sizeof(CsrUint16));
    message[0]    = CSR_BT_AV2_DSPM_OP_MESSAGE_ATU_SIZE;
    if (dspmInst->encoderToUse  == CSR_DSPM_CAPABILITY_SBC_ENCODER)
    {
        message[1]    = (dspmInst->framesPerPacket * dspmInst->frame_size) + CSR_BT_AV2_DSPM_RTP_HEADER_LENGTH; 
    }
    else
    {/* APT-X */
        message[1]    = CSR_APTX_ATU_SIZE;
    }
    
    return message;
}

/*static CsrUint16 *CsrBtCreateAptXFadeMsg(AvDspmType *dspmInst, CsrBool enable)
{
    CsrUint16 *message   = CsrPmemZalloc(sizeof(CsrUint16));

    if (enable)
    {
        message[0]    = CSR_BT_AV2_DSPM_OP_MESSAGE_ENABLE_FADE_OUT;
    }
    else
    {
        message[0]    = CSR_BT_AV2_DSPM_OP_MESSAGE_DISABLE_FADE_OUT;
    }
    
    
    return message;
}*/

void CsrBtAv2DspmConfigStreamRspHandler(av2instance_t *instData)
{/* All configured: connect the streams*/
    CsrDspmStreamConfigureCfm *prim = (CsrDspmStreamConfigureCfm *)instData->recvMsgP;
    
    instData->av2DspmCallBack = NULL;
   
    /*Check the config was successful, then Create Operators */
    if (prim->result == CSR_RESULT_SUCCESS)
    {/*We have CP, all configured, send the messages to the operator and connect streams!*/
        AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
        CsrUint8 avDspmNumMessages = 2;
        CsrDspmOperatorMessageInfo *operatorMessageInfo;
        
        
        if (dspmInst->encoderToUse  == CSR_DSPM_CAPABILITY_SBC_ENCODER)
        {
            operatorMessageInfo = CsrPmemZalloc(sizeof(CsrDspmOperatorMessageInfo) * avDspmNumMessages);
            operatorMessageInfo[0].opid              = dspmInst->operatorEncoder;
            operatorMessageInfo[0].messageLength     = 7; /* msg ID, subbands, block length, bitpool size, sample freq, channel mode and allocation method */
            operatorMessageInfo[0].message           = CsrBtCreateSbcSettingsOpMsg(instData);
        
            operatorMessageInfo[1].opid              = dspmInst->operatorEncoder;
            operatorMessageInfo[1].messageLength     = 2;
            operatorMessageInfo[1].message           = CsrBtCreateATUSizeOpMsg(dspmInst);
        }
        else
        {/* APT-X */
            avDspmNumMessages = 1;
            operatorMessageInfo = CsrPmemZalloc(sizeof(CsrDspmOperatorMessageInfo) * avDspmNumMessages);
            operatorMessageInfo[0].opid              = dspmInst->operatorEncoder;
            operatorMessageInfo[0].messageLength     = 2;
            operatorMessageInfo[0].message           = CsrBtCreateATUSizeOpMsg(dspmInst);
            /*
            operatorMessageInfo[1].opid              = dspmInst->operatorEncoder;
            operatorMessageInfo[1].messageLength     = 1;
            operatorMessageInfo[1].message           = CsrBtCreateAptXFadeMsg(dspmInst,TRUE);*/
        }
        
        CsrDspmOperatorMessageReqSend(instData->CsrSchedQid, avDspmNumMessages, operatorMessageInfo);
        instData->av2DspmCallBack = CsrBtAv2DspmStreamOpMsgRspHandler;
    }
    /* else
    {
        / * - TO BE DONE ---- WHAT??? * /
        
    }*/
}

void CsrBtAv2DspmStreamOpMsgRspHandler(av2instance_t *instData)
{
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
    CsrDspmOperatorMessageCfm *prim = (CsrDspmOperatorMessageCfm *)instData->recvMsgP;

    if (prim->result == CSR_RESULT_SUCCESS)
    {
        CsrBtAv2DspmSendConnectStreams(instData);
    }
    /* else
    {
        / * - TO BE DONE ---- WHAT??? * /
        
    }*/
}

void CsrBtAv2DspmSendConnectStreams(av2instance_t *instData)
{
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
    CsrUint8 avDspmNumConnections;
    CsrDspmConnectionInfo *connectionInfo;
    CsrUint8 i = 0;
    /* At this point it is assumed that the Create Operators was successful */

    if (dspmInst->pcmRightHandle != dspmInst->pcmLeftHandle)
    {
        avDspmNumConnections = 3;
    }
    else
    {
        avDspmNumConnections = 2;
    }

    connectionInfo = CsrPmemZalloc(sizeof(CsrDspmConnectionInfo) * avDspmNumConnections);

    /* Left PCM data to SBC encoder sink*/
    connectionInfo[i].cid        = CSR_DSPM_CID_INVALID;
    connectionInfo[i].cpidSource = dspmInst->pcmLeftHandle;
    connectionInfo[i].cpidSink   = CSR_DSPM_CPID_OPERATOR_SINK(dspmInst->operatorEncoder, 0);
    i++;

    if (dspmInst->pcmRightHandle != dspmInst->pcmLeftHandle)
    {/* Right PCM data to SBC encoder sink*/
        connectionInfo[i].cid        = CSR_DSPM_CID_INVALID;
        connectionInfo[i].cpidSource = dspmInst->pcmRightHandle;
        connectionInfo[i].cpidSink   = CSR_DSPM_CPID_OPERATOR_SINK(dspmInst->operatorEncoder, 1);
        i++;
    }
    /* SBC encoder source to shunt */
    connectionInfo[i].cid        = CSR_DSPM_CID_INVALID;
    connectionInfo[i].cpidSource = CSR_DSPM_CPID_OPERATOR_SOURCE(dspmInst->operatorEncoder, 0);
    connectionInfo[i].cpidSink   = dspmInst->shuntId; 
    
    
    CsrDspmConnectionCreateReqSend(instData->CsrSchedQid, avDspmNumConnections, connectionInfo);
    instData->av2DspmCallBack = CsrBtAv2DspmConnectionCreateRspHandler;
}

void CsrAv2DspmStreamReadyToStart(av2instance_t *instData)
{
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
    if (dspmInst->readyToStart == FALSE)
    {/* The filter has received the order to start the stream: make sure to start the operators if already set-up
        or to allow the operators to start when the DSP is set-up */
        dspmInst->readyToStart = TRUE;

        if ((dspmInst->operatorEncoder != CSR_BT_AV2_UNKNOWN_ID) &&
            (dspmInst->connIdLeftEncoder != CSR_BT_AV2_UNKNOWN_ID))
        {/* There is an operator and a connection to it: just start the operator */
            CsrDspmOpid *opId    = CsrPmemAlloc(sizeof(CsrDspmOpid));
            CsrMemCpy(opId, &dspmInst->operatorEncoder, sizeof(CsrDspmOpid));
            /* Now start the operator */   
            CsrDspmOperatorStartReqSend(instData->CsrSchedQid, 1, opId);
            instData->av2DspmCallBack = CsrBtAv2DspmOperatorStartRspHandler;
            dspmInst->readyToStart = FALSE;
        }
    }

}

void CsrBtAv2DspmConnectionCreateRspHandler(av2instance_t *instData)
{/* All connections in place: store the connection IDs received and start the operator so the audio stream can start */
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
    CsrDspmConnectionCreateCfm *prim = (CsrDspmConnectionCreateCfm *)instData->recvMsgP;
    CsrUint8 numConnections = 3;

    instData->av2DspmCallBack = NULL;
    if (dspmInst->pcmRightHandle == dspmInst->pcmLeftHandle)
    {
        numConnections = 2;
    }
    if((prim->result == CSR_RESULT_SUCCESS) && (prim->connectionInfoCount == numConnections))
    {
        
        CsrUint8 i = 0;
        
        dspmInst->connIdLeftEncoder  = prim->connectionInfo[i].cid;
        i++;
        if (dspmInst->pcmRightHandle != dspmInst->pcmLeftHandle)
        {
            dspmInst->connIdRightEncoder = prim->connectionInfo[i].cid;
            i++;
        }
        dspmInst->connIdEncoderShunt = prim->connectionInfo[i].cid;

        if (dspmInst->readyToStart)
        {
            CsrDspmOpid *opId    = CsrPmemAlloc(sizeof(CsrDspmOpid));
            CsrMemCpy(opId, &dspmInst->operatorEncoder, sizeof(CsrDspmOpid));
            /* Now start the operator */   
            CsrDspmOperatorStartReqSend(instData->CsrSchedQid, 1, opId);
            instData->av2DspmCallBack = CsrBtAv2DspmOperatorStartRspHandler;
            dspmInst->readyToStart = FALSE;
        }
    }
/*    else
    {
        / * Do what??? * /
    }*/
    
}


void CsrBtAv2DspmOperatorStartRspHandler(av2instance_t *instData)
{
    CsrDspmOperatorStartCfm *prim = (CsrDspmOperatorStartCfm *)instData->recvMsgP;
    instData->av2DspmCallBack = NULL;
    /* No point in checking the result code? What to do if the allocated and connected operator cannot be started??? */
}

/* DSPM audio disconnection helper functions */
void CsrBtAv2DspmSendDisconnectStreams(av2instance_t *instData)
{
    /* This means the following steps:
    1. Stop operators 
    2. Disconect streams 
    3. Destroy operators
    4. Close stream CPs
    */
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
    CsrDspmOpid *opId    = CsrPmemAlloc(sizeof(CsrDspmOpid));
    CsrMemCpy(opId, &dspmInst->operatorEncoder, sizeof(CsrDspmOpid));
    CsrDspmOperatorStopReqSend(instData->CsrSchedQid, 1, opId);
    instData->av2DspmCallBack = CsrBtAv2DspmOperatorStopRspHandler;
}


void CsrBtAv2DspmOperatorStopRspHandler(av2instance_t *instData)
{
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
    CsrDspmOperatorStopCfm *prim = (CsrDspmOperatorStopCfm *)instData->recvMsgP;
    CsrDspmCid *cid;
    CsrUint8 numConnections = 3;

    if (dspmInst->pcmRightHandle == dspmInst->pcmLeftHandle)
    {
        numConnections = 2;
    }
    
    cid = CsrPmemZalloc(numConnections * sizeof(CsrDspmCid));
    /* No need to check for the result... if the stop operation failed, the operator will be automatically stopped 
       when the connection is released */
    
    cid[0] = dspmInst->connIdLeftEncoder;
    if (dspmInst->pcmRightHandle == dspmInst->pcmLeftHandle)
    {
        cid[1] = dspmInst->connIdEncoderShunt;
    }
    else
    {
        cid[1] = dspmInst->connIdRightEncoder;
        cid[2] = dspmInst->connIdEncoderShunt;
    }
    CsrDspmConnectionDestroyReqSend(instData->CsrSchedQid, numConnections, cid);
    instData->av2DspmCallBack = CsrBtAv2DspmConnectionDestroyRspHandler;
}

void CsrBtAv2DspmConnectionDestroyRspHandler(av2instance_t *instData)
{
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
    CsrDspmConnectionDestroyCfm *prim = (CsrDspmConnectionDestroyCfm *)instData->recvMsgP;
    CsrDspmOpid *opId    = CsrPmemAlloc(sizeof(CsrDspmOpid));
    CsrMemCpy(opId, &dspmInst->operatorEncoder, sizeof(CsrDspmOpid));
    
    dspmInst->connIdLeftEncoder     = CSR_BT_AV2_UNKNOWN_ID;
    dspmInst->connIdRightEncoder    = CSR_BT_AV2_UNKNOWN_ID;
    dspmInst->connIdEncoderShunt    = CSR_BT_AV2_UNKNOWN_ID;

    CsrDspmOperatorDestroyReqSend(instData->CsrSchedQid, 1, opId);
    instData->av2DspmCallBack = CsrBtAv2DspmOperatorDestroyRspHandler;
}

void CsrBtAv2DspmOperatorDestroyRspHandler(av2instance_t *instData)
{
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
    CsrDspmOperatorDestroyCfm *prim = (CsrDspmOperatorDestroyCfm *)instData->recvMsgP;
    CsrUint8 avCount = 2;
    CsrDspmCid *cid = CsrPmemZalloc(avCount * sizeof(CsrDspmCid));
        
    dspmInst->operatorEncoder = CSR_BT_AV2_UNKNOWN_ID;

    cid[0] = dspmInst->pcmLeftHandle;
    cid[1] = dspmInst->pcmRightHandle;
    CsrDspmStreamCpCloseReqSend(instData->CsrSchedQid, avCount, cid);
    instData->av2DspmCallBack = CsrBtAv2DspmStreamCpCloseRspHandler;
}

void CsrBtAv2DspmStreamCpCloseRspHandler(av2instance_t *instData)
{
    AvDspmType *dspmInst = (AvDspmType *)&instData->avCon[instData->currentConnection].avDspmInst;
    CsrDspmStreamCpCloseCfm *prim = (CsrDspmStreamCpCloseCfm *)instData->recvMsgP;
    
    instData->av2DspmCallBack = NULL;
    dspmInst->pcmLeftHandle    = CSR_BT_AV2_UNKNOWN_ID;
    dspmInst->pcmRightHandle   = CSR_BT_AV2_UNKNOWN_ID;
}

#endif /*CSR_USE_DSPM*/
