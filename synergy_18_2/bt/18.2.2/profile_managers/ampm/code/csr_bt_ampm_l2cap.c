/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

/* Create a new L2CA CHANNEL instance and assign the supplied data. */
static CsrBtAmpmL2cap *csrBtAmpmL2capNew(
    CsrBtAmpmInstance * inst,
    CsrBtConnId cid,
    BD_ADDR_T bd_addr)
{
    CsrBtAmpmL2cap *pL2caChan = NULL;

    CSR_BT_AMPM_ASSERT(inst->pL2caChanListAnchor != NULL);

    pL2caChan = (CsrBtAmpmL2cap *) CsrPmemAlloc(sizeof(*pL2caChan));

    pL2caChan->state = AMPM_L2CAP_ST_NULL;
    pL2caChan->bd_addr = bd_addr;
    pL2caChan->cid = cid;
    pL2caChan->pUsingLogLink = NULL;
    pL2caChan->discReqLocalAmpId = 0;

    CsrBtAmpmListAddHead(inst->pL2caChanListAnchor,
        (CsrBtAmpmGenListMember *) pL2caChan);

    return (pL2caChan);
}

/* Sets the flow specifications of the L2CA CHANNEL instance */
static void csrBtAmpmL2capSetFlowspecs(
    CsrBtAmpmL2cap * pL2caChan,
    L2CA_FLOWSPEC_T * pTxFlowspec,
    L2CA_FLOWSPEC_T * pRxFlowspec)
{
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);
    CSR_BT_AMPM_ASSERT(pTxFlowspec != NULL);
    CSR_BT_AMPM_ASSERT(pRxFlowspec != NULL);

    pL2caChan->txFlowspec = *pTxFlowspec;
    pL2caChan->rxFlowspec = *pRxFlowspec;
}



/* Performs the checking of a generic list element against the
 * supplied channel id and returns TRUE if they match. */
static CsrBool csrBtAmpmL2capMatchesCid(
    const void *pCriteria,
    const void *pListEntry)
{
    CsrBtConnId cid;
    const CsrBtAmpmL2cap *pL2caChan;
    CsrBool doesEntryMatch = FALSE;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    cid = *((const CsrBtConnId *) pCriteria);
    pL2caChan = (const CsrBtAmpmL2cap *) pListEntry;

    if (pL2caChan->cid == cid)
    {
        doesEntryMatch = TRUE;
    }
    return (doesEntryMatch);
}

/* Runs the FSM associated with the specified L2CAP Channel
 * instance */
static void csrBtAmpmL2capFsmRun(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmL2cap * pL2caChan,
    fsm_event_t event,
    void *param2)
{
    fsm_result_t fsm_result;
    CsrBtAmpmFsmParam param;

    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    param.inst = inst;
    param.argp = pL2caChan;

    fsm_result =
        fsm_16bit_run(&csr_bt_ampm_l2cap_fsm, &(pL2caChan->state), event, &param,
        param2);

    CSR_BT_AMPM_ASSERT(FSM_RESULT_OK == fsm_result);

    /* Depending on the configuration, CSR_BT_AMPM_ASSERT may be invalid and thus fsm_result is unsed */
    CSR_UNUSED(fsm_result);
}


void CsrBtAmpmL2capInit(
    CsrBtAmpmInstance * inst)
{
    CSR_BT_AMPM_ASSERT(inst->pL2caChanListAnchor == NULL);
    inst->pL2caChanListAnchor = CsrBtAmpmListCreateAnchor();
}


void CsrBtAmpmL2capDeinit(CsrBtAmpmInstance * inst)
{
    if(inst->pL2caChanListAnchor != NULL)
    {
        CSR_BT_AMPM_ASSERT(inst->pL2caChanListAnchor != NULL);
        while (!CsrBtAmpmListIsEmpty(inst->pL2caChanListAnchor))
        {
            /* This function will automatically remove the item from
             * the list, so don't do it here */
            CsrBtAmpmL2capDelete(inst,
                                 (CsrBtAmpmL2cap*)inst->pL2caChanListAnchor->head);
        }
        CsrBtAmpmListDestroyAnchor(&(inst->pL2caChanListAnchor));
    }
}


void CsrBtAmpmL2capDelete(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    (void) CsrBtAmpmListRemoveMatchingMember(inst->pL2caChanListAnchor,
        CsrBtAmpmListEntryMatchesPtr, pL2caChan);
    CsrPmemFree((void *) pL2caChan);
}


CsrBtAmpmL2cap *CsrBtAmpmL2capGetByCid(
    CsrBtAmpmInstance * inst,
    CstBtAmpmGenListAnchor * pListAnchor,
    CsrBtConnId cid)
{
    CsrBtAmpmL2cap *pL2caChan;

    CSR_BT_AMPM_ASSERT(pListAnchor != NULL);

    /* Find the required element based on the Amp Id */
    pL2caChan =
        (CsrBtAmpmL2cap *) CsrBtAmpmListFindMatchingMember(pListAnchor,
        csrBtAmpmL2capMatchesCid, (const void *) &cid);
    return (pL2caChan);
}


CsrBool CsrBtAmpmL2capWaitingForPhyLink(
    CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);
    return (AMPM_L2CAP_ST_WAIT_PHYSICAL_LINK == pL2caChan->state);
}


CsrBool CsrBtAmpmL2capConnectedOverLogLink(
    CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);
    return (AMPM_L2CAP_ST_READY == pL2caChan->state);
}


CsrBool CsrBtAmpmL2capConnectingOverLogLink(
    CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);
    return (AMPM_L2CAP_ST_ESTABLISHING_CHANNEL == pL2caChan->state);
}


void CsrBtAmpmL2capPhyEvent(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink,
    fsm_event_t Event)
{
    CsrBtAmpmL2cap *pL2caChan = NULL;
    CsrBtAmpmGenListMember *pListMember;

    CSR_BT_AMPM_ASSERT(inst->pL2caChanListAnchor != NULL);
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    pListMember = inst->pL2caChanListAnchor->head;

    /* Iterate over the list of L2ca channels looking for ones that
     * reference the specified physical link, but not logical links.
     * As those that reference the logical links will get the event
     * via the logical link processing.
     */
    while (NULL != pListMember)
    {
        pL2caChan = (CsrBtAmpmL2cap *) pListMember;
        pListMember = pListMember->next;
        if (pPhyLink == pL2caChan->pUsingPhyLink &&
            NULL == pL2caChan->pUsingLogLink)
        {
            csrBtAmpmL2capFsmRun(inst, pL2caChan, Event, NULL);
        }
    }
}


CsrUint8 CsrBtAmpmL2capL2caChansUsingPhyLink(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmL2cap *pL2caChan = NULL;
    CsrBtAmpmGenListMember *pListMember;
    CsrUint8 UsingPhyLinkCount = 0;

    CSR_BT_AMPM_ASSERT(inst->pL2caChanListAnchor != NULL);
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);


    pListMember = inst->pL2caChanListAnchor->head;

    /* Iterate over the list of L2ca channels looking for ones that
     * reference the specified physical link
     */
    while (NULL != pListMember)
    {
        pL2caChan = (CsrBtAmpmL2cap *) pListMember;
        pListMember = pListMember->next;
        if (pPhyLink == pL2caChan->pUsingPhyLink)
        {
            UsingPhyLinkCount++;
        }
    }

    return (UsingPhyLinkCount);
}


CsrBool CsrBtAmpmL2capUsingPhyLinkAreAllDisconnecting(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmL2cap *pL2caChan = NULL;
    CsrBtAmpmGenListMember *pListMember;
    CsrBool AllDisconnecting = TRUE;

    CSR_BT_AMPM_ASSERT(inst->pL2caChanListAnchor != NULL);
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    pListMember = inst->pL2caChanListAnchor->head;

    /* Iterate over the list of L2ca channels looking for ones that
     * reference the specified physical link
     */
    while (NULL != pListMember && TRUE == AllDisconnecting)
    {
        pL2caChan = (CsrBtAmpmL2cap *) pListMember;
        pListMember = pListMember->next;
        if (pPhyLink == pL2caChan->pUsingPhyLink &&
            AMPM_L2CAP_ST_DISCONNECTING != pL2caChan->state)
        {
            AllDisconnecting = FALSE;
        }
    }
    return (AllDisconnecting);
}


void CsrBtAmpmL2capSwitchPhyLink(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pOldPhyLink,
    CsrBtAmpmPhyLink * pNewPhyLink)
{
    CsrBtAmpmL2cap *pL2caChan = NULL;
    CsrBtAmpmGenListMember *pListMember;

    CSR_BT_AMPM_ASSERT(inst->pL2caChanListAnchor != NULL);
    CSR_BT_AMPM_ASSERT(pOldPhyLink != NULL);
    CSR_BT_AMPM_ASSERT(pNewPhyLink != NULL);

    pListMember = inst->pL2caChanListAnchor->head;

    /* Iterate over the list of L2ca channels looking for ones that
     * reference the specified physical link
     */
    while (NULL != pListMember)
    {
        pL2caChan = (CsrBtAmpmL2cap *) pListMember;
        pListMember = pListMember->next;
        if (pOldPhyLink == pL2caChan->pUsingPhyLink)
        {
            pL2caChan->pUsingPhyLink = pNewPhyLink;
        }
    }
}

void CsrBtAmpmL2capDmAmpmConnectInd(
    CsrBtAmpmInstance * inst,
    DM_AMPM_CONNECT_IND_T * prim)
{
    CSR_BT_AMPM_ASSERT(prim != NULL);

    /* The DM CONNECT IND contains both the local and remote amp ids
     * that need to support a physical link between the two devices
     */
    if (CSR_BT_AMP_CONTROLLER_UNKNOWN != prim->local_amp_id &&
        CSR_BT_AMP_CONTROLLER_UNKNOWN != prim->remote_amp_id)
    {
        CsrBtAmpmPhyLink *pPhyLink = NULL;
        CsrUint8 localAmpId = CSR_BT_AMPM_L2CA_TO_AMP_ID(prim->local_amp_id);
        CsrUint8 remoteAmpId = CSR_BT_AMPM_L2CA_TO_AMP_ID(prim->remote_amp_id);

        /* Find the required local AMP instance based on the supplied AMP id */
        CsrBtAmpmLocalAmp *pLocalAmp = CsrBtAmpmLocalGetById(inst, localAmpId);

        if (NULL != pLocalAmp)
        {
            /* Obtain either an existing physical link or create a new one.
             * This will rattle the PHY FSM as necessary */
            pPhyLink = CsrBtAmpmPhyGetPhyForAmps(inst, localAmpId, remoteAmpId,
                                                 prim->bd_addr);
        }

        if (NULL != pPhyLink)
        {
            CsrBtAmpmL2cap *pL2caChan = csrBtAmpmL2capNew(inst,
                                                          prim->cid,
                                                          prim->bd_addr);

            pL2caChan->pUsingPhyLink = (void *) pPhyLink;
            csrBtAmpmL2capFsmRun(inst, pL2caChan,
                                 AMPM_L2CAP_EV_DM_AMPM_CONNECT, (void *) pPhyLink);
        }
        else
        {
            dm_ampm_connect_rsp(prim,
                                HCI_ERROR_NO_CONNECTION);
        }
    }
    else
    {
        dm_ampm_connect_rsp(prim,
                            HCI_ERROR_UNSPECIFIED);
    }

}


void CsrBtAmpmL2capDmAmpmConnectChannelInd(
    CsrBtAmpmInstance * inst,
    DM_AMPM_CONNECT_CHANNEL_IND_T * prim)
{
    CsrBtAmpmPhyLink *pPhyLink = NULL;
    CsrBtAmpmLogicalLink *pLogLink = NULL;
    CsrBtAmpmL2cap *pL2caChan = NULL;
    CsrUint8 localAmpId;

    CSR_BT_AMPM_ASSERT(prim != NULL);

    localAmpId = CSR_BT_AMPM_L2CA_TO_AMP_ID(prim->local_amp_id);

    /* We could be the terminating end of the logical link
     * in which case there will not have been a DM AMPM CONNECT IND to
     * trigger the creation of the logical link previously.
     *
     * So we need to determine if a physical link exists?
     */
    pPhyLink = CsrBtAmpmPhyGetByBdaddrAndLocalAmpId(inst,
                                                    prim->bd_addr,
                                                    localAmpId);

    if ((NULL != pPhyLink) &&
        !CsrBtAmpmLogDontStartLogicalLink(inst))
    {
        pL2caChan = CsrBtAmpmL2capGetByCid(inst,
                                           inst->pL2caChanListAnchor,
                                           prim->cid);

        if (NULL == pL2caChan)
        {
            /* There is a suitable local amp as the physical link is already
             * established on it so create the l2CA Chan instance
             */
            pL2caChan = csrBtAmpmL2capNew(inst, prim->cid, prim->bd_addr);

            pL2caChan->pUsingPhyLink = (void *) pPhyLink;
        }

        csrBtAmpmL2capSetFlowspecs(pL2caChan, &(prim->rx_flowspec),
            &(prim->tx_flowspec));

        if (L2CA_QOS_TYPE_BEST_EFFORT == prim->rx_flowspec.fs_service_type ||
            L2CA_QOS_TYPE_BEST_EFFORT == prim->tx_flowspec.fs_service_type)
        {
            pLogLink = CsrBtAmpmLogicalExistingBestEffortLink(inst, pPhyLink);
        }
        if (NULL == pLogLink)
        {
            pLogLink = CsrBtAmpmLogicalNew(inst, prim->bd_addr);
            CsrBtAmpmLogicalAssociatePhyLink(pLogLink, pPhyLink);
        }

        if (NULL != pLogLink)
        {
            /* Remove from the general L2Chan list and associate with the
             * specific logical link
             */
            (void) CsrBtAmpmListRemoveMatchingMember(inst->
                pL2caChanListAnchor, CsrBtAmpmListEntryMatchesPtr, pL2caChan);
            CsrBtAmpmLogicalAssociateL2capChan(pLogLink, pL2caChan);
            pL2caChan->pUsingLogLink = (void *) pLogLink;
        }
    }

    if (NULL != pL2caChan)
    {
        /* Need to update the L2CA Channel instance with the complete
         * flowspec (i.e. Tx/Rx).
         */
        pL2caChan->txFlowspec = prim->tx_flowspec;
        pL2caChan->rxFlowspec = prim->rx_flowspec;

        csrBtAmpmL2capFsmRun(inst, pL2caChan,
            AMPM_L2CAP_EV_DM_AMPM_CONNECT_CHANNEL, (void *) pLogLink);
    }
    else
    {
        /* send back a failure response???? */
        dm_ampm_connect_channel_rsp(prim,
                                    0, /* Logical link id */
                                    0, /* Physical link id */
                                    CSR_SCHED_QID_INVALID,
                                    HCI_ERROR_NO_CONNECTION);
    }
}


void CsrBtAmpmL2capDmAmpmDisconnectChannelInd(
    CsrBtAmpmInstance * inst,
    DM_AMPM_DISCONNECT_CHANNEL_IND_T * prim)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmL2cap *pL2caChan;

    CSR_BT_AMPM_ASSERT(prim != NULL);

    if (CsrBtAmpmLogicalL2capChanGetByLocalCid(inst, &pLogLink, &pL2caChan,
            prim->cid))
    {
        pL2caChan->discReqLocalAmpId = prim->local_amp_id;
        csrBtAmpmL2capFsmRun(inst, pL2caChan,
            AMPM_L2CAP_EV_DISCONNECT_CHANNEL, (void *) pLogLink);
    }
    else
    {
        /* Failed to find the corresponding logical link so maybe we just have
         * the L2ca channel instance.
         */

        pL2caChan =
            CsrBtAmpmL2capGetByCid(inst, inst->pL2caChanListAnchor,
            prim->cid);
        if (NULL != pL2caChan)
        {
            pL2caChan->discReqLocalAmpId = prim->local_amp_id;
            csrBtAmpmL2capFsmRun(inst, pL2caChan,
                AMPM_L2CAP_EV_DISCONNECT_CHANNEL, (void *) NULL);
        }
        else
        {
            /* Failed to even find the l2ca channel instance, so it must
             * have already been disconnected, so just respond to DM.
             */
            dm_ampm_disconnect_channel_rsp(prim,
                                           HCI_SUCCESS);
        }
    }

}

void CsrBtAmpmL2capPhyReadyInd(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    if (CsrBtAmpmL2capWaitingForPhyLink(pL2caChan))
    {
        csrBtAmpmL2capFsmRun(inst, pL2caChan,
            AMPM_L2CAP_EV_PHYSICAL_LINK_READY,
            (void *) pL2caChan->pUsingLogLink);
    }
}

void CsrBtAmpmL2capChannelReadyInd(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    csrBtAmpmL2capFsmRun(inst, pL2caChan, AMPM_L2CAP_EV_CHANNEL_READY_IND,
        (void *) pL2caChan->pUsingLogLink);
}

void CsrBtAmpmL2capChannelDisconnectInd(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    CsrBtAmpmListAddHead(inst->pL2caChanListAnchor,
        (CsrBtAmpmGenListMember *) pL2caChan);
    csrBtAmpmL2capFsmRun(inst, pL2caChan, AMPM_L2CAP_EV_DISCONNECT_CHANNEL,
        (void *) NULL);
}

void CsrBtAmpmL2capLinkFailureInd(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    CsrBtAmpmListAddHead(inst->pL2caChanListAnchor,
        (CsrBtAmpmGenListMember *) pL2caChan);
    csrBtAmpmL2capFsmRun(inst, pL2caChan, AMPM_L2CAP_EV_LINK_FAILURE,
        (void *) NULL);
}


void CsrBtAmpmL2capLinkModifiedSuccessInd(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    csrBtAmpmL2capFsmRun(inst, pL2caChan, AMPM_L2CAP_EV_LINK_MODIFIED_SUCCESS,
        (void *) pL2caChan->pUsingLogLink);
}


void CsrBtAmpmL2capLinkModifiedFailureInd(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    csrBtAmpmL2capFsmRun(inst, pL2caChan, AMPM_L2CAP_EV_LINK_MODIFIED_FAILURE,
        (void *) NULL);
}

fsm_event_t CsrBtAmpmL2capFsmObtainPhysicalLink(
    void *arg1,
    void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    fsm_event_t returnEvent = FSM_EVENT_NULL;   /* Assume that we have to wait for the physical link */

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    pPhyLink = (CsrBtAmpmPhyLink *) arg2;

    if (CsrBtAmpmPhyIsConnected(pPhyLink))
    {
        /* The physical link is connected so we don't have to wait for it */
        returnEvent = AMPM_L2CAP_EV_PHYSICAL_LINK_READY;
    }
    return (returnEvent);
}


fsm_event_t CsrBtAmpmL2capFsmPhysicalLinkReadyInd(
    void *arg1,
    void *arg2)
{
    DM_AMPM_CONNECT_IND_T indp;
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    /* Not checking arg2 as it may or may not be a physical link instance depending
     * on whether there was already a suitable physical link present or not.
     */
    param = (CsrBtAmpmFsmParam *) arg1;
    pL2caChan = (CsrBtAmpmL2cap *) param->argp;

    /* DM library expects IND primitive, so build it */
    CsrMemSet(&indp, 0, sizeof(DM_AMPM_CONNECT_IND_T));
    indp.bd_addr = pL2caChan->bd_addr;
    indp.cid = (CsrUint16)pL2caChan->cid;
    indp.local_amp_id = (CsrUint16) CsrBtAmpmPhyGetLocalAmpId((const CsrBtAmpmPhyLink *) (pL2caChan->pUsingPhyLink));
    indp.remote_amp_id = (CsrUint16) CsrBtAmpmPhyGetRemoteAmpId((const CsrBtAmpmPhyLink *) (pL2caChan->pUsingPhyLink));

    dm_ampm_connect_rsp(&indp, HCI_SUCCESS);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmL2capFsmSendDmAmpmConnectRspFail(
    void *arg1,
    void *arg2)
{
    DM_AMPM_CONNECT_IND_T indp;
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pL2caChan = (CsrBtAmpmL2cap *) param->argp;

    /* DM library expects IND primitive, so build it */
    CsrMemSet(&indp, 0, sizeof(DM_AMPM_CONNECT_IND_T));
    indp.bd_addr = pL2caChan->bd_addr;
    indp.cid = (CsrUint16)pL2caChan->cid;

    dm_ampm_connect_rsp(&indp, HCI_ERROR_NO_CONNECTION);

    return AMPM_L2CAP_EV_DELETE_REQ;
}


fsm_event_t CsrBtAmpmL2capFsmCreateChannel(
    void *arg1,
    void *arg2)
{
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pL2caChan = (CsrBtAmpmL2cap *) param->argp;
    pLogLink = (CsrBtAmpmLogicalLink *) arg2;

    CsrBtAmpmLogicalCreateChannelReq(param->inst, pLogLink, pL2caChan);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmL2capFsmChannelReadyInd(
    void *arg1,
    void *arg2)
{
    DM_AMPM_CONNECT_CHANNEL_IND_T indp;
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *localAmp;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pL2caChan = (CsrBtAmpmL2cap *) param->argp;
    pLogLink = (CsrBtAmpmLogicalLink *) arg2;

    localAmp = (CsrBtAmpmLocalAmp*)CsrBtAmpmLocalGetById(param->inst,
                                                         CsrBtAmpmPhyGetLocalAmpId(pLogLink->pPhyLink));

    /* DM library expects IND primitive */
    CsrMemSet(&indp, 0, sizeof(DM_AMPM_CONNECT_CHANNEL_IND_T));
    indp.tx_flowspec = (pL2caChan->txFlowspec);
    indp.rx_flowspec = (pL2caChan->rxFlowspec);
    indp.bd_addr = pL2caChan->bd_addr;
    indp.local_amp_id = CsrBtAmpmPhyGetLocalAmpId(pLogLink->pPhyLink);
    indp.cid = (CsrUint16)pL2caChan->cid;

    dm_ampm_connect_channel_rsp(&indp,
                                pLogLink->handle, /* logical link id */
                                pLogLink->pPhyLink->ampPLinkHandle, /* physical link id */
                                localAmp->dataQueue,
                                HCI_SUCCESS);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmL2capFsmSendDmAmpmConnectChannelRspFail(
    void *arg1,
    void *arg2)
{
    DM_AMPM_CONNECT_CHANNEL_IND_T indp;
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pL2caChan = (CsrBtAmpmL2cap *) param->argp;

    /* DM library expects IND primitive */
    CsrMemSet(&indp, 0, sizeof(DM_AMPM_CONNECT_CHANNEL_IND_T));
    indp.tx_flowspec = (pL2caChan->txFlowspec);
    indp.rx_flowspec = (pL2caChan->rxFlowspec);
    indp.bd_addr = pL2caChan->bd_addr;
    indp.cid = (CsrUint16)pL2caChan->cid;

    dm_ampm_connect_channel_rsp(&indp,
                                0, /* logical link id */
                                0, /* physical link id */
                                CSR_SCHED_QID_INVALID, /* data queue */
                                HCI_ERROR_NO_CONNECTION);

    return AMPM_L2CAP_EV_DELETE_REQ;
}


fsm_event_t CsrBtAmpmL2capFsmDisconnectChannelBeforePhysical(
    void *arg1,
    void *arg2)
{
    DM_AMPM_CONNECT_IND_T indp;
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pL2caChan = (CsrBtAmpmL2cap *) param->argp;

    /* DM library expects IND primitive, so build it */
    CsrMemSet(&indp, 0, sizeof(DM_AMPM_CONNECT_IND_T));
    indp.bd_addr = pL2caChan->bd_addr;
    indp.cid = (CsrUint16)pL2caChan->cid;

    dm_ampm_connect_rsp(&indp,
                         HCI_ERROR_CONN_TERM_LOCAL_HOST);

    if (NULL != pL2caChan->pUsingPhyLink)
    {
        CsrBtAmpmPhyLink *pPhyLink = pL2caChan->pUsingPhyLink;

        pL2caChan->pUsingPhyLink = NULL;
        CsrBtAmpmPhyTriggerDisconnectIfRequired(param->inst, pPhyLink);
    }

    return AMPM_L2CAP_EV_DISCONNECT_CHANNEL;
}

fsm_event_t CsrBtAmpmL2capFsmDisconnectChannelBeforeLogical(
    void *arg1,
    void *arg2)
{
    DM_AMPM_CONNECT_CHANNEL_IND_T indp;
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pL2caChan = (CsrBtAmpmL2cap *) param->argp;
    pLogLink = (CsrBtAmpmLogicalLink *) arg2;

    /* DM library expects IND primitive */
    CsrMemSet(&indp, 0, sizeof(DM_AMPM_CONNECT_CHANNEL_IND_T));
    indp.tx_flowspec = (pL2caChan->txFlowspec);
    indp.rx_flowspec = (pL2caChan->rxFlowspec);
    indp.bd_addr = pL2caChan->bd_addr;
    indp.cid = (CsrUint16)pL2caChan->cid;

    dm_ampm_connect_channel_rsp(&indp,
                                0, /* logical link id */
                                0, /* physical link id */
                                CSR_SCHED_QID_INVALID, /* data queue */
                                HCI_ERROR_CONN_TERM_LOCAL_HOST);

    /* It's possible that we might be getting disconnected before the
     * logical link exists.
     */
    if (NULL != pLogLink)
    {
        CsrBtAmpmLogicalDisconnectChannelReq(param->inst, pLogLink,
            pL2caChan);
        return FSM_EVENT_NULL;
    }
    else
    {
        if (NULL != pL2caChan->pUsingPhyLink)
        {
            CsrBtAmpmPhyLink *pPhyLink = pL2caChan->pUsingPhyLink;

            pL2caChan->pUsingPhyLink = NULL;
            CsrBtAmpmPhyTriggerDisconnectIfRequired(param->inst, pPhyLink);
        }

        return AMPM_L2CAP_EV_DISCONNECT_CHANNEL;
    }
}

fsm_event_t CsrBtAmpmL2capFsmDisconnectChannel(
    void *arg1,
    void *arg2)
{
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pL2caChan = (CsrBtAmpmL2cap *) param->argp;
    pLogLink = (CsrBtAmpmLogicalLink *) arg2;

    /* It's possible that we might be getting disconnected before the
     * logical link exists.
     */
    if (NULL != pLogLink)
    {
        CsrBtAmpmLogicalDisconnectChannelReq(param->inst, pLogLink,
            pL2caChan);
        return FSM_EVENT_NULL;
    }
    else
    {
        if (NULL != pL2caChan->pUsingPhyLink)
        {
            CsrBtAmpmPhyLink *pPhyLink = pL2caChan->pUsingPhyLink;

            pL2caChan->pUsingPhyLink = NULL;
            CsrBtAmpmPhyTriggerDisconnectIfRequired(param->inst, pPhyLink);
        }
        return AMPM_L2CAP_EV_DISCONNECT_CHANNEL;
    }
}


fsm_event_t CsrBtAmpmL2capFsmChannelDisconnected(
    void *arg1,
    void *arg2)
{
    DM_AMPM_DISCONNECT_CHANNEL_IND_T indp;
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pL2caChan = (CsrBtAmpmL2cap *) param->argp;

    /* Was the disconnect requested, if so then we need to generate a response */
    if (0 < pL2caChan->discReqLocalAmpId)
    {
        /* DM library requires IND primitive */
        CsrMemSet(&indp, 0, sizeof(DM_AMPM_DISCONNECT_CHANNEL_IND_T));
        indp.cid = (CsrUint16)pL2caChan->cid;
        indp.local_amp_id = pL2caChan->discReqLocalAmpId;

        dm_ampm_disconnect_channel_rsp(&indp,
                                       HCI_SUCCESS);
        return AMPM_L2CAP_EV_DELETE_REQ;
    }
    else
    {
        /* The disconnect was not expected and we don't have any outstanding signalling
         * to send to DM to indicate the failure associated with this CID.
         */
        if (NULL != pL2caChan->pUsingLogLink)
        {
            CsrBtAmpmLogicalDeasssociateL2capChan(pL2caChan->pUsingLogLink,
                pL2caChan);
            pL2caChan->pUsingLogLink = NULL;
            CsrBtAmpmListAddHead(param->inst->pL2caChanListAnchor,
                (CsrBtAmpmGenListMember *) pL2caChan);
        }
        /* If there is a physical link then it could be about to fail as well
         * and by keeping the L2ca chan instance associated with it then the
         * physical link will generate a DM AMPM DISCONNECT REQ.
         */
        if (NULL == pL2caChan->pUsingPhyLink ||
            !CsrBtAmpmPhyInstanceExists(param->inst,
                pL2caChan->pUsingPhyLink))
        {
            /* Nothing that we can do by keeping this instance so delete it. */
            return AMPM_L2CAP_EV_DELETE_REQ;
        }
    }
    /* Keep hold of the instance to trigger further behaviour later. */
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmL2capFsmDeleteReq(
    void *arg1,
    void *arg2)
{
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);

    /* The second argument may or may not be NULL depending on how we've
     * arrived in this transition, but we'll just ignore it anyway.
     */
    param = (CsrBtAmpmFsmParam *) arg1;
    pL2caChan = (CsrBtAmpmL2cap *) param->argp;

    CsrBtAmpmL2capDelete(param->inst, pL2caChan);

    return FSM_EVENT_NULL;
}
