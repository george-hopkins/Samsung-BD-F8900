/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_result.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_hidh_main.h"
#include "csr_bt_hidh_prim.h"
#include "csr_bt_hidh_con.h"
#include "csr_bt_hidh_sdc.h"
#include "csr_bt_hidh_data.h"
#include "csr_bt_hidh_util.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

/* ---------- HIDH instance handler jump table ---------- */
static hidhStateHandler_t hidhStateHandler[CSR_BT_HIDH_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtHidhConnectReqHandler,             /* CSR_BT_HIDH_CONNECT_REQ */
    CsrBtHidhConnectAcceptReqHandler,       /* CSR_BT_HIDH_CONNECT_ACCEPT_REQ */
    CsrBtHidhCancelConnectAcceptReqHandler, /* CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_REQ */
    CsrBtHidhDisconnectReqHandler,          /* CSR_BT_HIDH_DISCONNECT_REQ */
    CsrBtHidhControlReqHandler,             /* CSR_BT_HIDH_CONTROL_REQ */
    CsrBtHidhGetReportReqHandler,           /* CSR_BT_HIDH_GET_REPORT_REQ */
    CsrBtHidhSetReportReqHandler,           /* CSR_BT_HIDH_SET_REPORT_REQ */
    CsrBtHidhGetProtocolReqHandler,         /* CSR_BT_HIDH_GET_PROTOCOL_REQ */
    CsrBtHidhSetProtocolReqHandler,         /* CSR_BT_HIDH_SET_PROTOCOL_REQ */
    CsrBtHidhGetIdleReqHandler,             /* CSR_BT_HIDH_GET_IDLE_REQ */
    CsrBtHidhSetIdleReqHandler,             /* CSR_BT_HIDH_SET_IDLE_REQ */
    CsrBtHidhDataReqHandler,                /* CSR_BT_HIDH_DATA_REQ */
    CsrBtHidhRegisterUserReqHandler,        /* CSR_BT_HIDH_REGISTER_USER_REQ */
    NULL,                                   /* <EMPTY> */
    CsrBtHidhSecurityInReqHandler,          /* CSR_BT_HIDH_SECURITY_IN_REQ */
    CsrBtHidhSecurityOutReqHandler,         /* CSR_BT_HIDH_SECURITY_OUT_REQ */
};
/* ---------- End of jump tables ---------- */

static void csrBtHidhDelayMessage(CsrUint16 prim, void *msg)
{
    CsrSchedMessagePut(CSR_BT_HIDH_IFACEQUEUE, prim, msg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhInit
 *
 *  DESCRIPTION
 *      Initializes the HID host profile component
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhInit(void **gash)
{
    hidh_instData_t  *instData;
    CsrUint8    i;

    /*  allocate and initialise instance data space */
    *gash = (void *) CsrPmemZalloc(sizeof(hidh_instData_t));
    instData = *gash;

    CsrBtScSetSecInLevel(&instData->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_HIDH_MANDATORY_SECURITY_INCOMING,
        CSR_BT_HIDH_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_HIDH_SUCCESS,
        CSR_BT_RESULT_CODE_HIDH_UNACCEPTABLE_PARAMETER);

    CsrBtScSetSecOutLevel(&instData->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_HIDH_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_HIDH_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_HIDH_SUCCESS,
        CSR_BT_RESULT_CODE_HIDH_UNACCEPTABLE_PARAMETER);

    for(i=0; i<MAX_HID_CONNECTIONS; i++)
    {
        instData->slots[i] = CSR_SCHED_QID_INVALID;
        CSR_BT_HIDH_SUB_STATE_CHANGE(i, NULL_STATE);
    }

    /* Tell the SD that it must look for the CSR_BT_HID_PROFILE_UUID
       service, when it perform a SD_READ_AVAILABLE_SERVICE_REQ   */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_HID_PROFILE_UUID);

    CsrBtCml2caRegisterReqSend(CSR_BT_HIDH_IFACEQUEUE, CSR_BT_HID_CTRL_PSM);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      idleCmHandler
 *
 *  DESCRIPTION
 *      Handle an upstream CM primitive in IDLE state
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void idleCmHandler(hidh_instData_t *instData)
{
    void * msg;
    CsrPrim *primType;

    msg = instData->recvMsgP;
    primType = (CsrPrim*) msg;

    switch (*primType)
    {
        case CSR_BT_CM_L2CA_REGISTER_CFM:
        {
            CsrBtCmL2caRegisterCfm *prim;

            prim = (CsrBtCmL2caRegisterCfm *) msg;

            if( prim->localPsm == CSR_BT_HID_CTRL_PSM )
            {
                CsrBtCml2caRegisterReqSend(CSR_BT_HIDH_IFACEQUEUE, CSR_BT_HID_INTR_PSM);
            }
            else
            {
                CSR_BT_HIDH_STATE_CHANGE(instData->state, ACTIVE);
            }
            break;
        }
        default:
        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("HIDH", CSR_BT_CM_PRIM, *primType, (CsrUint8) instData->state);
#endif
            CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
            break;
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      activeCmHandler
 *
 *  DESCRIPTION
 *      Handle an upstream CM primitive in ACTIVE state.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void activeCmHandler(hidh_instData_t * instData)
{
    void * msg;
    CsrPrim *primType;

    msg = instData->recvMsgP;
    primType = (CsrPrim*) msg;

    switch (*primType)
    {
        case CSR_BT_CM_L2CA_DATA_IND:
            {
                CsrBtHidhCmL2caDataInd(instData);
                break;
            }
        case CSR_BT_CM_L2CA_DATA_CFM:
            {
                CsrBtHidhCmL2caDataCfm(instData);
                break;
            }
        case CSR_BT_CM_L2CA_CONNECT_CFM:
            {
                CsrBtHidhCmL2caConnectCfm(instData);
                break;
            }
        case CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM:
            {
                CsrBtHidhCmL2caConnectAcceptCfm(instData);
                break;
            }
        case CSR_BT_CM_L2CA_DISCONNECT_IND:
            {
                CsrBtHidhCmL2caDisconnectInd(instData);
                break;
            }
        case CSR_BT_CM_L2CA_MODE_CHANGE_IND:
            {
                CsrBtHidhCmL2caModeChangeInd(instData);
                break;
            }
        case CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM:
            {
                /* NOP */
                break;
            }
        case CSR_BT_CM_SDC_SEARCH_IND:
            {
                CsrBtHidhCmSdcSearchInd(instData);
                break;
            }
        case CSR_BT_CM_SDC_SEARCH_CFM:
            {
                CsrBtHidhCmSdcSearchCfm(instData);
                break;
            }
        case CSR_BT_CM_SDC_ATTRIBUTE_CFM:
            {
                CsrBtHidhCmSdcAttributeCfm(instData);
                break;
            }
        case CSR_BT_CM_SDC_CLOSE_IND:
            {
                CsrBtHidhCmSdcCloseInd(instData);
                break;
            }
        case CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM:
            {
                CsrBtHidhCmL2caCancelConnectAcceptCfm(instData);
                break;
            }
        case CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM:
            {
                CsrBtHidhCmL2caCancelConnectAcceptCfm(instData);
                break;
            }
        default:
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("HIDH", CSR_BT_CM_PRIM, *primType, (CsrUint8) instData->state);
#endif
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
                break;
            }
    }
}

#ifdef ENABLE_SHUTDOWN
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhDeinit
 *
 *  DESCRIPTION
 *      This function is called by the scheduler to perform a graceful shutdown
 *        of a scheduler task.
 *        This function must:
 *        1)    empty the input message queue and free any allocated memory in the
 *            messages.
 *        2)    free any instance data that may be allocated.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    CsrUint8 i;
    CsrBool    lastMsg;
    hidh_instData_t  *instData;

    instData = (hidh_instData_t *) (*gash);

    lastMsg = FALSE;

    while (!lastMsg)
    {
        lastMsg = !CsrSchedMessageGet(&msg_type, &msg_data);

        if (!lastMsg)
        {
            switch (msg_type)
            {
                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(msg_type, msg_data);
                    break;
                }

                case CSR_BT_HIDH_PRIM:
                {
                    CsrBtHidhFreeDownstreamMessageContents(msg_type, msg_data);
                    break;
                }

                case CSR_SCHED_PRIM:
                    break;

                default:
                {
                    break;
                }
            }

            CsrPmemFree (msg_data);
        }
    }

    for (i=0; i<MAX_HID_CONNECTIONS; i++)
    {
        freeSubInst(instData,i);
    }

    CsrPmemFree(instData);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhConnectionCleanUp
 *
 *  DESCRIPTION
 *      Clean up a connection (used if application has terminated)
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhConnectionCleanUp(hidh_instData_t *instData, CsrUint8 deviceId)
{
    hidh_subInstData_t *subInstData = &instData->subInstData[deviceId];

    if( subInstData->subState == NULL_STATE )
    {
        return;
    }

    subInstData->doTerminate = TRUE;

    if( subInstData->subState == AWAIT_ESTABLISH )
    {
        if( subInstData->timerId != 0)
        {
            CsrSchedTimerCancel(subInstData->timerId, NULL, NULL);
            subInstData->timerId=0;
        }

        if( subInstData->ctrl_ch.cid != 0)
        {
            CsrBtCml2caDisconnectReqSend(subInstData->ctrl_ch.cid);
        }
        else
        {
            CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_HIDH_IFACEQUEUE, CSR_BT_HID_CTRL_PSM);
            CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, CANCEL_ESTABLISH);
        }

        CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_HIDH_IFACEQUEUE, CSR_BT_HID_INTR_PSM);
    }
    else if( subInstData->subState == CONNECTED )
    {
        CsrBtCml2caDisconnectReqSend(subInstData->intr_ch.cid);

        CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, DISCONNECTING);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhHandler
 *
 *  DESCRIPTION
 *      Entry function for the HID host profile component, main handler.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhHandler(void **gash)
{
    hidh_instData_t    *instData;
    CsrUint16        eventClass=0;
    void            *msg=NULL;

    instData = (hidh_instData_t *) (*gash);

    CsrSchedMessageGet(&eventClass , &msg);

    instData->recvMsgP = msg;

    switch(eventClass)
    {
        case CSR_BT_HIDH_PRIM:
            {
                CsrBtHidhPrim         *primType;

                /* find the message type */
                primType = (CsrBtHidhPrim *)msg;

                if( instData->state == ACTIVE )
                {
                    if ( *primType < CSR_BT_HIDH_PRIM_DOWNSTREAM_COUNT)
                    {
                        hidhStateHandler[*primType](instData);
                    }
                    else
                    {
                        CsrBtHidhFreeDownstreamMessageContents(eventClass, msg);
                    }
                }
                else
                {
                    /* profile not ready (IDLE state) for requests yet, delay request half a second */
                    CsrSchedTimerSet(500000, csrBtHidhDelayMessage,CSR_BT_HIDH_PRIM,msg);
                    instData->recvMsgP = NULL;
                }
                break;
            }

        case CSR_BT_CM_PRIM:
            {
                switch (instData->state)
                {
                    case IDLE:
                    {
                        idleCmHandler(instData);
                        break;
                    }
                    case ACTIVE:
                    {
                        activeCmHandler(instData);
                        break;
                    }
                    default:
                    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrBtCmPrim *primType = (CsrBtCmPrim *) msg;
                        CsrStateEventException("HIDH", eventClass, *primType, (CsrUint8) instData->state);
#endif
                        CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
                        break;
                    }
                }
                break;
            }

        case CSR_SCHED_PRIM:
            {
                CsrEnvPrim *primType = (CsrEnvPrim *) msg;

                switch(*primType)
                {
                    case CSR_CLEANUP_IND:
                        {
                            CsrUint8 i;
                            CsrCleanupInd* prim;
                            prim = (CsrCleanupInd*) msg;
                            if(prim->phandle == instData->ctrlHandle)
                            {
                                /* the control application has terminated, clean up everything */
                                for( i=0;i<MAX_HID_CONNECTIONS;i++ )
                                {
                                    CsrBtHidhConnectionCleanUp(instData, i);
                                }
                            }
                            else
                            {
                                /* find the user application that terminated and clean up its connection */
                                for( i=0;i<MAX_HID_CONNECTIONS;i++ )
                                {
                                    if( prim->phandle == instData->subInstData[i].userHandle )
                                    {
                                        CsrBtHidhConnectionCleanUp(instData, i);
                                    }

                                    /* Clean slots too */
                                    if(prim->phandle == instData->slots[i])
                                    {
                                        instData->slots[i] = CSR_SCHED_QID_INVALID;
                                    }
                                }
                            }
                            break;
                        }
                    default:
                        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                            CsrStateEventException("HIDH",
                            eventClass,
                            *primType,
                            (CsrUint8) instData->state);
#endif
                            break;
                        }
                }

                break;
            }

        default:
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrPrim *type = (CsrPrim *) msg;
                CsrStateEventException("HIDH",
                    eventClass,
                    *type,
                    (CsrUint8) instData->state);
#endif
                break;
            }
    }
    CsrPmemFree(instData->recvMsgP);    /*  free the received message. if NULL the scheduler just ignores */
}

#ifdef CSR_LOG_ENABLE
void CsrBtHidhChangeState(const CsrCharString *file, CsrUint32 line,
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

