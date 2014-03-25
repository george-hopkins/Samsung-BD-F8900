/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

#define csrBtAmpmPhyStartHciTimerDebug(inst, phylink)   \
    CSR_BT_AMPM_ASSERT(phylink->hciResponseTid == 0);   \
    csrBtAmpmPhyStartHciTimer((inst), (phylink))


/* Collision backoff algorithm for BT address */
static CsrBool csrBtAmpmPhyAddressCollision(BD_ADDR_T *me,
                                            BD_ADDR_T *her)
{

    /* Compare each byte from the LSB until we find the device that
     * has a higher byte. Then that device wins */
    if (her->lap != me->lap)
    {
        if ((her->lap & 0x000000FF) < (me->lap & 0x000000FF) ||
            (her->lap & 0x0000FF00) < (me->lap & 0x0000FF00) ||
            (her->lap & 0x00FF0000) < (me->lap & 0x00FF0000))
        {
            return TRUE;
        }
    }
    else if (her->uap < me->uap)
    {
        return TRUE;
    }
    else if ((her->nap & 0x00FF) < (me->nap & 0x00FF) ||
             (her->nap & 0xFF00) < (me->nap & 0xFF00))
    {
        return TRUE;
    }
    else
    {
        /* Addresses are the same! */
        CSR_BT_AMPM_ASSERT(FALSE);
    }
    return FALSE;
}

/* Allocates a new AMP Physical Link Handle. */
static CsrUint8 csrBtAmpmPhyAllocHandle(
    CsrBtAmpmInstance * inst)
{
    CsrUint8 handleIndex = 1;
    CsrUint8 allocatedHandle = 0;

    while (0 == allocatedHandle &&
           handleIndex < CSR_BT_AMPM_MAX_PHYSICAL_LINK_HANDLE)
    {
        if ((inst->lastAllocatedAmpPhysicalHandle + handleIndex) <
            CSR_BT_AMPM_MAX_PHYSICAL_LINK_HANDLE &&
            !inst->phyHandleInUse[(inst->lastAllocatedAmpPhysicalHandle +
                                   handleIndex)])
        {
            inst->phyHandleInUse[(inst->lastAllocatedAmpPhysicalHandle +
                                  handleIndex)] = TRUE;
            allocatedHandle =
                (inst->lastAllocatedAmpPhysicalHandle + handleIndex);
        }
        else
        {
            if (handleIndex < inst->lastAllocatedAmpPhysicalHandle &&
                !inst->phyHandleInUse[handleIndex])
            {
                inst->phyHandleInUse[handleIndex] = TRUE;
                allocatedHandle = handleIndex;
            }
        }
    }
    inst->lastAllocatedAmpPhysicalHandle = allocatedHandle;
    return (allocatedHandle);
}

/* Creates a new phylink instance */
static CsrBtAmpmPhyLink *csrBtAmpmPhyNew(CsrBtAmpmInstance * inst,
                                         BD_ADDR_T bdAddr,
                                         CsrBool initiator)
{
    CsrBtAmpmPhyLink *pPhyLink = NULL;

    CSR_BT_AMPM_ASSERT(inst->pPhyLinkListAnchor != NULL);

    pPhyLink = (CsrBtAmpmPhyLink *) CsrPmemZalloc(sizeof(*pPhyLink));
    pPhyLink->state = AMPM_PHY_ST_NULL;
    pPhyLink->ampPLinkHandle = csrBtAmpmPhyAllocHandle(inst);
    pPhyLink->initiator = initiator;
    pPhyLink->bdAddr = bdAddr;
    pPhyLink->linkSupervisionTimeout = CSR_BT_AMP_LINK_SUPERVISION_TIMEOUT_AMP_DEFAULT;
    CsrBtAmpmListAddHead(inst->pPhyLinkListAnchor,
                         (CsrBtAmpmGenListMember *) pPhyLink);

    return pPhyLink;
}

/* Runs the FSM associated with the specified physical link
 * instance */
static void csrBtAmpmPhyFsmRun(CsrBtAmpmInstance * inst,
                               CsrBtAmpmPhyLink * pPhyLink,
                               fsm_event_t event,
                               void *param2)
{
    fsm_result_t fsm_result;
    CsrBtAmpmFsmParam param;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    param.inst = inst;
    param.argp = pPhyLink;

    fsm_result =
        fsm_16bit_run(&csr_bt_ampm_phy_fsm, &(pPhyLink->state), event, &param,
                      param2);

    CSR_BT_AMPM_ASSERT(FSM_RESULT_OK == fsm_result);

    /* Depending on the configuration, CSR_BT_AMPM_ASSERT may be invalid and thus fsm_result is unsed */
    CSR_UNUSED(fsm_result);
}

/* Performs the checking of a generic list element against the
 * supplied BT Device address and returns TRUE if they match. */
static CsrBool csrBtAmpmPhyMatchesBtAddr(
    const void *pCriteria,
    const void *pListEntry)
{
    BD_ADDR_T bdAddr;
    const CsrBtAmpmPhyLink *pPhyLink;
    CsrBool doesEntryMatch = FALSE;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    bdAddr = *((const BD_ADDR_T *) pCriteria);
    pPhyLink = (CsrBtAmpmPhyLink *) pListEntry;

    doesEntryMatch =
        bd_addr_eq((const BD_ADDR_T *) &(pPhyLink->bdAddr),
                   (const BD_ADDR_T *) &bdAddr);
    return (doesEntryMatch);
}

/* Performs the checking of a generic list element against the
 * supplied BT Device address and Local AMP ID and returns TRUE if
 * they match. */
static CsrBool csrBtAmpmPhyMatchesBtAddandLocalAmpId(
    const void *pCriteria,
    const void *pListEntry)
{
    const CsrBtAmpmPhySearchParams *pSearchParams;
    const CsrBtAmpmPhyLink *pPhyLink;
    CsrBool doesEntryMatch = FALSE;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    pSearchParams = ((CsrBtAmpmPhySearchParams *) pCriteria);
    pPhyLink = (CsrBtAmpmPhyLink *) pListEntry;

    if ((bd_addr_eq((const BD_ADDR_T *) &pPhyLink->bdAddr,
                    (const BD_ADDR_T *) &pSearchParams->bd_addr)) &&
        (CsrBtAmpmPhyGetLocalAmpId(pPhyLink) == pSearchParams->amp_id))
    {
        doesEntryMatch = TRUE;
    }
    return (doesEntryMatch);
}

/* Performs the checking of a generic list element against the
 * supplied Local AMP ID and returns TRUE if they match. */
static CsrBool csrBtAmpmPhyMatchesLocalAmpId(
    const void *pCriteria,
    const void *pListEntry)
{
    const CsrBtAmpmId *pId;
    const CsrBtAmpmPhyLink *pPhyLink;
    CsrBool doesEntryMatch = FALSE;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    pId = (const CsrBtAmpmId *) pCriteria;
    pPhyLink = (const CsrBtAmpmPhyLink *) pListEntry;

    if (CsrBtAmpmPhyGetLocalAmpId(pPhyLink) == *pId)
    {
        doesEntryMatch = TRUE;
    }
    return (doesEntryMatch);
}

/* Performs the checking of a generic list element against the
 * supplied Remote AMP ID and returns TRUE if they match. */
static CsrBool csrBtAmpmPhyMatchesRemoteAmpId(
    const void *pCriteria,
    const void *pListEntry)
{
    const CsrBtAmpmId *pId;
    const CsrBtAmpmPhyLink *pPhyLink;
    CsrBool doesEntryMatch = FALSE;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    pId = (const CsrBtAmpmId *) pCriteria;
    pPhyLink = (CsrBtAmpmPhyLink *) pListEntry;

    if (NULL != pPhyLink->pRemoteAMPInfo)
    {
        if (CsrBtAmpmPhyGetRemoteAmpId(pPhyLink) == *pId)
        {
            doesEntryMatch = TRUE;
        }
    }

    return (doesEntryMatch);
}


/* Performs the checking of a generic list element against the
 * supplied cid and Sdu Identifier and returns TRUE if they match. */
static CsrBool csrBtAmpmPhyMatchesCIDandSduIdentifier(
    const void *pCriteria,
    const void *pListEntry)
{
    const CsrBtAmpmPhySearchParams *pSearchParams;
    const CsrBtAmpmPhyLink *pPhyLink;
    CsrBool doesEntryMatch = FALSE;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    pSearchParams = ((CsrBtAmpmPhySearchParams *) pCriteria);
    pPhyLink = (CsrBtAmpmPhyLink *) pListEntry;

    if ((pPhyLink->pBtDevice->cid == pSearchParams->cid) &&
        (pPhyLink->sduIdentifier == pSearchParams->sdu_identifier))
    {
        doesEntryMatch = TRUE;
    }
    return (doesEntryMatch);
}

/* Performs the checking of a generic list element against the
 * supplied AMP Physical Link Handle and returns TRUE if they
 * match. */
static CsrBool csrBtAmpmPhyMatchesAmpPhyHandle(
    const void *pCriteria,
    const void *pListEntry)
{
    CsrUint8 amp_phy_handle;
    const CsrBtAmpmPhyLink *pPhyLink;
    CsrBool doesEntryMatch = FALSE;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    amp_phy_handle = *((const CsrUint8 *) pCriteria);
    pPhyLink = (const CsrBtAmpmPhyLink *) pListEntry;

    if (pPhyLink->ampPLinkHandle == amp_phy_handle)
    {
        doesEntryMatch = TRUE;
    }
    return (doesEntryMatch);
}

/* Starts the AMP Manager SDU timer for the specified physical link */
static void csrBtAmpmPhyStartSduTimer(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    param = CsrPmemAlloc(sizeof(CsrBtAmpmFsmParam));
    param->inst = inst;
    param->argp = pPhyLink;

    pPhyLink->sduResponseTid = CsrSchedTimerSet(CSR_BT_AMPM_SDU_REQUEST_RESPONSE_TIMEOUT,
                                               (void (*)(CsrUint16, void *)) CsrBtAmpmPhySduResponseTimeoutInd,
                                               0,
                                               param);

}

/* Starts the HCI timer for the specified physical link */
static void csrBtAmpmPhyStartHciTimer(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    param = CsrPmemAlloc(sizeof(CsrBtAmpmFsmParam));
    param->inst = inst;
    param->argp = pPhyLink;

    pPhyLink->hciResponseTid = CsrSchedTimerSet(CSR_BT_AMPM_HCI_RESPONSE_TIMEOUT,
                                               (void (*)(CsrUint16, void *)) CsrBtAmpmPhyHciResponseTimeoutInd,
                                               0,
                                               param);
}

/* Cancels the SDU timer asscoaied with the specified physical link
 * instance */
static void csrBtAmpmPhyCancelSduTimer(
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmFsmParam *param;
    CsrUint16 mi;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    if (0 != pPhyLink->sduResponseTid)
    {
        (void) CsrSchedTimerCancel(pPhyLink->sduResponseTid, &mi, (void **) &param);
        pPhyLink->sduResponseTid = 0;
        CsrPmemFree(param);
    }
}

/* Cancels the HCI timer associated with the specified physical link
 * instance */
static void csrBtAmpmPhyCancelHciTimer(
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmFsmParam *param;
    CsrUint16 mi;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    if (0 != pPhyLink->hciResponseTid)
    {
        (void) CsrSchedTimerCancel(pPhyLink->hciResponseTid, &mi, (void **) &param);
        pPhyLink->hciResponseTid = 0;
        CsrPmemFree(param);
    }
}

/* Removes FSM Instance from the list and frees the memmory */
static void csrBtAmpmPhyRemoveFsmInstance(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CSR_BT_AMPM_ASSERT(inst != NULL);
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    (void) CsrBtAmpmListRemoveMatchingMember(inst->pPhyLinkListAnchor,
                                             CsrBtAmpmListEntryMatchesPtr, pPhyLink);

    CsrBtAmpmLocalRemoveHciTxCmds(inst, CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                                  pPhyLink);

    if (NULL != pPhyLink->pRemoteAMPInfo)
    {
        if (NULL != pPhyLink->pRemoteAMPInfo->pAMPAssoc)
        {
            CsrPmemFree(pPhyLink->pRemoteAMPInfo->pAMPAssoc->data);
            CsrPmemFree(pPhyLink->pRemoteAMPInfo->pAMPAssoc);
        }
        CsrPmemFree(pPhyLink->pRemoteAMPInfo);
    }

    inst->phyHandleInUse[pPhyLink->ampPLinkHandle] = FALSE;
    CsrPmemFree(pPhyLink);
}

/* Generates an appropriate AMP DISCONNECT PHYSICAL LINK REQUEST
 * SDU */
static void csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequest(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrMblk *pMsgBlk;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    pPhyLink->sduIdentifier = CsrBtAmpmSduAllocIdentifier(inst);
    pPhyLink->expectedResponseSdu =
        CSR_BT_AMPM_DISCONNECT_PHYSICAL_LINK_RESPONSE;

    pMsgBlk =
        CsrBtAmpmSduCreateAmpDisconnectPhysicalLinkRequest(pPhyLink->
                                                           sduIdentifier, CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                                                           CsrBtAmpmPhyGetRemoteAmpId(pPhyLink));
    L2CA_DataWriteReq((l2ca_cid_t)pPhyLink->pBtDevice->cid, 0, pMsgBlk);
}


static void csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequestWithInvalidAmpId(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrMblk *pMsgBlk;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    pMsgBlk = CsrBtAmpmSduCreateAmpDisconnectPhysicalLinkRequest(
        CsrBtAmpmSduAllocIdentifier(inst),
        CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
        (CsrBtAmpmId) (CsrBtAmpmPhyGetRemoteAmpId(pPhyLink) + 1));
    L2CA_DataWriteReq((l2ca_cid_t)pPhyLink->pBtDevice->cid, 0, pMsgBlk);
}

/* Generates an appropriate AMP DISCONNECT PHYSICAL LINK RESPONSE
 * SDU */
static void csrBtAmpmPhySendAmpDisconnectPhysicalLinkResponse(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrMblk *pMsgBlk;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    pMsgBlk =
        CsrBtAmpmSduCreateAmpDisconnectPhysicalLinkResponse(pPhyLink->
                                                            sduIdentifier, CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                                                            CsrBtAmpmPhyGetRemoteAmpId(pPhyLink),
                                                            CSR_BT_AMPM_DISCONNECT_LINK_SUCCESSFUL);
    L2CA_DataWriteReq((l2ca_cid_t)pPhyLink->pBtDevice->cid, 0, pMsgBlk);
}

/* Returns TRUE if the physical link establishment has already
 * started */
static CsrBool csrBtAmpmPhyStartedPhysicalLinkCreation(
    const CsrBtAmpmPhyLink * pPhyLink)
{
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);
    return (pPhyLink->startedLinkCreation);
}

void CsrBtAmpmPhySetDontStartPhyLink(
    CsrBtAmpmInstance *inst,
    CsrBtAmpmTestDontStartPhyLinkReq *prim)
{
    inst->dontStartPhyLinkEnabled = prim->enabled;
    inst->dontStartPhyLinkType = prim->ampType;
}

void CsrBtAmpmPhySetPhyCollision(
    CsrBtAmpmInstance *inst,
    CsrBtAmpmTestPhyCollisionReq *prim)
{
    inst->phyLinkCollisionEnabled = prim->enabled;
}

CsrBool CsrBtAmpmPhyCollisionRequired(
    CsrBtAmpmInstance *inst)
{
    return inst->phyLinkCollisionEnabled;
}

CsrBool CsrBtAmpmPhyDontStartPhysLink(
    CsrBtAmpmInstance *inst,
    CsrBtAmpmPhyLink *phylink)
{
    if((inst->dontStartPhyLinkEnabled) &&
       (inst->dontStartPhyLinkType == phylink->pLocalAMPInfo->pAmpInfo->type))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

CsrUint8 CsrBtAmpmPhyGetHandle(
    const CsrBtAmpmPhyLink * pPhyLink)
{
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);
    return (pPhyLink->ampPLinkHandle);
}


CsrBtAmpmId CsrBtAmpmPhyGetLocalAmpId(
    const CsrBtAmpmPhyLink * pPhyLink)
{
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);
    /* the local amp info should always be present */
    CSR_BT_AMPM_ASSERT(pPhyLink->pLocalAMPInfo != NULL);

    return (CsrBtAmpmLocalGetId(pPhyLink->pLocalAMPInfo));
}

CsrBtAmpmId CsrBtAmpmPhyGetRemoteAmpId(
    const CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmId remoteAmpId = 0;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    /* We might still be waiting to determine if there is a suitable AMP at
     * the remote device to match our local device.
     */
    if (NULL != pPhyLink->pRemoteAMPInfo)
    {
        remoteAmpId = pPhyLink->pRemoteAMPInfo->id;
    }
    return (remoteAmpId);
}


CsrBool CsrBtAmpmPhyIsConnected(
    const CsrBtAmpmPhyLink * pPhyLink)
{
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);
    return (pPhyLink->connected && !CsrBtAmpmPhyIsDisconnecting(pPhyLink));
}


CsrBool CsrBtAmpmPhyIsDisconnecting(
    const CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBool Disconnecting = FALSE;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    if (AMPM_PHY_ST_DISCONNECT_PENDING == pPhyLink->state)
    {
        Disconnecting = TRUE;
    }
    return (Disconnecting);
}


CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByBdaddr(
    CsrBtAmpmInstance * inst,
    BD_ADDR_T bdAddr)
{
    CsrBtAmpmPhyLink *pPhyLink = NULL;

    /* Find the required element based on the BT address */
    pPhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                             pPhyLinkListAnchor, csrBtAmpmPhyMatchesBtAddr,
                                                             (const void *) &bdAddr);

    return (pPhyLink);
}


CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByBdaddrAndLocalAmpId(
    CsrBtAmpmInstance * inst,
    BD_ADDR_T bdAddr,
    CsrBtAmpmId id)
{
    CsrBtAmpmPhyLink *pPhyLink = NULL;
    CsrBtAmpmPhySearchParams searchParams;

    /* Filling Search Params to search list for physical link instance */
    searchParams.bd_addr = bdAddr;
    searchParams.amp_id = id;

    /* Find the required element based on the connection id */
    pPhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                             pPhyLinkListAnchor, csrBtAmpmPhyMatchesBtAddandLocalAmpId,
                                                             (const void *) &searchParams);

    return (pPhyLink);
}


CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByLocalAmpId(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmId id)
{
    CsrBtAmpmPhyLink *pPhyLink = NULL;

    pPhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                             pPhyLinkListAnchor, csrBtAmpmPhyMatchesLocalAmpId,
                                                             (const void *) &id);

    return (pPhyLink);
}


CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByRemoteAmpId(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmId id)
{
    CsrBtAmpmPhyLink *pPhyLink = NULL;

    /* Find the required element based on the connection id */
    pPhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                             pPhyLinkListAnchor, csrBtAmpmPhyMatchesRemoteAmpId,
                                                             (const void *) &id);
    return (pPhyLink);
}


CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByBdaddrAndSduIdentifier(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhySearchParams * pSearchParams)
{
    CsrBtAmpmPhyLink *pPhyLink = NULL;

    /* Find the required element based on the connection id */
    pPhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                             pPhyLinkListAnchor, csrBtAmpmPhyMatchesCIDandSduIdentifier,
                                                             (const void *) pSearchParams);
    return (pPhyLink);
}


CsrBtAmpmPhyLink *CsrBtAmpmPhyGetByAmpPhyHandle(
    CsrBtAmpmInstance * inst,
    CsrSchedQid amp_phy_handle)
{
    CsrBtAmpmPhyLink *pPhyLink;

    pPhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                             pPhyLinkListAnchor, csrBtAmpmPhyMatchesAmpPhyHandle,
                                                             (const void *) &amp_phy_handle);
    return (pPhyLink);
}


CsrBool CsrBtAmpmPhyInstanceExists(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmPhyLink *pActivePhyLink;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    pActivePhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                             pPhyLinkListAnchor, CsrBtAmpmListEntryMatchesPtr,
                                                             (const void *) pPhyLink);
    if (NULL != pActivePhyLink)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

void CsrBtAmpmPhyInit(CsrBtAmpmInstance * inst)
{
    CSR_BT_AMPM_ASSERT(inst->pPhyLinkListAnchor == NULL);
    inst->pPhyLinkListAnchor = CsrBtAmpmListCreateAnchor();
}

void CsrBtAmpmPhyDeinit(CsrBtAmpmInstance *inst)
{
    if(inst->pPhyLinkListAnchor != NULL)
    {
        while (!CsrBtAmpmListIsEmpty(inst->pPhyLinkListAnchor))
        {
            /* This function will automatically remove the item from
             * the list, so don't do it here */
            csrBtAmpmPhyRemoveFsmInstance(inst,
                                          (CsrBtAmpmPhyLink*)inst->pPhyLinkListAnchor->head);
        }
        CsrBtAmpmListDestroyAnchor(&(inst->pPhyLinkListAnchor));
    }
}

CsrBtAmpmPhyLink *CsrBtAmpmPhyGetPhyForAmps(CsrBtAmpmInstance * inst,
                                            CsrBtAmpmId localAmpId,
                                            CsrBtAmpmId remoteAmpId,
                                            BD_ADDR_T bdAddr)
{
    CsrBtAmpmPhyLink *pPhyLink;

    /* We might need to trigger AMP Discovery to the peer BT device
     * so create the physical link instance first and then check
     * for the Bt device and if not present then trigger the discovery
     */
    pPhyLink = CsrBtAmpmPhyGetByBdaddrAndLocalAmpId(inst, bdAddr, localAmpId);
    if (NULL == pPhyLink)
    {
        CsrBtAmpmLocalAmp *pLocalAmpInfo = CsrBtAmpmLocalGetById(inst, localAmpId);

        if (NULL != pLocalAmpInfo)
        {
            CsrBtAmpmBtdev *pBtDevice = CsrBtAmpmBtdevGetByBdaddr(inst, bdAddr);
            
            /* Create New Physical Link Instance */
            pPhyLink = csrBtAmpmPhyNew(inst, bdAddr, TRUE);

            /* Reference Local AMP Info in Physical Link Instance */
            pPhyLink->pLocalAMPInfo = pLocalAmpInfo;

            /* Based on the supplied AMP ids we know */
            pPhyLink->pRemoteAMPInfo =
                (CsrBtAmpmDataInfo *) CsrPmemZalloc(sizeof(*(pPhyLink->pRemoteAMPInfo)));
            pPhyLink->pRemoteAMPInfo->id = remoteAmpId;
            
            if (NULL != pBtDevice)
            {
                CsrBtAmpmBtdevRemoteAmpInfo *pRemoteAmpInfo =
                    CsrBtAmpmBtdevRemoteAmpInfoGetById(pBtDevice,
                                                       remoteAmpId);

                if (NULL != pRemoteAmpInfo && pRemoteAmpInfo->infoValid)
                {
                    /* Copy Remote AMP Info in Physical Link Instance */
                    pPhyLink->pRemoteAMPInfo->type = pRemoteAmpInfo->info.type;

                    pPhyLink->pRemoteAMPInfo->pAMPAssoc =
                        (CsrBtAmpmAmpAssoc *)CsrPmemZalloc(sizeof(*(pPhyLink->pRemoteAMPInfo->pAMPAssoc)));
                    pPhyLink->pRemoteAMPInfo->pAMPAssoc->data =
                        CsrPmemZalloc(pRemoteAmpInfo->info.maxHciAssocSize);

                    /* Reference Bt Device in Physical Link Instance */
                    pPhyLink->pBtDevice = pBtDevice;

                    if((CSR_BT_AMP_AMP_STATUS_AVAILABLE_BUT_PHYSICALLY_POWERED_DOWN == pLocalAmpInfo->pAmpInfo->status)
                        && (CSR_SCHED_QID_INVALID != inst->ampPowerOnQid))
                    {
                        csrBtAmpmPhyFsmRun(inst, pPhyLink,
                                                   AMPM_PHY_EV_POWER_ON_IND, NULL);
                    }
                    else
                    {

                        csrBtAmpmPhyFsmRun(inst, pPhyLink,
                                            AMPM_PHY_EV_CONNECT_REQ, NULL);
                    }
                }
                else
                {
                    /* Between the previous discovery and now the remote AMP may have
                     * changed state making it unusable.
                     */
                    csrBtAmpmPhyRemoveFsmInstance(inst, pPhyLink);
                    pPhyLink = NULL;
                }
            }
            else
            {
                /* The ACL might have closed so trigger discovery */
                csrBtAmpmPhyFsmRun(inst, pPhyLink, AMPM_PHY_EV_DISCOVERY_REQ,
                                   (void *) &(bdAddr));
            }
        }
    }
    return (pPhyLink);
}


void CsrBtAmpmPhyDisconnect(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    /* Not going to check that the pointer matches a physical link
     * instance in the list as we are assuming that this function
     * is called from somewhere where we know the pointer to be a
     * valid physical link.
     */
    csrBtAmpmPhyFsmRun(inst, pPhyLink, AMPM_PHY_EV_DISCONNECT_REQ, NULL);
}


void CsrBtAmpmPhyTriggerDisconnectIfRequired(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    /* Does the physical link need to be disconnected or not? */
    if (NULL == pPhyLink->pBtDevice || ((pPhyLink->connected ||
                                         pPhyLink->startedLinkCreation) &&
                                        (0 == CsrBtAmpmLogicalLinksUsingPhyLink(inst, pPhyLink) &&
                                         (0 == CsrBtAmpmL2capL2caChansUsingPhyLink(inst, pPhyLink) ||
                                          CsrBtAmpmL2capUsingPhyLinkAreAllDisconnecting(inst,
                                                                                        pPhyLink)))))

    {
        CsrBtAmpmPhyDisconnect(inst, pPhyLink);
    }
}


void CsrBtAmpmPhyLinkCommandStatusSuccess(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmPhyLink *pActivePhyLink;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    pActivePhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                             pPhyLinkListAnchor, CsrBtAmpmListEntryMatchesPtr,
                                                             (const void *) pPhyLink);
    if (NULL != pActivePhyLink)
    {
        csrBtAmpmPhyFsmRun(inst, pActivePhyLink,
                           AMPM_PHY_EV_HCI_PHYSICAL_SUCCESS, NULL);
    }
}


void CsrBtAmpmPhyLinkCommandStatusNeg(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink)
{
    CsrBtAmpmPhyLink *pActivePhyLink;

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    pActivePhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                             pPhyLinkListAnchor, CsrBtAmpmListEntryMatchesPtr,
                                                             (const void *) pPhyLink);
    if (NULL != pActivePhyLink)
    {
        /* Haven't actually started the physical link creation so no
         * disconnection is required.
         */
        pPhyLink->hciDisconnectNeeded = FALSE;
        csrBtAmpmPhyFsmRun(inst, pActivePhyLink,
                           AMPM_PHY_EV_HCI_PHYSICAL_FAILURE, NULL);
    }
}


void CsrBtAmpmPhyHciReadSupervisionTimeout(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPhyLink * pPhyLink,
    CsrBtAmpHciReadLinkSupervisionTimeoutReturn * pReturn)
{
    CsrBtAmpmPhyLink *pActivePhyLink;
    fsm_event_t this_event = AMPM_PHY_EV_HCI_READ_LSTO_FAILURE; /* Assume failure */

    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    pActivePhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                             pPhyLinkListAnchor, CsrBtAmpmListEntryMatchesPtr,
                                                             (const void *) pPhyLink);
    if (NULL != pActivePhyLink)
    {
        if (!pReturn->status)
        {
            this_event = AMPM_PHY_EV_HCI_READ_LSTO_SUCCESS;
        }
        csrBtAmpmPhyFsmRun(inst, pActivePhyLink, this_event, pReturn);
    }
}


void CsrBtAmpmPhySetCancelCreatePhyLinkOptions(
    CsrBtAmpmInstance * inst,
    CsrBool enabled,
    CsrBool afterCreatePhyLinkRsp,
    CsrBool invalidResponderAmpId,
    CsrUint32 delayAfterCreatePhyLinkReq)
{
    inst->sendCancelEnabled = enabled;
    if (enabled)
    {
        inst->sendCancelAfterCreatePhyLinkRsp = afterCreatePhyLinkRsp;
        inst->sendCancelWithInvalidResponderAmpId = invalidResponderAmpId;
        inst->sendCancelDelayAfterCreatePhyLinkReq =
            (delayAfterCreatePhyLinkReq * CSR_SCHED_MILLISECOND);
    }
    else
    {
        inst->sendCancelAfterCreatePhyLinkRsp = FALSE;
        inst->sendCancelWithInvalidResponderAmpId = FALSE;
        inst->sendCancelDelayAfterCreatePhyLinkReq = 0;
    }
}

void CsrBtAmpmPhyHciResponseTimeoutInd(CsrUint16 ignored,
                                       void *gash)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(gash != NULL);

    param = (CsrBtAmpmFsmParam *) gash;

    pPhyLink = (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(
        param->inst->pPhyLinkListAnchor,
        CsrBtAmpmListEntryMatchesPtr,
        param->argp);

    if (NULL != pPhyLink)
    {
        pPhyLink->hciResponseTid = 0;

        /* If timers are disabled then ignore it */
        if (((CsrBtAmpmInstance*)(param->inst))->ampmTimersEnabled)
        {
            csrBtAmpmPhyFsmRun(param->inst, pPhyLink,
                               AMPM_PHY_EV_HCI_RESPONSE_TIMEOUT_IND, NULL);
        }
    }
    CsrPmemFree(param);
}


void CsrBtAmpmPhySduResponseTimeoutInd(CsrUint16 ignored,
                                       void *pFsmInstance)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(pFsmInstance != NULL);

    param = (CsrBtAmpmFsmParam *) pFsmInstance;

    pPhyLink = (CsrBtAmpmPhyLink *)CsrBtAmpmListFindMatchingMember(
        param->inst->pPhyLinkListAnchor,
        CsrBtAmpmListEntryMatchesPtr,
        param->argp);

    if (NULL != pPhyLink)
    {
        pPhyLink->sduResponseTid = 0;

        /* If timers are disabled then ignore it */
        if (((CsrBtAmpmInstance*)(param->inst))->ampmTimersEnabled)
        {
            /* Signal the event into the btdev instance rather than
             * the physical link as the process of disconnecting the
             * ACL will trigger the local disconnection og the
             * physical link. Which is what we want. */
            CsrBtAmpmBtdevDisconnectReq(param->inst, pPhyLink->pBtDevice,
                                        CSR_BT_AMPM_RESULT_PEER_PROTOCOL_ERROR);
        }
    }
    CsrPmemFree(param);
}


void CsrBtAmpmPhySendCancelReqTimeoutInd(CsrUint16 ignored,
                                         void *gash)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(gash != NULL);

    param = (CsrBtAmpmFsmParam *) gash;

    pPhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(param->inst->
                                                             pPhyLinkListAnchor, CsrBtAmpmListEntryMatchesPtr, param->argp);
    if (NULL != pPhyLink && !CsrBtAmpmPhyIsDisconnecting(pPhyLink))
    {
        CsrBtAmpmPhyDisconnect(param->inst, pPhyLink);
    }
    CsrPmemFree(param);
}


void CsrBtAmpmPhyAmpDiscoverySuccess(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmBtdev * pBtDevice)
{
    CsrBtAmpmPhyLink *pPhyLink;

    /* Assume that there will only be a single physical link
     * that is waiting for the result of the discovery.
     */
    pPhyLink = CsrBtAmpmPhyGetByBdaddr(inst, pBtDevice->bdAddr);

    if (NULL != pPhyLink && !CsrBtAmpmPhyIsDisconnecting(pPhyLink))
    {
        CsrBtAmpmBtdevRemoteAmpInfo *pRemoteAmpInfo =
            CsrBtAmpmBtdevRemoteAmpInfoGetById(pBtDevice,
                                               pPhyLink->pRemoteAMPInfo->id);

        if (NULL != pRemoteAmpInfo && pRemoteAmpInfo->infoValid)
        {
            /* Copy Remote AMP Info in Physical Link Instance */
            pPhyLink->pRemoteAMPInfo->type = pRemoteAmpInfo->info.type;

            if(pPhyLink->pRemoteAMPInfo->pAMPAssoc)
            {
                CsrPmemFree(pPhyLink->pRemoteAMPInfo->pAMPAssoc->data);
                CsrPmemFree(pPhyLink->pRemoteAMPInfo->pAMPAssoc);
            }

            pPhyLink->pRemoteAMPInfo->pAMPAssoc =
                (CsrBtAmpmAmpAssoc *) CsrPmemZalloc(sizeof(*(pPhyLink->pRemoteAMPInfo->pAMPAssoc)));
            pPhyLink->pRemoteAMPInfo->pAMPAssoc->data =
                CsrPmemZalloc(pRemoteAmpInfo->info.maxHciAssocSize);

            /* Reference Bt Device in Physical Link Instance */
            pPhyLink->pBtDevice = pBtDevice;
            csrBtAmpmPhyFsmRun(inst,
                               pPhyLink,
                               AMPM_PHY_EV_DISCOVERY_SUCCESS,
                               NULL);
        }
        else
        {
            /* Inform any waiting logical links that we've failed to establish the physical link */
            CsrBtAmpmLogicalPhyLinkEvent(inst, pPhyLink,
                                         AMPM_LOGICAL_EV_PHY_DISCONNECT);
            csrBtAmpmPhyRemoveFsmInstance(inst, pPhyLink);
        }
    }
}


void CsrBtAmpmPhyAmpDiscoveryFailure(
    CsrBtAmpmInstance * inst,
    BD_ADDR_T bdAddr)
{
    CsrBtAmpmPhyLink *pPhyLink;

    /* Assume that there will only be a single physical link
     * that is waiting for the result of the discovery.
     */
    pPhyLink = CsrBtAmpmPhyGetByBdaddr(inst, bdAddr);

    if (NULL != pPhyLink && AMPM_PHY_ST_WAIT_AMP_DISCOVERY == pPhyLink->state)
    {
        csrBtAmpmPhyFsmRun(inst, pPhyLink, AMPM_PHY_EV_DISCOVERY_FAILURE,
                           NULL);
    }
}


void CsrBtAmpmPhyLocalDisconnectOnly(
    CsrBtAmpmInstance * inst,
    BD_ADDR_T bdAddr)
{
    CsrBtAmpmPhyLink *pPhyLink;

    /* Assume that there will only be a single physical link */
    pPhyLink = CsrBtAmpmPhyGetByBdaddr(inst, bdAddr);

    if (NULL != pPhyLink)
    {
        CsrUint8 numLogLinkUsing =
            CsrBtAmpmLogicalLinksUsingPhyLink(inst, pPhyLink);

        pPhyLink->pBtDevice = NULL;

        /* Local disconnect only so don't send or wait for any AMP Manager
         * physical link disconnect signalling.
         */
        pPhyLink->peerDisconnectNeeded = FALSE;
        pPhyLink->sendPeerDisconnectRspNeeded = FALSE;

        /* Disconnect any logical links first */
        if (0 < numLogLinkUsing)
        {
            CsrBtAmpmLogicalPhyLinkEvent(inst, pPhyLink,
                                         AMPM_LOGICAL_EV_DISCONNECT_REQ);
            /* When the last logical link is disconnected the disconnection
             * of the physical link will be triggered.
             */
        }
        else
        {
            /* Regardless of where we are in the physical link fsm
             * we have to disconnect the local HCI if we've started
             * to establish the link or have it established.
             */
            CsrBtAmpmPhyDisconnect(inst, pPhyLink);
        }
    }
}


void CsrBtAmpmPhyLocalAmpRemoved(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmId ampId)
{
    CsrBtAmpmPhyLink *pPhyLink;

    /* Need to terminate any physical links using the AMP but without
     * sending any signalling to the AMP is it isn't there any more.
     */
    pPhyLink = (CsrBtAmpmPhyLink *) inst->pPhyLinkListAnchor->head;

    while (NULL != pPhyLink)
    {
        CsrBtAmpmPhyLink *pNextPhyLink =
            (CsrBtAmpmPhyLink *) (((CsrBtAmpmGenListMember *) pPhyLink)->
                                  next);
        if (ampId == pPhyLink->pLocalAMPInfo->pAmpInfo->id)
        {
            dm_ampm_disconnect_req(&(pPhyLink->bdAddr),
                                   pPhyLink->pLocalAMPInfo->pAmpInfo->id,
                                   CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS, /* don't have a more appropriate alternative */
                                   0); /* AMP removed so no remaining active physical links */
            pPhyLink->hciDisconnectNeeded = FALSE;
            pPhyLink->peerDisconnectNeeded = FALSE;
            csrBtAmpmPhyFsmRun(inst, pPhyLink, AMPM_PHY_EV_DELETE_REQ, NULL);
        }
        pPhyLink = pNextPhyLink;
    }

}

void CsrBtAmpmPhyLocalAmpWriteRemoteAssocResponse(
    CsrBtAmpmInstance * inst,
    CsrUint8 PhysicalLinkHandle,
    CsrBool AssocWritten)
{
    CsrBtAmpmPhyLink *pPhyLink;

    /* Find the physical link associated with the handle
     * (if there is one) and then generate the appropriate event.
     */
    pPhyLink = CsrBtAmpmPhyGetByAmpPhyHandle(inst, PhysicalLinkHandle);

    if (NULL != pPhyLink && !CsrBtAmpmPhyIsDisconnecting(pPhyLink))
    {
        csrBtAmpmPhyCancelHciTimer(pPhyLink);
        if (AssocWritten)
        {
            csrBtAmpmPhyFsmRun(inst, pPhyLink,
                               AMPM_PHY_EV_HCI_WRITE_REMOTE_ASSOC_SUCCESS, NULL);
        }
        else
        {
            csrBtAmpmPhyFsmRun(inst, pPhyLink,
                               AMPM_PHY_EV_HCI_WRITE_REMOTE_ASSOC_FAILURE, NULL);
        }
    }
}


/* Physical link AMP assoc. Note that the pAMPAssoc pointer and its
 * contents must not be modified nor fredd */
void CsrBtAmpmPhyLocalAmpAssoc(CsrBtAmpmInstance * inst,
                               CsrBtAmpmPhyLink * pPossiblePhyLink,
                               CsrBtAmpmId ampId,
                               CsrBtAmpmAmpAssoc *pAMPAssoc)
{
    CsrBtAmpmPhyLink *pPhyLink;

    CSR_BT_AMPM_ASSERT(pPossiblePhyLink != NULL);
    CSR_BT_AMPM_ASSERT(pAMPAssoc != NULL);

    pPhyLink =
        (CsrBtAmpmPhyLink *) CsrBtAmpmListFindMatchingMember(inst->
                                                             pPhyLinkListAnchor, CsrBtAmpmListEntryMatchesPtr,
                                                             (const void *) pPossiblePhyLink);
    if (NULL != pPhyLink && !CsrBtAmpmPhyIsDisconnecting(pPhyLink))
    {
        csrBtAmpmPhyCancelHciTimer(pPhyLink);
        /* Assuming that a length of 0 indicates an error reading the amp assoc */
        if (0 < pAMPAssoc->length)
        {
            csrBtAmpmPhyFsmRun(inst, pPhyLink,
                               AMPM_PHY_EV_HCI_READ_LOCAL_ASSOC_SUCCESS,
                               (void *) pAMPAssoc);
        }
        else
        {
            csrBtAmpmPhyFsmRun(inst, pPhyLink,
                               AMPM_PHY_EV_HCI_READ_LOCAL_ASSOC_FAILURE,
                               NULL);
        }
    }
}

CsrBool CsrBtAmpmPhyAmpCommandReject(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBool SduHandled = FALSE;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);
    if (NULL != pBtDevice)
    {
        CsrBtAmpmPhyLink *pPhyLink;
        CsrBtAmpmPhySearchParams searchParams;

        /* filling search structure */
        searchParams.cid = cid;
        searchParams.amp_id = 0;
        searchParams.sdu_identifier =
            CSR_BT_AMPM_DECODE_U8_IE(pMsg, CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET);
        bd_addr_copy(&searchParams.bd_addr,
                     (const BD_ADDR_T *) &pBtDevice->bdAddr);

        pPhyLink =
            CsrBtAmpmPhyGetByBdaddrAndSduIdentifier(inst, &searchParams);
        if (NULL != pPhyLink)
        {
            csrBtAmpmPhyFsmRun(inst, pPhyLink, AMPM_PHY_EV_AMP_COMMAND_REJECT,
                               NULL);
            SduHandled = TRUE;
        }
    }
    return (SduHandled);
}


void CsrBtAmpmPhyAmpGetAmpAssocResponse(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);
    if (NULL != pBtDevice)
    {
        CsrBtAmpmPhyLink *pPhyLink;
        CsrBtAmpmPhySearchParams searchParams;

        /* filling search structure */
        searchParams.cid = cid;
        searchParams.amp_id = 0;
        searchParams.sdu_identifier =
            CSR_BT_AMPM_DECODE_U8_IE(pMsg, CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET);
        bd_addr_copy(&searchParams.bd_addr, (const BD_ADDR_T *) &pBtDevice->bdAddr);

        pPhyLink =
            CsrBtAmpmPhyGetByBdaddrAndSduIdentifier(inst, &searchParams);

        if (NULL != pPhyLink && !CsrBtAmpmPhyIsDisconnecting(pPhyLink))
        {
            if (CSR_BT_AMPM_GET_AMP_ASSOC_RESPONSE ==
                pPhyLink->expectedResponseSdu)
            {
                CsrUint8 ampId = CSR_BT_AMPM_DECODE_U8_IE(pMsg,
                                                          CSR_BT_AMPM_SDU_MESSAGE_OFFSET);
                CsrUint8 Status = CSR_BT_AMPM_DECODE_U8_IE(pMsg,
                                                           CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 1);

                if (CSR_BT_AMPM_GET_ASSOC_RSP_STATUS_SUCCESS == Status &&
                    ampId == CsrBtAmpmPhyGetRemoteAmpId(pPhyLink))
                {
                    CsrBtAmpmAmpAssoc *pAMPAssoc;

                    pAMPAssoc = pPhyLink->pRemoteAMPInfo->pAMPAssoc;
                    pAMPAssoc->length = CSR_BT_AMPM_DECODE_U16_IE(pMsg,
                                                                  CSR_BT_AMPM_SDU_LENGTH_OFFSET) - 2;

                    CsrPmemFree(pAMPAssoc->data);
                    pAMPAssoc->data = CsrPmemAlloc(pAMPAssoc->length);
                    (void) CsrMemCpy(pAMPAssoc->data,
                                     pMsg + CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 2,
                                     (CsrUint32) pAMPAssoc->length);

                    csrBtAmpmPhyFsmRun(inst, pPhyLink,
                                       AMPM_PHY_EV_ASSOC_RESPONSE_SUCCESS, NULL);
                }
                else
                {
                    csrBtAmpmPhyFsmRun(inst, pPhyLink,
                                       AMPM_PHY_EV_ASSOC_RESPONSE_FAILURE, NULL);
                }
            }
        }
    }
}

void CsrBtAmpmPhyAmpCreatePhysicalLinkRequest(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);

    if (NULL != pBtDevice)
    {
        CsrBtAmpmLocalAmp *pLocalAMPInfo;
        CsrBtAmpmBtdevRemoteAmpInfo *pRemoteAmpInfo;
        CsrMblk *pMsgBlk;

        /* Decode the SDU elements */
        CsrUint8 sduIdentifier = CSR_BT_AMPM_DECODE_U8_IE(pMsg, CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET);
        CsrUint16 assocLength = CSR_BT_AMPM_DECODE_U16_IE(pMsg,(CSR_BT_AMPM_SDU_LENGTH_OFFSET)) - 2;
        CsrBtAmpmId remoteAmpId = CSR_BT_AMPM_DECODE_U8_IE(pMsg, CSR_BT_AMPM_SDU_MESSAGE_OFFSET);
        CsrBtAmpmId localAmpId = CSR_BT_AMPM_DECODE_U8_IE(pMsg, CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 1);

        /* Provoke collision? */
        if(CsrBtAmpmPhyCollisionRequired(inst))
        {
            CsrMblk *mblk;
            CsrUint8 *pSDU;
            mblk = CsrMblkMallocCreate((void *)&(pSDU), (CsrMblkSize)(assocLength + 6));

            /* Use the original request from the peer to create the collision */
            CsrMemCpy(pSDU, pMsg, assocLength + 6);

            /* Need to flip the AMP Ids */
            *(pSDU + CSR_BT_AMPM_SDU_MESSAGE_OFFSET) = localAmpId;
            *(pSDU + CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 1) = remoteAmpId;

            L2CA_DataWriteReq((l2ca_cid_t)cid, 0, mblk);
        }

        /* Check if AmpID received in the request has valid AMP IDs */
        pLocalAMPInfo = CsrBtAmpmLocalGetById(inst, localAmpId);
        pRemoteAmpInfo = CsrBtAmpmBtdevRemoteAmpInfoGetById(pBtDevice, remoteAmpId);

        if (NULL == pLocalAMPInfo
            || (CsrBtAmpmBtdevIsDiscovered(pBtDevice) &&
                (NULL == pRemoteAmpInfo || !pRemoteAmpInfo->infoValid)))
        {
            /* If either AMPID received in AMP CREATE PHYSICAL LINK
             * REQUEST is invalid, then send negative AMP CREATE
             * PHYSICAL LINK RESPONSE */
            pMsgBlk = CsrBtAmpmSduCreateAmpCreatePhysicalLinkResponse(
                sduIdentifier,
                localAmpId,
                remoteAmpId,
                CSR_BT_AMPM_CREATE_PHYSICAL_LINK_INVALID_AMP_ID);

            L2CA_DataWriteReq((l2ca_cid_t)cid, 0, pMsgBlk);
        }
        else
        {
            CsrBtAmpmPhyLink *pExistingPhyLink;
            CsrBtAmpmPhyLink *pRequestPhyLink;
            CsrBtAmpmAmpAssoc *pAssoc;

            /* Determine if there is already a physical link that between
             * the local device and the peer device via the AMP that
             * is also specified in the request as this could be a collision
             */
            pExistingPhyLink = CsrBtAmpmPhyGetByBdaddrAndLocalAmpId(inst,
                                                                    pBtDevice->bdAddr,
                                                                    localAmpId);

            /* Create a new physical instance */
            pRequestPhyLink = csrBtAmpmPhyNew(inst, pBtDevice->bdAddr, FALSE);

            /* Reference Bt Device in Physical Link Instance */
            pRequestPhyLink->pBtDevice = pBtDevice;
            pRequestPhyLink->pLocalAMPInfo = pLocalAMPInfo;
            pRequestPhyLink->ampCreatePhysicalRspSduIdentifier = sduIdentifier;

            pRequestPhyLink->pRemoteAMPInfo =
                (CsrBtAmpmDataInfo *)CsrPmemZalloc(sizeof(*(pRequestPhyLink->pRemoteAMPInfo)));
            pRequestPhyLink->pRemoteAMPInfo->id = remoteAmpId;
            pRequestPhyLink->pRemoteAMPInfo->pAMPAssoc =
                (CsrBtAmpmAmpAssoc *)CsrPmemZalloc(sizeof(*(pRequestPhyLink->pRemoteAMPInfo->pAMPAssoc)));

            pAssoc = pRequestPhyLink->pRemoteAMPInfo->pAMPAssoc;
            pAssoc->length = assocLength;

            /* Assume that it is still valid to have an AMP Assoc length of 0 */
            if (0 < assocLength)
            {
                pAssoc->data = CsrPmemAlloc(assocLength);
                (void)CsrMemCpy((void *)(pAssoc->data),
                                (void *)(pMsg + CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 2),
                                (CsrUint32)assocLength);
            }
            else
            {
                pAssoc->data = NULL;
            }

            if ((NULL == pExistingPhyLink) &&
                !CsrBtAmpmPhyCollisionRequired(inst))
            {
                csrBtAmpmPhyFsmRun(inst,
                                   pRequestPhyLink,
                                   AMPM_PHY_EV_CREATE_PHYSICAL_REQ, NULL);
            }
            else
            {
                /* Have to establish how far the physical link has progressed and
                 * whether we have a collision scenario or not.
                 *
                 * If we've got as far as the physical link establishment starting
                 * i.e. either we responded with success to the physical link create request
                 * and the peer has then sent this new request in error.
                 *
                 * If we've not progressed to that point then this is a collision
                 * scenario and we have to determine whether to the continue with
                 * the current physical link or start again with the request that
                 * we have received.
                 *
                 * In either case the l2cap channel still needs to be maintained
                 * and related to the correct physical link.
                 */
                if(CsrBtAmpmPhyCollisionRequired(inst) ||
                   (!csrBtAmpmPhyStartedPhysicalLinkCreation(pExistingPhyLink) &&
                    !CsrBtAmpmPhyIsConnected(pExistingPhyLink)))
                {
                    BD_ADDR_T localAddr;
                    CsrBool wonCollision;

                    localAddr = CsrBtAmpmDataGetLocalBtAddr(inst);
                    wonCollision = csrBtAmpmPhyAddressCollision(&localAddr,
                                                                &(pBtDevice->bdAddr));

                    /* We're still in the early stages of creating the
                     * physical link and the other side either hasn't
                     * yet received the create request or we haven't
                     * actually got far enough in the process yet to
                     * send it.*/

                    if (wonCollision)
                    {
                        /* The local address is the higher so we'll
                         * reject the received request and continue
                         * with our own. */
                        pMsgBlk = CsrBtAmpmSduCreateAmpCreatePhysicalLinkResponse(
                            sduIdentifier,
                            localAmpId,
                            remoteAmpId,
                            CSR_BT_AMPM_CREATE_PHYSICAL_LINK_COLLISION);
                        L2CA_DataWriteReq((l2ca_cid_t)cid, 0, pMsgBlk);

                        /* Kill the link, but do not inform DM of the
                         * shutdown as it was never really started */
                        pRequestPhyLink->dontDisconnectDm = TRUE;
                        csrBtAmpmPhyFsmRun(inst,
                                           pRequestPhyLink,
                                           AMPM_PHY_EV_DELETE_REQ, NULL);
                    }
                    else
                    {
                        /* We lost so terminate our physical link and
                         * start a new one based on the received
                         * request. */
                        if(!CsrBtAmpmPhyCollisionRequired(inst))
                        {
                            CsrBtAmpmL2capSwitchPhyLink(inst,
                                                        pExistingPhyLink,
                                                        pRequestPhyLink);
                            pExistingPhyLink->dontDisconnectDm = TRUE;
                            csrBtAmpmPhyFsmRun(inst,
                                               pExistingPhyLink,
                                               AMPM_PHY_EV_DELETE_REQ,
                                               NULL);
                        }

                        csrBtAmpmPhyFsmRun(inst, pRequestPhyLink,
                                           AMPM_PHY_EV_CREATE_PHYSICAL_REQ,
                                           NULL);
                    }
                }
                else
                {
                    /* We are past the collision phase. Reject the new
                     * link */
                    pMsgBlk = CsrBtAmpmSduCreateAmpCreatePhysicalLinkResponse(sduIdentifier,
                                                                              localAmpId,
                                                                              remoteAmpId,
                                                                              CSR_BT_AMPM_CREATE_PHYSICAL_LINK_ALREADY_EXISTS);
                    L2CA_DataWriteReq((l2ca_cid_t)cid, 0, pMsgBlk);

                    /* Kill the link, but do not inform DM of the
                     * shutdown as it was never really started */
                    pRequestPhyLink->dontDisconnectDm = TRUE;
                    csrBtAmpmPhyFsmRun(inst, pRequestPhyLink,
                                       AMPM_PHY_EV_DELETE_REQ,
                                       NULL);
                }
            }
        }
    }
    else
    {
        /* The btdev is invalid. Cannot happen. */
        CSR_BT_AMPM_ASSERT(FALSE);
    }
}


void CsrBtAmpmPhyAmpCreatePhysicalLinkResponse(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);

    if (NULL != pBtDevice)
    {
        CsrBtAmpmPhyLink *pPhyLink;
        CsrBtAmpmPhySearchParams searchParams;

        /* filling search structure */
        searchParams.cid = cid;
        searchParams.amp_id = 0;
        searchParams.sdu_identifier =
            CSR_BT_AMPM_DECODE_U8_IE(pMsg, CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET);
        bd_addr_copy(&searchParams.bd_addr,
                     (const BD_ADDR_T *) &pBtDevice->bdAddr);

        pPhyLink =
            CsrBtAmpmPhyGetByBdaddrAndSduIdentifier(inst, &searchParams);

        if (NULL != pPhyLink)
        {
            if (CSR_BT_AMPM_CREATE_PHYSICAL_LINK_RESPONSE ==
                pPhyLink->expectedResponseSdu)
            {
                CsrBtAmpmId remoteAmpId = CSR_BT_AMPM_DECODE_U8_IE(pMsg,
                                                                   CSR_BT_AMPM_SDU_MESSAGE_OFFSET);
                CsrBtAmpmId localAmpId = CSR_BT_AMPM_DECODE_U8_IE(pMsg,
                                                                  CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 1);
                CsrUint8 Status = CSR_BT_AMPM_DECODE_U8_IE(pMsg,
                                                           CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 2);

                if (CSR_BT_AMPM_CREATE_PHYSICAL_LINK_STARTED == Status)
                {
                    if (localAmpId == CsrBtAmpmPhyGetLocalAmpId(pPhyLink) &&
                        remoteAmpId == CsrBtAmpmPhyGetRemoteAmpId(pPhyLink))
                    {
                        csrBtAmpmPhyFsmRun(inst, pPhyLink,
                                           AMPM_PHY_EV_AMP_CREATE_PHYS_RSP_SUCCESS, NULL);
                    }
                }
                else
                {
                    pPhyLink->peerDisconnectNeeded = FALSE;
                    csrBtAmpmPhyFsmRun(inst, pPhyLink,
                                       AMPM_PHY_EV_AMP_CREATE_PHYS_RSP_FAIL, NULL);
                }
            }
        }
    }
}


void CsrBtAmpmPhyAmpDisconnectPhysicalLinkRequest(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice = NULL;
    CsrBtAmpmPhyLink *pPhyLink = NULL;
    CsrMblk *pMsgBlk = NULL;
    CsrBtAmpmId localAmpId;
    CsrBtAmpmId remoteAmpId;
    CsrUint8 receivedSduId;
    CsrUint8 responseStatus = CSR_BT_AMPM_DISCONNECT_LINK_NOT_IN_PROGRESS;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    receivedSduId =
        CSR_BT_AMPM_DECODE_U8_IE(pMsg, CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET);
    remoteAmpId =
        (CsrBtAmpmId) CSR_BT_AMPM_DECODE_U8_IE(pMsg,
                                               CSR_BT_AMPM_SDU_MESSAGE_OFFSET);
    localAmpId =
        (CsrBtAmpmId) CSR_BT_AMPM_DECODE_U8_IE(pMsg,
                                               CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 1);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);
    if (NULL != pBtDevice)
    {
        CsrBtAmpmLocalAmp *pLocalAmp =
            CsrBtAmpmLocalGetById(inst, localAmpId);
        pPhyLink = CsrBtAmpmPhyGetByBdaddr(inst, pBtDevice->bdAddr);
        if (NULL != pPhyLink && NULL != pLocalAmp)
        {
            /* The following ensures that the amp ids in the disconnect request actually
             * relate to the physical link that has been found between this device
             * and the remote. As it could be the AMP ids don't match.
             */
            if (localAmpId != CsrBtAmpmPhyGetLocalAmpId(pPhyLink) ||
                remoteAmpId != CsrBtAmpmPhyGetRemoteAmpId(pPhyLink))
            {
                responseStatus =
                    CSR_BT_AMPM_DISCONNECT_LINK_INVALID_CONTROLLER_ID;
                pPhyLink = NULL;
            }
        }
        else if (NULL == pLocalAmp)
        {
            responseStatus =
                CSR_BT_AMPM_DISCONNECT_LINK_INVALID_CONTROLLER_ID;
            pPhyLink = NULL;
        }
    }
    if (NULL != pPhyLink)
    {
        pPhyLink->sduIdentifier = receivedSduId;
        pPhyLink->peerDisconnectNeeded = FALSE;
        pPhyLink->sendPeerDisconnectRspNeeded = TRUE;
        csrBtAmpmPhyFsmRun(inst, pPhyLink, AMPM_PHY_EV_AMP_DISC_PHYS_REQ,
                           NULL);
    }
    else
    {
        pMsgBlk =
            CsrBtAmpmSduCreateAmpDisconnectPhysicalLinkResponse(receivedSduId,
                                                                localAmpId, remoteAmpId, responseStatus);

        L2CA_DataWriteReq((l2ca_cid_t)cid, 0, pMsgBlk);
    }
}


void CsrBtAmpmPhyAmpDisconnectPhysicalLinkResponse(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);

    if (NULL != pBtDevice)
    {
        CsrBtAmpmPhyLink *pPhyLink;
        CsrBtAmpmPhySearchParams searchParams;

        /* filling search structure */
        searchParams.cid = cid;
        searchParams.amp_id = 0;
        searchParams.sdu_identifier =
            CSR_BT_AMPM_DECODE_U8_IE(pMsg, CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET);
        bd_addr_copy(&searchParams.bd_addr,
                     (const BD_ADDR_T *) &pBtDevice->bdAddr);

        pPhyLink =
            CsrBtAmpmPhyGetByBdaddrAndSduIdentifier(inst, &searchParams);

        if (NULL != pPhyLink)
        {
            if (CSR_BT_AMPM_DISCONNECT_PHYSICAL_LINK_RESPONSE ==
                pPhyLink->expectedResponseSdu)
            {
                csrBtAmpmPhyFsmRun(inst, pPhyLink,
                                   AMPM_PHY_EV_AMP_DISC_PHYS_RSP, NULL);
            }
        }
    }
}

void CsrBtAmpmPhyHciChannelSelect(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPalHciEventReq * ampm_hci_prim)
{
    CsrBtAmpHciChannelSelectEvt *pHciChannelSelect;
    CsrBtAmpmPhyLink *pPhyLink;

    CSR_BT_AMPM_ASSERT(ampm_hci_prim != NULL);
    CSR_BT_AMPM_ASSERT(ampm_hci_prim->data != NULL);

    pHciChannelSelect = (CsrBtAmpHciChannelSelectEvt *) ampm_hci_prim->data;

    pPhyLink =
        CsrBtAmpmPhyGetByAmpPhyHandle(inst,
                                      pHciChannelSelect->physicalLinkHandle);

    if (NULL != pPhyLink)
    {
        csrBtAmpmPhyFsmRun(inst, pPhyLink, AMPM_PHY_EV_HCI_CHANNEL_SELECT,
                           NULL);
    }
}


void CsrBtAmpmPhyHciPhysicalLinkComplete(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPalHciEventReq * ampm_hci_prim)
{
    CsrBtAmpHciPhysicalLinkCompleteEvt *pAmpmHciPhysicalLinkComplete;
    CsrBtAmpmPhyLink *pPhyLink;

    CSR_BT_AMPM_ASSERT(ampm_hci_prim != NULL);
    CSR_BT_AMPM_ASSERT(ampm_hci_prim->data != NULL);

    pAmpmHciPhysicalLinkComplete =
        (CsrBtAmpHciPhysicalLinkCompleteEvt *) ampm_hci_prim->data;

    pPhyLink =
        CsrBtAmpmPhyGetByAmpPhyHandle(inst,
                                      pAmpmHciPhysicalLinkComplete->physicalLinkHandle);

    if (NULL != pPhyLink)
    {
        fsm_event_t event = AMPM_PHY_EV_HCI_PHYS_COMPLETE_FAIL; /* Assume failure */

        if (CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS ==
            pAmpmHciPhysicalLinkComplete->status)
        {
            if (pPhyLink->initiator)
            {
                event = AMPM_PHY_EV_HCI_PHYS_COMPLETE_INIT;
            }
            else
            {
                event = AMPM_PHY_EV_HCI_PHYS_COMPLETE_SUCCESS;
            }
        }
        else
        {
            pPhyLink->hciDisconnectNeeded = FALSE;
        }
        if (AMPM_PHY_ST_DISCONNECT_PENDING != pPhyLink->state)
        {
            csrBtAmpmPhyFsmRun(inst, pPhyLink, event, NULL);
        }
    }
}


void CsrBtAmpmPhyHciDisconnectPhysicalLinkComplete(CsrBtAmpmInstance * inst,
                                                   CsrBtAmpmPhyLink * pPhyLink,
                                                   CsrBtAmpHciStatusCode status,
                                                   CsrUint8 reason)
{
    fsm_event_t event;
    CSR_BT_AMPM_ASSERT(pPhyLink != NULL);

    if (CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS != status)
    {
        CSR_BT_AMPM_ASSERT(FALSE);
    }

    if(reason == CSR_BT_AMP_HCI_STATUS_CODE_CONNECTION_TIMEOUT)
    {
        /* Link supervision timeouts should not attempt a graceful
         * A2MP disconnect of the physical link */
        event = AMPM_PHY_EV_HCI_DISC_PHYS_LSTO;
    }
    else
    {
        event = AMPM_PHY_EV_HCI_DISC_PHYS_COMPLETE;
    }

    csrBtAmpmPhyFsmRun(inst, pPhyLink, event, NULL);
}

void CsrBtAmpmPhyPowerOnRes(CsrBtAmpmInstance * inst, CsrBtAmpmPowerOnRes *res)
{
    CsrBtAmpmPhyLink *pPhysLink = CsrBtAmpmPhyGetByLocalAmpId(inst,
                                                  CSR_BT_AMPM_L2CA_TO_AMP_ID(res->localId));

    if( (NULL != pPhysLink) && (AMPM_PHY_ST_WAIT_POWER_ON == pPhysLink->state) )
    {
        fsm_event_t event = AMPM_PHY_EV_POWER_ON_FAIL;

        if(res->complete)
        {
            event = AMPM_PHY_EV_POWER_ON_SUCCESS;
        }
        csrBtAmpmPhyFsmRun(inst, pPhysLink, event, NULL);
    }
}

fsm_event_t CsrBtAmpmPhyFsmSendHciAcceptPhysicalLink(void *arg1,
                                                     void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmDataAmpKey *pAmpKey;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Obtain an appropriate key */
    pAmpKey = CsrBtAmpmDataKeysGetKeyForAmp(param->inst,
                                            pPhyLink->bdAddr,
                                            CsrBtAmpmLocalGetType(pPhyLink->pLocalAMPInfo));

    if (NULL == pAmpKey)
    {
        CsrMblk *pMsgBlk;

        /* Must have a key to use otherwise we respond with an error */
        pMsgBlk = CsrBtAmpmSduCreateAmpCreatePhysicalLinkResponse(pPhyLink->
                                                                  ampCreatePhysicalRspSduIdentifier,
                                                                  CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                                                                  CsrBtAmpmPhyGetRemoteAmpId(pPhyLink),
                                                                  CSR_BT_AMPM_CREATE_PHYSICAL_LINK_SECURITY_VIOLATION);
        L2CA_DataWriteReq((l2ca_cid_t)pPhyLink->pBtDevice->cid, 0, pMsgBlk);
        return AMPM_PHY_EV_DELETE_REQ;
    }
    else
    {
        if(!CsrBtAmpmPhyDontStartPhysLink(param->inst, pPhyLink))
        {
            CsrBtAmpmSendHciAcceptPhysicalLinkRequest(param->inst,
                                                      CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                                                      (void *) pPhyLink, pPhyLink->ampPLinkHandle,
                                                      pAmpKey->keySize,
                                                      pAmpKey->keyType,
                                                      pAmpKey->key);

            pPhyLink->hciDisconnectNeeded = TRUE;
            csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);
        }
        else
        {
            CsrPmemFree(pAmpKey->key);
        }

        CsrPmemFree(pAmpKey);
        pPhyLink->startedLinkCreation = TRUE;
        return FSM_EVENT_NULL;
    }

}


fsm_event_t CsrBtAmpmPhyFsmSetupFail(void *arg1,
                                     void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Need to stop response timers */
    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    csrBtAmpmPhyCancelHciTimer(pPhyLink);

    if (!pPhyLink->peerDisconnectNeeded && !pPhyLink->hciDisconnectNeeded)
    {
        /* Don't need to wait for a response */
        return AMPM_PHY_EV_DELETE_REQ;
    }
    else
    {
        if (pPhyLink->peerDisconnectNeeded)
        {
            csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequest(param->inst,
                                                             pPhyLink);
            csrBtAmpmPhyStartSduTimer(param->inst, pPhyLink);
        }
        if (pPhyLink->hciDisconnectNeeded)
        {
            CsrBtAmpmSendHciDisconnectPhysicalLink(
                param->inst,
                CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                (void *) pPhyLink, pPhyLink->ampPLinkHandle,
                CSR_BT_AMP_HCI_STATUS_CODE_CONNECTION_TERMINATED_BY_LOCAL_HOST);
            csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);
        }
        return FSM_EVENT_NULL;
    }
}


fsm_event_t CsrBtAmpmPhyFsmSetupPalDisconnected(void *arg1,
                                                void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Need to stop response timers */
    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    csrBtAmpmPhyCancelHciTimer(pPhyLink);

    pPhyLink->hciDisconnectNeeded = FALSE;

    if (pPhyLink->peerDisconnectNeeded)
    {
        csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequest(param->inst,
                                                         pPhyLink);
        csrBtAmpmPhyStartSduTimer(param->inst, pPhyLink);
        return FSM_EVENT_NULL;
    }
    else
    {
        /* Don't need to wait for a response */
        return AMPM_PHY_EV_DELETE_REQ;
    }
}


fsm_event_t CsrBtAmpmPhyFsmSetupPeerFailed(void *arg1,
                                           void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Need to stop response timers */
    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    csrBtAmpmPhyCancelHciTimer(pPhyLink);

    /* TESTING: Do we need to cancel? */
    if (param->inst->sendCancelEnabled &&
        param->inst->sendCancelAfterCreatePhyLinkRsp)
    {
        if (param->inst->sendCancelWithInvalidResponderAmpId)
        {
            csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequestWithInvalidAmpId
                (param->inst, pPhyLink);
        }
        else
        {
            csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequest(param->inst,
                                                             pPhyLink);
        }
    }

    pPhyLink->peerDisconnectNeeded = FALSE;

    if (pPhyLink->hciDisconnectNeeded)
    {
        CsrBtAmpmSendHciDisconnectPhysicalLink(
            param->inst,
            CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
            (void *) pPhyLink, pPhyLink->ampPLinkHandle,
            CSR_BT_AMP_HCI_STATUS_CODE_CONNECTION_TERMINATED_BY_LOCAL_HOST);
        csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);
        return FSM_EVENT_NULL;
    }
    else
    {
        /* Don't need to wait for a response */
        return AMPM_PHY_EV_DELETE_REQ;
    }
}

fsm_event_t CsrBtAmpmPhyFsmSendPowerOnInd(void *arg1,
                                            void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp = NULL;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;
    pLocalAmp = pPhyLink->pLocalAMPInfo;

    if(pLocalAmp != NULL)
    {
        CsrBtAmpmUpstreamPowerOnInd(param->inst->ampPowerOnQid,
                                    pLocalAmp->pAmpInfo->type,
                                    pLocalAmp->pAmpInfo->id,
                                    TRUE);
    }
    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtAmpmPhyFsmPowerOnSuccess(void *arg1,
                                            void *arg2)
{
    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);
    
    return AMPM_PHY_EV_CONNECT_REQ;
}
        
fsm_event_t CsrBtAmpmPhyFsmSetupPeerSuccess(void *arg1,
                                            void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Need to stop response timers */
    csrBtAmpmPhyCancelSduTimer(pPhyLink);

    pPhyLink->startedLinkCreation = TRUE;
    pPhyLink->peerDisconnectNeeded = TRUE;

    /* TESTING: Do we need to cancel? */
    if (param->inst->sendCancelEnabled &&
        param->inst->sendCancelAfterCreatePhyLinkRsp)
    {
        if (param->inst->sendCancelWithInvalidResponderAmpId)
        {
            csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequestWithInvalidAmpId
                (param->inst, pPhyLink);
            return FSM_EVENT_NULL;
        }
        else
        {
            csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequest(param->inst,
                                                             pPhyLink);
            pPhyLink->peerDisconnectNeeded = FALSE;
            return AMPM_PHY_EV_DISCONNECT_REQ;
        }
    }
    else
    {
        return FSM_EVENT_NULL;
    }
}

fsm_event_t CsrBtAmpmPhyFsmPeerCancellingConnection(void *arg1,
                                                    void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrMblk *pMsgBlk;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    pMsgBlk =
        CsrBtAmpmSduCreateAmpCreatePhysicalLinkResponse(pPhyLink->
                                                        ampCreatePhysicalRspSduIdentifier,
                                                        CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                                                        CsrBtAmpmPhyGetRemoteAmpId(pPhyLink),
                                                        CSR_BT_AMPM_CREATE_PHYSICAL_LINK_RECEIVED_DISCONNECT);

    L2CA_DataWriteReq((l2ca_cid_t)pPhyLink->pBtDevice->cid, 0, pMsgBlk);

    if (pPhyLink->hciDisconnectNeeded)
    {
        csrBtAmpmPhyCancelHciTimer(pPhyLink);
        CsrBtAmpmSendHciDisconnectPhysicalLink(
            param->inst,
            CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
            (void *) pPhyLink, pPhyLink->ampPLinkHandle,
            CSR_BT_AMP_HCI_STATUS_CODE_CONNECTION_TERMINATED_BY_LOCAL_HOST);
        csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);
        return FSM_EVENT_NULL;
    }
    else
    {
        return AMPM_PHY_EV_DELETE_REQ;
    }
}


fsm_event_t CsrBtAmpmPhyFsmPeerDisconnectingLink(void *arg1,
                                                 void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Need to stop response timers */
    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    csrBtAmpmPhyCancelHciTimer(pPhyLink);

    pPhyLink->peerDisconnectNeeded = FALSE;

    if (pPhyLink->hciDisconnectNeeded)
    {
        CsrBtAmpmSendHciDisconnectPhysicalLink(
            param->inst,
            CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
            (void *) pPhyLink, pPhyLink->ampPLinkHandle,
            CSR_BT_AMP_HCI_STATUS_CODE_CONNECTION_TERMINATED_BY_LOCAL_HOST);
        csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);
        return FSM_EVENT_NULL;
    }
    else
    {
        /* Don't need to wait for a response */
        return AMPM_PHY_EV_DELETE_REQ;
    }
}


fsm_event_t CsrBtAmpmPhyFsmSendHciCreatePhysicalLink(void *arg1,
                                                     void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmDataAmpKey *pAmpKey;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Need to stop response timer */
    csrBtAmpmPhyCancelSduTimer(pPhyLink);

    /* Obtain an appropriate key */
    pAmpKey = CsrBtAmpmDataKeysGetKeyForAmp(param->inst,
                                            pPhyLink->bdAddr,
                                            CsrBtAmpmLocalGetType(pPhyLink->pLocalAMPInfo));

    if (NULL == pAmpKey)
    {
        /* We don't have a key - epic fail */
        CsrBtAmpmBtdevDisconnectReq(param->inst, pPhyLink->pBtDevice,
                                    CSR_BT_AMPM_RESULT_SECURITY_FAILURE);
    }
    else
    {
        CsrBtAmpmSendHciCreatePhysicalLink(param->inst,
                                           CsrBtAmpmPhyGetLocalAmpId
                                           (pPhyLink), (void *) pPhyLink, pPhyLink->ampPLinkHandle,
                                           pAmpKey->keySize, pAmpKey->keyType, pAmpKey->key);

        CsrPmemFree(pAmpKey);

        pPhyLink->hciDisconnectNeeded = TRUE;
        csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);
    }

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmSendAmpCreatePhyLinkReq(void *arg1,
                                                   void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmAmpAssoc *pAMPAssoc;
    CsrMblk *pMsgBlk;
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmFsmParam *timerParam;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* This variable is passed from the local AMP instance. It or its
     * contents must not be modified nor freed */
    pAMPAssoc = (CsrBtAmpmAmpAssoc *) arg2;

    pPhyLink->sduIdentifier = CsrBtAmpmSduAllocIdentifier(param->inst);
    pPhyLink->expectedResponseSdu = CSR_BT_AMPM_CREATE_PHYSICAL_LINK_RESPONSE;

    pMsgBlk = CsrBtAmpmSduCreateAmpCreatePhysicalLinkRequest(pPhyLink->
                                                             sduIdentifier,
                                                             CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                                                             CsrBtAmpmPhyGetRemoteAmpId(pPhyLink),
                                                             (const CsrBtAmpmAmpAssoc *)pAMPAssoc);

    L2CA_DataWriteReq((l2ca_cid_t)pPhyLink->pBtDevice->cid, 0, pMsgBlk);
    csrBtAmpmPhyStartSduTimer(param->inst, pPhyLink);
    pPhyLink->peerDisconnectNeeded = TRUE;

    /* TESTING: Do we need to cancel the link setup */
    if (param->inst->sendCancelEnabled)
    {
        if (0 < param->inst->sendCancelDelayAfterCreatePhyLinkReq)
        {
            timerParam = CsrPmemAlloc(sizeof(CsrBtAmpmFsmParam));
            timerParam->inst = param->inst;
            timerParam->argp = pPhyLink;

            (void) CsrSchedTimerSet(param->inst->sendCancelDelayAfterCreatePhyLinkReq,
                                   CsrBtAmpmPhySendCancelReqTimeoutInd,
                                   0,
                                   timerParam);
            return FSM_EVENT_NULL;
        }
        else if (!param->inst->sendCancelAfterCreatePhyLinkRsp)
        {
            /* We need to cancel straight away */
            if (param->inst->sendCancelWithInvalidResponderAmpId)
            {
                csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequestWithInvalidAmpId(param->inst,
                                                                                 pPhyLink);
                return FSM_EVENT_NULL;
            }
            else
            {
                return AMPM_PHY_EV_DISCONNECT_REQ;
            }
        }
        return FSM_EVENT_NULL;
    }
    else
    {
        return FSM_EVENT_NULL;
    }
}


fsm_event_t CsrBtAmpmPhyFsmAmpCreatePhyLinkRspPend(void *arg1,
                                                   void *arg2)
{
    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmHciPhyLinkCompletePend(void *arg1,
                                                  void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Need to stop response timer */
    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    pPhyLink->startedLinkCreation = TRUE;

    /* TESTING: Do we need to cancel? */
    if (param->inst->sendCancelEnabled &&
        param->inst->sendCancelAfterCreatePhyLinkRsp)
    {
        if (param->inst->sendCancelWithInvalidResponderAmpId)
        {
            csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequestWithInvalidAmpId
                (param->inst, pPhyLink);
            return FSM_EVENT_NULL;
        }
        else
        {
            csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequest(param->inst,
                                                             pPhyLink);
            pPhyLink->peerDisconnectNeeded = FALSE;
            return AMPM_PHY_EV_DISCONNECT_REQ;
        }
    }
    else
    {
        return FSM_EVENT_NULL;
    }
}


fsm_event_t CsrBtAmpmPhyFsmConnected(void *arg1,
                                     void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Need to stop response timer */
    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    csrBtAmpmPhyCancelHciTimer(pPhyLink);

    pPhyLink->connected = TRUE;
    pPhyLink->startedLinkCreation = FALSE;
    pPhyLink->peerDisconnectNeeded = TRUE;

    /* Now have to obtain the link supervision timeout from the AMP */
    CsrBtAmpmSendHciReadLinkSupervisionTimeout(
        param->inst,
        CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
        pPhyLink, pPhyLink->ampPLinkHandle);
    csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);

    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtAmpmPhyFsmAmpCreatePhylinkRespTimeoutInd(void *arg1,
                                                          void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrMblk *pMsgBlk;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    csrBtAmpmPhyCancelHciTimer(pPhyLink);

    pPhyLink->sduIdentifier = CsrBtAmpmSduAllocIdentifier(param->inst);
    pMsgBlk =
        CsrBtAmpmSduCreateAmpDisconnectPhysicalLinkRequest(pPhyLink->
                                                           sduIdentifier, CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                                                           CsrBtAmpmPhyGetRemoteAmpId(pPhyLink));

    L2CA_DataWriteReq((l2ca_cid_t)pPhyLink->pBtDevice->cid, 0, pMsgBlk);
    csrBtAmpmPhyStartSduTimer(param->inst, pPhyLink);

    CsrBtAmpmSendHciDisconnectPhysicalLink(
        param->inst,
        CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
        (void *) pPhyLink, pPhyLink->ampPLinkHandle,
        CSR_BT_AMP_HCI_STATUS_CODE_CONNECTION_TERMINATED_BY_LOCAL_HOST);

    csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);
    pPhyLink->connected = FALSE;
    pPhyLink->startedLinkCreation = FALSE;

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmDeleteReq(void *arg1,
                                     void *arg2)
{
    CsrBtAmpmPhyLink *pDeletePhyLink;
    CsrUint8 numL2Chans = 0;
    CsrUint8 numPhysicalLinksUsingAmp = 0;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pDeletePhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Need to stop response timer */
    csrBtAmpmPhyCancelSduTimer(pDeletePhyLink);
    csrBtAmpmPhyCancelHciTimer(pDeletePhyLink);

    CsrBtAmpmLogicalPhyLinkEvent(param->inst, pDeletePhyLink,
                                 AMPM_LOGICAL_EV_PHY_DISCONNECT);

    /* If the L2Chan is capable of responding to L2CA it will do so
     * and delete itself otherwise the physical link will not trigger
     * any actions and it has to be deleted explicitly. This also
     * indicates to the physical link that it has to generate the DM
     * AMPM DISCONNECT REQ to DM.  to indicate that there are still
     * L2Chan instances associated with the physical link that are not
     * able to generate signalling into DM to indicate that they have
     * failed. */
    CsrBtAmpmL2capPhyEvent(param->inst, pDeletePhyLink,
                           AMPM_L2CAP_EV_PHYSICAL_LINK_DISCONNECT);

    numL2Chans = CsrBtAmpmL2capL2caChansUsingPhyLink(param->inst, pDeletePhyLink);
    if (0 < numL2Chans)
    {
        CsrBtAmpmL2capPhyEvent(param->inst, pDeletePhyLink,
                               AMPM_L2CAP_EV_DELETE_REQ);
    }

    if (pDeletePhyLink->sendPeerDisconnectRspNeeded)
    {
        csrBtAmpmPhySendAmpDisconnectPhysicalLinkResponse(param->inst,
                                                          pDeletePhyLink);
    }

    if (pDeletePhyLink->verifyResponseRequired)
    {
        pDeletePhyLink->verifyResponseRequired = FALSE;
        dm_ampm_verify_physical_link_rsp(&(pDeletePhyLink->bdAddr),
                                         pDeletePhyLink->verifyIdentifier,
                                         FALSE,   /* link doesn't exists */
                                         0, 0); /* dummy timer values */
    }

    /* We always have to inform DM of the physical link disconnecting
     * so that it can track when it can delete it's AMP instance that
     * it is using to track the buffer usage within the AMP. */
    if (1 < CsrBtAmpmListCount(param->inst->pPhyLinkListAnchor))
    {
        /* Need to determine how many other physical links are using
         * the same AMP as this one. */

        CsrBtAmpmPhyLink *pPhyLink =
            (CsrBtAmpmPhyLink *) param->inst->pPhyLinkListAnchor->head;

        while (NULL != pPhyLink)
        {
            CsrBtAmpmPhyLink *pNextPhyLink =
                (CsrBtAmpmPhyLink *) (((CsrBtAmpmGenListMember *) pPhyLink)->
                                      next);
            if (pDeletePhyLink != pPhyLink &&
                CsrBtAmpmPhyGetLocalAmpId(pDeletePhyLink) ==
                CsrBtAmpmPhyGetLocalAmpId(pPhyLink))
            {
                numPhysicalLinksUsingAmp++;
            }
            pPhyLink = pNextPhyLink;
        }
    }

    if(!pDeletePhyLink->dontDisconnectDm)
    {
        dm_ampm_disconnect_req(&(pDeletePhyLink->bdAddr),
                               CsrBtAmpmPhyGetLocalAmpId(pDeletePhyLink),
                               0x00, /* Success - no appropriate alternatives */
                               numPhysicalLinksUsingAmp);
    }

    csrBtAmpmPhyRemoveFsmInstance(param->inst, pDeletePhyLink);

    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtAmpmPhyFsmDiscoveryReq(void *arg1,
                                        void *arg2)
{
    CsrBtAmpmFsmParam *param;
    BD_ADDR_T *pBdAddr;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBdAddr = (BD_ADDR_T *) arg2;

    /* Kick the device discovery procedure. This will ensure the ACL
     * is brought up */
    CsrBtAmpmBtdevKickDiscovery(param->inst, *pBdAddr);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmPeerDisconnectingEstablishedLink(void *arg1,
                                                            void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    pPhyLink->peerDisconnectNeeded = FALSE;
    pPhyLink->sendPeerDisconnectRspNeeded = TRUE;

    CsrBtAmpmSendHciDisconnectPhysicalLink(
        param->inst,
        CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
        (void *) pPhyLink, pPhyLink->ampPLinkHandle,
        CSR_BT_AMP_HCI_STATUS_CODE_CONNECTION_TERMINATED_BY_LOCAL_HOST);

    csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);
    pPhyLink->connected = FALSE;
    pPhyLink->startedLinkCreation = FALSE;
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmLinkDropLsto(void *arg1,
                                        void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Need to stop response timers */
    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    csrBtAmpmPhyCancelHciTimer(pPhyLink);

    pPhyLink->hciDisconnectNeeded = FALSE;
    pPhyLink->peerDisconnectNeeded = FALSE;
    pPhyLink->connected = FALSE;
    pPhyLink->startedLinkCreation = FALSE;

    /* Link is gone. Clean up and do nothing else */
    return AMPM_PHY_EV_DELETE_REQ;
}


fsm_event_t CsrBtAmpmPhyFsmPalDisconnectingEstablishedLink(void *arg1,
                                                           void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequest(param->inst, pPhyLink);
    csrBtAmpmPhyStartSduTimer(param->inst, pPhyLink);
    pPhyLink->peerDisconnectNeeded = TRUE;
    pPhyLink->hciDisconnectNeeded = FALSE;
    pPhyLink->connected = FALSE;
    pPhyLink->startedLinkCreation = FALSE;

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmAmpmDisconnectingEstablishedLink(void *arg1,
                                                            void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    if (pPhyLink->peerDisconnectNeeded)
    {
        csrBtAmpmPhySendAmpDisconnectPhysicalLinkRequest(param->inst,
                                                         pPhyLink);
        csrBtAmpmPhyStartSduTimer(param->inst, pPhyLink);
    }

    if (pPhyLink->hciDisconnectNeeded)
    {
        CsrBtAmpmSendHciDisconnectPhysicalLink(
            param->inst,
            CsrBtAmpmPhyGetLocalAmpId(pPhyLink), (void *) pPhyLink, pPhyLink->ampPLinkHandle,
            CSR_BT_AMP_HCI_STATUS_CODE_CONNECTION_TERMINATED_BY_LOCAL_HOST);

        csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);
    }
    pPhyLink->connected = FALSE;
    pPhyLink->startedLinkCreation = FALSE;
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmSendGetRemoteAmpAssocReq(void *arg1,
                                                    void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrMblk *pMsgBlk;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    csrBtAmpmPhyCancelSduTimer(pPhyLink);

    /* We can continue attempting to establish the connection */
    pPhyLink->sduIdentifier = CsrBtAmpmSduAllocIdentifier(param->inst);
    pPhyLink->expectedResponseSdu = CSR_BT_AMPM_GET_AMP_ASSOC_RESPONSE;

    pMsgBlk =
        CsrBtAmpmSduCreateAmpGetAmpAssocRequest(pPhyLink->sduIdentifier,
                                                CsrBtAmpmPhyGetRemoteAmpId(pPhyLink));

    L2CA_DataWriteReq((l2ca_cid_t)pPhyLink->pBtDevice->cid, 0, pMsgBlk);
    csrBtAmpmPhyStartSduTimer(param->inst, pPhyLink);
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmSendHciWriteRemoteAmpAssoc(void *arg1,
                                                      void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    csrBtAmpmPhyCancelHciTimer(pPhyLink);

    /* We've started the process of establishing the physical link at the local
     * Pal, so from this point on we'll need to perform a disconnect at the pal
     * to correctly clear the link.
     */
    pPhyLink->hciDisconnectNeeded = TRUE;

    if (CsrBtAmpmLocalWriteRemoteAmpAssoc(param->inst,
                                          pPhyLink->pLocalAMPInfo,
                                          pPhyLink->ampPLinkHandle,
                                          pPhyLink->pRemoteAMPInfo->pAMPAssoc))
    {
        csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);
        return FSM_EVENT_NULL;
    }
    else
    {
        /* The local amp has rejected the remote AMP Assoc so we can not proceed
         * with the physical link establishment.
         */
        return AMPM_PHY_EV_HCI_WRITE_REMOTE_ASSOC_FAILURE;
    }
}

fsm_event_t CsrBtAmpmPhyFsmReadLocalAmpAssoc(void *arg1,
                                             void *arg2)
{
    const CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (const CsrBtAmpmPhyLink *) param->argp;

    CsrBtAmpmLocalReadAmpAssoc(param->inst, (void *) pPhyLink,
                               pPhyLink->pLocalAMPInfo, CSR_BT_AMPM_INVALID_SDU_IDENTIFIER,
                               pPhyLink->ampPLinkHandle, AMPM_LOCAL_EV_READ_ASSOC_REQ);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmAcceptorStartPhyLinkSuccess(void *arg1,
                                                       void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrMblk *pMsgBlk;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    pMsgBlk =
        CsrBtAmpmSduCreateAmpCreatePhysicalLinkResponse(pPhyLink->
                                                        ampCreatePhysicalRspSduIdentifier,
                                                        CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                                                        CsrBtAmpmPhyGetRemoteAmpId(pPhyLink),
                                                        CSR_BT_AMPM_CREATE_PHYSICAL_LINK_STARTED);

    L2CA_DataWriteReq((l2ca_cid_t)pPhyLink->pBtDevice->cid, 0, pMsgBlk);

    pPhyLink->peerDisconnectNeeded = TRUE;

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmAcceptorStartPhyLinkFail(void *arg1,
                                                    void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrMblk *pMsgBlk;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    pMsgBlk =
        CsrBtAmpmSduCreateAmpCreatePhysicalLinkResponse(pPhyLink->
                                                        ampCreatePhysicalRspSduIdentifier,
                                                        CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                                                        CsrBtAmpmPhyGetRemoteAmpId(pPhyLink),
                                                        CSR_BT_AMPM_CREATE_PHYSICAL_LINK_UNABLE_TO_START);

    L2CA_DataWriteReq((l2ca_cid_t)pPhyLink->pBtDevice->cid, 0, pMsgBlk);

    if (pPhyLink->hciDisconnectNeeded)
    {
        csrBtAmpmPhyCancelHciTimer(pPhyLink);
        CsrBtAmpmSendHciDisconnectPhysicalLink(
            param->inst,
            CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
            (void *) pPhyLink, pPhyLink->ampPLinkHandle,
            CSR_BT_AMP_HCI_STATUS_CODE_CONNECTION_TERMINATED_BY_LOCAL_HOST);
        csrBtAmpmPhyStartHciTimerDebug(param->inst, pPhyLink);
        return FSM_EVENT_NULL;
    }
    else
    {
        return AMPM_PHY_EV_DELETE_REQ;
    }
}


fsm_event_t CsrBtAmpmPhyFsmAcceptorStartPalDisconnected(void *arg1,
                                                        void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrMblk *pMsgBlk;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    csrBtAmpmPhyCancelHciTimer(pPhyLink);
    pMsgBlk =
        CsrBtAmpmSduCreateAmpCreatePhysicalLinkResponse(pPhyLink->
                                                        ampCreatePhysicalRspSduIdentifier,
                                                        CsrBtAmpmPhyGetLocalAmpId(pPhyLink),
                                                        CsrBtAmpmPhyGetRemoteAmpId(pPhyLink),
                                                        CSR_BT_AMPM_CREATE_PHYSICAL_LINK_UNABLE_TO_START);

    L2CA_DataWriteReq((l2ca_cid_t)pPhyLink->pBtDevice->cid, 0, pMsgBlk);

    return AMPM_PHY_EV_DELETE_REQ;
}


fsm_event_t CsrBtAmpmPhyFsmPalDisconnected(void *arg1,
                                           void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    pPhyLink->hciDisconnectNeeded = FALSE;
    csrBtAmpmPhyCancelHciTimer(pPhyLink);
    pPhyLink->connected = FALSE;
    pPhyLink->startedLinkCreation = FALSE;
    if (pPhyLink->peerDisconnectNeeded)
    {
        /* still waiting for the response from the Peer device to the
         * AMP DISCONNECT PHYSICAL LINK REQUEST
         */
        return FSM_EVENT_NULL;
    }
    else
    {
        /* Got all the responses that we expect so terminate */
        return AMPM_PHY_EV_DELETE_REQ;
    }
}


fsm_event_t CsrBtAmpmPhyFsmPeerDisconnected(void *arg1,
                                            void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    pPhyLink->peerDisconnectNeeded = FALSE;
    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    pPhyLink->connected = FALSE;
    pPhyLink->startedLinkCreation = FALSE;
    if (pPhyLink->hciDisconnectNeeded)
    {
        /* still waiting for the response from the pal to the
         * HCI PHYSICAL LINK DISCONNECT
         */
        return FSM_EVENT_NULL;
    }
    else
    {
        /* Got all the responses that we expect so terminate */
        return AMPM_PHY_EV_DELETE_REQ;
    }
}


fsm_event_t CsrBtAmpmPhyFsmPeerDisconnectReq(void *arg1,
                                             void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    pPhyLink->peerDisconnectNeeded = FALSE;
    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    pPhyLink->connected = FALSE;
    pPhyLink->startedLinkCreation = FALSE;
    if (pPhyLink->hciDisconnectNeeded)
    {
        /* still waiting for the response from the pal to the
         * HCI PHYSICAL LINK DISCONNECT
         */
        return FSM_EVENT_NULL;
    }
    else
    {
        /* Got all the responses that we expect so terminate */
        return AMPM_PHY_EV_DELETE_REQ;
    }
}


fsm_event_t CsrBtAmpmPhyFsmDisconnectTimeoutInd(void *arg1,
                                                void *arg2)
{
    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    /* Have not received all the expect responses, so just terminate */
    return AMPM_PHY_EV_DELETE_REQ;
}


fsm_event_t CsrBtAmpmPhyFsmConnectReqWhenConnected(void *arg1,
                                                   void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    /* Inform any waiting logical links that they can proceed */
    CsrBtAmpmL2capPhyEvent(param->inst, pPhyLink,
                           AMPM_L2CAP_EV_PHYSICAL_LINK_READY);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmSupervisionTimeout(void *arg1,
                                              void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpHciReadLinkSupervisionTimeoutReturn *pReturn;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;
    pReturn = (CsrBtAmpHciReadLinkSupervisionTimeoutReturn *) arg2;

    pPhyLink->linkSupervisionTimeout = pReturn->linkSupervisionTimeout;
    pPhyLink->haveReadLsto = TRUE;

    csrBtAmpmPhyCancelHciTimer(pPhyLink);

    /* Inform any waiting logical links that they can proceed */
    CsrBtAmpmL2capPhyEvent(param->inst, pPhyLink,
                           AMPM_L2CAP_EV_PHYSICAL_LINK_READY);

    /* Generate any verify physical link reponse that might be required */
    if (pPhyLink->verifyResponseRequired)
    {
        pPhyLink->verifyResponseRequired = FALSE;
        dm_ampm_verify_physical_link_rsp(&(pPhyLink->bdAddr),
                                         pPhyLink->verifyIdentifier,
                                         TRUE, /*link exists */
                                         CsrBtAmpmLocalGetBestEffortFlushTimeout(pPhyLink->pLocalAMPInfo),
                                         pPhyLink->linkSupervisionTimeout);
    }

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmPhyFsmPeerRspToDisconnect(void *arg1,
                                               void *arg2)
{
    CsrBtAmpmPhyLink *pPhyLink;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pPhyLink = (CsrBtAmpmPhyLink *) param->argp;

    pPhyLink->peerDisconnectNeeded = FALSE;
    csrBtAmpmPhyCancelSduTimer(pPhyLink);
    pPhyLink->connected = FALSE;
    pPhyLink->startedLinkCreation = FALSE;
    if (pPhyLink->hciDisconnectNeeded)
    {
        /* still waiting for the response from the pal to the
         * HCI PHYSICAL LINK DISCONNECT
         */
        return FSM_EVENT_NULL;
    }
    else
    {
        /* Got all the responses that we expect so terminate */
        return AMPM_PHY_EV_DELETE_REQ;
    }
}
