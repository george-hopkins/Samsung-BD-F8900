/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #5 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_hf_main_sef.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_am_lib.h"

/* Forward declarations */
void CsrBtHfLpTimeout(CsrUint16 mi, void *mv);
void CsrBtHfLpTimeoutHandler(HfMainInstanceData_t *inst);


static CsrBool FindPendingConnectionToBdAddress(HfMainInstanceData_t * instData,CsrBtDeviceAddr deviceAddr)
{
    CsrIntFast8 i = 0;
    HfInstanceData_t *linkPtr;
    CsrBool found = FALSE;

    for (i=0;i<(instData->maxHFConnections + instData->maxHSConnections);i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);

        if ((linkPtr->currentDeviceAddress.lap == deviceAddr.lap) &&
            (linkPtr->currentDeviceAddress.uap == deviceAddr.uap) &&
            (linkPtr->currentDeviceAddress.nap == deviceAddr.nap))
        {
            instData->index = i;
            if ((linkPtr->state == Connect_s) || (linkPtr->state == ServiceSearch_s))
            {
                return TRUE;
            }
        }
    }

    return found;
}

static CsrBtHfConnectionType getConnTypeFromConnId(HfMainInstanceData_t * instData, CsrBtHfConnectionId connId)
{
    CsrIntFast8 i = 0;
    HfInstanceData_t *linkPtr;

    for (i=0;i<(instData->maxHFConnections + instData->maxHSConnections);i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
        if(linkPtr->hfConnId == connId)
        {
            instData->index = i;
            if (i < instData->maxHFConnections)
            {
                linkPtr->linkType = CSR_BT_HF_CONNECTION_HF;
            }
            else
            {
                linkPtr->linkType = CSR_BT_HF_CONNECTION_HS;
            }
            return linkPtr->linkType;
        }
    }

    return CSR_BT_HF_CONNECTION_UNKNOWN;
}

static CsrBtHfConnectionType getConnectionTypeFromServerChannel(HfMainInstanceData_t * instData, CsrUint8 serverChannel)
{
    CsrIntFast8 i = 0;
    HfInstanceData_t *linkPtr;

    for (i=0;i<(instData->maxHFConnections + instData->maxHSConnections);i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
        if (linkPtr->serverChannel == serverChannel)
        {
            instData->index = i;
            if (i < instData->maxHFConnections)
            {
                linkPtr->linkType = CSR_BT_HF_CONNECTION_HF;
            }
            else
            {
                linkPtr->linkType = CSR_BT_HF_CONNECTION_HS;
            }
            return linkPtr->linkType;
        }
    }

    return CSR_BT_HF_CONNECTION_UNKNOWN;
}

static CsrBtHfConnectionType getConnectionType(HfMainInstanceData_t * instData, CsrUint32 connId)
{
    CsrIntFast8 i = 0;
    HfInstanceData_t *linkPtr;

    for (i=0;i<(instData->maxHFConnections + instData->maxHSConnections);i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
        if (linkPtr->hfConnId == connId)
        {
            instData->index = i;
            if (i < instData->maxHFConnections)
            {
                linkPtr->linkType = CSR_BT_HF_CONNECTION_HF;
            }
            else
            {
                linkPtr->linkType = CSR_BT_HF_CONNECTION_HS;
            }
            return linkPtr->linkType;
        }
    }

    return CSR_BT_HF_CONNECTION_UNKNOWN;
}

void DeactivateHsHandler(HfMainInstanceData_t * instData)
{
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    switch (linkPtr->state)
    {
        case Activate_s:
            {
                if (linkPtr->sdsRegistered)
                {
                    CsrBtCmCancelAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->serverChannel);
                }
                break;
            }
        case Connect_s:
            {
                break;
            }
        case Connected_s:
            {
                if (linkPtr->scoHandle != HF_SCO_UNUSED)
                {
                    CsrBtCmScoDisconnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                }
                else
                {
                    CsrBtCmScoCancelReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                    CsrBtCmDisconnectReqSend(linkPtr->hfConnId);
                }
                break;
            }
        case LpEnabled_s:
            {
                CsrBtCmDisconnectReqSend(linkPtr->hfConnId);
                break;
            }
    }
    instData->state = Deactivate_s;
}

void DeactivateHfHandler(HfMainInstanceData_t * instData)
{
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    switch (linkPtr->state)
    {
        case Activate_s:
            {
                CsrBtCmCancelAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->serverChannel);
                instData->state = Deactivate_s;
                break;
            }
        case ServiceSearch_s:
            {
                CsrBtHfSaveMessage(instData);
                instData->deactivated = TRUE;
                break;
            }
        case Connect_s:
            {
                instData->state = Deactivate_s;
                break;
            }
        case Connected_s:
        case LpEnabled_s:
            {
                instData->state = Deactivate_s;
                /* determine if we have a SCO connected */
                if (linkPtr->scoHandle == HF_SCO_UNUSED)
                {
                    CsrBtCmScoCancelReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                    CsrBtCmDisconnectReqSend(linkPtr->hfConnId);
                }
                else
                {
                    CsrBtCmScoDisconnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                }
                break;
            }
        default:
            {
                break;
            }
    }
}

static void csrBtHfHsUnregister(HfMainInstanceData_t * instData)
{
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (linkPtr->sdsRegistered)
    {
        instData->numberOfUnregister = 0;
        CsrBtCmSdsUnRegisterReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->serviceRecHandle);
    }
    else
    {
        CsrIntFast8 i = 0;
        CsrBool goToIdle = TRUE;

        linkPtr->deactivated = TRUE;
        linkPtr->serviceRecHandle = 0;

        for (i=0;((i<(instData->maxHFConnections + instData->maxHSConnections)) && goToIdle);i++)
        {
            linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
            if (!linkPtr->deactivated)
            {
                instData->index = i;
                goToIdle = FALSE;
                if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
                {
                    DeactivateHsHandler(instData);
                }
                else
                {
                    DeactivateHfHandler(instData);
                }
            }
        }

        if (goToIdle)
        {
            instData->state = Null_s;
            if(instData->doingCleanup)
            {
                CsrBtHfCleanup(instData);
            }
            else
            {
                CsrBtHfCleanup_reinit_instance(instData);
                CsrBtHfSendHfDeactivateCfm(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
            }
        }
    }
}

/*************************************************************************************
    A activate request from a higher layer is received.
************************************************************************************/
static void commonActivateHandler(HfMainInstanceData_t *instData)
{
    CsrIntFast8 i = 0;
    CsrBool msgSent = FALSE;
    HfInstanceData_t *linkPtr;

    instData->restoreFlag = FALSE;
    instData->deactivated = FALSE;

    for (i=0;((i < (instData->maxHFConnections + instData->maxHSConnections) )&& (!msgSent));i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
        if ((!linkPtr->sdsRegistered) && (linkPtr->state == Activate_s))
        {
            msgSent = TRUE;
            instData->index = i;
            if (i < instData->maxHFConnections)
            {
                CsrBtHfSendSdsRegisterReq(instData);
            }
            else
            {
                HsSendSdsRegisterReq(instData);
            }
        }
    }

    if (!msgSent)
    {
        instData->state = Activated_s;
        CsrBtHfSendHfActivateCfm(instData,CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
        for (i=0;i < (instData->maxHFConnections + instData->maxHSConnections);i++)
        {
            linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
            linkPtr->deactivated = FALSE;
            linkPtr->state = Activate_s;
            linkPtr->oldState = Activate_s;

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

        CsrBtHfSendHfHouseCleaning(instData);
    }
}
/************************************************************************************************************************************/
void InitReorganizedLinkInstance(HfInstanceData_t *newLinkPtr,HfMainInstanceData_t *instData)
{
    CsrUint16 mi;
    void *mv;

    newLinkPtr->lastAtCmdSent                   = idleCmd;
    newLinkPtr->disconnectReqReceived           = FALSE;
    newLinkPtr->disconnectPeerReceived          = FALSE;
    newLinkPtr->scoHandle                       = HF_SCO_UNUSED;
    newLinkPtr->linkState                       = CSR_BT_LINK_STATUS_DISCONNECTED;
    newLinkPtr->hfConnId                        = CSR_BT_CONN_ID_INVALID;
    newLinkPtr->audioPending                    = FALSE;
    newLinkPtr->lastAudioReq                    = FALSE;
    newLinkPtr->audioQuality                    = HCI_PKT_HV3;
    newLinkPtr->state                           = Activate_s;
    newLinkPtr->oldState                        = Activate_s;
    newLinkPtr->disconnectResult                = CSR_BT_RESULT_CODE_CM_SUCCESS;
    newLinkPtr->disconnectSupplier              = CSR_BT_SUPPLIER_CM;
    newLinkPtr->disconnectLocalTerminated       = TRUE;
    newLinkPtr->remoteVersion                   = 0;
    newLinkPtr->pcmSlot                         = 0;
    newLinkPtr->pcmReassign                     = FALSE;
    newLinkPtr->pcmMappingReceived              = FALSE;
    newLinkPtr->audioAcceptPending              = FALSE;
    newLinkPtr->atSequenceState                 = supportFeatures;
    CsrMemSet(newLinkPtr->serviceName,0,CSR_BT_MAX_FRIENDLY_NAME_LEN+1);
    CsrMemCpy(&newLinkPtr->audioSetupParams,&instData->generalAudioParams, sizeof(audioSetupParams_t));
    newLinkPtr->scoConnectAcceptPending         = FALSE;

    if (newLinkPtr->cindSupportString != NULL)
    {
        CsrPmemFree(newLinkPtr->cindSupportString);
        newLinkPtr->cindSupportString = NULL;
    }
    if (newLinkPtr->cindStartValueString != NULL)
    {
        CsrPmemFree(newLinkPtr->cindStartValueString);
        newLinkPtr->cindStartValueString = NULL;
    }
    if (newLinkPtr->chldStringStored != NULL)
    {
        CsrPmemFree(newLinkPtr->chldStringStored);
        newLinkPtr->chldStringStored = NULL;
    }
    newLinkPtr->searchAndCon                    = FALSE;
    newLinkPtr->searchOngoing                   = FALSE;
    newLinkPtr->pendingCancel                   = FALSE;

    if(newLinkPtr->lpTimer != 0)
    {
        CsrSchedTimerCancel(newLinkPtr->lpTimer,&mi,&mv);
        newLinkPtr->lpTimer         = 0;
    }
    if (newLinkPtr->data->atResponseTimerId != 0)
    {
        CsrSchedTimerCancel(newLinkPtr->data->atResponseTimerId, &mi, &mv);
        newLinkPtr->data->atResponseTimerId     = 0;
    }
    if (newLinkPtr->deregisterTimerId != 0)
    {
        CsrSchedTimerCancel(newLinkPtr->deregisterTimerId, &mi, &mv);
        newLinkPtr->deregisterTimerId           = 0;
    }
    newLinkPtr->lpCount             = HF_LP_RETRY;
    newLinkPtr->pendingModeChange   = FALSE;
    newLinkPtr->lpMode              = 0;
    newLinkPtr->lpWantedMode        = 0;
    newLinkPtr->incomingSlc         = FALSE;
    newLinkPtr->outgoingAudio       = FALSE;
    newLinkPtr->localDisconnect     = FALSE;
    newLinkPtr->pendingSlcXtraCmd   = FALSE;
    newLinkPtr->accepting           = FALSE;
}

CsrBool ReorganizeLinks(HfInstanceData_t *newLinkPtr, HfInstanceData_t *oldLinkptr, CsrUint8 oldActiveHFCons, CsrUint8 oldActiveHSCons,
            CsrUint8 *oldInactiveHFCons,CsrUint8 *oldInactiveHSCons,CsrUint8 newActiveHFCons, CsrUint8 newActiveHSCons,HfMainInstanceData_t *instData)
{
    CsrIntFast8 i,j,k,l,m;
    CsrUint8 total;
    CsrUint8 tmpOldInactiveHf = *oldInactiveHFCons;
    CsrUint8 tmpOldInactiveHs = *oldInactiveHSCons;
    CsrBool registerCm = FALSE;
    CsrUint8 numberOfNewInactive=0; /*,newIndex=0, oldIndex=0;*/
    CsrBool equalPointers = FALSE;
    CsrUint8 tmpServer = 0;
    HfHsData_t *tmpData = NULL;
    HfInstanceData_t tmpLinkPtr;
    CsrUint8 newInactiveHf = 0;

    if (newLinkPtr == oldLinkptr)
    {
        equalPointers = TRUE;
    }
    k = oldActiveHFCons + oldActiveHSCons; /* Index of first 'old' HF service record allocated and registered but not used */
    l = k + tmpOldInactiveHf; /* Index of first 'old' HS service record allocated and registered but not used */
    total = oldActiveHFCons + oldActiveHSCons + tmpOldInactiveHf + tmpOldInactiveHs;
    if (newActiveHFCons >= (tmpOldInactiveHf + oldActiveHFCons))
    {
        newInactiveHf = 0;
    }
    else
    {
        newInactiveHf = tmpOldInactiveHf + oldActiveHFCons - newActiveHFCons;
    }
    if (total < (newActiveHFCons + newActiveHSCons))
    {
        total = newActiveHFCons + newActiveHSCons;
    }
    if ((newActiveHFCons > tmpOldInactiveHf + oldActiveHFCons) ||
        (newActiveHSCons > tmpOldInactiveHs + oldActiveHSCons))
    {
        registerCm = TRUE;
    }

    /* First keep the active HF server channels */
    for (i=0; i<(oldActiveHFCons + tmpOldInactiveHf); i++)
    {
        if (i < oldActiveHFCons)
        {
            j = i;
        }
        else
        {
            j = k + i - oldActiveHFCons;
        }
        if (i < newActiveHFCons)
        {
            m = i;
        }
        else
        {
            m = i + newActiveHSCons;
        }
        /* If connecting, connected or in LP mode, copy all fields; otherwise only the serverchannel is relevant! */
        switch (oldLinkptr[j].state)
        {
            case Activate_s:
            {
                if (newLinkPtr[m].data == NULL)
                {
                    newLinkPtr[m].data = CsrPmemAlloc(sizeof(HfHsData_t));
                    CsrMemSet(newLinkPtr[m].data,0,sizeof(HfHsData_t));
                    newLinkPtr[m].data->allowed2SendCmData = TRUE;
                }
                if (equalPointers)
                {/* Exchange server channels... */
                    tmpServer = newLinkPtr[m].serverChannel;
                    tmpData = newLinkPtr[m].data;
                }
                InitReorganizedLinkInstance(&newLinkPtr[m], instData);
                newLinkPtr[m].serverChannel = oldLinkptr[j].serverChannel;
                newLinkPtr[m].sdsRegistered = oldLinkptr[j].sdsRegistered;
                newLinkPtr[m].serviceRecHandle = oldLinkptr[j].serviceRecHandle;
                newLinkPtr[m].linkType = oldLinkptr[j].linkType;
                if (equalPointers)
                {
                    oldLinkptr[j].serverChannel = tmpServer;
                    newLinkPtr[m].data = oldLinkptr[j].data;
                    oldLinkptr[j].data = tmpData;
                    if ((j> newActiveHSCons + newActiveHFCons) && (oldLinkptr[j].data != NULL))
                    {
                        CsrPmemFree(oldLinkptr[j].data);
                        oldLinkptr[j].data = NULL;
                    }
                }
                else
                {
                    if (oldLinkptr[j].data != NULL)
                    {
                        if (oldLinkptr[j].data->recvAtCmd != NULL)
                        {
                            CsrPmemFree(oldLinkptr[j].data->recvAtCmd);
                            oldLinkptr[j].data->recvAtCmd = NULL;
                        }
                        CsrPmemFree(oldLinkptr[j].data);
                        oldLinkptr[j].data = NULL;
                    }
                }
                if (newLinkPtr[m].data == NULL)
                {
                    newLinkPtr[m].data = CsrPmemAlloc(sizeof(HfHsData_t));
                    CsrMemSet(newLinkPtr[m].data,0,sizeof(HfHsData_t));
                    newLinkPtr[m].data->allowed2SendCmData = TRUE;
                }
                break;
            }
            case Connect_s:
            case Connected_s:
            case LpEnabled_s:
            case ServiceSearch_s:
            {
                if (equalPointers)
                {
                    CsrMemCpy(&tmpLinkPtr,&newLinkPtr[m],sizeof(HfInstanceData_t));
                    CsrMemCpy(&newLinkPtr[m],&oldLinkptr[j],sizeof(HfInstanceData_t));
                    CsrMemCpy(&oldLinkptr[j],&tmpLinkPtr,sizeof(HfInstanceData_t));
                }
                else
                {
                    CsrMemCpy(&newLinkPtr[m],&oldLinkptr[j], sizeof(HfInstanceData_t));
                    CsrMemCpy(newLinkPtr[m].data,oldLinkptr[j].data,sizeof(HfHsData_t));
                    newLinkPtr[m].cindSupportString = oldLinkptr[j].cindSupportString;
                    newLinkPtr[m].cindStartValueString = oldLinkptr[j].cindStartValueString;
                    newLinkPtr[m].chldStringStored = oldLinkptr[j].chldStringStored;
                    newLinkPtr[m].serverChannel = oldLinkptr[j].serverChannel;
                    newLinkPtr[m].sdsRegistered = oldLinkptr[j].sdsRegistered;
                    newLinkPtr[m].serviceRecHandle = oldLinkptr[j].serviceRecHandle;
                    newLinkPtr[m].linkType = oldLinkptr[j].linkType;
                }
                break;
            }
            default:
                break;
        }
    }
    /* All needed HF service records added to the new linkData. Add the needed HS records */
    for (i=0; i<(oldActiveHSCons + tmpOldInactiveHs); i++)
    {
        if (i < oldActiveHSCons)
        {
            j = i+oldActiveHFCons;
        }
        else
        {
            j = l + i - oldActiveHSCons;
        }
        if (i < newActiveHSCons)
        {/* First record to copy into is the next after the newly active HF records */
            m = i + newActiveHFCons;
        }
        else
        {/* The newly inactive HS records are placed after both the new HF and HS and the newly inactive HF records */
            m = newActiveHFCons + newInactiveHf + i;
        }
        switch (oldLinkptr[j].state)
        {
            case Activate_s:
            {
                if (newLinkPtr[m].data == NULL)
                {
                    newLinkPtr[m].data = CsrPmemAlloc(sizeof(HfHsData_t));
                    CsrMemSet(newLinkPtr[m].data,0,sizeof(HfHsData_t));
                    newLinkPtr[m].data->allowed2SendCmData = TRUE;
                }
                if (equalPointers)
                {/* Exchange server channels... */
                    tmpServer = newLinkPtr[m].serverChannel;
                    tmpData = newLinkPtr[m].data;
                }
                InitReorganizedLinkInstance(&newLinkPtr[m], instData);
                newLinkPtr[m].serverChannel = oldLinkptr[j].serverChannel;
                newLinkPtr[m].sdsRegistered = oldLinkptr[j].sdsRegistered;
                newLinkPtr[m].serviceRecHandle = oldLinkptr[j].serviceRecHandle;
                newLinkPtr[m].linkType = oldLinkptr[j].linkType;
                if (equalPointers)
                {
                    oldLinkptr[j].serverChannel = tmpServer;
                    newLinkPtr[m].data = oldLinkptr[j].data;
                    oldLinkptr[j].data = tmpData;
                }
                else
                {
                    if (oldLinkptr[j].data != NULL)
                    {
                        if (oldLinkptr[j].data->recvAtCmd != NULL)
                        {
                            CsrPmemFree(oldLinkptr[j].data->recvAtCmd);
                            oldLinkptr[j].data->recvAtCmd = NULL;
                        }
                        CsrPmemFree(oldLinkptr[j].data);
                        oldLinkptr[j].data = NULL;
                    }
                }
                break;
            }
            case Connect_s:
            case Connected_s:
            case LpEnabled_s:
            case ServiceSearch_s:
            {
                if (equalPointers)
                {
                    CsrMemCpy(&tmpLinkPtr,&newLinkPtr[m],sizeof(HfInstanceData_t));
                    CsrMemCpy(&newLinkPtr[m],&oldLinkptr[j],sizeof(HfInstanceData_t));
                    CsrMemCpy(&oldLinkptr[j],&tmpLinkPtr,sizeof(HfInstanceData_t));
                }
                else
                {
                    CsrMemCpy(&newLinkPtr[m],&oldLinkptr[j], sizeof(HfInstanceData_t));
                    CsrMemCpy(newLinkPtr[m].data,oldLinkptr[j].data,sizeof(HfHsData_t));
                    newLinkPtr[m].cindSupportString = oldLinkptr[j].cindSupportString;
                    newLinkPtr[m].cindStartValueString = oldLinkptr[j].cindStartValueString;
                    newLinkPtr[m].chldStringStored = oldLinkptr[j].chldStringStored;
                    newLinkPtr[m].sdsRegistered = oldLinkptr[j].sdsRegistered;
                    newLinkPtr[m].serviceRecHandle = oldLinkptr[j].serviceRecHandle;
                    newLinkPtr[m].linkType = oldLinkptr[j].linkType;
                }
                break;
            }
            default:
                break;
        }
    }

    /* Now that all the active links have been run through make sure to keep track
       of the inactive ones: the ones that were used before, but are not needed now */
    if (oldActiveHFCons > newActiveHFCons)
    {/* the previously inactive records are not relevant here */
        numberOfNewInactive = oldActiveHFCons - newActiveHFCons;
        *oldInactiveHFCons += numberOfNewInactive;
    }
    else
    {
        CsrUint8 tmp = newActiveHFCons - oldActiveHFCons;
        if (tmp > *oldInactiveHFCons)
        {/* All the previously inactive HF records are re-used */
            *oldInactiveHFCons = 0;
        }
        else
        {/* Only 'tmp'-number of the inactive HF records become active now */
            *oldInactiveHFCons -= tmp;
        }
    }
    if (oldActiveHSCons > newActiveHSCons)
    {/* the previously inactive records are not relevant here */
        numberOfNewInactive = oldActiveHSCons - newActiveHSCons;
        *oldInactiveHSCons += numberOfNewInactive;
    }
    else
    {
        CsrUint8 tmp = newActiveHSCons - oldActiveHSCons;
        if (tmp > *oldInactiveHSCons)
        {/* All the previously inactive HS records are re-used */
            *oldInactiveHSCons = 0;
        }
        else
        {/* Only 'tmp'-number of the inactive HS records become active now */
            *oldInactiveHSCons -= tmp;
        }
    }

    return registerCm;
}

/********************************************************************************************************
    Organize service records so records engaged in a connection are placed as the first in the list.
********************************************************************************************************/
void reOrderRecords(HfMainInstanceData_t * instData, CsrBtHfConnectionType type,CsrUint8 usedRecords)
{
    CsrUintFast8 i,j;
    HfInstanceData_t *linkPtr, *tmpPtr;
    HfInstanceData_t copyPtr;
    CsrUint8 start = 0, end = instData->maxHFConnections;
    CsrBool moved = FALSE;

    if (type == CSR_BT_HF_CONNECTION_HS)
    {
        start   = instData->maxHFConnections;
        end     += instData->maxHSConnections;
    }

    for (i=start; ((i < end) && (usedRecords > 0)); i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
        if (linkPtr->state != Activate_s)
        {/* service record in use: move to start of array */
            usedRecords--;
            moved = FALSE;
            for (j = start; ((j < i) && (!moved)); j++)
            {
                tmpPtr = (HfInstanceData_t *)&(instData->linkData[j]);
                if (tmpPtr->state == Activate_s)
                {/* Free entry found: do the switch! */
                    CsrMemCpy(&copyPtr,tmpPtr,sizeof(HfInstanceData_t));
                    CsrMemCpy(tmpPtr,linkPtr,sizeof(HfInstanceData_t));
                    CsrMemCpy(linkPtr,&copyPtr,sizeof(HfInstanceData_t));
                    moved = TRUE;
                }
            }
        }
    }
}

/*************************************************************************************
    Perform registration and allocation of servers towards the CM depending on the
    wishes of the application
************************************************************************************/
void CsrBtHfXStateActivateReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtHfActivateReq *prim;
    CsrIntFast8 i = 0;
    CsrUint8 totalNumber = 0;
    CsrUint8 allocatedHfRecords = instData->allocInactiveHfCons + instData->maxHFConnections;
    CsrUint8 allocatedHsRecords = instData->allocInactiveHsCons + instData->maxHSConnections;
    CsrBool  initialActivation = FALSE;
    CsrBool  reOrganize = FALSE;
    CsrBool  cmRegister = FALSE;
    CsrUint8 oldMaxHfCon = instData->maxHFConnections;
    CsrUint8 oldMaxHsCon = instData->maxHSConnections;
    HfInstanceData_t    *linkPtr, *oldLinkPtr = NULL;
    CsrUint8 nrActiveHf = 0;
    CsrUint8 nrActiveHs = 0;
    CsrUint8 nrOfActiveConnections = CsrBtHfGetNumberOfRecordsInUse(instData,&nrActiveHf,&nrActiveHs);

    prim = (CsrBtHfActivateReq *) instData->recvMsgP;

    instData->appHandle = prim->phandle;


    if (((prim->maxHFConnections + prim->maxHSConnections) == 0) ||
        (prim->maxSimultaneousConnections == 0) ||
        (prim->maxSimultaneousConnections < nrOfActiveConnections) ||
        (prim->maxHFConnections < nrActiveHf) ||
        (prim->maxHSConnections < nrActiveHs))
    {/* Invalid parameters: answer back and ignore! */
        CsrBtHfSendHfActivateCfm(instData,CSR_BT_RESULT_CODE_HF_INVALID_PARAMETER, CSR_BT_SUPPLIER_HF);
        return;
    }
    if (instData->reActivating != 0)
    {/* Already activating.... reject! */
        CsrBtHfSendHfActivateCfm(instData, CSR_BT_RESULT_CODE_HF_REJ_RESOURCES, CSR_BT_SUPPLIER_HF);
        return;
    }

    if (prim->maxHFConnections > instData->maxHFConnections )
    {
        instData->reActivating += prim->maxHFConnections - instData->maxHFConnections;
    }

    if (prim->maxHSConnections > instData->maxHSConnections )
    {
        instData->reActivating += prim->maxHSConnections - instData->maxHSConnections;
    }

    if (nrOfActiveConnections > 0)
    {/* Organize first if needed so used records are placed as the first ones in the array */
        if (nrActiveHf > 0)
        {
            reOrderRecords(instData, CSR_BT_HF_CONNECTION_HF,nrActiveHf);
        }
        if (nrActiveHs > 0)
        {
            reOrderRecords(instData, CSR_BT_HF_CONNECTION_HS,nrActiveHs);
        }
    }

    instData->localSupportedFeatures = prim->supportedFeatures;
    instData->mainConfig = prim->hfConfig;
    if (prim->atResponseTime > CSR_BT_AT_DEFAULT_RESPONSE_TIME)
    {
        instData->atRespWaitTime = prim->atResponseTime;
    }
    instData->maxHFConnections = prim->maxHFConnections;
    instData->maxHSConnections = prim->maxHSConnections;
    instData->maxTotalSimultaneousConnections = prim->maxSimultaneousConnections;
    totalNumber = instData->maxHSConnections + instData->maxHFConnections;
    if (instData->linkData != NULL)
    {/* Keep the old data to copy it if needed */
        oldLinkPtr = instData->linkData;
    }

    if (allocatedHfRecords > instData->maxHFConnections)
    {
        totalNumber += allocatedHfRecords - instData->maxHFConnections;
    }
    if (allocatedHsRecords > instData->maxHSConnections)
    {
        totalNumber += allocatedHsRecords - instData->maxHSConnections;
    }
    /* totalNumber shall now indicate how much space to allocate; not how many active records thare shall be */
    if ((instData->maxHSConnections + instData->maxHFConnections) >= (allocatedHfRecords + allocatedHsRecords))
    {
        cmRegister = TRUE;
    }
    else
    {
        reOrganize = TRUE;
    }
    if ((instData->linkData == NULL) ||
         (allocatedHfRecords < instData->maxHFConnections) || (allocatedHsRecords < instData->maxHSConnections))
    {/* Allocate new heap area if not allocated yet, or if number of records needed of different types is bigger than already allocated */
        initialActivation = TRUE;
        if (instData->linkData != NULL)
        {
            reOrganize = TRUE;
        }
        instData->linkData = (HfInstanceData_t *)CsrPmemAlloc(sizeof(HfInstanceData_t)*totalNumber);
    }

    if (initialActivation)
    {
        for (i=0;i<totalNumber;i++)
        {
            linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);

            CsrMemSet(linkPtr,0x00, sizeof(HfInstanceData_t));
            linkPtr->data = (HfHsData_t *)CsrPmemAlloc(sizeof(HfHsData_t));
            CsrMemSet(linkPtr->data,0x00, sizeof(HfHsData_t));
            linkPtr->serverChannel     = CSR_BT_NO_SERVER;
            linkPtr->hfConnId          = CSR_BT_CONN_ID_INVALID;
            linkPtr->deactivated       = TRUE;
            linkPtr->audioCodecType    = CSR_BT_C2C_ADPCM_OFF_VALUE;
            CsrMemCpy(&linkPtr->audioSetupParams,&instData->generalAudioParams, sizeof(audioSetupParams_t));
            instData->index = i;
            CsrBtHfInitInstanceData(instData);
        }
    }

    instData->index = 0;
    if (reOrganize)
    {
        cmRegister = ReorganizeLinks(instData->linkData, oldLinkPtr, oldMaxHfCon, oldMaxHsCon,
                                     &instData->allocInactiveHfCons, &instData->allocInactiveHsCons,
                                     instData->maxHFConnections, instData->maxHSConnections, instData);
    }
    if (initialActivation)
    {
        if (oldLinkPtr != NULL)
        {/* De-allocate the old heap area if a new area has been allocated */
            for (i=0;i<(oldMaxHfCon+oldMaxHsCon);i++)
            {
                if (oldLinkPtr[i].data != NULL)
                {
                    CsrBool found = FALSE;
                    CsrIntFast8 j;
                    for (j=0;(j<(instData->maxHFConnections+instData->maxHSConnections) && !found);j++)
                    {
                        if (oldLinkPtr[i].data == instData->linkData[j].data)
                        {
                            found = TRUE;
                        }
                    }
                    if (!found)
                    {
                        if (oldLinkPtr[i].data->recvAtCmd != NULL)
                        {
                            CsrPmemFree(oldLinkPtr[i].data->recvAtCmd);
                            oldLinkPtr[i].data->recvAtCmd = NULL;
                        }
                        CsrPmemFree(oldLinkPtr[i].data);
                        oldLinkPtr[i].data = NULL;
                    }
                }
            }
            CsrPmemFree(oldLinkPtr);
        }
        else
        {
            if (instData->maxHFConnections)
            {/* Tell the SD that it must look for the CSR_BT_HFG_PROFILE_UUID service,
               when it perform a SD_READ_AVAILABLE_SERVICE_REQ                  */
                CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_HFG_PROFILE_UUID);
            }
            if (instData->maxHSConnections)
            {/* Tell the SD that it must look for the CSR_BT_HFG_PROFILE_UUID service,
               when it perform a SD_READ_AVAILABLE_SERVICE_REQ                  */
                CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_HEADSET_AG_SERVICE_UUID);
            }
        }

        /* send a register to CM to allocate the internal server channel
        * number for the client role. */
        if (cmRegister)
        {
            CsrBtCmRegisterReqSend(CSR_BT_HF_IFACEQUEUE);
        }
        else
        {
            commonActivateHandler(instData);
        }
    }
    else
    {
        if (!reOrganize)
        {/* This means the "old inactive" records are reused as they are... which means there are no
            "CSR_UNUSED" records anymore: remember to update!  */
            if (instData->maxHFConnections <= instData->allocInactiveHfCons)
            {
                instData->allocInactiveHfCons -= instData->maxHFConnections;
            }
            if (instData->maxHSConnections <= instData->allocInactiveHsCons)
            {
                instData->allocInactiveHsCons -= instData->maxHSConnections;
            }
        }
        commonActivateHandler(instData);
    }
}


/*************************************************************************************
    The HF is now registered in the CM and a local server channel is allocated. Go on
    and register the allocated server channel in SDS for the HF.
************************************************************************************/
void CsrBtHfNullStateDeactivateReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtHfSendHfDeactivateCfm(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
}

void CsrBtHfNullStateCmRegisterCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmRegisterCfm *prim;
    CsrBool waitForNextRegister = FALSE;
    HfInstanceData_t *linkPtr = NULL;

    prim = (CsrBtCmRegisterCfm *) instData->recvMsgP;

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrIntFast8 i;

        for (i=0; ((i<(instData->maxHFConnections + instData->maxHSConnections)) && (!waitForNextRegister));i++)
        {
            linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
            instData->index = i;

            if (linkPtr->serverChannel == CSR_BT_NO_SERVER)
            {
                linkPtr->serverChannel = prim->serverChannel;
                if (i<(instData->maxHFConnections + instData->maxHSConnections-1))
                {
                    CsrBtCmRegisterReqSend(CSR_BT_HF_IFACEQUEUE);
                    waitForNextRegister = TRUE;
                }

            }
        }
        if (!waitForNextRegister)
        {
            if ((instData->maxHFConnections + instData->maxHSConnections) == 1)
            {/* Make sure to handle this special situation */
                if (linkPtr == NULL)
                {
                    linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
                }
                linkPtr->serverChannel = prim->serverChannel;
            }
            CsrBtHfSendHfHouseCleaning(instData);
            instData->state = Null_s;
            /* Now make sure to finish activation process: start SDS record setup */
            commonActivateHandler(instData);
        }
    }
}


/* Timer event function for low-power (sniff) timer */
void CsrBtHfLpTimeoutHandler(HfMainInstanceData_t *inst)
{
    if (inst != NULL)
    {
        CsrBool rerun;
        HfInstanceData_t *linkPtr;
        linkPtr = (HfInstanceData_t *)&(inst->linkData[inst->index]);

        /* Paranoid checks - may happen at a time-cancel combined
         * with a shutdown */
        if (linkPtr == NULL)
        {
            return;
        }

        rerun = FALSE;

        /* Override wanted mode if sniff not allowed? */
        if(inst->lpMask & CSR_BT_HF_PWR_DISABLE_SNIFF_REQ)
        {
            linkPtr->lpWantedMode = CSR_BT_ACTIVE_MODE;
        }

        /* Request LP-mode again? */
        if(linkPtr->lpWantedMode != linkPtr->lpMode)
        {
            rerun = TRUE;
            linkPtr->pendingModeChange = TRUE;
            CsrBtCmRfcModeChangeReqSend(linkPtr->hfConnId,
                                linkPtr->lpWantedMode);
        }

        /* Reschedule timer if needed */
        if(linkPtr->lpCount != 0)
        {
            linkPtr->lpCount--;
        }
        if(rerun && (linkPtr->lpCount > 0) && (linkPtr->lpTimer == 0))
        {
            linkPtr->lpTimer = CsrSchedTimerSet(HF_LP_TIME,
                                            CsrBtHfLpTimeout,
                                            (CsrUint16)inst->index,
                                            (void *)inst);
        }
    }
}

void CsrBtHfLpTimeout(CsrUint16 mi, void *mv)
{
    HfInstanceData_t *linkPtr;
    HfMainInstanceData_t *inst;

    inst = (HfMainInstanceData_t*) mv;
    inst->index = (CsrUint8)mi;
    linkPtr = (HfInstanceData_t *)&(inst->linkData[inst->index]);

    /* Timer was fired */
    linkPtr->lpTimer = 0;
    CsrBtHfLpTimeoutHandler(inst);

}

void CsrBtHfLpStop(HfMainInstanceData_t *inst)
{
    CsrUint16 mi;
    void *mv;
    HfInstanceData_t *linkPtr;

    linkPtr = (HfInstanceData_t *)&(inst->linkData[inst->index]);

    if(linkPtr->lpTimer != 0)
    {
        CsrSchedTimerCancel(linkPtr->lpTimer,
                           &mi,
                           &mv);
        linkPtr->lpTimer = 0;
    }
    linkPtr->lpCount = HF_LP_RETRY;
}

void CsrBtHfLpStart(HfMainInstanceData_t *inst, CsrBool now)
{
    if ((CSR_BT_HF_CNF_DISABLE_LOW_POWER & inst->mainConfig) == 0)
    {/* Only start LP handling if allowed by the application */
        HfInstanceData_t *linkPtr;
        linkPtr = (HfInstanceData_t *)&(inst->linkData[inst->index]);
            /* Restart timer? */
        if(linkPtr->lpTimer == 0)
        {
            linkPtr->lpTimer = CsrSchedTimerSet(HF_LP_TIME,
                                           CsrBtHfLpTimeout,
                                           (CsrUint16)inst->index,
                                           (void*)inst);
        }

        /* Always restart retry-counter */
        linkPtr->lpWantedMode = CSR_BT_SNIFF_MODE;
        linkPtr->lpCount = HF_LP_RETRY;

        /* Schedule an immediate LP request? */
        if(now)
        {
            linkPtr->lpCount++;
            CsrBtHfLpTimeoutHandler(inst);
        }
    }
}

/************************************************************************************
    Configure LP settings to be used when the profile is ready to use them
*************************************************************************************/
void CsrBtHfXStateConfigLowPowerReqHandler(HfMainInstanceData_t * inst)
{
    CsrBtHfConfigLowPowerReq *prim;
    CsrBool changed = FALSE;

    prim = (CsrBtHfConfigLowPowerReq*)inst->recvMsgP;

    /* Detect changes */
    if(inst->lpMask != prim->mask)
    {
        changed = TRUE;
    }

    /* Store new mask and priority */
    inst->lpMask = prim->mask;

    /* Reschedule a low-power run if anything changed */
    if(changed)
    {
        CsrIntFast8 i;
        for(i=0; i<(inst->maxHFConnections + inst->maxHSConnections); i++)
        {
            if(inst->linkData[i].state == Connected_s)
            {
                inst->index = i;
                CsrBtHfLpStart(inst, TRUE);
            }
        }
    }

  /* Send low power configuration confirmation */
    CsrBtHfSendConfirmMessage(inst,CSR_BT_HF_CONFIG_LOW_POWER_CFM);

}

#ifdef CSR_BT_INSTALL_HF_CONFIG_AUDIO
/************************************************************************************
    Configure audio settings to be used when the profile is ready to use them
*************************************************************************************/
void CsrBtHfXStateConfigAudioReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtHfConfigAudioReq *prim;

    prim = (CsrBtHfConfigAudioReq *) instData->recvMsgP;

    if ((instData->state != Activated_s) || (prim->connectionId == CSR_BT_HF_CONNECTION_ALL))
    {/* If connectionId == 0xFFFFFFFF, all existing connections shall be updated! */
        switch(prim->audioType)
        {
            case CSR_BT_HF_AUDIO_OUTGOING_PARAMETER_LIST_CONFIG:
            {
                CsrBtHfAudioLinkParameterListConfig *audSettings = (CsrBtHfAudioLinkParameterListConfig *)prim->audioSetting;

                instData->generalAudioParams.theAudioQuality  = audSettings->packetType;
                instData->generalAudioParams.theTxBandwidth  = audSettings->txBandwidth;
                instData->generalAudioParams.theRxBandwidth  = audSettings->rxBandwidth;
                instData->generalAudioParams.theMaxLatency  = audSettings->maxLatency;
                instData->generalAudioParams.theVoiceSettings = audSettings->voiceSettings;
                instData->generalAudioParams.theReTxEffort = audSettings->reTxEffort;

                if (prim->connectionId == CSR_BT_HF_CONNECTION_ALL)
                {
                    CsrIntFast8 i = 0;

                    for (i=0; i<(instData->maxHSConnections + instData->maxHFConnections); i++)
                    {
                        HfInstanceData_t  *linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                        if (linkPtr->scoHandle == HF_SCO_UNUSED)
                        {/* If audio connection exist, do not update. When connection closes, update from main settings! */
                            linkPtr->audioSetupParams.theAudioQuality  = audSettings->packetType;
                            linkPtr->audioSetupParams.theTxBandwidth  = audSettings->txBandwidth;
                            linkPtr->audioSetupParams.theRxBandwidth  = audSettings->rxBandwidth;
                            linkPtr->audioSetupParams.theMaxLatency  = audSettings->maxLatency;
                            linkPtr->audioSetupParams.theVoiceSettings = audSettings->voiceSettings;
                            linkPtr->audioSetupParams.theReTxEffort = audSettings->reTxEffort;
                        }
                        else
                        {
                            linkPtr->updateAudioSettingsFromMain = TRUE;
                        }
                    }
                }
                break;
            }
#ifdef CSR_USE_DSPM
            case CSR_BT_HF_AUDIO_DSPM_STREAMS_CONFIG:
            {

                if (prim->connectionId == CSR_BT_HF_CONNECTION_ALL)
                {
                    CsrBtHfAudioDspmStreamConfig *dspmAudConfig = (CsrBtHfAudioDspmStreamConfig *)prim->audioSetting;
                    CsrUint8 i = 0;
                    /* Copy the information into the generic HF main instance structure */
                    instData->dspmInst.sourceType     = dspmAudConfig->sourceType;
                    instData->dspmInst.sourceInstance = dspmAudConfig->sourceInstance;
                    instData->dspmInst.sourceChannel  = dspmAudConfig->sourceChannel;
                    instData->dspmInst.sinkType       = dspmAudConfig->sinkType;
                    instData->dspmInst.sinkInstance   = dspmAudConfig->sinkInstance;
                    instData->dspmInst.sinkChannel    = dspmAudConfig->sinkChannel;
                    instData->dspmInst.numSourceWbsConfigParams = dspmAudConfig->numSourceWbsConfigParams;
                    /* Make sure that there are no memory leaks: free the config pointers to begin with */
                    CsrPmemFree(instData->dspmInst.sourceWbsConfigParams);
                    CsrPmemFree(instData->dspmInst.sourceNbConfigParams);
                    CsrPmemFree(instData->dspmInst.sinkWbsConfigParams);
                    CsrPmemFree(instData->dspmInst.sinkNbConfigParams);
                    instData->dspmInst.sourceWbsConfigParams    = dspmAudConfig->sourceWbsConfigParams;
                    instData->dspmInst.numSourceNbConfigParams  = dspmAudConfig->numSourceNbConfigParams;
                    instData->dspmInst.sourceNbConfigParams     = dspmAudConfig->sourceNbConfigParams;
                    instData->dspmInst.numSinkWbsConfigParams   = dspmAudConfig->numSinkWbsConfigParams;
                    instData->dspmInst.sinkWbsConfigParams      = dspmAudConfig->sinkWbsConfigParams;
                    instData->dspmInst.numSinkNbConfigParams    = dspmAudConfig->numSinkNbConfigParams;
                    instData->dspmInst.sinkNbConfigParams       = dspmAudConfig->sinkNbConfigParams;

                    if (instData->dspmHfCallBack != CsrBtHfDspmConfigStreamRspHandler)
                    {/* Only update the configuration data if the link is not being configured at the moment! */
                        for (i=0; i<(instData->maxHSConnections + instData->maxHFConnections); i++)
                        {
                            HfInstanceData_t  *linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                            if (linkPtr)
                            {
                                linkPtr->dspmInst.sourceType     = dspmAudConfig->sourceType;
                                linkPtr->dspmInst.sourceInstance = dspmAudConfig->sourceInstance;
                                linkPtr->dspmInst.sourceChannel  = dspmAudConfig->sourceChannel;
                                linkPtr->dspmInst.sinkType       = dspmAudConfig->sinkType;
                                linkPtr->dspmInst.sinkInstance   = dspmAudConfig->sinkInstance;
                                linkPtr->dspmInst.sinkChannel    = dspmAudConfig->sinkChannel;
                                /* Make sure that there are no memory leaks: free the config pointers to begin with */
                                CsrPmemFree(linkPtr->dspmInst.sourceWbsConfigParams);
                                CsrPmemFree(linkPtr->dspmInst.sourceNbConfigParams);
                                CsrPmemFree(linkPtr->dspmInst.sinkWbsConfigParams);
                                CsrPmemFree(linkPtr->dspmInst.sinkNbConfigParams);
                                /* Each instance needs to keep its own copy of the config parameters */
                                linkPtr->dspmInst.sourceWbsConfigParams    = CsrPmemAlloc(dspmAudConfig->numSourceWbsConfigParams * sizeof(CsrBtHfStreamConfigParam));
                                linkPtr->dspmInst.sourceNbConfigParams     = CsrPmemAlloc(dspmAudConfig->numSourceNbConfigParams * sizeof(CsrBtHfStreamConfigParam));
                                linkPtr->dspmInst.sinkWbsConfigParams      = CsrPmemAlloc(dspmAudConfig->numSinkWbsConfigParams * sizeof(CsrBtHfStreamConfigParam));
                                linkPtr->dspmInst.sinkNbConfigParams       = CsrPmemAlloc(dspmAudConfig->numSinkNbConfigParams * sizeof(CsrBtHfStreamConfigParam));
                                linkPtr->dspmInst.numSourceWbsConfigParams = dspmAudConfig->numSourceWbsConfigParams;
                                CsrMemCpy(linkPtr->dspmInst.sourceWbsConfigParams, dspmAudConfig->sourceWbsConfigParams, dspmAudConfig->numSourceWbsConfigParams * sizeof(CsrBtHfStreamConfigParam));
                                linkPtr->dspmInst.numSourceNbConfigParams  = dspmAudConfig->numSourceNbConfigParams;
                                CsrMemCpy(linkPtr->dspmInst.sourceNbConfigParams, dspmAudConfig->sourceNbConfigParams, dspmAudConfig->numSourceNbConfigParams * sizeof(CsrBtHfStreamConfigParam));
                                linkPtr->dspmInst.numSinkWbsConfigParams   = dspmAudConfig->numSinkWbsConfigParams;
                                CsrMemCpy(linkPtr->dspmInst.sinkWbsConfigParams, dspmAudConfig->sinkWbsConfigParams, dspmAudConfig->numSinkWbsConfigParams * sizeof(CsrBtHfStreamConfigParam));
                                linkPtr->dspmInst.numSinkNbConfigParams    = dspmAudConfig->numSinkNbConfigParams;
                                CsrMemCpy(linkPtr->dspmInst.sinkNbConfigParams, dspmAudConfig->sinkNbConfigParams, dspmAudConfig->numSinkNbConfigParams * sizeof(CsrBtHfStreamConfigParam));
                            }
                        }
                    }
                }
                break;
            }
            
            case CSR_BT_HF_AUDIO_DSPM_STREAM_SINGLE_CONFIG:
            {
                CsrBtHfAudioDspStreamSingleConfig *configData = (CsrBtHfAudioDspStreamSingleConfig *)prim->audioSetting;
                CsrUint8 i = 0;
                for (i=0; i<(instData->maxHSConnections + instData->maxHFConnections); i++)
                {
                    HfInstanceData_t  *linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                    if (linkPtr->scoHandle == configData->scoHandle)
                    {/* the correct sco connection is found */
                        CsrBtHfDspmSendSingleConfigStream(instData, configData);
                        break;
                    }
                }
                break;
            }
#endif
            case CSR_BT_HF_AUDIO_INCOMING_ACCEPT_PARAMETER_CONFIG:
            {
                break;
            }
            case CSR_BT_HF_AUDIO_PREFERED_CODEC_SETUP:
            {
                break;
            }

            case CSR_BT_HF_AUDIO_PREFERED_SAMPLE_RATE_SETUP:
            {
                break;
            }

            default:
                break;
        }
    }
    else if (getConnTypeFromConnId(instData, prim->connectionId) == CSR_BT_HF_CONNECTION_HF)
    {
        HfInstanceData_t  *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
        switch(prim->audioType)
        {
            case CSR_BT_HF_AUDIO_OUTGOING_PARAMETER_LIST_CONFIG:
            {
                CsrBtHfAudioLinkParameterListConfig *audSettings = (CsrBtHfAudioLinkParameterListConfig *)prim->audioSetting;
                if (linkPtr->scoHandle == HF_SCO_UNUSED)
                {/* If audio connection exist, do not update. When connection closes, update from main settings! */
                    linkPtr->audioSetupParams.theAudioQuality  = audSettings->packetType;
                    linkPtr->audioSetupParams.theTxBandwidth  = audSettings->txBandwidth;
                    linkPtr->audioSetupParams.theRxBandwidth  = audSettings->rxBandwidth;
                    linkPtr->audioSetupParams.theMaxLatency  = audSettings->maxLatency;
                    linkPtr->audioSetupParams.theVoiceSettings = audSettings->voiceSettings;
                    linkPtr->audioSetupParams.theReTxEffort = audSettings->reTxEffort;
                }
                break;
            }
#ifdef CSR_USE_DSPM
            case CSR_BT_HF_AUDIO_DSPM_STREAMS_CONFIG:
            {
                if (instData->dspmHfCallBack != CsrBtHfDspmConfigStreamRspHandler)
                {/* Only update the configuration data if the link is not being configured at the moment! */
                    CsrBtHfAudioDspmStreamConfig *dspmAudConfig = (CsrBtHfAudioDspmStreamConfig *)prim->audioSetting;

                    linkPtr->dspmInst.sourceType     = dspmAudConfig->sourceType;
                    linkPtr->dspmInst.sourceInstance = dspmAudConfig->sourceInstance;
                    linkPtr->dspmInst.sourceChannel  = dspmAudConfig->sourceChannel;
                    linkPtr->dspmInst.sinkType       = dspmAudConfig->sinkType;
                    linkPtr->dspmInst.sinkInstance   = dspmAudConfig->sinkInstance;
                    linkPtr->dspmInst.sinkChannel    = dspmAudConfig->sinkChannel;
                    /* Make sure that there are no memory leaks: free the config pointers to begin with */
                    CsrPmemFree(linkPtr->dspmInst.sourceWbsConfigParams);
                    CsrPmemFree(linkPtr->dspmInst.sourceNbConfigParams);
                    CsrPmemFree(linkPtr->dspmInst.sinkWbsConfigParams);
                    CsrPmemFree(linkPtr->dspmInst.sinkNbConfigParams);
                    /* Each instance needs to keep its own copy of the config parameters */
                    linkPtr->dspmInst.sourceWbsConfigParams    = CsrPmemAlloc(dspmAudConfig->numSourceWbsConfigParams * sizeof(CsrBtHfStreamConfigParam));
                    linkPtr->dspmInst.sourceNbConfigParams     = CsrPmemAlloc(dspmAudConfig->numSourceNbConfigParams * sizeof(CsrBtHfStreamConfigParam));
                    linkPtr->dspmInst.sinkWbsConfigParams      = CsrPmemAlloc(dspmAudConfig->numSinkWbsConfigParams * sizeof(CsrBtHfStreamConfigParam));
                    linkPtr->dspmInst.sinkNbConfigParams       = CsrPmemAlloc(dspmAudConfig->numSinkNbConfigParams * sizeof(CsrBtHfStreamConfigParam));
                    linkPtr->dspmInst.numSourceWbsConfigParams = dspmAudConfig->numSourceWbsConfigParams;
                    CsrMemCpy(linkPtr->dspmInst.sourceWbsConfigParams, dspmAudConfig->sourceWbsConfigParams, dspmAudConfig->numSourceWbsConfigParams * sizeof(CsrBtHfStreamConfigParam));
                    linkPtr->dspmInst.numSourceNbConfigParams  = dspmAudConfig->numSourceNbConfigParams;
                    CsrMemCpy(linkPtr->dspmInst.sourceNbConfigParams, dspmAudConfig->sourceNbConfigParams, dspmAudConfig->numSourceNbConfigParams * sizeof(CsrBtHfStreamConfigParam));
                    linkPtr->dspmInst.numSinkWbsConfigParams   = dspmAudConfig->numSinkWbsConfigParams;
                    CsrMemCpy(linkPtr->dspmInst.sinkWbsConfigParams, dspmAudConfig->sinkWbsConfigParams, dspmAudConfig->numSinkWbsConfigParams * sizeof(CsrBtHfStreamConfigParam));
                    linkPtr->dspmInst.numSinkNbConfigParams    = dspmAudConfig->numSinkNbConfigParams;
                    CsrMemCpy(linkPtr->dspmInst.sinkNbConfigParams, dspmAudConfig->sinkNbConfigParams, dspmAudConfig->numSinkNbConfigParams * sizeof(CsrBtHfStreamConfigParam));
                }
                break;
            }

            case CSR_BT_HF_AUDIO_DSPM_STREAM_SINGLE_CONFIG:
            {
                CsrBtHfAudioDspStreamSingleConfig *configData = (CsrBtHfAudioDspStreamSingleConfig *)prim->audioSetting;                
                HfInstanceData_t  *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
                if (linkPtr->scoHandle == configData->scoHandle)
                {/* the correct sco connection is found */
                    CsrBtHfDspmSendSingleConfigStream(instData, configData);
                }
                break;
            }

#endif      
            case CSR_BT_HF_AUDIO_INCOMING_ACCEPT_PARAMETER_CONFIG:
            {
                break;
            }
            case CSR_BT_HF_AUDIO_PREFERED_CODEC_SETUP:
            {
                break;
            }

            case CSR_BT_HF_AUDIO_PREFERED_SAMPLE_RATE_SETUP:
            {
                break;
            }

            default:
                break;
        }
        
    }

    /* Send audio configuration confirmation */
    CsrBtHfSendConfirmMessage(instData,CSR_BT_HF_CONFIG_AUDIO_CFM);
    CsrPmemFree(prim->audioSetting);
}
#endif /* CSR_BT_INSTALL_HF_CONFIG_AUDIO */

/************************************************************************************
    Null State
*************************************************************************************/
void CsrBtHfNullStateCmSdsRegisterCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmSdsRegisterCfm * prim;
    CsrIntFast8 i = 0;
    HfInstanceData_t *linkPtr = NULL;
    CsrBool   reActivation = FALSE;

    if (instData->reActivating > 0)
    {
        reActivation = TRUE;
    }

    prim = (CsrBtCmSdsRegisterCfm *) instData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBool found =FALSE;
        if (instData->reActivating > 0)
        {
            instData->reActivating--;
        }

        for (i=0; ((i<(instData->maxHFConnections + instData->maxHSConnections)) && (!found)); i++)
        {
            linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
            if ((linkPtr->serviceRecHandle == 0) && (linkPtr->state == Activate_s))
            {
                found = TRUE;
                instData->index = i;
            }
        }

        linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
        if (found)
        {
            CsrUintFast8 idx = 0;

            if (instData->maxHFConnections)
            {
                idx = instData->maxHFConnections-1;
            }

            linkPtr->serviceRecHandle = prim->serviceRecHandle;
            linkPtr->sdsRegistered = TRUE;
            instData->linkRegistering = CSR_BT_HF_CONNECTION_UNKNOWN;
            if (instData->index < idx)
            {
                linkPtr->linkType = CSR_BT_HF_CONNECTION_HF;
                if ((!reActivation) || (instData->reActivating > 0))
                {
                    instData->index++;
                    CsrBtHfSendSdsRegisterReq(instData);
                }
                else
                {
                    instData->state = Activated_s;
                    CsrBtHfSendHfActivateCfm(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    CsrBtHfAllowConnectCheck(instData);
                }
            }
            else if (instData->index < (CsrUintFast8)(instData->maxHSConnections + instData->maxHFConnections - 1))
            {
                if (instData->index <= idx)
                {
                    linkPtr->linkType = CSR_BT_HF_CONNECTION_HF;
                }
                else
                {
                    linkPtr->linkType = CSR_BT_HF_CONNECTION_HS;
                }
                /*if ((!reActivation) || (instData->reActivating >= 0))
                {*/
                    for (;(instData->index < (CsrUintFast8)(instData->maxHFConnections + instData->maxHSConnections)); instData->index++)
                    {
                        linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
                        if (linkPtr->sdsRegistered == FALSE)
                        {
                            HsSendSdsRegisterReq(instData);
                            break;
                        }
                    }

                /*}
                else
                {
                    instData->state = Activated_s;
                    CsrBtHfSendHfActivateCfm(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    CsrBtHfAllowConnectCheck(instData);
                }*/
            }
            else
            {
                if (instData->index == (CsrUintFast8)(instData->maxHSConnections + instData->maxHFConnections - 1))
                {
                    if (instData->maxHSConnections)
                    {
                        linkPtr->linkType = CSR_BT_HF_CONNECTION_HS;
                    }
                    else
                    {
                        linkPtr->linkType = CSR_BT_HF_CONNECTION_HF;
                    }
                }
                instData->state = Activated_s;
                CsrBtHfSendHfActivateCfm(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                linkPtr = instData->linkData;
                for (i=0;i<(instData->maxHFConnections + instData->maxHSConnections);i++)
                {
                    linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                    linkPtr->deactivated = FALSE;
                    if ((!reActivation) || (linkPtr->state == Activate_s))
                    {/* If this is a re-activation or the link is connected, do not change the state of the link... */
                        linkPtr->oldState = Activate_s;
                        /*linkPtr->state = Activate_s;*/
                        if (i < instData->maxHFConnections)
                        {
                            CsrBtCmConnectAcceptReqSend(CSR_BT_HF_IFACEQUEUE,
                                                   HF_CLASS_OF_DEVICE,
                                                   0, CSR_BT_HF_PROFILE_DEFAULT_MTU_SIZE,
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
                CsrBtHfSendHfHouseCleaning(instData);
            }
        }
        else
        {
            instData->state = Activated_s;
            CsrBtHfSendHfActivateCfm(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
            instData->linkRegistering = CSR_BT_HF_CONNECTION_UNKNOWN;
            linkPtr = instData->linkData;

            for (i=0;i<(instData->maxHFConnections + instData->maxHSConnections);i++)
            {
                linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                linkPtr->deactivated = FALSE;
                linkPtr->oldState = Activate_s;
                linkPtr->state = Activate_s;
                linkPtr->sdsRegistered = TRUE;
                if (i < instData->maxHFConnections)
                {
                    CsrBtCmConnectAcceptReqSend(CSR_BT_HF_IFACEQUEUE,
                                           HF_CLASS_OF_DEVICE,
                                           0, CSR_BT_HF_PROFILE_DEFAULT_MTU_SIZE,
                                           linkPtr->serverChannel,
                                           instData->secIncoming,
                                           CSR_BT_HF_PROFILE_UUID,
                                           instData->modemStatus,
                                           CSR_BT_DEFAULT_BREAK_SIGNAL,
                                           CSR_BT_DEFAULT_MSC_TIMEOUT);
                }
                else
                {
                    if (instData->maxHSConnections != 0)
                    {
                        linkPtr->serviceRecHandle = prim->serviceRecHandle;
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
                    else
                    {
                        linkPtr = (HfInstanceData_t *)&(instData->linkData[i-1]);
                        linkPtr->serviceRecHandle = prim->serviceRecHandle;
                    }
                }
            }
            CsrBtHfSendHfHouseCleaning(instData);
        }
    }
    else
    {
        instData->linkRegistering = CSR_BT_HF_CONNECTION_UNKNOWN;
        linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

        if (linkPtr->serviceRecHandle == 0)
        {
            CsrBtHfSendSdsRegisterReq(instData);
        }
        else
        {
            if (instData->maxHSConnections != 0)
            {
                HsSendSdsRegisterReq(instData);
            }
        }
    }
}

/****************************************************************************
    Activated State
*****************************************************************************/
void CsrBtHfActivatedStateCmCancelAcceptConnectCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmCancelAcceptConnectCfm * prim;
    prim = (CsrBtCmCancelAcceptConnectCfm *) instData->recvMsgP;

    switch (getConnectionTypeFromServerChannel(instData, prim->serverChannel))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateCmConnectAcceptCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmConnectAcceptCfm * prim;
    prim = (CsrBtCmConnectAcceptCfm *) instData->recvMsgP;

    switch (getConnectionTypeFromServerChannel(instData, prim->serverChannel))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                   prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,prim->deviceAddr,prim->btConnId);
                }
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateCmDisconnectIndHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmDisconnectInd * prim;
    prim = (CsrBtCmDisconnectInd *) instData->recvMsgP;

    switch (getConnectionType(instData, prim->btConnId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                if (instData->linkData[instData->index].deregisterTimerId != 0)
                {/* Disconnected before un-registration of the service record */
                    CsrUint16 mi;
                    void *mv;
                    CsrSchedTimerCancel(instData->linkData[instData->index].deregisterTimerId,&mi,&mv);
                    instData->linkData[instData->index].deregisterTimerId = 0;
                }

                if (prim->status)
                {
                    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,instData->linkData[instData->index].currentDeviceAddress,
                                prim->btConnId);
                }

                CsrBtHfpHandler(instData);
                /* Make sure that the allowed connections can be established now...*/
                CsrBtHfAllowConnectCheck(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateCmScoConnectCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmScoConnectCfm * prim;
    prim = (CsrBtCmScoConnectCfm *) instData->recvMsgP;

    switch (getConnectionType(instData, prim->btConnId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateCmScoDisconnectIndHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmScoDisconnectInd * prim;
    prim = (CsrBtCmScoDisconnectInd *) instData->recvMsgP;

    switch (getConnectionType(instData, prim->btConnId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateCmScoAcceptConnectCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmScoAcceptConnectCfm * prim;
    prim = (CsrBtCmScoAcceptConnectCfm *) instData->recvMsgP;

    switch (getConnectionType(instData, prim->btConnId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateCmDataIndHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmDataInd * prim;
    prim = (CsrBtCmDataInd *) instData->recvMsgP;

    switch (getConnectionType(instData, prim->btConnId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateCmDataCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmDataCfm * prim;
    prim = (CsrBtCmDataCfm *) instData->recvMsgP;

    switch (getConnectionType(instData, prim->btConnId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}
/* Just ignore CSR_BT_CM_CONTROL_IND: it is not used at all....*/
void CsrBtHfXStateIgnoreCmControlIndHandler(HfMainInstanceData_t * instData)
{
    CSR_UNUSED(instData);
}

void CsrBtHfActivatedStateCmModeChangeIndHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmRfcModeChangeInd * prim;
    prim = (CsrBtCmRfcModeChangeInd *) instData->recvMsgP;

    switch (getConnectionType(instData, prim->btConnId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}
void CsrBtHfActivatedStateCmPortnegIndHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmPortnegInd * prim;
    CsrBtHfConnectionType conType;
    HfInstanceData_t *linkPtr;
    prim = (CsrBtCmPortnegInd *) instData->recvMsgP;

    conType = getConnectionType(instData, prim->btConnId);
    linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if ((conType == CSR_BT_HF_CONNECTION_HF) || (linkPtr->state == LpEnabled_s) || (linkPtr->state == Connected_s))
    {
        if (prim->request == TRUE)
        {
            RFC_PORTNEG_VALUES_T portPar;

            portPar.baud_rate = CSR_BT_DEFAULT_PORT_SPEED;
            portPar.data_bits = CSR_BT_DEFAULT_DATA_BITS;
            portPar.stop_bits = CSR_BT_DEFAULT_STOP_BITS;
            portPar.parity = CSR_BT_DEFAULT_PARITY;
            portPar.parity_type = CSR_BT_DEFAULT_PARITY_TYPE;
            portPar.flow_ctrl_mask = CSR_BT_DEFAULT_FLOW_CTRL_MASK;
            portPar.xon = CSR_BT_DEFAULT_XON;
            portPar.xoff = CSR_BT_DEFAULT_XOFF;
            portPar.parameter_mask = CSR_BT_DEFAULT_PARAMETER_MASK;
            CsrBtCmPortnegResSend(prim->btConnId, &portPar);
        }
        else
        {
            CsrBtCmPortnegResSend(prim->btConnId, &(prim->portPar) );
        }
    }
}


void CsrBtHfDeactivateStateCmSdsRegisterCfmHandler(HfMainInstanceData_t * instData)
{/* Unregister immediately! */
    CsrBtCmSdsRegisterCfm * prim = (CsrBtCmSdsRegisterCfm *) instData->recvMsgP;
    if ((prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) && (prim->resultSupplier == CSR_BT_SUPPLIER_CM))
    {
        CsrBtCmSdsUnRegisterReqSend(CSR_BT_HF_IFACEQUEUE, prim->serviceRecHandle);
    }
}

void CsrBtHfActivatedStateCmSdsRegisterCfmHandler(HfMainInstanceData_t * instData)
{
    CsrIntFast8 i = 0;
    CsrBool handled = FALSE;

    for (i=0;((i<(instData->maxHFConnections + instData->maxHSConnections)) && (!handled)); i++)
    {
        if ((instData->linkData[i].serviceRecHandle == 0) && (instData->linkData[i].state == Activate_s))
        {
            instData->index = i;
            handled = TRUE;
            CsrBtHfpHandler(instData);
        }
    }
}

void CsrBtHfActivatedStateCmSdsUnregisterCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmSdsUnregisterCfm * prim;
    CsrIntFast8 i = 0;
    CsrBool handled = FALSE;

    prim = (CsrBtCmSdsUnregisterCfm *) instData->recvMsgP;

    for (i=0;((i<(instData->maxHFConnections + instData->maxHSConnections)) && (!handled)); i++)
    {
        if (prim->serviceRecHandle == instData->linkData[i].serviceRecHandle)
        {
            instData->index = i;
            handled = TRUE;
            CsrBtHfpHandler(instData);
        }
    }
}

/**************************************************************************************
    Deactivate state
***************************************************************************************/
void CsrBtHfDeactivateStateCmCancelAcceptConnectCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmCancelAcceptConnectCfm * prim;
    prim = (CsrBtCmCancelAcceptConnectCfm *) instData->recvMsgP;

    if((prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS  || prim->resultCode == CSR_BT_RESULT_CODE_CM_NOTHING_TO_CANCEL)&&
       prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        csrBtHfHsUnregister(instData);
    }
    /* else wait for CSR_BT_CM_CONNECT_ACCEPT_CFM and disconnect */
}


void CsrBtHfDeactivateStateCmConnectAcceptCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmConnectAcceptCfm * prim;
    prim = (CsrBtCmConnectAcceptCfm *) instData->recvMsgP;

    /* this means that CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM failed. Disconnect at once */
    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
       prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,prim->deviceAddr,prim->btConnId);
        CsrBtCmDisconnectReqSend(prim->btConnId);
    }
    else
    {
        instData->numberOfUnregister = 0;
        CsrBtCmSdsUnRegisterReqSend(CSR_BT_HF_IFACEQUEUE, instData->linkData[instData->index].serviceRecHandle);
    }

}

void CsrBtHfDeactivateStateCmDisconnectIndHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmDisconnectInd * prim;
    CsrBtHfConnectionType conType;
    prim = (CsrBtCmDisconnectInd *) instData->recvMsgP;

    conType = getConnectionType(instData, prim->btConnId);
    
    if (instData->linkData[instData->index].deregisterTimerId != 0)
    {
        CsrUint16 mi;
        void *mv;
        CsrSchedTimerCancel(instData->linkData[instData->index].deregisterTimerId,&mi,&mv);
        instData->linkData[instData->index].deregisterTimerId = 0;
    }

    if (prim->status)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,instData->linkData[instData->index].currentDeviceAddress,
                                prim->btConnId);
        csrBtHfHsUnregister(instData);
    }
    else
    {
        if (CSR_BT_HF_CONNECTION_UNKNOWN != conType)
        {
            CsrBtCmDisconnectReqSend(prim->btConnId);
        }
    }
}

void CsrBtHfDeactivateStateCmScoConnectCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmScoConnectCfm * prim = (CsrBtCmScoConnectCfm *) instData->recvMsgP;

    /* this signal should not come in Deactivate_s, but may occur as a result of a race condition */
    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
       prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmScoDisconnectReqSend(CSR_BT_HF_IFACEQUEUE, prim->btConnId);
    }
}

void CsrBtHfDeactivateStateCmScoDisconnectIndHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmScoDisconnectInd * prim;
    prim = (CsrBtCmScoDisconnectInd *) instData->recvMsgP;

#ifdef CSR_USE_DSPM                   
    if (instData->hfDspmOpPending == DSPM_NOP)
    {
        CsrBtHfDspmSendDisconnectStreams(instData);
        instData->hfDspmOpPending = instData->index;
    }
#endif

    CsrBtCmDisconnectReqSend(prim->btConnId);
}

void CsrBtHfDeactivateStateCmScoAcceptConnectCfm(HfMainInstanceData_t * instData)
{
    CsrBtCmScoAcceptConnectCfm * prim;
    CsrIntFast8 i = 0;
    
    prim = (CsrBtCmScoAcceptConnectCfm *) instData->recvMsgP;
    
    CsrBtCmScoDisconnectReqSend(CSR_BT_HF_IFACEQUEUE, prim->btConnId);
    for (i=0;i<(instData->maxHFConnections + instData->maxHSConnections);i++)
    {
        instData->linkData[i].scoConnectAcceptPending = FALSE;
    }
}

void CsrBtHfDeactivateStateCmDataIndHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmDataInd * prim;
    CsrIntFast8 i=0;
    CsrBool found = FALSE;
    HfInstanceData_t *linkPtr;

    prim = (CsrBtCmDataInd *) instData->recvMsgP;
    CsrBtCmDataResSend(prim->btConnId);

    for (i=0;((i<(instData->maxHFConnections + instData->maxHSConnections)) && (!found));i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
        if (prim->btConnId == linkPtr->hfConnId)
        {
            found = TRUE;
            if ((linkPtr->state == Connect_s) && (linkPtr->linkType == CSR_BT_HF_CONNECTION_HS))
            {
                CsrBtCmDisconnectReqSend(prim->btConnId);
            }
        }
    }
    CsrPmemFree (prim->payload);
}

void CsrBtHfDeactivateStateCmDataCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmDataCfm * prim;
    CsrIntFast8 i=0;
    CsrBool found = FALSE;
    HfInstanceData_t *linkPtr;

    prim = (CsrBtCmDataCfm *) instData->recvMsgP;

    for (i=0;((i<(instData->maxHFConnections + instData->maxHSConnections)) && (!found));i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
        if (prim->btConnId == linkPtr->hfConnId)
        {
            found = TRUE;
            if ((linkPtr->state == Connect_s) && (linkPtr->linkType == CSR_BT_HF_CONNECTION_HS))
            {
                CsrBtCmDisconnectReqSend(prim->btConnId);
            }
        }
    }
}

void CsrBtHfDeactivateStateCmControlIndHandler(HfMainInstanceData_t * instData)
{
    /* ignore in deactivate */
    CSR_UNUSED(instData);
}
void CsrBtHfDeactivateStateCmModeChangeIndHandler(HfMainInstanceData_t * instData)
{
    /* ignore in deactivate */
    CSR_UNUSED(instData);

}


void CsrBtHfDeactivateStateCmSdsUnregisterCfmHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmSdsUnregisterCfm * prim;
    CsrIntFast8 i=0;
    CsrBool found = FALSE;
    HfInstanceData_t *linkPtr;

    prim = (CsrBtCmSdsUnregisterCfm *) instData->recvMsgP;

    instData->numberOfUnregister++;
    if ((prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM) || instData->numberOfUnregister == HF_NUMBER_OF_UNREGISTER)
    {
        for (i=0;((i<(instData->maxHFConnections + instData->maxHSConnections)) && (!found));i++)
        {
            linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
            if (linkPtr->serviceRecHandle == prim->serviceRecHandle)
            {
                found = TRUE;
                if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    linkPtr->sdsRegistered = FALSE;
                    linkPtr->serviceRecHandle = 0;
                }
                else
                {
                    linkPtr->sdsRegistered = TRUE;
                }
                linkPtr->deactivated = TRUE;
                instData->index = i;
            }
        }
        /* Now if no records activated left, just clean up and send deactivate cfm.*/
        found = FALSE;
        for (i=0;( (i<(instData->maxHFConnections + instData->maxHSConnections)) && (!found)); i++)
        {
            linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
            if (linkPtr->deactivated == FALSE)
            {
                instData->index = i;
                found = TRUE;
            }
        }
        if (!found)
        {/* Now reorganize all the records as inactive: HF first; HS last */
            if ((instData->maxHSConnections > 0) && (instData->allocInactiveHfCons > 0))
            {
                HfInstanceData_t *newPtr;
                CsrUint8 total = instData->maxHFConnections + instData->maxHSConnections + instData->allocInactiveHfCons + instData->allocInactiveHsCons;
                CsrUint8 tmpIdx = 0;
                newPtr = CsrPmemAlloc(total * sizeof(HfInstanceData_t));
                for (i=0;i<total;i++)
                {
                    if (i<instData->maxHFConnections)
                    {/* Firs the previously active HF records if any */
                        CsrMemCpy(&newPtr[i],&instData->linkData[i], sizeof(HfInstanceData_t));
                    }
                    else if (i<instData->maxHFConnections+instData->allocInactiveHfCons)
                    {/* The the previously inactive HF records if any */
                        tmpIdx = i + instData->maxHSConnections;
                        CsrMemCpy(&newPtr[i],&instData->linkData[tmpIdx], sizeof(HfInstanceData_t));
                    }
                    else if (i<instData->maxHFConnections + instData->maxHSConnections + instData->allocInactiveHfCons)
                    {/* Then the previoulsy active HS records if any */
                        tmpIdx = i - instData->allocInactiveHfCons;
                        CsrMemCpy(&newPtr[i],&instData->linkData[tmpIdx], sizeof(HfInstanceData_t));
                    }
                    else
                    { /* Last but not least the previously inactive HS records if any */
                        CsrMemCpy(&newPtr[i],&instData->linkData[i], sizeof(HfInstanceData_t));
                    }

                }
                /* now free the old pointer and let it point at the new ordered one instead */
                CsrPmemFree(instData->linkData);
                instData->linkData = newPtr;
            }
            instData->state = Null_s;
            instData->allocInactiveHfCons += instData->maxHFConnections;
            instData->allocInactiveHsCons += instData->maxHSConnections;

            if(instData->doingCleanup)
            {
                CsrBtHfCleanup(instData);
            }
            else
            {
                if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    CsrBtHfSendHfDeactivateCfm(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                }
                else
                {
                    CsrBtHfSendHfDeactivateCfm(instData, prim->resultCode, prim->resultSupplier);
                }

            }
            instData->maxHFConnections = 0;
            instData->maxHSConnections = 0;
         }
         else
         {
             linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
             if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HS)
             {
                 DeactivateHsHandler(instData);
             }
             else
             {
                 DeactivateHfHandler(instData);
             }
         }
    }
    else
    {
        /* unregister failed, try again */
        CsrBtCmSdsUnRegisterReqSend(CSR_BT_HF_IFACEQUEUE, prim->serviceRecHandle);
    }
}


void CsrBtHfCommonAtCmdPrimReqHandler(HfMainInstanceData_t *instData, CsrBtHfPrim *primType)
{
    switch(*primType)
    {
        case CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ:
        {
            CsrBtHfXStateHfGetAllStatusReqHandler(instData);
            break;
        }
        case CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ:
        {
            CsrBtHfXStateHfCopsReqHandler(instData);
            break;
        }
        case CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ:
        {
            CsrBtHfXStateHfSubscriberReqHandler(instData);
            break;
        }
        case CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ:
        {
            CsrBtHfXStateHfCallListReqHandler(instData);
            break;
        }
        case CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ:
        {
            CsrBtHfXStateHfSetExtErrorReqHandler(instData);
            break;
        }
        case  CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ:
        {
            CsrBtHfXStateHfSetClipHandler(instData);
            break;
        }
        case CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ:
        {
            CsrBtHfXStateHfSetCcwaHandler(instData);
            break;
        }
        case CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ:
        {
            CsrBtHfActivatedSetStatusIndUpdateReqHandler(instData);
            break;
        }
        case CSR_BT_HF_SET_ECHO_AND_NOISE_REQ:
        {
            CsrBtHfXStateHfSetNrecHandler(instData);
            break;
        }
        case CSR_BT_HF_SET_VOICE_RECOGNITION_REQ:
        {
            CsrBtHfXStateHfSetBvraHandler(instData);
            break;
        }
        case CSR_BT_HF_BT_INPUT_REQ:
        {
            CsrBtHfActivatedBtInputReqHandler(instData);
            break;
        }
        case CSR_BT_HF_GENERATE_DTMF_REQ:
        {
            CsrBtHfXStateDTMFReqHandler(instData);
            break;
        }
        case CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ:
        {
            CsrBtHfActivatedStateSpeakerGainStatusReqHandler(instData);
            break;
        }
        case CSR_BT_HF_MIC_GAIN_STATUS_REQ:
        {
            CsrBtHfActivatedStateMicGainStatusReqHandler(instData);
            break;
        }
        case CSR_BT_HF_DIAL_REQ:
        {
            CsrBtHfActivatedDialRequest(instData);
            break;
        }
        case CSR_BT_HF_CALL_ANSWER_REQ:
        {
            CsrBtHfActivatedStateAnswerReqHandler(instData);
            break;
        }
        case CSR_BT_HF_CALL_END_REQ:
        {
            CsrBtHfActivatedStateRejectReqHandler(instData);
            break;
        }
        case CSR_BT_HF_CALL_HANDLING_REQ:
        {
            CsrBtHfActivatedStateChldReqHandler(instData);
            break;
        }
        case CSR_BT_HF_AT_CMD_REQ:
        {
            CsrBtHfActivatedStateAtCmdReqHandler(instData);
            break;
        }
        case CSR_BT_HF_C2C_SF_REQ:
        {
            CsrBtHfXStateHfC2cSfReqHandler(instData);
            break;
        }
        case CSR_BT_HF_C2C_PWR_REQ:
        {
            CsrBtHfActivatedStateC2CPwrReqHandler(instData);
            break;
        }
        case CSR_BT_HF_C2C_BATT_REQ:
        {
            CsrBtHfActivatedStateC2CBattReqHandler(instData);
            break;
        }
        case CSR_BT_HF_C2C_GET_SMS_REQ:
        {
            CsrBtHfActivatedStateC2CGetSMSReqHandler(instData);
            break;
        }
        case CSR_BT_HF_INDICATOR_ACTIVATION_REQ:
        {
            CsrBtHfXStateIndicatorActivationReqHandler(instData);
            break;
        }
    }
}

void CsrBtHfXStateHfCommonAtCmdReqHandler(HfMainInstanceData_t *instData)
{
    if (instData->linkData[instData->index].lastAtCmdSent != idleCmd)
    {/* Save the whole message! */
        CsrMessageQueuePush(&instData->linkData[instData->index].data->cmDataReqQueue, CSR_BT_HF_PRIM, instData->recvMsgP);
        instData->recvMsgP = NULL;
    }
    else
    {
        CsrBtHfPrim         *primType;

        /* find the message type */
        primType = (CsrBtHfPrim *)instData->recvMsgP;

        CsrBtHfCommonAtCmdPrimReqHandler(instData,primType);

    }
}

void CsrBtHfXStateHfUpdateCodecSupportReqHandler(HfMainInstanceData_t *instData)
{
    if (instData->localSupportedFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION)
    {/* First update the supported codec list */
        CsrBtHfUpdateSupportedCodecReq *prim = (CsrBtHfUpdateSupportedCodecReq *) instData->recvMsgP;
        CsrBool changed = FALSE;
        
        if (prim->enable && !(instData->supportedCodecsMask & prim->codecMask) )
        {
            changed = TRUE;
            instData->supportedCodecsMask |= prim->codecMask;
        }
        else if (!prim->enable && (instData->supportedCodecsMask & prim->codecMask))
        {/* Disable */
            changed = TRUE;
            instData->supportedCodecsMask ^= prim->codecMask;
        }

        if (changed && prim->sendUpdate)
        {/* Indicate the change to the remote device if needed */
            CsrUintFast8 tmp = instData->index;
            CsrUint8 i;
            for (i=0; i < instData->maxHFConnections ;i++)
            {
                HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                if ((linkPtr->state == Connected_s) || (linkPtr->state == LpEnabled_s))
                {/* Handsfree connection found: is codec negotiation supported on it? */
                    if (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION)
                    {/* Yes: let the remote know about the change */
                        instData->index = i;
                        sendCodecSupport(instData);
                    }
                }
            }
            /* Restore the current connection index */
            instData->index = tmp;
        }

        CsrBtHfSendUpdateCodecSupportedCfm(instData);
    }
}

/*************************************************************************************
    Handle C2cSF request
************************************************************************************/
void CsrBtHfXStateHfC2cSfReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfC2cSfReq *prim = (CsrBtHfC2cSfReq *)instData->recvMsgP;

    if (prim->number < CSR_BT_C2C_NUMBER_OF_INDICATORS)
    {
        instData->locC2C[prim->number] = prim->value;
        if (instData->state == Activated_s)
        {/* The information must be sent to the remote device then (if it is a HFG) */
            HfInstanceData_t *linkPtr;

            if (prim->connectionId != CSR_BT_HF_CONNECTION_ALL)
            {
                if (getConnTypeFromConnId(instData,prim->connectionId) == CSR_BT_HF_CONNECTION_HF)
                {
                    linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
                    if ((linkPtr->linkType == CSR_BT_HF_CONNECTION_HF) &&
                        (linkPtr->linkInd.locC2C[prim->number] != prim->value) &&
                        (linkPtr->linkInd.remoteSupport))
                    {/* Send information to remote device */
                        CsrBtHfSendC2CSf(instData);
                    }
                    linkPtr->linkInd.locC2C[prim->number] = prim->value;
                }
                /*else just ignore....no valid connection Id provided!*/
            }
            else
            {
                CsrIntFast8 i;
                for (i=0;i<(instData->maxHFConnections + instData->maxHSConnections);i++)
                {
                    linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                    if ((linkPtr->linkType == CSR_BT_HF_CONNECTION_HF) &&
                        (linkPtr->linkInd.locC2C[prim->number] != prim->value) &&
                        (linkPtr->linkInd.remoteSupport))
                    {/* Send information to remote device */
                        CsrBtHfSendC2CSf(instData);
                    }
                    linkPtr->linkInd.locC2C[prim->number] = prim->value;
                }
            }
        }
    }
}

/*************************************************************************************
    startCopsSequence
************************************************************************************/
void CsrBtHfXStateHfCopsReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfGetCurrentOperatorSelectionReq *prim;
    CsrBool OpNotAllowed = TRUE;

    prim = (CsrBtHfGetCurrentOperatorSelectionReq *) instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if (instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF)
        {
            OpNotAllowed = FALSE;
            if (prim->forceResendingFormat)
            {
                /*instData->linkData[instData->index].atSequenceState = copsQuery;*/
                CsrBtHfAtCopsSetCommandSend(instData,prim->mode,prim->format);
                CsrBtHfAtCopsQuerySend(instData);
            }
            else
            {
                instData->linkData[instData->index].atSequenceState = rest;
                CsrBtHfAtCopsQuerySend(instData);
            }
        }
    }


    if (OpNotAllowed)
    {/* Not possible according to HSP spec */
        CsrBtHfGetCurrentOperatorSelectionCfm    *primCfm;

        primCfm = (CsrBtHfGetCurrentOperatorSelectionCfm *)CsrPmemAlloc(sizeof(CsrBtHfGetCurrentOperatorSelectionCfm));
        primCfm->type = CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM;
        primCfm->connectionId = prim->connectionId;

        primCfm->copsString = NULL;
        primCfm->cmeeResultCode = CSR_BT_CME_OPERATION_NOT_ALLOWED;

        CsrBtHfMessagePut(instData->appHandle, primCfm);
    }
}

/*****************************************************************************************
    Retrieve all the status indicators from the remote device
******************************************************************************************/
void CsrBtHfXStateHfGetAllStatusReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfGetAllStatusIndicatorsReq *prim;
    CsrBool OpNotAllowed = TRUE;

    prim = (CsrBtHfGetAllStatusIndicatorsReq *) instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if (instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF)
        {
            OpNotAllowed = FALSE;
            sendCindStatus(instData);
        }
    }


    if (OpNotAllowed)
    {/* Not possible according to HSP spec */
        CsrBtHfGetAllStatusIndicatorsCfm    *primCfm;

        primCfm = (CsrBtHfGetAllStatusIndicatorsCfm *)CsrPmemAlloc(sizeof(CsrBtHfGetAllStatusIndicatorsCfm));
        primCfm->type = CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM;
        primCfm->connectionId = prim->connectionId;
        primCfm->indicatorSupported = NULL;
        primCfm->indicatorValue     = NULL;
        primCfm->cmeeResultCode     = CSR_BT_CME_OPERATION_NOT_ALLOWED;

        CsrBtHfMessagePut(instData->appHandle, primCfm);
    }
}

/*****************************************************************************************
    Enable or disable the call waiting feature the remote device
******************************************************************************************/
void CsrBtHfXStateHfSetCcwaHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfSetCallWaitingNotificationReq *prim;
    CsrBool OpNotAllowed = TRUE;

    prim = (CsrBtHfSetCallWaitingNotificationReq *)instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((instData->linkData[instData->index].state == Connected_s) || (instData->linkData[instData->index].state == LpEnabled_s)))
        {/* Send AT+CCWA=X */
            OpNotAllowed = FALSE;
            CsrBtHfAtCcwaSend(instData,prim->enable);
        }
    }

    if (OpNotAllowed)
    {
        CsrBtHfSendHfGeneralCfmMsg(instData,CSR_BT_CME_OPERATION_NOT_ALLOWED,CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM);
    }
}


/*****************************************************************************************
    Enable or disable the noise reduction and echo cancelation feature the remote device
******************************************************************************************/
void CsrBtHfXStateHfSetNrecHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfSetEchoAndNoiseReq *prim;
    CsrBool OpNotAllowed = TRUE;

    prim = (CsrBtHfSetEchoAndNoiseReq *)instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((instData->linkData[instData->index].state == Connected_s) || (instData->linkData[instData->index].state == LpEnabled_s)))
        {/* Send AT+NREC=X */
            OpNotAllowed = FALSE;
            CsrBtHfAtNrecSend(instData,prim->enable);
        }
    }

    if (OpNotAllowed)
    {
        CsrBtHfSendHfGeneralCfmMsg(instData,CSR_BT_CME_OPERATION_NOT_ALLOWED,CSR_BT_HF_SET_ECHO_AND_NOISE_CFM);
    }
}



/*****************************************************************************************
    Start or stop the voice recognition feature the remote device
******************************************************************************************/
void CsrBtHfXStateHfSetBvraHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfSetVoiceRecognitionReq *prim;
    CsrBool OpNotAllowed = TRUE;

    prim = (CsrBtHfSetVoiceRecognitionReq *)instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((instData->linkData[instData->index].state == Connected_s) || (instData->linkData[instData->index].state == LpEnabled_s)))
        {/* Send AT+BVRA=X */
            OpNotAllowed = FALSE;
            CsrBtHfAtBvraSend(instData,prim->start);
        }
    }

    if (OpNotAllowed)
    {
        CsrBtHfSendHfGeneralCfmMsg(instData,CSR_BT_CME_OPERATION_NOT_ALLOWED,CSR_BT_HF_SET_VOICE_RECOGNITION_CFM);
    }
}

/*****************************************************************************************
    Ask the remote device to generate a DTMF tone
******************************************************************************************/
void CsrBtHfXStateDTMFReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfGenerateDtmfReq *prim;
    CsrBool OpNotAllowed = TRUE;

    prim = (CsrBtHfGenerateDtmfReq *)instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((instData->linkData[instData->index].state == Connected_s) || (instData->linkData[instData->index].state == LpEnabled_s)))
        {/* Send AT+VTS=X */
            OpNotAllowed = FALSE;
            CsrBtHfAtVtsSend(instData,prim->dtmf);
        }
    }

    if (OpNotAllowed)
    {
        CsrBtHfSendHfGeneralCfmMsg(instData,CSR_BT_CME_OPERATION_NOT_ALLOWED,CSR_BT_HF_GENERATE_DTMF_CFM);
    }
}

/*****************************************************************************************
    Enable or disable the call notification feature the remote device
******************************************************************************************/
void CsrBtHfXStateHfSetClipHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfSetCallNotificationIndicationReq *prim;
    CsrBool OpNotAllowed = TRUE;

    prim = (CsrBtHfSetCallNotificationIndicationReq *)instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((instData->linkData[instData->index].state == Connected_s) || (instData->linkData[instData->index].state == LpEnabled_s)))
        {/* Send AT+CLIP=X */
            OpNotAllowed = FALSE;
            CsrBtHfAtClipSend(instData,prim->enable);
        }
    }

    if (OpNotAllowed)
    {
        CsrBtHfSendHfGeneralCfmMsg(instData,CSR_BT_CME_OPERATION_NOT_ALLOWED,CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM);
    }
}

/*****************************************************************************************
    Enable or disable the extended error feature in the remote device
******************************************************************************************/
void CsrBtHfXStateHfSetExtErrorReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfSetExtendedAgErrorResultCodeReq *prim;
    CsrBool OpNotAllowed = TRUE;
    prim = (CsrBtHfSetExtendedAgErrorResultCodeReq *) instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((instData->linkData[instData->index].state == Connected_s) || (instData->linkData[instData->index].state == LpEnabled_s)))
        {/* Send AT+CMEE=X */
            OpNotAllowed = FALSE;
            CsrBtHfAtCmeeSetCommandSend(instData,prim->enable);
        }
    }

    if (OpNotAllowed)
    {
        CsrBtHfSendHfGeneralCfmMsg(instData,CSR_BT_CME_OPERATION_NOT_ALLOWED,CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM);
    }
}

/*****************************************************************************************
    get the current status of the indicators in the AG (CMER=3,0,?)
******************************************************************************************/
void CsrBtHfActivatedSetStatusIndUpdateReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfSetStatusIndicatorUpdateReq *prim;
    CsrBool OpNotAllowed = TRUE;
    prim = (CsrBtHfSetStatusIndicatorUpdateReq *) instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((instData->linkData[instData->index].state == Connected_s) || (instData->linkData[instData->index].state == LpEnabled_s)))
        {/* Send AT+CMER=3,0,enable */
            OpNotAllowed = FALSE;
            sendSetCmer(instData,prim->enable);
        }
    }

    if (OpNotAllowed)
    {
        CsrBtHfSendHfGeneralCfmMsg(instData,CSR_BT_CME_OPERATION_NOT_ALLOWED,CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM);
    }
}

/*****************************************************************************************
    request input data from the AG (BINP)
******************************************************************************************/
void CsrBtHfActivatedBtInputReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfBtInputReq *prim;
    CsrBool OpNotAllowed = TRUE;
    prim = (CsrBtHfBtInputReq *) instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((instData->linkData[instData->index].state == Connected_s) || (instData->linkData[instData->index].state == LpEnabled_s)))
        {/* Send AT+BINP */
            OpNotAllowed = FALSE;
            CsrBtHfAtBinpSend(instData,prim->dataRequest);
        }
    }

    if (OpNotAllowed)
    {/* Not possible according to HSP spec */
        CsrBtHfBtInputCfm   *primCfm;

        primCfm = (CsrBtHfBtInputCfm *)CsrPmemAlloc(sizeof(CsrBtHfBtInputCfm));
        primCfm->type = CSR_BT_HF_BT_INPUT_CFM;
        primCfm->connectionId = prim->connectionId;
        primCfm->dataRespString = NULL;
        primCfm->cmeeResultCode = CSR_BT_CME_OPERATION_NOT_ALLOWED;

        CsrBtHfMessagePut(instData->appHandle, primCfm);
    }
}


/*****************************************************************************************
    request to dial out (ATD)
******************************************************************************************/
void CsrBtHfActivatedDialRequest(HfMainInstanceData_t *instData)
{
    CsrBtHfDialReq  *prim;
    CsrBool OpNotAllowed = TRUE;

    prim = (CsrBtHfDialReq *) instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((instData->linkData[instData->index].state == Connected_s) || (instData->linkData[instData->index].state == LpEnabled_s)))
        {/* Send ATD */
            OpNotAllowed = FALSE;
            CsrBtHfAtDialSend(instData,prim->command,prim->number);
        }
    }

    if (OpNotAllowed)
    {/* Not possible according to HSP spec */
        CsrBtHfDialCfm   *primCfm;

        primCfm = (CsrBtHfDialCfm *)CsrPmemAlloc(sizeof(CsrBtHfDialCfm));
        primCfm->type = CSR_BT_HF_DIAL_CFM;
        primCfm->connectionId = prim->connectionId;
        primCfm->cmeeResultCode = CSR_BT_CME_OPERATION_NOT_ALLOWED;

        CsrBtHfMessagePut(instData->appHandle, primCfm);

        CsrPmemFree(prim->number);
    }

}

/*****************************************************************************************
    get the current call list (CLCC)
******************************************************************************************/
void CsrBtHfXStateHfCallListReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfGetCurrentCallListReq *prim;
    CsrBool OpNotAllowed = TRUE;
    prim = (CsrBtHfGetCurrentCallListReq *) instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((instData->linkData[instData->index].state == Connected_s) || (instData->linkData[instData->index].state == LpEnabled_s)))
        {/* Send AT+CLCC */
            OpNotAllowed = FALSE;
            CsrBtHfAtClccSend(instData);
        }
    }

    if (OpNotAllowed)
    {/* Not possible according to HSP spec */
        CsrBtHfGetCurrentCallListCfm   *primCfm;

        primCfm = (CsrBtHfGetCurrentCallListCfm *)CsrPmemAlloc(sizeof(CsrBtHfGetCurrentCallListCfm));
        primCfm->type = CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM;
        primCfm->connectionId = prim->connectionId;
        primCfm->cmeeResultCode = CSR_BT_CME_OPERATION_NOT_ALLOWED;

        CsrBtHfMessagePut(instData->appHandle, primCfm);
    }
}

/*****************************************************************************************
    get the subscriber number (CNUM)
******************************************************************************************/
void CsrBtHfXStateHfSubscriberReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfGetSubscriberNumberInformationReq *prim;
    CsrBool OpNotAllowed = TRUE;
    prim = (CsrBtHfGetSubscriberNumberInformationReq *)instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((instData->linkData[instData->index].state == Connected_s) || (instData->linkData[instData->index].state == LpEnabled_s)))
        {/* Send AT+CNUM */
            OpNotAllowed = FALSE;
            CsrBtHfAtCnumSend(instData);
        }
    }

    if (OpNotAllowed)
    {/* Not possible according to HSP spec */
        CsrBtHfGetSubscriberNumberInformationCfm   *primCfm;

        primCfm = (CsrBtHfGetSubscriberNumberInformationCfm *)CsrPmemAlloc(sizeof(CsrBtHfGetSubscriberNumberInformationCfm));
        primCfm->type = CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM;
        primCfm->connectionId = prim->connectionId;
        primCfm->cmeeResultCode = CSR_BT_CME_OPERATION_NOT_ALLOWED;

        CsrBtHfMessagePut(instData->appHandle, primCfm);
    }
}
/*****************************************************************************************
    request to send C2C Power info message
******************************************************************************************/
void CsrBtHfActivatedStateC2CPwrReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfC2cPwrReq  *prim;
    CsrBool OpNotAllowed = TRUE;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&instData->linkData[instData->index];

    prim = (CsrBtHfC2cPwrReq *) instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((linkPtr->linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((linkPtr->state == Connected_s) || (linkPtr->state == LpEnabled_s)) &&
            (linkPtr->linkInd.remC2C[CSR_BT_C2C_PWR_IND] == 1))
        {/* Send ATD */
            OpNotAllowed = FALSE;
            CsrBtHfSendC2CPwr(instData);
        }
    }

    if (OpNotAllowed)
    {/* Not possible according to HSP spec */
        CsrBtHfC2cPwrCfm   *primCfm;

        primCfm = (CsrBtHfC2cPwrCfm *)CsrPmemAlloc(sizeof(CsrBtHfC2cPwrCfm));
        primCfm->type = CSR_BT_HF_C2C_PWR_CFM;
        primCfm->connectionId = prim->connectionId;
        primCfm->cmeeResultCode = CSR_BT_CME_OPERATION_NOT_ALLOWED;

        CsrBtHfMessagePut(instData->appHandle, primCfm);
    }
}

/*****************************************************************************************
    request to send C2C Battery info message
******************************************************************************************/
void CsrBtHfActivatedStateC2CBattReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfC2cBattReq  *prim;
    CsrBool OpNotAllowed = TRUE;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&instData->linkData[instData->index];

    prim = (CsrBtHfC2cBattReq *) instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((linkPtr->linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((linkPtr->state == Connected_s) || (linkPtr->state == LpEnabled_s)) &&
            (linkPtr->linkInd.remC2C[CSR_BT_C2C_BAT_IND] == 1))
        {/* Send  */
            OpNotAllowed = FALSE;
            CsrBtHfSendC2CBatt(instData);
        }
    }

    if (OpNotAllowed)
    {/* Not possible according to HSP spec */
        CsrBtHfC2cBattCfm   *primCfm;

        primCfm = (CsrBtHfC2cBattCfm *)CsrPmemAlloc(sizeof(CsrBtHfC2cBattCfm));
        primCfm->type = CSR_BT_HF_C2C_BATT_CFM;
        primCfm->connectionId = prim->connectionId;
        primCfm->cmeeResultCode = CSR_BT_CME_OPERATION_NOT_ALLOWED;

        CsrBtHfMessagePut(instData->appHandle, primCfm);
    }
}

/*****************************************************************************************
    request to send C2C get SMS info message
******************************************************************************************/
void CsrBtHfActivatedStateC2CGetSMSReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfC2cGetSmsReq  *prim;
    CsrBool OpNotAllowed = TRUE;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&instData->linkData[instData->index];

    prim = (CsrBtHfC2cGetSmsReq *) instData->recvMsgP;

    if (instData->state == Activated_s)
    {
        if ((linkPtr->linkType == CSR_BT_HF_CONNECTION_HF) &&
            ((linkPtr->state == Connected_s) || (linkPtr->state == LpEnabled_s)) &&
            (linkPtr->linkInd.remC2C[CSR_BT_C2C_SMS_IND] == 1))
        {/* Send */
            OpNotAllowed = FALSE;
            CsrBtHfSendC2CGetSMS(instData);
        }
    }

    if (OpNotAllowed)
    {/* Not possible according to HSP spec */
        CsrBtHfC2cGetSmsCfm  *primCfm;

        primCfm = (CsrBtHfC2cGetSmsCfm *)CsrPmemAlloc(sizeof(CsrBtHfC2cGetSmsCfm));
        primCfm->type = CSR_BT_HF_C2C_GET_SMS_CFM;
        primCfm->connectionId = prim->connectionId;
        primCfm->cmeeResultCode = CSR_BT_CME_OPERATION_NOT_ALLOWED;
        primCfm->smsString    = NULL;

        CsrBtHfMessagePut(instData->appHandle, primCfm);
    }
}

/**************************************************************************************************/
void CsrBtHfActivatedStateHfDeactivateReqHandler(HfMainInstanceData_t * instData)
{
    DeactivateHfHandler(instData);
}

void CsrBtHfActivatedStateHfCancelReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtHfCancelConnectReq * prim;
    prim = (CsrBtHfCancelConnectReq *) instData->recvMsgP;
    /* First check whether there is a pending connection to that bd_address....*/
    if (FindPendingConnectionToBdAddress(instData, prim->deviceAddr))
    {/* The function above has found the proper instData->index...
       at this point we might not know what type of connection this is */
       CsrBtHfpHandler(instData);
    }
    /* if no pending connection is found, just ignore this request; probably it is due to a
       cross situation bwteen a connection just established and the application trying to
       cancel the operation. The application shall disconnect if it wants to, when the
       it receives the crossing service connect indication */
}

void CsrBtHfActivatedStateHfDisconnectReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtHfDisconnectReq * prim;
    prim = (CsrBtHfDisconnectReq *) instData->recvMsgP;

    switch (getConnTypeFromConnId(instData, prim->connectionId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateHfServiceConnectReq(HfMainInstanceData_t * instData)
{
    CsrUint8 nrActiveHf = 0;
    CsrUint8 nrActiveHs = 0;

    if (instData->maxTotalSimultaneousConnections > CsrBtHfGetNumberOfRecordsInUse(instData,&nrActiveHf,&nrActiveHs))
    {
        CsrBtHfServiceConnectReq * prim;
        CsrUint8    serviceCount;
        CsrIntFast8    i = 0;
        HfInstanceData_t *linkPtr;

        serviceCount = 0;
        prim = (CsrBtHfServiceConnectReq *) instData->recvMsgP;

        if ((prim->connectionType == CSR_BT_HF_CONNECTION_HF) || (prim->connectionType == CSR_BT_HF_CONNECTION_UNKNOWN))
        {
            for (i=0; i < instData->maxHFConnections; i++)
            {
                linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                if ((linkPtr->state == Activate_s) && (linkPtr->sdsRegistered))
                {
                    serviceCount ++;
                }
            }
        }

        if  ((prim->connectionType == CSR_BT_HF_CONNECTION_HS) || (prim->connectionType == CSR_BT_HF_CONNECTION_UNKNOWN))
        {
            for (i=instData->maxHFConnections; i < (instData->maxHSConnections + instData->maxHFConnections); i++)
            {
                linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                if ((linkPtr->state == Activate_s) && (linkPtr->sdsRegistered))
                {
                    serviceCount ++;
                }
            }
        }
        instData->linkRecoveryCounter=0;
        instData->currentDeviceAddress = prim->deviceAddr;
        if (serviceCount != 0)
        {
            startSdcFeatureSearch(instData, TRUE);
        }
        else
        {
            CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_REJECTED_BY_PROFILE, CSR_BT_SUPPLIER_HF);
        }
    }
    else
    {
        CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_MAX_NUM_OF_CONNECTIONS, CSR_BT_SUPPLIER_HF);
    }
}

void CsrBtHfActivatedStateAudioDisconnectReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtHfAudioDisconnectReq * prim;
    HfInstanceData_t *linkPtr;
    CsrBtHfConnectionType conType;

    prim = (CsrBtHfAudioDisconnectReq *) instData->recvMsgP;
    conType = getConnTypeFromConnId(instData, prim->connectionId);
    linkPtr  = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    switch (conType)
    {
        case CSR_BT_HF_CONNECTION_HF:
            {
               linkPtr->outgoingAudio = TRUE;

                linkPtr->audioQuality = linkPtr->audioSetupParams.theAudioQuality;
                linkPtr->lastAudioReq = FALSE;

                if (linkPtr->scoHandle != HF_SCO_UNUSED)
                {/* If no audio connection exists, just answer back */
                    CsrBtCmScoDisconnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                    linkPtr->audioPending = TRUE;
                }
                else
                {/* else if audio connection exists, just disconnect; no need to get out of LP mode */
                    CsrBtHfSendHfAudioDisconnectCfm(instData, linkPtr->scoHandle,
                        CSR_BT_RESULT_CODE_HF_SYNCHRONOUS_CONNECTION_LIMIT_EXCEEDED,
                        CSR_BT_SUPPLIER_HF);
                }
                break;
            }
        case CSR_BT_HF_CONNECTION_HS:
            {
                linkPtr->outgoingAudio = TRUE;
                if (linkPtr->state == Connected_s)
                {
                    linkPtr->lastAudioReq = FALSE;
                    if (!linkPtr->audioPending)
                    {
                        if (((linkPtr->scoHandle != HF_SCO_UNUSED)))
                        { /* Send Button pressed signal to indicate we want to open or close audio,
                           * and wait for AG to connect or remove SCO */
                            sendCkpd(instData);
                            linkPtr->audioPending = TRUE;
                        }
                    }
                }
                else if (linkPtr->state == LpEnabled_s)
                {
                    linkPtr->lastAudioReq = FALSE;
                    if (linkPtr->audioPending == FALSE)
                    {
                        linkPtr->audioPending = TRUE;
                        CsrBtCmRfcModeChangeReqSend(linkPtr->hfConnId,
                                            CSR_BT_LINK_STATUS_CONNECTED);
                    }
                }
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateAudioReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtHfAudioConnectReq * prim;
    HfInstanceData_t *linkPtr;
    CsrBtHfConnectionType conType;

    prim = (CsrBtHfAudioConnectReq *) instData->recvMsgP;
    conType = getConnTypeFromConnId(instData, prim->connectionId);
    linkPtr  = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    if (linkPtr->audioAcceptPending)
    {/* Reject: there is an incoming audio ongoing: do not allow outgoing to the same device! */
        CsrBtHfSendHfAudioInd(instData, 0,0,0,0,0,0,0,
                            CSR_BT_RESULT_CODE_HF_SYNCHRONOUS_CONNECTION_ALREADY_CONNECTING,
                            0xDEAD,
                            CSR_BT_SUPPLIER_HF);
    }
    else
    {
        switch (conType)
        {
            case CSR_BT_HF_CONNECTION_HF:
                {
                   linkPtr->outgoingAudio = TRUE;

                    CsrBtHfpHandler(instData);
                    break;
                }
            case CSR_BT_HF_CONNECTION_HS:
                {
                    linkPtr->outgoingAudio = TRUE;
                    linkPtr->pcmSlot = prim->pcmSlot;
                    linkPtr->pcmReassign = prim->pcmRealloc;
                    linkPtr->pcmMappingReceived = TRUE;

                    if (linkPtr->state == Connected_s)
                    {
                        linkPtr->lastAudioReq = TRUE;
                        if (!linkPtr->audioPending)
                        {
                            if (linkPtr->scoHandle == HF_SCO_UNUSED)
                            { /* Send Button pressed signal to indicate we want to open or close audio,
                               * and wait for AG to connect or remove SCO */
                                sendCkpd(instData);
                                linkPtr->audioPending = TRUE;
                            }
                        }
                    }
                    else if (linkPtr->state == LpEnabled_s)
                    {
                        linkPtr->lastAudioReq = TRUE;
                        if (linkPtr->audioPending == FALSE)
                        {
                            linkPtr->audioPending = TRUE;
                            CsrBtCmRfcModeChangeReqSend(linkPtr->hfConnId,
                                                CSR_BT_LINK_STATUS_CONNECTED);
                        }
                    }
                    break;
                }
            default:
                {
                    break;
                }
        }
    }
}

void CsrBtHfActivatedStateMapScoPcmResHandler(HfMainInstanceData_t * instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    if ( (linkPtr->state == Connected_s) ||
         ((linkPtr->linkType == CSR_BT_HF_CONNECTION_HF) && (linkPtr->state == LpEnabled_s)) )
    {
        CsrBtHfAudioAcceptConnectRes *prim = (CsrBtHfAudioAcceptConnectRes *)instData->recvMsgP;
        CsrBtCmScoCommonParms *scoParms = (CsrBtCmScoCommonParms *)CsrPmemAlloc(sizeof(CsrBtCmScoCommonParms));

        linkPtr->pcmSlot            = prim->pcmSlot;
        linkPtr->pcmReassign        = prim->pcmReassign;
        if (prim->acceptResponse == HCI_SUCCESS)
        {
            linkPtr->pcmMappingReceived = TRUE;
        }
        linkPtr->audioAcceptPending = FALSE;

         if (/* (linkPtr->remoteVersion == CSR_BT_FIRST_HFP_CODEC_NEG_ESCO) &&*/
            (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION) &&
            (instData->localSupportedFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION))
        {
            linkPtr->audioSetupParams.theAudioQuality = CSR_BT_HF_WBS_AUDIO_QUALITY; /* 2-EV3 may be used */

            if (linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC)
            {/* mSBC: transparent data; 2-EV3 and EV3 may be used; tx-rx-bdwidth = 8000
                max Latency 8 ms or 13 msec (T1 and T2 settings) retransmission effort = 0x02 */
                linkPtr->audioSetupParams.theVoiceSettings |= CSR_BT_AIRCODING_TRANSPARENT_DATA;
                linkPtr->audioSetupParams.theAudioQuality = CSR_BT_HF_WBS_AUDIO_QUALITY;
                linkPtr->audioSetupParams.theMaxLatency = CSR_BT_HF_WBS_T2_LATENCY;
            }
            else if (linkPtr->codecToUse == CSR_BT_WBS_CVSD_CODEC)
            {/* CVSD audio: accept default settings */
                linkPtr->audioSetupParams.theVoiceSettings = CSR_BT_ESCO_DEFAULT_CONNECT_VOICE_SETTINGS; 
                linkPtr->audioSetupParams.theAudioQuality  = CSR_BT_ESCO_DEFAULT_CONNECT_AUDIO_QUALITY;
                linkPtr->audioSetupParams.theMaxLatency    = CSR_BT_ESCO_DEFAULT_CONNECT_MAX_LATENCY;
            }
        }
            scoParms->audioQuality = linkPtr->audioSetupParams.theAudioQuality;
            scoParms->txBandwidth = linkPtr->audioSetupParams.theTxBandwidth;
            scoParms->rxBandwidth = linkPtr->audioSetupParams.theRxBandwidth;
            scoParms->maxLatency = linkPtr->audioSetupParams.theMaxLatency;
            scoParms->voiceSettings =linkPtr->audioSetupParams.theVoiceSettings;
            scoParms->reTxEffort = linkPtr->audioSetupParams.theReTxEffort;
            CsrPmemFree(prim->acceptParameters);

        CsrBtCmMapScoPcmExtResSend(linkPtr->hfConnId,
                           prim->acceptResponse,
                           scoParms,
                           prim->pcmSlot,
                           prim->pcmReassign);
    }
}

void CsrBtHfMainXStateMapScoPcmIndHandler(HfMainInstanceData_t * instData)
{
    CsrBtCmMapScoPcmInd *cmPrim;

    cmPrim = (CsrBtCmMapScoPcmInd *)instData->recvMsgP;

    switch (getConnectionType(instData, cmPrim->btConnId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateAnswerReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtHfCallAnswerReq * prim;
    prim = (CsrBtHfCallAnswerReq *) instData->recvMsgP;

    switch (getConnTypeFromConnId(instData, prim->connectionId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateRejectReqHandler(HfMainInstanceData_t * instData)
{
    /* Only for HF */
    CsrBtHfpHandler(instData);
}

void CsrBtHfActivatedStateSpeakerGainStatusReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtHfSpeakerGainStatusReq * prim;
    prim = (CsrBtHfSpeakerGainStatusReq *) instData->recvMsgP;

    switch (getConnTypeFromConnId(instData, prim->connectionId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateMicGainStatusReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtHfMicGainStatusReq * prim;
    prim = (CsrBtHfMicGainStatusReq *) instData->recvMsgP;
    switch (getConnTypeFromConnId(instData, prim->connectionId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateAtCmdReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtHfAtCmdReq * prim;
    prim = (CsrBtHfAtCmdReq *) instData->recvMsgP;

    switch (getConnTypeFromConnId(instData, prim->connectionId))
    {
        case CSR_BT_HF_CONNECTION_HF:
        case CSR_BT_HF_CONNECTION_HS:
            {
                CsrBtHfpHandler(instData);
                break;
            }
        default:
            {
                break;
            }
    }
}

void CsrBtHfActivatedStateChldReqHandler(HfMainInstanceData_t * instData)
{
    /* This signal is only for the HF */
    CsrBtHfpHandler(instData);
}

/* Internal: check if ADPCM is supported by chip */
void CsrBtHfGetC2CAdpcmLocalSupportedReqHandler(HfMainInstanceData_t *instData)
{
    if (instData->appHandle != 0xAA)
    {
        CsrBtHfGetC2cAdpcmLocalSupportedInd *hfPrim;
        hfPrim          = CsrPmemAlloc(sizeof(CsrBtHfGetC2cAdpcmLocalSupportedInd));
        hfPrim->type    = CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND;
        hfPrim->result   =  FALSE;
        CsrBtHfMessagePut(instData->appHandle, hfPrim );
    }
}

void CsrBtHfXStateSetDeregisterTimeReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfDeregisterTimeReq *prim = (CsrBtHfDeregisterTimeReq*) instData->recvMsgP;
    CsrBtHfDeregisterTimeCfm *hfPrim;

    instData->deregisterTime = prim->waitSeconds;
    /* Send confirmation message */
    hfPrim = (CsrBtHfDeregisterTimeCfm *)CsrPmemAlloc(sizeof(CsrBtHfDeregisterTimeCfm));
    hfPrim->type = CSR_BT_HF_DEREGISTER_TIME_CFM;
    hfPrim->result = CSR_BT_CME_SUCCESS;

    CsrBtHfMessagePut(instData->appHandle, hfPrim );
}

void CsrBtHfMainIgnoreMessage(HfMainInstanceData_t *instData)
{
    /* Just ignore; don't raise exception */
}

void CsrBtHfXStateIndicatorActivationReqHandler(HfMainInstanceData_t *instData)
{
    CsrBtHfIndicatorActivationReq *prim = (CsrBtHfIndicatorActivationReq *)instData->recvMsgP;
    CsrBtHfConnectionType conType = getConnTypeFromConnId(instData, prim->connectionId);

    if ((conType != CSR_BT_HF_CONNECTION_HF) ||
        ((instData->linkData[instData->index].state != Connected_s) && (instData->linkData[instData->index].state != LpEnabled_s)) ||
        (instData->linkData[instData->index].cindSupportString == NULL))
    {/* The AT+BIA command shall only be issued on HFP connections; it is meant to be valid only for the SLC established and
        shall not be 'remembered' from connection to connection!
        Send negative confirmation back */
        CsrBtHfIndicatorActivationCfm *hfPrim = (CsrBtHfIndicatorActivationCfm *)CsrPmemAlloc(sizeof(CsrBtHfIndicatorActivationCfm));
        hfPrim->type         = CSR_BT_HF_INDICATOR_ACTIVATION_CFM;
        hfPrim->connectionId = prim->connectionId;
        if (conType == CSR_BT_HF_CONNECTION_HF)
        {/* Correct type link but not in the proper state */
            hfPrim->result       = CSR_BT_CME_NO_CONNECTION_TO_PHONE;
        }
        else
        {/* Wrong type of link: either not existent or HS connection */
            hfPrim->result       = CSR_BT_CME_OPERATION_NOT_ALLOWED;
        }

        CsrBtHfMessagePut(instData->appHandle, hfPrim );
    }
    else
    {/* Get the information, format the AT+BIA command and send it */
        sendBia(instData);
    }

}

