/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

void CsrBtAmpmHandler(CsrBtAmpmInstance * inst)
{
    void *prim = inst->msg;
    switch (*(CsrUint16 *) (prim))
    {
        /* --------- Application ---------- */

        case CSR_BT_AMPM_REGISTER_REQ:
            CsrBtAmpmAppRegisterReq(inst, (CsrBtAmpmRegisterReq*)prim);
            break;

        case CSR_BT_AMPM_DEREGISTER_REQ:
            CsrBtAmpmAppDeregisterReq(inst, (CsrBtAmpmDeregisterReq*)prim);
            break;

        case CSR_BT_AMPM_CONTROLLER_REQ:
            CsrBtAmpmAppControllerReq(inst, (CsrBtAmpmControllerReq*)prim);
            break;

        case CSR_BT_AMPM_MOVE_REQ:
            CsrBtAmpmAppCmMoveChanReq(inst, (CsrBtAmpmMoveReq*)prim);
            break;

        case CSR_BT_AMPM_MOVE_RES:
            CsrBtAmpmAppCmMoveChanRes(inst, (CsrBtAmpmMoveRes*)prim);
            break;

        case CSR_BT_AMPM_REGISTER_POWER_ON_REQ:
            CsrBtAmpmAppRegisterPowerOnReq(inst, (CsrBtAmpmRegisterPowerOnReq*)prim);
            break;

        case CSR_BT_AMPM_POWER_ON_RES:
            CsrBtAmpmAppPowerOnRes(inst, (CsrBtAmpmPowerOnRes*)prim);
            break;

        /* --------- PAL ---------- */

        case CSR_BT_AMPM_PAL_REGISTER_REQ:
            CsrBtAmpmLocalRegisterReq(inst,
                                      (const CsrBtAmpmPalRegisterReq *) prim);
            break;

        case CSR_BT_AMPM_PAL_DEREGISTER_REQ:
            CsrBtAmpmLocalDeregisterReq(inst,
                                        (const CsrBtAmpmPalDeregisterReq *) prim);
            break;

        /* --------- Test ---------- */

        case CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ:
        {
            CsrBtAmpmTestTimerControlReq *req;
            req = (CsrBtAmpmTestTimerControlReq*)prim;
            CsrBtAmpmDataSetTimersEnableState(inst,
                                              req->enabled);
            break;
        }

        case CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ:
            CsrBtAmpmPhySetDontStartPhyLink(inst,
                                            (CsrBtAmpmTestDontStartPhyLinkReq*)prim);
            break;

        case CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ:
            CsrBtAmpmLogSetDontStartLogicalLink(inst,
                                                (CsrBtAmpmTestDontStartLogLinkReq*)prim);
            break;

        case CSR_BT_AMPM_TEST_PHY_COLLISION_REQ:
            CsrBtAmpmPhySetPhyCollision(inst,
                                        (CsrBtAmpmTestPhyCollisionReq*)prim);
            break;

        case CSR_BT_AMPM_TEST_SDU_REQ:
            CsrBtAmpmBtdevTestSduReq(inst,
                                     (CsrBtAmpmTestSduReq*)prim);
            break;

        case CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ:
        {
            CsrBtAmpmTestRouteReceivedSduReq *req;
            req = (CsrBtAmpmTestRouteReceivedSduReq *) prim;
            CsrBtAmpmDataSetRxSduControlOption(inst,
                                               req->rxSduControl,
                                               req->qid);
        }
        break;

        case CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ:
        {
            CsrBtAmpmTestCancelCreatePhysicalLinkReq *req;
            req = (CsrBtAmpmTestCancelCreatePhysicalLinkReq *) prim;
            CsrBtAmpmPhySetCancelCreatePhyLinkOptions(inst,
                                                      req->enabled,
                                                      req->sendAfterResponse,
                                                      req->invalidResponderAmpId,
                                                      req->delayAfterReq);
        }
        break;

        case CSR_BT_AMPM_TEST_IGNORE_SDU_REQ:
        {
            CsrBtAmpmTestIgnoreSduReq *req;
            req = (CsrBtAmpmTestIgnoreSduReq *) prim;
            CsrBtAmpmDataSetIgnoreSduOptions(inst,
                                             req->enabled,
                                             req->sduId,
                                             req->ignoreTimes);
        }
        break;

        case CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ:
        {
            CsrBtAmpmTestChangeAmpKeyReq *req;
            req = (CsrBtAmpmTestChangeAmpKeyReq *) prim;
            CsrBtAmpmDataSetChangeAmpKeyOptions(inst,
                                                req->enabled,
                                                req->ampType,
                                                req->changeTimes);
        }
        break;

        case CSR_BT_AMPM_TEST_GET_ASSOC_REQ:
        {
            CsrBtAmpmTestGetAssocReq* req;
            req = (CsrBtAmpmTestGetAssocReq*)prim;
            CsrBtAmpmSendTestGetAssocReq(inst,
                                         &req->bdAddr,
                                         req->signalId,
                                         req->ampId);
        }
        break;

        case CSR_BT_AMPM_TEST_HCI_REQ:
        {
            CsrBtAmpmTestHciReq* req;
            req = (CsrBtAmpmTestHciReq*)prim;
            CsrBtAmpmLocalHciTxCmd(inst,
                                   req->ampId,
                                   NULL,
                                   req->data);
            req->data = NULL;
            req->length = 0;
        }
        break;

        /* --------- Profile ---------- */

        case CSR_BT_AMPM_PRF_MULTI_CONN_REQ:
            CsrBtAmpmPrfMultiConnReqHandler(inst, (CsrBtAmpmPrfMultiConnReq*)prim);
            break;

        default:
            /* Ignore */
            break;
    }

}
