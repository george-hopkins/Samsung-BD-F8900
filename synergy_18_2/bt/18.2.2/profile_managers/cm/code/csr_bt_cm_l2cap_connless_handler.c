/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
#ifdef CSR_BT_INSTALL_L2CAP_CONNLESS_SUPPORT

#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_util.h"

/* Search input struct */
typedef struct
{
    CsrBtDeviceAddr *addr;
    psm_t psm;
} cmL2caClSearch;

/* Send upstream data confirm */
static void csrBtCmL2caConnlessDataCfm(cmInstanceData_t *cmData,
                                       cmL2caConnlessElement *connless,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier resultSupplier)
{
    CsrBtCmL2caConnectionlessDataCfm *prim;
    cmL2caConnElement *l2con;

    l2con = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromPsm,
                                 &(connless->psm));
    if(l2con && l2con->cmL2caConnInst)
    {
        prim = (CsrBtCmL2caConnectionlessDataCfm*)CsrPmemAlloc(sizeof(CsrBtCmL2caConnectionlessDataCfm));
        prim->type = CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM;
        prim->connectionlessPsm = connless->psm;
        prim->deviceAddr = connless->deviceAddr;
        prim->resultCode = resultCode;
        prim->resultSupplier =  resultSupplier;

        CsrBtCmPutMessage(l2con->cmL2caConnInst->appHandle, prim);
    }
}

/* Send upstream data indication */
static void csrBtCmL2caConnlessDataInd(cmInstanceData_t *cmData,
                                       cmL2caConnlessElement *connless,
                                       CsrUint16 length,
                                       CsrUint8 *payload)
{
    CsrBtCmL2caConnectionlessDataInd *prim;
    cmL2caConnElement *l2con;

    l2con = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromPsm,
                                 &(connless->psm));
    if(l2con && l2con->cmL2caConnInst)
    {
        prim = (CsrBtCmL2caConnectionlessDataInd*)CsrPmemAlloc(sizeof(CsrBtCmL2caConnectionlessDataInd));
        prim->type = CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND;
        prim->connectionlessPsm = connless->psm;
        prim->length = length;
        prim->payload = payload;
        prim->deviceAddr = connless->deviceAddr;

        CsrBtCmPutMessage(l2con->cmL2caConnInst->appHandle, prim);
    }
    else
    {
        /* Not good. Can't send data */
        CsrPmemFree(payload);
        CsrBtCmGeneralWarning(L2CAP_PRIM, CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND,
                              "Connless data indication can not find PSM target");
    }
}

/* Find connless element given address */
static CsrBool csrBtCmL2caConnlessListFindAddrPsm(CsrCmnListElm_t *cle, void *value)
{
    cmL2caConnlessElement *elm = (cmL2caConnlessElement*)cle;
    cmL2caClSearch *search = (cmL2caClSearch*)value;

    if((search->psm == elm->psm) &&
       bd_addr_eq(search->addr, &elm->deviceAddr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/* Find connless element given btConnId */
static CsrBool csrBtCmL2caConnlessListFindCid(CsrCmnListElm_t *cle, void *value)
{
    cmL2caConnlessElement *elm = (cmL2caConnlessElement*)cle;
    CsrBtConnId btConnId = *(CsrBtConnId*)value;

    if(btConnId == elm->btConnId)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Helper function to find connless element given address and psm */
static cmL2caConnlessElement *csrBtCmL2caConnlessGetByAddrPsm(cmInstanceData_t *cmData,
                                                     CsrBtDeviceAddr *addr,
                                                     psm_t psm)
{
    cmL2caClSearch search;

    search.addr = addr;
    search.psm = psm;
    return  (cmL2caConnlessElement*)CsrCmnListSearch((CsrCmnList_t*)&cmData->l2caVar.connlessList,
                                                     csrBtCmL2caConnlessListFindAddrPsm,
                                                     &search);
}

/* Unamp timer */
static void csrBtL2caConnlessUnmapTimer(CsrUint16 btConnId, void *ip)
{
    cmL2caConnlessElement *connless;
    cmInstanceData_t *cmData = (cmInstanceData_t*)ip;

    connless = (cmL2caConnlessElement*)CsrCmnListSearch((CsrCmnList_t*)&cmData->l2caVar.connlessList,
                                                        csrBtCmL2caConnlessListFindCid,
                                                        &btConnId);
    if(connless)
    {
        connless->unmapTimer = 0;
        L2CA_UnmapFixedCidReq(CM_GET_UINT16ID_FROM_BTCONN_ID(connless->btConnId));
    }
}

/* List remove function */
void csrBtCmL2caConnlessListRemove(CsrCmnListElm_t *cle)
{
    cmL2caConnlessElement *elm = (cmL2caConnlessElement*)cle;
    CsrUint16 msg_type;
    void *msg_data;

    /* Simply pop the tx queue. We only ever store downstream
     * connectionless data elements on this queue! */
    while(CsrMessageQueuePop(&elm->queue, &msg_type, &msg_data))
    {
        CsrBtCmFreeDownstreamMessageContents(msg_type, msg_data);
    }

    if(elm->unmapTimer != 0)
    {
        CsrSchedTimerCancel(elm->unmapTimer, NULL, NULL);
        elm->unmapTimer = 0;
    }
}

/* Create connless list element */
static cmL2caConnlessElement *csrBtCmL2caConnlessAddElement(cmInstanceData_t *cmData,
                                                            CsrBtDeviceAddr *addr,
                                                            psm_t psm)
{
    cmL2caConnlessElement *elm;
    elm = (cmL2caConnlessElement*)CsrCmnListElementAddLast(&(cmData->l2caVar.connlessList),
                                                           sizeof(cmL2caConnlessElement));
    elm->deviceAddr = *addr;
    elm->psm = psm;
    elm->queue = NULL;
    elm->btConnId = L2CA_CID_INVALID;
    return elm;
}

static void csrBtCmL2caConnlessSendQueue(cmInstanceData_t *cmData,
                                          cmL2caConnlessElement *connless)
{
    CsrUint16 msg_type;
    void *msg_data;

    while(CsrMessageQueuePop(&connless->queue, &msg_type, &msg_data))
    {
        CsrBtCmL2caConnectionlessDataReq *prim = (CsrBtCmL2caConnectionlessDataReq*)msg_data;
        if(connless->btConnId != L2CA_CID_INVALID)
        {
            L2CA_DataWriteReq(CM_GET_UINT16ID_FROM_BTCONN_ID(connless->btConnId),
                              prim->length,
                              prim->payload);
            prim->length = 0;
            prim->payload = NULL;
        }
        else
        {
            CsrBtCmGeneralWarning(L2CAP_PRIM, prim->type,
                                  "Send connless queue to invalid CID");
        }
        CsrBtCmFreeDownstreamMessageContents(msg_type, msg_data);
    }
}

static void csrBtCmL2caConnlessKill(cmInstanceData_t *cmData,
                                    cmL2caConnlessElement *connless,
                                    CsrBtResultCode result,
                                    CsrBtSupplier supplier)
{
    CsrUint16 msg_type;
    void *msg_data;

    while(CsrMessageQueuePop(&connless->queue, &msg_type, &msg_data))
    {
        CsrBtCmFreeDownstreamMessageContents(msg_type, msg_data);
        csrBtCmL2caConnlessDataCfm(cmData, connless,
                                   result, supplier);
    }
}

/* Upper-layer request to send connless data */
void CsrBtCmL2caConnlessDataReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmL2caConnectionlessDataReq *prim = (CsrBtCmL2caConnectionlessDataReq*)cmData->recvMsgP;
    cmL2caConnlessElement *connless;
    CsrBool save = FALSE;
    l2ca_conflags_t flags;

    if((prim->deviceAddr.nap == 0) &&
       (prim->deviceAddr.uap == 0) &&
       (prim->deviceAddr.lap == 0))
    {
        flags = L2CA_CONFLAG_BROADCAST;
    }
    else
    {
        /* For UCD, locking the ACL is required for bringing it up */
        flags = L2CA_CONFLAG_LOCK_ACL;
    }

    connless = csrBtCmL2caConnlessGetByAddrPsm(cmData, &prim->deviceAddr, prim->connectionlessPsm);
    if(connless)
    {
        /* Stop unmap timer to allow L2CAP a chance to send the data */
        if(connless->unmapTimer != 0)
        {
            CsrSchedTimerCancel(connless->unmapTimer, NULL, NULL);
            connless->unmapTimer = 0;
        }

        if(connless->btConnId != L2CA_CID_INVALID)
        {
            /* Tx to L2CAP directly and erase evidence from original
             * request */
            csrBtCmL2caConnlessSendQueue(cmData, connless);
            L2CA_DataWriteReq(CM_GET_UINT16ID_FROM_BTCONN_ID(connless->btConnId),
                              prim->length,
                              prim->payload);
            prim->length = 0;
            prim->payload = NULL;
        }
        else
        {
            save = TRUE;
        }
    }
    else
    {
        connless = csrBtCmL2caConnlessAddElement(cmData,
                                                 &prim->deviceAddr,
                                                 prim->connectionlessPsm);
        L2CA_MapConnectionlessCidReq(&prim->deviceAddr,
                                     prim->connectionlessPsm, /* local */
                                     prim->connectionlessPsm, /* remote */
                                     0, /* con_ctx */
                                     flags);
        save = TRUE;
    }

    if(save && connless)
    {
        CsrMessageQueuePush(&(connless->queue), CSR_BT_CM_PRIM, prim);
        cmData->recvMsgP = NULL;
    }
}

/* L2CAP upstream: Fixed channel has been mapped */
void CsrBtCmL2caMapFixedCidIndHandler(cmInstanceData_t *cmData)
{
    L2CA_MAP_FIXED_CID_IND_T *prim = (L2CA_MAP_FIXED_CID_IND_T*)cmData->recvMsgP;
    cmL2caConnlessElement *connless;

    /* Blindly accept the fixed channel, but don't lock the ACL */
    L2CA_MapConnectionlessCidRsp(prim->cid,
                                 0, /* con_ctx */
                                 prim->cl_local_psm, /* ucd_remote_psm */
                                 0); /* flags, don't lock */

    /* Does map exist, or must we create a new one? */
    connless = csrBtCmL2caConnlessGetByAddrPsm(cmData,
                                               &prim->addrt.addr,
                                               prim->cl_local_psm);
    if(!connless)
    {
        connless = csrBtCmL2caConnlessAddElement(cmData,
                                                 &prim->addrt.addr,
                                                 prim->cl_local_psm);
    }

    connless->btConnId = CM_CREATE_L2CA_CONN_ID(prim->cid);

    /* If no data is pending, make sure we eventually unmap this channel */
    if((connless->queue == NULL) &&
       (connless->unmapTimer == 0))
    {
        connless->unmapTimer = CsrSchedTimerSet(CSR_BT_CM_CONNECTIONLESS_UNMAP_DELAY,
                                               csrBtL2caConnlessUnmapTimer,
                                               CM_GET_UINT16ID_FROM_BTCONN_ID(connless->btConnId),
                                               cmData);
    }

    csrBtCmL2caConnlessSendQueue(cmData, connless);
}

/* L2CAP upstream: Fixed channel map complete */
void CsrBtCmL2caMapFixedCidCfmHandler(cmInstanceData_t *cmData)
{
    L2CA_MAP_FIXED_CID_CFM_T *ind = (L2CA_MAP_FIXED_CID_CFM_T*)cmData->recvMsgP;
    cmL2caConnlessElement *connless;
    CsrBtConnId btConnId = CM_CREATE_L2CA_CONN_ID(ind->cid);
    connless = csrBtCmL2caConnlessGetByAddrPsm(cmData,
                                               &ind->addrt.addr,
                                               ind->cl_local_psm);
    if(!connless)
    {
        connless = (cmL2caConnlessElement*)CsrCmnListSearch((CsrCmnList_t*)&cmData->l2caVar.connlessList,
                                                            csrBtCmL2caConnlessListFindCid,
                                                            &btConnId);
    }

    if((ind->fixed_cid == L2CA_CID_CONNECTIONLESS) &&
       (connless != NULL))
    {
        if(ind->result == L2CA_MISC_INITIATING)
        {
            /* ACL being set up, ignore */
            connless->btConnId = btConnId;
        }
        else if(ind->result == L2CA_MISC_SUCCESS)
        {
            /* Ready */
            connless->btConnId = btConnId;
            csrBtCmL2caConnlessSendQueue(cmData, connless);
        }
        else
        {
            /* Error */
            csrBtCmL2caConnlessKill(cmData, connless,
                                    (CsrBtResultCode)ind->result,
                                    CSR_BT_SUPPLIER_L2CAP_MISC);
        }
    }
    else
    {
        /* Unmatching primitive */
        CsrBtCmGeneralWarning(L2CAP_PRIM, ind->type,
                              "Invalid connectionless fixed channel map indication");
    }
}

/* L2CAP upstream: Fixed channel unmapped */
void CsrBtCmL2caUnmapFixedCidIndHandler(cmInstanceData_t *cmData)
{
    L2CA_UNMAP_FIXED_CID_IND_T *prim;
    cmL2caConnlessElement *connless;
    CsrBtConnId btConnId;

    prim = (L2CA_UNMAP_FIXED_CID_IND_T*)cmData->recvMsgP;
    btConnId = CM_CREATE_L2CA_CONN_ID(prim->cid);
    connless = (cmL2caConnlessElement*)CsrCmnListSearch((CsrCmnList_t*)&cmData->l2caVar.connlessList,
                                                        csrBtCmL2caConnlessListFindCid,
                                                        &btConnId);
    if(connless)
    {
        csrBtCmL2caConnlessKill(cmData, connless,
                                CSR_BT_RESULT_CODE_CM_UNSPECIFIED_ERROR,
                                CSR_BT_SUPPLIER_CM);
    }
}

/* L2CAP upstream: Datawrite confirm */
CsrBool CsrBtCmL2caConnlessDatawriteCfmHandler(cmInstanceData_t *cmData)
{
    L2CA_DATAWRITE_CFM_T *prim;
    cmL2caConnlessElement *connless;
    CsrBtConnId btConnId;

    prim = (L2CA_DATAWRITE_CFM_T*)cmData->recvMsgP;
    btConnId = CM_CREATE_L2CA_CONN_ID(prim->cid);
    connless = (cmL2caConnlessElement*)CsrCmnListSearch((CsrCmnList_t*)&cmData->l2caVar.connlessList,
                                                        csrBtCmL2caConnlessListFindCid,
                                                        &btConnId);
    if(connless)
    {
        csrBtCmL2caConnlessDataCfm(cmData, connless,
                                   CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);

        /* Start kill timer if queue is empty */
        if((connless->queue == NULL) &&
           (connless->unmapTimer == 0))
        {
            connless->unmapTimer = CsrSchedTimerSet(CSR_BT_CM_CONNECTIONLESS_UNMAP_DELAY,
                                                   csrBtL2caConnlessUnmapTimer,
                                                   CM_GET_UINT16ID_FROM_BTCONN_ID(connless->btConnId),
                                                   cmData);
        }

        /* Pump more data through */
        csrBtCmL2caConnlessSendQueue(cmData, connless);
        return TRUE;
    }
    else
    {
        /* Not connectionless data - unhandled */
        return FALSE;
    }
}

/* L2CAP upstream: Dataread indication */
CsrBool CsrBtCmL2caConnlessDatareadIndHandler(cmInstanceData_t *cmData)
{
    L2CA_DATAREAD_IND_T *prim = (L2CA_DATAREAD_IND_T*)cmData->recvMsgP;
    cmL2caConnlessElement *connless;
    CsrBtConnId btConnId = CM_CREATE_L2CA_CONN_ID(prim->cid);

    connless = (cmL2caConnlessElement*)CsrCmnListSearch((CsrCmnList_t*)&cmData->l2caVar.connlessList,
                                                        csrBtCmL2caConnlessListFindCid,
                                                        &btConnId);
    if(connless)
    {
        if((prim->length == 0) &&
           (prim->data != NULL))
        {
            CsrUint8 *ptr;
            CsrUint16 length;

            length = CsrMblkGetLength((CsrMblk *) prim->data);
            ptr = CsrBtMblkConsumeToMemory((CsrMblk **) &prim->data);
            csrBtCmL2caConnlessDataInd(cmData,
                                       connless,
                                       length,
                                       ptr);
            return TRUE;
        }
    }

    /* Something went wrong */
    return FALSE;
}

/* Initialise connless instance */
void CsrBtCmL2caConnlessInit(cmInstanceData_t *cmData)
{
    CsrCmnListInit(&(cmData->l2caVar.connlessList), 0, NULL, csrBtCmL2caConnlessListRemove);
}

#endif
#endif
