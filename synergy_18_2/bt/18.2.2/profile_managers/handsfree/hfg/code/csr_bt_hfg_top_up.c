/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bccmd_prim.h"
#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_proto.h"


/* Port parameter indication on rfcomm */
void CsrBtHfgMainXCmPortnegIndHandler(HfgMainInstance_t *inst)
{
    CsrBtCmPortnegInd *prim;
    prim = (CsrBtCmPortnegInd *)inst->msg;

    if(prim->request == TRUE)
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

        CsrBtCmPortnegResSend(prim->btConnId,
                         &portPar);
    }
    else
    {
        CsrBtCmPortnegResSend(prim->btConnId,
                         &(prim->portPar));
    }
}


/* Profile (server channel) registered at the CM */
void CsrBtHfgMainNullCmRegisterCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmRegisterCfm * prim;
    prim = (CsrBtCmRegisterCfm*)inst->msg;

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrUintFast8 i;
        for(i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
        {
            if(inst->service[i].chan == CSR_BT_NO_SERVER)
            {
                /* Save server channel in array */
                inst->service[i].chan = prim->serverChannel;

                /* Make sure we need the *next* channel before
                 * requesting it... */
                if((i+1) < CSR_BT_HFG_NUM_CHANNELS)
                {
                    if(inst->service[i].type != inst->service[i+1].type)
                    {
                        /* Tell the SD that it must look for the
                           CSR_BT_HS_PROFILE_UUID service, when it perform
                           a SD_READ_AVAILABLE_SERVICE_REQ          */
                        CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_HS_PROFILE_UUID);

                        /* Next serverchannel differs form this one,
                         * so register channel with a specific
                         * service */
                        CsrBtCmRegisterReqSend(CSR_BT_HFG_IFACEQUEUE);
                    }
                    else
                    {
                        /* Extra service, no specific service */
                        CsrBtCmRegisterReqSend(CSR_BT_HFG_IFACEQUEUE);
                    }
                }
                else
                {
                    /* All server channels registered - change "pseudo-state" */
                    inst->initSequenceDone = TRUE;
                    CsrBtHfgMainSendHfgHouseCleaning(inst);
                }
                break;
            }
        }
    }
}


/* Service record has been registered. This must have been started before a
 * deactivation - clean it up again */
void CsrBtHfgMainNullCmSdsRegisterCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmSdsRegisterCfm *prim;
    prim = (CsrBtCmSdsRegisterCfm*)inst->msg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        /* update information */
        CsrBtCmSdsUnRegisterReqSend(CSR_BT_HFG_IFACEQUEUE,prim->serviceRecHandle);
        return;
    }
    CsrBtHfgMainSendHfgHouseCleaning(inst);
}


/* Service record has been registered. This means that user has
 * activated us, so continue the SDS register, or trigger transition
 * and start accepting connections */
void CsrBtHfgMainIdleCmSdsRegisterCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmSdsRegisterCfm *prim;
    prim = (CsrBtCmSdsRegisterCfm*)inst->msg;

    /* Update state information if success */
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtHfgRecordRegisterConfirm(inst,
                                 prim->serviceRecHandle);
    }

    /* Continue SDS setup */
    if(CsrBtHfgRecordUpdate(inst))
    {
        /* ...yes and request sent, bail out now */
        return;
    }

    /* SDS completed. Enter active state and start the connection
     * accept run */
    HFG_CHANGE_STATE(inst->oldState, inst->state);
    HFG_CHANGE_STATE(inst->state, MainActive_s);

    CsrBtHfgMainSendHfgHouseCleaning(inst);
    if (CsrBtHfgConnectionUpdate(inst))
    {/* Useless check implemented to avoid 'coverity' issues... */
        return;
    }
}

/* Cancel accept confirm */
void CsrBtHfgMainActiveCmCancelAcceptConnectCfmHandler(HfgMainInstance_t *inst)
{
    CsrBool cancelFlag;
    CsrBtCmCancelAcceptConnectCfm *prim;
    HfgInstance_t *linkData;
    prim = (CsrBtCmCancelAcceptConnectCfm*)inst->msg;

    /* Update state */
    if((prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS ||prim->resultCode == CSR_BT_RESULT_CODE_CM_NOTHING_TO_CANCEL) && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        cancelFlag = TRUE;
        CsrBtHfgConnectionConfirm(inst,
                             prim->serverChannel);
    }
    else
    {
        cancelFlag = FALSE;
    }

    /* Continue updating the accept connection level? */
    if(CsrBtHfgConnectionUpdate(inst))
    {
        /* Yes, so bail out now */
        return;
    }

    if((inst->activeConnection != CSR_BT_HFG_NO_CONID) && cancelFlag)
    {
        /* We don't have any incoming connections at this point, so now it
         * is time to check what we should do about the established
         * connection (if such an active connection exists) */
        linkData = &(inst->linkData[inst->activeConnection]);
        if (linkData)
        {
            if (!linkData->pendingSearch)
            {
                CsrBtHfgConnectionStart(linkData);
            }
        }
    }
    else if(cancelFlag && (CsrBtHfgFindLinkWithServerChan(inst, prim->serverChannel) != NULL))
    {
        /* We did not find any connection active for setup, so pass
         * the cancel to the connection handler if such one exists
         * Note that we might not find a link-instance as we cancel
         * connections when the maximum has been reached (for
         * example) */
        CsrBtHfgMainXSecondCm(inst);
    }
    else
    {
        /* Don't do anything. We've stored the cancel-status above, so
         * everything is find and dandy at this point */
        ;
    }

}

/* Connect accept confirm */
void CsrBtHfgMainActiveCmConnectAcceptCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmConnectAcceptCfm *prim;
    HfgInstance_t *linkData;
    CsrUintFast8 i;
    CsrUint8 index;

    prim = (CsrBtCmConnectAcceptCfm*)inst->msg;

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
       prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, prim->deviceAddr,
                    prim->btConnId);
        /* Update state */
        CsrBtHfgConnectionConfirm(inst,
                             prim->serverChannel);

        /* Re-use instance from server channel. This will only happen
         * if a crossing outgoing connection was made on the same server channel */
        linkData = CsrBtHfgFindLinkIndexWithServerChan(inst, prim->serverChannel, &index);
        
        if(!linkData)
        {
            /* Also if an outgoing connection attempt is ongoing to the same 
               device, on a different server channel this should be cancelled */
            for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
            {
                if (( CSR_BT_BD_ADDR_EQ( inst->linkData[i].address, prim->deviceAddr ) ) &&
                        (inst->linkData[i].oldState == Connect_s) && (inst->linkData[i].state == ServiceSearch_s))
                {
                    /* Proceed with cancel connect depending on link state */
                    if(inst->linkData[i].pendingSearch)
                    {
                        if (inst->linkData[i].searchAndCon)
                        {
                            CsrBtUtilRfcConCancel((void*)&(inst->linkData[i]),inst->sdpHfgSearchConData);
                            inst->linkData[i].pendingCancel = TRUE;
                        }
                        else
                        {
                            CsrBtUtilSdcSearchCancel((void*)&(inst->linkData[i]),inst->sdpHfgSearchData);
                            CsrBtHfgLinkConnectFailed(&(inst->linkData[i]));
                        }
                    }
                }
                else if (( CSR_BT_BD_ADDR_EQ( inst->linkData[i].address, prim->deviceAddr ) ) &&
                        (inst->linkData[i].state == Connect_s) )
                {
                    CsrBtHfgInitializeConnInstance(inst,(CsrUint8) i);
                }
            }
            /* Server channel not known, so find empty instance */
            linkData = CsrBtHfgFindLinkIndexAvailable(inst, &index);
        }

        if(linkData != NULL)
        {   
            /* Ok, connection has been established, so setup our
             * instance and either start SDC or take down the SDS,
             * which will then start SDC */
            if (((linkData->address.nap != prim->deviceAddr.nap) ||
                 (linkData->address.uap != prim->deviceAddr.uap) ||
                 (linkData->address.lap != prim->deviceAddr.lap)) &&
                 (linkData->state == Connect_s))
            {/* incoming connection from a different device than the one
                an outgoing connection is trying to be established to: just
                inform the application that the outgoing connection is not
                possible and carry on with the incoming one */
                CsrBtHfgSendHfgServiceConnectInd(linkData, CSR_BT_RESULT_CODE_HFG_ALREADY_CONNECTING, CSR_BT_SUPPLIER_HFG);
            }
                        
            CsrBtHfgInitializeConnInstance(inst, index);

            linkData->address = prim->deviceAddr;
            linkData->hfgConnId = prim->btConnId;
            linkData->maxFrameSize = prim->profileMaxFrameSize;

            linkData->pendingSearch = TRUE;
            HFG_CHANGE_STATE(linkData->oldState, Activate_s);
            HFG_CHANGE_STATE(linkData->state, ServiceSearch_s);
            linkData->lpMode = CSR_BT_LINK_STATUS_CONNECTED;
            linkData->lpWantedMode = CSR_BT_LINK_STATUS_SNIFF;

            linkData->serverChannel = prim->serverChannel;

            /* Determine what server channel that was connected to to
             * see if it's an HFG or AG */
            linkData->linkType = CSR_BT_HFG_CONNECTION_UNKNOWN;
            for(i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
            {
                if(inst->service[i].chan == linkData->serverChannel)
                {
                    linkData->linkType = inst->service[i].type;
                    break;
                }
            }

            /* Set active connection, which will be recognised in the
             * cancel confirm handler where we continue the setup */
            inst->activeConnection = linkData->index;

            /* New incoming connection. Update the other one if
             * necessary */
            if(CsrBtHfgConnectionUpdate(inst))
            {
                /* Yes, bail out and handle the rest in cancel
                 * connect accept confirm */
                return;
            }
            else
            {
                /* Ok to continue connection */
                CsrBtHfgConnectionStart(linkData);
            }
        }
        else
        {
            /* Ignore */
        }
    }
    else
    {
        if (prim->resultCode != CSR_BT_RESULT_CODE_CM_ALREADY_CONNECTING)
        {/* Connect to peer failed during par-neg or other. Activate
          * the service again to let others connect */
            CsrBtCmConnectAcceptReqSend(CSR_BT_HFG_IFACEQUEUE,
                                   CSR_BT_HFG_CLASS_OF_DEVICE,
                                   0,
                                   CSR_BT_HFG_PROFILE_DEFAULT_MTU_SIZE,
                                   prim->serverChannel,
                                   inst->secIncoming,
                                   CSR_BT_HFG_PROFILE_UUID,
                                   inst->modemStatus,
                                   CSR_BT_DEFAULT_BREAK_SIGNAL,
                                   CSR_BT_DEFAULT_MSC_TIMEOUT);
        }
    }
}

/* Service record registered */
void CsrBtHfgMainActiveCmSdsRegisterCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmSdsRegisterCfm *prim;
    prim = (CsrBtCmSdsRegisterCfm*)inst->msg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        /* Update with state information */
        CsrBtHfgRecordRegisterConfirm(inst,
                                 prim->serviceRecHandle);

        /* Update records and connections */
        if(CsrBtHfgUpdateService(inst))
        {
            return;
        }
        CsrBtHfgMainSendHfgHouseCleaning(inst);
    }
    else
    {
        /* Try SDS register again */
        CsrBtHfgRecordUpdate(inst);
    }
}

/* Service record unregistered */
void CsrBtHfgMainActiveCmSdsUnregisterCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmSdsUnregisterCfm *prim = (CsrBtCmSdsUnregisterCfm*)inst->msg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrUintFast8 i;

        /* Update state */
        CsrBtHfgRecordUnregisterConfirm(inst,
                                   prim->serviceRecHandle);

        /* Continue record and connection update */
        if(CsrBtHfgUpdateService(inst))
        {
            /* Yes, bail out */
            return;
        }

        /* Send house cleaning to both instances */
        for (i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
        {
            CsrBtHfgSendHfgHouseCleaning(&inst->linkData[i]);
        }
    }
    else
    {
        /* Try SDS unregister again */
        if(CsrBtHfgRecordExists(inst,prim->serviceRecHandle))
        {
            CsrBtCmSdsUnRegisterReqSend(CSR_BT_HFG_IFACEQUEUE, prim->serviceRecHandle);
        }
    }
}

/* Cancel accept in deactivate */
void CsrBtHfgMainDeactivateCmCancelAcceptConnectCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmCancelAcceptConnectCfm *prim;
    prim = (CsrBtCmCancelAcceptConnectCfm*)inst->msg;

    if((prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS || prim->resultCode == CSR_BT_RESULT_CODE_CM_NOTHING_TO_CANCEL) &&
         prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        HfgInstance_t *link;

        /* Update state */
        CsrBtHfgConnectionConfirm(inst,
                             prim->serverChannel);

        /* Clear instance */
        link = CsrBtHfgFindLinkWithServerChan(inst, prim->serverChannel);
        if(link != NULL)
        {
            CsrBtHfgInitializeConnInstance(inst, link->index);
            CsrBtHfgEmptySaveQueue(link);
        }

        /* Continue taking down connect accepts? */
        if(!CsrBtHfgConnectionGoIdle(inst))
        {
            /* Done, reset everything and send deactivate confirm */
            CsrBtHfgMainSendHfgDeactivateCfm(inst);
            CsrBtHfgResetProfile(inst);
            HFG_CHANGE_STATE(inst->state, MainNull_s);
        }
    }
    else
    {
        /* Wait for CSR_BT_CM_CONNECT_ACCEPT_CFM */
        ;
    }
}

/* Connected in deactivate state */
void CsrBtHfgMainDeactivateCmConnectCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmConnectCfm *prim = (CsrBtCmConnectCfm*)inst->msg;

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
       prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,prim->deviceAddr,
                    prim->btConnId);
        /* Connection established, disconnect now and exit */
        CsrBtCmDisconnectReqSend(prim->btConnId);
        return;
    }
    /* Continue down the road to idle-land */
    else if(!CsrBtHfgConnectionGoIdle(inst))
    {
        /* Everything done */
        CsrBtHfgMainSendHfgDeactivateCfm(inst);
        CsrBtHfgResetProfile(inst);
        HFG_CHANGE_STATE(inst->state, MainNull_s);
    }
}

/* Accept connections in deactivate state */
void CsrBtHfgMainDeactivateCmConnectAcceptCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmConnectAcceptCfm *prim;
    prim = (CsrBtCmConnectAcceptCfm*)inst->msg;

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
       prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, prim->deviceAddr,
                    prim->btConnId);
        /* Connection established, disconnect now and exit */
        CsrBtCmDisconnectReqSend(prim->btConnId);
    }
    else
    {
        HfgInstance_t *link;

        /* Clear instance */
        link = CsrBtHfgFindLinkWithServerChan(inst, prim->serverChannel);
        if(link != NULL)
        {
            CsrBtHfgInitializeConnInstance(inst, link->index);
            CsrBtHfgEmptySaveQueue(link);
        }

        /* Continue down the road to idle-land */
        if(!CsrBtHfgConnectionGoIdle(inst))
        {
            /* Everything done */
            CsrBtHfgMainSendHfgDeactivateCfm(inst);
            CsrBtHfgResetProfile(inst);
            HFG_CHANGE_STATE(inst->state, MainNull_s);
        }
    }
}

/* Disconnect in deactivate state */
void CsrBtHfgMainDeactivateCmDisconnectIndHandler(HfgMainInstance_t *inst)
{
    CsrBtCmDisconnectInd *prim;
    prim = (CsrBtCmDisconnectInd*)inst->msg;

    if(prim->status)
    {
        /* Clear instance */
        HfgInstance_t *link;
        CsrUint8 index;
        link = CsrBtHfgFindLinkIndexWithConnId(inst, prim->btConnId, &index);

        if(link != NULL)
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,link->address,
                    prim->btConnId);
            CsrBtHfgInitializeConnInstance(inst, index);
            CsrBtHfgEmptySaveQueue(link);
        }

        /* Continue down the road to idle-land */
        if(!CsrBtHfgConnectionGoIdle(inst))
        {
            /* Everything done */
            CsrBtHfgMainSendHfgDeactivateCfm(inst);
            CsrBtHfgResetProfile(inst);
            HFG_CHANGE_STATE(inst->state, MainNull_s);
        }
    }
    else
    {
        /* Try disconnect again */
        CsrBtCmDisconnectReqSend(prim->btConnId);
    }
}

/* SCO connect in deactivate */
void CsrBtHfgMainDeactivateCmScoConnectCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmScoConnectCfm *prim;
    prim = (CsrBtCmScoConnectCfm*)inst->msg;

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* Disconnect SCO         */
        CsrBtCmScoDisconnectReqSend(CSR_BT_HFG_IFACEQUEUE, prim->btConnId);
    }
    else
    { /* Drop RFCOMM connection */
        CsrBtCmDisconnectReqSend(prim->btConnId);
    }
}

/* SCO disconnect in deactivate */
void CsrBtHfgMainDeactivateCmScoDisconnectIndHandler(HfgMainInstance_t *inst)
{
    CsrBtCmScoDisconnectInd *prim;
    prim = (CsrBtCmScoDisconnectInd*)inst->msg;

    /* Drop RFCOMM connection */
    CsrBtCmDisconnectReqSend(prim->btConnId);
}

/* SCO accept connection in deactivate */
void CsrBtHfgMainDeactivateCmScoAcceptConnectCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmScoAcceptConnectCfm *prim;
    prim = (CsrBtCmScoAcceptConnectCfm*)inst->msg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* Disconnect SCO         */
        CsrBtCmScoDisconnectReqSend(CSR_BT_HFG_IFACEQUEUE, prim->btConnId);
    }
    else
    { /* Drop RFCOMM connection */
        CsrBtCmDisconnectReqSend(prim->btConnId);
    }
}

/* Data in deactivate */
void CsrBtHfgMainDeactivateCmDataIndHandler(HfgMainInstance_t *inst)
{
    CsrBtCmDataInd *prim;
    prim = (CsrBtCmDataInd*)inst->msg;

    /* Drop and free it */
    CsrBtCmDataResSend(prim->btConnId);
    CsrBtCmDisconnectReqSend(prim->btConnId);
}

/* Data sent in deactivate state */
void CsrBtHfgMainDeactivateCmDataCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmDataCfm *prim;
    prim = (CsrBtCmDataCfm*)inst->msg;

    /* Drop connection */
    CsrBtCmDisconnectReqSend(prim->btConnId);
}

/* SDS registered in deactivate state */
void CsrBtHfgMainDeactivateCmSdsRegisterCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmSdsRegisterCfm *prim;
    prim = (CsrBtCmSdsRegisterCfm*)inst->msg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        /* Update state */
        CsrBtHfgRecordRegisterConfirm(inst, prim->serviceRecHandle);
    }

    /* Continue towards idle-state */
    if(!CsrBtHfgConnectionGoIdle(inst))
    {
        CsrBtHfgMainSendHfgDeactivateCfm(inst);
        CsrBtHfgResetProfile(inst);
        HFG_CHANGE_STATE(inst->state, MainNull_s);
    }
}

/* SDS unregistered in deactivate state */
void CsrBtHfgMainXCmSdsUnregisterCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmSdsUnregisterCfm *prim = (CsrBtCmSdsUnregisterCfm*)inst->msg;

    if (!(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM))
    {
        if(CsrBtHfgRecordExists(inst,prim->serviceRecHandle))
        {
            CsrBtCmSdsUnRegisterReqSend(CSR_BT_HFG_IFACEQUEUE,prim->serviceRecHandle);
        }
    }
    else
    {
        /* Successfully unregistered, update state */
        CsrBtHfgRecordUnregisterConfirm(inst, prim->serviceRecHandle);
    }
}


/* SDS unregistered in deactivate state */
void CsrBtHfgMainDeactivateCmSdsUnregisterCfmHandler(HfgMainInstance_t *inst)
{
    CsrBtCmSdsUnregisterCfm *prim = (CsrBtCmSdsUnregisterCfm*)inst->msg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        /* Update state */
        CsrBtHfgRecordUnregisterConfirm(inst, prim->serviceRecHandle);
    }

    /* Continue towards idle-state */
    if(!CsrBtHfgConnectionGoIdle(inst))
    {
        CsrBtHfgMainSendHfgDeactivateCfm(inst);
        CsrBtHfgResetProfile(inst);
        HFG_CHANGE_STATE(inst->state, MainNull_s);
    }
}

#ifdef CSR_BT_INSTALL_HFG_C2C
/* handle answer from cm about adpcm support */
void CsrBtHfgMainBccmdCfmHandler(HfgMainInstance_t *inst)
{
    CsrBccmdCfm * prim =  (CsrBccmdCfm *) inst->msg;

    if(prim->status == CSR_RESULT_SUCCESS)
    { /* The operation requested by the corresponding GETREQ or SETREQ is a SUCCESS */
        if(prim->seqNo == inst->bccmdSeqNo)
        {
            /* Convert the payload from XAP format, and copy the value into the given
            parameter.    */
            switch(prim->varId)
            {
                case 0x2859: /* ADPCM_LOCAL_SUPPORTED*/
                {
                    CsrUint16 support;
                    CsrUint8  *temp = prim->payload;

                    CSR_CONVERT_16_FROM_XAP(support, temp);
                     /* we are now allowed to move to next seq no */
                    inst->bccmdSeqNo++;
                    if(support)
                    {
                        inst->adpcmLocalSupported = TRUE;
                    }
                    else
                    {
                        inst->adpcmLocalSupported = FALSE;
                    }
                    if (inst->pendingAdcpmQuery)
                    {
                        CsrBtHfgSendHfgLocalAdpcmSupportInd(inst,inst->adpcmLocalSupported);
                        inst->pendingAdcpmQuery = FALSE;
                    }
                    break;
                }
                case 0x5039: /* VOICE_CODEC */
                case 0x5037:
                {
                    HfgInstance_t *linkData = CsrBtHfgFindLinkWithSeqNo(inst, prim->seqNo);

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    if (linkData)
#endif
                    {
                        CsrBtHfgXSecondBccmdCfmHandler(linkData);
                    }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    else
                    {
                        /* State/event error. The signals above may be received in some cross situations */
                        CsrGeneralException("HFG", (CsrUint16)0, prim->type,"Invalid (non-connection) BCCMD primitive");
                    }
#endif
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
}

void CsrBtHfgXSecondBccmdCfmHandler(HfgInstance_t *inst)
{
    CsrBtHfgAudioCodecStatus *codecSet;
    /* nothing interesting in return value at this point We just send a status rapport */
    CsrBtHfgGetMainInstance(inst)->bccmdSeqNo++; /* we are now allowed to move to next seq no */
    codecSet = (CsrBtHfgAudioCodecStatus*)CsrPmemAlloc(sizeof(CsrBtHfgAudioCodecStatus));
    codecSet->supportedMask = inst->ind.c2c[CSR_BT_C2C_ADPCM_IND];
    codecSet->currentCodec = inst->audioCodecType;
    CsrBtHfgSendAudioCodecHfgStatusAudioInd(inst,codecSet);

}
#endif
