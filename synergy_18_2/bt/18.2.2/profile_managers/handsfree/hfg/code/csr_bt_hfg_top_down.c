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
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_proto.h"


/* Handle Deactivate request received in IDLE state */
void CsrBtHfgMainIdleDeactivateReqHandler(HfgMainInstance_t *inst)
{

    HFG_CHANGE_STATE(inst->state, MainDeactivate_s);
    if(!CsrBtHfgConnectionGoIdle(inst))
    {
        CsrBtHfgMainSendHfgDeactivateCfm(inst);
        CsrBtHfgResetProfile(inst);
        HFG_CHANGE_STATE(inst->state, MainNull_s);
    }
}

/* Cancel pending connection from save-queue */
void CsrBtHfgMainNullIdleHfgCancelConnectReqHandler(HfgMainInstance_t *inst)
{
    CsrBtHfgCancelConnectReq *prim;
    CsrMessageQueueType *localSaveQueue;
    void *msg;
    CsrUint16 event;

    localSaveQueue = NULL;
    prim = (CsrBtHfgCancelConnectReq *)inst->msg;

    /* Check SaveMessages queue to see if there is any
     * CSR_BT_HFG_SERVICE_CONNECT_REQs and move them from global to temporary
     * savequeue, but take out the connection requests */
    while(CsrMessageQueuePop(&inst->saveQueue, &event, &msg))
    {
        CsrBool found;
        CsrBtHfgPrim *primType;
        primType = (CsrBtHfgPrim*)msg;

        found = FALSE;
        if(*primType == CSR_BT_HFG_SERVICE_CONNECT_REQ)
        {
            CsrBtHfgServiceConnectReq *connectPrim;
            connectPrim = (CsrBtHfgServiceConnectReq*)msg;

            if(bd_addr_eq(&prim->deviceAddr, &connectPrim->deviceAddr) &&
               (inst->appHandle != CSR_SCHED_QID_INVALID))
            {
                /* Found the connect request to cancel. Send
                 * confirmation to app. We have to construct the
                 * message ourselves here as no linkData currently
                 * exists for it */
                CsrBtHfgServiceConnectInd *newPrim;
                newPrim = (CsrBtHfgServiceConnectInd*)CsrPmemAlloc(sizeof(CsrBtHfgServiceConnectInd));

                newPrim->type = CSR_BT_HFG_SERVICE_CONNECT_IND;
                newPrim->connectionId = CSR_BT_HFG_CONID_NONE;
                newPrim->connectionType = CSR_BT_HFG_CONNECTION_UNKNOWN;
                newPrim->serviceName = NULL;
                bd_addr_zero(&newPrim->deviceAddr);
                newPrim->supportedFeatures = 0;
                newPrim->remoteVersion = 0;
                newPrim->resultCode = CSR_BT_RESULT_CODE_HFG_CANCELLED_CONNECT_ATTEMPT;
                newPrim->resultSupplier = CSR_BT_SUPPLIER_HFG;
                newPrim->btConnId = CSR_BT_CONN_ID_INVALID;

                found = TRUE;
                CsrBtHfgMessagePut(inst->appHandle, newPrim);
                CsrPmemFree(msg);
            }
        }
        if(!found)
        {
            CsrMessageQueuePush(&localSaveQueue, event, msg);
        }
    }

    /* Move from temporary queue back to global save-queue */
    while(CsrMessageQueuePop(&localSaveQueue, &event, &msg))
    {
        CsrMessageQueuePush(&inst->saveQueue, event, msg);
    }
}

void CsrBtHfgMainIndicatorSetupReqHandler(HfgMainInstance_t *inst)
{
    CsrBtHfgStatusIndicatorSetReq *prim;
    CsrUintFast8 i = 0;
    prim = (CsrBtHfgStatusIndicatorSetReq*)inst->msg;

    /* Update */
    if((prim->indicator >= CSR_BT_SERVICE_INDICATOR) &&
       (prim->indicator < CSR_BT_CIEV_NUMBER_OF_INDICATORS))
    {
        /* This means the indicators changed before any connections were made
           just update the main indicators information  */
        inst->ind.ciev[prim->indicator] = prim->value;
    }
    else
    {
        if (prim->indicator == CSR_BT_GATHERED_CALL_INDICATORS)
        {
            switch(prim->value)
            {
                case CSR_BT_NO_CALL_ACTIVE_NO_CALL_SETUP:
                    {
                        inst->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR] = CSR_BT_NO_CALL_ACTIVE_VALUE;
                        inst->ind.ciev[CSR_BT_CALL_SETUP_STATUS_INDICATOR] = CSR_BT_NO_CALL_SETUP_VALUE;
                        break;
                    }

                case CSR_BT_CALL_ACTIVE_NO_CALL_SETUP:
                    {
                        inst->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR] = CSR_BT_CALL_ACTIVE_VALUE;
                        inst->ind.ciev[CSR_BT_CALL_SETUP_STATUS_INDICATOR] = CSR_BT_NO_CALL_SETUP_VALUE;
                        break;
                    }

                case CSR_BT_CALL_ACTIVE_CALL_SETUP:
                    {
                        inst->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR] = CSR_BT_CALL_ACTIVE_VALUE;
                        inst->ind.ciev[CSR_BT_CALL_SETUP_STATUS_INDICATOR] = CSR_BT_INCOMING_CALL_SETUP_VALUE;
                        break;
                    }
            }

        }
    }

    /* Now update all link instances */
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        CsrMemCpy(inst->linkData[i].ind.ciev , inst->ind.ciev, CSR_BT_CIEV_NUMBER_OF_INDICATORS);
    }
}

/* Setup incoming security level */
void CsrBtHfgMainXSecurityInReqHandler(HfgMainInstance_t *inst)
{
    CsrBtResultCode rval;
    CsrBtHfgSecurityInReq *prim;

    prim = (CsrBtHfgSecurityInReq*)inst->msg;

    rval = CsrBtScSetSecInLevel(&inst->secIncoming, prim->secLevel,
        CSR_BT_HANDSFREE_GW_MANDATORY_SECURITY_INCOMING,
        CSR_BT_HANDSFREE_GW_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_HFG_SUCCESS,
        CSR_BT_RESULT_CODE_HFG_UNACCEPTABLE_PARAMETER);

    CsrBtHfgSendCmSecurityInCfm(prim->appHandle,
         rval, CSR_BT_SUPPLIER_HFG);
}

/* Setup outgoing security level */
void CsrBtHfgMainXSecurityOutReqHandler(HfgMainInstance_t *inst)
{
    CsrBtResultCode rval;
    CsrBtHfgSecurityOutReq *prim;
    prim = (CsrBtHfgSecurityOutReq*)inst->msg;

    rval = CsrBtScSetSecOutLevel(&inst->secOutgoing, prim->secLevel,
        CSR_BT_HANDSFREE_GW_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_HANDSFREE_GW_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_HFG_SUCCESS,
        CSR_BT_RESULT_CODE_HFG_UNACCEPTABLE_PARAMETER);

    CsrBtHfgSendCmSecurityOutCfm(prim->appHandle,
        rval, CSR_BT_SUPPLIER_HFG);
}

/* Activate profile */
void CsrBtHfgMainNullHfgActivateReqHandler(HfgMainInstance_t * inst)
{
    CsrBtHfgActivateReq *prim;
    CsrUintFast8 i;

    if(FALSE == inst->initSequenceDone)
    {
        CsrBtHfgMainSaveMessage(inst);
        return;
    }

    prim = (CsrBtHfgActivateReq*)inst->msg;

    HFG_CHANGE_STATE(inst->state, MainIdle_s);
    inst->appHandle = prim->phandle;
    inst->atParser = prim->atMode;
    inst->locSupFeatures = prim->supportedFeatures;
    inst->hfgConfig = prim->hfgConfig;
    inst->callConfig = prim->callConfig;
    if (inst->locSupFeatures & CSR_BT_HFG_SUPPORT_INBAND_RINGING)
    {
        inst->ind.other[CSR_BT_HFG_INBAND_RINGING] = 1;
    }
    else
    {
        inst->ind.other[CSR_BT_HFG_INBAND_RINGING] = 0;
    }
    inst->maxConnections = prim->numConnections;
    if(inst->maxConnections > CSR_BT_HFG_NUM_SERVERS)
    {
        inst->maxConnections = CSR_BT_HFG_NUM_SERVERS;
    }
    else if(inst->maxConnections < 1)
    {
        inst->maxConnections = 1;
    }

    CsrMemSet(inst->localName, 0, CSR_BT_MAX_FRIENDLY_NAME_LEN+1);
    if(prim->serviceName)
    {
        CsrStrNCpy((char*)(inst->localName),
                (char*)prim->serviceName,
                CSR_BT_MAX_FRIENDLY_NAME_LEN+1);
    }
    else
    {
        CsrStrNCpy((char*)(inst->localName),
                CSR_BT_HFG_SERVICE_NAME,
                CSR_BT_MAX_FRIENDLY_NAME_LEN+1);
    }
    CsrUtf8StrTruncate(inst->localName, CSR_BT_MAX_FRIENDLY_NAME_LEN);

    /* Re-initialize connection instances */
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        CsrBtHfgInitializeConnInstance(inst, (CsrUint8)i);
        HFG_CHANGE_STATE(inst->linkData[i].state, Activate_s);
    }

    /* Start SDS record setup */
    CsrBtHfgRecordUpdate(inst);
}

/* Deactivate profile */
void CsrBtHfgMainActiveHfgDeactivateReqHandler(HfgMainInstance_t *inst)
{
    /* Begin transition to null state */
    HFG_CHANGE_STATE(inst->state, MainDeactivate_s);
    if(!CsrBtHfgConnectionGoIdle(inst))
    {
        /* Ok, profile is in idle state already */
        CsrBtHfgMainSendHfgDeactivateCfm(inst);
        CsrBtHfgResetProfile(inst);
        HFG_CHANGE_STATE(inst->state, MainNull_s);
    }
}

/* Outgoing connect to headset */
void CsrBtHfgMainActiveHfgServiceConnectReqHandler(HfgMainInstance_t *inst)
{
    CsrBtHfgServiceConnectReq *prim;
    HfgInstance_t *linkData;
    CsrUintFast8 i, num;
    CsrUint8 index;

    prim = (CsrBtHfgServiceConnectReq*)inst->msg;

    num = 0;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if(inst->linkData[i].state == Connected_s)
        {
            num++;
        }
    }

    if (num >= inst->maxConnections)
    {/* Max number of connections allowed already achieved...reject this! */
        linkData = CsrBtHfgFindLinkAvailable(inst);
        if (linkData)
        {
            CsrBtHfgSendHfgServiceConnectInd(linkData, CSR_BT_RESULT_CODE_HFG_MAX_NUM_OF_CONNECTIONS, CSR_BT_SUPPLIER_HFG);
        }
        return;
    }
    /* If outgoing connection already pending, store this on the
     * queue and bail out */
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if((inst->linkData[i].state == Connect_s) ||
           (inst->linkData[i].state == ServiceSearch_s))
        {
            CsrBtHfgMainSaveMessage(inst);
            return;
        }
    }

    /* Check that we don't already have a connection to the specified
     * address */
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if((inst->linkData[i].address.nap == prim->deviceAddr.nap) &&
           (inst->linkData[i].address.uap == prim->deviceAddr.uap) &&
           (inst->linkData[i].address.lap == prim->deviceAddr.lap))
        {
            /* Ignore connect request from app and bail out */
            return;
        }
    }

    linkData = CsrBtHfgFindLinkIndexAvailable(inst,&index);
    if(linkData != NULL)
    {
        CsrBtHfgInitializeConnInstance(inst, index);
        linkData->serverChannel = CsrBtHfgFindFreeServerChannel(inst);
        if(linkData->serverChannel == CSR_BT_NO_SERVER)
        {
            return;
        }

        inst->activeConnection = linkData->index;

        linkData->linkType = prim->connectionType;
        linkData->address.nap = prim->deviceAddr.nap;
        linkData->address.uap = prim->deviceAddr.uap;
        linkData->address.lap = prim->deviceAddr.lap;
        HFG_CHANGE_STATE(linkData->state, Connect_s);
        HFG_CHANGE_STATE(linkData->oldState, Activate_s);

        /* Start incoming connection update (cancel accept connect) */
        if(CsrBtHfgConnectionUpdate(inst))
        {
            /* Request sent, bail out now */
            return;
        }
        else
        {
            /* No conflict between SDS records and incoming
             * connections found (typically because of crossing
             * signal), so just continue the setup by starting the SDC
             * search */
            CsrBtHfgStartSdcSearch(linkData);
        }
    }
}

/* Set HFG specific sniff settings */
void CsrBtHfgMainXHfgConfigSniffReqHandler(HfgMainInstance_t *inst)
{
    CsrBtHfgConfigSniffReq *prim;
    CsrBool changed;

    prim = (CsrBtHfgConfigSniffReq*)inst->msg;
    changed = FALSE;

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
        CsrUintFast8 i;
        for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
        {
            if(inst->linkData[i].state == Connected_s)
            {
                CsrBtHfgLpStart(&(inst->linkData[i]), TRUE);
            }
        }
    }
}

/* Environment message in idle or active state */
void CsrBtHfgMainIdleActiveEnvCleanupHandler(HfgMainInstance_t *inst)
{
    CsrCleanupInd *prim;
    prim = (CsrCleanupInd*)inst->msg;

    /* This cleanup is for a appHandle from this profile */
    if(prim->phandle == inst->appHandle)
    {
        /* Handle in exact same way as if we received a deactivate
         * request from the user application, but make sure that the
         * app-handle is invalid so we don't send any more messages to
         * the user (who's not there anymore) */
        inst->appHandle = CSR_SCHED_QID_INVALID;
        CsrBtHfgMainActiveHfgDeactivateReqHandler(inst);
    }
    else
    {
        /* Message was not for our application, so simply ignore it */
        ;
    }
}

#ifdef CSR_BT_INSTALL_HFG_C2C
void CsrBtHfgMainLocalAdpcmSupportReqHandler(HfgMainInstance_t *inst)
{
    if (inst->adpcmLocalSupported)
    { /* We already know; just answer */
        CsrBtHfgSendHfgLocalAdpcmSupportInd(inst,inst->adpcmLocalSupported);
    }
    else
    { /* Is this flag FALSE because we have not asked yet or not received an answer?
         Just in case, ask about it...*/
        inst->pendingAdcpmQuery = TRUE;
        CsrBtHfgMainGetC2CAdpcmSupported(inst->bccmdSeqNo);
    }
}
#endif

void CsrBtHfgMainXSetDeregisterTimeReqHandler(HfgMainInstance_t *inst)
{
    CsrBtHfgDeregisterTimeReq *prim = (CsrBtHfgDeregisterTimeReq*)inst->msg;

    inst->deRegisterTime = prim->waitSeconds;

    CsrBtHfgSendHfgDeregisterTimeCfm(inst->appHandle, 0 /*CSR_BT_HFG_AT_CMD_SUCCESS*/);
}

