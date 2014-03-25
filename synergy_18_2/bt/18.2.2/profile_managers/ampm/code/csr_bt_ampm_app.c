/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.
   
REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

typedef enum
{
    CSR_BT_AMPM_NOAMP_ERROR,
    CSR_BT_AMPM_NOAMP_QUIET
} csrBtAmpmAppNoamp;

typedef struct
{
    CsrSchedQid qid;
    CsrBtDeviceAddr addr;
    void *val;
} csrBtAmpmAppSearchParam;

/* Count number of available local/remote AMP pairs */
static CsrUint8 CsrBtAmpmAppCountAmpPairs(CsrBtAmpmInstance *inst,
                                          CsrBtAmpmBtdev *btdev)
{
    CsrUint8 count = 0;
    CsrUint8 i;
    CsrBtAmpmLocalAmp *local;

    for(local = (CsrBtAmpmLocalAmp*)CsrCmnListGetFirst(&inst->localAmpList);
        local != NULL;
        local = (CsrBtAmpmLocalAmp*)local->next)
    {
        CsrBtAmpmDataRemoteAmp *remote;
        remote = btdev->pAMP_ListEntry;

        for(i=0;
            i < btdev->numOfAMPs;
            i++, remote++)
        {
            if(remote->type == local->pAmpInfo->type)
            {
                count++;
            }
        }
    }

    return count;
}

/* Fill AMP controller listing */
void CsrBtAmpmAppFillControllerList(CsrBtAmpmInstance *inst,
                                    CsrBtAmpmBtdev *btdev,
                                    CsrBtAmpmControllerEntry *list)
{
    CsrUint8 c=0;
    CsrUint8 i;
    CsrBtAmpmLocalAmp *local;

    for(local = (CsrBtAmpmLocalAmp*)CsrCmnListGetFirst(&inst->localAmpList);
        local != NULL;
        local = (CsrBtAmpmLocalAmp*)local->next)
    {
        CsrBtAmpmDataRemoteAmp *remote;
        remote = btdev->pAMP_ListEntry;

        for(i=0;
            i < btdev->numOfAMPs;
            i++, remote++)
        {
            if(remote->type == local->pAmpInfo->type)
            {
                list[c].ampType = local->pAmpInfo->type;
                list[c].status = remote->status;
                list[c].caps = CSR_BT_AMPM_AMPLIST_CAPS_UNUSED;
                list[c].localId = local->pAmpInfo->id;
                list[c].remoteId = remote->id;
                c++;
            }
        }
    }
}

/* Guess a local/remote AMP controller ID pair */
static CsrBool CsrBtAmpmAppGuessControllerIds(CsrBtAmpmInstance *inst,
                                              CsrBtAmpmBtdev *btdev,
                                              CsrBtAmpController *localId,
                                              CsrBtAmpController *remoteId)
{
    CsrUint8 i;
    CsrBtAmpmLocalAmp *local;

    for(local = (CsrBtAmpmLocalAmp*)CsrCmnListGetFirst(&inst->localAmpList);
        local != NULL;
        local = (CsrBtAmpmLocalAmp*)local->next)
    {
        CsrBtAmpmDataRemoteAmp *remote;
        remote = btdev->pAMP_ListEntry;

        for(i=0;
            i < btdev->numOfAMPs;
            i++, remote++)
        {
            if(remote->type == local->pAmpInfo->type)
            {
                *localId = local->pAmpInfo->id;
                *remoteId = remote->id;
                return TRUE;
            }
        }
    }
    return FALSE;
}


/* App list element initialiser */
static void csrBtAmpmAppElemInit(CsrCmnListElm_t *elem)
{
    CsrBtAmpmAppSubscriberElem *new = (CsrBtAmpmAppSubscriberElem*)elem;
    CsrMemSet(&(new->qid),
              0,
              sizeof(CsrBtAmpmAppSubscriberElem) - sizeof(CsrCmnListElm_t));
}


/* Search appList by <qid,addr,connId> */
static CsrBool csrBtAmpmAppElemSearchFull(CsrCmnListElm_t *entry,
                                          void *sp)
{
    CsrBtAmpmAppSubscriberElem *elm = (CsrBtAmpmAppSubscriberElem*)entry;
    csrBtAmpmAppSearchParam *search = (csrBtAmpmAppSearchParam*)sp;

    if((elm->qid == search->qid) &&
       CsrBtBdAddrEq(&elm->addr, &search->addr) &&
       (elm->btConnId == *(CsrBtConnId*)search->val))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Search appList by <qid,connId> */
static CsrBool csrBtAmpmAppElemSearchWildcard(CsrCmnListElm_t *entry,
                                              void *sp)
{
    CsrBtAmpmAppSubscriberElem *elm = (CsrBtAmpmAppSubscriberElem*)entry;
    csrBtAmpmAppSearchParam *search = (csrBtAmpmAppSearchParam*)sp;

    if((elm->qid == search->qid) &&
       (elm->btConnId == *(CsrBtConnId*)search->val))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Search applist by connId */
static CsrBool csrBtAmpmAppElemSearchConnId(CsrCmnListElm_t *entry,
                                            void *sp)
{
    CsrBtAmpmAppSubscriberElem *elm = (CsrBtAmpmAppSubscriberElem*)entry;
    CsrBtConnId *btConnId = (CsrBtConnId*)sp;

    if(elm->btConnId == *btConnId)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Search applist for first entry with the test-mode bit set */
static CsrBool csrBtAmpmAppElemSearchTestApp(CsrCmnListElm_t *entry,
                                             void *sp)
{
    CsrBtAmpmAppSubscriberElem *elm = (CsrBtAmpmAppSubscriberElem*)entry;
    CSR_UNUSED(sp);
    if((elm->flags & CSR_BT_AMPM_FLAGS_ENABLE_TEST) == CSR_BT_AMPM_FLAGS_ENABLE_TEST)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Discover request list element initialiser */
static void csrBtAmpmAppDiscoveryElemInit(CsrCmnListElm_t *elem)
{
    CsrBtAmpmAppDiscoveryElem *new = (CsrBtAmpmAppDiscoveryElem*)elem;
    CsrMemSet(&(new->qid),
              0,
              sizeof(CsrBtAmpmAppDiscoveryElem) - sizeof(CsrCmnListElm_t));
}

/* Discover request search */
static CsrBool csrBtAmpmAppDiscoveryElemSearchAddr(CsrCmnListElm_t *entry,
                                                   void *sp)
{
    CsrBtAmpmAppDiscoveryElem *elm = (CsrBtAmpmAppDiscoveryElem*)entry;
    CsrBtDeviceAddr *addr = (CsrBtDeviceAddr*)sp;

    if(CsrBtBdAddrEq(&elm->addr, addr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Generate controller indication for btdev/app pair. Returns number
 * of valid entries */
static CsrUint8 csrBtAmpmAppGenControllerMsg(CsrBtAmpmPrim type,
                                             CsrBtAmpmInstance *inst,
                                             CsrSchedQid qid,
                                             CsrBtDeviceAddr *addr,
                                             CsrBtConnId btConnId,
                                             CsrBtAmpmBtdev *btdev,
                                             CsrBtAmpmResult result,
                                             csrBtAmpmAppNoamp noamp)
{
    CsrBtAmpmControllerEntry *amps;
    CsrUint8 num;

    num = (CsrUint8)(btdev != NULL
                     ? CsrBtAmpmAppCountAmpPairs(inst, btdev)
                     : 0);

    /* No AMP pairs available, bail out */
    if(num == 0)
    {
        if(noamp == CSR_BT_AMPM_NOAMP_ERROR)
        {
            CsrBtAmpmUpstreamControllerDone(type, qid, btConnId, *addr,
                                            0, NULL,
                                            result);
        }
        return 0;
    }

    /* AMP pairs available, so successful. Send up list */
    amps = CsrPmemZalloc(sizeof(CsrBtAmpmControllerEntry) * num);
    CsrBtAmpmAppFillControllerList(inst, btdev, amps);
    CsrBtAmpmUpstreamControllerDone(type,
                                    qid, btConnId, *addr,
                                    num, amps,
                                    CSR_BT_AMPM_RESULT_SUCCESS);

    return num;
}

/* Discovery failed towards named device */
void CsrBtAmpmAppFailNotification(CsrBtAmpmInstance *inst,
                                  CsrBtDeviceAddr *addr,
                                  CsrBtAmpmResult result)
{
    CsrBtAmpmAppDiscoveryElem *disc;

    /* Don't allow double register for same qid/connId pair */
    /* Remove any pending requests related to this registration */
    while((disc = (CsrBtAmpmAppDiscoveryElem*)CsrCmnListSearch(&inst->discoveryList,
                                                               csrBtAmpmAppDiscoveryElemSearchAddr,
                                                               addr)) != NULL)
    {
        (void)csrBtAmpmAppGenControllerMsg(CSR_BT_AMPM_CONTROLLER_CFM,
                                           inst,
                                           disc->qid,
                                           &disc->addr,
                                           disc->btConnId,
                                           NULL,
                                           result,
                                           CSR_BT_AMPM_NOAMP_ERROR);

        CsrCmnListElementRemove(&inst->discoveryList,
                                (CsrCmnListElm_t*)disc);
    }
}


/* Trigger application notification scan update */
void CsrBtAmpmAppCheckNotification(CsrBtAmpmInstance *inst)
{
    CsrBtAmpmAppSubscriberElem *app;
    CsrBtAmpmAppDiscoveryElem *disc;
    CsrBtAmpmBtdev *btdev;
    CsrUint8 num;

    /* Scan subscribers */
    for(app = (CsrBtAmpmAppSubscriberElem*)CsrCmnListGetFirst(&inst->appList);
        app != NULL;
        app = (CsrBtAmpmAppSubscriberElem*)app->next)
    {
        btdev = CsrBtAmpmBtdevGetByBdaddr(inst, app->addr);

        if((app->btConnId != CSR_BT_CONN_ID_INVALID) &&
           !CsrBtBdAddrEqZero(&app->addr) &&
           (btdev != NULL))
        {
            if((app->eventMask & CSR_BT_AMPM_EVENTS_AUTO_DISCOVERY) &&
               CsrBtAmpmBtdevIsAmpable(btdev))
            {
                /* Report AMP listing now that we know peer exists,
                 * matches a subscription - and has more than the
                 * BR/EDR in the list */
                num = csrBtAmpmAppGenControllerMsg(CSR_BT_AMPM_CONTROLLER_IND,
                                                   inst, app->qid,
                                                   &app->addr,
                                                   app->btConnId,
                                                   btdev,
                                                   CSR_BT_AMPM_RESULT_SUCCESS,
                                                   CSR_BT_AMPM_NOAMP_QUIET);

                /* If we sent up a non-empty list the last time and
                 * the list is now empty, we must inform app of list
                 * emptiness... */
                if((num == 0) && (app->lastAmpsCount > 0))
                {
                    CsrBtAmpmUpstreamControllerDone(CSR_BT_AMPM_CONTROLLER_IND,
                                                    app->qid,
                                                    app->btConnId,
                                                    app->addr,
                                                    0, NULL,
                                                    CSR_BT_AMPM_RESULT_SUCCESS);
                }
                app->lastAmpsCount = num;
            } /* availability */

            /* Is automatic move request mode enabled? */
            if(app->flags & CSR_BT_AMPM_FLAGS_AUTO_MOVE_ALWAYS)
            {
                CsrBtAmpController localId = CSR_BT_AMP_CONTROLLER_UNKNOWN;
                CsrBtAmpController remoteId = CSR_BT_AMP_CONTROLLER_UNKNOWN;
                if(CsrBtAmpmAppGuessControllerIds(inst, btdev, &localId, &remoteId))
                {
                    CsrBtAmpmAppCmAutoMoveChan(inst, app, localId, remoteId);
                }
            }
        } /* valid btConnId, addr, btdev */
    }

    /* Scan discovery requestors, and remove requests as we find
     * them */
    disc = (CsrBtAmpmAppDiscoveryElem*)CsrCmnListGetFirst(&inst->discoveryList);
    while(disc)
    {
        btdev = CsrBtAmpmBtdevGetByBdaddr(inst, disc->addr);

        if((btdev != NULL) &&
           CsrBtAmpmBtdevIsDiscovered(btdev))
        {
            CsrBtAmpmAppDiscoveryElem *next;

            /* Discovery request matches, but peer does not have
             * any AMPS */
            csrBtAmpmAppGenControllerMsg(CSR_BT_AMPM_CONTROLLER_CFM,
                                         inst,
                                         disc->qid,
                                         &disc->addr,
                                         disc->btConnId,
                                         btdev,
                                         CSR_BT_AMPM_RESULT_NO_REMOTE_AMPS,
                                         CSR_BT_AMPM_NOAMP_ERROR);

            /* Remove element and progress */
            next = (CsrBtAmpmAppDiscoveryElem*)disc->next;
            CsrCmnListElementRemove(&inst->discoveryList,
                                    (CsrCmnListElm_t*)disc);
            disc = next;
        }
        else
        {
            /* Next */
            disc = (CsrBtAmpmAppDiscoveryElem*)disc->next;
        }
    }
}

/* App register */
void CsrBtAmpmAppRegisterReq(CsrBtAmpmInstance *inst,
                             CsrBtAmpmRegisterReq *req)
{
    CsrBtAmpmAppSubscriberElem *app;
    csrBtAmpmAppSearchParam search;
    CsrBool first;

    /* Sanity check */
    if(req->qid == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    /* Will this registration be the first one? */
    first = !CsrBtAmpmAppIsEnabled(inst);

    /* Don't allow double register for same qid/connId pair */
    search.qid = req->qid;
    search.addr = req->addr;
    search.val = &req->btConnId;
    app = (CsrBtAmpmAppSubscriberElem*)CsrCmnListSearch(&inst->appList,
                                                        csrBtAmpmAppElemSearchFull,
                                                        &search);
    if((app == NULL) &&
       (req->btConnId != CSR_BT_CONN_ID_INVALID))
    {
        app = (CsrBtAmpmAppSubscriberElem*)CsrCmnListSearch(&inst->appList,
                                                            csrBtAmpmAppElemSearchConnId,
                                                            &req->btConnId);
    }
    if(app != NULL)
    {
        CsrBtAmpmUpstreamRegisterCfm(req->qid,
                                     CSR_BT_AMPM_EVENTS_DISABLE_ALL,
                                     req->btConnId,
                                     req->addr,
                                     CSR_BT_AMPM_RESULT_ALREADY_REGISTERED);
        return;
    }

    app = (CsrBtAmpmAppSubscriberElem*)CsrCmnListElementAddFirst(&inst->appList,
                                                       sizeof(CsrBtAmpmAppSubscriberElem));
    app->qid = req->qid;
    app->flags = req->flags;
    app->eventMask = req->eventMask;
    app->btConnId = req->btConnId;
    app->addr = req->addr;

    CsrBtAmpmUpstreamRegisterCfm(req->qid,
                                 req->eventMask,
                                 req->btConnId,
                                 req->addr,
                                 CSR_BT_AMPM_RESULT_SUCCESS);

    /* Force scan of all existing connections if this registration is
     * the first one */
    if(first)
    {
        CsrBtAmpmBtdev *btdev;
        for(btdev = (CsrBtAmpmBtdev*)inst->pBTDeviceListAnchor->head;
            btdev != NULL;
            btdev = (CsrBtAmpmBtdev*)((CsrBtAmpmGenListMember*)btdev)->next)
        {
            CsrBtAmpmBtdevKickDiscovery(inst, btdev->bdAddr);
        }
    }

    /* Trigger immediate notitication scan */
    CsrBtAmpmAppCheckNotification(inst);
}

/* App unregister */
void CsrBtAmpmAppDeregisterReq(CsrBtAmpmInstance *inst,
                               CsrBtAmpmDeregisterReq *req)
{
    CsrBtAmpmAppSubscriberElem *app;
    csrBtAmpmAppSearchParam search;
    CsrBtAmpmAppDiscoveryElem *disc;

    /* Sanity check */
    if(req->qid == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    /* Don't allow double register for same qid/connId pair */
    search.qid = req->qid;
    search.addr = req->addr;
    search.val = &req->btConnId;
    app = (CsrBtAmpmAppSubscriberElem*)CsrCmnListSearch(&inst->appList,
                                                        csrBtAmpmAppElemSearchFull,
                                                        &search);
    if(app == NULL)
    {
        CsrBtAmpmUpstreamDeregisterCfm(req->qid,
                                       req->btConnId,
                                       req->addr,
                                       CSR_BT_AMPM_RESULT_ERROR);
        return;
    }

    /* We can only remove idle instances */
    if(app->state != CSR_BT_AMPM_APP_STATE_IDLE)
    {
        CsrBtAmpmUpstreamDeregisterCfm(req->qid,
                                       req->btConnId,
                                       req->addr,
                                       CSR_BT_AMPM_RESULT_WRONG_STATE);
        return;
    }

    /* Remove any pending requests related to this registration */
    while((disc = (CsrBtAmpmAppDiscoveryElem*)CsrCmnListSearch(&inst->discoveryList,
                                                               csrBtAmpmAppDiscoveryElemSearchAddr,
                                                               &req->addr)) != NULL)
    {
        csrBtAmpmAppGenControllerMsg(CSR_BT_AMPM_CONTROLLER_CFM,
                                     inst,
                                     disc->qid,
                                     &disc->addr,
                                     disc->btConnId,
                                     NULL,
                                     CSR_BT_AMPM_RESULT_NOT_REGISTERED,
                                     CSR_BT_AMPM_NOAMP_ERROR);

        CsrCmnListElementRemove(&inst->discoveryList,
                                (CsrCmnListElm_t*)disc);
    }

    CsrBtAmpmUpstreamDeregisterCfm(req->qid,
                                   req->btConnId,
                                   req->addr,
                                   CSR_BT_AMPM_RESULT_SUCCESS);

    CsrCmnListElementRemove(&inst->appList,
                            (CsrCmnListElm_t*)app);
}

/* App register power on handler */
void CsrBtAmpmAppRegisterPowerOnReq(CsrBtAmpmInstance *inst,
                             CsrBtAmpmRegisterPowerOnReq *req)
{
    inst->ampPowerOnQid = req->qid;
}

void CsrBtAmpmAppPowerOnRes(CsrBtAmpmInstance *inst,
                             CsrBtAmpmPowerOnRes *res)
{
    if(!CsrBtAmpmLocalPowerOnRes(inst,res))
    {
        CsrBtAmpmPhyPowerOnRes(inst, res);
    }
}

/* App requests controller listing */
void CsrBtAmpmAppControllerReq(CsrBtAmpmInstance *inst,
                               CsrBtAmpmControllerReq *req)
{
    CsrBtAmpmAppSubscriberElem *app;
    csrBtAmpmAppSearchParam search;
    CsrBtAmpmResult result = CSR_BT_AMPM_RESULT_SUCCESS;
    CsrBtAmpmBtdev *btdev;
    CsrBtAmpmAppDiscoveryElem *disc;

    /* App must be registered */
    search.qid = req->qid;
    search.addr = req->addr;
    search.val = &req->btConnId;
    app = (CsrBtAmpmAppSubscriberElem*)CsrCmnListSearch(&inst->appList,
                                              csrBtAmpmAppElemSearchWildcard,
                                              &search);
    if((result == CSR_BT_AMPM_RESULT_SUCCESS) &&
       (app == NULL))
    {
        result = CSR_BT_AMPM_RESULT_NOT_REGISTERED;
    }

    /* SSP must be enabled */
    if((result == CSR_BT_AMPM_RESULT_SUCCESS) &&
       !inst->ampSspEnabled)
    {
        result = CSR_BT_AMPM_RESULT_SSP_NOT_ENABLED;
    }

    /* Must have a target address */
    if((result == CSR_BT_AMPM_RESULT_SUCCESS) &&
       CsrBtBdAddrEqZero(&req->addr))
    {
        result = CSR_BT_AMPM_RESULT_INVALID_PARAM;
    }

    /* Must have local controller to do this */
    if((result == CSR_BT_AMPM_RESULT_SUCCESS) &&
       (CsrCmnListGetCount(&inst->localAmpList) == 0))
    {
        result = CSR_BT_AMPM_RESULT_NO_LOCAL_AMPS;
    }

    /* Bail out if error detected */
    if(result != CSR_BT_AMPM_RESULT_SUCCESS)
    {
        CsrBtAmpmUpstreamControllerDone(CSR_BT_AMPM_CONTROLLER_CFM,
                                        req->qid,
                                        req->btConnId,
                                        req->addr,
                                        0, /* ampsCount */
                                        NULL, /* amps */
                                        result);
        return;
    }

    /* Check if remote device is known */
    btdev = CsrBtAmpmBtdevGetByBdaddr(inst, req->addr);
    if((btdev != NULL) &&
       (CsrBtAmpmBtdevIsDiscovered(btdev)))
    {
        /* Discovery request matches, but peer does not have any
         * AMPS. Generate response now, and bail out */
        csrBtAmpmAppGenControllerMsg(CSR_BT_AMPM_CONTROLLER_CFM,
                                     inst,
                                     app->qid,
                                     &app->addr,
                                     app->btConnId,
                                     btdev,
                                     CSR_BT_AMPM_RESULT_NO_REMOTE_AMPS,
                                     CSR_BT_AMPM_NOAMP_ERROR);
        return;
    }

    /* Queue discovery request */
    disc = (CsrBtAmpmAppDiscoveryElem*)CsrCmnListElementAddLast(&inst->discoveryList,
                                                                sizeof(CsrBtAmpmAppDiscoveryElem));
    disc->qid = req->qid;
    disc->btConnId = req->btConnId;
    disc->addr = req->addr;

    /* Start discovery, and/or trigger immediate notification */
    CsrBtAmpmBtdevKickDiscovery(inst, req->addr);
    CsrBtAmpmAppCheckNotification(inst);
}

/* Return TRUE if AMPM has at least one registered application */
CsrBool CsrBtAmpmAppIsEnabled(CsrBtAmpmInstance *inst)
{
    return (CsrCmnListGetCount(&inst->appList) > 0);
}

/* Return subscriber given btConnId */
CsrBtAmpmAppSubscriberElem *CsrBtAmpmAppGetSubscriber(CsrBtAmpmInstance *inst,
                                                      CsrBtConnId btConnId)
{
    return (CsrBtAmpmAppSubscriberElem*)CsrCmnListSearch(&inst->appList,
                                                         csrBtAmpmAppElemSearchConnId,
                                                         &btConnId);
}

/* Return subscriber with test mode enabled */
CsrBtAmpmAppSubscriberElem *CsrBtAmpmAppGetTestSubscriber(CsrBtAmpmInstance *inst)
{
    return (CsrBtAmpmAppSubscriberElem*)CsrCmnListSearch(&inst->appList,
                                                         csrBtAmpmAppElemSearchTestApp,
                                                         NULL);
}


/* Subsystem init function */
void CsrBtAmpmAppInit(CsrBtAmpmInstance *inst)
{
    CsrCmnListInit(&inst->appList, 0,
                   csrBtAmpmAppElemInit,
                   NULL);

    CsrCmnListInit(&inst->discoveryList, 0,
                   csrBtAmpmAppDiscoveryElemInit,
                   NULL);
}

/* Subsystem shutdown function */
void CsrBtAmpmAppDeinit(CsrBtAmpmInstance *inst)
{
    CsrCmnListDeinit(&inst->appList);
    CsrCmnListDeinit(&inst->discoveryList);
}
