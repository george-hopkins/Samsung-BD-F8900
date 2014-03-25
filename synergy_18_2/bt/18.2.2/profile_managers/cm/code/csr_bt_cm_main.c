/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_env_prim.h"
#include "csr_bt_cm_main.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_cm_callback_q.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_sdc.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_le.h"
#include "dmlib.h"
#include "csr_bt_cm_bccmd.h"
#include "csr_tm_bluecore_lib.h"
#include "csr_tm_bluecore_prim.h"

/*************************************************************************************
  CsrBtCmInit:
************************************************************************************/
void CsrBtCmInit(void **gash)
{ /* Allocate and initialise CM instance data space */
    cmInstanceData_t  *cmData;

    *gash = (void *) CsrPmemZalloc(sizeof(cmInstanceData_t));
    cmData = (cmInstanceData_t *) *gash;

    CsrBtCmInitInstData(cmData);
    CsrTmBlueCoreActivateTransportReqSend(CSR_BT_CM_IFACEQUEUE);
}

/*************************************************************************************
  CsrBtCmHandler:
************************************************************************************/
void CsrBtCmHandler(void **gash)
{
    cmInstanceData_t    *cmData;
    CsrUint16            eventClass=0;
    void *                msg=NULL;

    cmData = (cmInstanceData_t *) (*gash);
    CsrSchedMessageGet(&eventClass , &msg);

    if (eventClass == CSR_SCHED_PRIM)
    {
        CsrPmemFree(msg);
        return;
    }
    cmData->recvMsgP = msg;

    switch(eventClass)
    {
        case CSR_BT_CM_PRIM:
        { /* Received a event from the profiles */
            CsrBtCmProfileProvider(cmData);
            break;
        }
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
        case RFCOMM_PRIM :
        { /* Received a event from the RFCOMM layer */
            CsrBtCmRfcArrivalHandler(cmData);
            break;
        }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
        case L2CAP_PRIM :
        { /* Received a event from the L2CAP layer */
            CsrBtCmL2CaArrivalHandler(cmData);
            break;
        }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
        case CSR_BT_BNEP_PRIM:
        { /* Received a event from the BNEP layer */
            CsrBtCmBnepArrivalHandler(cmData);
            break;
        }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
        case DM_PRIM:
        { /* Received a event from the DM layer */
            CsrBtCmDmArrivalHandler(cmData);
            break;
        }
        case SDP_PRIM:
        { /* Received a event from the SDP layer */
            CsrBtCmSdcArrivalHandler(cmData);
            break;
        }
        case CSR_BCCMD_PRIM:
        {
            CsrBtCmBccmdArrivalHandler(cmData);
            break;
        }
        case CSR_TM_BLUECORE_PRIM:
        {
            CsrPrim *primType = (CsrPrim *)cmData->recvMsgP;

            if (*primType == CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM)
            {
                CsrTmBluecoreActivateTransportCfm *prim = (CsrTmBluecoreActivateTransportCfm *) cmData->recvMsgP;
                prim = (CsrTmBluecoreActivateTransportCfm *) cmData->recvMsgP;

                if (prim->result == CSR_RESULT_SUCCESS)
                {
                    dm_am_register_req(CSR_BT_CM_IFACEQUEUE);
                }
                else
                {
                    CsrBtCmGeneralException(CSR_TM_BLUECORE_PRIM, *primType, "CsrTmBluecoreActivateTransportCfm with a failure");
                }
            }
            else
            {
                CsrBtCmGeneralException(CSR_TM_BLUECORE_PRIM, *primType, "Unhandle CsrTmBlueCore message");
            }
            break;
        }
        default :
        {
            CsrBtCmStateEventException(eventClass, 0, cmData->globalState);
            break;
        }
    }
    CsrPmemFree(cmData->recvMsgP);
}

#ifdef ENABLE_SHUTDOWN
/****************************************************************************
    This function is called by the scheduler to perform a graceful shutdown
    of a scheduler task.
    This function must:
    1)    empty the input message queue and free any allocated memory in the
        messages.
    2)    free any instance data that may be allocated.
****************************************************************************/
void CsrBtCmDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    CsrBool lastMsg;
    CsrUintFast16 i = 0;
    cmInstanceData_t *cmData;

    cmData = (cmInstanceData_t *) (*gash);
    lastMsg = FALSE;

    /* Remove message off both save queues and the scheduler queue */
    while (!lastMsg)
    {
        if (!CsrMessageQueuePop(&cmData->dmVar.SaveQueue, &msg_type, &msg_data))
        {
            if (!CsrMessageQueuePop(&cmData->smVar.SaveQueue, &msg_type, &msg_data))
            {
                lastMsg = (CsrBool)(!CsrSchedMessageGet(&msg_type, &msg_data));
            }
        }
        if (!lastMsg)
        {
            switch (msg_type)
            {
                case CSR_BCCMD_PRIM:
                {
                    CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, msg_data);
                    break;
                }
                case CSR_BT_CM_PRIM:
                {
                    CsrPrim *primType = (CsrPrim *) msg_data;

                    if(*primType >= CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)
                    {
                        CsrBtCmFreeUpstreamMessageContents(msg_type, msg_data);
                    }
                    else
                    {
                        CsrBtCmFreeDownstreamMessageContents(msg_type, msg_data);
                    }
                    break;
                }
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
                case RFCOMM_PRIM :
                {
                    rfc_free_primitive(msg_data);
                    msg_data = NULL;
                    break;
                }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

                case DM_PRIM:
                {
                    dm_free_upstream_primitive(msg_data);
                    msg_data = NULL;
                    break;
                }

                case SDP_PRIM:
                {
                    sdp_free_upstream_primitive(msg_data);
                    msg_data = NULL;
                    break;
                }

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
                case L2CAP_PRIM:
                {
                    L2CA_FreePrimitive((L2CA_UPRIM_T *) msg_data);
                    msg_data = NULL;
                    break;
                }

#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
                case CSR_BT_BNEP_PRIM :
                {
                    CsrPrim    *primType;

                    primType = (CsrPrim *) msg_data;
                    switch (*primType)
                    {
                        case BNEP_EXTENDED_DATA_IND :
                        {
                            BNEP_EXTENDED_DATA_IND_T *prim;

                            prim = (BNEP_EXTENDED_DATA_IND_T *) msg_data;
                            CsrMblkDestroy(prim->mblk);
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
                default:
                {
                    break;
                }
            }
            CsrPmemFree (msg_data);
        }

    }

    /* Now free the remaining instance data */

    CsrBtCmCallbackDeinit(cmData);

    if (cmData->dmVar.dmCacheParamTable != NULL)
    {
        CsrBtCmRemoveCacheParamTable(cmData);
    }

    if (cmData->sdcVar.sdcSearchList)
    {
        sdcSearchElement    * currentElement;
        sdcSearchElement    * nextElement;

        nextElement    =    cmData->sdcVar.sdcSearchList;
        do
        {
            currentElement    = nextElement;
            nextElement        = currentElement->nextSdcElement;
            for (i = 0; i < currentElement->numOfSearchPtr; i++)
            {
                if (currentElement->searchPtrArray[i])
                {
                    CsrPmemFree(currentElement->searchPtrArray[i]->serverChannelList);
                    CsrPmemFree(currentElement->searchPtrArray[i]->serviceHandle);
                    CsrPmemFree(currentElement->searchPtrArray[i]->validResult);
                    CsrPmemFree(currentElement->searchPtrArray[i]);
                }
            }
            CsrPmemFree(currentElement->searchPtrArray);
            CsrPmemFree(currentElement->serviceList);
            CsrPmemFree(currentElement->uuidSet);
            CsrPmemFree(currentElement->uuid128ServiceList);
            CsrPmemFree(currentElement);

        } while(nextElement != NULL);
    }

    CsrBtCmFlushCmCacheStopTimer(cmData);
    CsrPmemFree(cmData->dmVar.eventFilters);
    CsrPmemFree(cmData->dmVar.localName);

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    CsrPmemFree(cmData->rfcVar.localServerChList);
    CsrCmnListDeinit(&(cmData->rfcVar.connList));
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

    CsrBtUtilBllFreeLinkedList(&cmData->subscriptions, CsrBtUtilBllPfreeWrapper);
    CsrBtUtilBllFreeLinkedList(&cmData->afhMaps, CsrBtUtilBllPfreeWrapper);
    CsrBtUtilBllFreeLinkedList(&cmData->lstoList, CsrBtUtilBllPfreeWrapper);

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    if (cmData->dmVar.localEirData)
    {
        /* Free all Extended Inquiry Response related instance data */
        CsrPmemFree(cmData->dmVar.localEirData->currentName);

        CsrPmemFree(cmData->dmVar.localEirData->services);

        CsrPmemFree(cmData->dmVar.localEirData->manufacturerData);

        CsrPmemFree(cmData->dmVar.localEirData->requestedName);

        CsrPmemFree(cmData->dmVar.localEirData->requestedServices);

        CsrPmemFree(cmData->dmVar.localEirData);
    }
#endif /* CSR_BT_BT_VERSION */

#ifdef CSR_BT_LE_ENABLE
    CsrBtCmLeDeinit(cmData);
#endif

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    CsrCmnListDeinit(&(cmData->l2caVar.connList));
#ifdef CSR_BT_INSTALL_L2CAP_CONNLESS_SUPPORT
    CsrCmnListDeinit(&(cmData->l2caVar.connlessList));
#endif
    CsrCmnListDeinit(&cmData->remoteVersion);
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

    CsrPmemFree(cmData);
}
#endif /* ENABLE_SHUTDOWN */

#ifdef CSR_LOG_ENABLE
void CsrBtCmChangeState(const CsrCharString *file, CsrUint32 line,
                              const CsrCharString *fsmName,
                              CsrUint32 *state, CsrUint32 newState, const CsrCharString *newStateName)
{
    CsrLogStateTransition((CSR_LOG_STATE_TRANSITION_MASK_FSM_NAME |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE_STR |
                           CSR_LOG_STATE_TRANSITION_MASK_PREV_STATE),
                          0,
                          fsmName,
                          *state,
                          NULL,
                          0,
                          NULL,
                          newState,
                          newStateName,
                          line,
                          file);

    *state = newState;
}
#endif

