/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

/* Structure to handle queuing of AMPM TEST SDU REQUEST whilst the BT
 * Device is not in an appropriate state to respond to the request. */
typedef struct
{
    CsrBtAmpmGenListMember header;
    CsrBtAmpmTestSduReq request;
} csrBtAmpmBtdevTestSduRequest;

static void csrBtAmpmBtdevStartSduTimer(CsrBtAmpmInstance * inst,
                                        CsrBtAmpmBtdev * pBtDevice)
{
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);
    CSR_BT_AMPM_ASSERT(pBtDevice->sduResponseTimer == 0);

    param  = (CsrBtAmpmFsmParam*)CsrPmemAlloc(sizeof(CsrBtAmpmFsmParam));
    param->inst = inst;
    param->argp = pBtDevice;
    pBtDevice->sduResponseTimer = CsrSchedTimerSet(CSR_BT_AMPM_SDU_REQUEST_RESPONSE_TIMEOUT,
                                                  (void (*)(CsrUint16, void *)) CsrBtAmpmBtdevSduResponseTimeoutInd,
                                                  0, param);
}

static CsrBtAmpmBtdev *csrBtAmpmBtdevNew(CsrBtAmpmInstance * inst,
                                         BD_ADDR_T bdAddr)
{
    CsrBtAmpmBtdev *pBtDevice = NULL;

    /* Check we have not already reached the maximum number of BT
     * devices we can support. */
    pBtDevice = CsrPmemAlloc(sizeof(*pBtDevice));

    pBtDevice->bdAddr = bdAddr;
    pBtDevice->cid = 0;
    pBtDevice->state = AMPM_BTDEV_ST_NULL;
    pBtDevice->ampManagerStatus = AMPM_STATUS_UNKNOWN;
    pBtDevice->hasBeenDiscovered = FALSE;
    pBtDevice->haveAllRemoteAmpInfo = FALSE;
    pBtDevice->numOfAMPs = 0;
    pBtDevice->pAMP_ListEntry = NULL;
    pBtDevice->sduResponseTimer = 0;
    pBtDevice->pSendAmpSduReqListAnchor = CsrBtAmpmListCreateAnchor();
    pBtDevice->pRemoteAmpInfoListAnchor = CsrBtAmpmListCreateAnchor();

    CsrBtAmpmListAddHead(inst->pBTDeviceListAnchor,
                         (CsrBtAmpmGenListMember *) pBtDevice);

    return pBtDevice;
}

static void csrBtAmpmBtdevDelete(CsrBtAmpmInstance * inst,
                                 CsrBtAmpmBtdev * pBtDevice)
{
    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);

    CsrBtAmpmListRemoveMember(inst->pBTDeviceListAnchor,
                              (CsrBtAmpmGenListMember *) pBtDevice);

    CsrPmemFree(pBtDevice->pAMP_ListEntry);

    while (!CsrBtAmpmListIsEmpty(pBtDevice->pSendAmpSduReqListAnchor))
    {
        CsrBtAmpmGenListMember *pListMember;
        pListMember = CsrBtAmpmListRemoveHead(pBtDevice->pSendAmpSduReqListAnchor);
        CsrPmemFree(pListMember);
    }
    CsrBtAmpmListDestroyAnchor(&(pBtDevice->pSendAmpSduReqListAnchor));

    while (!CsrBtAmpmListIsEmpty(pBtDevice->pRemoteAmpInfoListAnchor))
    {
        CsrBtAmpmGenListMember *pListMember;
        pListMember = CsrBtAmpmListRemoveHead(pBtDevice->pRemoteAmpInfoListAnchor);
        CsrPmemFree(pListMember);
    }
    CsrBtAmpmListDestroyAnchor(&(pBtDevice->pRemoteAmpInfoListAnchor));

    CsrPmemFree(pBtDevice);
}


static void csrBtAmpmBtdevFsmRun(CsrBtAmpmInstance * inst,
                                 CsrBtAmpmBtdev * pBtDevice,
                                 fsm_event_t event,
                                 void *param2)
{
    fsm_result_t fsm_result;
    CsrBtAmpmFsmParam param;

    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);

    param.inst = inst;
    param.argp = pBtDevice;

    fsm_result = fsm_16bit_run(&csr_bt_ampm_btdev_fsm, &(pBtDevice->state), event, &param, /* arg1 */
                               param2);                /* arg2 */

    CSR_BT_AMPM_ASSERT(FSM_RESULT_OK == fsm_result);

    /* Depending on the configuration, CSR_BT_AMPM_ASSERT may be invalid and thus fsm_result is unsed */
    CSR_UNUSED(fsm_result);
}


static CsrBool csrBtAmpmBtdevMatchesBtAddress(const void *pCriteria,
                                              const void *pListEntry)
{
    BD_ADDR_T bdAddr;
    const CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    bdAddr = *((const BD_ADDR_T *) pCriteria);
    pBtDevice = (const CsrBtAmpmBtdev *) pListEntry;

    return (bd_addr_eq((const BD_ADDR_T *) &(pBtDevice->bdAddr),
                       (const BD_ADDR_T *) &bdAddr));
}



static CsrBool csrBtAmpmBtdevMatchesCID(const void *pCriteria,
                                        const void *pListEntry)
{
    CsrBtConnId cid;
    const CsrBtAmpmBtdev *pBtDevice;
    CsrBool doesEntryMatch = FALSE;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    cid = *((const CsrBtConnId *) pCriteria);
    pBtDevice = (const CsrBtAmpmBtdev *) pListEntry;

    if (pBtDevice->cid == cid)
    {
        doesEntryMatch = TRUE;
    }
    return (doesEntryMatch);
}

static CsrBool csrBtAmpmBtdevRemoteAmpInfoMatchesId(const void *pCriteria,
                                                    const void *pListEntry)
{
    CsrBtAmpmId ampId;
    const CsrBtAmpmBtdevRemoteAmpInfo *pRemoteAMPInfo;
    CsrBool doesEntryMatch = FALSE;

    CSR_BT_AMPM_ASSERT(pCriteria != NULL);
    CSR_BT_AMPM_ASSERT(pListEntry != NULL);

    ampId = *((const CsrBtAmpmId *) pCriteria);
    pRemoteAMPInfo = (const CsrBtAmpmBtdevRemoteAmpInfo *) pListEntry;

    if (ampId == pRemoteAMPInfo->info.id)
    {
        doesEntryMatch = TRUE;
    }
    return (doesEntryMatch);
}

static void csrBtAmpmBtdevCancelTimer(CsrSchedTid * pTid)
{
    CsrBtAmpmFsmParam *param;
    CsrUint16 mi;

    CSR_BT_AMPM_ASSERT(pTid != NULL);

    if (0 != *pTid)
    {
        (void) CsrSchedTimerCancel(*pTid, &mi, (void **) &param);
        CsrPmemFree(param);
        *pTid = 0;
    }
}

CsrBtAmpController CsrBtAmpmBtdevFindMatchingRemoteController(CsrBtAmpmInstance *inst,
                                                             BD_ADDR_T *addr,
                                                             CsrBtAmpController localId)
{
    CsrBtAmpmLocalAmp *local;
    CsrBtAmpmBtdev *btdev;

    btdev = CsrBtAmpmBtdevGetByBdaddr(inst, *addr);
    local = CsrBtAmpmLocalGetById(inst, (CsrBtAmpmId)localId);

    if(local && btdev)
    {
        CsrBtAmpmDataRemoteAmp *remote;
        CsrUint8 i;

        /* Scan remote AMP info, looking for the entry where the
         * physical type matches our local physical type */
        remote = btdev->pAMP_ListEntry;
        for(i=0;
            i < btdev->numOfAMPs;
            i++, remote++)
        {
            if(remote->type == local->pAmpInfo->type)
            {
                return remote->id;
            }
        }
    }

    /* Could not find matching controller ID */
    return CSR_BT_AMP_CONTROLLER_UNKNOWN;
}


static CsrBool csrBtAmpmBtdevRequestOutstandingRemoteAmpInfo(CsrBtAmpmInstance * inst,
                                                             CsrBtAmpmBtdev * pBtDevice)
{
    CsrBtAmpmDataRemoteAmp *pAmpListEntry;
    CsrUint8 currentIndex = 0;
    CsrBool sentReq = FALSE;

    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);

    /* See if we've got any remote amps that we have not received the
     * info for yet and if so then request info for the first one that
     * we find. */
    pAmpListEntry = (CsrBtAmpmDataRemoteAmp *) pBtDevice->pAMP_ListEntry;
    while (pBtDevice->numOfAMPs > currentIndex &&
           !sentReq)
    {
        if (CSR_BT_AMP_AMP_CONTROLLER_TYPE_BR_EDR != pAmpListEntry->type)
        {
            /* Have we already got the info for this AMP or not? */
            CsrBtAmpmBtdevRemoteAmpInfo *pRemoteAmpInfo =
                CsrBtAmpmBtdevRemoteAmpInfoGetById(pBtDevice,
                                                   pAmpListEntry->id);

            if (NULL == pRemoteAmpInfo)
            {
                CsrMblk *pMsgBlk;

                pMsgBlk = CsrBtAmpmSduCreateAmpGetInfoRequest(
                    CsrBtAmpmSduAllocIdentifier(inst),
                    pAmpListEntry->id);

                L2CA_DataWriteReq((l2ca_cid_t)pBtDevice->cid, 0, pMsgBlk);
                sentReq = TRUE;
                csrBtAmpmBtdevCancelTimer(&(pBtDevice->sduResponseTimer));
                csrBtAmpmBtdevStartSduTimer(inst, pBtDevice);
            }
        }
        pAmpListEntry++;
        currentIndex++;
    }
    return (sentReq);
}

static CsrBool csrBtAmpmBtdevAmpInAmpList(const CsrBtAmpmDataRemoteAmp * pAmpEntry,
                                          const CsrUint16 numOfAmps,
                                          const CsrUint8 * pAmpList)
{
    CsrBool ampInList = FALSE;

    CSR_BT_AMPM_ASSERT(pAmpEntry != NULL);
    CSR_BT_AMPM_ASSERT(pAmpList != NULL);

    if (0 < numOfAmps)
    {
        CsrBtAmpmDataRemoteAmp *pAmpListEntry =
            (CsrBtAmpmDataRemoteAmp *) pAmpList;
        CsrUint16 currentIndex = 1;

        while (!ampInList && numOfAmps >= currentIndex)
        {
            if (pAmpEntry->id == pAmpListEntry->id &&
                pAmpEntry->type == pAmpListEntry->type)
            {
                ampInList = TRUE;
            }
            pAmpListEntry++;
            currentIndex++;
        }
    }
    return (ampInList);
}

static void csrBtAmpmBtdevChangesInRemoteAmpList(CsrBtAmpmInstance * inst,
                                                 CsrBtAmpmBtdev * pBtDevice,
                                                 const CsrUint16 lengthOfAmpList,
                                                 const CsrUint8 * pAmpList)
{
    const CsrUint8 numOfNewAMPs =
        (CsrUint8) (lengthOfAmpList / sizeof(CsrBtAmpmDataRemoteAmp));

    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);
    CSR_BT_AMPM_ASSERT(pAmpList != NULL);

    /* Has the Bt Device instance actually got any remote AMPs currrntly (it might not have)? */
    if (0 < pBtDevice->numOfAMPs)
    {
        CsrBtAmpmDataRemoteAmp *pCurrentAmpEntry = pBtDevice->pAMP_ListEntry;
        CsrUint8 currentAmpIndex = 1;

        /* Need to determine which AMPs have been removed and which are new
         * and take any appropriate actions based on the changes.
         */
        while (pBtDevice->numOfAMPs >= currentAmpIndex)
        {
            if (!csrBtAmpmBtdevAmpInAmpList((const CsrBtAmpmDataRemoteAmp *)
                                            pCurrentAmpEntry, numOfNewAMPs, pAmpList))
            {
                CsrBtAmpmPhyLink *pPhyLink =
                    CsrBtAmpmPhyGetByRemoteAmpId(inst,
                                                 pCurrentAmpEntry->id);
                CsrBtAmpmBtdevRemoteAmpInfo *pRemoteAmpInfo =
                    CsrBtAmpmBtdevRemoteAmpInfoGetById(pBtDevice,
                                                       pCurrentAmpEntry->id);

                /* Is there a physical link using this remote amp? */
                if (NULL != pPhyLink)
                {
                    /* We're assuming that there is only ever a single physical link to a particular
                     * device, so as there is a physical link using a removed remote amp we need to
                     * disconnect it.
                     */
                    CsrBtAmpmPhyLocalDisconnectOnly(inst, pBtDevice->bdAddr);
                }
                /* Also need to remove the remote amp info */
                CsrBtAmpmListRemoveMember(pBtDevice->pRemoteAmpInfoListAnchor,
                                          (CsrBtAmpmGenListMember *) pRemoteAmpInfo);
                CsrPmemFree((void *) pRemoteAmpInfo);
            }
            pCurrentAmpEntry++;
            currentAmpIndex++;
        }

        /* Free any previous list */
        CsrPmemFree((void *) pBtDevice->pAMP_ListEntry);
        pBtDevice->pAMP_ListEntry = NULL;
        pBtDevice->numOfAMPs = 0;
    }

    if (0 < numOfNewAMPs)
    {
        pBtDevice->numOfAMPs = numOfNewAMPs;
        pBtDevice->pAMP_ListEntry =
            (CsrBtAmpmDataRemoteAmp *)
            CsrPmemAlloc((CsrUint32) (lengthOfAmpList));
        (void) CsrMemCpy((void *) (pBtDevice->pAMP_ListEntry),
                         (const void *) (pAmpList), (CsrUint32) (lengthOfAmpList));

        /* Request the info of any new AMPs */
        (void) csrBtAmpmBtdevRequestOutstandingRemoteAmpInfo(inst, pBtDevice);
    }
}

static void csrBtAmpmBtdevUpdateRemoteAmpList(CsrBtAmpmInstance * inst,
                                              CsrBtAmpmBtdev * pBtDevice,
                                              const CsrUint16 lengthOfAmpList,
                                              const CsrUint8 * pAmpList)
{
    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);
    CSR_BT_AMPM_ASSERT(pAmpList != NULL);

    /* Has the device actually got any AMPs (it might not have)? */
    if (0 < pBtDevice->numOfAMPs)
    {
        csrBtAmpmBtdevChangesInRemoteAmpList(inst, pBtDevice, lengthOfAmpList,
                                             pAmpList);

        pBtDevice->numOfAMPs = 0;
        CsrPmemFree((void *) pBtDevice->pAMP_ListEntry);
        pBtDevice->pAMP_ListEntry = NULL;
    }

    if (0 < lengthOfAmpList)
    {
        pBtDevice->numOfAMPs =
            (CsrUint8) (lengthOfAmpList / sizeof(CsrBtAmpmDataRemoteAmp));
        pBtDevice->pAMP_ListEntry =
            (CsrBtAmpmDataRemoteAmp *)CsrPmemAlloc((CsrUint32) (lengthOfAmpList));
        (void) CsrMemCpy((void *) (pBtDevice->pAMP_ListEntry),
                         (const void *) (pAmpList), (CsrUint32) (lengthOfAmpList));
    }
}

static CsrBool csrBtAmpmBtdevAmpIdInAmpList(CsrBtAmpmId ampId,
                                            CsrUint8 numOfAmps,
                                            CsrUint8 * pAmpList)
{
    CsrBool ampInList = FALSE;

    CSR_BT_AMPM_ASSERT(pAmpList != NULL);

    if (0 < numOfAmps)
    {
        CsrBtAmpmDataRemoteAmp *pAmpListEntry =
            (CsrBtAmpmDataRemoteAmp *) pAmpList;
        CsrUint16 currentIndex = 1;

        while (!ampInList && numOfAmps >= currentIndex)
        {
            if (ampId == pAmpListEntry->id)
            {
                ampInList = TRUE;
            }
            pAmpListEntry++;
            currentIndex++;
        }
    }
    return (ampInList);
}

static void csrBtAmpmBtdevReportDiscoveryComplete(CsrBtAmpmInstance * inst,
                                                  CsrBtAmpmBtdev * pBtDevice)
{
    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);
    CsrBtAmpmPhyAmpDiscoverySuccess(inst, pBtDevice);
    CsrBtAmpmAppCheckNotification(inst);
}

static void csrBtAmpmBtdevReportDiscoveryFailure(CsrBtAmpmInstance * inst,
                                                 CsrBtAmpmBtdev * pBtDevice)
{
    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);
    CsrBtAmpmPhyAmpDiscoveryFailure(inst, pBtDevice->bdAddr);
}

static void csrBtAmpmBtdevReportAmpmChannelDisconnected(CsrBtAmpmInstance * inst,
                                                        CsrBtAmpmBtdev * pBtDevice)
{
    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);
    CsrBtAmpmPhyLocalDisconnectOnly(inst, pBtDevice->bdAddr);
}


static void csrBtAmpmBtdevSendAmpDiscoverRequest(CsrBtAmpmInstance * inst,
                                                 CsrBtAmpmBtdev * pBtDevice)
{
    CsrMblk *pMsgBlk;

    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);

    pBtDevice->sduIdentifier = CsrBtAmpmSduAllocIdentifier(inst);
    pMsgBlk = CsrBtAmpmSduCreateAmpDiscoverRequest(pBtDevice->sduIdentifier,
                                                   CsrBtAmpmLocalMaxMtuSupportedbyAmps(inst));

    L2CA_DataWriteReq((l2ca_cid_t)pBtDevice->cid, 0, pMsgBlk);
    csrBtAmpmBtdevStartSduTimer(inst, pBtDevice);
}

static void csrBtAmpmBtdevRespondToAmpDiscoverRequest(CsrBtAmpmInstance * inst,
                                                      CsrBtAmpmBtdev * pBtDevice,
                                                      CsrUint8 sduIdentifier)
{
    CsrMblk *pMsgBlk;

    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);

    /* identifier from incoming request */
    pMsgBlk = CsrBtAmpmSduCreateAmpDiscoverResponse(inst,
                                                    sduIdentifier,
                                                    CsrBtAmpmLocalMaxMtuSupportedbyAmps(inst));
    L2CA_DataWriteReq((l2ca_cid_t)pBtDevice->cid, 0, pMsgBlk);
}

static void csrBtAmpmBtdevQueueTestSduReq(CsrBtAmpmBtdev *pBtDevice,
                                          CsrBtAmpmTestSduReq *pRequest)
{
    csrBtAmpmBtdevTestSduRequest *pQueueEntry;

    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);
    CSR_BT_AMPM_ASSERT(pRequest != NULL);

    pQueueEntry = (csrBtAmpmBtdevTestSduRequest *) CsrPmemAlloc(sizeof(*pQueueEntry));
    (void)CsrMemCpy((void *) &(pQueueEntry->request),
                    pRequest,
                    sizeof(*pRequest));
    CsrBtAmpmListAddTail(pBtDevice->pSendAmpSduReqListAnchor,
                         (CsrBtAmpmGenListMember *)pQueueEntry);

    /* As we copied the members (and pointers) when filling out the
     * queue structure, we must clear the original primitive */
    pRequest->length = 0;
    pRequest->data = NULL;
}

static void csrBtAmpmBtdevSendQeueuedTestSdus(CsrBtAmpmBtdev * pBtDevice)
{
    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);

    /* Do we have any pending test SDUs to send */
    while (!CsrBtAmpmListIsEmpty(pBtDevice->pSendAmpSduReqListAnchor))
    {
        /* Now have the information to handle these requests */
        csrBtAmpmBtdevTestSduRequest *pQueueEntry;
        CsrMblk *pMsgBlk;
        CsrUint8 *pSDU;
        CsrBtAmpmGenListMember *pListMember;
        pListMember = CsrBtAmpmListRemoveHead(pBtDevice->pSendAmpSduReqListAnchor);

        pQueueEntry = (csrBtAmpmBtdevTestSduRequest *) pListMember;
        pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU),
                                     pQueueEntry->request.length);

        CsrMemCpy(pSDU, pQueueEntry->request.data,
                  pQueueEntry->request.length);
        L2CA_DataWriteReq((l2ca_cid_t)pBtDevice->cid, 0, pMsgBlk);
        CsrPmemFree((void *) pListMember);
    }
}


/* Trigger BT device discovery */
void CsrBtAmpmBtdevKickDiscovery(CsrBtAmpmInstance *inst,
                                 BD_ADDR_T addr)
{
    /* If the AMP Manager functionlity is not supported then ignore
     * this request */
    CsrBtAmpmBtdev *pBtDevice;

    /* Create new device if it doesn't exist already */
    pBtDevice = CsrBtAmpmBtdevGetByBdaddr(inst, addr);
    if (NULL == pBtDevice)
    {
        /* Need to create the BT device instance and trigger the AMP
         * discovery process. */
        pBtDevice = csrBtAmpmBtdevNew(inst, addr);
    }

    /* Initiate the discovery procedure if at least one application
     * exists, and state allows it */
    if(CsrBtAmpmAppIsEnabled(inst) &&
       !CsrBtAmpmBtdevIsDiscovered(pBtDevice))
    {
        /* This will start the L2CAP extended features check,
         * which automatically will trigger ACL creation */
        csrBtAmpmBtdevFsmRun(inst,
                             pBtDevice,
                             AMPM_BTDEV_EV_DISCOVER_EV,
                             NULL);
    }
}

CsrBtAmpmBtdev *CsrBtAmpmBtdevGetByBdaddr(CsrBtAmpmInstance * inst,
                                          BD_ADDR_T bdAddr)
{
    CsrBtAmpmBtdev *pBtDevice;

    /* Find the required element based on the connection id */
    pBtDevice = (CsrBtAmpmBtdev *)CsrBtAmpmListFindMatchingMember(
        inst->pBTDeviceListAnchor,
        csrBtAmpmBtdevMatchesBtAddress,
        (const void *) &bdAddr);

    return (pBtDevice);
}


CsrBtAmpmBtdev *CsrBtAmpmBtdevGetByCid(CsrBtAmpmInstance * inst,
                                       CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;

    /* Find the required element based on the connection id */
    pBtDevice = (CsrBtAmpmBtdev *)CsrBtAmpmListFindMatchingMember(
        inst->pBTDeviceListAnchor,
        csrBtAmpmBtdevMatchesCID,
        (const void *) &cid);

    return (pBtDevice);
}


CsrBtAmpmBtdevRemoteAmpInfo *CsrBtAmpmBtdevRemoteAmpInfoGetById(CsrBtAmpmBtdev * pBtDevice,
                                                                CsrBtAmpmId Id)
{
    CsrBtAmpmBtdevRemoteAmpInfo *pRemoteAmpInfo;

    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);

    /* Find the required element based on the connection id */
    pRemoteAmpInfo =
        (CsrBtAmpmBtdevRemoteAmpInfo *)
        CsrBtAmpmListFindMatchingMember(pBtDevice->pRemoteAmpInfoListAnchor,
                                        csrBtAmpmBtdevRemoteAmpInfoMatchesId, (const void *) &Id);
    return (pRemoteAmpInfo);
}


CsrUint8 CsrBtAmpmBtdevRemoteAmpStatusGetById(CsrBtAmpmBtdev * pBtDevice,
                                              CsrBtAmpmId Id)
{
    CsrBtAmpmDataRemoteAmp *pAmpListEntry;
    CsrUint8 currentIndex = 0;

    /* Default to not usable */
    CsrUint8 RemoteAmpStatus = CSR_BT_AMP_AMP_STATUS_NO_CAPACITY_AVAILABLE_FOR_BLUETOOTH_OPERATION;

    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);

    pAmpListEntry = (CsrBtAmpmDataRemoteAmp *) pBtDevice->pAMP_ListEntry;
    while (pBtDevice->numOfAMPs > currentIndex &&
           pAmpListEntry->id != Id)
    {
        pAmpListEntry++;
        currentIndex++;
    }
    if (pAmpListEntry->id == Id)
    {
        RemoteAmpStatus = pAmpListEntry->status;
    }
    return (RemoteAmpStatus);
}


void CsrBtAmpmBtdevInit(CsrBtAmpmInstance * inst)
{
    CSR_BT_AMPM_ASSERT(inst->pBTDeviceListAnchor == NULL);
    inst->pBTDeviceListAnchor = CsrBtAmpmListCreateAnchor();
}

void CsrBtAmpmBtdevDeinit(CsrBtAmpmInstance * inst)
{
    if(inst->pBTDeviceListAnchor != NULL)
    {
        CSR_BT_AMPM_ASSERT(inst->pBTDeviceListAnchor != NULL);
        while (!CsrBtAmpmListIsEmpty(inst->pBTDeviceListAnchor))
        {
            /* This function will automatically remove the item from
             * the list, so don't do it here */
            csrBtAmpmBtdevDelete(inst,
                                 (CsrBtAmpmBtdev*)inst->pBTDeviceListAnchor->head);
        }
        CsrBtAmpmListDestroyAnchor(&(inst->pBTDeviceListAnchor));
    }
}

/* Done reading local AMP assoc on behalf of the peer.  Note: The
 * pAMPAssoc pointer and its contents must not be modified nor
 * freed */
void CsrBtAmpmBtdevLocalAmpAssoc(CsrBtAmpmInstance * inst,
                                 CsrBtAmpmBtdev * pPossibleBtDevice,
                                 CsrUint8 sduIdentifier,
                                 CsrBtAmpmId ampId,
                                 CsrBtAmpmAmpAssoc *pAMPAssoc)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pPossibleBtDevice != NULL);
    CSR_BT_AMPM_ASSERT(pAMPAssoc != NULL);

    /* Is this a Bt Device instance or not? */
    pBtDevice = (CsrBtAmpmBtdev *)CsrBtAmpmListFindMatchingMember(inst->pBTDeviceListAnchor,
                                                                  CsrBtAmpmListEntryMatchesPtr,
                                                                  (const void *) pPossibleBtDevice);

    if (NULL != pBtDevice)
    {
        CsrUint8 status = CSR_BT_AMPM_GET_ASSOC_RSP_STATUS_SUCCESS;
        CsrMblk *pMsgBlk;

        /* Assuming that a length of 0 indicates an error reading the
         * amp assoc */
        if (0 == pAMPAssoc->length)
        {
            /* We don't have a code for reporting failure to read the
             * Assoc from the AMP so use the only failure code that we
             * have. */
            status = CSR_BT_AMPM_GET_ASSOC_RSP_STATUS_INVALID_AMP_ID;
        }
        pMsgBlk = CsrBtAmpmSduCreateAmpGetAmpAssocResponse(sduIdentifier,
                                                           status,
                                                           ampId, pAMPAssoc);
        L2CA_DataWriteReq((l2ca_cid_t)pBtDevice->cid, 0, pMsgBlk);
    }
}

void CsrBtAmpmBtdevLocalAmpChangeInd(CsrBtAmpmInstance * inst)
{
    CsrBtAmpmBtdev *pBtDevice;

    /* Iterate over any existing Bt Device instances and inform them
     * that there has been a change in the locally available AMPs */
    pBtDevice = (CsrBtAmpmBtdev *) inst->pBTDeviceListAnchor->head;

    while (NULL != pBtDevice)
    {
        csrBtAmpmBtdevFsmRun(inst, pBtDevice,
                             AMPM_BTDEV_EV_LOCAL_AMP_CHANGE_IND, NULL);
        pBtDevice =
            (CsrBtAmpmBtdev *) (((CsrBtAmpmGenListMember *) pBtDevice)->next);
    }

    /* Inform application about change in AMPs */
    CsrBtAmpmAppCheckNotification(inst);
}

CsrBtAmpmDataRemoteAmp *CsrBtAmpmBtdevGetRemoteAmpInfoById(CsrBtAmpmBtdev * pBtDevice,
                                                           CsrBtAmpmId ampId)
{
    CsrBtAmpmDataRemoteAmp *pRemoteAMPInfo = NULL;
    CsrBtAmpmDataRemoteAmp *pTemp;
    int count;

    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);

    pTemp = pBtDevice->pAMP_ListEntry;

    for (count = 0; count < pBtDevice->numOfAMPs; count++, pTemp++)
    {
        if (pTemp->id == ampId)
        {
            pRemoteAMPInfo = pTemp;
            break;
        }
    }
    return (pRemoteAMPInfo);
}

CsrBtAmpmDataRemoteAmp *CsrBtAmpmBtdevGetRemoteAmpInfoByType(CsrBtAmpmBtdev * pBtDevice,
                                                             CsrBtAmpmType ampType)
{
    CsrBtAmpmDataRemoteAmp *pRemoteAMPInfo = NULL;
    CsrBtAmpmDataRemoteAmp *pTemp;
    int count;

    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);

    pTemp = pBtDevice->pAMP_ListEntry;

    for (count = 0; count < pBtDevice->numOfAMPs; count++, pTemp++)
    {
        if (pTemp->type == ampType)
        {
            pRemoteAMPInfo = pTemp;
            break;
        }
    }
    return (pRemoteAMPInfo);
}

void CsrBtAmpmBtdevTestSduReq(CsrBtAmpmInstance * inst,
                              CsrBtAmpmTestSduReq * pAmpmTestSduReq)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pAmpmTestSduReq != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByBdaddr(inst, pAmpmTestSduReq->bdAddr);

    if (NULL != pBtDevice &&
        AMPM_STATUS_PRESENT == CsrBtAmpmBtdevAmpManagerStatus(pBtDevice))
    {
        CsrMblk *pMsgBlk;
        CsrUint8 *pSDU;

        pMsgBlk =
            CsrMblkMallocCreate((void *) &(pSDU), pAmpmTestSduReq->length);
        (void) CsrMemCpy(pSDU, pAmpmTestSduReq->data,
                         pAmpmTestSduReq->length);
        L2CA_DataWriteReq((l2ca_cid_t)pBtDevice->cid, 0, pMsgBlk);
    }
    else
    {
        if (NULL == pBtDevice)
        {
            CsrBtAmpmBtdevKickDiscovery(inst,
                                        pAmpmTestSduReq->bdAddr);
        }
        else
        {
            csrBtAmpmBtdevQueueTestSduReq(pBtDevice, pAmpmTestSduReq);
        }
    }
}

void CsrBtAmpmBtdevDisconnectReq(CsrBtAmpmInstance * inst,
                                 CsrBtAmpmBtdev * pBtDevice,
                                 CsrBtAmpmResult result)
{
    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);

    /* Notify apps about failure */
    CsrBtAmpmAppFailNotification(inst, &pBtDevice->bdAddr, result);

    csrBtAmpmBtdevFsmRun(inst, pBtDevice, AMPM_BTDEV_EV_DISCONNECT_REQ, NULL);
}


void CsrBtAmpmBtdevDisconnectInd(CsrBtAmpmInstance * inst,
                                 BD_ADDR_T bdAddr,
                                 CsrBtAmpmResult result)
{
    CsrBtAmpmBtdev *pBtDevice;

    /* Notify apps about failure */
    CsrBtAmpmAppFailNotification(inst, &bdAddr, result);

    pBtDevice = CsrBtAmpmBtdevGetByBdaddr(inst, bdAddr);
    if (NULL != pBtDevice)
    {
        csrBtAmpmBtdevFsmRun(inst, pBtDevice,
                             AMPM_BTDEV_EV_DISCONNECT_IND, NULL);
    }
}

void CsrBtAmpmBtdevSduResponseTimeoutInd(CsrUint16 ignored,
                                         const void *pFsmInstance)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(pFsmInstance != NULL);
    param = (CsrBtAmpmFsmParam *) pFsmInstance;

    /* Have to check that the instance data is still valid */
    pBtDevice = (CsrBtAmpmBtdev *)CsrBtAmpmListFindMatchingMember(
        param->inst->pBTDeviceListAnchor,
        CsrBtAmpmListEntryMatchesPtr,
        (const void *) pFsmInstance);

    if (NULL != pBtDevice)
    {
        pBtDevice->sduResponseTimer = 0;

        /* If timers are disabled then ignore it */
        if (((CsrBtAmpmInstance*)(param->inst))->ampmTimersEnabled)
        {
            /* If we fail to receive an expected response to an AMP
             * Manager request sdu then we need to disconnect the ACL
             * link to the peer device. */
            CsrBtAmpmBtdevDisconnectReq(param->inst, pBtDevice,
                                        CSR_BT_AMPM_RESULT_PEER_PROTOCOL_ERROR);
        }
    }
    CsrPmemFree(param);
}

CsrBool CsrBtAmpmBtdevIsDiscovered(CsrBtAmpmBtdev * pBtDevice)
{
    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);
    return (pBtDevice->hasBeenDiscovered &&
            pBtDevice->haveAllRemoteAmpInfo);
}

CsrBool CsrBtAmpmBtdevIsAmpable(CsrBtAmpmBtdev *pBtDevice)
{
    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);
    return (pBtDevice->hasBeenDiscovered &&
            pBtDevice->haveAllRemoteAmpInfo &&
            (pBtDevice->numOfAMPs > 1));
}

CsrBtAmpmBtdevAmpmStatus CsrBtAmpmBtdevAmpManagerStatus(CsrBtAmpmBtdev * pBtDevice)
{
    CSR_BT_AMPM_ASSERT(pBtDevice != NULL);
    return (pBtDevice->ampManagerStatus);
}

void CsrBtAmpmL2capGetInfoCfm(CsrBtAmpmInstance * inst,
                              L2CA_GETINFO_CFM_T * prim)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(prim != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByBdaddr(inst, prim->bd_addr);
    if (NULL != pBtDevice)
    {
        fsm_event_t event;

        /* What info type is this response for? */
        if ((L2CA_GETINFO_SUCCESS == prim->result) &&
            (L2CA_GETINFO_TYPE_EXT_FEAT == pBtDevice->getInfoTypeRequested) &&
            (L2CA_GETINFO_SIZE_EXT_FEAT == prim->length) &&
            (L2CA_EXT_FEAT_FIXED_CH == (prim->info_data[0] & L2CA_EXT_FEAT_FIXED_CH)) &&
            (L2CA_EXT_FEAT_ENH_RETRANS == (prim->info_data[0] & L2CA_EXT_FEAT_ENH_RETRANS)))
        {
            event = AMPM_BTDEV_EV_FIXED_CHANS_SUPPORTED_IND;
        }
        else if ((L2CA_GETINFO_SUCCESS == prim->result) &&
                 (L2CA_GETINFO_TYPE_FIX_CH == pBtDevice->getInfoTypeRequested) &&
                 (L2CA_GETINFO_SIZE_FIX_CH == prim->length) &&
                 (L2CA_FIXED_CH_AMP_MANAGER == (prim->info_data[0] & L2CA_FIXED_CH_AMP_MANAGER)))
        {
            event = AMPM_BTDEV_EV_AMPM_CHAN_SUPPORTED_IND;
        }
        else
        {
            CsrBtAmpmResult result;
            result = (CsrBtAmpmResult)(((prim->result == L2CA_GETINFO_FAILED) ||
                                        (prim->result == L2CA_GETINFO_TIMEOUT))
                                       ? CSR_BT_AMPM_RESULT_NO_CONNECTION
                                       : CSR_BT_AMPM_RESULT_NO_REMOTE_AMP_MANAGER);

            /* Not supported or error. Poke FSM and bail out */
            CsrBtAmpmBtdevDisconnectInd(inst, pBtDevice->bdAddr,
                                        result);
            return;
        }

        /* Call the state machine */
        csrBtAmpmBtdevFsmRun(inst, pBtDevice, event, NULL);
    }
}

void CsrBtAmpmL2capMapFixedCidCfm(CsrBtAmpmInstance * inst,
                                  L2CA_MAP_FIXED_CID_CFM_T * prim)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(prim != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByBdaddr(inst, prim->addrt.addr);

    if (NULL != pBtDevice)
    {
        if(prim->result == L2CA_MISC_SUCCESS)
        {
            csrBtAmpmBtdevFsmRun(inst, pBtDevice,
                                 AMPM_BTDEV_EV_L2CA_MAP_FIXED_CID_CFM,
                                 (void*)prim);
        }
        else if(prim->result == L2CA_MISC_INITIATING)
        {
            /* Ignore initiating */
        }
        else
        {
            /* L2CAP has failed to allocate the dynamic CID so
             * terminate the btdev instance. Most likely cause is that
             * the ACL has disconnected since AMPM making the request
             * for the cid */
            CsrBtAmpmAppFailNotification(inst, &pBtDevice->bdAddr,
                                         CSR_BT_AMPM_RESULT_NO_CONNECTION);

            csrBtAmpmBtdevFsmRun(inst, pBtDevice,
                                 AMPM_BTDEV_EV_DELETE_REQ,
                                 NULL);
        }
    }
}

void CsrBtAmpmL2capMapFixedCidInd(CsrBtAmpmInstance * inst,
                                  L2CA_MAP_FIXED_CID_IND_T * prim)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(prim != NULL);

   pBtDevice = CsrBtAmpmBtdevGetByBdaddr(inst, prim->addrt.addr);

    /* This may have beaten the DM ACL OPEN IND so if we haven't got a
     * BT Device instance then create one. */
    if (NULL == pBtDevice)
    {
        pBtDevice = csrBtAmpmBtdevNew(inst, prim->addrt.addr);
    }

    if (NULL != pBtDevice)
    {
        csrBtAmpmBtdevFsmRun(inst, pBtDevice,
                             AMPM_BTDEV_EV_L2CA_MAP_FIXED_CID_IND,
                             (void*)prim);
    }
}

void CsrBtAmpmBtdevDmAclConnHandleInd(CsrBtAmpmInstance * inst,
                                  const DM_ACL_CONN_HANDLE_IND_T *prim)
{
    CSR_BT_AMPM_ASSERT(prim != NULL);

    if (prim->status == HCI_SUCCESS)
    {/* Try to trigger discovery only if the connection succeeded */
        CsrBtAmpmBtdevKickDiscovery(inst, prim->addrt.addr);

        /* We may need to read the stored link keys */
        CsrBtAmpmDataScTrigger(inst, (BD_ADDR_T*)&prim->addrt.addr);
    }
}

void CsrBtAmpmBtdevDmAclClosedInd(CsrBtAmpmInstance * inst,
                                  const DM_ACL_CLOSED_IND_T * prim)
{
    CSR_BT_AMPM_ASSERT(prim != NULL);

    CsrBtAmpmBtdevDisconnectInd(inst, prim->addrt.addr,
                                CSR_BT_AMPM_RESULT_NO_CONNECTION);
}

void CsrBtAmpmBtdevRemoteFeaturesInd(CsrBtAmpmInstance *inst,
                                     DM_AMPM_REMOTE_FEATURES_IND_T *prim)
{
    CSR_BT_AMPM_ASSERT(prim != NULL);

    if((prim->flags & DM_AMPM_REMOTE_FEATURES_SSP) == DM_AMPM_REMOTE_FEATURES_SSP)
    {
        /* Try to trigger discovery */
        CsrBtAmpmBtdevKickDiscovery(inst, prim->bd_addr);
    }
    else
    {
        /* Stop AMPM interrogation if device doesn't support SSP */
        CsrBtAmpmBtdevDisconnectInd(inst, prim->bd_addr,
                                    CSR_BT_AMPM_RESULT_NO_REMOTE_AMP_MANAGER);
    }
}

CsrBool CsrBtAmpmBtdevAmpCommandReject(CsrBtAmpmInstance *inst,
                                       const CsrUint8 *pMsg,
                                       CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBool SduHandled = FALSE;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);

    /* If we find the match BT device associated with the correct CID
     * and it was responsible for sending the SDU then pass the reject
     * to it for handling.
     */
    if (NULL != pBtDevice)
    {
        if (*(++pMsg) == pBtDevice->sduIdentifier)
        {
            csrBtAmpmBtdevFsmRun(inst, pBtDevice,
                                 AMPM_BTDEV_EV_AMP_COMMAND_REJECT,
                                 NULL);
            SduHandled = TRUE;
        }
    }
    return (SduHandled);
}


void CsrBtAmpmBtdevAmpDiscoverRequest(CsrBtAmpmInstance * inst,
                                      const CsrUint8 * pMsg,
                                      CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    /* This is about the discover request SDU, not the application
     * discovery action */
    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);
    if (NULL != pBtDevice)
    {
        csrBtAmpmBtdevFsmRun(inst, pBtDevice,
                             AMPM_BTDEV_EV_AMP_DISCOVER_REQUEST,
                             (void*)pMsg);
    }
}


void CsrBtAmpmBtdevAmpDiscoverResponse(CsrBtAmpmInstance * inst,
                                       const CsrUint8 * pMsg,
                                       CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);

    if (NULL != pBtDevice)
    {
        csrBtAmpmBtdevFsmRun(inst, pBtDevice,
                             AMPM_BTDEV_EV_AMP_DISCOVER_RESPONSE,
                             (void*)pMsg);
    }
}


void CsrBtAmpmBtdevAmpChangeNotify(CsrBtAmpmInstance * inst,
                                   const CsrUint8 * pMsg,
                                   CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);

    if (NULL != pBtDevice)
    {
        csrBtAmpmBtdevFsmRun(inst, pBtDevice, AMPM_BTDEV_EV_AMP_CHANGE_NOTIFY,
                             (void*)pMsg);
    }
}


void CsrBtAmpmBtdevAmpChangeResponse(
    CsrBtAmpmInstance * inst,
    const CsrUint8 * pMsg,
    CsrBtConnId cid)
{
    const CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);

    if (NULL != pBtDevice)
    {
        /* Any handling for NOT receiving the RESPONSE to the NOTIFY
         * is undefined so having sent the indication if we don't get
         * a RESPONSE should we resend the NOTIFY or not?
         *
         * If we assume that having sent the NOTIFY we don't really care
         * whether the destination device receives it or not then there is no
         * use for the RESPONSE.
         *
         * So just ignore it.
         */
    }
}


void CsrBtAmpmBtdevAmpGetInfoRequest(CsrBtAmpmInstance * inst,
                                     const CsrUint8 * pMsg,
                                     CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);

    if (NULL != pBtDevice)
    {
        CsrBtAmpmId amp_id;
        CsrBtAmpmLocalAmp *pLocalAMPInfo;

        amp_id =
            CSR_BT_AMPM_DECODE_U8_IE(pMsg, CSR_BT_AMPM_SDU_MESSAGE_OFFSET);

        /* Check if AMPID received in the request is a valid local AMPID */
        pLocalAMPInfo = CsrBtAmpmLocalGetById(inst, amp_id);

        if (NULL == pLocalAMPInfo)
        {
            CsrBtAmpmDataInfo ampInfo;
            CsrMblk *pMsgBlk;

            /* If the AMP ID is invalid we have to send response fields set to zero */
            CsrMemSet((void*)&ampInfo, 0, sizeof(CsrBtAmpmDataInfo));

            ampInfo.id = amp_id;

            pMsgBlk =
                CsrBtAmpmSduCreateAmpGetInfoResponse(CSR_BT_AMPM_DECODE_U8_IE
                                                     (pMsg, CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET),
                                                     CSR_BT_AMPM_GET_INFO_RSP_STATUS_INVALID_AMP_ID,
                                                     (const CsrBtAmpmDataInfo *) &ampInfo);
            L2CA_DataWriteReq((l2ca_cid_t)cid, 0, pMsgBlk);
        }
        else
        {
            /* The required AMP exists so trigger reading the AMP info */
            CsrBtAmpmLocalReadAmpInfo(pBtDevice,
                                      CSR_BT_AMPM_DECODE_U8_IE(pMsg,
                                                               CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET), pLocalAMPInfo);
        }
    }
}


void CsrBtAmpmBtdevAmpGetInfoResponse(CsrBtAmpmInstance * inst,
                                      const CsrUint8 * pMsg,
                                      CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);

    if (NULL != pBtDevice)
    {
        csrBtAmpmBtdevFsmRun(inst, pBtDevice,
                             AMPM_BTDEV_EV_AMP_GET_INFO_RESPONSE,
                             (void*)pMsg);
    }
}


void CsrBtAmpmBtdevAmpGetAmpAssocRequest(CsrBtAmpmInstance * inst,
                                         const CsrUint8 * pMsg,
                                         CsrBtConnId cid)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pMsg != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, cid);
    if (NULL != pBtDevice)
    {
        CsrBtAmpmId amp_id =
            CSR_BT_AMPM_DECODE_U8_IE(pMsg, CSR_BT_AMPM_SDU_MESSAGE_OFFSET);
        CsrBtAmpmLocalAmp *pLocalAMP = CsrBtAmpmLocalGetById(inst, amp_id);

        /* Check if AMPID received in the request is a valid local AMPID */
        if (NULL == pLocalAMP)
        {
            CsrBtAmpmAmpAssoc DummyAssoc;
            CsrMblk *pMsgBlk;

            pMsgBlk =
                CsrBtAmpmSduCreateAmpGetAmpAssocResponse
                (CSR_BT_AMPM_DECODE_U8_IE(pMsg,
                                          CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET),
                 CSR_BT_AMPM_GET_ASSOC_RSP_STATUS_INVALID_AMP_ID, amp_id,
                 &DummyAssoc);
            L2CA_DataWriteReq((l2ca_cid_t)cid, 0, pMsgBlk);
        }
        else
        {
            fsm_event_t fsmEvent = AMPM_LOCAL_EV_READ_ASSOC_REQ;
            
            /* The required AMP exists, put assoc request on queue */
            if( (CSR_BT_AMP_AMP_STATUS_AVAILABLE_BUT_PHYSICALLY_POWERED_DOWN == pLocalAMP->pAmpInfo->status)
                && (CSR_SCHED_QID_INVALID != inst->ampPowerOnQid) )
            {
                /* but first request a power on of the local AMP controller */
                fsmEvent = AMPM_LOCAL_EV_POWER_ON_GET_ASSOC_REQ;
            }
            
            CsrBtAmpmLocalReadAmpAssoc(inst, pBtDevice, pLocalAMP,
                                       CSR_BT_AMPM_DECODE_U8_IE(pMsg,
                                                                CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET),
                                       CSR_BT_AMPM_INVALID_PHYSICAL_LINK_HANDLE,
                                       fsmEvent);
        }
    }
}

fsm_event_t CsrBtAmpmBtdevFsmCheckExtFeatures(void *arg1,
                                              void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    /* Only info required is the address of the BT device we should be
     * attempting to discover, so arg 2 is not required */
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;

    /* Check that the extended features are supported by the remote device */
    L2CA_GetInfoReq(&(pBtDevice->bdAddr),
                    CSR_BT_AMPM_IFACEQUEUE,
                    L2CA_GETINFO_TYPE_EXT_FEAT,
                    0, 0);
    pBtDevice->getInfoTypeRequested = L2CA_GETINFO_TYPE_EXT_FEAT;
    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmBtdevFsmFixedChannelSupported(void *arg1,
                                                   void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    /* Only info required is the address of the BT device we should
     * be attempting to discover, so arg 2 is not required
     */
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;

    /* Check what fixed channels are supported by the remote device */
    L2CA_GetInfoReq(&(pBtDevice->bdAddr),
                    CSR_BT_AMPM_IFACEQUEUE,
                    L2CA_GETINFO_TYPE_FIX_CH, 0, 0);
    pBtDevice->getInfoTypeRequested = L2CA_GETINFO_TYPE_FIX_CH;

    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtAmpmBtdevFsmAmpmChannelSupported(void *arg1,
                                                  void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;
    TYPED_BD_ADDR_T ad;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    /* Only info required is the address of the BT device we should
     * be attempting to discover, so arg 2 is not required
     */
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;

    ad.addr = pBtDevice->bdAddr;
    ad.type = CSR_BT_ADDR_PUBLIC;
    L2CA_MapFixedCidReq(&ad,
                        L2CA_CID_AMP_MANAGER,
                        0, FALSE);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmBtdevFsmDeleteReq(void *arg1,
                                       void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);

    /* Don't bother checking arg2 as we don't know what transition
     * this one was called from but we're not going to use arg2 so
     * just ignore it. */
    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;

    /* Need to tidy up any other data associated with the device */
    csrBtAmpmBtdevDelete(param->inst, pBtDevice);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmBtdevFsmL2caMapFixedCidCfm(void *arg1,
                                                void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;
    const L2CA_MAP_FIXED_CID_CFM_T *prim;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;
    prim = (const L2CA_MAP_FIXED_CID_CFM_T *) arg2;

    pBtDevice->cid = prim->cid;
    pBtDevice->ampManagerStatus = AMPM_STATUS_PRESENT;
    csrBtAmpmBtdevSendQeueuedTestSdus(pBtDevice);

    csrBtAmpmBtdevSendAmpDiscoverRequest(param->inst, pBtDevice);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmBtdevFsmL2caMapFixedCidInd(void *arg1,
                                                void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;
    const L2CA_MAP_FIXED_CID_IND_T *prim;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;
    prim = (const L2CA_MAP_FIXED_CID_IND_T *) arg2;

    pBtDevice->ampManagerStatus = AMPM_STATUS_PRESENT;

    L2CA_MapFixedCidRsp(prim->cid, 0, FALSE);
    pBtDevice->cid = prim->cid;
    csrBtAmpmBtdevSendQeueuedTestSdus(pBtDevice);

    /* Autonomously start the discovery if at least one APP is
     * registered */
    if(CsrBtAmpmAppIsEnabled(param->inst))
    {
        csrBtAmpmBtdevSendAmpDiscoverRequest(param->inst, pBtDevice);
    }

    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtAmpmBtdevFsmRunDiscovery(void *arg1,
                                          void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    /* Only info required is the address of the BT device we should be
     * attempting to discover, so arg 2 is not required */
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;

    if(CsrBtAmpmAppIsEnabled(param->inst))
    {
        csrBtAmpmBtdevSendAmpDiscoverRequest(param->inst, pBtDevice);
    }

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmBtdevFsmAmpDiscoverRejected(void *arg1,
                                                 void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;

    csrBtAmpmBtdevCancelTimer(&(pBtDevice->sduResponseTimer));

    /* Can now consider the discovery process complete */
    pBtDevice->hasBeenDiscovered = TRUE;

    /* We can not establish what AMPs are present at the peer device
     * as it has rejected our request so assume that it has none.
     * Inform whoever needs to know.
     */
    csrBtAmpmBtdevReportDiscoveryFailure(param->inst, pBtDevice);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmBtdevFsmAmpDiscoverReq(void *arg1,
                                            void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;
    const CsrUint8 *pAmpDiscoverReq;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;
    pAmpDiscoverReq = (const CsrUint8 *) arg2;

    csrBtAmpmBtdevRespondToAmpDiscoverRequest(param->inst, pBtDevice,
                                              CSR_BT_AMPM_DECODE_U8_IE(pAmpDiscoverReq,
                                                                       CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET));

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmBtdevFsmAmpDiscoverRsp(void *arg1,
                                            void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmDataRemoteAmp *pAmpListEntry;
    const CsrUint8 *pAmpDiscoverRsp;
    CsrUint16 lengthOfAmpList = 0;

    /* The extended feature mask doesn't actually contain any fields
     * that we're interested in at the moment so we just need to be
     * able to read over it and move onto the AMP list. */
    CsrUint16 extFeatureMaskField;

    /* Each entry is a minimum of 2 bytes that will be extended by
     * another 2 byte entry if the top most bit is set. */
    CsrUint16 lengthOfExtFeatureMask = CSR_BT_AMPM_SIZEOF_EXT_FEATURE_MASK;
    CsrUint8 currentIndex = 0;
    CsrBool sentReq = FALSE;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;
    pAmpDiscoverRsp = (const CsrUint8 *) arg2;

    csrBtAmpmBtdevCancelTimer(&(pBtDevice->sduResponseTimer));

    /* Can now consider the discovery process complete */
    pBtDevice->hasBeenDiscovered = TRUE;

    /* We don't have the actual length of the SDU but it has been
     * validated already so we should not read past the end of it due
     * to incorrectly formatted extended features mask fields. */
    extFeatureMaskField = CSR_BT_AMPM_DECODE_U16_IE(pAmpDiscoverRsp,
                                                    CSR_BT_AMPM_DISCOVER_EXT_FEATURE_MASK_OFFSET);
    while (CSR_BT_AMPM_MORE_EXT_FEATURE_MASKS ==
           (extFeatureMaskField & CSR_BT_AMPM_MORE_EXT_FEATURE_MASKS))
    {
        lengthOfExtFeatureMask += CSR_BT_AMPM_SIZEOF_EXT_FEATURE_MASK;
        extFeatureMaskField =
            CSR_BT_AMPM_DECODE_U16_IE(pAmpDiscoverRsp,
                                      CSR_BT_AMPM_DISCOVER_EXT_FEATURE_MASK_OFFSET +
                                      lengthOfExtFeatureMask);
    }

    /* Need to remove the length of the fields that are not part of
     * the AMP list. */
    lengthOfAmpList = (CSR_BT_AMPM_DECODE_U16_IE(pAmpDiscoverRsp, CSR_BT_AMPM_SDU_LENGTH_OFFSET)
                       - (2 + lengthOfExtFeatureMask));
    
    csrBtAmpmBtdevUpdateRemoteAmpList(param->inst, pBtDevice, lengthOfAmpList,
                                      (pAmpDiscoverRsp + CSR_BT_AMPM_SDU_MESSAGE_OFFSET + 2 +
                                       lengthOfExtFeatureMask));

    /* Need to get the AMP info for each AMP at the remote device so
     * that we know basic capabilities of the AMP so that we can
     * decide whether to us the AMP to support a connection or not. */
    pAmpListEntry = (CsrBtAmpmDataRemoteAmp *) pBtDevice->pAMP_ListEntry;
    while (pBtDevice->numOfAMPs > currentIndex &&
           !sentReq)
    {
        if (CSR_BT_AMP_AMP_CONTROLLER_TYPE_BR_EDR != pAmpListEntry->type )
        {
            CsrMblk *pMsgBlk;

            pMsgBlk =
                CsrBtAmpmSduCreateAmpGetInfoRequest(CsrBtAmpmSduAllocIdentifier(param->inst),
                                                    pAmpListEntry->id);

            L2CA_DataWriteReq((l2ca_cid_t)pBtDevice->cid, 0, pMsgBlk);
            sentReq = TRUE;
        }
        pAmpListEntry++;
        currentIndex++;
    }

    if (sentReq)
    {
        CsrBtAmpmFsmParam *tmp;

        tmp = (CsrBtAmpmFsmParam*)CsrPmemZalloc(sizeof(CsrBtAmpmFsmParam));
        tmp->inst = param->inst;
        tmp->argp = pBtDevice;
        pBtDevice->haveAllRemoteAmpInfo = FALSE;
        pBtDevice->sduResponseTimer = CsrSchedTimerSet(CSR_BT_AMPM_SDU_REQUEST_RESPONSE_TIMEOUT,
                                                      (void (*)(CsrUint16, void *)) CsrBtAmpmBtdevSduResponseTimeoutInd,
                                                      0, tmp);
    }
    else
    {
        /* Does anyone need to be informed about this e.g. physical link? */
        pBtDevice->haveAllRemoteAmpInfo = TRUE;
        csrBtAmpmBtdevReportDiscoveryComplete(param->inst, pBtDevice);
    }

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmBtdevFsmAmpChangeInd(void *arg1,
                                          void *arg2)
{
    CsrMblk *pMsgBlk;
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmBtdev *pBtDevice;
    const CsrUint8 *pAmpChangeInd;
    CsrUint16 lengthOfAmpList;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;
    pAmpChangeInd = (const CsrUint8 *) arg2;

    /* Send the AMP CHANGE RESPONSE SDU (if we can) */
    pMsgBlk = CsrBtAmpmSduCreateAmpChangeResponse(
        CSR_BT_AMPM_DECODE_U8_IE(pAmpChangeInd,
                                 CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET)); /* Identifier from incoming request */
    L2CA_DataWriteReq((l2ca_cid_t)pBtDevice->cid, 0, pMsgBlk);

    /* Assuming that if the status of an AMP currently involved in a
     * physical link alters then that link will be disconnected and
     * that we don't have to worry about checking for that here.
     * So all we're going to do is replace the previous list.
     */
    lengthOfAmpList =
        CSR_BT_AMPM_DECODE_U16_IE(pAmpChangeInd,
                                  CSR_BT_AMPM_SDU_LENGTH_OFFSET);

    csrBtAmpmBtdevUpdateRemoteAmpList(param->inst, pBtDevice, lengthOfAmpList,
                                      (pAmpChangeInd + CSR_BT_AMPM_LIST_OFFSET_IN_CHANGE_IND));

    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtAmpmBtdevFsmLocalAmpChangeInd(void *arg1,
                                               void *arg2)
{
    CsrMblk *pMsgBlk;
    CsrBtAmpmFsmParam *param;
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;

    /* Send the AMP CHANGE INDICATION SDU
     * NOTE: not retaining the allocated SDU Identifier or starting the response timer
     *       as we're not worried whether we get the confirm or not.
     */
    csrBtAmpmBtdevCancelTimer(&(pBtDevice->sduResponseTimer));
    pMsgBlk =
        CsrBtAmpmSduCreateAmpChangeNotify(param->inst,
                                          CsrBtAmpmSduAllocIdentifier(param->inst));
    L2CA_DataWriteReq((l2ca_cid_t)pBtDevice->cid, 0, pMsgBlk);

    return FSM_EVENT_NULL;
}


fsm_event_t CsrBtAmpmBtdevFsmDisconnectReq(void *arg1,
                                           void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;
    TYPED_BD_ADDR_T ad;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;

    /* Disconnect the fixed channel */
    ad.addr = pBtDevice->bdAddr;
    ad.type = CSR_BT_ADDR_PUBLIC;
    dm_acl_close_req(&ad,
                     DM_ACL_FLAG_FORCE, /*flags*/
                     HCI_ERROR_OETC_USER, /*reason*/
                     NULL);

    csrBtAmpmBtdevReportAmpmChannelDisconnected(param->inst, pBtDevice);
    csrBtAmpmBtdevCancelTimer(&(pBtDevice->sduResponseTimer));

    /* App failure notification happens at event trigger */

    /* Trigger deletion of the instance */
    return AMPM_BTDEV_EV_DELETE_REQ;
}


fsm_event_t CsrBtAmpmBtdevFsmDisconnectInd(void *arg1,
                                           void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(NULL == arg2);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;

    csrBtAmpmBtdevReportAmpmChannelDisconnected(param->inst, pBtDevice);
    csrBtAmpmBtdevCancelTimer(&(pBtDevice->sduResponseTimer));

    /* App failure notification happens at event trigger */

    /* Trigger deletion of the instance */
    return AMPM_BTDEV_EV_DELETE_REQ;
}


fsm_event_t CsrBtAmpmBtdevFsmAmpGetInfoRsp(void *arg1,
                                           void *arg2)
{
    CsrBtAmpmBtdev *pBtDevice;
    CsrBtAmpmFsmParam *param;
    CsrUint8 sduIdentifier;
    CsrUint8 RspStatus;
    CsrBtAmpmBtdevRemoteAmpInfo *pRemoteAMPInfoEntry;

    CSR_BT_AMPM_ASSERT(arg1 != NULL);
    CSR_BT_AMPM_ASSERT(arg2 != NULL);

    param = (CsrBtAmpmFsmParam *) arg1;
    pBtDevice = (CsrBtAmpmBtdev *) param->argp;

    csrBtAmpmBtdevCancelTimer(&(pBtDevice->sduResponseTimer));

    pRemoteAMPInfoEntry = (CsrBtAmpmBtdevRemoteAmpInfo *)CsrPmemAlloc(sizeof(*pRemoteAMPInfoEntry));
    CsrBtAmpmSduDecodeAmpGetInfoResponse((CsrUint8 *) arg2,
                                         &sduIdentifier,
                                         &RspStatus,
                                         &(pRemoteAMPInfoEntry->info));

    if (CSR_BT_AMPM_GET_INFO_RSP_STATUS_SUCCESS == RspStatus)
    {
        /* The AMP Status was part of the info but has now moved to
         * the AMP DISCOVERY RESPONSE however it is more convenient to
         * have the amp status for both local and remote amps as part
         * of the info. */
        pRemoteAMPInfoEntry->infoValid = TRUE;
        pRemoteAMPInfoEntry->info.status = CsrBtAmpmBtdevRemoteAmpStatusGetById(pBtDevice,
                                                                                pRemoteAMPInfoEntry->info.id);

        /* Is this a new AMP or replacing info for an existing one, or
         * is it an error and we have received info for an AMP that is
         * not in the list any more. */
        if (csrBtAmpmBtdevAmpIdInAmpList(pRemoteAMPInfoEntry->info.id,
                                         pBtDevice->numOfAMPs,
                                         (CsrUint8 *) (pBtDevice->pAMP_ListEntry)))
        {
            CsrBtAmpmBtdevRemoteAmpInfo *pExistingAmpInfo;

            pExistingAmpInfo =
                CsrBtAmpmBtdevRemoteAmpInfoGetById(pBtDevice,
                                                   pRemoteAMPInfoEntry->info.id);

            if (NULL != pExistingAmpInfo)
            {
                CsrBtAmpmListRemoveMember(pBtDevice->pRemoteAmpInfoListAnchor,
                                          (CsrBtAmpmGenListMember *) pExistingAmpInfo);
                CsrPmemFree((void *) pExistingAmpInfo);
            }
            CsrBtAmpmListAddHead(pBtDevice->pRemoteAmpInfoListAnchor,
                                 (CsrBtAmpmGenListMember *) pRemoteAMPInfoEntry);
        }
        else
        {
            CsrPmemFree((void *) pRemoteAMPInfoEntry);
        }

        if (!csrBtAmpmBtdevRequestOutstandingRemoteAmpInfo(param->inst,
                                                           pBtDevice))
        {
            pBtDevice->haveAllRemoteAmpInfo = TRUE;
            /* Does anyone need to be informed about this
             * e.g. physical link? */
            csrBtAmpmBtdevReportDiscoveryComplete(param->inst, pBtDevice);
        }
    }
    else
    {
        pRemoteAMPInfoEntry->infoValid = FALSE;
        CsrPmemFree(pRemoteAMPInfoEntry);
    }

    return FSM_EVENT_NULL;
}
