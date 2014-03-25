/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_proto.h"

/* Local: Get number of fully-SLC-connected connections */
static CsrUint8 csrBtHfgActiveConnections(HfgMainInstance_t *inst)
{
    CsrUintFast8 i;
    CsrUint8 num;

    num = 0;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if(inst->linkData[i].state == Connected_s)
        {
            num++;
        }
    }
    return num;
}

/* Local: Remove incoming connection setup. We simply scan the service
 * structures and remove the first one we find */
static CsrBool csrBtHfgConnectionRemove(HfgMainInstance_t *inst)
{
    CsrUintFast8 i;

    for(i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
    {
        if(inst->service[i].accepting)
        {
            CsrBtCmCancelAcceptConnectReqSend(CSR_BT_HFG_IFACEQUEUE,
                                         inst->service[i].chan);
            return TRUE;
        }
    }
    return FALSE;
}

/* Local: Remove incoming connection for a serverchannel that is found
 * to be in use by a connection -- which indicates that the connection
 * has already been established */
static CsrBool csrBtHfgConnectionConflict(HfgMainInstance_t *inst)
{
    CsrUintFast8 i;
    HfgService_t *ser;

    for(i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
    {
        ser = &(inst->service[i]);

        if(CsrBtHfgOccupiedServerChannel(inst, ser->chan) &&
           ser->accepting)
        {
            CsrBtCmCancelAcceptConnectReqSend(CSR_BT_HFG_IFACEQUEUE,
                                         ser->chan);
            return TRUE;
        }
    }
    return FALSE;
}

/* Local: Setup incoming connections for all 'blocks' (HFG/AG) that
 * have a service-record and is not already accepting connections.
 * Note that connect-accept-req does not return, so we simply send all
 * the requests at once */
static CsrBool csrBtHfgConnectionAccept(HfgMainInstance_t *inst)
{
    CsrUintFast8 i;
    CsrUintFast8 j;
    CsrUint8 found;
    CsrBool retVal = FALSE;

    /* Scan each 'block' of service structures to see if a whole block
     * is _not_ registered. If so, register the given record */
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        /* Scan block */
        found = 0;
        for(j=(i*CSR_BT_HFG_NUM_RECORDS); j<((i+1)*CSR_BT_HFG_NUM_RECORDS); j++)
        {
            found = found | inst->service[j].registered;
            if(found != 0)
            {
                break;
            }
        }

        if(found != 0)
        {
            /* Item found to be registered, accept for the connections
             * that's not already accepting */
            HfgService_t *ser;
            CsrUintFast8 k;
            HfgInstance_t *link;
            CsrBtDeviceAddr  tmpAddress;

            link = &(inst->linkData[i]);

            tmpAddress.nap = link->address.nap;
            tmpAddress.uap = link->address.uap;
            tmpAddress.lap = link->address.lap;

            for(k=0; k<CSR_BT_HFG_NUM_SERVERS; k++)
            {
                link = &(inst->linkData[k]);
                if((link->address.nap == tmpAddress.nap) &&
                   (link->address.uap == tmpAddress.uap) &&
                   (link->address.lap == tmpAddress.lap) &&
                   (i != k) &&
                   (link->linkType == inst->linkData[i].linkType))
                {/* This means an incoming and an outgoing connection to the same device cross.
                    SDC search already ongoing or started elsewhere: return TRUE so no new SDC
                    search is started */
                    retVal = TRUE;
                    break;
                }
            }

            for(j=(i*CSR_BT_HFG_NUM_RECORDS); j<((i+1)*CSR_BT_HFG_NUM_RECORDS); j++)
            {
                ser = &(inst->service[j]);
                if(!CsrBtHfgOccupiedServerChannel(inst, ser->chan) &&
                   !ser->accepting)
                {
                    /* Found entry -- begin accepting connections */
                    ser->accepting = TRUE;
                    CsrBtCmConnectAcceptReqSend(CSR_BT_HFG_IFACEQUEUE,
                                           CSR_BT_HFG_CLASS_OF_DEVICE,
                                           0,
                                           CSR_BT_HFG_PROFILE_DEFAULT_MTU_SIZE,
                                           ser->chan,
                                           inst->secIncoming,
                                           CSR_BT_HFG_PROFILE_UUID,
                                           inst->modemStatus,
                                           CSR_BT_DEFAULT_BREAK_SIGNAL,
                                           CSR_BT_DEFAULT_MSC_TIMEOUT);
                }
            }
        }
    }
    return retVal;
}

/* Setup incoming connections depending on the desired state of
 * connection instances, pending signals etc. The function returns
 * TRUE if any requests have been sent and FALSE when not (ie. when
 * the connection setup/takedown has been completed) */
CsrBool CsrBtHfgConnectionUpdate(HfgMainInstance_t *inst)
{
    /* General plan:
     *
     * Remove all accept-connect (incoming connections) if:
     * - Main state is null/deactivate
     * - Max number of active (connected) connections reached
     *
     * Scan service table
     * - Remove incoming connections with occupied serverchannel
     *
     * Setup incoming connections, based on
     * - Block (HFG/AG) have a service record
     * - Not occupied serverchannel
     * - Not already accepting
 */

    /* Remove connections if state says so, or if the maximum number
     * of connections has been reached. Bail out even if we didn't
     * request any changes */
    if((inst->state == MainNull_s) ||
       (inst->state == MainDeactivate_s) ||
       (csrBtHfgActiveConnections(inst) >= inst->maxConnections))
    {
        if(csrBtHfgConnectionRemove(inst))
        {
            return TRUE;
        }
        else
        {
            /* We didn't do anything, but need to exit */
            return FALSE;
        }
    }

    /* Check for conflicting server channels between registered SDS
     * and connections */
    if(csrBtHfgConnectionConflict(inst))
    {
        return TRUE;
    }

    /* Finally, check if we should setup one of the incoming
     * connections. This will send requests if necessary, but there's
     * no result as confirms are not sent until a connection is
     * actually there or has been cancelled.
     * However if one of the incoming connections should be setup, and
     * a link attached to the service record in question contains a
     * bd_address equal to one in use in other serverchannel, make sure
     * not to start SDC search once again.... */
     if (csrBtHfgConnectionAccept(inst))
     {
         return TRUE;
     }

    return FALSE;
}

/* Update the instance data for a received connection accept or cancel
 * primitive. We only flag the SDS container as not accepting
 * connections anymore */
void CsrBtHfgConnectionConfirm(HfgMainInstance_t *inst,
                          CsrUint8 chan)
{
    CsrUintFast8 i;

    for(i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
    {
        if(inst->service[i].chan == chan)
        {
            inst->service[i].accepting = FALSE;
            return;
        }
    }
}

/* Initiate incoming/outgoing HFG/AG outgoing connection. This
 * function may not be called before the other incoming connections
 * have been taken down or otherwise setup correctly. The function is
 * merely used to kickstart the next step (like SDC-search,
 * AT-sequence, etc.) */
void CsrBtHfgConnectionStart(HfgInstance_t *linkData)
{
    if(linkData->state == Connect_s)
    {
        /* Outgoing - check for pending disconnect/cancel that may
         * have crossed us */
        if(linkData->pendingDisconnect)
        {
            /* Disconnect pending completed */
            CsrBtHfgSendHfgDisconnectInd(linkData, TRUE, RFC_NORMAL_DISCONNECT, CSR_BT_SUPPLIER_RFCOMM);
            linkData->pendingDisconnect = FALSE;
            CsrBtHfgLinkConnectFailed(linkData);
        }
        else if(linkData->pendingCancel)
        {
            /* Can't cancel while cancelling... */
            CsrBtHfgSendHfgServiceConnectInd(linkData, CSR_BT_RESULT_CODE_HFG_CANCELLED_CONNECT_ATTEMPT, CSR_BT_SUPPLIER_HFG);
            linkData->pendingCancel = FALSE;
            CsrBtHfgLinkConnectFailed(linkData);
        }
        else
        {
            /* Start connection via the SDC search */
            CsrBtHfgStartSdcSearch(linkData);
        }
    }
    else
    {
        /* Incoming */
        HFG_CHANGE_STATE(linkData->oldState, Activate_s);

        if(linkData->linkType == CSR_BT_HFG_CONNECTION_HFG)
        {  /* HFG - start service search */
           CmnCsrBtLinkedListStruct *sdpTagList = NULL;
            CsrUint16 shIndex;

            sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_HF_PROFILE_UUID, &shIndex);
            if (sdpTagList != NULL)
            {
                linkData->pendingSearch = TRUE;
                CsrBtUtilSdrInsertLocalServerChannel(sdpTagList, shIndex, linkData->serverChannel);
                CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SUPPORTED_FEATURES_ATTRIBUTE_IDENTIFIER, NULL, 0);
                CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, NULL, 0);
                CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);

                CsrBtUtilSdcSearchStart((void *)linkData, CsrBtHfgGetMainInstance(linkData)->sdpHfgSearchData, sdpTagList, linkData->address);
            }
            else
            {
                linkData->pendingSearch = FALSE;
            }
        }
        else
        {
            /* AG - connection done */
            linkData->remSupFeatures = CSR_BT_HF_DEFAULT_SUPPORTED_FEATURES;
            HFG_CHANGE_STATE(linkData->state, Connected_s);
            linkData->linkType = CSR_BT_HFG_CONNECTION_AG;

            CsrBtHfgSendHfgServiceConnectInd(linkData, CSR_BT_RESULT_CODE_HFG_SUCCESS, CSR_BT_SUPPLIER_HFG);
            CsrBtHfgLinkConnectSuccess(linkData);
            CsrBtHfgGetMainInstance(linkData)->activeConnection = CSR_BT_HFG_NO_CONID;
        }
    }
}

/* Force shutdown of the first active connection. Return TRUE if a
 * request was sent, otherwise FALSE */
CsrBool CsrBtHfgConnectionClose(HfgInstance_t *linkData)
{
    switch(linkData->state)
    {
        case Activate_s:
            /* Nothing special required */
            return FALSE;

        case Connect_s:
            /* Only signal received in outgoing connection state
             * is CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM, so wait for
             * that */
            return FALSE;

        case ServiceSearch_s:
            /* Only send disconnect if we don't have any pending
             * SDC signals */
            if(!linkData->pendingSearch)
            {
                CsrBtCmDisconnectReqSend(linkData->hfgConnId);
                return TRUE;
            }
            else
            {
                if (!linkData->pendingCancel)
                {
                    if (linkData->searchAndCon)
                    {
                        linkData->pendingCancel = TRUE;
                        CsrBtUtilRfcConCancel((void*)(CsrBtHfgGetMainInstance(linkData)),CsrBtHfgGetMainInstance(linkData)->sdpHfgSearchConData);
                    }
                    else
                    {
                        CsrBtUtilSdcSearchCancel((void*)(CsrBtHfgGetMainInstance(linkData)),CsrBtHfgGetMainInstance(linkData)->sdpHfgSearchData);
                    }
                }
            }
            return FALSE;

        case Connected_s:
            /* Send disconnect */
            if(linkData->scoHandle == CSR_SCHED_QID_INVALID)
            {
                CsrBtCmScoCancelReqSend(CSR_BT_HFG_IFACEQUEUE, linkData->hfgConnId);
                CsrBtCmDisconnectReqSend(linkData->hfgConnId);
                return TRUE;
            }
            else
            {
#ifdef CSR_USE_DSPM
                if (CsrBtHfgGetMainInstance(linkData)->hfgDspmOpPending == DSPM_NOP)
                {
                    CsrBtHfgDspmSendDisconnectStreams(linkData);
                }
#endif
                CsrBtCmScoDisconnectReqSend(CSR_BT_HFG_IFACEQUEUE,
                                       linkData->hfgConnId);
                return TRUE;
            }

        default:
            /* Not possible */
            return FALSE;
    }
}

/* Start whatever step is necessary to bring the profile back to idle
 * state. Return TRUE when some request was sent, or FALSE if we are
 * able to enter the state directly. Note that this is *NOT* done
 * here! */
CsrBool CsrBtHfgConnectionGoIdle(HfgMainInstance_t *inst)
{
    CsrUintFast8 i;

    /* Close links */
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if(CsrBtHfgConnectionClose(&(inst->linkData[i])))
        {
            return TRUE;
        }
    }

    /* No active links, don't accept incoming */
    if(CsrBtHfgConnectionUpdate(inst))
    {
        return TRUE;
    }

    /* No accepting connections, unregister SDS */
    if(CsrBtHfgRecordUpdate(inst))
    {
        return TRUE;
    }

    /* Check that the SDS records are indeed unregistered */
    for(i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
    {
        if(inst->service[i].registered || inst->service[i].accepting)
        {
            return TRUE;
        }
    }

    /* Ready to enter idle state */
    return FALSE;
}

