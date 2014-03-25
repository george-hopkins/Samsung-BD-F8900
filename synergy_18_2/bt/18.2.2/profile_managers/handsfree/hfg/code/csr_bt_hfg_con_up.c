/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_proto.h"
#ifdef CSR_USE_DSPM
#include "csr_dspm_lib.h"
#endif
/* Cancelled connection */
void CsrBtHfgConnectCmCancelAcceptConnectCfmHandler(HfgInstance_t *inst)
{
    CsrBtCmCancelAcceptConnectCfm *prim;

    prim = (CsrBtCmCancelAcceptConnectCfm*)inst->msg;
    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if(inst->pendingDisconnect)
        {
            CsrBtHfgSendHfgDisconnectInd(inst, TRUE, prim->resultCode, prim->resultSupplier);
            inst->pendingDisconnect = FALSE;
            CsrBtHfgLinkConnectFailed(inst);
        }
        else if(inst->pendingCancel)
        {
            CsrBtHfgSendHfgServiceConnectInd(inst, CSR_BT_RESULT_CODE_HFG_CANCELLED_CONNECT_ATTEMPT, CSR_BT_SUPPLIER_HFG);
            inst->pendingCancel = FALSE;
            CsrBtHfgLinkConnectFailed(inst);
        }
        else
        {
            CsrBtHfgStartSdcSearch(inst);
        }
    }
    else
    {
        /* If not success, wait for CSR_BT_CM_CONNECT_ACCEPT_CFM */
        ;
    }
}

/* LP mode change while connected */
void CsrBtHfgConnectedCmModeChangeIndHandler(HfgInstance_t *inst)
{
    CsrBtCmRfcModeChangeInd *prim;
    CsrUint8 oldMode;
    CsrBool pending;
    HfgMainInstance_t *mainInst;

    prim = (CsrBtCmRfcModeChangeInd*)inst->msg;
    mainInst = CsrBtHfgGetMainInstance(inst);
    oldMode = inst->lpMode;
    pending = inst->pendingModeChange;

    /* Next mode-change is per default not from ourselves */
    inst->pendingModeChange = FALSE;

    /* Success so just store the actual link state which may be
     * different from the wanted but CM will ensure that link state
     * is changed later */
    if(prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
       (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS ||
        prim->resultCode == CSR_BT_RESULT_CODE_CM_MODE_CHANGE_REQUEST_OVERRULED))
    {
        inst->lpMode = prim->mode;

        /* This is a remote request as we did not request it */
        if(!pending)
        {
            if(prim->mode != inst->lpWantedMode)
            {
                /* Peer requested a mode we don't like, so restart the
                 * timer to begin sniff-mode retry. The
                 * disconnect-on-no-lp will eventually be handled by
                 * failed attempts to reenter sniff-mode */
                CsrBtHfgLpStart(inst, FALSE);
            }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
            else if((prim->mode == CSR_BT_LINK_STATUS_PARK) &&
               !(mainInst->lpMask & CSR_BT_HFG_PWR_DISABLE_PARK_ACP))
            {
                /* Remote peer requested park and we're allowed to
                 * enter -- set park as new wanted mode */
                inst->lpWantedMode = prim->mode;
            }
#endif
            else
            {
                /* Ok, new mode matches */
            }
        }

        /* Begin housecleaning to pump pending messages if any */
        CsrBtHfgSendHfgHouseCleaning(inst);
    }
    else
    {
        /* We requested a mode change, but it failed for the last time
         * and the user has told us to disconnect no-LP links */
        if(pending &&
           (inst->lpCount == 0) &&
           (CsrBtHfgGetMainInstance(inst)->lpMask & CSR_BT_HFG_PWR_DISCONNECT_ON_NO_LP) &&
           (inst->scoHandle == CSR_SCHED_QID_INVALID))
        {
            /* Disconnect link */
            inst->pendingDisconnect = TRUE;
            inst->lpWantedMode = CSR_BT_LINK_STATUS_DISCONNECTED;
            CsrBtCmDisconnectReqSend(inst->hfgConnId);
        }
    }

    /* Send LP update */
    if(mainInst->hfgConfig & CSR_BT_HFG_CNF_LP_STATUS)
    {
        CsrBtHfgSendHfgStatusLpInd(inst,
                              prim->mode,
                              oldMode,
                              inst->lpWantedMode,
                              (CsrBool)(pending ? FALSE : TRUE),
                              prim->resultCode,
                              prim->resultSupplier);
    }
}

/* Disconnect indication handler */
void CsrBtHfgXCmDisconnectIndHandler(HfgInstance_t *inst)
{
    CsrBtCmDisconnectInd *prim;
    prim = (CsrBtCmDisconnectInd*)inst->msg;

    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,inst->address,
                    prim->btConnId);
    /* Release the audio stream if any */
#ifdef CSR_USE_DSPM
    if (CsrBtHfgGetMainInstance(inst)->hfgDspmOpPending == DSPM_NOP)
    {
        CsrBtHfgDspmSendDisconnectStreams(inst);
    }
#endif
    /* Stop both ringing and LP changer */
    CsrBtHfgRingStop(inst);
    CsrBtHfgLpStop(inst);
    if (inst->deRegisterTimer != 0)
    {
        void *mv;
        CsrUint16 mi;
        CsrSchedTimerCancel(inst->deRegisterTimer,&mi,&mv);
        inst->deRegisterTimer = 0;
    }
    /* Clean local save queue */
    CsrBtHfgEmptySaveQueue(inst);

    if(inst->pendingDisconnect)
    {
        if(!prim->status)
        {
            CsrBtCmDisconnectReqSend(prim->btConnId);
        }
        else
        {
            CsrBtHfgLinkDisconnect(inst, prim->localTerminated, CSR_BT_RESULT_CODE_HFG_SUCCESS, CSR_BT_SUPPLIER_HFG);
        }
    }
    else if(inst->pendingCancel)
    {
        if(!prim->status)
        {
            CsrBtCmDisconnectReqSend(prim->btConnId);
        }
        else
        {
            if(inst->state == Connected_s)
            {
                CsrBtHfgLinkDisconnect(inst, prim->localTerminated, CSR_BT_RESULT_CODE_HFG_CANCELLED_CONNECT_ATTEMPT, CSR_BT_SUPPLIER_HFG);
            }
            else
            {

                CsrBtHfgSendHfgServiceConnectInd(inst, CSR_BT_RESULT_CODE_HFG_CANCELLED_CONNECT_ATTEMPT, CSR_BT_SUPPLIER_HFG);
                CsrBtHfgLinkConnectFailed(inst);
            }
        }
    }
    else
    {
        /* No disconnect req received so disconnect must be result
         * of LP fail. If result is 'link loss' the link is
         * unexpected terminated by remote side => start scanning
         * and inform app layer and let it decide how to
         * continue. Peer side has intentionally released the
         * connection => send info to app layer and await further
         * action */
        inst->pendingSco = FALSE;
        CsrBtHfgLinkDisconnect(inst, prim->localTerminated, prim->reasonCode, prim->reasonSupplier);
    }
}


/* RFCOMM data received */
void CsrBtHfgXCmDataIndHandler(HfgInstance_t *inst)
{
    CsrBtCmDataInd *cmPrim;
    CsrUintFast16 i;
    cmPrim = (CsrBtCmDataInd*)inst->msg;

    CsrBtCmDataResSend(inst->hfgConnId);

    /* AT commands may be divided into several packets this may be
     * continuation of a command where the first part is already
     * stored */
    if(inst->atCmd == NULL)
    {
        /* No data is currently stored from a previous command. */
        inst->atLen = cmPrim->payloadLength;
        inst->atCmd = CsrPmemAlloc(inst->atLen);
        CsrMemCpy(inst->atCmd, cmPrim->payload, inst->atLen);
    }
    else
    {
        /* Data is already received. Allocate space for the new data
         * and append */
        CsrUint16 currentAtCmdSize;
        CsrUint8 *p;

        currentAtCmdSize = inst->atLen;
        inst->atLen = currentAtCmdSize + cmPrim->payloadLength;
        p = (CsrUint8*) CsrPmemAlloc(inst->atLen);

        /* copy existing data and append it */
        CsrMemCpy(p, inst->atCmd, currentAtCmdSize);
        CsrMemCpy((p + currentAtCmdSize), cmPrim->payload, cmPrim->payloadLength);

        /* Data from inst and signal copied, so free it */
        CsrPmemFree(inst->atCmd);
        inst->atCmd = p;
    }

    /* If end of line received call the interpreter for the received
     * data. If end of line is NOT received, wait for more data to
     * come */
    for(i=(inst->atLen-1); i>0; i--)
    {
        if(inst->atCmd[i] == '\r')
        {
            /* Parse AT data */
            CsrBtHfgAtInterpret(inst);
            break;
        }
    }
}

/* Data was sent to the CM */
void CsrBtHfgXCmDataCfmHandler(HfgInstance_t *inst)
{
    CsrUint16 class;
    void *msg;

    if (inst->waitForDataCfm != 0)
    {
        inst->waitForDataCfm--;
        if (inst->waitForDataCfm == 0)
        {
            HFG_CHANGE_STATE(inst->atState, At5End_s);
            csrBtHfgSlcHfgDone(inst);
        }
    }
    /* Data was sent - pop from queue and send again, otherwise, open
     * up for transmission */
    if(CsrMessageQueuePop(&inst->cmTxQueue, &class, &msg))
    {
        CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE, class, msg);
    }
    else
    {
        inst->cmTxReady = TRUE;
    }
}

