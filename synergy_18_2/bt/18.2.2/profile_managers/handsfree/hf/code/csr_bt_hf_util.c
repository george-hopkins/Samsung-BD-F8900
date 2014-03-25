/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hf_main.h"
#include "csr_bt_hf_prim.h"
#include "csr_bt_hf_dspm.h"
#include "csr_bt_hf_util.h"


#include "csr_bt_hfhs_data_sef.h"
#include "csr_bt_hf_lib.h"
/* Inclusions for use of common SDP search library*/
#include "csr_bt_sdc_support.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"
#include "csr_am_lib.h"

/* change this value according to the actual length of the service
 *name string length. The other index's will be adjusted according to
 *this length */
#define HF_SERVICE_NAME_LEN                        15                /* actual length of service name */

/* do not change these values */
#define HF_SERVER_CHANNEL_INDEX                    ((CsrUint8) 27)
#define HF_SERVICE_NAME_LEN_INDEX                  ((CsrUint8) 45)    /* index for service name length */
#define HF_SERVICE_NAME_INDEX                      ((CsrUint8) 46)    /* fill in the service name from this index */

/* Index to network parameter and supported features parameter The
 * network parameter is a single byte. The supported features is 2
 * byte and the value must be stored with MSB first */
#define HF_SUPPORTED_FEATURES_INDEX                ((CsrUint8) (HF_SERVICE_NAME_LEN_INDEX + HF_SERVICE_NAME_LEN + 5))

#define HS_SERVER_CHANNEL_INDEX                    30
#define HS_REMOTE_AUDIO_INDEX                      60

#define MAX_SERVICE_RECORDS_SEARCH            4  /* Max number of record handles with HFG or AG that amongst which we decide to connect to */

static const CsrUint8 sdsHfServiceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,   /* AttrID , ServiceClassIDList */
    0x35,0x06,        /* 6 bytes in total DataElSeq */
    0x19,0x11,0x1E,   /* 2 byte UUID, Service class = Hands Free */
    0x19,0x12,0x03,   /* 2 byte UUID Service class = GenericAudio */

    /* protocol descriptor list */
    0x09,0x00,0x04,   /* AttrId ProtocolDescriptorList */
    0x35,0x0c,        /* 11 bytes in total DataElSeq */
    0x35,0x03,        /* 3 bytes in DataElSeq */
    0x19, 0x01,0x00,  /* 2 byte UUID, Protocol = L2CAP */

    0x35,0x05,        /* 5 bytes in DataElSeq */
    0x19, 0x00,0x03,  /* 1 byte UUID Protocol = RFCOMM */
    0x08, 0x00,       /* 1 byte UINT - server channel template value 0 - to be filled in by app */

    /* profile descriptor list */
    0x09,0x00,0x09,   /* AttrId, ProfileDescriptorList */
    0x35,0x08,        /* 10 bytes in total DataElSeq */
    0x35,0x06,        /* 6 bytes in total DataElSeq */
    0x19, 0x11,0x1E,  /* 2 byte UUID, Service class = Hands Free */
    0x09, 0x01,0x06,  /* 2 byte UINT, version = 1.06 */

    /* service name */
    0x09, 0x01, 0x00, /* AttrId - Service Name */
    0x25, 0x0F,       /* 15 byte string */
    'H','a','n','d','s','-','F', 'r', 'e', 'e', ' ', 'u', 'n', 'i', 't',

    /* Supported features - passed in from the application */
    0x09, 0x03, 0x11,  /* AttrId - Supported Features */
    0x09, 0x00, 0x04,  /* 2 byte UINT - supported features */
                       /* EC and/or NR fn          - 0 (LSB) */
                       /* 3 way calls              - 0 */
                       /* CLI presentation         - 0 */
                       /* Voice recognition fn     - 0 */
                       /* Remote volume control    - 0 */
                       /* Wide-band speech         - 0 */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

static const CsrUint8 sdsHsServiceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,    /* AttrID , ServiceClassIDList */
    0x35,0x09,         /* 6 bytes in total DataElSeq */
    0x19,0x11,0x08,    /* 2 byte UUID, Service class = Headset = 0x1108 */
    0x19,0x12,0x03,    /* 2 byte UUID, Service class = Generic Audio = 0x1203 */
    0x19,0x11,0x31,    /* 2 byte UUID, Service class = Headset - HS (version 1.2) = 0x1131 */

    /* protocol descriptor list */
    0x09,0x00,0x04,    /* AttrId ProtocolDescriptorList */
    0x35,0x0C,         /* 12 bytes in total DataElSeq */

    /* L2CAP */
    0x35,0x03,         /* 3 bytes in DataElSeq */
    0x19,0x01,0x00,    /* 2 byte UUID, Protocol = L2CAP */

    /* RFCOMM */
    0x35,0x05,         /* 5 bytes in DataElSeq */
    0x19,0x00,0x03,    /* 2 byte UUID Protocol = RFCOMM */
    0x08,0x00,         /* 1 byte UINT - server channel template value 0 - to be filled in by app (index:31) */

    /* Bluetooth Profile descriptor List */
    0x09,0x00,0x09,    /* AttrId Bluetooth Profile DescriptorList = 0x0009 */
    0x35,0x08,         /* 8 bytes in total DataElSeq */
    0x35,0x06,         /* 6 bytes in total DataElSeq */
    0x19,0x11,0x08,    /* 2 byte UUID, Service class = Headset = 0x1108 */

    /* Profile version */
    0x09,0x01,0x02,     /* 2 byte UINT - Profile version = 0x0102 = version 1.2 */

    /* Service name */
    0x09,0x01,0x00,     /* AttrId - Service Name. Use language base attribute 0x0100 (primary language) */
    0x25, 0x07,         /* length of service string: 7 */
    'H','e','a','d','s','e','t',    /* string = "Headset" */

    /* Remote audio volume control */
    0x09,0x03,0x02,     /* AttrId - Remote audio volume control = 0x0302 */
    0x28,0x00,          /* Boolean. Remote control template value = FALSE. (index: 61) */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

void CsrBtHfMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_HF_PRIM, msg);
}

/***********************************************************************************
    Has the application enabled one or more C2C features?
************************************************************************************/
CsrBool getLocC2CEnabled(HfMainInstanceData_t *instData)
{
    CsrUintFast8 i;
    
    for (i= 0; i< CSR_BT_C2C_ADPCM_IND;i++)
    {
        if(instData->locC2C[i] != 0)
        {/* At least feature index 'i' is enabled...*/
            return TRUE;
        }
    }

    return FALSE;
}

/***********************************************************************************
    Make sure not to allow more connections than allowed by the application
************************************************************************************/
void CsrBtHfCancelAcceptCheck(HfMainInstanceData_t *instData)
{
    CsrUint8 nrActiveHf = 0;
    CsrUint8 nrActiveHs = 0;

    if (instData->maxTotalSimultaneousConnections == CsrBtHfGetNumberOfRecordsInUse(instData,&nrActiveHf,&nrActiveHs))
    {/* Maximum reached! */
        CsrIntFast8 i;
        HfInstanceData_t *linkPtr;
        for (i = 0; i<(instData->maxHFConnections +instData->maxHSConnections);i++)
        {
            linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
            if ((linkPtr->state == Activate_s) && (linkPtr->accepting))
            {/* Not connected: cancel connect accept */
                linkPtr->accepting = FALSE;
                CsrBtCmCancelAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->serverChannel);
            }
        }
    }
}

/***********************************************************************************
    Make sure to allow more connections if allowed by the application
************************************************************************************/
void CsrBtHfAllowConnectCheck(HfMainInstanceData_t *instData)
{
    CsrUint8 nrActiveHf = 0;
    CsrUint8 nrActiveHs = 0;

    if ((instData->maxTotalSimultaneousConnections > CsrBtHfGetNumberOfRecordsInUse(instData,&nrActiveHf,&nrActiveHs)) &&
        (instData->state != Deactivate_s))
    {/* Allowed more connections */
        CsrIntFast8 i;
        HfInstanceData_t *linkPtr;
        for (i = 0; i<(instData->maxHFConnections +instData->maxHSConnections);i++)
        {
            linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
            if ((linkPtr->state == Activate_s) && (linkPtr->accepting == FALSE) && (linkPtr->sdsRegistered))
            {/* Not connected: cancel connect accept */
                linkPtr->accepting = TRUE;
                if (i < instData->maxHFConnections )
                {
                    CsrBtCmConnectAcceptReqSend(CSR_BT_HF_IFACEQUEUE,
                                   HF_CLASS_OF_DEVICE,
                                   0,
                                   CSR_BT_HF_PROFILE_DEFAULT_MTU_SIZE,
                                   linkPtr->serverChannel,
                                   instData->secIncoming,
                                   CSR_BT_HF_PROFILE_UUID,
                                   instData->modemStatus,
                                   CSR_BT_DEFAULT_BREAK_SIGNAL,
                                   CSR_BT_DEFAULT_MSC_TIMEOUT);

                }
                else
                {
                    CsrBtCmConnectAcceptReqSend(CSR_BT_HF_IFACEQUEUE,
                                   HS_CLASS_OF_DEVICE,
                                   0,
                                   CSR_BT_HF_PROFILE_DEFAULT_MTU_SIZE,
                                   linkPtr->serverChannel,
                                   instData->secIncoming,
                                   CSR_BT_HS_PROFILE_UUID,
                                   instData->modemStatus,
                                   CSR_BT_DEFAULT_BREAK_SIGNAL,
                                   CSR_BT_DEFAULT_MSC_TIMEOUT);
                }

            }
        }
    }
}

/***********************************************************************************
    Find out how many active or pending connections exist at any given moment
************************************************************************************/
CsrUint8 CsrBtHfGetNumberOfRecordsInUse(HfMainInstanceData_t *instData,CsrUint8 *nrActiveHf,CsrUint8 *nrActiveHs)
{
    CsrUint8 retValue = 0;
    CsrIntFast8 i;
    *nrActiveHf = 0;
    *nrActiveHs = 0;
    for (i=0;i<(instData->maxHFConnections + instData->maxHSConnections);i++)
    {
        if (instData->linkData[i].state != Activate_s)
        {/* Then it is either connected or trying to connect... */
            retValue++;
            if (i < instData->maxHFConnections)
            {
                (*nrActiveHf)++;
            }
            else
            {
                (*nrActiveHs)++;
            }
        }
    }

    return retValue;
}

/***********************************************************************************
    Used when sending messages internally or to higher layer.
***********************************************************************************/
void CsrBtHfSendHfHouseCleaning(HfMainInstanceData_t *instData)
{
    CsrBtHfHouseCleaning    *prim;

    prim = (CsrBtHfHouseCleaning *) CsrPmemAlloc(sizeof(CsrBtHfHouseCleaning));
    prim->type = CSR_BT_HF_HOUSE_CLEANING;
    instData->restoreFlag = TRUE;
    CsrBtHfMessagePut(CSR_BT_HF_IFACEQUEUE, prim);
}

/***********************************************************************************
    Save the message specified by recvMsgP in instData on the internal savequeue.
    Remember to set the recvMsgP to NULL in order not to free it on exit.
***********************************************************************************/
void CsrBtHfSaveMessage(HfMainInstanceData_t *instData)
{
    CsrMessageQueuePush(&instData->saveQueue, CSR_BT_HF_PRIM, instData->recvMsgP);
    instData->recvMsgP = NULL;
}

/*******************************************************************************
    init instance data for one transaction
*******************************************************************************/
void CsrBtHfInitInstanceData(HfMainInstanceData_t *instData)
{
    HfInstanceData_t * hfInstData;
    CsrUint16 mi;
    void *mv;

    hfInstData = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    hfInstData->mainInstance = instData;

    if (hfInstData->data->atResponseTimerId != 0)
    {
        CsrSchedTimerCancel(hfInstData->data->atResponseTimerId, &mi, &mv);
        hfInstData->data->atResponseTimerId     = 0;
    }
    if (hfInstData->deregisterTimerId != 0)
    {
        CsrSchedTimerCancel(hfInstData->deregisterTimerId, &mi, &mv);
        hfInstData->deregisterTimerId           = 0;
    }
    if (hfInstData->cindSupportString != NULL)
    {
        CsrPmemFree(hfInstData->cindSupportString);
        hfInstData->cindSupportString = NULL;
    }
    if (hfInstData->cindStartValueString != NULL)
    {
        CsrPmemFree(hfInstData->cindStartValueString);
        hfInstData->cindStartValueString = NULL;
    }
    hfInstData->nrOfIndicators      = 0;
    if (hfInstData->chldStringStored != NULL)
    {
        CsrPmemFree(hfInstData->chldStringStored);
        hfInstData->chldStringStored = NULL;
    }
    if(hfInstData->lpTimer != 0)
    {
        CsrSchedTimerCancel(hfInstData->lpTimer,&mi,&mv);
        hfInstData->lpTimer         = 0;
    }

    hfInstData->data->recvAtCmdSize             = 0;
    hfInstData->data->recvAtCmd                 = NULL;
    hfInstData->data->allowed2SendCmData        = TRUE;
    hfInstData->data->dataReceivedInConnected   = FALSE;
    hfInstData->data->cmDataReqQueue            = NULL;

    hfInstData->lastAtCmdSent                   = idleCmd;

    hfInstData->disconnectReqReceived           = FALSE;
    hfInstData->disconnectPeerReceived          = FALSE;
    hfInstData->scoHandle                       = HF_SCO_UNUSED;
    hfInstData->linkState                       = CSR_BT_LINK_STATUS_DISCONNECTED;
    hfInstData->hfConnId                        = CSR_BT_CONN_ID_INVALID;

    hfInstData->audioPending                    = FALSE;
    hfInstData->lastAudioReq                    = FALSE;
    hfInstData->audioQuality                    = HCI_PKT_HV3;
    hfInstData->state                           = Activate_s; /* */
    hfInstData->oldState                        = Activate_s;
    hfInstData->disconnectResult                = CSR_BT_RESULT_CODE_CM_SUCCESS;
    hfInstData->disconnectSupplier              = CSR_BT_SUPPLIER_CM;
    hfInstData->remoteVersion                   = 0;
    hfInstData->pcmSlot                         = 0;
    hfInstData->pcmReassign                     = FALSE;
    hfInstData->pcmMappingReceived              = FALSE;
    hfInstData->audioAcceptPending              = FALSE;
    hfInstData->atSequenceState                 = supportFeatures;
    CsrMemSet(hfInstData->serviceName,0,CSR_BT_MAX_FRIENDLY_NAME_LEN+1);

    hfInstData->scoConnectAcceptPending         = FALSE;
    hfInstData->searchAndCon                    = FALSE;
    hfInstData->searchOngoing                   = FALSE;
    hfInstData->pendingCancel                   = FALSE;
    CsrMemCpy(&hfInstData->audioSetupParams,&instData->generalAudioParams, sizeof(audioSetupParams_t));
    hfInstData->localSupportedFeatures          = instData->localSupportedFeatures;
    hfInstData->linkInd.remoteSupport           = FALSE;
    CsrMemSet(hfInstData->linkInd.remC2C,0,CSR_BT_C2C_NUMBER_OF_INDICATORS);
    CsrMemCpy(hfInstData->linkInd.locC2C,instData->locC2C,CSR_BT_C2C_NUMBER_OF_INDICATORS);

    hfInstData->lpCount             = HF_LP_RETRY;
    hfInstData->pendingModeChange   = FALSE;
    hfInstData->lpMode              = 0;
    hfInstData->lpWantedMode        = 0;
    hfInstData->incomingSlc         = FALSE;
    hfInstData->outgoingAudio       = FALSE;
    hfInstData->localDisconnect     = FALSE;
    hfInstData->pendingSlcXtraCmd   = FALSE;
    hfInstData->accepting           = FALSE;
#ifdef CSR_USE_DSPM
    CsrHfDspmInit(instData);
#endif
}

/*******************************************************************************
    Empty the internal save queue's. Called before entering idle state.
*******************************************************************************/
void CsrBtHfSaveQueueCleanUp(HfMainInstanceData_t *instData)
{
    CsrUint16            eventClass;
    void                *msg;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    /* pop all (if any) messages from the message save queue */
    while (CsrMessageQueuePop(&(instData->saveQueue), &eventClass, &msg))
    {
        CsrBtHfPrim         *primType;

        /* find the message type */
        primType = (CsrBtHfPrim *)msg;
        if (*primType == CSR_BT_HF_AT_CMD_REQ)
        {
            CsrBtHfAtCmdReq *prim;

            prim = (CsrBtHfAtCmdReq *) msg;
            CsrPmemFree(prim->atCmdString);
        }
        CsrPmemFree(msg);
    }

    /* pop all (if any) messages from the CM save queue */
    if (linkPtr->data != NULL)
    {
        while (CsrMessageQueuePop( &(linkPtr->data->cmDataReqQueue ), &eventClass, &msg))
        {
            if (eventClass == CSR_BT_CM_PRIM)
            {
                CsrBtCmDataReq *prim;

                prim = (CsrBtCmDataReq *) msg;
                CsrPmemFree(prim->payload);
                CsrPmemFree(msg);
            }
            else if (eventClass == CSR_BT_HF_PRIM)
            {
                CsrBtHfPrim  *primType = (CsrBtHfPrim *)msg;

                if (*primType == CSR_BT_HF_AT_CMD_REQ)
                {
                    CsrBtHfAtCmdReq *prim;

                    prim = (CsrBtHfAtCmdReq *) msg;
                    CsrPmemFree(prim->atCmdString);
                }
                else if (*primType == CSR_BT_HF_DIAL_REQ)
                {
                    CsrBtHfDialReq *prim = (CsrBtHfDialReq *)msg;
                    CsrPmemFree(prim->number);
                }
                CsrPmemFree(msg);
            }
        }
        CsrPmemFree(linkPtr->data->recvAtCmd);
        linkPtr->data->recvAtCmdSize = 0;
        linkPtr->data->recvAtCmd = NULL;
    }
    linkPtr->lastAtCmdSent = idleCmd;
}




/*************************************************************************************
    Send a confirm message to app
************************************************************************************/
void CsrBtHfSendConfirmMessage(HfMainInstanceData_t *instData, CsrBtHfPrim type)
{
    CsrBtHfConfigAudioCfm * prim;

    prim = (CsrBtHfConfigAudioCfm *) CsrPmemAlloc( sizeof (CsrBtHfConfigAudioCfm));
    prim->type = type;
    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
    Send a CSR_BT_HF_MIC_GAIN_IND to app
************************************************************************************/
void CsrBtHfSendHfMicGainInd(HfMainInstanceData_t *instData,
                        CsrUint8 returnValue)
{
    CsrBtHfMicGainInd    *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim    = (CsrBtHfMicGainInd *)CsrPmemAlloc(sizeof(CsrBtHfMicGainInd));
    prim->type        = CSR_BT_HF_MIC_GAIN_IND;
    prim->gain        = returnValue;
    prim->connectionId = linkPtr->hfConnId;

    CsrBtHfMessagePut(instData->appHandle, prim);
}


/*************************************************************************************
    Send a CSR_BT_HF_SPEAKER_GAIN_IND to app
************************************************************************************/
void CsrBtHfSendHfSpeakerGainInd(HfMainInstanceData_t *instData,
                            CsrUint8 returnValue)
{
    CsrBtHfSpeakerGainInd    *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim    = (CsrBtHfSpeakerGainInd *)CsrPmemAlloc(sizeof(CsrBtHfSpeakerGainInd));
    prim->type        = CSR_BT_HF_SPEAKER_GAIN_IND;
    prim->gain        = returnValue;
    prim->connectionId = linkPtr->hfConnId;

    CsrBtHfMessagePut(instData->appHandle, prim);
}

/********************************************************************************************
    Send a cfm message to the app that only takes type, connectionId and result as parameters
*********************************************************************************************/
void CsrBtHfSendHfGeneralCfmMsg(HfMainInstanceData_t *instData, CsrUint16 result, CsrBtHfPrim type)
{
    HF_GENERAL_CFM_T  *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim    = (HF_GENERAL_CFM_T *)CsrPmemAlloc(sizeof(HF_GENERAL_CFM_T));
    prim->type        = type;
    prim->connectionId = linkPtr->hfConnId;
    prim->cmeeResultCode = result;

    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
    Send a CsrBtHfInbandRingSettingChangedInd to app
************************************************************************************/
void CsrBtHfSendHfInBandRingToneInd(HfMainInstanceData_t *instData,
                               CsrBool                returnValue)
{
    CsrBtHfInbandRingSettingChangedInd    *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim                 = (CsrBtHfInbandRingSettingChangedInd *)CsrPmemAlloc(sizeof(CsrBtHfInbandRingSettingChangedInd));
    prim->type           = CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND;
    prim->inbandRingingActivated  = returnValue;
    prim->connectionId = linkPtr->hfConnId;

    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
    Send a CSR_BT_HF_C2C_SF_IND to app
************************************************************************************/
void CsrBtHfSendHfC2CSfInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CsrBtHfC2cSfInd *prim = CsrPmemAlloc(sizeof(CsrBtHfC2cSfInd));
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim->type = CSR_BT_HF_C2C_SF_IND;
    prim->connectionId = linkPtr->hfConnId;
    if (atTextString != NULL)
    {
        prim->indicatorsLength = (CsrUint16)(CsrStrLen((char*)atTextString) + 1);
        prim->indicators = CsrPmemAlloc(prim->indicatorsLength);
        CsrStrNCpyZero((char *)prim->indicators,(char *)atTextString,prim->indicatorsLength);
    }
    else
    {
        prim->indicatorsLength = 0;
        prim->indicators = NULL;
    }

    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
    Send a CSR_BT_HF_C2C_BATT_IND to app
************************************************************************************/
void CsrBtHfSendHfC2CBattInd(HfMainInstanceData_t *instData)
{
    CsrBtHfC2cBattInd *prim = CsrPmemAlloc(sizeof(CsrBtHfC2cBattInd));
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim->type = CSR_BT_HF_C2C_BATT_IND;
    prim->connectionId = linkPtr->hfConnId;

    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
    Send a CSR_BT_HF_C2C_SMS_IND to app
************************************************************************************/
void CsrBtHfSendHfC2CSmsInd(HfMainInstanceData_t *instData, CsrUint8 index)
{
    CsrBtHfC2cSmsInd *prim = CsrPmemAlloc(sizeof(CsrBtHfC2cSmsInd));
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim->type  = CSR_BT_HF_C2C_SMS_IND;
    prim->connectionId = linkPtr->hfConnId;
    prim->index = index;

    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
    Send a CSR_BT_HF_C2C_GET_SMS_CFM to app
************************************************************************************/
void CsrBtHfSendHfC2CGetSMSCfm(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CsrBtHfC2cGetSmsCfm *prim = CsrPmemAlloc(sizeof(CsrBtHfC2cGetSmsCfm));
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim->type  = CSR_BT_HF_C2C_GET_SMS_CFM;
    prim->connectionId = linkPtr->hfConnId;
    if (atTextString != NULL)
    {
        prim->smsString = CsrPmemAlloc(CsrStrLen((char*)atTextString + 1));
        CsrStrNCpyZero((char *)prim->smsString,(char *)atTextString,CsrStrLen((char*)atTextString + 1));
        prim->cmeeResultCode = CSR_BT_CME_SUCCESS;
    }
    else
    {
        prim->smsString = NULL;
        prim->cmeeResultCode = CSR_BT_CME_INVALID_INDEX;
    }

    CsrBtHfMessagePut(instData->appHandle, prim);

}

/*************************************************************************************
    Send a CSR_BT_HF_C2C_TXT_IND to app
************************************************************************************/
void CsrBtHfSendHfC2CTxtInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CsrBtHfC2cTxtInd *prim = CsrPmemAlloc(sizeof(CsrBtHfC2cTxtInd));
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim->type  = CSR_BT_HF_C2C_TXT_IND;
    prim->connectionId = linkPtr->hfConnId;
    if (atTextString != NULL)
    {
        prim->txtString = CsrPmemAlloc(CsrStrLen((char*)atTextString + 1));
        CsrStrNCpyZero((char *)prim->txtString,(char *)atTextString,CsrStrLen((char*)atTextString + 1));
    }
    else
    {
        prim->txtString = NULL;
    }
    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
    Send a CSR_BT_HF_AUDIO_DISCONNECT_CFM to app
************************************************************************************/
void CsrBtHfSendHfAudioDisconnectCfm(HfMainInstanceData_t *instData,
                           CsrUint16 scoHandle,
                           CsrBtResultCode      resultCode,
                           CsrBtSupplier  resultSupplier)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    CsrBtHfAudioDisconnectCfm    *prim;
    prim    = (CsrBtHfAudioDisconnectCfm *)CsrPmemAlloc(sizeof(CsrBtHfAudioDisconnectCfm));
    linkPtr->outgoingAudio = FALSE;
    prim->type        = CSR_BT_HF_AUDIO_DISCONNECT_CFM;
    prim->resultCode = resultCode;
    prim->scoHandle = scoHandle;
    prim->resultSupplier   = resultSupplier;
    prim->connectionId = linkPtr->hfConnId;
    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
    Send a CSR_BT_HF_AUDIO_DISCONNECT_IND to app
************************************************************************************/
void CsrBtHfSendHfAudioDisconnectInd(HfMainInstanceData_t *instData,
                           CsrUint16 scoHandle,
                           CsrBtReasonCode      reasonCode,
                           CsrBtSupplier  reasonSupplier)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    CsrBtHfAudioDisconnectInd    *prim;
    prim    = (CsrBtHfAudioDisconnectInd *)CsrPmemAlloc(sizeof(CsrBtHfAudioDisconnectInd));
    prim->type        = CSR_BT_HF_AUDIO_DISCONNECT_IND;
    prim->scoHandle = scoHandle;
    prim->reasonCode = reasonCode;
    prim->reasonSupplier   = reasonSupplier;
    prim->connectionId = linkPtr->hfConnId;
    CsrBtHfMessagePut(instData->appHandle, prim);
    /* If there is an audio stream, remember to release it */
    /* This will be a new DSPM function */
}

/*************************************************************************************
    Send a CSR_BT_HF_AUDIO_CONNECT_IND to app
************************************************************************************/
void CsrBtHfSendHfAudioInd(HfMainInstanceData_t *instData,
                           CsrUint8              pcmSlot,
                           CsrUint8              theScoLinkType,
                           CsrUint8              weSco,
                           CsrUint16             rxPacketLength,
                           CsrUint16             txPacketLength,
                           CsrUint8              airMode,
                           CsrUint8              txInterval,
                           CsrBtResultCode      resultCode,
                           CsrBtReasonCode      reasonCode,
                           CsrBtSupplier  resultSupplier)
{
    CsrBtHfAudioConnectInd    *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim    = (CsrBtHfAudioConnectInd *)CsrPmemAlloc(sizeof(CsrBtHfAudioConnectInd));
    if (linkPtr->outgoingAudio)
    {
        linkPtr->outgoingAudio = FALSE;
        prim->type        = CSR_BT_HF_AUDIO_CONNECT_CFM;
    }
    else
    {
        prim->type        = CSR_BT_HF_AUDIO_CONNECT_IND;
    }
    prim->linkType = theScoLinkType;
    prim->connectionId = linkPtr->hfConnId;
    prim->scoHandle = linkPtr->scoHandle;
    prim->pcmSlot = pcmSlot;
    prim->txInterval = txInterval;
    prim->weSco = weSco;
    prim->rxPacketLength = rxPacketLength;
    prim->txPacketLength = txPacketLength;
    prim->airMode = airMode;
    prim->codecUsed = linkPtr->audioCodecType;
    prim->sampleRate = linkPtr->sampleRate;
    prim->resultCode = resultCode;
    prim->resultSupplier   = resultSupplier;
    CsrBtHfMessagePut(instData->appHandle, prim);

}

/*************************************************************************************
    Send a CSR_BT_HF_SERVICE_CONNECT_IND to app
************************************************************************************/
void CsrBtHfSendHfServiceConnectInd(HfMainInstanceData_t *instData,
                                    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtHfServiceConnectInd *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim = (CsrBtHfServiceConnectInd *)CsrPmemAlloc(sizeof(CsrBtHfServiceConnectInd));
    linkPtr->nrOfIndicators = 0;
    if (linkPtr->incomingSlc)
    {
        prim->type = CSR_BT_HF_SERVICE_CONNECT_IND;
    }
    else
    {
        prim->type = CSR_BT_HF_SERVICE_CONNECT_CFM;
    }
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->indicatorSupported = NULL;
    prim->indicatorValue = NULL;
    prim->chldString = NULL;
    prim->serviceName = NULL;
    prim->connectionType = linkPtr->linkType;
    prim->btConnId = CSR_BT_CONN_ID_INVALID;

    if ((linkPtr->sdsRegistered) &&
        (resultCode == CSR_BT_RESULT_CODE_HF_SUCCESS) &&
        (resultSupplier == CSR_BT_SUPPLIER_HF))
    {/* Remove service record now or after timeout */

        if (instData->deregisterTime == 0)
        {
            CsrBtCmSdsUnRegisterReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->serviceRecHandle);
        }
        else
        {
            linkPtr->deregisterTimerId = CsrSchedTimerSet((CsrTime)(instData->deregisterTime * 1000000),
                                                                HsDeregisterTimeout,(CsrUint16)instData->index,(void*)instData);
        }
    }

    if (prim->connectionType == CSR_BT_HF_CONNECTION_HF)
    {
        prim->deviceAddr = linkPtr->currentDeviceAddress;
        prim->supportedFeatures = linkPtr->supportedFeatures;
        prim->network = linkPtr->network;
        prim->remoteVersion = linkPtr->remoteVersion;
        linkPtr->serviceHandleListCount     = 0;
        prim->serviceName = CsrPmemAlloc(CSR_BT_MAX_FRIENDLY_NAME_LEN + 1);
        CsrUtf8StrNCpyZero((CsrUtf8String*)prim->serviceName,linkPtr->serviceName,CSR_BT_MAX_FRIENDLY_NAME_LEN + 1);
        CsrUtf8StrTruncate((CsrUtf8String*)prim->serviceName, CSR_BT_MAX_FRIENDLY_NAME_LEN); 
        prim->connectionId = linkPtr->hfConnId;
        if (linkPtr->cindSupportString != NULL)
        {
            CsrUint16 length = (CsrUint16)CsrStrLen((char *)linkPtr->cindSupportString)+1;
            prim->indicatorSupported = CsrPmemAlloc(length);
            CsrMemCpy(prim->indicatorSupported,linkPtr->cindSupportString,length);
            prim->indicatorSupported[length-1] = 0;
        }
        if (linkPtr->cindStartValueString != NULL)
        {
            CsrUintFast8 j = 0;
            CsrUint16 length = (CsrUint16)CsrStrLen((char *)linkPtr->cindStartValueString)+1;
            prim->indicatorValue = CsrPmemAlloc(length);
            CsrMemCpy(prim->indicatorValue,linkPtr->cindStartValueString,length);
            prim->indicatorValue[length-1] = 0;
            linkPtr->nrOfIndicators = 1;
            for (j=0;prim->indicatorValue[j] != '\0'; j++)
            {
                if (prim->indicatorValue[j] == ',')
                {
                    linkPtr->nrOfIndicators++;
                }
            }
        }
        if (linkPtr->chldStringStored != NULL)
        {
            CsrUint16 length = (CsrUint16)CsrStrLen((char *)linkPtr->chldStringStored)+1;
            prim->chldString = CsrPmemAlloc(length);
            CsrStrNCpyZero((char *)prim->chldString,(char *)linkPtr->chldStringStored,length);
        }
        if ((resultCode == CSR_BT_RESULT_CODE_HF_SUCCESS) &&
            (resultSupplier == CSR_BT_SUPPLIER_HF))
        {
            CsrBool c2cEnabled = getLocC2CEnabled(instData);

            if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CLIP_ACTIVATION) &&
                (instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CCWA_ACTIVATION) &&
                (instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CMEE_ACTIVATION) &&
                (c2cEnabled == FALSE))
            {
                CsrBtHfLpStart(instData, TRUE);
            }
            else
            {
                linkPtr->pendingSlcXtraCmd = FALSE;

                if (c2cEnabled)
                {/* send C2C init */
                    linkPtr->pendingSlcXtraCmd = TRUE;
                    /* make sure to send the correct C2C data! */
                    CsrMemCpy(linkPtr->linkInd.locC2C,instData->locC2C,CSR_BT_C2C_NUMBER_OF_INDICATORS);
                    CsrBtHfSendC2CInitMsgReq(instData);
                }

                if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CLIP_ACTIVATION) == 0)
                {/* send AT+CLIP=1? */
                    if (linkPtr->localSupportedFeatures & CSR_BT_HF_SUPPORT_CLI_PRESENTATION_CAPABILITY)
                    {/* YES. Send it */
                        CsrBtHfSetCallNotificationIndicationReqSend(linkPtr->serverChannel,TRUE);
                        linkPtr->pendingSlcXtraCmd = TRUE;
                    }
                }
                if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CCWA_ACTIVATION) == 0)
                {/* send AT+CCWA=1 */
                    if (((linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_THREE_WAY_CALLING) ||
                         (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_ENHANCED_CALL_CONTROL)) &&
                        ((linkPtr->localSupportedFeatures & CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING) ||
                         (linkPtr->localSupportedFeatures & CSR_BT_HF_SUPPORT_ENHANCED_CALL_CONTROL)))
                    {/* YES. Send it */
                        CsrBtHfSetCallWaitingNotificationReqSend(linkPtr->serverChannel,TRUE);
                        linkPtr->pendingSlcXtraCmd = TRUE;
                    }

                }
                if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CMEE_ACTIVATION) == 0)
                {/* send AT+CMEE=1 */
                    if (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES)
                    {/* YES. Send it */
                        CsrBtHfSetExtendedAgErrorResultCodeReqSend(linkPtr->serverChannel,TRUE);
                        linkPtr->pendingSlcXtraCmd = TRUE;
                    }
                }
                if (linkPtr->pendingSlcXtraCmd == FALSE)
                {
                    CsrBtHfLpStart(instData, TRUE);
                }
            }
        }
    }
    else if (prim->connectionType == CSR_BT_HF_CONNECTION_HS)
    {
        prim->deviceAddr = linkPtr->currentDeviceAddress;
        prim->supportedFeatures = 0;
        prim->network = 0;
        prim->remoteVersion = 0;
        linkPtr->serviceHandleListCount     = 0;
        prim->serviceName = CsrPmemAlloc(CSR_BT_MAX_FRIENDLY_NAME_LEN + 1);
        CsrStrNCpyZero(prim->serviceName, "Audio Gateway",CSR_BT_MAX_FRIENDLY_NAME_LEN + 1);
        CsrUtf8StrTruncate((CsrUtf8String*)prim->serviceName, CSR_BT_MAX_FRIENDLY_NAME_LEN);
        prim->connectionId = linkPtr->hfConnId;
    }
    else
    {
        prim->deviceAddr = instData->currentDeviceAddress;
        prim->supportedFeatures = 0;
        prim->network = 0;
        prim->remoteVersion = 0;
        prim->connectionId = CSR_BT_HF_CONNECTION_ALL;
    }

    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
    Send a CSR_BT_HF_DISCONNECT_IND to app
************************************************************************************/
void CsrBtHfSendHfDisconnectInd(HfMainInstanceData_t *instData,
                                CsrBtResultCode reasonCode, CsrBtSupplier reasonSupplier)
{
    CsrBtHfDisconnectInd *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim = (CsrBtHfDisconnectInd *)CsrPmemAlloc(sizeof(CsrBtHfDisconnectInd));
    if (linkPtr->disconnectReqReceived)
    {
        prim->type = CSR_BT_HF_DISCONNECT_CFM;
    }
    else
    {
        prim->type = CSR_BT_HF_DISCONNECT_IND;
    }
    prim->connectionId   = linkPtr->hfConnId;
    if (reasonCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        reasonSupplier == CSR_BT_SUPPLIER_CM)
    {
        prim->reasonCode = CSR_BT_RESULT_CODE_HF_SUCCESS;
        prim->reasonSupplier = CSR_BT_SUPPLIER_HF;
    }
    else
    {
        prim->reasonCode     = reasonCode;
        prim->reasonSupplier = reasonSupplier;
    }
    CsrBtHfMessagePut(instData->appHandle, prim);

    CsrBtHfLpStop(instData);
}

/*************************************************************************************
    Send a CSR_BT_HF_CALL_RINGING_IND to app
************************************************************************************/
void CsrBtHfSendHfRingInd(HfMainInstanceData_t *instData)
{
    CsrBtHfCallRingingInd *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim = (CsrBtHfCallRingingInd *)CsrPmemAlloc(sizeof(CsrBtHfCallRingingInd));
    prim->type = CSR_BT_HF_CALL_RINGING_IND;
    prim->connectionId = linkPtr->hfConnId;

    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
    Send a CSR_BT_HF_STATUS_LOW_POWER_IND to app
************************************************************************************/
void CsrBtHfSendHfStatusInd(HfMainInstanceData_t *instData, CsrUint8 status, CsrUint8 old,
                            CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    if (CSR_BT_HF_CNF_ENABLE_LOW_POWER_STATUS & instData->mainConfig)
    {
        CsrBtHfStatusLowPowerInd    *prim;
        HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

        prim = (CsrBtHfStatusLowPowerInd *) CsrPmemAlloc(sizeof(CsrBtHfStatusLowPowerInd));
        prim->type            = CSR_BT_HF_STATUS_LOW_POWER_IND;

        prim->currentMode    = status;
        prim->oldMode = old;
        prim->wantedMode = linkPtr->lpWantedMode;
        if (linkPtr->pendingModeChange)
        {
            prim->remoteReq = FALSE;
        }
        else
        {
            prim->remoteReq = TRUE;
        }
        prim->resultCode = resultCode;
        prim->resultSupplier = resultSupplier;
        prim->connectionId  = linkPtr->hfConnId;

        CsrBtHfMessagePut(instData->appHandle, prim);
    }
}

/*************************************************************************************
    Send a CSR_BT_HF_DEACTIVATE_CFM to app
************************************************************************************/
void CsrBtHfSendHfDeactivateCfm(HfMainInstanceData_t *instData,
                                CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtHfDeactivateCfm    *prim;

    prim = (CsrBtHfDeactivateCfm *) CsrPmemAlloc(sizeof(CsrBtHfDeactivateCfm));
    prim->type            = CSR_BT_HF_DEACTIVATE_CFM;
    prim->resultCode      = resultCode;
    prim->resultSupplier  = resultSupplier;
    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
    Send a CSR_BT_HF_ACTIVATE_CFM to app
************************************************************************************/
void CsrBtHfSendHfActivateCfm(HfMainInstanceData_t *instData,
                              CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtHfActivateCfm    *prim;

    prim = (CsrBtHfActivateCfm *) CsrPmemAlloc(sizeof(CsrBtHfActivateCfm));
    prim->type           = CSR_BT_HF_ACTIVATE_CFM;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtHfMessagePut(instData->appHandle, prim);
}


/*************************************************************************************
    Send a CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM to app
************************************************************************************/
void CsrBtHfSendIndicatorsUpdateCfm(HfMainInstanceData_t *instData, CsrUint16 result)
{
    CsrBtHfGetAllStatusIndicatorsCfm  *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim = (CsrBtHfGetAllStatusIndicatorsCfm *)CsrPmemAlloc(sizeof(CsrBtHfGetAllStatusIndicatorsCfm));
    prim->type = CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM;
    prim->connectionId          = linkPtr->hfConnId;
    prim->indicatorSupported    = NULL;
    prim->indicatorValue        = NULL;
    if (result == CSR_BT_CME_SUCCESS)
    {
        if (linkPtr->cindSupportString != NULL)
        {
            CsrUint16 length = (CsrUint16)CsrStrLen((char *)linkPtr->cindSupportString)+1;
            prim->indicatorSupported = CsrPmemAlloc(length);
            CsrMemCpy(prim->indicatorSupported,linkPtr->cindSupportString,length);
            prim->indicatorSupported[length-1] = 0;
        }
        if (linkPtr->cindStartValueString != NULL)
        {
            CsrUint16 length = (CsrUint16)CsrStrLen((char *)linkPtr->cindStartValueString)+1;
            prim->indicatorValue = CsrPmemAlloc(length);
            CsrMemCpy(prim->indicatorValue,linkPtr->cindStartValueString,length);
            prim->indicatorValue[length-1] = 0;
        }
    }

    prim->cmeeResultCode = result;

    CsrBtHfMessagePut(instData->appHandle, prim);
}


/*************************************************************************************
    Send a CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM to app
************************************************************************************/
void CsrBtHfSendUpdateCodecSupportedCfm(HfMainInstanceData_t *instData)
{
    CsrBtHfUpdateSupportedCodecCfm *prim = (CsrBtHfUpdateSupportedCodecCfm *)CsrPmemAlloc(sizeof(CsrBtHfUpdateSupportedCodecCfm));
   
    prim->type = CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM;
    prim->resultCode     = CSR_BT_RESULT_CODE_HF_SUCCESS;
    prim->resultSupplier = CSR_BT_SUPPLIER_HF;

    CsrBtHfMessagePut(instData->appHandle, prim);
}


static const audioSetupParams_t scoNegotiationOrder[] =
{
    {
        CSR_BT_ESCO_DEFAULT_CONNECT_AUDIO_QUALITY,
        CSR_BT_ESCO_DEFAULT_CONNECT_TX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_CONNECT_RX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_CONNECT_MAX_LATENCY,
        CSR_BT_ESCO_DEFAULT_CONNECT_VOICE_SETTINGS,
        CSR_BT_ESCO_DEFAULT_CONNECT_RE_TX_EFFORT,
        CSR_BT_ESCO_DEFAULT_CONNECT,
        0,
        FALSE
    },

    {
        CSR_BT_ESCO_DEFAULT_2P0_S3_AUDIO_QUALITY,
        CSR_BT_ESCO_DEFAULT_2P0_S3_TX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_2P0_S3_RX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_2P0_S3_MAX_LATENCY,
        CSR_BT_ESCO_DEFAULT_2P0_S3_VOICE_SETTINGS,
        CSR_BT_ESCO_DEFAULT_2P0_S3_RE_TX_EFFORT,
        CSR_BT_ESCO_DEFAULT_2P0_S3,
        0,
        FALSE
    },

    {
        CSR_BT_ESCO_DEFAULT_2P0_S2_AUDIO_QUALITY,
        CSR_BT_ESCO_DEFAULT_2P0_S2_TX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_2P0_S2_RX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_2P0_S2_MAX_LATENCY,
        CSR_BT_ESCO_DEFAULT_2P0_S2_VOICE_SETTINGS,
        CSR_BT_ESCO_DEFAULT_2P0_S2_RE_TX_EFFORT,
        CSR_BT_ESCO_DEFAULT_2P0_S2,
        0,
        FALSE
    },

    {
        CSR_BT_ESCO_DEFAULT_1P2_S1_AUDIO_QUALITY,
        CSR_BT_ESCO_DEFAULT_1P2_S1_TX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_1P2_S1_RX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_1P2_S1_MAX_LATENCY,
        CSR_BT_ESCO_DEFAULT_1P2_S1_VOICE_SETTINGS,
        CSR_BT_ESCO_DEFAULT_1P2_S1_RE_TX_EFFORT,
        CSR_BT_ESCO_DEFAULT_1P2_S1,
        0,
        FALSE
    },

    {
        CSR_BT_SCO_DEFAULT_1P1_AUDIO_QUALITY,
        CSR_BT_SCO_DEFAULT_1P1_TX_BANDWIDTH,
        CSR_BT_SCO_DEFAULT_1P1_RX_BANDWIDTH,
        CSR_BT_SCO_DEFAULT_1P1_MAX_LATENCY,
        CSR_BT_SCO_DEFAULT_1P1_VOICE_SETTINGS,
        CSR_BT_SCO_DEFAULT_1P1_RE_TX_EFFORT,
        CSR_BT_SCO_DEFAULT_1P1,
        0,
        FALSE
    },
};

/*************************************************************************************
    Find the number of negotiations we need to do according to the start parameter 'start_setting'
************************************************************************************/
static CsrUint16 csrBtHfGetNegotiateCount(CsrUint8 start_setting)
{
    if (start_setting == CSR_BT_SCO_DEFAULT_1P1)
    {
        return 1;
    }
    else
    {
        return CSR_ARRAY_SIZE(scoNegotiationOrder);
    }
}

/*************************************************************************************
    Find the offset to the given start parameter 'start_setting'
************************************************************************************/
static CsrUint8 csrBtHfGetNegotiateOffset(CsrUint8 start_setting)
{
    if (start_setting == CSR_BT_SCO_DEFAULT_1P1)
    {
        /* The last entry in the negotiation order */
        return CSR_ARRAY_SIZE(scoNegotiationOrder)-1;
    }
    else
    {
        /* Skip the user specified parameters. They need to be handled differently */
        return 1;
    }
}

/*************************************************************************************
    Add the default (user specified) SCO parameters to the list.
************************************************************************************/
static void csrBtHfHandleDefaultConnectScoParms(HfInstanceData_t *instData, CsrBtCmScoCommonParms *parms, CsrUint16 *parmsOffset, CsrUint8 default_setting)
{
    if (default_setting == CSR_BT_ESCO_DEFAULT_CONNECT)
    {
        hci_pkt_type_t audioQuality = scoNegotiationOrder[0].theAudioQuality;

        if((CSR_BT_ESCO_DEFAULT_CONNECT_AUDIO_QUALITY & 0x03F8) != 0x03C0)
        {
            /* If the user has specified valid eSCO packet types to try in csr_bt_usr_config.h then only try the
            eSCO types because the spec. requires that we at least try S1 before defaulting to SCO */
            audioQuality = CSR_BT_ESCO_DEFAULT_CONNECT_AUDIO_QUALITY & 0x03F8;
        }
        else
        { /* Otherwise just use the SCO settings the user has specified and make sure we dont try
            other eSCO settings */
            audioQuality = CSR_BT_ESCO_DEFAULT_CONNECT_AUDIO_QUALITY;
        }

        CsrBtCmCommonScoConnectBuild(parms,
                                parmsOffset,
                                audioQuality,
                                scoNegotiationOrder[0].theTxBandwidth,
                                scoNegotiationOrder[0].theRxBandwidth,
                                scoNegotiationOrder[0].theMaxLatency,
                                scoNegotiationOrder[0].theVoiceSettings,
                                scoNegotiationOrder[0].theReTxEffort);
    }
}

void CsrBtHfSendCmScoConnectReq(HfInstanceData_t *instData, CsrUint8 default_setting, audioSetupParams_t *audioParms)
{
    CsrBtCmScoCommonParms *parms;
    CsrUint16 parmsOffset;
    CsrUint16 parmsLen;
    CsrUintFast32 idx;

    CsrUint8 pcmSlot       = audioParms ? audioParms->pcmSlot : instData->pcmSlot;
    CsrBool  pcmReassign   = audioParms ? audioParms->pcmReassign : instData->pcmReassign;
    /* Do we need to handle the parms specified by the app? */
    CsrUint16 appParmCount = audioParms ? 1 : 0;
    appParmCount         += csrBtHfGetNegotiateCount(default_setting);

    CsrBtCmCommonScoConnectPrepare(&parms, &parmsOffset, &parmsLen, appParmCount);

    if (audioParms)
    {
        /* Handle the parameters from the app if any */
        CsrBtCmCommonScoConnectBuild(parms,
                                &parmsOffset,
                                audioParms->theAudioQuality,
                                audioParms->theTxBandwidth,
                                audioParms->theRxBandwidth,
                                audioParms->theMaxLatency,
                                audioParms->theVoiceSettings,
                                audioParms->theReTxEffort);
    }

    csrBtHfHandleDefaultConnectScoParms(instData, parms, &parmsOffset, default_setting);

    for (idx = csrBtHfGetNegotiateOffset(default_setting); idx < CSR_ARRAY_SIZE(scoNegotiationOrder); ++idx)
    {
        CsrBtCmCommonScoConnectBuild(parms,
                                &parmsOffset,
                                scoNegotiationOrder[idx].theAudioQuality,
                                scoNegotiationOrder[idx].theTxBandwidth,
                                scoNegotiationOrder[idx].theRxBandwidth,
                                scoNegotiationOrder[idx].theMaxLatency,
                                scoNegotiationOrder[idx].theVoiceSettings,
                                scoNegotiationOrder[idx].theReTxEffort);
    }
#ifdef CSR_USE_DSPM
    pcmSlot = CSR_BT_PCM_DONT_MAP;
#endif
    CsrBtCmScoConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                        pcmSlot,
                        pcmReassign,
                        parms,
                        parmsLen,
                        instData->hfConnId);
}

/*************************************************************************************
    Send CSR_BT_CM_SDS_REGISTER_REQ to register service record for HF
************************************************************************************/
void CsrBtHfSendSdsRegisterReq(HfMainInstanceData_t *instData)
{
    /* make sure that the HS is not in the precess of registering */
    if (instData->linkRegistering == CSR_BT_HF_CONNECTION_UNKNOWN)
    {
        HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
        CsrUint8 * record;
        CsrUint16 num_rec_bytes;

        instData->linkRegistering = CSR_BT_HF_CONNECTION_HF;
        /* copy the record */
        num_rec_bytes = sizeof(sdsHfServiceRecord);
        record = (CsrUint8 *) CsrPmemAlloc(num_rec_bytes );
        CsrMemCpy(record, sdsHfServiceRecord, num_rec_bytes);

        /* insert the server channel found during register in appropriate place */
        record[HF_SERVER_CHANNEL_INDEX] = linkPtr->serverChannel;
        record[HF_SUPPORTED_FEATURES_INDEX + 1] = (CsrUint8)((instData->localSupportedFeatures) & CSR_BT_HFP_SDP_SUPPORT_MASK);
        if (instData->localSupportedFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION)
        {
            record[HF_SUPPORTED_FEATURES_INDEX + 1] |= CSR_BT_HFP_SDP_CODEC_NEGOTIATION;
        }
        CsrBtCmSdsRegisterReqSend(CSR_BT_HF_IFACEQUEUE, record, num_rec_bytes);
    }
    else
    {
        /* else set flag to indicate that HF has tried to register */
        instData->registerPending = TRUE;
    }
}

/*************************************************************************************
    Send CSR_BT_CM_SDS_REGISTER_REQ to register service record for HS
************************************************************************************/
void HsSendSdsRegisterReq(HfMainInstanceData_t *instData)
{
    if (instData->linkRegistering == CSR_BT_HF_CONNECTION_UNKNOWN)
    {
        CsrUint8 * record;
        CsrUint16 num_rec_bytes;

        instData->linkRegistering = CSR_BT_HF_CONNECTION_HS;
        /* copy the record */
        num_rec_bytes = sizeof(sdsHsServiceRecord);
        record = (CsrUint8 *) CsrPmemAlloc(num_rec_bytes );
        CsrMemCpy(record, sdsHsServiceRecord, num_rec_bytes);

        /* insert the server channel found during register in appropriate place */
        record[HS_SERVER_CHANNEL_INDEX] = instData->linkData[instData->index].serverChannel;
        record[HS_REMOTE_AUDIO_INDEX] = !(instData->mainConfig & CSR_BT_HF_CNF_DISABLE_REMOTE_VOLUME_CONTROL);
        CsrBtCmSdsRegisterReqSend(CSR_BT_HF_IFACEQUEUE, record, num_rec_bytes);
    }
    else
    {
        instData->registerPending = TRUE;
    }
}

/*************************************************************************************
*   Deregister timeout handling
************************************************************************************/
void HsDeregisterTimeout(CsrUint16 mi, void *mv)
{
    HfMainInstanceData_t *instData = (HfMainInstanceData_t*)mv;

    instData->linkData[instData->index].deregisterTimerId = 0;
    CsrBtCmSdsUnRegisterReqSend(CSR_BT_HF_IFACEQUEUE, instData->linkData[instData->index].serviceRecHandle);
}

void sendBia(HfMainInstanceData_t * instData)
{
    CsrUintFast16   i = 1, len;
    CsrUint8    *body;
    CsrBtHfIndicatorActivationReq *prim = (CsrBtHfIndicatorActivationReq *)instData->recvMsgP;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    len = (CsrUint16)((linkPtr->nrOfIndicators *2) + BIA_CMD_INDEX);
    /* Allocate space enough for a message with the number of indicators supported by the remote device */
    body = CsrPmemAlloc(len+1);
    CsrMemSet(body,0,len+1);
    CsrMemCpy(body, BIA_CMD, BIA_CMD_INDEX);
    for (i=BIA_CMD_INDEX; i<len ; i++)
    {
        CsrUint8 value = (prim->indicatorBitMask & 1) + '0';
        body[i] = value;
        i++;
        if (i<(len-1))
        {
            body[i] = ',';
            /* shift value one bit (divide by two) to get the next bit value */
            prim->indicatorBitMask  >>= 1;
        }
        else
        {
            body[i] = '\r';
        }
    }
    body[i] = 0;
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = bia;
    }
    CsrBtHfHsSendCmDataReq(instData,(CsrUint16) len, body);
}

void sendBrsf(HfMainInstanceData_t * instData)
{
    CsrUint16    length;
    CsrUint8    *body;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (linkPtr->localSupportedFeatures >= 100)
    {
        length = SUPPORT_FEATURES_LENGTH;
        body = CsrPmemAlloc(length);
        CsrMemCpy(body, SUPPORT_FEATURES, length);
        body[8]  = (CsrUint8)(48 +  linkPtr->localSupportedFeatures / 100);
        body[9]  = (CsrUint8)(48 + ((linkPtr->localSupportedFeatures - ((body[8]-48)*100)) / 10));
        body[10] = (CsrUint8)(48 + (linkPtr->localSupportedFeatures % 10));
    }
    else if (linkPtr->localSupportedFeatures >= 10)
    {
        length = SUPPORT_FEATURES_LENGTH - 1;
        body = CsrPmemAlloc(length);
        CsrMemCpy(body, SUPPORT_FEATURES, length);
        body[8]  = (CsrUint8)(48 +  linkPtr->localSupportedFeatures / 10);
        body[9]  = (CsrUint8)(48 + (linkPtr->localSupportedFeatures % 10));
        body[10] = '\r';
    }
    else   /* localSupportedFeatures < 10 */
    {
        length = SUPPORT_FEATURES_LENGTH - 2;
        body = CsrPmemAlloc(length);
        CsrMemCpy(body, SUPPORT_FEATURES, length);
        body[8]  = (CsrUint8)(48 + (linkPtr->localSupportedFeatures));
        body[9]  = '\r';
    }
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = bsrf;
    }
    CsrBtHfHsSendCmDataReq(instData, length, body);
}

CsrUint8 CsrBtHfgNumberOfXtraCodecsSupported(HfMainInstanceData_t * instData)
{
    CsrUint8 codecs = 0;
    CsrUint8 i;
    
    for (i = 1; i < sizeof(instData->supportedCodecsMask); i++)
    {
        if ((instData->supportedCodecsMask & (1 << i)) > 0 )
        {
            codecs++;
        }
    }
    return codecs;
}

void sendCodecSupport(HfMainInstanceData_t * instData)
{
    CsrUint8        *body;
    CsrUint16       payloadLen;
    CsrUint8        nrXtraCodecs = CsrBtHfgNumberOfXtraCodecsSupported(instData);
    
    payloadLen = CODEC_SUPPORT_LENGTH + (CODEC_SUPPORT_LENGTH_PER_XTRA_CODEC * nrXtraCodecs);
    body = CsrPmemAlloc(payloadLen);
    CsrMemCpy(body, CODEC_SUPPORT, CODEC_SUPPORT_LENGTH);
    if (nrXtraCodecs)
    {/* Now we need to add the extra codecs to the string */
        CsrUint16 i;
        CsrUint16 idx = CODEC_SUPPORT_FIRST_XTRA_CODEC_INDEX;
        body[payloadLen-1] = '\r'; /* make sure to remember the termination character, which needs to be moved when new codecs added */
        for (i = 0; nrXtraCodecs > 0; i++)
        {
            CsrUint8 charVal = (CsrUint8)(instData->supportedCodecsMask & (1 << (i+1)));
            
            if (charVal > 0 )
            {
                body[idx] = ',';
                body[idx+1] = (CsrUint8)(charVal + '0'); /* ASCII value */
                idx += CODEC_SUPPORT_LENGTH_PER_XTRA_CODEC;
                nrXtraCodecs--;
            }
        }
    }

    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = bac;
    }
    CsrBtHfHsSendCmDataReq(instData, payloadLen, body);
}

void CsrBtHfSendAtBcc(HfMainInstanceData_t *instData)
{
    CsrUint8    *body = CsrPmemAlloc(BCC_CMD_LENGTH);
    
    CsrMemCpy(body,BCC_CMD,BCC_CMD_LENGTH);
    
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = bcc;
    }
    CsrBtHfHsSendCmDataReq(instData, BCC_CMD_LENGTH, body);
}

void CsrBtHfSendAtBcs(HfMainInstanceData_t *instData)
{
    CsrUint8    *body = CsrPmemAlloc(BCS_CMD_LENGTH);
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    CsrMemCpy(body,BCS_CMD,BCS_CMD_LENGTH);
    body[BCS_CMD_INDEX] = linkPtr->codecToUse + '0';
    
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = bcs;
    }
    CsrBtHfHsSendCmDataReq(instData, BCS_CMD_LENGTH, body);
}

void sendCindSupport(HfMainInstanceData_t * instData)
{
    CsrUint8        *body;

    body = CsrPmemAlloc(CIND_SUPPORT_LENGTH);
    CsrMemCpy(body, CIND_SUPPORT, CIND_SUPPORT_LENGTH);
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = cindSupportCmd;
    }
    CsrBtHfHsSendCmDataReq(instData, CIND_SUPPORT_LENGTH, body);
}

void sendCindStatus(HfMainInstanceData_t * instData)
{
    CsrUint8        *body;

    body = CsrPmemAlloc(CIND_STATUS_LENGTH);
    CsrMemCpy(body, CIND_STATUS, CIND_STATUS_LENGTH);
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = cindStatusCmd;
    }
    CsrBtHfHsSendCmDataReq(instData, CIND_STATUS_LENGTH, body);
}

void sendSetCmer(HfMainInstanceData_t * instData,CsrBool enable)
{
    CsrUint8        *body;

    body = CsrPmemAlloc(SET_CMER_LENGTH);
    if (enable)
    {
        CsrMemCpy(body, SET_CMER, SET_CMER_LENGTH);
    }
    else
    {
        CsrMemCpy(body, RESET_CMER, SET_CMER_LENGTH);
    }

    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = cmer;
    }
    CsrBtHfHsSendCmDataReq(instData, SET_CMER_LENGTH, body);
}

void sendCallHoldStatus(HfMainInstanceData_t * instData)
{
    CsrUint8        *body;

    body = CsrPmemAlloc(CALL_HOLD_STATUS_LENGTH);
    CsrMemCpy(body, CALL_HOLD_STATUS, CALL_HOLD_STATUS_LENGTH);
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = chldSupport;
    }
    CsrBtHfHsSendCmDataReq(instData, CALL_HOLD_STATUS_LENGTH, body);
}

void startSdcFeatureSearch(HfMainInstanceData_t * instData, CsrBool outgoing)
{

    CmnCsrBtLinkedListStruct *sdpTagList = NULL;
    CsrUint16 shIndex;
    CsrBtDeviceAddr deviceAddr = instData->currentDeviceAddress;
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    /* Find first available entry if outgoing...*/
    if (outgoing)
    {
        CsrBtHfServiceConnectReq * prim;
        CsrBool stop = FALSE;
        CsrIntFast8 i =0;

        prim = (CsrBtHfServiceConnectReq *) instData->recvMsgP;

        if ((prim->connectionType == CSR_BT_HF_CONNECTION_HF) || (prim->connectionType == CSR_BT_HF_CONNECTION_UNKNOWN))
        {
            for (i=0; ((i< instData->maxHFConnections) && (!stop)); i++)
            {
                linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);

                if (linkPtr->linkState == CSR_BT_LINK_STATUS_DISCONNECTED)
                { /* hf Index found */
                    instData->searchIdxHf = (CsrUint8)i;
                    linkPtr->searchOngoing = TRUE;
                    stop = TRUE;
                }
            }
        }
        stop = FALSE;
        if ((prim->connectionType == CSR_BT_HF_CONNECTION_HS) || (prim->connectionType == CSR_BT_HF_CONNECTION_UNKNOWN))
        {/* Now find HS index if HS connection to be tried...*/
            for (i=instData->maxHFConnections; ((i < (instData->maxHFConnections + instData->maxHSConnections)) && (!stop)); i++)
            {
                linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                if (linkPtr->linkState == CSR_BT_LINK_STATUS_DISCONNECTED)
                {
                    instData->searchIdxHs = (CsrUint8)i;
                    linkPtr->searchOngoing = TRUE;
                    stop = TRUE;
                }
            }
        }
    }
    else if ((linkPtr->searchOngoing == FALSE) &&
        ((!outgoing) || (linkPtr->linkState == CSR_BT_LINK_STATUS_DISCONNECTED) ))
    {/* Perform HF search if not performing HF search already and
        either trying to connect or already connected in an incoming connection */
        linkPtr->searchOngoing = TRUE;
        if (instData->index < instData->maxHFConnections)
        {
            instData->searchIdxHf = (CsrUint8)instData->index;
            instData->searchIdxHs = 0xFF;
        }
        else
        {
            instData->searchIdxHs = (CsrUint8)instData->index;
            instData->searchIdxHf = 0xFF;
        }
    }

    if (instData->searchIdxHf != 0xFF)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->searchIdxHf]);
        if (linkPtr->searchOngoing)
        {
            sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_HFG_PROFILE_UUID, &shIndex);

            if (NULL != sdpTagList)
            {
                linkPtr->searchAndCon = outgoing;
                linkPtr->serviceHandleListCount          = 0;
                /* Set default values before search operation, to ensure spec compliance if search fails or needs not be performed.*/
                linkPtr->supportedFeatures      = HF_HFG_DEFAULT_SUPPORTED_FEATURES;
                linkPtr->network                = 1; /* Default according to HFP spec.*/
                CsrUtf8StrNCpyZero(linkPtr->serviceName, (CsrUtf8String*)"Audio Gateway",CSR_BT_MAX_FRIENDLY_NAME_LEN + 1);
                CsrUtf8StrTruncate((CsrUtf8String*)linkPtr->serviceName, CSR_BT_MAX_FRIENDLY_NAME_LEN);

                CsrBtUtilSdrInsertLocalServerChannel(sdpTagList, shIndex, linkPtr->serverChannel);

                if (linkPtr->searchAndCon)
                { /* Outgoing connection: make sure to find the remote server channel to use!*/
                    CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);
                    /* Now choose the attributes to search for.... */
                    if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_OUT_SDP_SEARCH) == 0)
                    {/* Search not disabled: now add tags for attributes to search. Always search for remote protocol verison number at least */
                        if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_OUT_SUP_FEATURES_SEARCH) == 0)
                        {/* Supported features search not disabled */
                            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SUPPORTED_FEATURES_ATTRIBUTE_IDENTIFIER, NULL, 0);
                        }
                        if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_OUT_NETWORK_SEARCH) == 0)
                        {/* Network attribute search not disabled */
                            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_NETWORK_ATTRIBUTE_IDENTIFIER, NULL, 0);
                        }
                        if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_OUT_SERVICE_NAME_SEARCH) == 0)
                        {/* Service name search not disabled */
                            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, NULL, 0);
                        }
                    }
                }
                else
                { /* This is an incoming HF connection; find out what to look for (if anything at all!) */
                    if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_INC_SDP_SEARCH) == 0)
                    {/* search enabled */
                        CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);
                        if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_INC_SUP_FEATURES_SEARCH) == 0)
                        {/* Supported features search not disabled */
                            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SUPPORTED_FEATURES_ATTRIBUTE_IDENTIFIER, NULL, 0);
                        }
                        if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_INC_NETWORK_SEARCH) == 0)
                        {/* Network attribute search not disabled */
                            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_NETWORK_ATTRIBUTE_IDENTIFIER, NULL, 0);
                        }
                        if ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_INC_SERVICE_NAME_SEARCH) == 0)
                        {/* Service name search not disabled */
                            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, NULL, 0);
                        }
                    }
                }
                if (linkPtr->searchAndCon)
                {
                    linkPtr->oldState = Connect_s;
                    linkPtr->currentDeviceAddress = instData->currentDeviceAddress;
                }
                deviceAddr = linkPtr->currentDeviceAddress;
                linkPtr->state = ServiceSearch_s;
            }
            else
            {
                linkPtr->searchOngoing = FALSE;
            }
        }
    }


    if (instData->searchIdxHs != 0xFF)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->searchIdxHs]);
        if (linkPtr->searchOngoing)
        {
            sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_HEADSET_AG_SERVICE_UUID, &shIndex);
            if (NULL != sdpTagList)
            {
                linkPtr->searchAndCon  = outgoing;
                linkPtr->serviceHandleListCount = 0;

                CsrBtUtilSdrInsertLocalServerChannel(sdpTagList, shIndex, linkPtr->serverChannel);

                if (linkPtr->searchAndCon)
                {
                    linkPtr->oldState = Activate_s;
                    linkPtr->currentDeviceAddress = instData->currentDeviceAddress;
                    linkPtr->state = Connect_s;
                }
            }
            else
            {
                linkPtr->searchOngoing = FALSE;
            }
        }
    }

   if ((instData->searchIdxHs != 0xFF) || (instData->searchIdxHf != 0xFF))
   {
       if (instData->searchIdxHf != 0xFF)
       {
           instData->index = instData->searchIdxHf;
           linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->searchIdxHf]);
       }
       else
       {
           instData->index = instData->searchIdxHs;
           linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->searchIdxHs]);
       }

       if (outgoing)
        {
            CsrBtUtilRfcConStart((void *)instData, instData->sdpHfSearchConData, sdpTagList,deviceAddr,
                            instData->secOutgoing, FALSE, NULL,
                            CSR_BT_HF_PROFILE_DEFAULT_MTU_SIZE,
                            instData->modemStatus, 0);
        }
        else
        {
            CsrBtUtilSdcSearchStart((void *)instData, instData->sdpHfSearchData, sdpTagList, deviceAddr);
        }
   }
   else
   {/* search not started! If outgoing, let the app know */
       if (outgoing)
       {
           CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SDC_SEARCH_FAILED, CSR_BT_SUPPLIER_HF);
       }
   }

}

static CsrBool csrBtHfSdpGetBluetoothProfileDescriptorList(CmnCsrBtLinkedListStruct *bll_p,
                                                        CsrUint16  serviceHandleIndex,
                                                        CsrUint16  *version)
{
    CsrBool    retBool = FALSE;

    CsrUint8  *att_p;
    CsrUintFast16 nofAttributes,x;
    CsrUint16   attDataLen, nofBytesToAttribute, emptyAttSize, consumedBytes, totalConsumedBytes = 0, tempVar;
    CsrUint32  returnValue, protocolValue;

    if (TRUE == CsrBtUtilSdrGetNofAttributes(bll_p, serviceHandleIndex, &nofAttributes))
    {
        for (x=0; x<nofAttributes; x++)
        {
            att_p = CsrBtUtilSdrGetAttributePointer(bll_p, serviceHandleIndex,(CsrUint16) x, &nofBytesToAttribute);
            /* Check if the UUID in the 'outer' attribute struct is correct */
            CsrMemCpy(&tempVar, att_p + SDR_ENTRY_INDEX_ATTRIBUTE_UUID, SDR_ENTRY_SIZE_SERVICE_UINT16);
            if (CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER == tempVar)
            {
                CsrBtUtilSdrGetEmptyAttributeSize(&emptyAttSize);
                CsrMemCpy(&tempVar, att_p, SDR_ENTRY_SIZE_SERVICE_UINT16);
                attDataLen = tempVar - emptyAttSize + SDR_ENTRY_SIZE_TAG_LENGTH;

                /* First extract the attribute uuid from the attribute SDP data */
                if (TRUE == CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA,
                                              attDataLen,
                                              &returnValue,
                                              &consumedBytes,
                                              FALSE))
                {
                    /* Check if the UUID in the 'inner' attribute sdp data struct is correct */
                    if (CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER == returnValue)
                    {
                        attDataLen = attDataLen - consumedBytes;
                        totalConsumedBytes += consumedBytes;
                        /* first find the protocol UUID */
                        if (TRUE == CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes,
                                                      attDataLen,
                                                      &protocolValue,
                                                      &consumedBytes,
                                                      TRUE))
                        {
                            attDataLen = attDataLen - consumedBytes;
                            totalConsumedBytes += consumedBytes;
                            /* Now find the value */
                            if (TRUE == CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes,
                                                          attDataLen,
                                                          &returnValue,
                                                          &consumedBytes,
                                                          TRUE))
                            {
                                attDataLen = attDataLen - consumedBytes;
                                totalConsumedBytes += consumedBytes;

                                if (CSR_BT_HF_PROFILE_UUID == protocolValue)
                                {
                                    *version = (CsrUint16)returnValue;
                                    retBool = TRUE;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return retBool;
}

static void csrBtHfUtilCarryOnAfterSdp(HfMainInstanceData_t *instData,
                                       CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* This function is only called if the connection establsihement failed or if it succeeded and this is a HF connection */
    HfInstanceData_t    *linkPtr;
    if (instData->searchIdxHf != 0xff)
    {
       instData->index = instData->searchIdxHf;
    }
    if (instData->searchIdxHs != 0xff)
    {
       linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->searchIdxHs]);
       if (linkPtr->state == Connect_s)
       {
           linkPtr->state = Activate_s;
       }
       if (instData->searchIdxHf == 0xFF)
       {
            instData->index = instData->searchIdxHs;
       }
    }
    linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    instData->searchIdxHf = 0xFF;
    instData->searchIdxHs = 0xFF;

    if (resultCode == CSR_BT_RESULT_CODE_HF_SUCCESS &&
        resultSupplier == CSR_BT_SUPPLIER_HF)
    {
        if ((linkPtr->disconnectReqReceived) || (linkPtr->pendingCancel))
        {
            if (linkPtr->disconnectPeerReceived)
            {
                CsrBtHfSendHfDisconnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                CsrBtHfSaveQueueCleanUp(instData);
                linkPtr->linkState = CSR_BT_LINK_STATUS_DISCONNECTED;
                linkPtr->state = Activate_s;
                CsrBtHfAllowConnectCheck(instData);
            }
            else if(linkPtr->disconnectReqReceived)
            {
                CsrBtCmDisconnectReqSend(linkPtr->hfConnId);
            }
            else
            { /* pendingCancel */
                if (linkPtr->state == Connected_s)
                {
                    CsrBtHfSendHfDisconnectInd(instData, CSR_BT_RESULT_CODE_HF_CANCELLED_CONNECT_ATTEMPT, CSR_BT_SUPPLIER_HF);
                }
                else
                {
                    /* App does not not about connection, so simply
                     * make it look like it was cancelled */
                    CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_CANCELLED_CONNECT_ATTEMPT, CSR_BT_SUPPLIER_HF);
                }
                CsrBtHfSaveQueueCleanUp(instData);
                linkPtr->linkState = CSR_BT_LINK_STATUS_DISCONNECTED;
                linkPtr->state = Activate_s;
                CsrBtHfAllowConnectCheck(instData);
                CsrBtHfInitInstanceData(instData);
            }
        }
        else
        {
            if (linkPtr->disconnectPeerReceived)
            {
                CsrBtHfSaveQueueCleanUp(instData);
                linkPtr->linkState = CSR_BT_LINK_STATUS_DISCONNECTED;
                linkPtr->state = Activate_s;
                if (linkPtr->oldState == Connect_s)
                {
                    CsrBtHfSendHfDisconnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                }
                CsrBtHfAllowConnectCheck(instData);
                CsrBtHfInitInstanceData(instData);
            }
            else
            {
                /* SDC finished. */
                if ((linkPtr->oldState == Activate_s) || (linkPtr->oldState == Connect_s))
                {
                    linkPtr->serviceState = btConnect_s;

                    /*If TRANSPARENT mode, then the App will send BRSF */
                    if ((instData->mainConfig & CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE) == CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE)
                    {
                        linkPtr->state = Connected_s;
                        linkPtr->atSequenceState = serviceLevel; /* Must set to guarantee that BT_HF_DISCONNECT_IND is issued properly */
                        CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF); 
                        CsrBtHfAcceptIncomingSco(instData);
                        /* Make sure not to allow more connections than indicated by the application */
                        CsrBtHfCancelAcceptCheck(instData);
                    }
                    else
                    {
                        startAtFeatureSearch(instData);
                    }
                }
            }
        }
    }
    else
    {
        if ((linkPtr->disconnectPeerReceived) || (linkPtr->pendingCancel))
        {
            /* other side already disconnect rfcomm link */
            CsrBtHfSaveQueueCleanUp(instData);
            linkPtr->linkState = CSR_BT_LINK_STATUS_DISCONNECTED;
            linkPtr->state = Activate_s;
            CsrBtHfAllowConnectCheck(instData);
            if ((linkPtr->oldState == Connect_s) && (!linkPtr->pendingCancel))
            {
                CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_CONNECT_ATTEMPT_FAILED, CSR_BT_SUPPLIER_HF);
            }
            else if (linkPtr->pendingCancel)
            {
                /* App does not not about connection, so simply
                 * make it look like it was cancelled */
                CsrBtHfSendHfServiceConnectInd(instData,CSR_BT_RESULT_CODE_HF_CANCELLED_CONNECT_ATTEMPT, CSR_BT_SUPPLIER_HF);
            }
            CsrBtHfInitInstanceData(instData);
        }
        else
        {
            if (linkPtr->oldState == Activate_s)
            {
                /* SDC search failed, but rfcomm connection already established. Use default features for AG and start AT sequence */
                linkPtr->supportedFeatures = HF_HFG_DEFAULT_SUPPORTED_FEATURES;
                linkPtr->network = HF_HFG_DEFAULT_NETWORK_FEATURES;
                linkPtr->serviceState = btConnect_s;

                /*If TRANSPARENT mode, then the App will send BRSF/AT features */
                if ((instData->mainConfig & CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE) == CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE)
                {
                    linkPtr->state = Connected_s;
                    linkPtr->atSequenceState = serviceLevel; /* Must set to guarantee that BT_HF_DISCONNECT_IND is issued properly */
                    CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    CsrBtHfAcceptIncomingSco(instData);
                    /* Make sure not to allow more connections than indicated by the application */
                    CsrBtHfCancelAcceptCheck(instData);
                }
                else
                {
                    startAtFeatureSearch(instData);
                }
            }
            else
            {
                CsrBtHfSaveQueueCleanUp(instData);
                linkPtr->linkState = CSR_BT_LINK_STATUS_DISCONNECTED;
                linkPtr->state = Activate_s;
                CsrBtHfAllowConnectCheck(instData);
                CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SDC_SEARCH_FAILED, CSR_BT_SUPPLIER_HF);
            }
        }
    }
}


void CsrBtHfSdcResultHandler(void                     * inst,
                             CmnCsrBtLinkedListStruct * sdpTagList,
                             CsrBtDeviceAddr          deviceAddr,
                             CsrBtResultCode          resultCode,
                             CsrBtSupplier      resultSupplier)
{
    HfMainInstanceData_t *instData = (HfMainInstanceData_t *) inst;
    CsrBtUuid32    tmpUuid = 0;
    CsrBtHfConnectionType   localConnectionType = CSR_BT_HF_CONNECTION_UNKNOWN;
    HfInstanceData_t    *linkPtr = instData->linkData;

    if (instData->searchIdxHf != 0xFF)
    {
        if (linkPtr[instData->searchIdxHf].searchOngoing == TRUE)
        {
            instData->index = instData->searchIdxHf;
            localConnectionType = CSR_BT_HF_CONNECTION_HF;
        }
        else
        {
            instData->index = instData->searchIdxHs;
            localConnectionType = CSR_BT_HF_CONNECTION_HS;
        }
    }
    else
    {
        if (instData->searchIdxHs != 0xFF)
        {
            instData->index = instData->searchIdxHs;
        }
        localConnectionType = CSR_BT_HF_CONNECTION_HS;
    }


    if (instData->searchIdxHf != 0xFF)
    {
        linkPtr[instData->searchIdxHf].searchOngoing = FALSE;
        linkPtr[instData->searchIdxHf].searchAndCon = FALSE;
    }
    if (instData->searchIdxHs != 0xFF)
    {
        linkPtr[instData->searchIdxHs].searchOngoing = FALSE;
        linkPtr[instData->searchIdxHs].searchAndCon = FALSE;
    }

    linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (!linkPtr->disconnectReqReceived && !linkPtr->disconnectPeerReceived && !linkPtr->pendingCancel)
    {
        if (resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
            resultSupplier == CSR_BT_SUPPLIER_CM  &&
            sdpTagList != NULL)
        {
            CsrUintFast16    numOfSdpRecords = CsrBtUtilBllGetNofEntriesEx(sdpTagList);
            CsrUintFast16    sdpRecordIndex;
            CsrUint16    tmpResult;
            CsrUint16    dummy1, dummy2; /* Currently CSR_UNUSED */
            CsrUint32    returnValue;
            CsrUint8     *string;
            CsrUint16    stringLen;
            CsrUint16    version = 0;

            for (sdpRecordIndex = 0; sdpRecordIndex < numOfSdpRecords; sdpRecordIndex++)
            {
                if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList,
                                                sdpRecordIndex,
                                                &tmpUuid,
                                                &tmpResult,
                                                &dummy1,
                                                &dummy2))
                {
                    if (tmpResult == SDR_SDC_SEARCH_SUCCESS)
                    {
                        CsrBtUuid32 serviceHandleTemp = 0;

                        if (tmpUuid == CSR_BT_HFG_PROFILE_UUID)
                        { /* Handsfree connection */
                           localConnectionType = CSR_BT_HF_CONNECTION_HF;
                           if (TRUE == CsrBtUtilSdrGetServiceHandle(sdpTagList, sdpRecordIndex, &serviceHandleTemp))
                           {
                               linkPtr->serviceHandle = serviceHandleTemp;
                           }

                          if (TRUE == CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(sdpTagList, sdpRecordIndex,
                                                        CSR_BT_SUPPORTED_FEATURES_ATTRIBUTE_IDENTIFIER, &returnValue))
                          {
                              linkPtr->supportedFeatures = (CsrUint16) returnValue;
                              /* Make sure that the "global" local supported features are passed on to the link... */
                              linkPtr->localSupportedFeatures = instData->localSupportedFeatures;
                          }
                          if (TRUE == CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(sdpTagList, sdpRecordIndex,
                                                        CSR_BT_NETWORK_ATTRIBUTE_IDENTIFIER, &returnValue))
                          {
                              linkPtr->network = (CsrUint8) returnValue;
                          }
                          if (TRUE == CsrBtUtilSdrGetStringAttributeFromAttributeUuid(sdpTagList, sdpRecordIndex, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, &string, &stringLen))
                          {
                              if (stringLen > CSR_BT_MAX_FRIENDLY_NAME_LEN)
                              {
                                  stringLen = CSR_BT_MAX_FRIENDLY_NAME_LEN;
                              }
                              CsrMemCpy(linkPtr->serviceName, string, stringLen);
                              CsrUtf8StrTruncate((CsrUtf8String*)linkPtr->serviceName, stringLen);
                          }
                          if (TRUE == csrBtHfSdpGetBluetoothProfileDescriptorList(sdpTagList, (CsrUint16)sdpRecordIndex, &version))
                          {
                              linkPtr->remoteVersion = version;
                          }
                        }
                       else if (tmpUuid == CSR_BT_HEADSET_AG_SERVICE_UUID)
                        {  /* AG: handset connection  */
                           localConnectionType = CSR_BT_HF_CONNECTION_HS;
                           if (TRUE == CsrBtUtilSdrGetServiceHandle(sdpTagList, sdpRecordIndex, &serviceHandleTemp))
                           {
                               linkPtr->serviceHandle = serviceHandleTemp;
                           }

                        }
                    }
                    else
                    {/* This is a HS connection...*/
                        localConnectionType = CSR_BT_HF_CONNECTION_HS;
                    }
                }
            }
        }
    }
    else
    {
        linkPtr->serviceHandleListCount     = 0;
    }

    CsrBtUtilBllFreeLinkedList(&sdpTagList, CsrBtUtilBllPfreeWrapper);

    /* Now that we have handled the SDC close message, decide what to do based on whether the
       service discovery operation went well or not, and on whether the application has decided to cancel
       or diconnect before connection establishement */
        if ((resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) &&
            (resultSupplier == CSR_BT_SUPPLIER_CM) &&
            (localConnectionType != CSR_BT_HF_CONNECTION_UNKNOWN))
        {
            if (localConnectionType == CSR_BT_HF_CONNECTION_HF)
            {  /* HFP connection */
                csrBtHfUtilCarryOnAfterSdp(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                if (linkPtr->state == Connect_s)
                {
                   linkPtr->state = Activate_s;
                }
            }
            else
            {  /* HSP connection */
                if (((CsrBtCmConnectCfm*)(instData->recvMsgP))->type == CSR_BT_CM_CONNECT_CFM)
                {
                    CsrBtCmConnectCfm * prim;
                    prim = (CsrBtCmConnectCfm *) instData->recvMsgP;


                    linkPtr->currentDeviceAddress = prim->deviceAddr;
                    if (!linkPtr->disconnectReqReceived)
                    {
                        linkPtr->linkState = CSR_BT_LINK_STATUS_CONNECTED;
                        linkPtr->hfConnId = prim->btConnId;
                        linkPtr->data->maxRfcFrameSize = prim->profileMaxFrameSize;
                        linkPtr->scoConnectAcceptPending = TRUE;
                        CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                                  linkPtr->hfConnId,
                                                  linkPtr->audioSetupParams.theAudioQuality,
                                                  linkPtr->audioSetupParams.theTxBandwidth,
                                                  linkPtr->audioSetupParams.theRxBandwidth,
                                                  linkPtr->audioSetupParams.theMaxLatency,
                                                  linkPtr->audioSetupParams.theVoiceSettings,
                                                  linkPtr->audioSetupParams.theReTxEffort);
                        /* Connection Established. Send AT+CKPD to AG */
                        sendCkpd(instData);
                        CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                        linkPtr->state = Connected_s;
                        CsrBtHfLpStart(instData,TRUE);
                    }
                    else
                    {
                        CsrBtCmDisconnectReqSend(prim->btConnId);
                    }
                }
                else
                {
                    linkPtr->state = Connect_s;
                    linkPtr->linkType = CSR_BT_HF_CONNECTION_HS;
                    CsrBtHfpHandler(instData);
                    if (linkPtr->state == ServiceSearch_s)
                    {
                        linkPtr->state = Activate_s;
                    }
                }
            }
        }
        else
        {
            if (localConnectionType == CSR_BT_HF_CONNECTION_HS)
            {
                if (((CsrBtCmConnectCfm*)(instData->recvMsgP))->type == CSR_BT_CM_CONNECT_CFM)
                {
                    linkPtr->linkType = localConnectionType;
                    CsrBtHfpHandler(instData);
                }
                else if (resultCode != CSR_BT_RESULT_CODE_CM_SUCCESS ||
                         resultSupplier != CSR_BT_SUPPLIER_CM)
                {
                    CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_CONNECT_ATTEMPT_FAILED, CSR_BT_SUPPLIER_HF);
                }
                linkPtr->state = Activate_s;
            }
            else
            {/* HF_CONNECTION*/
                if (((CsrBtCmConnectCfm*)(instData->recvMsgP))->type == CSR_BT_CM_CONNECT_CFM)
                {   /* search was started from hf_main_sef because of connect req: outgoing connection */
                    if (resultCode != CSR_BT_RESULT_CODE_CM_SUCCESS ||
                        resultSupplier != CSR_BT_SUPPLIER_CM)
                    {
                        if (linkPtr->pendingCancel)
                        {
                            CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_CANCELLED_CONNECT_ATTEMPT, CSR_BT_SUPPLIER_HF);
                            linkPtr->pendingCancel = FALSE;
                        }
                        else
                        {
                            CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SDC_SEARCH_FAILED, CSR_BT_SUPPLIER_HF);
                        }
                        linkPtr->state = Activate_s;
                    }
                    else
                    {
                        csrBtHfUtilCarryOnAfterSdp(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    }
                }
                else
                {   /* this is HF doing SDC search after another device has connected */
                    if (resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                        resultSupplier == CSR_BT_SUPPLIER_CM)
                    {
                        csrBtHfUtilCarryOnAfterSdp(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    }
                    else
                    {
                        csrBtHfUtilCarryOnAfterSdp(instData, resultCode, resultSupplier);
                    }
                }
            }
        }
}


void CsrBtHfRfcSdcConResultHandler(void                        *inst,
                                   CsrUint8               localServerCh,
                                   CsrUint32                    hfConnId,
                                   CsrBtDeviceAddr             deviceAddr,
                                   CsrUint16                    maxFrameSize,
                                   CsrBool                      validPortPar,
                                   RFC_PORTNEG_VALUES_T        portPar,
                                   CsrBtResultCode             resultCode,
                                   CsrBtSupplier         resultSupplier,
                                   CmnCsrBtLinkedListStruct    *sdpTagList)
{

    HfMainInstanceData_t *instData = (HfMainInstanceData_t *) inst;
    CsrIntFast8 i = 0;
    CsrBool stop = FALSE;
    HfInstanceData_t    *linkPtr;

    for (i=0; ((i< instData->maxHFConnections + instData->maxHSConnections) && (!stop)); i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);

        if (linkPtr->serverChannel == localServerCh)
        { /* index found */
            instData->index = i;
            if (instData->searchIdxHf != 0xFF)
            {
                if (instData->index != instData->searchIdxHf)
                {
                    instData->linkData[instData->searchIdxHf].searchAndCon = FALSE;
                    instData->linkData[instData->searchIdxHf].searchOngoing = FALSE;
                    instData->searchIdxHf = 0xFF;
                }
            }
            if (instData->searchIdxHs != 0xFF)
            {
                if (instData->index != instData->searchIdxHs)
                {
                    if (instData->linkData[instData->searchIdxHs].state == Connect_s)
                    {
                        instData->linkData[instData->searchIdxHs].state = Activate_s;
                    }
                    instData->linkData[instData->searchIdxHs].searchOngoing = FALSE;
                    instData->linkData[instData->searchIdxHs].searchAndCon = FALSE;
                    instData->searchIdxHs = 0xFF;
                }
            }
            stop = TRUE;
        }
    }

    linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if ( ((resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && resultSupplier == CSR_BT_SUPPLIER_CM) ||
          (linkPtr->oldState == Activate_s)) &&
          (linkPtr->searchOngoing == TRUE) )
    {
        linkPtr->hfConnId = hfConnId;
        linkPtr->data->maxRfcFrameSize = maxFrameSize;
        linkPtr->oldState = Connect_s;
        linkPtr->serviceState = btConnect_s;
        linkPtr->linkState = CSR_BT_LINK_STATUS_CONNECTED;
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,deviceAddr,hfConnId);
        if((instData->mainConfig & CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE) == CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE)
        {
            linkPtr->state = Connected_s;
        }
        else
        {
            linkPtr->state = ServiceSearch_s;
        }

    }

    CsrBtHfSdcResultHandler(instData, sdpTagList, deviceAddr, resultCode, resultSupplier);
}

CsrUint8 getUsedHfConnections(HfMainInstanceData_t *inst)
{
    CsrUint8 returnVal = 0;
    CsrUintFast8 i;

    for (i=0;i<inst->maxHFConnections;i++)
    {
        if ((inst->linkData[i].state == Connected_s) || (inst->linkData[i].state == LpEnabled_s))
        {
            returnVal++;
        }
    }

    return returnVal;
}

CsrUint8 getUsedHsConnections(HfMainInstanceData_t *inst)
{
    CsrUint8 returnVal = 0;
    CsrIntFast8 i;

    for (i=inst->maxHFConnections;i<(inst->maxHFConnections + inst->maxHSConnections);i++)
    {
        if ((inst->linkData[i].state == Connected_s) || (inst->linkData[i].state == LpEnabled_s))
        {
            returnVal++;
        }
    }

    return returnVal;
}

void CsrBtHfSdcSelectServiceHandler(void                    * instData,
                               void                    * cmSdcRfcInstData,
                               CsrBtDeviceAddr            deviceAddr,
                               CsrUint8           serverChannel,
                               CsrUint16                entriesInSdpTaglist,
                               CmnCsrBtLinkedListStruct * sdpTagList)
{
    CsrUint16 *serviceHandleIndexList = CsrPmemAlloc(sizeof(CsrUint16) * MAX_SERVICE_RECORDS_SEARCH);
    CsrBtUuid32    tmpUuid = 0;
    CsrUint16    sdpRecordIndex;
    CsrUint16    idx = 0;
    CsrUint16    tmpResult;
    CsrUint16    dummy1,dummy2;
    HfMainInstanceData_t *inst = (HfMainInstanceData_t *) instData;

    if (inst->maxHFConnections > getUsedHfConnections(inst))
    {/* Still possible to establish a HFP connection */

        /* First find all the HFG records*/
        for (sdpRecordIndex = 0; ((sdpRecordIndex < entriesInSdpTaglist) && (idx < MAX_SERVICE_RECORDS_SEARCH)); sdpRecordIndex++)
        {
            if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList,
                                            sdpRecordIndex,
                                            &tmpUuid,
                                            &tmpResult,
                                            &dummy1,
                                            &dummy2))
            {
                if (tmpResult == SDR_SDC_SEARCH_SUCCESS)
                {
                    if (tmpUuid == CSR_BT_HFG_PROFILE_UUID)
                    {
                        serviceHandleIndexList[idx] = sdpRecordIndex;
                        idx++;
                    }
                }
            }
        }
    }


    if (inst->maxHSConnections > getUsedHsConnections(inst))
    {/* Still possible to establish a HSP connection */

        for (sdpRecordIndex = 0; ((sdpRecordIndex < entriesInSdpTaglist) && (idx < MAX_SERVICE_RECORDS_SEARCH)); sdpRecordIndex++)
        {
            if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList,
                                            sdpRecordIndex,
                                            &tmpUuid,
                                            &tmpResult,
                                            &dummy1,
                                            &dummy2))
            {
                if (tmpResult == SDR_SDC_SEARCH_SUCCESS)
                {
                    if (tmpUuid == CSR_BT_HEADSET_AG_SERVICE_UUID)
                    {
                        serviceHandleIndexList[idx] = sdpRecordIndex;
                        idx++;
                    }
                }
            }
        }
    }

    /* Select the preferred service or services to connect to in prioritized order*/
    CsrBtUtilRfcConSetServiceHandleIndexList(instData, cmSdcRfcInstData, serviceHandleIndexList,idx);

}

void sendCkpd(HfMainInstanceData_t * instData)
{
    CsrUint8        *body;

    body = CsrPmemAlloc(CKPD200_LENGTH);
    CsrMemCpy(body, CKPD200, CKPD200_LENGTH);
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = ckpd;
    }
    CsrBtHfHsSendCmDataReq(instData, CKPD200_LENGTH, body);
}


void startAtFeatureSearch(HfMainInstanceData_t * instData)
{
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if ((linkPtr->remoteVersion != CSR_BT_FIRST_HFP_NO_ESCO) || (linkPtr->remoteVersion == 0))
    {/* Version 0.96 does not care about AT+BRSF; so do not send it if the remote
        version supported is found and is 0.96 */
        linkPtr->atSequenceState = supportFeatures;
        sendBrsf(instData);
    }
    else
    {
        linkPtr->atSequenceState = cindSupport;
        linkPtr->serviceState = serviceConnect_s;
        sendCindSupport(instData);
    }
    /* Make sure not to allow more connections than indicated by the application */
    CsrBtHfCancelAcceptCheck(instData);
}

/*************************************************************************************
    Send a CSR_BT_HF_CALL_HANDLING_IND to app
************************************************************************************/
void CsrBtHfSendHfCallHandlingInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CsrBtHfCallHandlingInd  *prim;
    char *index_i;

    prim = (CsrBtHfCallHandlingInd  *)CsrPmemAlloc(sizeof(CsrBtHfCallHandlingInd));
    prim->type = CSR_BT_HF_CALL_HANDLING_IND;

    index_i = (char *)atTextString;
    index_i = CsrStrChr(index_i,' ');

    if(index_i == NULL)
    {
        index_i = (char *)atTextString;
        index_i = CsrStrChr(index_i,':');
    }
    if(index_i != NULL)
    {
        prim->event = index_i[1] - '0';
        prim->connectionId = instData->linkData[instData->index].hfConnId;
        CsrBtHfMessagePut(instData->appHandle, prim);
    }
    else
    {
        CsrPmemFree(prim);
    }
}

/*************************************************************************************
    Send AT+COPS (operator name format setting)
************************************************************************************/
void CsrBtHfAtCopsSetCommandSend(HfMainInstanceData_t *instData,CsrUint8 mode, CsrUint8 format)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(COPS_SET_FORMAT));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, COPS_SET_FORMAT);
    *(dataPtr + COPS_SET_FORMAT_INDEX) = format;
    *(dataPtr + COPS_SET_MODE_INDEX) = mode;
    instData->linkData[instData->index].data->dataReceivedInConnected = TRUE;
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = copsSet;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}

/*************************************************************************************
    Send AT+COPS=? (operator name query)
************************************************************************************/
void CsrBtHfAtCopsQuerySend(HfMainInstanceData_t *instData)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(COPS_QUERY));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, COPS_QUERY);

    instData->linkData[instData->index].data->dataReceivedInConnected = TRUE;
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = copsQueryCmd;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}

/*************************************************************************************
    Send AT+CLCC (call list)
************************************************************************************/
void CsrBtHfAtClccSend(HfMainInstanceData_t *instData)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(CALL_LIST_QUERY));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, CALL_LIST_QUERY);
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = clcc;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}

/*************************************************************************************
    Send AT+CCWA (call waiting)
************************************************************************************/
void CsrBtHfAtCcwaSend(HfMainInstanceData_t *instData, CsrBool enable)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(CALL_WAITING_CMD));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, CALL_WAITING_CMD);
    if (enable)
    {
        *(dataPtr + CCWA_VALUE_INDEX) = '1';
    }
    else
    {
        *(dataPtr + CCWA_VALUE_INDEX) = '0';
    }
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = ccwa;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}


/*************************************************************************************
    Send AT+CLIP (calling line identification)
************************************************************************************/
void CsrBtHfAtClipSend(HfMainInstanceData_t *instData, CsrBool enable)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(CLIP_COMMAND));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, CLIP_COMMAND);
    if (enable)
    {
        *(dataPtr + CLIP_VALUE_INDEX) = '1';
    }
    else
    {
        *(dataPtr + CLIP_VALUE_INDEX) = '0';
    }
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = clip;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}

/*************************************************************************************
    Send AT+NREC (noise reduction and echo cancellation)
************************************************************************************/
void CsrBtHfAtNrecSend(HfMainInstanceData_t *instData, CsrBool enable)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(ECHO_NOISE_REDUCTION_CMD));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, ECHO_NOISE_REDUCTION_CMD);
    if (enable)
    {
        *(dataPtr + NREC_VALUE_INDEX) = '1';
    }
    else
    {
        *(dataPtr + NREC_VALUE_INDEX) = '0';
    }
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = nrec;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}


/*************************************************************************************
    Send AT+BVRA (voice recognition)
************************************************************************************/
void CsrBtHfAtBvraSend(HfMainInstanceData_t *instData, CsrBool start)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(VOICE_RECOGNITION_CMD));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, VOICE_RECOGNITION_CMD);
    if (start)
    {
        *(dataPtr + BVRA_VALUE_INDEX) = '1';
    }
    else
    {
        *(dataPtr + BVRA_VALUE_INDEX) = '0';
    }
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = bvra;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}


/*************************************************************************************
    Send AT+VTS (generate DTMF)
************************************************************************************/
void CsrBtHfAtVtsSend(HfMainInstanceData_t *instData, CsrUint8 dtmf)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(DTMF_SEND_CMD));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, DTMF_SEND_CMD);

    *(dataPtr + DTMF_VALUE_INDEX) = dtmf;

    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = vts;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}


/*************************************************************************************
    Send AT+CNUM (subscription number)
************************************************************************************/
void CsrBtHfAtCnumSend(HfMainInstanceData_t *instData)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(SUBSCRIBER_QUERY));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, SUBSCRIBER_QUERY);
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = cnum;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}

/*************************************************************************************
    Send AT+CMEE (extended error)
************************************************************************************/
void CsrBtHfAtCmeeSetCommandSend(HfMainInstanceData_t *instData,CsrBool enable)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(EXTENDED_ERROR));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, EXTENDED_ERROR);
    if (enable)
    {
        *(dataPtr + EXTENDED_ERROR_INDEX) = '1';
    }
    else
    {
        *(dataPtr + EXTENDED_ERROR_INDEX) = '0';
    }
    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = cmee;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}

/*************************************************************************************
    Send AT+CIND (indicators status query)
************************************************************************************/
void CsrBtHfAtStatusIndValueSend(HfMainInstanceData_t *instData)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(CIND_STATUS));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, CIND_STATUS);

    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = cindSupportCmd;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}

/*************************************************************************************
    Send AT+BINP (BT input: associate a phone book entry to a voice tag)
************************************************************************************/
void CsrBtHfAtBinpSend(HfMainInstanceData_t *instData, CsrUint32 dataRequest)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    strLen = (CsrUint16)(CsrStrLen(BT_INPUT_CMD));
    dataPtr = CsrPmemAlloc(strLen+1);
    CsrStrCpy((char*) dataPtr, BT_INPUT_CMD);

    *(dataPtr + BINP_VALUE_INDEX) = (CsrUint8)dataRequest + '0'; /* Value in ASCII: only value '1' allowed so far! */

    if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
    {
        instData->linkData[instData->index].lastAtCmdSent = binp;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);
}

/*************************************************************************************
    Send ATD'number', ATD>'memory index' or AT+BLDN (dial number, dial number at
    memory index, or redial, respectively)
************************************************************************************/
void CsrBtHfAtDialSend(HfMainInstanceData_t *instData, CsrBtHfDialCommand  command, CsrCharString *number)
{
    CsrUint8         *dataPtr;
    CsrUint16       strLen;

    switch(command)
    {
        case CSR_BT_HF_DIAL_NUMBER:
        case CSR_BT_HF_DIAL_MEMORY:
        {
            CsrUint8  index = DIAL_CMD_INDEX;
            
            strLen = (CsrUint16)(DIAL_CMD_INDEX + CsrStrLen((char*)number));
            
            if (command == CSR_BT_HF_DIAL_MEMORY)
            {
                strLen++;
                index++;
            }
            dataPtr = CsrPmemAlloc(strLen);
            CsrMemSet(dataPtr,0x00,strLen);
            if (command == CSR_BT_HF_DIAL_NUMBER)
            {
                CsrStrCpy((char*) dataPtr, DIAL_CMD);
            }
            else
            {
                CsrStrCpy((char*) dataPtr, DIAL_MEM_CMD);
            }
            if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
            {
                CsrMemCpy(&dataPtr[index],(CsrUint8 *)number,CsrStrLen((char*)number));
                if (command == CSR_BT_HF_DIAL_NUMBER)
                {
                    instData->linkData[instData->index].lastAtCmdSent = dialNumber;
                }
                else
                {
                   instData->linkData[instData->index].lastAtCmdSent = dialMem;
                }
            }
            break;
        }
        default:
        {/* This can only be redial.... */
            strLen = (CsrUint16)(CsrStrLen(REDIAL_CMD));
            dataPtr = CsrPmemAlloc(strLen+1);
            CsrStrCpy((char*) dataPtr, REDIAL_CMD);
            if (instData->linkData[instData->index].lastAtCmdSent == idleCmd)
            {
                instData->linkData[instData->index].lastAtCmdSent = redial;
            }
            break;
        }
    }

    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);

    CsrPmemFree(number);
}


/*************************************************************************************
    Send AT+CSRSF: (C2C initiation)
************************************************************************************/
void CsrBtHfSendC2CInitMsgReq(HfMainInstanceData_t *instData)
{
/*    CsrUint8          *dataPtr;
    CsrUint16       strLen;
    CsrUintFast8          i;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    dataPtr = CsrPmemAlloc(CSR_INIT_CMD_LENGTH);
    CsrStrCpy((char*) dataPtr, CSR_INIT_CMD);
    for (i=1; i < CSR_BT_C2C_NUMBER_OF_INDICATORS ;i++)
    {
        CsrUint8 j = CSR_INIT_VALUE_INDEX + ((i-1)*2);

        dataPtr[j] = linkPtr->linkInd.locC2C[i] + '0';
        if (i == CSR_BT_C2C_NUMBER_OF_INDICATORS - 1)
        {
            dataPtr[j+1] = '\r';
            if ((j+2) < CSR_INIT_CMD_LENGTH)
            {
                dataPtr[j+2] = 0;
            }
            else
            {
                dataPtr[CSR_INIT_CMD_LENGTH -1] = 0;
            }
        }
        else
        {
            dataPtr[j+1] = ',';
        }
    }
    strLen = CsrStrLen((char*)dataPtr)+1;

    if (linkPtr->lastAtCmdSent == idleCmd)
    {
        linkPtr->lastAtCmdSent = idleCmd;
    }
    CsrBtHfHsSendCmDataReq(instData,strLen,dataPtr);*/
}

/*************************************************************************************
    Send AT+CSR:(index,value)[,(index,value)] (C2C update)
************************************************************************************/
void CsrBtHfSendC2CSf(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    CsrBtHfC2cSfReq  *prim = (CsrBtHfC2cSfReq *)instData->recvMsgP;
    CsrUint8          *dataPtr = CsrPmemAlloc(CSR_UPDATE_CMD_LENGTH);

    CsrStrCpy((char*) dataPtr, CSR_UPDATE_CMD);
    dataPtr[CSR_UPDATE_VALUE1_INDEX] = prim->number + '0';
    dataPtr[CSR_UPDATE_VALUE2_INDEX] = prim->value + '0';

    if (linkPtr->lastAtCmdSent == idleCmd)
    {
        linkPtr->lastAtCmdSent = c2c;
    }
    CsrBtHfHsSendCmDataReq(instData,CSR_UPDATE_CMD_LENGTH,dataPtr);
}

/*************************************************************************************
    Send AT+CSRBATT:value (C2C battery status update)
************************************************************************************/
void CsrBtHfSendC2CBatt(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    CsrBtHfC2cBattReq  *prim = (CsrBtHfC2cBattReq *)instData->recvMsgP;
    CsrUint8          *dataPtr = CsrPmemAlloc(CSR_BATT_INFO_CMD_LENGTH);

    CsrStrCpy((char*) dataPtr, CSR_BATT_INFO_CMD);
    dataPtr[CSR_BATT_VALUE_INDEX] = prim->value + '0';

    if (linkPtr->lastAtCmdSent == idleCmd)
    {
        linkPtr->lastAtCmdSent = c2cbatt;
    }
    CsrBtHfHsSendCmDataReq(instData,CSR_BATT_INFO_CMD_LENGTH,dataPtr);
}

/*************************************************************************************
    Send AT+CSRGETSMS:index (C2C get SMS stored at index given)
************************************************************************************/
void CsrBtHfSendC2CGetSMS(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    CsrBtHfC2cGetSmsReq  *prim = (CsrBtHfC2cGetSmsReq *)instData->recvMsgP;
    CsrUint8    *dataPtr = CsrPmemAlloc(CSR_GET_SMS_CMD_LENGTH);
    CsrUint8    tmpVal = prim->index;
    CsrUintFast8    i;

    CsrMemSet(dataPtr,0,CSR_GET_SMS_CMD_LENGTH);
    CsrStrCpy((char*)dataPtr, CSR_GET_SMS_CMD);

    for (i=0;i<3;i++)
    { /* Max 3 digits */
        CsrUint8 val = 10*(2-i);
        if (val > 0)
        {
            dataPtr[CSR_GET_SMS_VALUE_INDEX+i] = (tmpVal / val) + '0';
            tmpVal = tmpVal % val;
        }
        else
        {/* In this case, tmpVal is a value below 10: just use it as it is */
            dataPtr[CSR_GET_SMS_VALUE_INDEX+i] = tmpVal + '0';
        }
    }

    if (linkPtr->lastAtCmdSent == idleCmd)
    {
        linkPtr->lastAtCmdSent = c2cSms;
    }
    CsrBtHfHsSendCmDataReq(instData,CSR_GET_SMS_CMD_LENGTH,dataPtr);
}

/*************************************************************************************
    Send AT+CSRPWR:source (C2C power source information: charging/not charging)
************************************************************************************/
void CsrBtHfSendC2CPwr(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    CsrBtHfC2cPwrReq  *prim = (CsrBtHfC2cPwrReq *)instData->recvMsgP;
    CsrUint8          *dataPtr = CsrPmemAlloc(CSR_PWR_INFO_CMD_LENGTH);

    CsrStrCpy((char*) dataPtr, CSR_POWER_INFO_CMD);
    dataPtr[CSR_PWR_VALUE_INDEX] = prim->value + '0';

    if (linkPtr->lastAtCmdSent == idleCmd)
    {
        linkPtr->lastAtCmdSent = c2cpwr;
    }
    CsrBtHfHsSendCmDataReq(instData,CSR_PWR_INFO_CMD_LENGTH,dataPtr);
}



