/****************************************************************************


               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

/* Prf list element initialiser */
static void csrBtAmpmPrfElemInit(CsrCmnListElm_t *elem)
{
    CsrBtAmpmPrfElem *new = (CsrBtAmpmPrfElem*)elem;
    CsrMemSet(&(new->master),
              0,
              sizeof(CsrBtAmpmPrfElem) - sizeof(CsrCmnListElm_t));
}

/* Prf list element freer */
static void csrBtAmpmPrfElemDeinit(CsrCmnListElm_t *elem)
{
    CsrBtAmpmPrfElem *die = (CsrBtAmpmPrfElem*)elem;
    CsrPmemFree(die->slaves);
}

/* Search multiConnList by slave ID */
static CsrBool csrBtAmpmPrfElemSearchSlave(CsrCmnListElm_t *entry,
                                           void *sp)
{
    CsrBtAmpmPrfElem *elm = (CsrBtAmpmPrfElem*)entry;
    CsrBtConnId *slave = (CsrBtConnId*)sp;
    CsrUint8 i;

    for(i=0; i<elm->numSlaves; i++)
    {
        if(elm->slaves[i] == *slave)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/* Search multiConnList by master ID */
static CsrBool csrBtAmpmPrfElemSearchMaster(CsrCmnListElm_t *entry,
                                            void *sp)
{
    CsrBtAmpmPrfElem *elm = (CsrBtAmpmPrfElem*)entry;
    CsrBtConnId *master = (CsrBtConnId*)sp;

    if(elm->master == *master)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/* Profile hints about multi-channel usage */
void CsrBtAmpmPrfMultiConnReqHandler(CsrBtAmpmInstance *inst,
                                     CsrBtAmpmPrfMultiConnReq *prim)
{
    CsrBtAmpmPrfElem *prf;

    prf = (CsrBtAmpmPrfElem*)CsrCmnListSearch(&inst->multiConnList,
                                              csrBtAmpmPrfElemSearchMaster,
                                              &(prim->master));

    /* Delete PRF slave list and exit. Do not care about things that
     * have gone */
    if(prim->slavesCount == 0)
    {
        if(prf)
        {
            CsrCmnListElementRemove(&inst->multiConnList,
                                    (CsrCmnListElm_t*)prf);
        }
        return;
    }

    /* Create new entry */
    if(!prf)
    {
        prf = (CsrBtAmpmPrfElem*)CsrCmnListElementAddLast(&inst->multiConnList,
                                                          sizeof(CsrBtAmpmPrfElem));
    }

    /* Update existing entry by consuming primitive members */
    CsrPmemFree(prf->slaves);
    prf->numSlaves = prim->slavesCount;
    prf->slaves = prim->slaves;
    prim->slaves = NULL;
    prim->slavesCount = 0;

    /* Always attempt to move slaves whenever there is an
     * update. L2CAP will ensure that moves to same controller is
     * rejected locally, and we do not care about result codes in the
     * case of slaves */
    CsrBtAmpmPrfMasterTrigger(inst, prf->master);
}

/* Does master btConnId trigger move of slaves? */
void CsrBtAmpmPrfMasterTrigger(CsrBtAmpmInstance *inst,
                               CsrBtConnId master)
{
    CsrBtAmpmPrfElem *prf;
    CsrBtAmpmAppSubscriberElem *sub;

    sub = CsrBtAmpmAppGetSubscriber(inst, master);
    prf = (CsrBtAmpmPrfElem*)CsrCmnListSearch(&inst->multiConnList,
                                              csrBtAmpmPrfElemSearchMaster,
                                              &master);
    if(prf && sub)
    {
        CsrUint8 i;
        CsrBtAmpController remoteId;

        if(sub->localId == CSR_BT_AMP_CONTROLLER_BREDR)
        {
            /* The BR/EDR controller is known */
            remoteId = CSR_BT_AMP_CONTROLLER_BREDR;
        }
        else
        {
            /* Scan for remoteId */
            remoteId = CsrBtAmpmBtdevFindMatchingRemoteController(inst,
                                                                  &sub->addr,
                                                                  sub->localId);
        }

        /* In case we could not find the remote ID, bail out */
        if(remoteId == CSR_BT_AMP_CONTROLLER_UNKNOWN)
        {
            return;
        }

        /* Send the move request for all slaves */
        for(i=0; i<prf->numSlaves; i++)
        {
            CsrBtCmMoveChannelReqSend((CsrBtConnId)prf->slaves[i],
                                          remoteId,
                                          sub->localId);
        }
    }
}

/* Does slave exist in PRF table? */
CsrBool CsrBtAmpmPrfSlaveAllow(CsrBtAmpmInstance *inst,
                               CsrBtConnId slave)
{
    CsrBtAmpmPrfElem *prf;

    /* If PRF entry found, this is a multi-connection profile slave */
    prf = (CsrBtAmpmPrfElem*)CsrCmnListSearch(&inst->multiConnList,
                                              csrBtAmpmPrfElemSearchSlave,
                                              &slave);
    return (CsrBool)((prf != NULL)
                     ? TRUE
                     : FALSE);
}

/* Subsystem init function */
void CsrBtAmpmPrfInit(CsrBtAmpmInstance *inst)
{
    CsrCmnListInit(&inst->multiConnList, 0,
                   csrBtAmpmPrfElemInit,
                   csrBtAmpmPrfElemDeinit);
}

/* Subsystem shutdown function */
void CsrBtAmpmPrfDeinit(CsrBtAmpmInstance *inst)
{
    CsrCmnListDeinit(&inst->multiConnList);
}
