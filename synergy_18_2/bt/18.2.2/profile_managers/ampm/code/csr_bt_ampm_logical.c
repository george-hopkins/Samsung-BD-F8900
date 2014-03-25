/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

/* Runs the FSM associated with the specified logical link instance */
static void csrBtAmpmLogicalFsmRun(CsrBtAmpmInstance * inst,
                                   CsrBtAmpmLogicalLink * pLogLink,
                                   fsm_event_t event,
                                   void *param2)
{
    fsm_result_t fsm_result;
    CsrBtAmpmFsmParam param;

    CSR_BT_AMPM_ASSERT(pLogLink != NULL);

    param.inst = inst;
    param.argp = pLogLink;

    fsm_result =
        fsm_16bit_run(&csr_bt_ampm_logical_fsm, &(pLogLink->state), event, &param,
                      param2);

    CSR_BT_AMPM_ASSERT(FSM_RESULT_OK == fsm_result);

    /* Depending on the configuration, CSR_BT_AMPM_ASSERT may be invalid and thus fsm_result is unsed */
    CSR_UNUSED(fsm_result);
}


/* Deletes the specified Logical Link instance */
static void csrBtAmpmLogicalDelete(CsrBtAmpmInstance * inst,
                                   CsrBtAmpmLogicalLink * pLogLink)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    CSR_BT_AMPM_ASSERT(pLogLink->pLocalL2caChanListAnchor != NULL);

    if (NULL != pLogLink->pPhyLink)
    {
        CsrBtAmpmLocalRemoveHciTxCmds(inst,
                                      CsrBtAmpmPhyGetLocalAmpId(pLogLink->pPhyLink), pLogLink);
    }
    while (!CsrBtAmpmListIsEmpty(pLogLink->pLocalL2caChanListAnchor))
    {
        CsrBtAmpmL2cap *pL2caChan =
            (CsrBtAmpmL2cap *) CsrBtAmpmListRemoveHead(pLogLink->
                                                       pLocalL2caChanListAnchor);
        pL2caChan->pUsingLogLink = NULL;
        CsrBtAmpmL2capChannelDisconnectInd(inst, pL2caChan);
    }
    CsrBtAmpmListDestroyAnchor(&(pLogLink->pLocalL2caChanListAnchor));

    CsrBtAmpmListRemoveMember(inst->pLogicalLinkListAnchor,
                              (CsrBtAmpmGenListMember *) pLogLink);
    CsrPmemFree(pLogLink);
}

/* Performs the checking of a generic list element against the
 * supplied AMP Logical Link Handle and returns TRUE if they match. */
static CsrBool csrBtAmpmLogicalMatchesHandle(const void *pCriteria,
                                             const void *pListEntry)
{
    CsrBtAmpmLogicalHandle handle;
    const CsrBtAmpmLogicalLink *pLogLink;
    CsrBool doesEntryMatch = FALSE;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    handle = *((const CsrBtAmpmLogicalHandle *) pCriteria);
    pLogLink = (const CsrBtAmpmLogicalLink *) pListEntry;

    if (pLogLink->handle == handle)
    {
        doesEntryMatch = TRUE;
    }
    return (doesEntryMatch);
}

/* Cancels the supplied timer id */
static void csrBtAmpmLogicalCancelTimer(CsrSchedTid *pTid)
{
    CsrBtAmpmFsmParam *param;
    CsrUint16 mi;

    CSR_BT_AMPM_ASSERT(pTid != NULL);

    if (0 != *pTid)
    {
        (void) CsrSchedTimerCancel(*pTid, &mi, (void **) &param);
        *pTid = 0;
        CsrPmemFree(param);
    }
}

/* Clears the specified flowspec */
static void csrBtAmpmLogicalClearFlowspec(CsrBtAmpAmpFlowSpec * pFlowSpec)
{
    CSR_BT_AMPM_ASSERT(pFlowSpec != NULL);

    pFlowSpec->id = 0;
    pFlowSpec->serviceType = 0;
    pFlowSpec->maximumSduSize = 0;
    pFlowSpec->sduInterArrivalTime = 0;
    pFlowSpec->accessLatency = 0;
    pFlowSpec->flushTimeout = 0;
}

/* Copies the source L2CAP extended flowspec to the specified HCI
 * format flowspec */
static void csrBtAmpmLogicalCpyL2caFlowspec(CsrBtAmpAmpFlowSpec * pDestFlowSpec,
                                            L2CA_FLOWSPEC_T * pSourceFlowSpec)
{
    CSR_BT_AMPM_ASSERT(pDestFlowSpec != NULL);
    CSR_BT_AMPM_ASSERT(pSourceFlowSpec != NULL);

    pDestFlowSpec->id = pSourceFlowSpec->fs_identifier;
    pDestFlowSpec->serviceType = pSourceFlowSpec->fs_service_type;
    pDestFlowSpec->maximumSduSize = pSourceFlowSpec->fs_max_sdu;
    pDestFlowSpec->sduInterArrivalTime = pSourceFlowSpec->fs_interarrival;
    pDestFlowSpec->accessLatency = pSourceFlowSpec->fs_latency;
    pDestFlowSpec->flushTimeout = pSourceFlowSpec->fs_flush_to;
}


/* Adds the source flowspec to the destination */
static void csrBtAmpmLogicalAddL2caFlowspec(CsrBtAmpmLogicalLink * pLogLink,
                                            CsrBtAmpAmpFlowSpec * pDestFlowSpec,
                                            L2CA_FLOWSPEC_T * pSourceFlowSpec)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    CSR_BT_AMPM_ASSERT(pDestFlowSpec != NULL);
    CSR_BT_AMPM_ASSERT(pSourceFlowSpec != NULL);

    /* The assumption here is that this function has been called
     * to aggregate two best effort flow specs together.
     */
    CSR_BT_AMPM_ASSERT(L2CA_QOS_TYPE_GUARANTEED != pDestFlowSpec->serviceType);
    CSR_BT_AMPM_ASSERT(L2CA_QOS_TYPE_GUARANTEED !=
                       pSourceFlowSpec->fs_service_type);

    if (L2CA_QOS_TYPE_NO_TRAFFIC != pSourceFlowSpec->fs_service_type)
    {
        /* Firstly check if either just wants the most that can be supplied
         * indicated by Max SDU Size and SDU inter-arrival time set to
         * 0xFFFFFFFF, as this will remain 0xFFFFFFFF.
         */
        if (L2CA_FLOWSPEC_MAX_SDU ==
            pSourceFlowSpec->fs_max_sdu ||
            L2CA_FLOWSPEC_MAX_SDU ==
            pDestFlowSpec->maximumSduSize)
        {
            pDestFlowSpec->maximumSduSize =
                L2CA_FLOWSPEC_MAX_SDU;
            pDestFlowSpec->sduInterArrivalTime =
                L2CA_FLOWSPEC_INTERAR_DEFAULT;
        }
        else
        {
            CsrUint32 AmpTotalBandwidth =
                CsrBtAmpmLocalGetMaxBandwidth(pLogLink->pPhyLink->
                                              pLocalAMPInfo);
            CsrUint32 SourceBandwidth = 0;
            CsrUint32 DestBandwidth = 0;
            CsrUint32 CombinedBandwidth = 0;

            /* Otherwise we need to add the two bandwidths together and then
             * arrive back at the figure for the SDU inter-arrival time by dividing
             * by the Maximum SDU size of the particular AMP that the logical
             * link is running over. We also need to avoid exceeding the maximum
             * bandwidth of the actual AMP.
             *
             * Assuming that we can get anyway with doing this in each direction
             * seperately and not exceed the total bandwidth, otherwise the AMP should
             * not have been selected for supporting the channel.
             */

            if (0 < pSourceFlowSpec->fs_max_sdu)
            {
                SourceBandwidth =
                    (pSourceFlowSpec->fs_max_sdu * 8) /
                    pSourceFlowSpec->fs_interarrival;
            }
            if (L2CA_QOS_TYPE_NO_TRAFFIC != pDestFlowSpec->serviceType &&
                0 < pDestFlowSpec->maximumSduSize)
            {
                DestBandwidth =
                    (pDestFlowSpec->maximumSduSize * 8) /
                    pDestFlowSpec->sduInterArrivalTime;
            }
            CombinedBandwidth = SourceBandwidth + DestBandwidth;

            if (CombinedBandwidth > AmpTotalBandwidth)
            {
                /* We can not exceed the total bandwidth available
                 * from the device because it will reject the
                 * request. */
                CombinedBandwidth = AmpTotalBandwidth;
            }
            /* Probably the best thing to do would be to only allow channels that require
             * more than the BT bandwidth, so that we don't get bandwidths
             * produced that are less than 1 Mbps
             */
            if (0 < CombinedBandwidth)
            {
                pDestFlowSpec->maximumSduSize =
                    CsrBtAmpmLocalGetMaxPduSize(pLogLink->pPhyLink->
                                                pLocalAMPInfo);
                pDestFlowSpec->sduInterArrivalTime =
                    (pDestFlowSpec->maximumSduSize * 8) / CombinedBandwidth;
            }
        }
    }
}

/* Aggregates the best effort L2CAP channels that are using the
 * logical link together. */
static void csrBtAmpmLogicalAggregateBestEffortFlowspecs(CsrBtAmpmLogicalLink * pLogLink)
{
    CsrBtAmpmGenListMember *pListMember = NULL;

    CSR_BT_AMPM_ASSERT(pLogLink != NULL);

    /* Initialise the values so that we don't aggregate the values for the link
     * already with all the l2ca channels using it again.
     */
    pLogLink->txFlowspec.maximumSduSize = 0;
    pLogLink->txFlowspec.sduInterArrivalTime = 0;
    pLogLink->rxFlowspec.maximumSduSize = 0;
    pLogLink->rxFlowspec.sduInterArrivalTime = 0;

    /* Iterate over the list aggregating the channels together. */
    pListMember = pLogLink->pLocalL2caChanListAnchor->head;
    while (NULL != pListMember)
    {
        /* Even if the max SDU size is set to 0xFFFF we still need
         * to iterate over the list to determine the number of channels
         * that are aggreagted together.
         */
        CsrBtAmpmL2cap *pL2caChan = (CsrBtAmpmL2cap *) pListMember;

        if (CsrBtAmpmL2capConnectedOverLogLink(pL2caChan) ||
            CsrBtAmpmL2capConnectingOverLogLink(pL2caChan))
        {
            /* Combine in the new L2ca flow specs */
            csrBtAmpmLogicalAddL2caFlowspec(pLogLink, &(pLogLink->txFlowspec),
                                            &(pL2caChan->txFlowspec));
            csrBtAmpmLogicalAddL2caFlowspec(pLogLink, &(pLogLink->rxFlowspec),
                                            &(pL2caChan->rxFlowspec));
        }
        pListMember = pListMember->next;
    }
}

/* Reports the logical link failure to any associated L2CA channel
 * instances */
static void csrBtAmpmLogicalReportLinkFailureToL2caChans(CsrBtAmpmInstance * inst,
                                                         CsrBtAmpmLogicalLink * pLogLink)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);

    while (!CsrBtAmpmListIsEmpty(pLogLink->pLocalL2caChanListAnchor))
    {
        CsrBtAmpmL2cap *pL2caChan =
            (CsrBtAmpmL2cap *) CsrBtAmpmListRemoveHead(pLogLink->
                                                       pLocalL2caChanListAnchor);
        pL2caChan->pUsingLogLink = NULL;
        CsrBtAmpmL2capLinkFailureInd(inst, pL2caChan);
    }
}

/* Reports the logical link disconnect to any associated L2CA channel
 * instances and removes the association */
static void csrBtAmpmLogicalReportLinkDisconnectToL2caChans(CsrBtAmpmInstance * inst,
                                                            CsrBtAmpmLogicalLink * pLogLink)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);

    while (!CsrBtAmpmListIsEmpty(pLogLink->pLocalL2caChanListAnchor))
    {
        CsrBtAmpmL2cap *pL2caChan =
            (CsrBtAmpmL2cap *) CsrBtAmpmListRemoveHead(pLogLink->
                                                       pLocalL2caChanListAnchor);
        pL2caChan->pUsingLogLink = NULL;
        CsrBtAmpmL2capChannelDisconnectInd(inst, pL2caChan);
    }
}

/* Returns TRUE if the specified logical link is in the process of
 * connecting */
static CsrBool csrBtAmpmLogicalIsConnecting(CsrBtAmpmLogicalLink * pLogLink)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    return (AMPM_LOGICAL_ST_CREATING_LINK == pLogLink->state);
}

/* Returns TRUE if the specified logical link is in the process of
 * disconnecting */
static CsrBool csrBtAmpmLogicalIsDisconnecting(CsrBtAmpmLogicalLink * pLogLink)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    return (AMPM_LOGICAL_ST_DISCONNECTING == pLogLink->state ||
            AMPM_LOGICAL_ST_CANCELLING == pLogLink->state);
}


/* Returns TRUE if the specified logical link maybe started */
static CsrBool csrBtAmpmLogicalCanStart(CsrBtAmpmInstance * inst,
                                        CsrBtAmpmLogicalLink * pLogLinkToStart)
{
    CsrBtAmpmGenListMember *pListMember = inst->pLogicalLinkListAnchor->head;
    CsrBool CanStart = TRUE;

    /* If there is already a logical link been created on the same AMP
     * as this one then we have to wait for that to finsih before we
     * can create this one.  Some AMP might be able to handle
     * concurrent logical link creation but some might not so rather
     * than over complicate the AMP Manager with extra logic to handle
     * the AMP possibly rejecting a request to create a logical link,
     * it is easier to only allow the creation of one logical link at
     * a time. */
    while (TRUE == CanStart && NULL != pListMember)
    {
        CsrBtAmpmLogicalLink *pOtherLogLink =
            (CsrBtAmpmLogicalLink *) pListMember;

        if (pOtherLogLink != pLogLinkToStart &&
            (CsrBtAmpmPhyGetLocalAmpId(pOtherLogLink->pPhyLink) ==
             CsrBtAmpmPhyGetLocalAmpId(pLogLinkToStart->pPhyLink)))
        {
            if (csrBtAmpmLogicalIsConnecting(pOtherLogLink))
            {
                CanStart = FALSE;
            }
        }
        pListMember = pListMember->next;
    }
    return (CanStart);
}

/* Will start the first logical link that it finds that is using the
 * same AMP as the supplied logical link and has been waiting for a
 * logical link to complete connecting. */
static void csrBtAmpmLogicalStartWaitingLink(CsrBtAmpmInstance * inst,
                                             CsrBtAmpmLogicalLink * pLogLink)
{
    CsrBtAmpmGenListMember *pListMember = inst->pLogicalLinkListAnchor->head;
    CsrBool LinkStarted = FALSE;

    while (!LinkStarted && NULL != pListMember)
    {
        CsrBtAmpmLogicalLink *pOtherLogLink =
            (CsrBtAmpmLogicalLink *) pListMember;

        if ((pOtherLogLink != pLogLink) &&
            (CsrBtAmpmPhyGetLocalAmpId(pOtherLogLink->pPhyLink) ==
             CsrBtAmpmPhyGetLocalAmpId(pLogLink->pPhyLink)))
        {
            if (csrBtAmpmLogicalIsConnecting(pOtherLogLink))
            {
                LinkStarted = TRUE;
            }
            else if (pOtherLogLink->waitingToStart)
            {
                CsrBtAmpmL2cap *pL2caChan = NULL;

                pL2caChan =
                    (CsrBtAmpmL2cap *) pOtherLogLink->
                    pLocalL2caChanListAnchor->head;

                csrBtAmpmLogicalFsmRun(inst, pOtherLogLink,
                                       AMPM_LOGICAL_EV_CREATE_REQ, pL2caChan);
                LinkStarted = TRUE;
            }
        }
        pListMember = pListMember->next;
    }
}

void CsrBtAmpmLogSetDontStartLogicalLink(CsrBtAmpmInstance *inst,
                                         CsrBtAmpmTestDontStartLogLinkReq *prim)
{
    inst->dontStartLogicalLinkEnabled = prim->enabled;
}

CsrBool CsrBtAmpmLogDontStartLogicalLink(CsrBtAmpmInstance *inst)
{
    return inst->dontStartLogicalLinkEnabled;
}

CsrBtAmpmLogicalLink *CsrBtAmpmLogicalNew(CsrBtAmpmInstance * inst,
                                          BD_ADDR_T bd_addr)
{
    CsrBtAmpmLogicalLink *pLogLink;

    CSR_BT_AMPM_ASSERT(inst->pLogicalLinkListAnchor != NULL);

    pLogLink = (CsrBtAmpmLogicalLink *) CsrPmemAlloc(sizeof(*pLogLink));

    pLogLink->state = AMPM_LOGICAL_ST_NULL;
    pLogLink->pPhyLink = NULL;
    pLogLink->pLocalL2caChanListAnchor = CsrBtAmpmListCreateAnchor();
    pLogLink->pL2caChanTriggedConnection = NULL;
    pLogLink->pL2caChanTriggedModify = NULL;
    pLogLink->alreadyAttemptedCancel = FALSE;
    pLogLink->waitingToStart = FALSE;

    /* The handle is supplied by the PAL on successfully creating
     * the logical link.
     *
     *  pLogLink->handle;
     */

    /* For the moment just clear the flowspecs these will be assigned later
     * when we actually move to either creating the logical link or modifying
     * an existing one.
     */
    csrBtAmpmLogicalClearFlowspec(&(pLogLink->txFlowspec));
    csrBtAmpmLogicalClearFlowspec(&(pLogLink->txFlowspec));
    csrBtAmpmLogicalClearFlowspec(&(pLogLink->previousTxFlowspec));
    csrBtAmpmLogicalClearFlowspec(&(pLogLink->previousRxFlowspec));

    pLogLink->bd_addr = bd_addr;

    /* Add it to the global list of logical links within AMP Manager */
    CsrBtAmpmListAddTail(inst->pLogicalLinkListAnchor,
                         (CsrBtAmpmGenListMember *) pLogLink);

    return (pLogLink);
}


void CsrBtAmpmLogicalAssociateL2capChan(CsrBtAmpmLogicalLink * pLogLink,
                                        CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    /* Don't perfrom any processing associated with either the logical link or
     * the L2ca channel. As this is triggered from the L2ca channel as it is aware
     * of why it is been associaed with this logical link.
     */
    CsrBtAmpmListAddHead(pLogLink->pLocalL2caChanListAnchor,
                         (CsrBtAmpmGenListMember *) pL2caChan);
}


void CsrBtAmpmLogicalDeasssociateL2capChan(CsrBtAmpmLogicalLink * pLogLink,
                                           CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    /* Don't perfrom any processing associated with either the logical link or
     * the L2ca channel. As this is triggered from the L2ca channel as it is aware
     * of why it is been deassociaed from this logical link.
     */
    CsrBtAmpmListRemoveMember(pLogLink->pLocalL2caChanListAnchor,
                              (CsrBtAmpmGenListMember *) pL2caChan);
}


void CsrBtAmpmLogicalAssociatePhyLink(CsrBtAmpmLogicalLink * pLogLink,
                                      CsrBtAmpmPhyLink * pPhyLink)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    CSR_BT_AMPM_ASSERT(NULL == pLogLink->pPhyLink);
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    /* Don't perfrom any processing associated with either the logical link or
     * the physical link.
     */
    pLogLink->pPhyLink = pPhyLink;
}


void CsrBtAmpmLogicalDeassociatePhyLink(CsrBtAmpmLogicalLink * pLogLink,
                                        CsrBtAmpmPhyLink * pPhyLink)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);
    CSR_BT_AMPM_ASSERT(pPhyLink == pLogLink->pPhyLink);

    /* Don't perfrom any processing associated with either the logical link or
     * the physical link.
     */
    pLogLink->pPhyLink = NULL;
}


CsrUint8 CsrBtAmpmLogicalLinksUsingPhyLink(CsrBtAmpmInstance * inst,
                                           CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmLogicalLink *pLogLink = NULL;
    CsrBtAmpmGenListMember *pListMember = inst->pLogicalLinkListAnchor->head;
    CsrUint8 LinksUsingPhyLink = 0;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    while (NULL != pListMember)
    {
        pLogLink = (CsrBtAmpmLogicalLink *) pListMember;

        if (pPhyLink == pLogLink->pPhyLink)
        {
            LinksUsingPhyLink++;
        }
        pListMember = pListMember->next;
    }
    return (LinksUsingPhyLink);
}


CsrBtAmpmLogicalLink *CsrBtAmpmLogicalExistingBestEffortLink(CsrBtAmpmInstance * inst,
                                                             CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmLogicalLink *pLogLink = NULL;
    CsrBtAmpmGenListMember *pListMember = inst->pLogicalLinkListAnchor->head;
    CsrBool foundBestEffort = FALSE;

    while (FALSE == foundBestEffort && NULL != pListMember)
    {
        pLogLink = (CsrBtAmpmLogicalLink *) pListMember;

        /* A link maybe best effort in only one direction if the other is no traffic.
         * A link can not be best effort in one direction and guaranteed in the other.
         */
        if ((pPhyLink == pLogLink->pPhyLink) &&
            ((CSR_BT_AMP_HCI_SERVICE_TYPE_BEST_EFFORT == pLogLink->rxFlowspec.serviceType) ||
             (CSR_BT_AMP_HCI_SERVICE_TYPE_BEST_EFFORT == pLogLink->txFlowspec.serviceType)) &&
            !csrBtAmpmLogicalIsDisconnecting(pLogLink))
        {
            foundBestEffort = TRUE;
        }
        else
        {
            pListMember = pListMember->next;
            pLogLink = NULL;
        }
    }
    return pLogLink;
}

CsrBool CsrBtAmpmLogicalL2capChanGetByLocalCid(CsrBtAmpmInstance * inst,
                                               CsrBtAmpmLogicalLink ** ppLogLink,
                                               CsrBtAmpmL2cap ** ppL2caChan,
                                               CsrBtConnId cid)
{
    CsrBtAmpmGenListMember *pListMember = inst->pLogicalLinkListAnchor->head;
    CsrBool FoundCid = FALSE;

    CSR_BT_AMPM_ASSERT(ppLogLink != NULL);
    CSR_BT_AMPM_ASSERT(ppL2caChan != NULL);

    *ppLogLink = NULL;
    *ppL2caChan = NULL;

    while (FALSE == FoundCid && NULL != pListMember)
    {
        *ppLogLink = (CsrBtAmpmLogicalLink *) pListMember;
        *ppL2caChan =
            CsrBtAmpmL2capGetByCid(inst,
                                   (*ppLogLink)->pLocalL2caChanListAnchor, cid);

        if (NULL != *ppL2caChan)
        {
            FoundCid = TRUE;
        }
        else
        {
            pListMember = pListMember->next;
        }
    }
    return (FoundCid);
}


CsrBtAmpmLogicalLink *CsrBtAmpmLogicalGetByHandle(CsrBtAmpmInstance * inst,
                                                  CsrBtAmpmLogicalHandle handle)
{
    CsrBtAmpmLogicalLink *pLogLink = NULL;

    /* Find the required element based on the local Cid */
    pLogLink =
        (CsrBtAmpmLogicalLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                                 pLogicalLinkListAnchor, csrBtAmpmLogicalMatchesHandle,
                                                                 (const void *) &handle);
    return (pLogLink);
}


void CsrBtAmpmLogicalInit(CsrBtAmpmInstance * inst)
{
    CSR_BT_AMPM_ASSERT(inst->pLogicalLinkListAnchor == NULL);
    inst->pLogicalLinkListAnchor = CsrBtAmpmListCreateAnchor();
}


void CsrBtAmpmLogicalDeinit(CsrBtAmpmInstance * inst)
{
    if(inst->pLogicalLinkListAnchor != NULL)
    {
        CSR_BT_AMPM_ASSERT(inst->pLogicalLinkListAnchor != NULL);
        while (!CsrBtAmpmListIsEmpty(inst->pLogicalLinkListAnchor))
        {
            /* This function will automatically remove the item from
             * the list, so don't do it here */
            csrBtAmpmLogicalDelete(inst,
                                   (CsrBtAmpmLogicalLink*)inst->pLogicalLinkListAnchor->head);
        }
        CsrBtAmpmListDestroyAnchor(&(inst->pLogicalLinkListAnchor));
    }
}


CsrBool CsrBtAmpmLogicalIsConnected(CsrBtAmpmLogicalLink * pLogLink)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    return (AMPM_LOGICAL_ST_LINK_ESTABLISHED == pLogLink->state ||
            AMPM_LOGICAL_ST_MODIFYING_FLOWSPEC == pLogLink->state);
}


void CsrBtAmpmLogicalGetBandwidthUsage(CsrBtAmpmLogicalLink * pLogLink,
                                       CsrBool * pIsGuaranteed,
                                       CsrUint32 * pBandwidth)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    CSR_BT_AMPM_ASSERT(pIsGuaranteed != NULL);
    CSR_BT_AMPM_ASSERT(pBandwidth != NULL);

    /* Assuming here that the flowspec in the logical link
     * instance reflects the combined flowspecs of all the
     * L2ca channels using it.
     */

    if ((0 < pLogLink->rxFlowspec.maximumSduSize) &&
        (L2CA_FLOWSPEC_MAX_SDU > pLogLink->rxFlowspec.maximumSduSize))
    {
        *pBandwidth = ((pLogLink->rxFlowspec.maximumSduSize * 8) /
                       pLogLink->rxFlowspec.sduInterArrivalTime);
    }
    if ((0 < pLogLink->txFlowspec.maximumSduSize) &&
        (L2CA_FLOWSPEC_MAX_SDU > pLogLink->txFlowspec.maximumSduSize))
    {
        *pBandwidth += ((pLogLink->txFlowspec.maximumSduSize * 8) /
                        pLogLink->txFlowspec.sduInterArrivalTime);
    }

    if ((L2CA_QOS_TYPE_GUARANTEED == pLogLink->rxFlowspec.serviceType) ||
        (L2CA_QOS_TYPE_GUARANTEED == pLogLink->txFlowspec.serviceType))
    {
        *pIsGuaranteed = TRUE;
    }
    else
    {
        *pIsGuaranteed = FALSE;
    }
}


void CsrBtAmpmLogicalGetBandwidthUsageForPhyLink(CsrBtAmpmInstance * inst,
                                                 CsrBtAmpmPhyLink * pPhyLink,
                                                 CsrUint32 * pUnguaranteedbandwidth,
                                                 CsrUint32 * pGuaranteedBandwidth)
{
    CsrBtAmpmLogicalLink *pLogLink = NULL;
    CsrBtAmpmGenListMember *pListMember;

    CSR_BT_AMPM_ASSERT(inst->pLogicalLinkListAnchor != NULL);
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);
    CSR_BT_AMPM_ASSERT(pUnguaranteedbandwidth != NULL);
    CSR_BT_AMPM_ASSERT(pGuaranteedBandwidth != NULL);

    *pGuaranteedBandwidth = 0;
    *pUnguaranteedbandwidth = 0;

    /* Need to iterate over all the logical links using the physical
     * link to determine the total bandwidth use both guaranteed and
     * unguaranteed.
     */
    pListMember = inst->pLogicalLinkListAnchor->head;

    /* Iterate over the list of logical links looking for links that
     * reference the specified physical link.
     */
    while (NULL != pListMember)
    {
        pLogLink = (CsrBtAmpmLogicalLink *) pListMember;
        pListMember = pListMember->next;
        if (pPhyLink == pLogLink->pPhyLink)
        {
            CsrUint32 BandwidthOfLink=0;
            CsrBool BandwidthIsGuaranteed = FALSE;

            CsrBtAmpmLogicalGetBandwidthUsage(pLogLink,
                                              &BandwidthIsGuaranteed, &BandwidthOfLink);
            if (BandwidthIsGuaranteed)
            {
                *pGuaranteedBandwidth += BandwidthOfLink;
            }
            else
            {
                *pUnguaranteedbandwidth += BandwidthOfLink;
            }
        }
    }
}

void CsrBtAmpmLogicalPhyLinkEvent(CsrBtAmpmInstance * inst,
                                  CsrBtAmpmPhyLink * pPhyLink,
                                  fsm_event_t Event)
{
    CsrBtAmpmLogicalLink *pLogLink = NULL;
    CsrBtAmpmGenListMember *pListMember;

    CSR_BT_AMPM_ASSERT(inst->pLogicalLinkListAnchor != NULL);
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    pListMember = inst->pLogicalLinkListAnchor->head;

    /* Iterate over the list of logical links looking for links that
     * reference the specified physical link.
     */
    while (NULL != pListMember)
    {
        pLogLink = (CsrBtAmpmLogicalLink *) pListMember;
        pListMember = pListMember->next;
        if (pPhyLink == pLogLink->pPhyLink)
        {
            csrBtAmpmLogicalFsmRun(inst, pLogLink, Event, NULL);
        }
    }
}


void CsrBtAmpmLogicalCreateChannelReq(CsrBtAmpmInstance * inst,
                                      CsrBtAmpmLogicalLink * pLogLink,
                                      CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    /* Is the link been created or modified? */
    if (!csrBtAmpmLogicalIsDisconnecting(pLogLink))
    {
        if (CsrBtAmpmLogicalIsConnected(pLogLink))
        {
            csrBtAmpmLogicalFsmRun(inst, pLogLink,
                                   AMPM_LOGICAL_EV_MODIFIED_FLOWSPEC_REQ, pL2caChan);
        }
        else if (!csrBtAmpmLogicalIsConnecting(pLogLink))
        {
            if (csrBtAmpmLogicalCanStart(inst, pLogLink))
            {
                csrBtAmpmLogicalFsmRun(inst, pLogLink, AMPM_LOGICAL_EV_CREATE_REQ,
                                       pL2caChan);
            }
            else
            {
                /* Will need to start the link once the other has completed setting up */
                pLogLink->waitingToStart = TRUE;
            }
        }
    }
}


void CsrBtAmpmLogicalDisconnectChannelReq(CsrBtAmpmInstance * inst,
                                          CsrBtAmpmLogicalLink * pLogLink,
                                          CsrBtAmpmL2cap * pL2caChan)
{
    CSR_BT_AMPM_ASSERT(pLogLink != NULL);
    CSR_BT_AMPM_ASSERT(pL2caChan != NULL);

    /* Need to determine if we are disconnecting the logical link or are we going
     * to attempt to modify it.
     */
    if (1 == CsrBtAmpmListCount(pLogLink->pLocalL2caChanListAnchor))
    {
        csrBtAmpmLogicalFsmRun(inst, pLogLink, AMPM_LOGICAL_EV_DISCONNECT_REQ,
                               NULL);
    }
    else
    {
        /* Record the current flow specs incase the modify fails */
        pLogLink->previousTxFlowspec = pLogLink->txFlowspec;
        pLogLink->previousRxFlowspec = pLogLink->rxFlowspec;

        /* Combine in the new L2ca flow specs */
        csrBtAmpmLogicalAggregateBestEffortFlowspecs(pLogLink);

        CsrBtAmpmListRemoveMember(pLogLink->pLocalL2caChanListAnchor,
                                  (CsrBtAmpmGenListMember *) pL2caChan);

        csrBtAmpmLogicalFsmRun(inst, pLogLink,
                               AMPM_LOGICAL_EV_MODIFIED_FLOWSPEC_REQ, pL2caChan);
    }
}

void CsrBtAmpmLogicalHciResponseTimeoutInd(CsrUint16 ignored,
                                           const void *pFsmInstance)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(pFsmInstance != NULL);
    param = (CsrBtAmpmFsmParam *) pFsmInstance;

    pLogLink = (CsrBtAmpmLogicalLink *) CsrBtAmpmListFindMatchingMember(
        param->inst->pLogicalLinkListAnchor,
        CsrBtAmpmListEntryMatchesPtr,
        (const void *) pFsmInstance);

    if (NULL != pLogLink)
    {
        pLogLink->responseTid = 0;

        /* If timers are disabled then ignore it */
        if (((CsrBtAmpmInstance*)(param->inst))->ampmTimersEnabled)
        {
            csrBtAmpmLogicalFsmRun(param->inst, pLogLink,
                                   AMPM_LOGICAL_EV_RESPONSE_TIMEOUT, NULL);
        }
    }
    CsrPmemFree(param);
}

void CsrBtAmpmLogicalHciLogicalLinkComplete(CsrBtAmpmInstance * inst,
                                            CsrBtAmpmPalHciEventReq * pAmpmHciPrim)
{
    CsrBtAmpHciLogicalLinkCompleteEvt *pHciLogicalLinkComplete;
    CsrBtAmpmPhyLink *pPhyLink;

    CSR_BT_AMPM_ASSERT(pAmpmHciPrim != NULL);
    CSR_BT_AMPM_ASSERT(pAmpmHciPrim->data != NULL);

    pHciLogicalLinkComplete =
        (CsrBtAmpHciLogicalLinkCompleteEvt *) (pAmpmHciPrim->data);

    /* The handle is allocated by the PAL so the only thing we have to match the
     * required logical link is the physical link handle and the assumption that
     * only one logical link is requested at a time.
     */
    pPhyLink =
        CsrBtAmpmPhyGetByAmpPhyHandle(inst,
                                      pHciLogicalLinkComplete->physicalLinkHandle);

    if (NULL != pPhyLink)
    {
        CsrBtAmpmGenListMember *pListMember =
            inst->pLogicalLinkListAnchor->head;
        CsrBool foundLogLink = FALSE;

        while (!foundLogLink && NULL != pListMember)
        {
            CsrBtAmpmLogicalLink *pLogLink =
                (CsrBtAmpmLogicalLink *) pListMember;
            if (pPhyLink == pLogLink->pPhyLink &&
                csrBtAmpmLogicalIsConnecting(pLogLink) &&
                pHciLogicalLinkComplete->txFlowSpecId ==
                pLogLink->txFlowspec.id)
            {
                fsm_event_t event = AMPM_LOGICAL_EV_COMPLETE_FAILURE;

                foundLogLink = TRUE;

                if (CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS ==
                    pHciLogicalLinkComplete->status)
                {
                    event = AMPM_LOGICAL_EV_COMPLETE_SUCCESS;
                    pLogLink->handle =
                        pHciLogicalLinkComplete->logicalLinkHandle;
                }
                csrBtAmpmLogicalFsmRun(inst, pLogLink, event, NULL);
            }
            pListMember = pListMember->next;
        }
    }
}


void CsrBtAmpmLogicalHciFlowSpecModifyComplete(CsrBtAmpmInstance * inst,
                                               CsrBtAmpmPalHciEventReq * pAmpmHciPrim)
{
    CsrBtAmpHciFlowSpecModifyCompleteEvt *pHciFlowSpecModifyComplete;
    CsrBtAmpmLogicalLink *pLogLink;

    CSR_BT_AMPM_ASSERT(pAmpmHciPrim != NULL);
    CSR_BT_AMPM_ASSERT(pAmpmHciPrim->data != NULL);

    pHciFlowSpecModifyComplete =
        (CsrBtAmpHciFlowSpecModifyCompleteEvt *) (pAmpmHciPrim->data);

    pLogLink =
        CsrBtAmpmLogicalGetByHandle(inst, pHciFlowSpecModifyComplete->handle);
    if (NULL != pLogLink)
    {
        fsm_event_t event = AMPM_LOGICAL_EV_FLOWSPEC_FAILURE;

        if (CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS ==
            pHciFlowSpecModifyComplete->status)
        {
            event = AMPM_LOGICAL_EV_FLOWSPEC_SUCCESS;
        }
        csrBtAmpmLogicalFsmRun(inst, pLogLink, event, NULL);
    }
}


void CsrBtAmpmLogicalHciDisconnectLogicalLinkComplete(CsrBtAmpmInstance * inst,
                                                      CsrBtAmpmPalHciEventReq * pAmpmHciPrim)
{
    CsrBtAmpHciDisconnectLogicalLinkCompleteEvt
        *pHciDisconnectLogicalLinkComplete;
    CsrBtAmpmLogicalLink *pLogLink;

    CSR_BT_AMPM_ASSERT(pAmpmHciPrim != NULL);
    CSR_BT_AMPM_ASSERT(pAmpmHciPrim->data != NULL);

    pHciDisconnectLogicalLinkComplete =
        (CsrBtAmpHciDisconnectLogicalLinkCompleteEvt*)(pAmpmHciPrim->data);

    pLogLink = CsrBtAmpmLogicalGetByHandle(inst,
                                           pHciDisconnectLogicalLinkComplete->logicalLinkHandle);

    if (NULL != pLogLink)
    {
        fsm_event_t event;

        if((CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS == pHciDisconnectLogicalLinkComplete->status) &&
           (CSR_BT_AMP_HCI_STATUS_CODE_CONNECTION_TIMEOUT == pHciDisconnectLogicalLinkComplete->reason))
        {
            event = AMPM_LOGICAL_EV_DISCONNECT_LSTO;
        }
        else if(CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS == pHciDisconnectLogicalLinkComplete->status)
        {
            event = AMPM_LOGICAL_EV_DISCONNECT_SUCCESS;
        }
        else
        {
            event = AMPM_LOGICAL_EV_DISCONNECT_FAILURE;
        }

        csrBtAmpmLogicalFsmRun(inst, pLogLink, event, NULL);
    }
}


void CsrBtAmpmLogicalHciCommandStatusFailure(CsrBtAmpmInstance * inst,
                                             CsrBtAmpmLogicalLink * pLogLink)
{
    CsrBtAmpmLogicalLink *pActiveLogLink;

    CSR_BT_AMPM_ASSERT(pLogLink != NULL);

    pActiveLogLink =
        (CsrBtAmpmLogicalLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                                 pLogicalLinkListAnchor, CsrBtAmpmListEntryMatchesPtr,
                                                                 (const void *) pLogLink);
    if (NULL != pActiveLogLink)
    {
        csrBtAmpmLogicalFsmRun(inst, pLogLink,
                               AMPM_LOGICAL_EV_COMMAND_FAILURE, NULL);
    }
}


void CsrBtAmpmLogicalHciLogicalLinkCancel(CsrBtAmpmInstance * inst,
                                          CsrBtAmpmLogicalLink * pLogLink,
                                          CsrBtAmpHciLogicalLinkCancelReturn * pReturn)
{
    CsrBtAmpmLogicalLink *pActiveLogLink;
    fsm_event_t this_event = AMPM_LOGICAL_EV_CANCEL_FAILURE;    /* Assume failure */

    CSR_BT_AMPM_ASSERT(pLogLink != NULL);

    pActiveLogLink =
        (CsrBtAmpmLogicalLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                                 pLogicalLinkListAnchor, CsrBtAmpmListEntryMatchesPtr,
                                                                 (const void *) pLogLink);
    if (NULL != pActiveLogLink)
    {
        if (!pReturn->status)
        {
            this_event = AMPM_LOGICAL_EV_CANCEL_SUCCESS;
        }
        csrBtAmpmLogicalFsmRun(inst, pLogLink, this_event, NULL);
    }
}

fsm_event_t CsrBtAmpmLogicalFsmCreateLogicalLink(void *arg1,
                                                 void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmFsmParam *timerParam;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;
    pL2caChan = (CsrBtAmpmL2cap *) arg2;

    /* As we are creating the logical link we need to obtain the initial
     * flowspec from the l2ca channel.
     */
    csrBtAmpmLogicalCpyL2caFlowspec(&(pLogLink->txFlowspec),
                                    &(pL2caChan->txFlowspec));
    csrBtAmpmLogicalCpyL2caFlowspec(&(pLogLink->rxFlowspec),
                                    &(pL2caChan->rxFlowspec));
    /* Need to track which L2ca channel to respond back to as we might
     * have others added whilst we wait for the link to establish.
     */
    pLogLink->pL2caChanTriggedConnection = pL2caChan;

    CsrBtAmpmSendHciCreateLogicalLink(
        param->inst,
        CsrBtAmpmPhyGetLocalAmpId(pLogLink->pPhyLink),
        (void *) pLogLink,
        CsrBtAmpmPhyGetHandle(pLogLink->pPhyLink),
        *((CsrBtAmpAmpFlowSpec *) & (pLogLink->txFlowspec)),
        *((CsrBtAmpAmpFlowSpec *) & (pLogLink->rxFlowspec)));

    timerParam = CsrPmemAlloc(sizeof(CsrBtAmpmFsmParam));
    timerParam->inst = param->inst;
    timerParam->argp = pLogLink;
    pLogLink->responseTid = CsrSchedTimerSet(CSR_BT_AMPM_HCI_RESPONSE_TIMEOUT,
                                             (void (*)(CsrUint16, void *)) CsrBtAmpmLogicalHciResponseTimeoutInd,
                                             0, timerParam);
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLogicalFsmPhysicalEstablishmentFailed(void *arg1,
                                                           void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;
    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));
    csrBtAmpmLogicalReportLinkFailureToL2caChans(param->inst, pLogLink);
    pLogLink->pPhyLink = NULL;

    return AMPM_LOGICAL_EV_DELETE_REQ;
}

fsm_event_t CsrBtAmpmLogicalFsmEstablishmentFailed(void *arg1,
                                                   void *arg2)
{
    CsrBtAmpmPhyLink *pWasUsingPhyLink;
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));
    csrBtAmpmLogicalReportLinkFailureToL2caChans(param->inst, pLogLink);
    csrBtAmpmLogicalStartWaitingLink(param->inst, pLogLink);
    pWasUsingPhyLink = pLogLink->pPhyLink;
    pLogLink->pPhyLink = NULL;
    CsrBtAmpmPhyTriggerDisconnectIfRequired(param->inst, pWasUsingPhyLink);

    return AMPM_LOGICAL_EV_DELETE_REQ;
}

fsm_event_t CsrBtAmpmLogicalFsmCancelLogicalLink(void *arg1,
                                                 void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *timerParam;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    if (!pLogLink->alreadyAttemptedCancel)
    {
        csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));

        CsrBtAmpmSendHciLogicalLinkCancel(
            param->inst,
            CsrBtAmpmPhyGetLocalAmpId(pLogLink->pPhyLink),
            (void *) pLogLink,
            CsrBtAmpmPhyGetHandle(pLogLink->pPhyLink),
            pLogLink->txFlowspec.id);

        timerParam = CsrPmemAlloc(sizeof(CsrBtAmpmFsmParam));
        timerParam->inst = param->inst;
        timerParam->argp = pLogLink;
        pLogLink->responseTid = CsrSchedTimerSet(CSR_BT_AMPM_HCI_RESPONSE_TIMEOUT,
                                                 (void (*)(CsrUint16, void *)) CsrBtAmpmLogicalHciResponseTimeoutInd,
                                                 0, timerParam);
    }
    csrBtAmpmLogicalStartWaitingLink(param->inst, pLogLink);
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLogicalFsmDisconnectLogicalLink(void *arg1,
                                                     void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *timerParam;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));

    CsrBtAmpmSendHciDisconnectLogicalLink(
        param->inst,
        CsrBtAmpmPhyGetLocalAmpId(pLogLink->pPhyLink),
        (void *) pLogLink, pLogLink->handle);

    timerParam = CsrPmemAlloc(sizeof(CsrBtAmpmFsmParam));
    timerParam->inst = param->inst;
    timerParam->argp = pLogLink;
    pLogLink->responseTid = CsrSchedTimerSet(CSR_BT_AMPM_HCI_RESPONSE_TIMEOUT,
                                             (void (*)(CsrUint16, void *)) CsrBtAmpmLogicalHciResponseTimeoutInd,
                                             0, timerParam);
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLogicalFsmLinkEstablished(void *arg1,
                                               void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;
    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));

    CsrBtAmpmL2capChannelReadyInd(param->inst,
                                  pLogLink->pL2caChanTriggedConnection);
    pLogLink->pL2caChanTriggedConnection = NULL;
    csrBtAmpmLogicalStartWaitingLink(param->inst, pLogLink);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLogicalFsmModifyFlowspecReq(void *arg1,
                                                 void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmL2cap *pL2caChan;
    CsrBtAmpmFsmParam *timerParam;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;
    pL2caChan = (CsrBtAmpmL2cap *) arg2;

    /* Track which L2CA Channel instance is triggering the flow spec
     * modification for the logical link so that we can send back the
     * result to the correct instance.
     */
    pLogLink->pL2caChanTriggedModify = pL2caChan;

    /* Record the current flow specs incase the modify fails */
    pLogLink->previousTxFlowspec = pLogLink->txFlowspec;
    pLogLink->previousRxFlowspec = pLogLink->rxFlowspec;

    /* Combine in the new L2ca flow specs */
    csrBtAmpmLogicalAggregateBestEffortFlowspecs(pLogLink);

    CsrBtAmpmSendHciFlowSpecModify(
        param->inst,
        CsrBtAmpmPhyGetLocalAmpId(pLogLink->pPhyLink),
        (void *) pLogLink, pLogLink->handle,
        *((CsrBtAmpAmpFlowSpec *) & (pLogLink->txFlowspec)),
        *((CsrBtAmpAmpFlowSpec *) & (pLogLink->rxFlowspec)));

    timerParam = CsrPmemAlloc(sizeof(CsrBtAmpmFsmParam));
    timerParam->inst = param->inst;
    timerParam->argp = pLogLink;
    pLogLink->responseTid = CsrSchedTimerSet(CSR_BT_AMPM_HCI_RESPONSE_TIMEOUT,
                                             (void (*)(CsrUint16, void *)) CsrBtAmpmLogicalHciResponseTimeoutInd,
                                             0, timerParam);
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLogicalFsmPhyLinkDisconnectInd(void *arg1,
                                                    void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));
    csrBtAmpmLogicalReportLinkFailureToL2caChans(param->inst, pLogLink);
    pLogLink->pPhyLink = NULL;
    return AMPM_LOGICAL_EV_DELETE_REQ;
}


fsm_event_t CsrBtAmpmLogicalFsmLinkFailure(void *arg1,
                                           void *arg2)
{
    CsrBtAmpmPhyLink *pWasUsingPhyLink;
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));
    csrBtAmpmLogicalReportLinkFailureToL2caChans(param->inst, pLogLink);
    pWasUsingPhyLink = pLogLink->pPhyLink;
    pLogLink->pPhyLink = NULL;
    CsrBtAmpmPhyTriggerDisconnectIfRequired(param->inst, pWasUsingPhyLink);

    return AMPM_LOGICAL_EV_DELETE_REQ;
}


fsm_event_t CsrBtAmpmLogicalFsmLinkFailureLsto(void *arg1,
                                               void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));
    csrBtAmpmLogicalReportLinkFailureToL2caChans(param->inst, pLogLink);

    /* Link drop due to link supervision timeouts does not provoke the
     * A2MP physical disconnect procedure */
    pLogLink->pPhyLink = NULL;

    return AMPM_LOGICAL_EV_DELETE_REQ;
}


fsm_event_t CsrBtAmpmLogicalFsmLinkDisconnected(void *arg1,
                                                void *arg2)
{
    CsrBtAmpmPhyLink *pWasUsingPhyLink;
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));

    csrBtAmpmLogicalReportLinkDisconnectToL2caChans(param->inst, pLogLink);
    pWasUsingPhyLink = pLogLink->pPhyLink;
    pLogLink->pPhyLink = NULL;
    CsrBtAmpmPhyTriggerDisconnectIfRequired(param->inst, pWasUsingPhyLink);

    return AMPM_LOGICAL_EV_DELETE_REQ;
}

/* Do we actually do something different to the successful case or not? */
fsm_event_t CsrBtAmpmLogicalFsmLinkDisconnectFailed(void *arg1,
                                                    void *arg2)
{
    CsrBtAmpmPhyLink *pWasUsingPhyLink;
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));
    csrBtAmpmLogicalReportLinkDisconnectToL2caChans(param->inst, pLogLink);
    pWasUsingPhyLink = pLogLink->pPhyLink;
    pLogLink->pPhyLink = NULL;
    CsrBtAmpmPhyTriggerDisconnectIfRequired(param->inst, pWasUsingPhyLink);

    /* Do we need to reset the pal or anything else? We can't,
     * really. Just try again */
    return AMPM_LOGICAL_EV_DELETE_REQ;
}

fsm_event_t CsrBtAmpmLogicalFsmLinkDisconnectTimeout(void *arg1,
                                                     void *arg2)
{
    CsrBtAmpmPhyLink *pWasUsingPhyLink;
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));
    csrBtAmpmLogicalReportLinkDisconnectToL2caChans(param->inst, pLogLink);
    pWasUsingPhyLink = pLogLink->pPhyLink;
    pLogLink->pPhyLink = NULL;
    CsrBtAmpmPhyTriggerDisconnectIfRequired(param->inst, pWasUsingPhyLink);

    return AMPM_LOGICAL_EV_DELETE_REQ;
}


fsm_event_t CsrBtAmpmLogicalFsmDeleteReq(void *arg1,
                                         void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));
    csrBtAmpmLogicalDelete(param->inst, pLogLink);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLogicalFsmModifySuccess(void *arg1,
                                             void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));

    CsrBtAmpmL2capLinkModifiedSuccessInd(param->inst,
                                         pLogLink->pL2caChanTriggedModify);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLogicalFsmModifyFailed(void *arg1,
                                            void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    csrBtAmpmLogicalCancelTimer(&(pLogLink->responseTid));

    CsrBtAmpmL2capLinkModifiedFailureInd(param->inst,
                                         pLogLink->pL2caChanTriggedModify);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLogicalFsmCancelFailed(void *arg1,
                                            void *arg2)
{
    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    /* This only happens if the logical link actually establishes correctly
     * before the cancel can take place, so the link is established and we
     * need to disconnect it instead now.
     */

    return AMPM_LOGICAL_EV_DISCONNECT_REQ;
}

fsm_event_t CsrBtAmpmLogicalFsmDisconnectUnstartedLink(void *arg1,
                                                       void *arg2)
{
    CsrBtAmpmLogicalLink *pLogLink;
    CsrBtAmpmPhyLink *pWasUsingPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLogLink = (CsrBtAmpmLogicalLink *) param->argp;

    /* Have not even started the connection attempt */
    csrBtAmpmLogicalReportLinkDisconnectToL2caChans(param->inst, pLogLink);
    pWasUsingPhyLink = pLogLink->pPhyLink;
    pLogLink->pPhyLink = NULL;
    CsrBtAmpmPhyTriggerDisconnectIfRequired(param->inst, pWasUsingPhyLink);

    return AMPM_LOGICAL_EV_DELETE_REQ;
}
