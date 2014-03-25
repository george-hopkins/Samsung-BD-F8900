/****************************************************************************


               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

/* Guard timer paramter */
typedef struct
{
    CsrBtAmpmInstance *inst;
    CsrBtAmpmAppSubscriberElem *sub;
    CsrBtConnId btConnId;
} csrBtAmpmAppTimerParam;

/* Move-ind timer fire */
static void csrBtAmpmAppTimerFire(CsrUint16 mi, void *mv)
{
    csrBtAmpmAppTimerParam *par = mv;

    par->sub->moveIndTimer = 0;

    /* Reject move, and tell app it failed */
    CsrBtCmMoveChannelResSend(par->sub->btConnId, FALSE);
    CsrBtAmpmUpstreamMoveDone(CSR_BT_AMPM_MOVE_CMP_IND,
                              par->sub->qid,
                              par->btConnId,
                              CSR_BT_AMP_CONTROLLER_UNKNOWN,
                              CSR_BT_AMPM_RESULT_MOVE_RSP_TIMEOUT,
                              CSR_BT_SUPPLIER_AMPM);

    CSR_UNUSED(mi);
}

/* Stop move-ind timer guard */
static void csrBtAmpmAppTimerStop(CsrBtAmpmInstance *inst,
                                  CsrBtAmpmAppSubscriberElem *sub)
{
    if(sub->moveIndTimer != 0)
    {
        void *mv = NULL;
        CsrSchedTimerCancel(sub->moveIndTimer,
                            NULL, &mv);
        sub->moveIndTimer = 0;
        CsrPmemFree(mv);
    }
}

/* Start move-ind timer guard */
static void csrBtAmpmAppTimerStart(CsrBtAmpmInstance *inst,
                                   CsrBtAmpmAppSubscriberElem *sub,
                                   CsrBtConnId btConnId)
{
    csrBtAmpmAppTimerParam *par;

    csrBtAmpmAppTimerStop(inst, sub);

    par = CsrPmemAlloc(sizeof(csrBtAmpmAppTimerParam));
    par->inst = inst;
    par->sub = sub;
    par->btConnId = btConnId;

    sub->moveIndTimer = CsrSchedTimerSet(CSR_BT_AMPM_APP_MOVE_GUARD,
                                        csrBtAmpmAppTimerFire,
                                        0, par);
}

/* Fixup result/supplier codes */
static void csrBtAmpmAppCmFixupResult(CsrBtResultCode *res,
                                      CsrBtSupplier *sup)
{
    if((*sup == CSR_BT_SUPPLIER_CM) &&
       (*res == CSR_BT_RESULT_CODE_CM_SUCCESS))
    {
        *res = CSR_BT_AMPM_RESULT_SUCCESS;
        *sup = CSR_BT_SUPPLIER_AMPM;
    }

    if((*sup == CSR_BT_SUPPLIER_L2CAP_MOVE) &&
       (*res == L2CA_MOVE_SUCCESS))
    {
        *res = CSR_BT_AMPM_RESULT_SUCCESS;
        *sup = CSR_BT_SUPPLIER_AMPM;
    }

}

/* Have we asked about other RFCOMM channels */
CsrBtAmpmAppSubscriberElem *csrBtAmpmAppGetActiveRfc(CsrBtAmpmInstance *inst,
                                                     CsrBtDeviceAddr *addr)
{
    CsrBtAmpmAppSubscriberElem *app;

    /* Scan subscribers */
    for(app = (CsrBtAmpmAppSubscriberElem*)CsrCmnListGetFirst(&inst->appList);
        app != NULL;
        app = (CsrBtAmpmAppSubscriberElem*)app->next)
    {
        if(CsrBtBdAddrEq(&app->addr, addr) &&
           CSR_BT_CONN_ID_IS_RFC(app->btConnId) &&
           (app->moveIndTimer != 0))
        {
            return app;
        }
    }
    return NULL;
}

/* Redirect move channel cfm */
static void csrBtAmpmAppCmMoveChanCfm(CsrBtAmpmInstance *inst,
                                      CsrBtCmMoveChannelCfm *prim)
{
    CsrBtAmpmAppSubscriberElem *sub;
    CsrBool rfcAuto;

    /* Ignore initiating result codes from L2CAP */
    if((prim->resultSupplier == CSR_BT_SUPPLIER_L2CAP_MOVE) &&
       ((prim->resultCode == L2CA_MOVE_PENDING) ||
        (prim->resultCode == L2CA_MOVE_INITIATING)))
    {
        return;
    }

    /* This is a multi-connection profile slave. Ignore results */
    if(CsrBtAmpmPrfSlaveAllow(inst, prim->btConnId))
    {
        return;
    }

    sub = CsrBtAmpmAppGetSubscriber(inst,prim->btConnId);

    /* Don't inform non-active RFCOMM connections */
    rfcAuto = FALSE;
    if(CSR_BT_CONN_ID_IS_RFC(prim->btConnId))
    {
        CsrBtAmpmAppSubscriberElem *act;
        act = csrBtAmpmAppGetActiveRfc(inst, &prim->deviceAddr);
        if(act && sub && (act != sub) &&
           CsrBtBdAddrEq(&act->addr, &sub->addr))
        {
            /* RFCOMM connection matches peer but this isn't our
             * channel. Perhaps app wants to know about auto-moves? */
            rfcAuto = TRUE;
        }        
    }

    /* This concludes an automatic move */
    if((sub != NULL) && 
       ((sub->autoMove) || rfcAuto))
    {
        csrBtAmpmAppTimerStop(inst, sub);
        csrBtAmpmAppCmFixupResult(&prim->resultCode,
                                  &prim->resultSupplier);

        if(sub->eventMask & CSR_BT_AMPM_EVENTS_AUTO_MOVE)
        {
            CsrBtAmpmUpstreamMoveDone(CSR_BT_AMPM_AUTO_MOVE_CMP_IND,
                                      sub->qid,
                                      prim->btConnId,
                                      prim->localControl,
                                      prim->resultCode,
                                      prim->resultSupplier);
        }
        sub->state = CSR_BT_AMPM_APP_STATE_IDLE;
        sub->localId = prim->localControl;
        sub->autoMove = FALSE;

        /* Trigger move of multi-connection slave channels */
        CsrBtAmpmPrfMasterTrigger(inst, prim->btConnId);

        /* If the move was for a RFCOMM channel for the right peer,
         * but not for this channel, this is where we bail */
        return;
    }

    /* Test subscribers can 'hijack' conns with the invalid conn id */
    if(sub == NULL)
    {
        sub = CsrBtAmpmAppGetTestSubscriber(inst);
    }

    if(sub != NULL)
    {
        csrBtAmpmAppCmFixupResult(&prim->resultCode,
                                  &prim->resultSupplier);
        CsrBtAmpmUpstreamMoveDone(CSR_BT_AMPM_MOVE_CFM,
                                  sub->qid,
                                  prim->btConnId,
                                  prim->localControl,
                                  prim->resultCode,
                                  prim->resultSupplier);
        sub->state = CSR_BT_AMPM_APP_STATE_IDLE;
        sub->localId = prim->localControl;

        /* Trigger move of multi-connection slave channels */
        CsrBtAmpmPrfMasterTrigger(inst, prim->btConnId);
    }
    else
    {
        /* Can't do anything about this */
        ;
    }
}

/* Redirect move channel ind */
static void csrBtAmpmAppCmMoveChanInd(CsrBtAmpmInstance *inst,
                                      CsrBtCmMoveChannelInd *prim)
{
    CsrBtAmpmAppSubscriberElem *sub;
    sub = CsrBtAmpmAppGetSubscriber(inst,prim->btConnId);

    /* This is a multi-connection profile slave. Allow the move to
     * proceed, and stop any further processing */
    if(CsrBtAmpmPrfSlaveAllow(inst, prim->btConnId))
    {
        CsrBtCmMoveChannelResSend(prim->btConnId, TRUE);
        return;
    }

    /* If channel is RFCOMM-based, ensure that we only ask _one_
     * application */
    if(CSR_BT_CONN_ID_IS_RFC(prim->btConnId))
    {
        if(csrBtAmpmAppGetActiveRfc(inst, &prim->deviceAddr) != NULL)
        {
            /* One app has already been asked, bail out */
            return;
        }
    }

    /* If application has enabled auto-accept, do so */
    if((sub != NULL) &&
       (sub->flags & CSR_BT_AMPM_FLAGS_AUTO_MOVE_ACCEPT))
    {
        sub->autoMove = TRUE;
        sub->state = CSR_BT_AMPM_APP_STATE_MOVING_RESP;
        CsrBtCmMoveChannelResSend(prim->btConnId, TRUE);
        return;
    }

    /* Test subscribers can 'hijack' conns with the invalid conn id */
    if(sub == NULL)
    {
        sub = CsrBtAmpmAppGetTestSubscriber(inst);
    }

    if(sub != NULL)
    {
        CsrBtAmpmUpstreamMoveInd(sub->qid,
                                 prim->btConnId,
                                 prim->localControl);
        csrBtAmpmAppTimerStart(inst, sub, prim->btConnId);
        sub->state = CSR_BT_AMPM_APP_STATE_MOVING_RESP;
    }
    else
    {
        /* No subscriber - reject*/
        CsrBtCmMoveChannelResSend(prim->btConnId, FALSE);
    }
}

/* Redirect move channel cmp ind */
static void csrBtAmpmAppCmMoveChanCmpInd(CsrBtAmpmInstance *inst,
                                         CsrBtCmMoveChannelCmpInd *prim)
{
    CsrBtAmpmAppSubscriberElem *sub;
    CsrBool rfcAuto;

    /* Ignore initiating result codes from L2CAP */
    if((prim->resultSupplier == CSR_BT_SUPPLIER_L2CAP_MOVE) &&
       ((prim->resultCode == L2CA_MOVE_PENDING) ||
        (prim->resultCode == L2CA_MOVE_INITIATING)))
    {
        return;
    }

    /* This is a multi-connection profile slave. Ignore results */
    if(CsrBtAmpmPrfSlaveAllow(inst, prim->btConnId))
    {
        return;
    }

    sub = CsrBtAmpmAppGetSubscriber(inst,prim->btConnId);

    /* Don't inform non-active RFCOMM connections */
    rfcAuto = FALSE;
    if(CSR_BT_CONN_ID_IS_RFC(prim->btConnId))
    {
        CsrBtAmpmAppSubscriberElem *act;
        act = csrBtAmpmAppGetActiveRfc(inst, &prim->deviceAddr);
        if(act && sub && (act != sub) &&
           CsrBtBdAddrEq(&act->addr, &sub->addr))
        {
            /* RFCOMM connection matches peer but this isn't our
             * channel. Perhaps app wants to know about auto-moves? */
            rfcAuto = TRUE;
        }        
    }

    /* This concludes an automatic move */
    if((sub != NULL) &&
       ((sub->autoMove) || rfcAuto))
    {
        csrBtAmpmAppTimerStop(inst, sub);
        csrBtAmpmAppCmFixupResult(&prim->resultCode,
                                  &prim->resultSupplier);

        if(sub->eventMask & CSR_BT_AMPM_EVENTS_AUTO_MOVE)
        {
            CsrBtAmpmUpstreamMoveDone(CSR_BT_AMPM_AUTO_MOVE_CMP_IND,
                                      sub->qid,
                                      prim->btConnId,
                                      prim->localControl,
                                      prim->resultCode,
                                      prim->resultSupplier);
        }
        sub->state = CSR_BT_AMPM_APP_STATE_IDLE;
        sub->localId = prim->localControl;
        sub->autoMove = FALSE;

        /* Trigger move of multi-connection slave channels */
        CsrBtAmpmPrfMasterTrigger(inst, prim->btConnId);

        /* If the move was for a RFCOMM channel for the right peer,
         * but not for this channel, this is where we bail */
        return;
    }

    /* Test subscribers can 'hijack' conns with the invalid conn id */
    if(sub == NULL)
    {
        sub = CsrBtAmpmAppGetTestSubscriber(inst);
    }

    if((sub != NULL) &&
       (sub->state == CSR_BT_AMPM_APP_STATE_MOVING_RESP_CMP))
    {
        csrBtAmpmAppTimerStop(inst, sub);
        csrBtAmpmAppCmFixupResult(&prim->resultCode,
                                  &prim->resultSupplier);
        CsrBtAmpmUpstreamMoveDone(CSR_BT_AMPM_MOVE_CMP_IND,
                                  sub->qid,
                                  prim->btConnId,
                                  prim->localControl,
                                  prim->resultCode,
                                  prim->resultSupplier);
        sub->state = CSR_BT_AMPM_APP_STATE_IDLE;
        sub->localId = prim->localControl;

        /* Trigger move of multi-connection slave channels */
        CsrBtAmpmPrfMasterTrigger(inst, prim->btConnId);
    }
    else
    {
        /* Can't do anything about this */
        ;
    }
}

/* Automatic move request */
void CsrBtAmpmAppCmAutoMoveChan(CsrBtAmpmInstance *inst,
                                CsrBtAmpmAppSubscriberElem *sub,
                                CsrBtAmpController localId,
                                CsrBtAmpController remoteId)

{
    if((sub->state == CSR_BT_AMPM_APP_STATE_IDLE) &&
       (remoteId != CSR_BT_AMP_CONTROLLER_UNKNOWN) &&
       (localId !=  CSR_BT_AMP_CONTROLLER_UNKNOWN))
    {
        CsrBtCmMoveChannelReqSend(sub->btConnId,
                                      remoteId,
                                      localId);
        sub->state = CSR_BT_AMPM_APP_STATE_MOVING_INIT;
        sub->autoMove = TRUE;
    }
}

/* Redirect move channel req */
void CsrBtAmpmAppCmMoveChanReq(CsrBtAmpmInstance *inst,
                               CsrBtAmpmMoveReq *prim)
{
    CsrBtAmpmAppSubscriberElem *sub;

    sub = CsrBtAmpmAppGetSubscriber(inst, prim->btConnId);

    /* Test subscribers can 'hijack' conns with the invalid conn id */
    if(sub == NULL)
    {
        sub = CsrBtAmpmAppGetTestSubscriber(inst);
    }

    if(sub != NULL)
    {
        if(prim->btConnId == CSR_BT_CONN_ID_INVALID)
        {
            /* Move already in progress, bail out */
            CsrBtAmpmUpstreamMoveDone(CSR_BT_AMPM_MOVE_CFM,
                                      sub->qid,
                                      prim->btConnId,
                                      CSR_BT_AMP_CONTROLLER_UNKNOWN,
                                      CSR_BT_AMPM_RESULT_INVALID_PARAM,
                                      CSR_BT_SUPPLIER_AMPM);
            return;
        }

        if(sub->state != CSR_BT_AMPM_APP_STATE_IDLE)
        {
            /* Move already in progress, bail out */
            CsrBtAmpmUpstreamMoveDone(CSR_BT_AMPM_MOVE_CFM,
                                      sub->qid,
                                      prim->btConnId,
                                      CSR_BT_AMP_CONTROLLER_UNKNOWN,
                                      CSR_BT_AMPM_RESULT_WRONG_STATE,
                                      CSR_BT_SUPPLIER_AMPM);
            return;
        }

        CsrBtCmMoveChannelReqSend(prim->btConnId,
                                      prim->remoteId,
                                      prim->localId);
        sub->state = CSR_BT_AMPM_APP_STATE_MOVING_INIT;
    }
    else
    {
        /* Can't do anything about this */
        ;
    }
}

/* Redirect move channel res */
void CsrBtAmpmAppCmMoveChanRes(CsrBtAmpmInstance *inst,
                               CsrBtAmpmMoveRes *prim)
{
    CsrBtAmpmAppSubscriberElem *sub;
    sub = CsrBtAmpmAppGetSubscriber(inst, prim->btConnId);

    /* Test subscribers can 'hijack' conns with the invalid conn id */
    if(sub == NULL)
    {
        sub = CsrBtAmpmAppGetTestSubscriber(inst);
    }

    if(sub != NULL)
    {
        if(prim->btConnId == CSR_BT_CONN_ID_INVALID)
        {
            CsrBtAmpmUpstreamMoveDone(CSR_BT_AMPM_MOVE_CMP_IND,
                                      sub->qid,
                                      prim->btConnId,
                                      CSR_BT_AMP_CONTROLLER_UNKNOWN,
                                      CSR_BT_AMPM_RESULT_INVALID_PARAM,
                                      CSR_BT_SUPPLIER_AMPM);
            return;
        }

        if(sub->state != CSR_BT_AMPM_APP_STATE_MOVING_RESP)
        {
            CsrBtAmpmUpstreamMoveDone(CSR_BT_AMPM_MOVE_CMP_IND,
                                      sub->qid,
                                      prim->btConnId,
                                      CSR_BT_AMP_CONTROLLER_UNKNOWN,
                                      CSR_BT_AMPM_RESULT_WRONG_STATE,
                                      CSR_BT_SUPPLIER_AMPM);
            return;
        }

        csrBtAmpmAppTimerStop(inst, sub);
        CsrBtCmMoveChannelResSend(prim->btConnId,
                                      prim->accept);
        sub->state = CSR_BT_AMPM_APP_STATE_MOVING_RESP_CMP;
    }
    else
    {
        /* Can't do anything about this */
        ;
    }
}

/* Main handler for CM primitives */
void CsrBtAmpmAppCmHandler(CsrBtAmpmInstance *inst)
{
    switch(*(CsrUint16*)inst->msg)
    {
        case CSR_BT_CM_MOVE_CHANNEL_CFM:
            csrBtAmpmAppCmMoveChanCfm(inst, (CsrBtCmMoveChannelCfm*)inst->msg);
            break;

        case CSR_BT_CM_MOVE_CHANNEL_IND:
            csrBtAmpmAppCmMoveChanInd(inst, (CsrBtCmMoveChannelInd*)inst->msg);
            break;

        case CSR_BT_CM_MOVE_CHANNEL_CMP_IND:
            csrBtAmpmAppCmMoveChanCmpInd(inst, (CsrBtCmMoveChannelCmpInd*)inst->msg);
            break;
    }
}
