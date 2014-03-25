/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

typedef struct
{
    CsrBtAmpmGenListMember header;
    void *pSender;
    hci_op_code_t opcode;
    CsrBtAmpmPalHciEventReq *pAmpmHciPrim;
} csrBtAmpmLocalAmpHciCmd;

/* May receive multiple requests for the AMP Assoc so need to be able
 * to queue the requests. */
typedef struct
{
    CsrBtAmpmGenListMember header;
    void *pRequester;
    CsrUint8 sduIdentifier;
    CsrUint8 physicalLinkHandle;
} csrBtAmpmLocalGetAmpAssocRequest;

/* Performs the checking of a generic list element against the
 * supplied Amp Id and returns TRUE if they match. */
static CsrBool csrBtAmpmLocalMatchesId(CsrCmnListElm_t *pListEntry,
                                       void *pCriteria)
{
    CsrBtAmpmId id;
    const CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    id = *((const CsrBtAmpmId *) pCriteria);
    pLocalAmp = (const CsrBtAmpmLocalAmp *) pListEntry;

    if (pLocalAmp->pAmpInfo->id == id)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/* Performs the checking of a generic list element against the
 * supplied Amp Type and returns TRUE if they match. */
static CsrBool csrBtAmpmLocalMatchesType(CsrCmnListElm_t *pListEntry,
                                         void *pCriteria)
{
    CsrBtAmpmType type;
    const CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    type = *((const CsrBtAmpmType *) pCriteria);
    pLocalAmp = (const CsrBtAmpmLocalAmp *) pListEntry;

    if (pLocalAmp->pAmpInfo->type == type)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Performs the checking of a generic list element against the
 * requester pointer and returns TRUE if they match. */
static CsrBool csrBtAmpmLocalHciCmdMatchesRequester(const void *pCriteria,
                                                    const void *pListEntry)
{
    const csrBtAmpmLocalAmpHciCmd *pHciCmd;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    pHciCmd = (const csrBtAmpmLocalAmpHciCmd *) pListEntry;

    if (pHciCmd->pSender == pCriteria)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Starts a backstop HCI timer associated with the AMP so that
 * hopefully we don't sit just waiting forever */
static void csrBtAmpmLocalStartHciTimer(CsrBtAmpmInstance *inst,
                                        CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    if (inst->ampmTimersEnabled)
    {
        CsrBtAmpmFsmParam *param;
        param = (CsrBtAmpmFsmParam*)CsrPmemAlloc(sizeof(CsrBtAmpmFsmParam));
        param->inst = inst;
        param->argp = pLocalAmp;

        pLocalAmp->responseTid = CsrSchedTimerSet(CSR_BT_AMPM_HCI_RESPONSE_TIMEOUT,
                                                 CsrBtAmpmLocalHciResponseTimeoutInd,
                                                 0,
                                                 param);
    }
}

/* Cancels the HCI timer for the supplied local AMP */
static void csrBtAmpmLocalCancelHciTimer(CsrBtAmpmLocalAmp * pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    if (0 != pLocalAmp->responseTid)
    {
        CsrBtAmpmFsmParam *param;
        CsrUint16 mi;

        (void)CsrSchedTimerCancel(pLocalAmp->responseTid, &mi, (void **) &param);
        pLocalAmp->responseTid = 0;
        CsrPmemFree(param);
    }
}

/* Initialises the Assoc related parameters ready for
 * reading an assoc value from the amp. */
static void csrBtAmpmLocalInitialiseAssoc(CsrBtAmpmLocalAmp * pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    CSR_BT_AMPM_ASSERT(0 != pLocalAmp->pAmpInfo->maxHciAssocSize);

    /* Initialise the parameters associated with storing the Assoc */
    pLocalAmp->assocExpectedLength = 0;
    pLocalAmp->assocLengthRemaining = 0;
    pLocalAmp->assocLengthSoFar = 0;

    CsrPmemFree(pLocalAmp->ampAssoc);
    pLocalAmp->ampAssoc = (CsrUint8 *) CsrPmemAlloc(pLocalAmp->pAmpInfo->maxHciAssocSize);
    pLocalAmp->assocInsertOffset = 0;
}


/* Clears the Assoc related parmeters of the local amp */
static void csrBtAmpmLocalClearAssoc(CsrBtAmpmLocalAmp * pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    /* Initialise the parameters associated with storing the Assoc */
    pLocalAmp->assocExpectedLength = 0;
    pLocalAmp->assocLengthRemaining = 0;
    pLocalAmp->assocLengthSoFar = 0;
    CsrPmemFree(pLocalAmp->ampAssoc);
    pLocalAmp->ampAssoc = NULL;
    pLocalAmp->assocInsertOffset = 0;
    pLocalAmp->assocReadOffset = 0;
}


/* Will write the next segment of the Remote Amp Assoc previously
 * loaded into ampAssoc of the local amp. */
static void csrBtAmpmLocalWriteRemoteAssocSegment(CsrBtAmpmInstance *inst,
                                                  CsrBtAmpmLocalAmp * pLocalAmp)
{
    CsrBtAmpAmpAssoc hciAmpAssoc;

    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    CSR_BT_AMPM_ASSERT(0 != pLocalAmp->pAmpInfo->maxHciAssocSize);

    if (0 < pLocalAmp->assocLengthRemaining)
    {
        CsrUint16 segmentSize;

        if (CSR_BT_AMPM_MAX_WRITE_REMOTE_ASSOC_SEGMENT <
            pLocalAmp->assocLengthRemaining)
        {
            segmentSize = CSR_BT_AMPM_MAX_WRITE_REMOTE_ASSOC_SEGMENT;
        }
        else
        {
            segmentSize = pLocalAmp->assocLengthRemaining;
        }
        hciAmpAssoc.dataLen = segmentSize;

        /* The HCI Sending will free any pointers passed to it so we
         * need to take a copy of the Assoc data fragment.
         */
        hciAmpAssoc.data = (CsrUint8 *) CsrPmemAlloc(segmentSize);
        (void) CsrMemCpy(hciAmpAssoc.data,
                         pLocalAmp->ampAssoc + pLocalAmp->assocReadOffset,
                         segmentSize);

        CsrBtAmpmSendHciWriteRemoteAmpAssoc(inst,
                                            pLocalAmp->pAmpInfo->id,
                                            (void *) pLocalAmp,
                                            pLocalAmp->writeRemoteAssocPhysicalLinkHandle,
                                            pLocalAmp->assocLengthSoFar,
                                            pLocalAmp->assocLengthRemaining,
                                            hciAmpAssoc);

        pLocalAmp->assocLengthRemaining -= segmentSize;
        pLocalAmp->assocLengthSoFar += segmentSize;

        /* Move the read pointer for the next segment (if there is one) */
        pLocalAmp->assocReadOffset += segmentSize;
    }
}

/* Deletes the specified HCI command queue structure */
static void csrBtAmpmLocalDeleteHciCmd(csrBtAmpmLocalAmpHciCmd * pLocalAmp_HciCmd)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp_HciCmd != NULL);

    if (NULL != pLocalAmp_HciCmd->pAmpmHciPrim)
    {
        CsrPmemFree((void *) pLocalAmp_HciCmd->pAmpmHciPrim->data);
        CsrPmemFree((void *) pLocalAmp_HciCmd->pAmpmHciPrim);
    }
    CsrPmemFree((void *) pLocalAmp_HciCmd);
}



/* Will attempt to transmit any queued HCI commands for the specified
 * local AMP */
static void csrBtAmpmLocalTxQueuedHciCmd(CsrBtAmpmInstance *inst,
                                         CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    if (0 < CsrBtAmpmListCount(pLocalAmp->pHciCmdQueueAnchor))
    {
        csrBtAmpmLocalAmpHciCmd *pHciCmd = ((csrBtAmpmLocalAmpHciCmd *)
                                            (pLocalAmp->pHciCmdQueueAnchor->head));

        /* Elements stay on the queue until the response has been
         * received, so only transmit element if it has not been so
         * already. */
        if(pHciCmd->pAmpmHciPrim != NULL)
        {
            CsrBtAmpmUpstreamPalHciCommandInd(pHciCmd->pAmpmHciPrim->ampId,
                                              pHciCmd->pAmpmHciPrim->data,
                                              pLocalAmp->controlQueue);

            /* Attached data has been consumed. Free container and mark as sent */
            CsrPmemFree(pHciCmd->pAmpmHciPrim);
            pHciCmd->pAmpmHciPrim = NULL;
        }
    }
}

/* Runs the FSM associated with the specified local amp instance */
static void csrBtAmpmLocalFsmRun(CsrBtAmpmInstance *inst,
                                 CsrBtAmpmLocalAmp *pLocalAmp,
                                 fsm_event_t event,
                                 void *param2)
{
    fsm_result_t fsm_result;
    CsrBtAmpmFsmParam param;

    /* Setup structured arg1 */
    param.inst = inst;
    param.argp = pLocalAmp;

    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    fsm_result =
        fsm_16bit_run(&csr_bt_ampm_local_fsm, &(pLocalAmp->state), event,
                      (void *) &param, param2);

    CSR_BT_AMPM_ASSERT(FSM_RESULT_OK == fsm_result);

    /* Depending on the configuration, CSR_BT_AMPM_ASSERT may be invalid and thus fsm_result is unsed */
    CSR_UNUSED(fsm_result);
}

/* Will generate an appropriate AMP INFO RESPONSE SDU to the peer amp
 * manager represented by the Bt Device instance */
static void csrBtAmpmLocalSendAmpInfoToRequester(CsrBtAmpmLocalAmp *pLocalAmp,
                                                 CsrBtAmpmBtdev *pBtDevice,
                                                 CsrUint8 sduIdentifier)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);
    CSR_BT_AMPM_ASSERT(pLocalAmp->pAmpInfo != NULL);

    if (NULL != pBtDevice)
    {
        CsrMblk *pMsgBlk;

        pMsgBlk =
            CsrBtAmpmSduCreateAmpGetInfoResponse(sduIdentifier,
                                                 CSR_BT_AMPM_GET_INFO_RSP_STATUS_SUCCESS,
                                                 (const CsrBtAmpmDataInfo *) (pLocalAmp->pAmpInfo));
        L2CA_DataWriteReq((l2ca_cid_t)pBtDevice->cid, 0, pMsgBlk);
    }
}

/* Will check if there are any outstanding requests for the AMP
 * ASSOC and if so then trigger reading it. */
static void csrBtAmpmLocalTriggerReadAmpAssocIfRequired(CsrBtAmpmInstance *inst,
                                                        CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    if (!CsrBtAmpmListIsEmpty(pLocalAmp->pReadAmpAssocQueueAnchor))
    {
        csrBtAmpmLocalGetAmpAssocRequest *pGetAmpAssocReq =
            (csrBtAmpmLocalGetAmpAssocRequest *) pLocalAmp->
            pReadAmpAssocQueueAnchor->head;
        csrBtAmpmLocalFsmRun(inst, pLocalAmp, AMPM_LOCAL_EV_READ_ASSOC_REQ,
                             (void *) &(pGetAmpAssocReq->physicalLinkHandle));
    }
}


/* Local AMP list element initialiser */
static void csrBtAmpmLocalAmpListInit(CsrCmnListElm_t *elem)
{
    CsrBtAmpmLocalAmp *new;
    new = (CsrBtAmpmLocalAmp*)elem;

    CsrMemSet(&new->state,
              0,
              sizeof(CsrBtAmpmLocalAmp) - sizeof(CsrCmnListElm_t));

    new->state = AMPM_LOCAL_ST_NULL;
    new->pAmpInfo = CsrPmemZalloc(sizeof(CsrBtAmpmDataInfo));;
    new->pAmpInfo->status = CSR_BT_AMP_AMP_STATUS_NO_CAPACITY_AVAILABLE_FOR_BLUETOOTH_OPERATION;
}

/* Local AMP list element deinitialiser */
static void csrBtAmpmLocalAmpListFree(CsrCmnListElm_t *elem)
{
    CsrBtAmpmLocalAmp* die;
    die = (CsrBtAmpmLocalAmp*)elem;

    csrBtAmpmLocalCancelHciTimer(die);

    while (!CsrBtAmpmListIsEmpty(die->pHciCmdQueueAnchor))
    {
        CsrBtAmpmGenListMember *pListMember =
            CsrBtAmpmListRemoveHead(die->pHciCmdQueueAnchor);
        csrBtAmpmLocalDeleteHciCmd((csrBtAmpmLocalAmpHciCmd *) pListMember);
    }
    CsrBtAmpmListDestroyAnchor(&(die->pHciCmdQueueAnchor));

    while (!CsrBtAmpmListIsEmpty(die->pReadAmpAssocQueueAnchor))
    {
        CsrBtAmpmGenListMember *pListMember =
            CsrBtAmpmListRemoveHead(die->pReadAmpAssocQueueAnchor);
        CsrPmemFree((csrBtAmpmLocalGetAmpAssocRequest *) pListMember);
    }
    CsrBtAmpmListDestroyAnchor(&(die->pReadAmpAssocQueueAnchor));

    CsrPmemFree(die->ampAssoc);
    CsrPmemFree(die->pAmpInfo);
}

void CsrBtAmpmLocalInit(CsrBtAmpmInstance *inst)
{
    CsrCmnListInit(&inst->localAmpList, 0,
                   csrBtAmpmLocalAmpListInit,
                   csrBtAmpmLocalAmpListFree);
}


void CsrBtAmpmLocalDeinit(CsrBtAmpmInstance *inst)
{
    while(CsrCmnListGetCount(&inst->localAmpList) > 0)
    {
        /* We must remove elements manually as we would otherwise kick
         * the FSM */
        CsrBtAmpmLocalAmp *elem = (CsrBtAmpmLocalAmp*)CsrCmnListGetFirst(&inst->localAmpList);
        elem->initialised = FALSE;
        CsrBtAmpmLocalDelete(inst, elem);
    }
    CsrCmnListDeinit(&inst->localAmpList);
}

CsrBtAmpmLocalAmp *CsrBtAmpmLocalNew(CsrBtAmpmInstance *inst,
                                     CsrBtAmpmId id,
                                     CsrBtAmpmType type,
                                     CsrSchedQid dataQid,
                                     CsrSchedQid controlQid,
                                     CsrUint32 flags)
{
    CsrBtAmpmLocalAmp *pLocalAmp = NULL;

    /* Check we have not already reached the maximum number of BT
     * devices we can support. */
    if (CSR_BT_AMPM_MAX_LOCAL_AMPS > CsrCmnListGetCount(&inst->localAmpList))
    {
        pLocalAmp = (CsrBtAmpmLocalAmp*)CsrCmnListElementAddFirst(&inst->localAmpList,
                                                                  sizeof(CsrBtAmpmLocalAmp));
        pLocalAmp->state = AMPM_LOCAL_ST_NULL;
        pLocalAmp->dataQueue = dataQid;
        pLocalAmp->controlQueue = controlQid;
        pLocalAmp->pAmpInfo->id = id;
        pLocalAmp->pAmpInfo->type = type;
        pLocalAmp->pHciCmdQueueAnchor = CsrBtAmpmListCreateAnchor();
        pLocalAmp->pReadAmpAssocQueueAnchor = CsrBtAmpmListCreateAnchor();

        /* Inform the PAL that it's been registered. This must happen
         * before the FSM is cranked */
        CsrBtAmpmUpstreamPalRegisterCfm(controlQid,
                                        type,
                                        id,
                                        CSR_BT_AMPM_RESULT_SUCCESS,
                                        0); /* flags */

        /* Trigger reading of the amp info to populate the above data */
        /* Call the state machine */
        csrBtAmpmLocalFsmRun(inst,
                             pLocalAmp,
                             AMPM_LOCAL_EV_READ_INFO_REQ,
                             NULL);
    }
    else
    {
        /* Inform the PAL that we failed */
        CsrBtAmpmUpstreamPalRegisterCfm(controlQid,
                                        type,
                                        id,
                                        CSR_BT_AMPM_RESULT_TOO_MANY_PALS,
                                        0); /* flags */

    }
    return (pLocalAmp);
}


void CsrBtAmpmLocalDelete(CsrBtAmpmInstance *inst,
                          CsrBtAmpmLocalAmp *pLocalAmp)
{
    CsrBool ampHadInitialised = FALSE;

    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    ampHadInitialised = pLocalAmp->initialised;
    CsrCmnListElementRemove(&inst->localAmpList,
                            (CsrCmnListElm_t*)pLocalAmp);

    /* If any other Bt Devices might have been aware of the Local AMP
     * then prompt them to update the peer devices with the new local
     * AMP list. */
    if (ampHadInitialised)
    {
        CsrBtAmpmBtdevLocalAmpChangeInd(inst);
    }
}


void CsrBtAmpmLocalStatusChange(CsrBtAmpmInstance *inst,
                                CsrBtAmpmId id,
                                CsrBtAmpAmpStatus ampStatus)
{
    CsrBtAmpmLocalAmp *pLocalAmp;

    /* Find the required element based on the Amp Id */
    pLocalAmp = (CsrBtAmpmLocalAmp*)CsrCmnListSearch(&inst->localAmpList,
                                                     csrBtAmpmLocalMatchesId,
                                                     &id);

    if (NULL != pLocalAmp)
    {
        if (ampStatus != pLocalAmp->pAmpInfo->status)
        {
            pLocalAmp->pAmpInfo->status = ampStatus;
            /* Need to trigger updating the peer AMP Managers with the
             * status change as this may affect their AMP selection
             * when setting up a connection to this device. */
            CsrBtAmpmBtdevLocalAmpChangeInd(inst);
        }
    }
}


CsrBtAmpmId CsrBtAmpmLocalGetId(const CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    return (pLocalAmp->pAmpInfo->id);
}


CsrBtAmpmType CsrBtAmpmLocalGetType(const CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    return (pLocalAmp->pAmpInfo->type);
}


CsrUint8 CsrBtAmpmLocalGetStatus(const CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    return (pLocalAmp->pAmpInfo->status);
}


CsrBtAmpmLocalAmp *CsrBtAmpmLocalGetById(CsrBtAmpmInstance *inst,
                                         CsrBtAmpmId id)
{
    return (CsrBtAmpmLocalAmp*)CsrCmnListSearch(&inst->localAmpList,
                                                csrBtAmpmLocalMatchesId,
                                                &id);
}


CsrBtAmpmId CsrBtAmpmLocalGetNewId(CsrBtAmpmInstance *inst)
{
    CsrBtAmpmId id;
    CsrBtAmpmLocalAmp *local;

    for(id = 1; id < 255; id++)
    {
        local = (CsrBtAmpmLocalAmp*)CsrCmnListSearch(&inst->localAmpList,
                                                     csrBtAmpmLocalMatchesId,
                                                     &id);
        if(local == NULL)
        {
            /* Unused */
            return id;
        }
    }

    /* We should not reach this point */
    CSR_BT_AMPM_ASSERT(id != 0xFF);
    return 0;
}

CsrBtAmpmLocalAmp *CsrBtAmpmLocalGetByType(CsrBtAmpmInstance *inst,
                                           CsrBtAmpmType type)
{
    return (CsrBtAmpmLocalAmp*)CsrCmnListSearch(&inst->localAmpList,
                                                csrBtAmpmLocalMatchesType,
                                                &type);
}


CsrUint16 CsrBtAmpmLocalMaxMtuSupportedbyAmps(CsrBtAmpmInstance *inst)
{
    CsrBtAmpmLocalAmp *pLocalAmp;
    CsrUint16 maxMtu = CSR_BT_AMPM_L2CONF_MTU_MPS;

    if(CsrCmnListGetCount(&inst->localAmpList) > 0)
    {
        for(pLocalAmp = (CsrBtAmpmLocalAmp*)CsrCmnListGetFirst(&inst->localAmpList);
            pLocalAmp != NULL;
            pLocalAmp = (CsrBtAmpmLocalAmp*)pLocalAmp->next)
        {
            CSR_BT_AMPM_ASSERT(pLocalAmp->pAmpInfo != NULL);

            if (maxMtu < pLocalAmp->pAmpInfo->maxPDUSize)
            {
                /* Ensure we don't exceed 16 bits... */
                maxMtu = (CsrUint16)(CSRMIN((pLocalAmp->pAmpInfo->maxPDUSize),
                                            0xFFFF));
            }
        }
    }
    return (maxMtu);
}


CsrUint16 CsrBtAmpmLocalGetMaxPduSize(CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    CSR_BT_AMPM_ASSERT(pLocalAmp->initialised);

    return ((CsrUint16) (pLocalAmp->pAmpInfo->maxPDUSize));
}


CsrUint32 CsrBtAmpmLocalGetMaxBandwidth(CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    CSR_BT_AMPM_ASSERT(pLocalAmp->initialised);

    return (pLocalAmp->pAmpInfo->totalBandwidth);
}


CsrUint32 CsrBtAmpmLocalGetBestEffortFlushTimeout(CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    CSR_BT_AMPM_ASSERT(pLocalAmp->initialised);

    return (pLocalAmp->pAmpInfo->bestEffortFlushTimeout);
}


void CsrBtAmpmLocalReadAmpInfo(CsrBtAmpmBtdev *pRequester,
                               CsrUint8 sduIdentifier,
                               CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pRequester != NULL);
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    /* We should always be able to respond immediately to a read amp
     * info request from a peer AMP Manager as the BtDevice instances
     * can only "see" a local AMP after it is initialised and we've
     * read the amp info. */
    csrBtAmpmLocalSendAmpInfoToRequester(pLocalAmp, pRequester,
                                         sduIdentifier);
}

void CsrBtAmpmLocalHciNop(CsrBtAmpmInstance *inst,
                          CsrBtAmpmLocalAmp *pLocalAmp)
{
    /* Nothing to do really -- except inject event into FSM */
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    csrBtAmpmLocalFsmRun(inst, pLocalAmp, AMPM_LOCAL_EV_HCI_NOP_EVENT, NULL);
}

void CsrBtAmpmLocalReadAmpAssoc(CsrBtAmpmInstance *inst,
                                void *pRequester,
                                CsrBtAmpmLocalAmp *pLocalAmp,
                                CsrUint8 sduIdentifier,
                                CsrUint8 physicalLinkHandle,
                                fsm_event_t fsmEvent )
{
    csrBtAmpmLocalGetAmpAssocRequest *pGetAmpAssocReq =
        CsrPmemAlloc(sizeof(*pGetAmpAssocReq));

    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    pGetAmpAssocReq->pRequester = pRequester;
    pGetAmpAssocReq->sduIdentifier = sduIdentifier;
    pGetAmpAssocReq->physicalLinkHandle = physicalLinkHandle;
    CsrBtAmpmListAddTail(pLocalAmp->pReadAmpAssocQueueAnchor,
                         (CsrBtAmpmGenListMember *) pGetAmpAssocReq);
    csrBtAmpmLocalFsmRun(inst, pLocalAmp, fsmEvent,
                         (void *) &physicalLinkHandle);
}

CsrBool CsrBtAmpmLocalWriteRemoteAmpAssoc(CsrBtAmpmInstance *inst,
                                          CsrBtAmpmLocalAmp *pLocalAmp,
                                          CsrUint8 physicalLinkHandle,
                                          CsrBtAmpmAmpAssoc *pAssoc)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    CSR_BT_AMPM_ASSERT(pAssoc != NULL);

    if (pLocalAmp->pAmpInfo->maxHciAssocSize >= pAssoc->length)
    {
        csrBtAmpmLocalClearAssoc(pLocalAmp);
        pLocalAmp->assocLengthRemaining = pAssoc->length;
        pLocalAmp->assocLengthSoFar = 0;
        CsrPmemFree(pLocalAmp->ampAssoc);
        pLocalAmp->ampAssoc = CsrMemDup(pAssoc->data, pAssoc->length);
        pLocalAmp->assocReadOffset = 0;
        pLocalAmp->writeRemoteAssocPhysicalLinkHandle = physicalLinkHandle;
        csrBtAmpmLocalFsmRun(inst, pLocalAmp,
                             AMPM_LOCAL_EV_WRITE_REMOTE_ASSOC_REQ, NULL);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Main transmit function: Put element on queue and poke the
* transmitter */
void CsrBtAmpmLocalHciTxCmd(CsrBtAmpmInstance *inst,
                            CsrBtAmpmId id,
                            void *pRequester,
                            CsrUint8 *pHciPrim)
{
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(pHciPrim != NULL);

    pLocalAmp = CsrBtAmpmLocalGetById(inst, id);

    if (NULL != pLocalAmp)
    {
        csrBtAmpmLocalAmpHciCmd *pHciCmd = CsrPmemZalloc(sizeof(csrBtAmpmLocalAmpHciCmd));
        CsrBtAmpmPalHciEventReq *pAmpmHciPrim = CsrPmemZalloc(sizeof(CsrBtAmpmPalHciEventReq));

        pAmpmHciPrim->ampId = id;
        pAmpmHciPrim->data = (CsrUint8 *) pHciPrim;

        pHciCmd->pSender = pRequester;
        pHciCmd->opcode = *((CsrUint16 *) pHciPrim);
        pHciCmd->pAmpmHciPrim = pAmpmHciPrim;

        CsrBtAmpmListAddTail(pLocalAmp->pHciCmdQueueAnchor,
                             (CsrBtAmpmGenListMember *) pHciCmd);

        /* Poke the queue transmitter */
        csrBtAmpmLocalTxQueuedHciCmd(inst, pLocalAmp);
    }
}


void CsrBtAmpmLocalRemoveHciTxCmds(CsrBtAmpmInstance *inst,
                                   CsrBtAmpmId id,
                                   void *pRequester)
{
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(pRequester != NULL);

    pLocalAmp = CsrBtAmpmLocalGetById(inst, id);

    if (NULL != pLocalAmp)
    {
        if (0 < CsrBtAmpmListCount(pLocalAmp->pHciCmdQueueAnchor))
        {
            csrBtAmpmLocalAmpHciCmd *pHciCmd =
                (csrBtAmpmLocalAmpHciCmd *)
                CsrBtAmpmListFindMatchingMember(pLocalAmp->pHciCmdQueueAnchor,
                                                csrBtAmpmLocalHciCmdMatchesRequester,
                                                (const void *)pRequester);

            while (NULL != pHciCmd)
            {
                CsrBtAmpmListRemoveMember(pLocalAmp->pHciCmdQueueAnchor,
                                          (CsrBtAmpmGenListMember *) pHciCmd);

                csrBtAmpmLocalDeleteHciCmd(pHciCmd);

                pHciCmd =
                    (csrBtAmpmLocalAmpHciCmd *)
                    CsrBtAmpmListFindMatchingMember(pLocalAmp->
                                                    pHciCmdQueueAnchor, csrBtAmpmLocalHciCmdMatchesRequester,
                                                    pRequester);
            }
        }
    }
}


void *CsrBtAmpmLocalStatusForQueuedHciCmd(CsrBtAmpmInstance *inst,
                                          CsrBtAmpmLocalAmp *pLocalAmp,
                                          CsrUint16 opcode)
{
    void *pRequester = NULL;

    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    if (0 < CsrBtAmpmListCount(pLocalAmp->pHciCmdQueueAnchor))
    {
        csrBtAmpmLocalAmpHciCmd *pHciCmd =
            (csrBtAmpmLocalAmpHciCmd *) (pLocalAmp->pHciCmdQueueAnchor->head);

        if (opcode == pHciCmd->opcode)
        {
            pHciCmd = ((csrBtAmpmLocalAmpHciCmd*)
                       CsrBtAmpmListRemoveHead(pLocalAmp->pHciCmdQueueAnchor));

            /* As we only transmit one command and then wait for the
             * status response as long as the op codes match we can
             * assume that this is the command that the status relates
             * to. */
            pRequester = pHciCmd->pSender;
            csrBtAmpmLocalDeleteHciCmd(pHciCmd);

            /* Is there anything else to transmit? */
            csrBtAmpmLocalTxQueuedHciCmd(inst, pLocalAmp);
        }
    }
    return (pRequester);
}

void *CsrBtAmpmLocalRemoveQueuedHciCmd(CsrBtAmpmInstance *inst,
                                       CsrBtAmpmLocalAmp *pLocalAmp,
                                       CsrUint16 opcode)
{
    void *pRequester = NULL;

    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    if (0 < CsrBtAmpmListCount(pLocalAmp->pHciCmdQueueAnchor))
    {
        csrBtAmpmLocalAmpHciCmd *pHciCmd;
        pHciCmd = (csrBtAmpmLocalAmpHciCmd*)(pLocalAmp->pHciCmdQueueAnchor->head);

        if (opcode == pHciCmd->opcode)
        {
            pHciCmd = ((csrBtAmpmLocalAmpHciCmd *)
                       CsrBtAmpmListRemoveHead(pLocalAmp->pHciCmdQueueAnchor));
            pRequester = pHciCmd->pSender;
            csrBtAmpmLocalDeleteHciCmd(pHciCmd);

            /* Is there anything else to transmit? */
            csrBtAmpmLocalTxQueuedHciCmd(inst, pLocalAmp);
        }
    }
    return (pRequester);
}

/* Timed event function for lost HCI commands. Note that this is only
 * invoked if timers are enabled (ie. test interface has not disabled
 * timers) */
void CsrBtAmpmLocalHciQueueRemovalTimeoutInd(CsrUint16 ignored,
                                             void *argp)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;
    csrBtAmpmLocalAmpHciCmd *pHciCmd;

    CSR_BT_AMPM_ASSERT(argp != NULL);

    param = (CsrBtAmpmFsmParam *) argp;
    pLocalAmp = (CsrBtAmpmLocalAmp *)CsrCmnListSearch(&param->inst->localAmpList,
                                                      (CsrCmnListSearchFunc_t)CsrBtAmpmListEntryMatchesPtr,
                                                      param->argp);
    if (NULL != pLocalAmp)
    {
        pHciCmd = ((csrBtAmpmLocalAmpHciCmd *)
                   CsrBtAmpmListRemoveHead(pLocalAmp->pHciCmdQueueAnchor));
        csrBtAmpmLocalDeleteHciCmd(pHciCmd);

        /* Is there anything else to transmit? */
        csrBtAmpmLocalTxQueuedHciCmd(param->inst, pLocalAmp);
    }
    CsrPmemFree(param);
}

void CsrBtAmpmLocalHciResponseTimeoutInd(CsrUint16 ignored, void *argp)
{
    CsrBtAmpmLocalAmp *pLocalAmp;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(argp != NULL);
    param = (CsrBtAmpmFsmParam *) argp;

    /* Have to check that the instance data is still valid */
    pLocalAmp = (CsrBtAmpmLocalAmp*)CsrCmnListSearch(&param->inst->localAmpList,
                                                     (CsrCmnListSearchFunc_t)CsrBtAmpmListEntryMatchesPtr,
                                                     param->argp);
    if (NULL != pLocalAmp)
    {
        pLocalAmp->responseTid = 0;

        /* If timers are disabled then ignore it */
        if (((CsrBtAmpmInstance*)(param->inst))->ampmTimersEnabled)
        {
            csrBtAmpmLocalFsmRun(param->inst,
                                 pLocalAmp,
                                 AMPM_LOCAL_EV_HCI_RESPONSE_TIMEOUT_IND,
                                 NULL);

            /* We also need to remove the HCI command from the tx
             * queue otherwise it will prevent the sending of any
             * further commands. */
            CsrBtAmpmLocalRemoveHciTxCmds(param->inst,
                                          pLocalAmp->pAmpInfo->id,
                                          (void *) pLocalAmp);
        }
    }
    CsrPmemFree(param);
}


void CsrBtAmpmLocalReadLocalAmpInfoTimeoutInd(CsrUint16 ignored,
                                              void *argp)
{
    CsrBtAmpmLocalAmp *pLocalAmp;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(argp != NULL);
    param = (CsrBtAmpmFsmParam *) argp;

    pLocalAmp = (CsrBtAmpmLocalAmp*)CsrCmnListSearch(&param->inst->localAmpList,
                                                     (CsrCmnListSearchFunc_t)CsrBtAmpmListEntryMatchesPtr,
                                                     param->argp);
    if(pLocalAmp != NULL)
    {
        pLocalAmp->responseTid = 0;
    }

    if (NULL != pLocalAmp && !pLocalAmp->initialised)
    {
        /* If timers are disabled then ignore it */
        if (((CsrBtAmpmInstance*)(param->inst))->ampmTimersEnabled)
        {
            csrBtAmpmLocalFsmRun(param->inst,
                                 pLocalAmp,
                                 AMPM_LOCAL_EV_READ_INFO_REQ,
                                 NULL);
        }
    }
    CsrPmemFree(param);
}

void CsrBtAmpmLocalHciReadAmpInfoFailure(CsrBtAmpmInstance *inst,
                                         CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    csrBtAmpmLocalFsmRun(inst, pLocalAmp, AMPM_LOCAL_EV_HCI_READ_INFO_FAILURE,
                         NULL);
}


void CsrBtAmpmLocalHciReadAmpAssocFailure(CsrBtAmpmInstance *inst,
                                          CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    csrBtAmpmLocalFsmRun(inst, pLocalAmp,
                         AMPM_LOCAL_EV_HCI_READ_ASSOC_FAILURE, NULL);
}


void CsrBtAmpmLocalHciWriteRemoteAmpAssocFailure(CsrBtAmpmInstance *inst,
                                                 CsrBtAmpmLocalAmp *pLocalAmp)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);

    csrBtAmpmLocalFsmRun(inst, pLocalAmp, AMPM_LOCAL_EV_REMOTE_ASSOC_FAILURE,
                         NULL);
}

void CsrBtAmpmLocalHciReadLocalAmpInfo(CsrBtAmpmInstance *inst,
                                       CsrBtAmpmLocalAmp *pLocalAmp,
                                       CsrBtAmpHciReadLocalAmpInfoReturn *pReadLocalAmpInfoReturn)
{
    fsm_event_t this_event = AMPM_LOCAL_EV_HCI_READ_INFO_FAILURE;   /* Assume failure */
    void *pTransitionData = NULL;

    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    CSR_BT_AMPM_ASSERT(pReadLocalAmpInfoReturn != NULL);

    if (!pReadLocalAmpInfoReturn->status)
    {
        this_event = AMPM_LOCAL_EV_HCI_READ_INFO_SUCCESS;
        pTransitionData = (void *) pReadLocalAmpInfoReturn;
    }

    csrBtAmpmLocalFsmRun(inst, pLocalAmp, this_event, pTransitionData);
}

void CsrBtAmpmLocalHciReadLocalAmpAssoc(CsrBtAmpmInstance *inst,
                                        CsrBtAmpmLocalAmp *pLocalAmp,
                                        CsrBtAmpHciReadLocalAmpAssocReturn *pReadLocalAmpAssocReturn)
{
    fsm_event_t this_event = AMPM_LOCAL_EV_HCI_READ_ASSOC_FAILURE;  /* Assume failure */
    void *pTransitionData = NULL;

    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    CSR_BT_AMPM_ASSERT(pReadLocalAmpAssocReturn != NULL);

    if (!pReadLocalAmpAssocReturn->status)
    {
        this_event = AMPM_LOCAL_EV_HCI_READ_ASSOC_SUCCESS;
        pTransitionData = (void *) pReadLocalAmpAssocReturn;
    }

    csrBtAmpmLocalFsmRun(inst, pLocalAmp, this_event, pTransitionData);
}


void CsrBtAmpmLocalHciWriteRemoteAmpAssoc(CsrBtAmpmInstance *inst,
                                          CsrBtAmpmLocalAmp *pLocalAmp,
                                          CsrBtAmpHciWriteRemoteAmpAssocReturn *pWriteRemoteAmpAssocReturn)
{
    fsm_event_t this_event = AMPM_LOCAL_EV_REMOTE_ASSOC_FAILURE;    /* Assume failure */

    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    CSR_BT_AMPM_ASSERT(pWriteRemoteAmpAssocReturn != NULL);

    if (!pWriteRemoteAmpAssocReturn->status)
    {
        this_event = AMPM_LOCAL_EV_REMOTE_ASSOC_SUCCESS;
    }

    csrBtAmpmLocalFsmRun(inst, pLocalAmp, this_event, NULL);
}

void CsrBtAmpmLocalRegisterReq(CsrBtAmpmInstance *inst,
                               const CsrBtAmpmPalRegisterReq *pAmpRegisterReq)
{
    CsrBtAmpmId id;

    CSR_BT_AMPM_ASSERT(pAmpRegisterReq != NULL);
    id = CsrBtAmpmLocalGetNewId(inst);

    if(id != 0)
    {
        (void)CsrBtAmpmLocalNew(inst,
                                id,
                                pAmpRegisterReq->ampType,
                                pAmpRegisterReq->dataQid,
                                pAmpRegisterReq->controlQid,
                                pAmpRegisterReq->flags);
    }
    else
    {
        /* Inform app that we failed to register an ID */
        CsrBtAmpmUpstreamPalRegisterCfm(pAmpRegisterReq->controlQid,
                                        pAmpRegisterReq->ampType,
                                        CSR_BT_AMPM_INVALID_ID,
                                        CSR_BT_AMPM_RESULT_NO_ID,
                                        0); /* flags */
    }
}

void CsrBtAmpmLocalDeregisterReq(CsrBtAmpmInstance *inst,
                                 const CsrBtAmpmPalDeregisterReq *pAmpDeregisterReq)
{
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(pAmpDeregisterReq != NULL);

    pLocalAmp = CsrBtAmpmLocalGetById(inst, pAmpDeregisterReq->ampId);

    if (NULL != pLocalAmp)
    {
        /* Inform PAL that it has been removed (before it's deleted) */
        CsrBtAmpmUpstreamPalDeregisterCfm(pLocalAmp->controlQueue,
                                          pLocalAmp->pAmpInfo->id,
                                          CSR_BT_AMPM_RESULT_SUCCESS);


        CsrBtAmpmPhyLocalAmpRemoved(inst, pAmpDeregisterReq->ampId);
        CsrBtAmpmLocalDelete(inst, pLocalAmp);
    }
}

CsrBool CsrBtAmpmLocalPowerOnRes(CsrBtAmpmInstance *inst,
                             CsrBtAmpmPowerOnRes *pAmpPowerOnRes)
{
    CsrBtAmpmLocalAmp *pLocalAmp = CsrBtAmpmLocalGetById(inst,
                                            CSR_BT_AMPM_L2CA_TO_AMP_ID(pAmpPowerOnRes->localId));
    
    if( (NULL != pLocalAmp) && (AMPM_LOCAL_ST_POWER_ON_READ_ASSOC == pLocalAmp->state) )
    {
        CsrUint8 physicalLinkHandle = CSR_BT_AMPM_INVALID_PHYSICAL_LINK_HANDLE;
        fsm_event_t fsmEvent = AMPM_LOCAL_EV_POWER_ON_FAILURE;
        
        if( pAmpPowerOnRes->complete )
        {
            fsmEvent = AMPM_LOCAL_EV_POWER_ON_OK;
        }
        
        csrBtAmpmLocalFsmRun(inst, pLocalAmp, fsmEvent, &physicalLinkHandle);
        
        return TRUE;
    }
    return FALSE;
}

fsm_event_t CsrBtAmpmLocalFsmReadInfoReqPending(void *arg1,
                                                void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;

    CsrBtAmpmSendHciReadLocalAmpInfo(param->inst,
                                     pLocalAmp->pAmpInfo->id,
                                     (void *) pLocalAmp);
    csrBtAmpmLocalStartHciTimer(param->inst, pLocalAmp);
    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtAmpmLocalFsmHciNopCmdComplete(void *arg1,
                                               void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;
    if((pLocalAmp != NULL) &&
       !pLocalAmp->initialised)
    {
        /* Cancel timer and then inject the standard read command */
        csrBtAmpmLocalCancelHciTimer(pLocalAmp);
        return AMPM_LOCAL_EV_READ_INFO_REQ;
    }
    else
    {
        /* NOP event wasn't useful for us */
        return FSM_EVENT_NULL;
    }
}


fsm_event_t CsrBtAmpmLocalFsmReadInfoSuccess(void *arg1,
                                             void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;
    CsrBtAmpmDataInfo *pAmpInfo = NULL;
    CsrBtAmpHciReadLocalAmpInfoReturn *pReadLocalAmpInfoReturn;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;
    pReadLocalAmpInfoReturn = (CsrBtAmpHciReadLocalAmpInfoReturn *) arg2;

    csrBtAmpmLocalCancelHciTimer(pLocalAmp);

    pLocalAmp->initialised = TRUE;
    pAmpInfo = pLocalAmp->pAmpInfo;
    pAmpInfo->status = pReadLocalAmpInfoReturn->ampStatus;
    pAmpInfo->totalBandwidth = pReadLocalAmpInfoReturn->totalBandwidth;
    pAmpInfo->guaranteedBandwidth =
        pReadLocalAmpInfoReturn->maxGuaranteedBandwidth;
    pAmpInfo->minLatency = pReadLocalAmpInfoReturn->minLatency;
    pAmpInfo->maxFlushTimeout = pReadLocalAmpInfoReturn->maxFlushTimeout;
    pAmpInfo->bestEffortFlushTimeout =
        pReadLocalAmpInfoReturn->bestEffortFlushTimeout;

    /* Difference in the field sizes between the AMP and the HCI spec
     * will need to be resolved but for the moment assume that we'll
     * only get sizes that fit into the smaller generic amp field
     * size */
    CSR_BT_AMPM_ASSERT(0xFFFF >= pReadLocalAmpInfoReturn->maxPduSize);

    pAmpInfo->maxPDUSize =
        (CsrUint16) (pReadLocalAmpInfoReturn->maxPduSize);

    /* At the moment the AMP Manager amp types and HCI amp types are
     * not aligned so we have to convert from one to the other. */
    if (CSR_BT_AMP_AMP_CONTROLLER_TYPE_802_11 ==
        pReadLocalAmpInfoReturn->controllerType)
    {
        pAmpInfo->type = CSR_BT_AMP_AMP_CONTROLLER_TYPE_802_11;
    }
    else if (CSR_BT_AMP_AMP_CONTROLLER_TYPE_ECMA_368 ==
             pReadLocalAmpInfoReturn->controllerType)
    {
        pAmpInfo->type = CSR_BT_AMP_AMP_CONTROLLER_TYPE_ECMA_368;
    }
    else
    {
        /* Assume that this is a Fake AMP */
        pAmpInfo->type = CSR_BT_AMP_AMP_CONTROLLER_TYPE_FAKE;
    }
    pAmpInfo->maxHciAssocSize = pReadLocalAmpInfoReturn->ampAssocLength;
    pAmpInfo->palCapabilities = pReadLocalAmpInfoReturn->palCapabilities;

    /* AMP INFO should only be requested by the local AMP instance
     * following the initial indication that the AMP is available to
     * obtain the AMP type and general capibilities.
     *
     * So we need to inform any existing BT Device instances that the
     * locally available AMPs have now changed.*/
    CsrBtAmpmBtdevLocalAmpChangeInd(param->inst);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLocalFsmReadInfoFailure(void *arg1,
                                             void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;

    csrBtAmpmLocalCancelHciTimer(pLocalAmp);

    /* Working with the 802.11 PAL it seems that we can connect to the
     * PAL and attempt to read the local amp info before it is ready,
     * so as long as the AMP remains registered with AMP Manager and
     * we haven't initialised we'll continue to attempt to read the
     * local amp info from it. */
    if (!pLocalAmp->initialised)
    {
        CsrBtAmpmFsmParam *timerPar;
        timerPar = (CsrBtAmpmFsmParam*)CsrPmemAlloc(sizeof(CsrBtAmpmFsmParam));
        timerPar->inst = param->inst;
        timerPar->argp = pLocalAmp;

        pLocalAmp->responseTid = CsrSchedTimerSet(CSR_BT_AMPM_HCI_READ_INFO_RETRY,
                                                 CsrBtAmpmLocalReadLocalAmpInfoTimeoutInd,
                                                 0,
                                                 timerPar);
    }

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLocalFsmReadAssocReq(void *arg1,
                                          void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;
    CsrUint8 *pPhysicalLinkHandle;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;
    pPhysicalLinkHandle = (CsrUint8 *) arg2;

    csrBtAmpmLocalInitialiseAssoc(pLocalAmp);
    CsrBtAmpmSendHciReadLocalAmpAssoc(param->inst,
                                      pLocalAmp->pAmpInfo->id,
                                      (void *)pLocalAmp,
                                      *pPhysicalLinkHandle,
                                      pLocalAmp->assocLengthSoFar,
                                      pLocalAmp->pAmpInfo->maxHciAssocSize);
    csrBtAmpmLocalStartHciTimer(param->inst, pLocalAmp);
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLocalFsmReadAssocSuccess(void *arg1,
                                              void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;
    CsrBtAmpHciReadLocalAmpAssocReturn *pReadLocalAmpAssocReturn;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;
    pReadLocalAmpAssocReturn = (CsrBtAmpHciReadLocalAmpAssocReturn *) arg2;

    csrBtAmpmLocalCancelHciTimer(pLocalAmp);

    /* Check the length tally */
    if (0 == pLocalAmp->assocExpectedLength)
    {
        /* First fragment will contain the full length of the Assoc */
        pLocalAmp->assocExpectedLength =
            pReadLocalAmpAssocReturn->remainingLength;
    }

    /* The HCI packet size means that larger Assocs need to be chopped up
     * into smaller packets that need to be reassembled for transmission
     * between AMP Managers. The total size of Assoc is CSR_BT_AMPM_MAX_AMP_ASSOC_SIZE.
     */
    (void)CsrMemCpy(pLocalAmp->ampAssoc + pLocalAmp->assocInsertOffset,
                    pReadLocalAmpAssocReturn->assocFragment.data,
                    pReadLocalAmpAssocReturn->assocFragment.dataLen);

    pLocalAmp->assocLengthSoFar += (CsrUint16)(pReadLocalAmpAssocReturn->assocFragment.dataLen);
    pLocalAmp->assocInsertOffset += pReadLocalAmpAssocReturn->assocFragment.dataLen;

    if (pLocalAmp->assocLengthSoFar < pReadLocalAmpAssocReturn->remainingLength)
    {
        /* If we've not read all of it so request the next part */
        CsrBtAmpmSendHciReadLocalAmpAssoc(param->inst,
                                          pLocalAmp->pAmpInfo->id,
                                          (void *) pLocalAmp,
                                          pLocalAmp->physicalLinkHandle,
                                          pLocalAmp->assocLengthSoFar,
                                          pLocalAmp->pAmpInfo->maxHciAssocSize);
        csrBtAmpmLocalStartHciTimer(param->inst, pLocalAmp);
        return FSM_EVENT_NULL;
    }
    else
    {
        return AMPM_LOCAL_EV_HCI_READ_ASSOC_COMPLETE;
    }
}


fsm_event_t CsrBtAmpmLocalFsmReadAssocFailure(void *arg1,
                                              void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;

    csrBtAmpmLocalCancelHciTimer(pLocalAmp);

    /* Is anyone else interested in this information? */
    while (!CsrBtAmpmListIsEmpty(pLocalAmp->pReadAmpAssocQueueAnchor))
    {
        csrBtAmpmLocalGetAmpAssocRequest *pRequest =
            (csrBtAmpmLocalGetAmpAssocRequest *)
            CsrBtAmpmListRemoveHead(pLocalAmp->pReadAmpAssocQueueAnchor);
        CsrBtAmpmAmpAssoc pAMPAssoc;

        pAMPAssoc.length = 0;
        pAMPAssoc.data = NULL;

        /* Is the requester a BtDevice instance or a physical link? */
        if (CSR_BT_AMPM_INVALID_SDU_IDENTIFIER != pRequest->sduIdentifier)
        {
            /* The request was triggered by getting an AMP GET AMP
             * ASSOC REQUEST from a peer Bt Device */
            CsrBtAmpmBtdevLocalAmpAssoc(param->inst,
                                        (CsrBtAmpmBtdev *) pRequest->pRequester,
                                        pRequest->sduIdentifier,
                                        pLocalAmp->pAmpInfo->id,
                                        &pAMPAssoc);
        }
        else
        {
            CsrBtAmpmPhyLocalAmpAssoc(param->inst, pRequest->pRequester,
                                      pLocalAmp->pAmpInfo->id,
                                      &pAMPAssoc);
        }
        CsrPmemFree(pRequest);
    }
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLocalFsmReadAssocCompleted(void *arg1,
                                                void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    /* Will have the same parameters as CsrBtAmpmLocalFsmReadAssocSuccess
     * transition but not interested in the second argument.
     */
    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;

    /* Is anyone else interested in this information? */
    while (!CsrBtAmpmListIsEmpty(pLocalAmp->pReadAmpAssocQueueAnchor))
    {
        csrBtAmpmLocalGetAmpAssocRequest *pRequest =
            (csrBtAmpmLocalGetAmpAssocRequest *)
            CsrBtAmpmListRemoveHead(pLocalAmp->pReadAmpAssocQueueAnchor);
        CsrBtAmpmAmpAssoc pAMPAssoc;

        /* The functions invoked below are not permitted to modify or
         * free any members in pAMPAssoc */
        pAMPAssoc.length = pLocalAmp->assocLengthSoFar;
        pAMPAssoc.data = pLocalAmp->ampAssoc;

        /* Is the requester a BtDevice instance or a physical link? */
        if (CSR_BT_AMPM_INVALID_SDU_IDENTIFIER != pRequest->sduIdentifier)
        {
            /* The request was triggered by getting an AMP GET AMP
             * ASSOC REQUEST from a peer Bt Device */
            CsrBtAmpmBtdevLocalAmpAssoc(param->inst,
                                        (CsrBtAmpmBtdev *) pRequest->pRequester,
                                        pRequest->sduIdentifier,
                                        pLocalAmp->pAmpInfo->id,
                                        &pAMPAssoc);
        }
        else
        {
            CsrBtAmpmPhyLocalAmpAssoc(param->inst, pRequest->pRequester,
                                      pLocalAmp->pAmpInfo->id,
                                      &pAMPAssoc);
        }
        CsrPmemFree(pRequest);
    }
    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtAmpmLocalFsmWriteRemoteAssocReq(void *arg1,
                                                 void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;

    csrBtAmpmLocalWriteRemoteAssocSegment(param->inst,
                                          pLocalAmp);
    csrBtAmpmLocalStartHciTimer(param->inst, pLocalAmp);
    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtAmpmLocalFsmWriteRemoteAssocSuccess(void *arg1,
                                                     void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;

    csrBtAmpmLocalCancelHciTimer(pLocalAmp);

    /* The last segment was successfully written so we need to decide
     * if we've finished or do we need to send more segments?
     */
    if (0 < pLocalAmp->assocLengthRemaining)
    {
        csrBtAmpmLocalWriteRemoteAssocSegment(param->inst,
                                              pLocalAmp);
        csrBtAmpmLocalStartHciTimer(param->inst, pLocalAmp);
        return FSM_EVENT_NULL;
    }
    else
    {
        return AMPM_LOCAL_EV_REMOTE_ASSOC_COMPLETE;
    }
}

fsm_event_t CsrBtAmpmLocalFsmWriteRemoteAssocFailure(void *arg1,
                                                     void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;

    csrBtAmpmLocalCancelHciTimer(pLocalAmp);

    /* Failed writing the full Remote Amp Assoc so is there a
     * physical link instance that needs to know?
     */
    CsrBtAmpmPhyLocalAmpWriteRemoteAssocResponse(param->inst,
                                                 pLocalAmp->writeRemoteAssocPhysicalLinkHandle, FALSE);
    csrBtAmpmLocalTriggerReadAmpAssocIfRequired(param->inst, pLocalAmp);
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmLocalFsmWriteRemoteAssocCompleted(void *arg1,
                                                       void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;

    /* Successfully written the full Remote Amp Assoc so is there a
     * physical link instance that needs to know?
     */
    CsrBtAmpmPhyLocalAmpWriteRemoteAssocResponse(param->inst,
                                                 pLocalAmp->writeRemoteAssocPhysicalLinkHandle, TRUE);
    csrBtAmpmLocalTriggerReadAmpAssocIfRequired(param->inst, pLocalAmp);
    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtAmpmLocalFsmPowerOnInd(void *arg1,
                                       void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;

    CsrBtAmpmUpstreamPowerOnInd(param->inst->ampPowerOnQid,
                                pLocalAmp->pAmpInfo->type,
                                pLocalAmp->pAmpInfo->id,
                                FALSE);

    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtAmpmLocalFsmDeleteReq(void *arg1,
                                       void *arg2)
{
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmLocalAmp *pLocalAmp;
    CsrBool ampHadInitialised = FALSE;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pLocalAmp = (CsrBtAmpmLocalAmp *) param->argp;

    ampHadInitialised = pLocalAmp->initialised;
    CsrBtAmpmLocalDelete(param->inst, pLocalAmp);

    /* Inform any Bt Device instances that the avilable local
     * AMPs have changed if we got as far as obtaining the local
     * AMP info. If we didn't get as far as obtaining the local
     * AMP info then the Bt Device instances didn't know about this
     * AMP anyway.
     */
    if (ampHadInitialised)
    {
        CsrBtAmpmBtdevLocalAmpChangeInd(param->inst);
    }

    return FSM_EVENT_NULL;
}

