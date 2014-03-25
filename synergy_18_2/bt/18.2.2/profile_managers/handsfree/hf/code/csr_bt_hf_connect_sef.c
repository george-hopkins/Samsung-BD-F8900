/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
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
#include "csr_bt_hf_util.h"
#include "csr_bt_hf_at_inter.h"

/*************************************************************************************
    Connection attempt from peer complete, if success start service search.
************************************************************************************/
void CsrBtHfActivateStateCmConnectAcceptCfmHandler(HfMainInstanceData_t *instData)
{
    CsrBtCmConnectAcceptCfm *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    CsrUint8 nrActiveHf = 0;
    CsrUint8 nrActiveHs = 0;
    CsrUint8 nrOfActiveConnections = CsrBtHfGetNumberOfRecordsInUse(instData,&nrActiveHf,&nrActiveHs);

    prim = (CsrBtCmConnectAcceptCfm *) instData->recvMsgP;

    if (instData->maxTotalSimultaneousConnections > nrOfActiveConnections)
    {/* Allowed to establish more connections */
        instData->currentDeviceAddress = linkPtr->currentDeviceAddress = prim->deviceAddr;

        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
            prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            linkPtr->hfConnId = prim->btConnId;
            linkPtr->data->maxRfcFrameSize    = prim->profileMaxFrameSize;
            linkPtr->oldState = Activate_s;
            linkPtr->linkState = CSR_BT_LINK_STATUS_CONNECTED;
            linkPtr->incomingSlc = TRUE;
            if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
            {/* HFP */
                linkPtr->state = ServiceSearch_s;
                linkPtr->serviceState = sdcState_s;
                startSdcFeatureSearch(instData, FALSE);
            }
            else
            {/* HSP */
                instData->numberOfUnregister = 0;
                /* Do not de-register service record. */
                linkPtr->scoConnectAcceptPending = TRUE;

                CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                          linkPtr->hfConnId,
                                          linkPtr->audioSetupParams.theAudioQuality,
                                          linkPtr->audioSetupParams.theTxBandwidth,
                                          linkPtr->audioSetupParams.theRxBandwidth,
                                          linkPtr->audioSetupParams.theMaxLatency,
                                          linkPtr->audioSetupParams.theVoiceSettings,
                                          linkPtr->audioSetupParams.theReTxEffort);
                CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                linkPtr->state = Connected_s;
            }
            /* Now make sure not to accept more connections if the maximum allowed is reached */
            linkPtr->accepting = FALSE;
            CsrBtHfCancelAcceptCheck(instData);
        }
        else
        {/* connect to peer failed during par neg or other. Activate the service again  to let others connect */
            if (prim->resultCode != CSR_BT_RESULT_CODE_CM_ALREADY_CONNECTING)
            {
               CsrBtHfAllowConnectCheck(instData);
            }
           /* If already connecting, just wait for the incoming connection....*/
        }
    }
    else
    {/* This must be due to a cross between cancel_accept_connect_req and incoming connection.
        Just make sure to remove the link.... */
        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
            prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {/* Disconnect */
            CsrBtCmDisconnectReqSend(prim->btConnId);
        }
    }
}


void CsrBtHfServiceSearchStateCmCancelAcceptCfmHandler(HfMainInstanceData_t * instData)
{
    if (instData->linkData[instData->index].linkType == CSR_BT_HF_CONNECTION_HF)
    {
        CsrBtCmCancelAcceptConnectCfm * prim;
        prim = (CsrBtCmCancelAcceptConnectCfm *) instData->recvMsgP;

        if((prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS  || prim->resultCode == CSR_BT_RESULT_CODE_CM_NOTHING_TO_CANCEL)&&
            prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            instData->linkData[instData->index].state = Connect_s;
        }
        /* wait for CmConnectAcceptCfm */
    }
}



/*************************************************************************************
    Link state has changed. Record new link state.
************************************************************************************/
void CsrBtHfServiceSearchStateCmModeChangeIndHandler(HfMainInstanceData_t *instData)
{
    CsrBtCmRfcModeChangeInd *prim;
    CsrUint8 old = instData->linkData[instData->index].lpMode;

    prim = (CsrBtCmRfcModeChangeInd *) instData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->linkData[instData->index].linkState = prim->mode;
        instData->linkData[instData->index].lpMode = prim->mode;
        switch(prim->mode)
        {
            case CSR_BT_LINK_STATUS_CONNECTED:
                CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_CONNECTED, old, prim->resultCode, prim->resultSupplier);
                break;
            case CSR_BT_LINK_STATUS_HOLD:
                CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_HOLD, old, prim->resultCode, prim->resultSupplier);
                break;
            case CSR_BT_LINK_STATUS_SNIFF:
                CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_SNIFF, old, prim->resultCode, prim->resultSupplier);
                break;
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
            case CSR_BT_LINK_STATUS_PARK:
                CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_PARK, old, prim->resultCode, prim->resultSupplier);
                break;
#endif
            default:
                break;
        }
    }
    instData->linkData[instData->index].pendingModeChange = FALSE;
}

void CsrBtHfConnectStateCmCancelAcceptConnectCfmHandler(HfMainInstanceData_t *instData)
{

    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HS)
    {
        CsrBtCmCancelAcceptConnectCfm * prim;
        prim = (CsrBtCmCancelAcceptConnectCfm *) instData->recvMsgP;
        if (linkPtr->state == Connect_s)
        {
            if((prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS  || prim->resultCode == CSR_BT_RESULT_CODE_CM_NOTHING_TO_CANCEL)&&
                prim->resultSupplier == CSR_BT_SUPPLIER_CM)
            {
                if (!linkPtr->disconnectReqReceived)
                {
                    linkPtr->oldState = Connect_s;
                    instData->numberOfUnregister = 0;
                }
                else
                {
                    CsrBtHfInitInstanceData(instData);
                    CsrBtHfAllowConnectCheck(instData);
                }
            }
            /* else wait for CSR_BT_CM_CONNECT_ACCEPT_CFM */
        }
    }
    /* else do nothing... Just ignore */
}

void CsrBtHfXStateCmConnectAcceptCfmHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
    {
        CsrBtCmConnectAcceptCfm * prim;
        prim = (CsrBtCmConnectAcceptCfm *) instData->recvMsgP;

        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
            prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            CsrBtCmDisconnectReqSend(prim->btConnId);
        }
    }
}

/*************************************************************************************
    Result of mode change request is received.
************************************************************************************/
void CsrBtHfConnectedStateCmModeChangeIndHandler(HfMainInstanceData_t *instData)
{
    CsrBtCmRfcModeChangeInd *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    CsrUint8 old = linkPtr->lpMode;

    prim = (CsrBtCmRfcModeChangeInd *) instData->recvMsgP;
    /* success so just store the actual link state which may be different from the wanted
       but CM will ensure that link state is changed later */
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        linkPtr->linkState = linkPtr->lpMode = prim->mode;
        /* remote side can send mode change at any time so we must check the actual mode
           before entering lpenabled state */
        if ( (prim->mode == CSR_BT_SNIFF_MODE) 
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
            || (prim->mode == CSR_BT_PARK_MODE) 
#endif
            )
        {
            /* change state and stop timer (if running) now that we change to LP mode */
            linkPtr->state = LpEnabled_s;
        }
        switch(prim->mode)
        {
            case CSR_BT_LINK_STATUS_CONNECTED:
                CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_CONNECTED, old, prim->resultCode, prim->resultSupplier);
                break;
            case CSR_BT_LINK_STATUS_HOLD:
                CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_HOLD, old, prim->resultCode, prim->resultSupplier);
                break;
            case CSR_BT_LINK_STATUS_SNIFF:
                CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_SNIFF, old, prim->resultCode, prim->resultSupplier);
                break;
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
            case CSR_BT_LINK_STATUS_PARK:
                CsrBtHfSendHfStatusInd(instData, CSR_BT_LINK_STATUS_PARK, old, prim->resultCode, prim->resultSupplier);
                break;
#endif
            default:

                break;
        }
    }
    instData->linkData[instData->index].pendingModeChange = FALSE;
}

/*************************************************************************************
    Handle the Disconnect req in LpEnabled or connected state, send disconnect.
************************************************************************************/
void CsrBtHfXStateHfDisconnectReqHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    linkPtr->localDisconnect = TRUE;
    linkPtr->disconnectReqReceived = TRUE;

    if (linkPtr->linkState == CSR_BT_LINK_STATUS_DISCONNECTED)
    {
        CsrBtHfSendHfDisconnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
        CsrBtHfSaveQueueCleanUp(instData);
        linkPtr->state = Activate_s;
        if (linkPtr->sdsRegistered)
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
            CsrBtHfSendSdsRegisterReq(instData);
        }
    }
    else
    {
        if (linkPtr->scoHandle != HF_SCO_UNUSED)
        {
            CsrBtCmScoDisconnectReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
#ifdef CSR_USE_DSPM                   
            if (instData->hfDspmOpPending == DSPM_NOP)
            {
                CsrBtHfDspmSendDisconnectStreams(instData);
                instData->hfDspmOpPending = instData->index;
            }
#endif
        }
        CsrBtCmDisconnectReqSend(linkPtr->hfConnId);
    }
}

/*************************************************************************************
    Inform higher layer and clean up.
************************************************************************************/
void CsrBtHfXStateCmDisconnectIndHandler(HfMainInstanceData_t *instData)
{
    CsrBtCmDisconnectInd *prim;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim = (CsrBtCmDisconnectInd *) instData->recvMsgP;

#ifdef CSR_USE_DSPM    
    if (instData->hfDspmOpPending == DSPM_NOP)
    {
        CsrBtHfDspmSendDisconnectStreams(instData);
        instData->hfDspmOpPending = instData->index;
    }
    /* In any case, make sure that the operators are removed when the audio streams are disconnected */
    linkPtr->dspmInst.cleanUp = TRUE;
#endif
    if (linkPtr->state != Activate_s)
    {
        if (!prim->status)
        {
            CsrBtCmDisconnectReqSend(prim->btConnId);
        }
        else if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
        {
            if (linkPtr->state != ServiceSearch_s)
            {
                linkPtr->pcmMappingReceived = FALSE;
                if (linkPtr->disconnectReqReceived == TRUE)
                {
                    CsrBtHfSaveQueueCleanUp(instData);

                    if (linkPtr->sdsRegistered)
                    {
                        CsrBtHfSendHfDisconnectInd(instData, prim->reasonCode, prim->reasonSupplier);
                        CsrBtHfInitInstanceData(instData);
                        linkPtr->disconnectReqReceived = FALSE;
                    }
                    else
                    {
                        linkPtr->state = Activate_s;
                        CsrBtHfSendSdsRegisterReq(instData);
                        if (instData->reActivating > 0)
                        {/* Already registrating records: make sure to register as many as we have to... */
                            instData->reActivating++;
                        }
                        linkPtr->disconnectResult = CSR_BT_RESULT_CODE_CM_SUCCESS;
                        linkPtr->disconnectSupplier = CSR_BT_SUPPLIER_CM;
                        linkPtr->disconnectLocalTerminated = TRUE;
                    }
                }
                else if ((linkPtr->state == Connected_s) || (linkPtr->state == LpEnabled_s))
                {   /* no cancel received so disconnect must be result of lp fail */
                    CsrBtHfSaveQueueCleanUp(instData);
                    if (linkPtr->sdsRegistered)
                    {
                        if (linkPtr->atSequenceState >= serviceLevel)
                        {
                            CsrBtHfSendHfDisconnectInd(instData, prim->reasonCode, prim->reasonSupplier);
                        }
                        CsrBtHfInitInstanceData(instData);
                    }
                    else
                    {
                        linkPtr->disconnectResult = prim->reasonCode;
                        linkPtr->disconnectSupplier = prim->reasonSupplier;
                        linkPtr->disconnectLocalTerminated = prim->localTerminated;
                        linkPtr->state = Activate_s;
                        CsrBtHfSendSdsRegisterReq(instData);
                        if (instData->reActivating > 0)
                        {/* Already registrating records: make sure to register as many as we have to... */
                            instData->reActivating++;
                        }
                    }
                }
                linkPtr->audioPending = FALSE;
                linkPtr->linkState = CSR_BT_LINK_STATUS_DISCONNECTED;
            }
            else /* Service search state */
            {
                if (linkPtr->disconnectReqReceived)
                {
                    /* a cancel has been received so send a cancel cfm to app layer */
                    CsrBtHfSendHfDisconnectInd(instData, prim->reasonCode, prim->reasonSupplier);
                    CsrBtHfInitInstanceData(instData);
                    linkPtr->state = Activate_s;
                }
                else /* check the state of the SLC establishment*/
                {
                    if (linkPtr->serviceState == sdcState_s)
                    {
                        /* this means that the other device disconnected during SDC search*/
                        linkPtr->disconnectPeerReceived = TRUE;
                    }
                    else if (linkPtr->serviceState == btConnect_s)
                    {
                        /* this is when connection has been established, but before serviceConnectInd has been sent to app */
                        if (linkPtr->oldState == Connect_s)
                        {
                            /* app tried to connect. inform that connect failed */
                            if (linkPtr->pendingCancel)
                            {
                                CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_CANCELLED_CONNECT_ATTEMPT, CSR_BT_SUPPLIER_HF);
                            }
                            else
                            {
                                CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_CONNECT_ATTEMPT_FAILED, CSR_BT_SUPPLIER_HF);
                            }
                        }
                        CsrBtHfInitInstanceData(instData);
                        /* Remember to register the service record if needed */
                        if (linkPtr->sdsRegistered == FALSE)
                        {
                            instData->linkRegistering = CSR_BT_HF_CONNECTION_UNKNOWN;
                            CsrBtHfSendSdsRegisterReq(instData);
                            if (instData->reActivating > 0)
                            {/* Already registrating records: make sure to register as many as we have to... */
                                instData->reActivating++;
                            }
                        }
                        linkPtr->state = Activate_s;
                    }
                    else
                    {
                        /* HfServiceConnectInd has been sent to app */
                        CsrBtHfSendHfDisconnectInd(instData, prim->reasonCode, prim->reasonSupplier);
                        CsrBtHfInitInstanceData(instData);
                        linkPtr->state = Activate_s;
                    }
                }
                linkPtr->pcmMappingReceived = FALSE;
                linkPtr->linkState = CSR_BT_LINK_STATUS_DISCONNECTED;
            }
        }
        else
        {/* HSP */
            if (linkPtr->disconnectReqReceived)
            {
                linkPtr->disconnectResult = CSR_BT_RESULT_CODE_HF_SUCCESS;
                linkPtr->disconnectSupplier = CSR_BT_SUPPLIER_HF;
                linkPtr->state = Activate_s;
                linkPtr->pcmMappingReceived = FALSE;
                /* App requested Disconnect */
                if (linkPtr->sdsRegistered)
                {
                    CsrBtHfSendHfDisconnectInd(instData, linkPtr->disconnectResult, linkPtr->disconnectSupplier);
                    linkPtr->disconnectReqReceived = FALSE;
                }
                else
                {
                    HsSendSdsRegisterReq(instData);
                    if (instData->reActivating > 0)
                    {/* Already registrating records: make sure to register as many as we have to... */
                        instData->reActivating++;
                    }
                }
            }
            else
            {
                /* this means other side disconnected or abnormal disconnect. send result to app */
                CsrBtHfSaveQueueCleanUp(instData);
                /* save result for DISCONNECT_IND */
                linkPtr->disconnectResult = prim->reasonCode;
                linkPtr->disconnectSupplier = prim->reasonSupplier;
                linkPtr->disconnectLocalTerminated = prim->localTerminated;
                linkPtr->pcmMappingReceived = FALSE;
                if (linkPtr->sdsRegistered)
                {
                    CsrBtHfSendHfDisconnectInd(instData,linkPtr->disconnectResult, linkPtr->disconnectSupplier);
                    linkPtr->disconnectReqReceived = FALSE;
                    CsrBtHfInitInstanceData(instData);
                }
                else
                {
                    linkPtr->oldState = linkPtr->state;
                    linkPtr->state = Activate_s;
                    HsSendSdsRegisterReq(instData);
                    if (instData->reActivating > 0)
                    {/* Already registrating records: make sure to register as many as we have to... */
                        instData->reActivating++;
                    }
                }
            }
        }
    }
}


/*************************************************************************************
    function called in connect and service search states to set the cancel req
    received flag. When the flag is set the confirm message on either connect or
    sdp search close will check the flag and disconnect.
************************************************************************************/
void CsrBtHfXStateHfCancelReqHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if ((!linkPtr->pendingCancel) && (linkPtr->searchOngoing))
    {
        if (linkPtr->searchAndCon)
        {
            linkPtr->pendingCancel = TRUE;
            CsrBtUtilRfcConCancel((void*)(instData),instData->sdpHfSearchConData);
        }
        else
        {
            CsrBtUtilSdcSearchCancel((void*)(instData),instData->sdpHfSearchData);
        }
    }
    else
    {
        if (!linkPtr->searchOngoing)
        {
            linkPtr->pendingCancel = TRUE;
            CsrBtCmDisconnectReqSend(linkPtr->hfConnId);
        }
    }
    if (*((CsrBtHfPrim *)(instData->recvMsgP)) == CSR_BT_HF_DISCONNECT_REQ)
    {
        linkPtr->disconnectReqReceived = TRUE;
    }
    else
    {/* HF_CANCEL_REQ */
        linkPtr->pendingCancel = TRUE;
    }
}

