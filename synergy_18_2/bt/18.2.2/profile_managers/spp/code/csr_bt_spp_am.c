/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_util.h"
#include "csr_am_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#include "csr_bt_spp_main.h"
#include "csr_bt_spp_prim.h"
#include "csr_bt_spp_sef.h"
#include "csr_env_prim.h"

#ifdef CSR_BT_INSTALL_SPP_EXTENDED

void CsrSppConnectStream(SppInstanceData_t *inst, CsrAmAudioType audioType, CsrUint8 pcmSlot, CsrUint16 escoHandle)
{
    /* Set up the audio stream (mono) */
    CsrEndpointType  source, sourceR;
    CsrAmEndPointDefType  *sinkId, *tmpSinkId;

    CSR_UNUSED(escoHandle);

    CsrMemSet(&sourceR,0,sizeof(CsrEndpointType));

    source.endPoint.endPoint =  CSR_AM_ENDPOINT_PCM;
    source.endPoint.instance =  CSR_AM_INSTANCE_0;
    source.endPoint.channel  =  pcmSlot;
    source.configDataCount   =  0;
    source.configData        =  NULL;
    

    sinkId = CsrPmemAlloc(sizeof(CsrAmEndPointDefType));

    
    tmpSinkId = (CsrAmEndPointDefType  *)&(sinkId[0]);
    tmpSinkId->endPoint = CSR_AM_ENDPOINT_PCM;
    tmpSinkId->instance = CSR_AM_INSTANCE_0;
    tmpSinkId->channel = pcmSlot;
    
    CsrAmAudioPathConnectReqSend(CSR_BT_SPP_IFACEQUEUE, source, sourceR, 1, sinkId, 0, NULL, audioType, FALSE);
}

/********************************************** AM callback functions *************************************/
/* Call back function for handling of AM init cfm */
void CsrSppAmInitCfm(SppInstanceData_t *inst)
{
    CsrAmInitCfm *prim = (CsrAmInitCfm *)inst->recvMsgP;

    if (prim->result == CSR_RESULT_SUCCESS)
    {
        if (prim->buildId >= CSR_BT_FIRST_USE_AM_BUILD_ID)
        {
            inst->audioUseAm =  UseAm;
        }
        else
        {
            inst->audioUseAm =  DoNotUseAm;
        }
    }
    else
    {
        inst->audioUseAm = DoNotUseAm;
    }
    inst->amSppCallBack = NULL;
}

/* Call back function for handling of AM audio path connect cfm */
void CsrSppAmConnectCfm(SppInstanceData_t *inst)
{
    CsrAmAudioPathConnectCfm *prim = (CsrAmAudioPathConnectCfm *)inst->recvMsgP;
    
    switch (prim->result)
    {
        case CSR_RESULT_SUCCESS:
        {
            inst->amConnId = prim->amConnectionId;
            inst->amSppCallBack = NULL;
            break;
        }
        case CSR_AM_RESULT_ERROR_SRC_BUSY:
        case CSR_AM_RESULT_ERROR_SINK_BUSY:
        {/* Try another SRC/SNK */
            inst->amSppCallBack = NULL;
            break;
        }
        case CSR_AM_RESULT_UNSUPPORTED:
        {/* The init cfm somehow was misunderstood: the AM does not allow connections with the
            current firmware buildId; fallback to the "old" API */
            inst->audioUseAm = DoNotUseAm;
            inst->amSppCallBack = NULL;
            break;
        }
        default:/* do what? */
        {
            inst->amSppCallBack = NULL;
            break;
        }
    }    
}

/* Call back function for handling of AM audio path release cfm */
void CsrSppAmReleaseCfm(SppInstanceData_t *inst)
{
    inst->amConnId      = CSR_AM_NO_CONN_ID;
    inst->amSppCallBack = NULL;
}

#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
