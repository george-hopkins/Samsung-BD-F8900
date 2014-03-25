/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_sdc.h"
#include "csr_bt_cm_dm_sc_ssp_handler.h"
#include "csr_bt_cm_events_handler.h"

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
#include "csr_bt_cm_bnep.h"
#endif

/* --- Static functions --- */

/* CM cache table related functions */
static void csrBtCmInitializeCacheParamEntry(dmCacheParamEntry * cacheEntry)
{
    bd_addr_zero(&(cacheEntry->deviceAddr));
    cacheEntry->seqNumber        = 0;
    cacheEntry->clockOffset      = HCI_CLOCK_OFFSET_INVALID;
    cacheEntry->pageScanMode     = HCI_PAGE_SCAN_MODE_DEFAULT;
    cacheEntry->pageScanRepMode  = HCI_PAGE_SCAN_REP_MODE_R2;
}

static void csrBtCmInitializeCacheEntryPool(dmCacheParamPool * currentEntryPool)
{
    if (currentEntryPool)
    {
        CsrUintFast16    i;

        for (i = 0; i < CM_DM_CACHE_PARAM_POOL_SIZE; i++)
        {
            csrBtCmInitializeCacheParamEntry(&(currentEntryPool->cacheTable[i]));
        }
        currentEntryPool->nextCachePool                 = NULL;
        currentEntryPool->numOfEntriesInPool            = 0;
    }
}

static void csrBtCmInsertEntryInCacheParamTable(cmInstanceData_t *cmData,
                                           dmCacheParamEntry ** returnEntry,
                                           CsrBtDeviceAddr *devAddr)
{
    dmCacheParamPool * currentEntryPool;

    if (cmData->dmVar.dmCacheParamTable)
    {
        dmCacheParamPool    * previousEntryPool;
        dmCacheParamEntry   * absLowestSeqNr;
        dmCacheParamEntry   * lowestSeqNrAfterWrapAround;

        currentEntryPool           = cmData->dmVar.dmCacheParamTable;
        absLowestSeqNr             = NULL;
        lowestSeqNrAfterWrapAround = NULL;

        do
        {
            CsrUintFast16    i;

            for ( i = 0; i < CM_DM_CACHE_PARAM_POOL_SIZE; i++)
            {
                if (CsrBtBdAddrEqZero(&(currentEntryPool->cacheTable[i].deviceAddr)) ||
                    CsrBtBdAddrEq( &(currentEntryPool->cacheTable[i].deviceAddr), devAddr ) )
                {
                    if ( CsrBtBdAddrEqZero(&(currentEntryPool->cacheTable[i].deviceAddr)) )
                    {
                        currentEntryPool->numOfEntriesInPool++;
                    }
                    *returnEntry = &(currentEntryPool->cacheTable[i]);
                    return;
                }
                else
                {
                    if ( (absLowestSeqNr == NULL) || (currentEntryPool->cacheTable[i].seqNumber < absLowestSeqNr->seqNumber) )
                    {
                        absLowestSeqNr = &(currentEntryPool->cacheTable[i]);
                    }

                    if ( currentEntryPool->cacheTable[i].seqNumber > (cmData->dmVar.seqNumber - 1) )
                    {
                        if ( (lowestSeqNrAfterWrapAround == NULL) ||
                            (currentEntryPool->cacheTable[i].seqNumber < lowestSeqNrAfterWrapAround->seqNumber) )
                        {
                            lowestSeqNrAfterWrapAround = &(currentEntryPool->cacheTable[i]);
                        }
                    }
                }
            }

            previousEntryPool   = currentEntryPool;
            currentEntryPool    = currentEntryPool->nextCachePool;
        }while (currentEntryPool);

        if(cmData->dmVar.numberOfCachePools < CM_DM_MAX_NUM_OF_CACHE_POOLS)
        {
            currentEntryPool                    = (dmCacheParamPool *) CsrPmemAlloc(sizeof(dmCacheParamPool));
            previousEntryPool->nextCachePool    = currentEntryPool;
            cmData->dmVar.numberOfCachePools++;
        }
        else
        {
            /* Overwrite existing entry */
            if (lowestSeqNrAfterWrapAround != NULL)
            {
                *returnEntry = lowestSeqNrAfterWrapAround;
            }
            else
            {
                *returnEntry = absLowestSeqNr;
            }
            return;
        }
    }
    else
    {
        cmData->dmVar.dmCacheParamTable  = (dmCacheParamPool *) CsrPmemAlloc(sizeof(dmCacheParamPool));
        currentEntryPool                 = cmData->dmVar.dmCacheParamTable;
        cmData->dmVar.numberOfCachePools = 1;
    }
    csrBtCmInitializeCacheEntryPool(currentEntryPool);
    currentEntryPool->numOfEntriesInPool++;
    *returnEntry = &(currentEntryPool->cacheTable[0]);
}

static CsrBool csrBtCmGetEntryInCacheParamTable(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, dmCacheParamEntry ** returnEntry)
{
    if (cmData->dmVar.dmCacheParamTable)
    {
        CsrUintFast16            i;
        dmCacheParamPool * currentEntryPool;

        currentEntryPool = cmData->dmVar.dmCacheParamTable;

        do
        {
            for ( i = 0; i < CM_DM_CACHE_PARAM_POOL_SIZE; i++)
            {
                if (bd_addr_eq(&(currentEntryPool->cacheTable[i].deviceAddr), &deviceAddr))
                {
                    *returnEntry = &(currentEntryPool->cacheTable[i]);
                   return TRUE;
                }
            }
            currentEntryPool        = currentEntryPool->nextCachePool;
        } while (currentEntryPool);

        *returnEntry = NULL;
        return FALSE;
    }
    else
    {
        *returnEntry = NULL;
        return FALSE;
    }
}

static void csrBtCmFlushCacheEntryPool(dmCacheParamPool * currentEntryPool)
{
    if (currentEntryPool)
    {
        CsrUintFast16    i;

        for (i = 0; i < CM_DM_CACHE_PARAM_POOL_SIZE; i++)
        {
            currentEntryPool->cacheTable[i].pageScanMode     = HCI_PAGE_SCAN_MODE_DEFAULT;
            currentEntryPool->cacheTable[i].pageScanRepMode  = HCI_PAGE_SCAN_REP_MODE_R2;
        }
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrBtCmRemoveCacheParamTable(cmInstanceData_t *cmData)
{
    if (cmData->dmVar.dmCacheParamTable)
    {
        dmCacheParamPool * currentEntryPool;
        dmCacheParamPool * previousEntryPool;
        currentEntryPool = cmData->dmVar.dmCacheParamTable;

        do
        {
            previousEntryPool = currentEntryPool;
            currentEntryPool  = currentEntryPool->nextCachePool;
            CsrPmemFree(previousEntryPool);

        } while (currentEntryPool);

        cmData->dmVar.numberOfCachePools = 0;
        cmData->dmVar.dmCacheParamTable = NULL;
    }
}
#endif

static void csrBtCmCmDmCacheEntryAddSeqNr(cmInstanceData_t *cmData, dmCacheParamEntry *entry)
{

    entry->seqNumber        = cmData->dmVar.seqNumber;

    if (CM_DM_MAX_SEQ_NUMBER == cmData->dmVar.seqNumber)
    {
        cmData->dmVar.seqNumber = 1;
    }
    else
    {
        cmData->dmVar.seqNumber = (CsrUint16)(cmData->dmVar.seqNumber + 1);
    }
}

/* DM communication related functions */

static void csrBtCmWriteCachedClockOffset(CsrBtDeviceAddr devAddr,
                              CsrUint16     clockOffset)
{
    dm_write_cached_clock_offset_req(&devAddr,
                                     clockOffset,
                                     NULL);
}


static void csrBtCmWriteCachedPageMode(CsrBtDeviceAddr         devAddr,
                           page_scan_mode_t     pageScanMode,
                           page_scan_rep_mode_t pageScanRepMode)
{
    dm_write_cached_page_mode_req(&devAddr,
                                  pageScanMode,
                                  pageScanRepMode,
                                  NULL);
}


/* Handler functions for resuming prior activities */

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
static void csrBtCmDmWriteCachedPageModeCfmRfcHandler(cmInstanceData_t *cmData, cmRfcConnElement * theRfcElement)
{
    if (theRfcElement && theRfcElement->cmRfcConnInst)
    {
        cmRfcConnInstType *theLogicalLink = theRfcElement->cmRfcConnInst;

        if (!cmData->rfcVar.cancelConnect)
        {
            CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connect_s);
            CsrBtCmRfcStartInitiateConnection(cmData, theLogicalLink);
            CsrBtCmDmLocalQueueHandler();
        }
        else
        { /* The application has requested to cancel the connection */
            CsrUint8 numOfConnection     = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, theLogicalLink->deviceAddr);

            if(numOfConnection == 0)
            { /* There is no other connection attach to the device address.
                Inform the application. */
                CsrBtCmConnectCfmMsgSend(cmData, CSR_BT_RESULT_CODE_CM_CANCELLED, CSR_BT_SUPPLIER_CM);
                CsrBtCmDmLocalQueueHandler();
            }
            else
            { /* Currently there is at least one more connection attach to the
                device address. Ensure that SNIFF and park is supported */
                cmData->smVar.resultCode        = CSR_BT_RESULT_CODE_CM_CANCELLED;
                cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_CM;
                CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, theLogicalLink->deviceAddr, KEEP_CURRENT_PLAYER);
                CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connect_s);
            }
        }
    }
    else
    {
        if (theRfcElement)
        { /* If theRfcElement == NULL then this function has allready been called */
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
        CsrBtCmDmLocalQueueHandler();
    }
}
#endif


#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
static void csrBtCmDmWriteCachedPageModeCfmL2caHandler(cmInstanceData_t *cmData, cmL2caConnElement * theL2caElement)
{
    if (theL2caElement && theL2caElement->cmL2caConnInst)
    {
        cmL2caConnInstType *l2CaConnection = theL2caElement->cmL2caConnInst;

        if (!cmData->l2caVar.cancelConnect)
        {
            /* Check whether we need to apply the legacy
             * pairing ACL detach work around */
            if(!CsrBtCmL2caCheckLegacyDetach(cmData, l2CaConnection))
            {
                CsrBtCml2caAutoConnectSetup(cmData, l2CaConnection);
                CsrBtCmDmLocalQueueHandler();
            }
        }
        else
        {
            CsrBtCmL2caConnectCancelCleanup(cmData, theL2caElement, TRUE);
        }
    }
    else
    {
        if (theL2caElement)
        { /* If theL2caElement == NULL then this function has allready been called */
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
        CsrBtCmDmLocalQueueHandler();
    }
}
#endif


#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
static void csrBtCmDmWriteCachedPageModeCfmBnepHandler(cmInstanceData_t *cmData)
{
    CsrUint8                                 theIndex;
    bnepTable *bnepConnection;

    theIndex    = returnIndexToThisBdAddr(cmData, cmData->dmVar.cacheTargetDev);
    bnepConnection = &(cmData->bnepVar.connectVar[theIndex]);

    if (!cmData->bnepVar.cancelConnect)
    {
        CSR_BT_CM_STATE_CHANGE(bnepConnection->state, bnepConnect_s);
        bnepConnection->actualMode      = CSR_BT_ACTIVE_MODE;
        CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectState, CM_BNEP_CONNECT);
        CsrBtBnepConnectReqSend(cmData->bnepVar.connectReqFlags, CsrBtBdAddrToEtherAddr(&bnepConnection->deviceAddr));
        CsrBtCmDmLocalQueueHandler();

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
        /* Raise event that connection operation is started */
        CsrBtCmPropgateEvent(cmData,
                            CsrBtCmPropagateInquiryPageEvents,
                            CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE,
                            HCI_SUCCESS,
                            NULL,
                            NULL);
#endif
    }
    else
    {
        CsrUint8 numOfConnection         = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, bnepConnection->deviceAddr);
        cmData->smVar.resultCode        = CSR_BT_RESULT_CODE_CM_CANCELLED;
        cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_CM;

        CsrBtCmBnepConnectIndMsgSend(cmData, cmData->bnepVar.appHandle, ID_EMPTY,
                        CsrBtBdAddrToEtherAddr(&bnepConnection->deviceAddr), 0, 0, 0,
                        cmData->smVar.resultCode, cmData->smVar.resultSupplier);

        if(numOfConnection == 0)
        { /* There is no other connection attach to the device address.
          Inform the application, restore the local service manager
            queue, and clear up the bnep connection table                  */
            CsrBtCmBnepClearBnepTableIndex(bnepConnection);
            CsrBtCmDmLocalQueueHandler();
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
        else
        { /* Currently there is at least one more connection attach to the
            device address. Ensure that SNIFF and park is supported         */
            CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, bnepConnection->deviceAddr, KEEP_CURRENT_PLAYER);
            CSR_BT_CM_STATE_CHANGE(bnepConnection->state, bnepConnectCfm_s);

        }
        CsrBtCmDmUpdateAndClearCachedParamReqSend(bnepConnection->deviceAddr);
    }
}
#endif


static void csrBtCmDmWriteCachedPageModeCfmSdcHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdcStartHandler(cmData, cmData->dmVar.cacheTargetDev);
    CsrBtCmDmLocalQueueHandler();
}

static void csrBtCmDmWriteCachedPageModeCfmScHandler(cmInstanceData_t *cmData)
{
    if (cmData->dmVar.cancelConnect == TRUE)
    {
        switch(cmData->dmVar.dmMsgTypeInProgress)
        {
            case CSR_BT_CM_SM_ACL_OPEN_REQ:
                CsrBtCmDmAclOpenCfm(cmData,
                                    &cmData->dmVar.cacheTargetDev, HCI_ERROR_CONN_TERM_LOCAL_HOST);
                break;

            case CSR_BT_CM_SM_BONDING_REQ:
                CsrBtCmDmBondingCfm(cmData,
                                    &cmData->dmVar.cacheTargetDev, HCI_ERROR_CONN_TERM_LOCAL_HOST);
                break;

            default:
                CsrBtCmStateEventException(CSR_BT_CM_PRIM, cmData->dmVar.dmMsgTypeInProgress, cmData->globalState);
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                /* DM queue unlocked in outer scope */
                break;
        }
        cmData->dmVar.cancelConnect = FALSE;
    }
    else
    {
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.state, scConnect_s);
        switch(cmData->dmVar.dmMsgTypeInProgress)
        {
            case CSR_BT_CM_SM_ACL_OPEN_REQ:
            {
                TYPED_BD_ADDR_T ad;
                ad.addr = cmData->dmVar.cacheTargetDev;
                ad.type = CSR_BT_ADDR_PUBLIC;
                dm_acl_open_req(&ad, NULL);
                break;
            }

            case CSR_BT_CM_SM_BONDING_REQ:
            {
                TYPED_BD_ADDR_T ad;
                ad.addr = cmData->dmVar.cacheTargetDev;
                ad.type = CSR_BT_ADDR_PUBLIC;
                dm_sm_bonding_req(&ad, NULL);
                break;
            }

            default:
                CsrBtCmStateEventException(CSR_BT_CM_PRIM, cmData->dmVar.dmMsgTypeInProgress, cmData->globalState);
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                /* DM queue unlocked in outer scope */
                break;
        }
    }

    CsrBtCmDmLocalQueueHandler();
}

static void csrBtCmDmWriteCachedPageModeCfmCmHandler(cmInstanceData_t *cmData)
{
    if(cmData->dmVar.dmMsgTypeInProgress == CSR_BT_CM_READ_REMOTE_NAME_REQ)
    {
        if (cmData->dmVar.cancelRemoteName)
        {
            CsrBtCmReadRemoteNameCfmSend(cmData, cmData->dmVar.appHandle, cmData->dmVar.readNameDeviceAddr,
                                                        CSR_BT_RESULT_CODE_CM_CANCELLED, CSR_BT_SUPPLIER_CM);
            cmData->dmVar.readingName       = FALSE;
            cmData->dmVar.cancelRemoteName  = FALSE;
            cmData->dmVar.remoteNamePending = TRUE;  /* Make the cfm handler treat it correctly */
            CsrBtCmDmUpdateAndClearCachedParamDirect(cmData, cmData->dmVar.readNameDeviceAddr);
        }
        else
        {
            cmData->dmVar.readingName = TRUE;
            dm_hci_remote_name_request(&cmData->dmVar.cacheTargetDev, NULL);
        }
    }
    else
    {
        /* UNKNOWN ACTION */
        /* This never happens */
    }
}


/* --- Externally available functions --- */

void CsrBtCmDmWriteCachedPageModeCfmHandler(cmInstanceData_t *cmData)
{
    switch(cmData->smVar.activePlayer)
    {
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    case RFC_PLAYER:
        {
            csrBtCmDmWriteCachedPageModeCfmRfcHandler(cmData, cmData->rfcVar.connElement);
            break;
        }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
        case L2CAP_PLAYER:
        {
            csrBtCmDmWriteCachedPageModeCfmL2caHandler(cmData, cmData->l2caVar.connElement);
            break;
        }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
        case BNEP_PLAYER:
        {
            csrBtCmDmWriteCachedPageModeCfmBnepHandler(cmData);
            break;
        }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

        case SDC_PLAYER:
        {
            csrBtCmDmWriteCachedPageModeCfmSdcHandler(cmData);
            break;
        }

        case CM_PLAYER:
        {
            csrBtCmDmWriteCachedPageModeCfmCmHandler(cmData);
            break;
        }

        case SC_PLAYER:
        {
            csrBtCmDmWriteCachedPageModeCfmScHandler(cmData);
            break;
        }

    default:
        {
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
            CsrBtCmDmLocalQueueHandler();
            break;
        }
    }
}

CsrBool isCacheParamsKnown(cmInstanceData_t      *cmData,
                          CsrBtDeviceAddr          devAddr,
                          CsrUint16              *clockOffset,
                          page_scan_mode_t      *pageScanMode,
                          page_scan_rep_mode_t  *pageScanRepMode )
{
    dmCacheParamEntry * entryCacheTable;

    entryCacheTable = NULL;

    if( csrBtCmGetEntryInCacheParamTable(cmData, devAddr, &entryCacheTable) )
    {
        *clockOffset        = entryCacheTable->clockOffset;
        *pageScanMode       = entryCacheTable->pageScanMode;
        *pageScanRepMode    = entryCacheTable->pageScanRepMode;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void CsrBtCmDmWriteCacheParamsDirect(cmInstanceData_t         *cmData,
                                CsrBtDeviceAddr             devAddr,
                                CsrUint16                 clockOffset,
                                page_scan_mode_t         pageScanMode,
                                page_scan_rep_mode_t     pageScanRepMode,
                                CsrUint8                  activePlayer,
                                CsrBtCmPrim                dmMsgTypeInProgress)
{

    cmData->smVar.activePlayer = activePlayer;
    cmData->dmVar.dmMsgTypeInProgress = dmMsgTypeInProgress;

    /* Store the page-mode information */
    cmData->dmVar.pageScanMode    = pageScanMode;
    cmData->dmVar.pageScanRepMode = pageScanRepMode;
    cmData->dmVar.cacheTargetDev  = devAddr;

    /* start sending parameters directly to DM */
    /* send clock offset now and send page mode information on clock offset cfm */
    csrBtCmWriteCachedClockOffset(devAddr,
                             clockOffset);
}

void CsrBtCmDmWriteCacheParamsReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmDmWriteCacheParamsReq    *cmPrim;

    cmPrim                             = (CsrBtCmDmWriteCacheParamsReq *) cmData->recvMsgP;

    CsrBtCmDmWriteCacheParamsDirect(cmData,
                               cmPrim->devAddr,
                               cmPrim->clockOffset,
                               cmPrim->pageScanMode,
                               cmPrim->pageScanRepMode,
                               cmPrim->activePlayer,
                               0);
}

void CsrBtCmDmUpdateAndClearCachedParamReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmDmUpdateAndClearCachedParamReq  *cmPrim;
    cmPrim                                     = (CsrBtCmDmUpdateAndClearCachedParamReq *) cmData->recvMsgP;

    CsrBtCmDmUpdateAndClearCachedParamDirect(cmData, cmPrim->devAddr);
}



void CsrBtCmDmWriteCachedClockOffsetCfmHandler(cmInstanceData_t *cmData)
{
    /* The DM has confirmed that it has received a clock offset */
    /* There is no check to see if it was success or failure */
    /* because in case of failure performance will only degrade */
    /* a bit, hence proceed with the sequence  by sending the */
    /* page scan mode and page scan rep mode */

    csrBtCmWriteCachedPageMode(cmData->dmVar.cacheTargetDev,
                          cmData->dmVar.pageScanMode,
                          cmData->dmVar.pageScanRepMode);
}

void CsrBtCmDmStoreCacheParams(cmInstanceData_t     *cmData,
                           CsrBtDeviceAddr         devAddr,
                           CsrUint16             clockOffset,
                           page_scan_mode_t     pageScanMode,
                           page_scan_rep_mode_t pageScanRepMode)
{

    dmCacheParamEntry * newEntry = NULL;

    csrBtCmInsertEntryInCacheParamTable(cmData, &newEntry, &devAddr);

    if(newEntry != NULL)
    {
        csrBtCmCmDmCacheEntryAddSeqNr(cmData, newEntry);
        newEntry->deviceAddr        = devAddr;
        newEntry->clockOffset       = clockOffset;
        newEntry->pageScanMode      = pageScanMode;
        newEntry->pageScanRepMode   = pageScanRepMode;
    }
}


void CsrBtCmDmClearParamCacheCfmHandler(cmInstanceData_t *cmData)
{
    if (cmData->dmVar.remoteNamePending)
    {
        cmData->dmVar.cancelRemoteName = FALSE;
        cmData->dmVar.remoteNamePending = FALSE;
    }
        CsrBtCmDmLocalQueueHandler();
    }

void CsrBtCmDmHciReadClockOffsetCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_READ_CLOCK_OFFSET_CFM_T     *dmPrim;
    dmCacheParamEntry * newEntry            = NULL;

    dmPrim      = (DM_HCI_READ_CLOCK_OFFSET_CFM_T *) cmData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        csrBtCmInsertEntryInCacheParamTable(cmData, &newEntry, &(dmPrim->bd_addr));

        if(newEntry != NULL)
        {
            csrBtCmCmDmCacheEntryAddSeqNr(cmData, newEntry);
            newEntry->deviceAddr        = dmPrim->bd_addr;
            newEntry->clockOffset       = dmPrim->clock_offset;
        }
    }
    dm_clear_param_cache_req(&(dmPrim->bd_addr),NULL);
}


/* Loopback functions to lock the DM queue */

void CsrBtCmDmWriteCacheParamsReqSend(CsrUint8                activePlayer,
                             CsrBtDeviceAddr           devAddr,
                             CsrUint16               clockOffset,
                             page_scan_mode_t       pageScanMode,
                             page_scan_rep_mode_t   pageScanRepMode)
{
    CsrBtCmDmWriteCacheParamsReq    *prim;

    prim    = (CsrBtCmDmWriteCacheParamsReq *)CsrPmemAlloc(sizeof(CsrBtCmDmWriteCacheParamsReq));

    prim->type              = CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ;
    prim->activePlayer      = activePlayer;
    prim->devAddr           = devAddr;
    prim->clockOffset       = clockOffset;
    prim->pageScanMode      = pageScanMode;
    prim->pageScanRepMode   = pageScanRepMode;
    CsrBtCmPutMessage(CSR_BT_CM_IFACEQUEUE, prim);
}


void CsrBtCmDmUpdateAndClearCachedParamReqSend(CsrBtDeviceAddr  devAddr)
{
    CsrBtCmDmUpdateAndClearCachedParamReq    *prim;

    prim    = (CsrBtCmDmUpdateAndClearCachedParamReq *)CsrPmemAlloc(sizeof(CsrBtCmDmUpdateAndClearCachedParamReq));

    prim->type              = CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ;
    prim->devAddr           = devAddr;
    CsrBtCmPutMessage(CSR_BT_CM_IFACEQUEUE, prim);
}

void CsrBtCmDmUpdateAndClearCachedParamDirect(cmInstanceData_t *cmData, CsrBtDeviceAddr  devAddr)
{
    CsrBool             linkExists = FALSE;
    cmData->dmVar.cacheTargetDev  = devAddr;

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    if(CsrBtCmRfcFindRfcConnElementFromDeviceAddrState(cmData, &devAddr, connected_s))
    {
        linkExists = TRUE;
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    if (!linkExists)
    {
        if(CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromConnectedSDeviceAddr, &devAddr))
        {
            linkExists = TRUE;
        }
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    if (!linkExists)
    {
        CsrUintFast8 i;

        for( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
        { /* Search through the BNEP connection table */

            if(bd_addr_eq(&(devAddr), &(cmData->bnepVar.connectVar[i].deviceAddr)))
            {
                if( cmData->bnepVar.connectVar[i].state == bnepConnected_s)
                { /* Thie given address is right. */
                    linkExists = TRUE;
                }
            }
        }
    }
#endif

    if(linkExists == TRUE && cmData->dmVar.remoteNamePending == FALSE)
    {
        dm_hci_read_clock_offset(&devAddr,NULL);
    }
    else
    {
        dm_clear_param_cache_req(&devAddr,NULL);
    }
}

void CsrBtCmFlushCmCacheStopTimer(cmInstanceData_t *cmData)
{
    if (cmData->dmVar.cacheFlushTimerId != 0)
    {/* Cancel the timer if it is already running */
        CsrSchedTimerCancel(cmData->dmVar.cacheFlushTimerId, NULL, NULL);
        cmData->dmVar.cacheFlushTimerId = 0;
    }
}

void CsrBtCmFlushCmCacheStartTimer(cmInstanceData_t *cmData)
{
    /* Stop the timer if it is already running - should not be running at this point */
    CsrBtCmFlushCmCacheStopTimer(cmData);

    /* Start the timer */
    cmData->dmVar.cacheFlushTimerId = CsrSchedTimerSet(CM_DM_CACHE_FLUSH_TIMEOUT, (void (*) (CsrUint16, void *)) CsrBtCmFlushCmCacheTimeout, 0, (void *)  cmData);
}

void CsrBtCmFlushCmCacheTimeout(CsrUint8 dummy, cmInstanceData_t *cmData)
{
    CsrUintFast8         index;
    dmCacheParamPool    *currentPool;
    dmCacheParamPool    *nextPool;

    CSR_UNUSED(dummy);

    currentPool = cmData->dmVar.dmCacheParamTable;

    cmData->dmVar.cacheFlushTimerId = 0; /* Timer was fired */

    for (index = 0; index < CM_DM_MAX_NUM_OF_CACHE_POOLS && currentPool != NULL ; index++ )
    {
        nextPool = currentPool->nextCachePool;
        csrBtCmFlushCacheEntryPool(currentPool);
        currentPool = nextPool;
    }
}
