/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_sched.h"
#include "csr_panic.h"
#include "csr_env_prim.h"
#include "csr_bt_result.h"
#include "csr_bt_saps_main.h"
#include "csr_bt_saps_sef.h"
#include "csr_bt_sc_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

/* SAPS jump table */
static const SapsStateHandlerType sapsStateHandlers[SapsNumberOfStates][CSR_BT_SAPS_PRIM_DOWNSTREAM_COUNT] =
{
    /* Init_s */
    {
        CsrBtSapsActivateInitState,                              /* CSR_BT_SAPS_ACTIVATE_REQ */
        CsrBtSapsDeactivateInitState,                            /* CSR_BT_SAPS_DEACTIVATE_REQ */
        NULL,                                               /* CSR_BT_SAPS_CONNECT_RES */
        NULL,                                               /* CSR_BT_SAPS_DISCONNECT_REQ */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_APDU_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_ATR_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_OFF_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_ON_RES */
        NULL,                                               /* CSR_BT_SAPS_RESET_SIM_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */
        NULL,                                               /* CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */
        NULL,                                               /* CSR_BT_SAPS_SEND_STATUS_REQ */
        CsrBtSapsSecurityInReqHandler,                           /* CSR_BT_SAPS_SECURITY_IN_REQ */
    },
    /* Idle_s */
    {
        CsrBtSapsActivateIdleState,                              /* CSR_BT_SAPS_ACTIVATE_REQ */
        CsrBtSapsDeactivateIdleState,                            /* CSR_BT_SAPS_DEACTIVATE_REQ */
        NULL,                                               /* CSR_BT_SAPS_CONNECT_RES */
        NULL,                                               /* CSR_BT_SAPS_DISCONNECT_REQ */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_APDU_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_ATR_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_OFF_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_ON_RES */
        NULL,                                               /* CSR_BT_SAPS_RESET_SIM_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */
        NULL,                                               /* CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */
        NULL,                                               /* CSR_BT_SAPS_SEND_STATUS_REQ */
        CsrBtSapsSecurityInReqHandler,                           /* CSR_BT_SAPS_SECURITY_IN_REQ */
    },
    /* Activated_s */
    {
        NULL,                                               /* CSR_BT_SAPS_ACTIVATE_REQ */
        CsrBtSapsDeactivateActivatedState,                       /* CSR_BT_SAPS_DEACTIVATE_REQ */
        NULL,                                               /* CSR_BT_SAPS_CONNECT_RES */
        NULL,                                               /* CSR_BT_SAPS_DISCONNECT_REQ */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_APDU_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_ATR_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_OFF_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_ON_RES */
        NULL,                                               /* CSR_BT_SAPS_RESET_SIM_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */
        NULL,                                               /* CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */
        NULL,                                               /* CSR_BT_SAPS_SEND_STATUS_REQ */
        CsrBtSapsSecurityInReqHandler,                           /* CSR_BT_SAPS_SECURITY_IN_REQ */
    },
    /* Connected_s */
    {
        NULL,                                               /* CSR_BT_SAPS_ACTIVATE_REQ */
        CsrBtSapsDeactivateReqConnectedState,                    /* CSR_BT_SAPS_DEACTIVATE_REQ */
        CsrBtSapsConnectResConnectedState,                       /* CSR_BT_SAPS_CONNECT_RES */
        NULL,                                               /* CSR_BT_SAPS_DISCONNECT_REQ */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_APDU_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_ATR_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_OFF_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_ON_RES */
        NULL,                                               /* CSR_BT_SAPS_RESET_SIM_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */
        NULL,                                               /* CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */
        NULL,                                               /* CSR_BT_SAPS_SEND_STATUS_REQ */
        CsrBtSapsSecurityInReqHandler,                           /* CSR_BT_SAPS_SECURITY_IN_REQ */
    },
    /* SapConnected_s */
    {
        NULL,                                               /* CSR_BT_SAPS_ACTIVATE_REQ */
        CsrBtSapsDeactivateReqSapConnectedState,                 /* CSR_BT_SAPS_DEACTIVATE_REQ */
        NULL,                                               /* CSR_BT_SAPS_CONNECT_RES */
        CsrBtSapsDisconnectReqSapConnectedState,                 /* CSR_BT_SAPS_DISCONNECT_REQ */
        CsrBtSapsTransferApduResConnectedState,                  /* CSR_BT_SAPS_TRANSFER_APDU_RES */
        CsrBtSapsTransferAtrResSapConnectedState,                /* CSR_BT_SAPS_TRANSFER_ATR_RES */
        CsrBtSapsTransferPowerSimOffResSapConnectedState,        /* CSR_BT_SAPS_POWER_SIM_OFF_RES */
        CsrBtSapsTransferPowerSimOnResSapConnectedState,         /* CSR_BT_SAPS_POWER_SIM_ON_RES */
        CsrBtSapsTransferResetSimResSapConnectedState,           /* CSR_BT_SAPS_RESET_SIM_RES */
        CsrBtSapsTransferCardStatusSapConnectedState,            /* CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */
        CsrBtSapsSetTransferProtocolSapConnectedState,           /* CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */
        CsrBtSapsSendStatusReqSapConnectedState,                 /* CSR_BT_SAPS_SEND_STATUS_REQ */
        CsrBtSapsSecurityInReqHandler,                           /* CSR_BT_SAPS_SECURITY_IN_REQ */
    },
    /* Disconnecting_s */
    {
        NULL,                                               /* CSR_BT_SAPS_ACTIVATE_REQ */
        CsrBtSapsDeactivateReqDisconnectingState,                /* CSR_BT_SAPS_DEACTIVATE_REQ */
        CsrBtSapsConnectResDisconnectingState,                   /* CSR_BT_SAPS_CONNECT_RES */
        NULL,                                               /* CSR_BT_SAPS_DISCONNECT_REQ */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_APDU_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_ATR_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_OFF_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_ON_RES */
        NULL,                                               /* CSR_BT_SAPS_RESET_SIM_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */
        NULL,                                               /* CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */
        NULL,                                               /* CSR_BT_SAPS_SEND_STATUS_REQ */
        CsrBtSapsSecurityInReqHandler,                           /* CSR_BT_SAPS_SECURITY_IN_REQ */
    },
    /* Deactivate_s */
    {
        NULL,                                               /* CSR_BT_SAPS_ACTIVATE_REQ */
        NULL,                                               /* CSR_BT_SAPS_DEACTIVATE_REQ */
        NULL,                                               /* CSR_BT_SAPS_CONNECT_RES */
        NULL,                                               /* CSR_BT_SAPS_DISCONNECT_REQ */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_APDU_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_ATR_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_OFF_RES */
        NULL,                                               /* CSR_BT_SAPS_POWER_SIM_ON_RES */
        NULL,                                               /* CSR_BT_SAPS_RESET_SIM_RES */
        NULL,                                               /* CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */
        NULL,                                               /* CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */
        NULL,                                               /* CSR_BT_SAPS_SEND_STATUS_REQ */
        CsrBtSapsSecurityInReqHandler,                           /* CSR_BT_SAPS_SECURITY_IN_REQ */
    }
};

/* CM jump table */
/* PLEASE NOTE: only the top most part of the CM primitives are included */
static const SapsStateHandlerType cmStateHandlers[SapsNumberOfStates][CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT] =
{
    /* Init_s */
    {
        NULL,                                               /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtSapsInitStateCmRegisterHandler,                /* CSR_BT_CM_REGISTER_CFM */
        NULL,                                               /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_DATA_IND */
        NULL,                                               /* CSR_BT_CM_DATA_CFM */
        NULL,                                               /* CSR_BT_CM_CONTROL_IND */
        NULL,                                               /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Idle_s */
    {
        NULL,                                               /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                               /* CSR_BT_CM_REGISTER_CFM */
        NULL,                                               /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_DATA_IND */
        NULL,                                               /* CSR_BT_CM_DATA_CFM */
        NULL,                                               /* CSR_BT_CM_CONTROL_IND */
        NULL,                                               /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Activated_s */
    {
        NULL,                                               /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_CFM */
        CsrBtSapsActivatedStateConnectAcceptHandler,        /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                               /* CSR_BT_CM_REGISTER_CFM */
        NULL,                                               /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_DATA_IND */
        NULL,                                               /* CSR_BT_CM_DATA_CFM */
        NULL,                                               /* CSR_BT_CM_CONTROL_IND */
        NULL,                                               /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Connected_s */
    {
        NULL,                                               /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                               /* CSR_BT_CM_REGISTER_CFM */
        CsrBtSapsConnectStateDisconnectInd,                 /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtSapsDataIndHandler,                            /* CSR_BT_CM_DATA_IND */
        CsrBtSapsDataCfmHandler,                            /* CSR_BT_CM_DATA_CFM */
        CsrBtSapsControlInd,                                /* CSR_BT_CM_CONTROL_IND */
        CsrBtSapsModeChangeInd,                             /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* SapConnected_s */
    {
        NULL,                                               /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                               /* CSR_BT_CM_REGISTER_CFM */
        CsrBtSapsSapConnectedStateDisconnectInd,            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtSapsDataIndHandler,                            /* CSR_BT_CM_DATA_IND */
        CsrBtSapsDataCfmHandler,                            /* CSR_BT_CM_DATA_CFM */
        CsrBtSapsControlInd,                                /* CSR_BT_CM_CONTROL_IND */
        CsrBtSapsModeChangeInd,                             /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Disconnecting_s */
    {
        NULL,                                               /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                               /* CSR_BT_CM_REGISTER_CFM */
        CsrBtSapsDisconnectingStateDisconnectInd,           /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtSapsDataIndHandler,                            /* CSR_BT_CM_DATA_IND */
        CsrBtSapsDataCfmHandler,                            /* CSR_BT_CM_DATA_CFM */
        CsrBtSapsControlInd,                                /* CSR_BT_CM_CONTROL_IND */
        CsrBtSapsModeChangeInd,                             /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Deactivate_s */
    {
        CsrBtSapsDeactivateCancelAcceptConnectCfm,          /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_CONNECT_CFM */
        CsrBtSapsDeactivateAcceptConnectCfm,                /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                               /* CSR_BT_CM_REGISTER_CFM */
        CsrBtSapsDeactivateDisconnectInd,                   /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                               /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                               /* CSR_BT_CM_DATA_IND */
        CsrBtSapsDataCfmHandler,                            /* CSR_BT_CM_DATA_CFM */
        CsrBtSapsControlInd,                                /* CSR_BT_CM_CONTROL_IND */
        CsrBtSapsModeChangeInd,                             /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                               /* CSR_BT_CM_PORTNEG_CFM */
    }
};
/* ---------- End of jump tables ---------- */

void CsrBtSapsInit(void **gash)
{
    SapsInstanceData_t  *instData;

    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemZalloc(sizeof(SapsInstanceData_t));
    instData = (SapsInstanceData_t *) *gash;

    /* clear own instance data for first time use */
    instData->transmitReady     = TRUE;

    CsrBtScSetSecInLevel(&instData->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_SIM_ACCESS_MANDATORY_SECURITY_INCOMING,
        CSR_BT_SIM_ACCESS_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_SAP_SUCCESS,
        CSR_BT_RESULT_CODE_SAP_UNACCEPTABLE_PARAMETER);

    /* send a register to CM to allocate the internal server channel
     * number for the client role. */
    CsrBtCmRegisterReqSend(CSR_BT_SAPS_IFACEQUEUE);
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
void CsrBtSapsDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    CsrBool    lastMsg;
    SapsInstanceData_t  *instData;

    instData = (SapsInstanceData_t *) (*gash);

    /* continue to poll any message of the input queue */
    lastMsg = FALSE;

    while (!lastMsg)
    {
        if (!CsrMessageQueuePop(&instData->saveQueue, &msg_type, &msg_data))
        {
            lastMsg = !CsrSchedMessageGet(&msg_type, &msg_data);
        }
        if (!lastMsg)
        {
            switch (msg_type)
            {
                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(msg_type, msg_data);
                    break;
                }

                case CSR_BT_SAPS_PRIM:
                {
                    CsrBtSapsFreeDownstreamMessageContents(msg_type, msg_data);
                    break;
                }

                case CSR_SCHED_PRIM:
                    break;

                default:
                    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrGeneralException("SAPS",
                         0,
                                             msg_type,
                                             "Unknown PRIM in Deinit");
#endif
                        break;
                    }
            }
            CsrPmemFree (msg_data);
        }
    }
    CsrPmemFree(instData);
}
#endif

void CsrBtSapsHandler(void **gash)
{
    SapsInstanceData_t    *instData;
    CsrUint16            eventClass=0;
    void                *msg=NULL;

    instData = (SapsInstanceData_t *) (*gash);

    if(!instData->restoreSapsFlag)
    {
        CsrSchedMessageGet(&eventClass , &msg);
    }
    else
    {
        if(!CsrMessageQueuePop(&instData->saveQueue, &eventClass, &msg))
        {
            instData->restoreSapsFlag = FALSE;
            CsrSchedMessageGet(&eventClass , &msg);
        }
    }

    instData->recvMsgP = msg;

    switch(eventClass)
    {
        case CSR_BT_SAPS_PRIM:
            {
                CsrBtSapsPrim         *primType;

                /* find the message type */
                primType = (CsrBtSapsPrim *)msg;
                if ((*primType < CSR_BT_SAPS_PRIM_DOWNSTREAM_COUNT) && sapsStateHandlers[instData->state][*primType] != NULL)
                {
                    sapsStateHandlers[instData->state][*primType](instData);
                }

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                else if (*primType != CSR_BT_SAPS_HOUSE_CLEANING)
                {
                    /* State/Event ERROR! */
                    CsrGeneralException("SAPS",
                                         (CsrUint16) instData->state,
                                         (CsrUint16) (*primType - CSR_PRIM_UPSTREAM),
                                         "Unknown SAPS prim or undefined state");
                    CsrBtSapsFreeDownstreamMessageContents(eventClass, msg);
                }
#endif

                break;
            }

        case CSR_BT_CM_PRIM:
        {
            CsrPrim         *primType;

            /* find the message type */
            primType = (CsrPrim *)msg;
            if (((CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST) < CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT) &&
                (cmStateHandlers[instData->state][(CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)] != NULL))
            {
                cmStateHandlers[instData->state][(CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)](instData);
            }
            else if ((*primType == CSR_BT_CM_SDS_REGISTER_CFM) && ((instData->state == Idle_s) || (instData->state == Deactivate_s)))
            {
                if (instData->state == Idle_s)
                {
                    CsrBtSapsIdleStateCmSdsRegisterCfm(instData);
                }
                else
                {
                    CsrBtSapsDeactivateSdsRegisterCfm(instData);
                }
            }
            else if ((*primType == CSR_BT_CM_SDS_UNREGISTER_CFM) && (instData->state == Deactivate_s))
            {
                CsrBtSapsDeactivateSdsUnregisterCfm(instData);
            }
            else
            { /* State/Event ERROR! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("SAPS", (CsrUint16) instData->state, *primType, "Unknown CM prim or undefined state");
#endif
                CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
            }
            break;
        }

        case CSR_SCHED_PRIM:
            break;

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                /* State/Event ERROR! */
                CsrGeneralException("SAPS",
                                     (CsrUint16) instData->state,
                                     eventClass,
                                     "Unknown primitive type received in main");
            }
#endif
    }
    /* free the received message. if the ptr is NULL just CsrSched just ignores */
    CsrPmemFree(instData->recvMsgP);
}



