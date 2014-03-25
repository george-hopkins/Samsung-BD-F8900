/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_proto.h"
/* Find connection index given address */
CsrUint8 CsrBtHfgFindIndexWithAddr(HfgMainInstance_t *inst,
                                   CsrBtDeviceAddr *addr)
{
    CsrUintFast8 i;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if (bd_addr_eq(addr, &inst->linkData[i].address))
        {
            return (CsrUint8)i;
        }
    }
    return CSR_BT_HFG_NO_CONID;
}

/* Find connection index given connection Id */
CsrUint8 CsrBtHfgFindIndexWithConnId(HfgMainInstance_t *inst,
                                   CsrUint32 hfgConnId)
{
    if (hfgConnId != CSR_BT_HFG_CONNECTION_ALL)
    {
        CsrUintFast8 i;
        for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
        {
            if (inst->linkData[i].hfgConnId == hfgConnId)
            {
                return (CsrUint8)i;
            }
        }
    }
    return CSR_BT_HFG_NO_CONID;
}

/* Find linkData from HFG primitive */
CsrUint8 CsrBtHfgFindIndexWithHfgPrim(HfgMainInstance_t *inst,
                               CsrUint16 type)
{
    switch(type)
    {
        case CSR_BT_HFG_ACTIVATE_REQ:
            {
                return CSR_BT_HFG_NO_CONID;
            }

        case CSR_BT_HFG_DEACTIVATE_REQ:
            {
                return CSR_BT_HFG_NO_CONID;
            }

        case CSR_BT_HFG_SERVICE_CONNECT_REQ:
            {
                return CSR_BT_HFG_NO_CONID;
            }

        case CSR_BT_HFG_CANCEL_CONNECT_REQ:
            {
                CsrBtHfgCancelConnectReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithAddr(inst, &(prim->deviceAddr));
            }

        case CSR_BT_HFG_DISCONNECT_REQ:
            {
                CsrBtHfgDisconnectReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_AUDIO_CONNECT_REQ:
            {
                CsrBtHfgAudioConnectReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }
        case CSR_BT_HFG_AUDIO_DISCONNECT_REQ:
            {
                CsrBtHfgAudioDisconnectReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }
        case CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES:
            {
                CsrBtHfgAudioAcceptConnectRes *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_SECURITY_IN_REQ:
            {
                return CSR_BT_HFG_NO_CONID;
            }

        case CSR_BT_HFG_SECURITY_OUT_REQ:
            {
                return CSR_BT_HFG_NO_CONID;
            }

        case CSR_BT_HFG_CONFIG_SNIFF_REQ:
            {
                return CSR_BT_HFG_NO_CONID;
            }

        case CSR_BT_HFG_CONFIG_AUDIO_REQ:
            {
                CsrBtHfgConfigAudioReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_RING_REQ:
            {
                CsrBtHfgRingReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_CALL_WAITING_REQ:
            {
                CsrBtHfgCallWaitingReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_CALL_HANDLING_REQ:
            {
                CsrBtHfgCallHandlingReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_CALL_HANDLING_RES:
            {
                CsrBtHfgCallHandlingRes *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_DIAL_RES:
            {
                CsrBtHfgDialRes *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_SPEAKER_GAIN_REQ:
            {
                CsrBtHfgSpeakerGainReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_MIC_GAIN_REQ:
            {
                CsrBtHfgMicGainReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_AT_CMD_REQ:
            {
                CsrBtHfgAtCmdReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_OPERATOR_RES:
            {
                CsrBtHfgOperatorRes *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_CALL_LIST_RES:
            {
                CsrBtHfgCallListRes *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_SUBSCRIBER_NUMBER_RES:
            {
                CsrBtHfgSubscriberNumberRes *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_STATUS_INDICATOR_SET_REQ:
            {
                CsrBtHfgStatusIndicatorSetReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_INBAND_RINGING_REQ:
            {
                CsrBtHfgInbandRingingReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_BT_INPUT_RES:
            {
                CsrBtHfgBtInputRes *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_VOICE_RECOG_REQ:
            {
                CsrBtHfgVoiceRecogReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_C2C_SF_REQ:
            {
                CsrBtHfgC2cSfReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_C2C_TXT_REQ:
            {
                CsrBtHfgC2cTxtReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_C2C_SMS_ARRIVE_REQ:
            {
                CsrBtHfgC2cSmsArriveReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_C2C_SMS_TXT_REQ:
            {
                CsrBtHfgC2cSmsTxtReq *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_VOICE_RECOG_RES:
            {
                CsrBtHfgVoiceRecogRes *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }

        case CSR_BT_HFG_MANUAL_INDICATOR_RES:
            {
                CsrBtHfgManualIndicatorRes *prim = inst->msg;
                return CsrBtHfgFindIndexWithConnId(inst, prim->connectionId);
            }
        case CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ:
            {
                return CSR_BT_HFG_NO_CONID;
            }
        case CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ:
            {
                return CSR_BT_HFG_NO_CONID;
            }

    }

    return CSR_BT_HFG_NO_CONID;
}

/* Find linkData from HFG primitive */
HfgInstance_t *CsrBtHfgFindLinkWithCmPrim(HfgMainInstance_t *inst,
                                     CsrUint16 type)
{
    switch(type)
    {
        case CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM:
            {
                CsrBtCmCancelAcceptConnectCfm *prim = inst->msg;
                return CsrBtHfgFindLinkWithServerChan(inst, prim->serverChannel);
            }

        case CSR_BT_CM_CONNECT_CFM:
            {
                CsrBtCmConnectCfm *prim = inst->msg;
                return CsrBtHfgFindLinkWithAddr(inst, &(prim->deviceAddr));
            }

        case CSR_BT_CM_CONNECT_ACCEPT_CFM:
            {
                CsrBtCmConnectAcceptCfm *prim = inst->msg;
                return CsrBtHfgFindLinkWithServerChan(inst, prim->serverChannel);
            }

        case CSR_BT_CM_REGISTER_CFM:
            {
                CsrBtCmRegisterCfm *prim = inst->msg;
                return CsrBtHfgFindLinkWithServerChan(inst, prim->serverChannel);
            }

        case CSR_BT_CM_DISCONNECT_IND:
            {
                CsrBtCmDisconnectInd *prim = inst->msg;
                return CsrBtHfgFindLinkWithConnId(inst, prim->btConnId);
            }

        case CSR_BT_CM_SCO_CONNECT_CFM:
            {
                CsrBtCmScoConnectCfm *prim = inst->msg;
                return CsrBtHfgFindLinkWithConnId(inst, prim->btConnId);
            }

        case CSR_BT_CM_SCO_DISCONNECT_IND:
            {
                CsrBtCmScoDisconnectInd *prim = inst->msg;
                return CsrBtHfgFindLinkWithConnId(inst, prim->btConnId);
            }

        case CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM:
            {
                CsrBtCmScoAcceptConnectCfm *prim = inst->msg;
                return CsrBtHfgFindLinkWithConnId(inst, prim->btConnId);
            }

        case CSR_BT_CM_DATA_IND:
            {
                CsrBtCmDataInd *prim = inst->msg;
                return CsrBtHfgFindLinkWithConnId(inst, prim->btConnId);
            }

        case CSR_BT_CM_DATA_CFM:
            {
                CsrBtCmDataCfm *prim = inst->msg;
                return CsrBtHfgFindLinkWithConnId(inst, prim->btConnId);
            }

        case CSR_BT_CM_CONTROL_IND:
            {
                CsrBtCmControlInd *prim = inst->msg;
                return CsrBtHfgFindLinkWithConnId(inst, prim->btConnId);
            }

        case CSR_BT_CM_RFC_MODE_CHANGE_IND:
            {
                CsrBtCmRfcModeChangeInd *prim = inst->msg;
                return CsrBtHfgFindLinkWithConnId(inst, prim->btConnId);
            }

        case CSR_BT_CM_PORTNEG_IND:
            {
                CsrBtCmPortnegInd *prim = inst->msg;
                return CsrBtHfgFindLinkWithConnId(inst, prim->btConnId);
            }

        case CSR_BT_CM_SDC_SEARCH_IND:
            {
                CsrBtCmSdcSearchInd *prim = inst->msg;
                return CsrBtHfgFindLinkWithAddr(inst, &(prim->deviceAddr));
            }

        case CSR_BT_CM_SDC_SEARCH_CFM:
            {
                CsrBtCmSdcSearchCfm *prim = inst->msg;
                return CsrBtHfgFindLinkWithAddr(inst, &(prim->deviceAddr));
            }

        case CSR_BT_CM_SDC_SERVICE_SEARCH_CFM:
            {
                return NULL;
            }

        case CSR_BT_CM_SDC_OPEN_CFM:
            {
                return NULL;
            }

        case CSR_BT_CM_SDC_ATTRIBUTE_CFM:
            {
                CsrBtCmSdcAttributeCfm *prim = inst->msg;
                return CsrBtHfgFindLinkWithAddr(inst, &(prim->deviceAddr));
            }

        case CSR_BT_CM_SDC_CLOSE_IND:
            {
                CsrBtCmSdcCloseInd *prim = inst->msg;
                return CsrBtHfgFindLinkWithAddr(inst, &(prim->deviceAddr));
            }

        case CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM:
            {
                CsrBtCmSdcReleaseResourcesCfm *prim = inst->msg;
                return CsrBtHfgFindLinkWithAddr(inst, &(prim->deviceAddr));
            }

        case CSR_BT_CM_SDS_REGISTER_CFM:
            {
                return NULL;
            }

        case CSR_BT_CM_SDS_UNREGISTER_CFM:
            {
                return NULL;
            }

        case CSR_BT_CM_MAP_SCO_PCM_IND:
            {
                CsrBtCmMapScoPcmInd *prim = inst->msg;
                return CsrBtHfgFindLinkWithConnId(inst, prim->btConnId);
            }

        default:
            {
                /* We shouln't reach this point */
                return NULL;
            }
    }

}

/* Find connection instance given server channel */
HfgInstance_t *CsrBtHfgFindLinkWithServerChan(HfgMainInstance_t *inst,
                                         CsrUint8 sc)
{
    CsrUintFast8 i;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if (inst->linkData[i].serverChannel == sc)
        {
            return &inst->linkData[i];
        }
    }
    return NULL;
}

HfgInstance_t *CsrBtHfgFindLinkIndexWithServerChan(HfgMainInstance_t *inst,
                                              CsrUint8 sc,
                                              CsrUint8 *index)
{
    CsrUintFast8 i;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if (inst->linkData[i].serverChannel == sc)
        {
            *index = (CsrUint8)i;
            return &inst->linkData[i];
        }
    }
    *index = 0xff;
    return NULL;
}

/* Find connection instance given connection Id */
HfgInstance_t *CsrBtHfgFindLinkWithConnId(HfgMainInstance_t *inst, CsrUint32 hfgConnId)
{
    CsrUintFast8 i;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if (inst->linkData[i].hfgConnId == hfgConnId)
        {
            return &inst->linkData[i];
        }
    }
    return NULL;
}

HfgInstance_t *CsrBtHfgFindLinkIndexWithConnId(HfgMainInstance_t *inst, CsrUint32 hfgConnId, CsrUint8 *index)
{
    CsrUintFast8 i;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if (inst->linkData[i].hfgConnId == hfgConnId)
        {
            *index = (CsrUint8)i;
            return &inst->linkData[i];
        }
    }
    *index = 0xff;
    return NULL;
}

#ifdef CSR_BT_INSTALL_HFG_C2C
/* Find connection instance given bccmd SeqNo */
HfgInstance_t *CsrBtHfgFindLinkWithSeqNo(HfgMainInstance_t *inst,
                                    CsrUint16 seqNo)
{
    CsrUintFast8 i;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if(inst->linkData[i].bccmdSeqNo == seqNo)
        {
            return &inst->linkData[i];
        }
    }
    return NULL;
}
#endif

/* Find connection instance that's not in use */
HfgInstance_t *CsrBtHfgFindLinkAvailable(HfgMainInstance_t *inst)
{
    CsrUintFast8 i;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if(inst->linkData[i].linkType == CSR_BT_HFG_CONNECTION_UNKNOWN)
        {
            return &(inst->linkData[i]);
        }
    }
    return NULL;
}

HfgInstance_t *CsrBtHfgFindLinkIndexAvailable(HfgMainInstance_t *inst, CsrUint8 *index)
{
    CsrUintFast8 i;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if((inst->linkData[i].linkType == CSR_BT_HFG_CONNECTION_UNKNOWN) 
            &&  (inst->linkData[i].serverChannel == CSR_BT_NO_SERVER) )
        {
            *index = (CsrUint8)i;
            return &(inst->linkData[i]);
        }
    }
    *index = 0xff;
    return NULL;
}

/* Check if server channel is in use by a connection */
CsrBool CsrBtHfgOccupiedServerChannel(HfgMainInstance_t *inst,
                                 CsrUint8 sc)
{
    CsrUintFast8 i;
    HfgInstance_t *link;

    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        link = &(inst->linkData[i]);
        if((link->serverChannel == sc) &&
           ((link->state != Activate_s) || (link->state != Unused_s)))
        {
            return TRUE;
        }
    }
    return FALSE;
}
/* Find connection instance given address */
HfgInstance_t *CsrBtHfgFindLinkWithAddr(HfgMainInstance_t *inst,
                                   CsrBtDeviceAddr *addr)
{
    CsrUintFast8 i;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if((inst->linkData[i].address.nap == addr->nap) &&
           (inst->linkData[i].address.uap == addr->uap) &&
           (inst->linkData[i].address.lap == addr->lap))
        {
            return &inst->linkData[i];
        }
    }
    return NULL;
}

/* Find the first CSR_UNUSED server channel */
CsrUint8 CsrBtHfgFindFreeServerChannel(HfgMainInstance_t *inst)
{
    CsrUintFast8 i;

    /* Try to find a server-channel that's not in use and not
     * registered */
    for(i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
    {
        if((inst->service[i].registered == FALSE) &&
           !CsrBtHfgOccupiedServerChannel(inst, inst->service[i].chan))
        {
            return inst->service[i].chan;
        }
    }

    /* Fallback to picking the first CSR_UNUSED channel */
    for(i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
    {
        if(!CsrBtHfgOccupiedServerChannel(inst, inst->service[i].chan))
        {
            return inst->service[i].chan;
        }
    }
    return CSR_BT_NO_SERVER;
}

