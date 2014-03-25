/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_env_prim.h"
#include "csr_bt_result.h"
#include "csr_bt_sapc_main.h"
#include "csr_bt_sapc_sef.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

/* ---------- SAPC jump table ---------- */
static const SapcStateHandlerType sapcStateHandlers[SapcNumberOfStates][CSR_BT_SAPC_PRIM_DOWNSTREAM_COUNT] =
{
    /* Init_s */
    {
        CsrBtSapcSavemessage,                                         /* CSR_BT_SAPC_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_DISCONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_APDU_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_ATR_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_OFF_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_ON_REQ */
        NULL,                                                    /* CSR_BT_SAPC_RESET_SIM_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */
        NULL,                                                    /* CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */
        CsrBtSapcSecurityOutReqHandler,                               /* CSR_BT_SAPC_SECURITY_OUT_REQ */
    },
    /* Idle_s */
    {
        CsrBtSapcIdleStateConnectReqHandler,                          /* CSR_BT_SAPC_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_DISCONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_APDU_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_ATR_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_OFF_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_ON_REQ */
        NULL,                                                    /* CSR_BT_SAPC_RESET_SIM_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */
        NULL,                                                    /* CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */
        CsrBtSapcSecurityOutReqHandler,                               /* CSR_BT_SAPC_SECURITY_OUT_REQ */
    },
    /* Search_s */
    {
        NULL,                                                    /* CSR_BT_SAPC_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_DISCONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_APDU_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_ATR_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_OFF_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_ON_REQ */
        NULL,                                                    /* CSR_BT_SAPC_RESET_SIM_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */
        NULL,                                                    /* CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */
        CsrBtSapcSecurityOutReqHandler,                               /* CSR_BT_SAPC_SECURITY_OUT_REQ */
    },
    /* Connected_s */
    {
        NULL,                                                    /* CSR_BT_SAPC_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_DISCONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_APDU_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_ATR_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_OFF_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_ON_REQ */
        NULL,                                                    /* CSR_BT_SAPC_RESET_SIM_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */
        NULL,                                                    /* CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */
        CsrBtSapcSecurityOutReqHandler,                          /* CSR_BT_SAPC_SECURITY_OUT_REQ */
    },
    /* ConnectedFail_s */
    {
        NULL,                                                    /* CSR_BT_SAPC_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_DISCONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_APDU_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_ATR_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_OFF_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_ON_REQ */
        NULL,                                                    /* CSR_BT_SAPC_RESET_SIM_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */
        NULL,                                                    /* CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */
        CsrBtSapcSecurityOutReqHandler,                          /* CSR_BT_SAPC_SECURITY_OUT_REQ */
    },
    /* SapConnected_s */
    {
        NULL,                                                    /* CSR_BT_SAPC_CONNECT_REQ */
        CsrBtSapcSapConnectedStateDisconnectReqHandler,               /* CSR_BT_SAPC_DISCONNECT_REQ */
        CsrBtSapcSapConnectedStateTransferApduReqHandler,             /* CSR_BT_SAPC_TRANSFER_APDU_REQ */
        CsrBtSapcSapConnectedStateTransferAtrReq,                     /* CSR_BT_SAPC_TRANSFER_ATR_REQ */
        CsrBtSapcSapConnectedStatePowerSimOffReq,                     /* CSR_BT_SAPC_POWER_SIM_OFF_REQ */
        CsrBtSapcSapConnectedStatePowerSimOnReq,                      /* CSR_BT_SAPC_POWER_SIM_ON_REQ */
        CsrBtSapcSapConnectedStateResetSimReq,                        /* CSR_BT_SAPC_RESET_SIM_REQ */
        CsrBtSapcSapConnectedStateTransferCardStatusReq,              /* CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */
        CsrBtSapcSapConnectedStateSetTransferProtocolReq,             /* CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */
        CsrBtSapcSecurityOutReqHandler,                               /* CSR_BT_SAPC_SECURITY_OUT_REQ */
    },
    /* Disconnecting_s */
    {
        NULL,                                                    /* CSR_BT_SAPC_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_DISCONNECT_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_APDU_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_ATR_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_OFF_REQ */
        NULL,                                                    /* CSR_BT_SAPC_POWER_SIM_ON_REQ */
        NULL,                                                    /* CSR_BT_SAPC_RESET_SIM_REQ */
        NULL,                                                    /* CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */
        NULL,                                                    /* CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */
        CsrBtSapcSecurityOutReqHandler,                               /* CSR_BT_SAPC_SECURITY_OUT_REQ */
    }
};

/* ---------- CM jump table ---------- */
/* PLEASE NOTE: only the top most part of the CM primitives are included */
static const SapcStateHandlerType cmStateHandlers[SapcNumberOfStates][CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT] =
{
    /* Init_s */
    {
        NULL,                                                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtSapcInitStateCmRegisterHandler,                     /* CSR_BT_CM_REGISTER_CFM */
        NULL,                                                    /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_DATA_IND */
        NULL,                                                    /* CSR_BT_CM_DATA_CFM */
        NULL,                                                    /* CSR_BT_CM_CONTROL_IND */
        NULL,                                                    /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Idle_s */
    {
        NULL,                                                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        NULL,                                                    /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_DATA_IND */
        NULL,                                                    /* CSR_BT_CM_DATA_CFM */
        NULL,                                                    /* CSR_BT_CM_CONTROL_IND */
        NULL,                                                    /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Search_s */
    {
        NULL,                                                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        NULL,                                                    /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_DATA_IND */
        NULL,                                                    /* CSR_BT_CM_DATA_CFM */
        NULL,                                                    /* CSR_BT_CM_CONTROL_IND */
        NULL,                                                    /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Connected_s */
    {
        NULL,                                                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        CsrBtSapcConnectedStateCmDisconnectIndHandler,           /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtSapcDataIndHandler,                                 /* CSR_BT_CM_DATA_IND */
        CsrBtSapcDataCfmHandler,                                 /* CSR_BT_CM_DATA_CFM */
        CsrBtSapcXStateCmControlIndHandler,                      /* CSR_BT_CM_CONTROL_IND */
        CsrBtSapcXStateCmModeChangeIndHandler,                   /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* ConnectedFail_s */
    {
        NULL,                                                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        CsrBtSapcConnectedStateCmDisconnectIndHandler,           /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_DATA_IND */
        NULL,                                                    /* CSR_BT_CM_DATA_CFM */
        CsrBtSapcXStateCmControlIndHandler,                      /* CSR_BT_CM_CONTROL_IND */
        CsrBtSapcXStateCmModeChangeIndHandler,                   /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* SapConnected_s */
    {
        NULL,                                                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        CsrBtSapcSapConnectedStateCmDisconnectIndHandler,        /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtSapcDataIndHandler,                                 /* CSR_BT_CM_DATA_IND */
        CsrBtSapcDataCfmHandler,                                 /* CSR_BT_CM_DATA_CFM */
        CsrBtSapcXStateCmControlIndHandler,                      /* CSR_BT_CM_CONTROL_IND */
        CsrBtSapcXStateCmModeChangeIndHandler,                   /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Disconnecting_s */
    {
        NULL,                                                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        CsrBtSapcDisconnectingStateCmDisconnectIndHandler,       /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtSapcDataIndHandler,                                 /* CSR_BT_CM_DATA_IND */
        CsrBtSapcDataCfmHandler,                                 /* CSR_BT_CM_DATA_CFM */
        CsrBtSapcXStateCmControlIndHandler,                      /* CSR_BT_CM_CONTROL_IND */
        CsrBtSapcXStateCmModeChangeIndHandler,                   /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    }
};
/* ---------- End of jump tables ---------- */

void CsrBtSapcInit(void **gash)
{
    SapcInstanceData_t  *instData;
    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemZalloc(sizeof(SapcInstanceData_t));
    instData = (SapcInstanceData_t *) *gash;

    /* clear own instance data for first time use */
    instData->SapState              = SapIdle_s;
    instData->transmitReady         = TRUE;

    CsrBtScSetSecOutLevel(&instData->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_SIM_ACCESS_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_SIM_ACCESS_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_SAP_SUCCESS,
        CSR_BT_RESULT_CODE_SAP_UNACCEPTABLE_PARAMETER);

     /* Tell the SD that it must look for the
        CSR_BT_SIM_ACCESS_PROFILE_UUID service, when it
        perform a SD_READ_AVAILABLE_SERVICE_REQ */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_SIM_ACCESS_PROFILE_UUID);

    CsrBtCmRegisterReqSend(CSR_BT_SAPC_IFACEQUEUE);
    instData->sdpSapcSearchConData  = CsrBtUtilSdpRfcConInit(NULL,NULL, CsrBtSapcRfcSdcConResultHandler, CSR_BT_SAPC_IFACEQUEUE);
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
void CsrBtSapcDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    CsrBool    lastMsg;
    SapcInstanceData_t  *instData;

    instData = (SapcInstanceData_t *) (*gash);

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

                case CSR_BT_SAPC_PRIM:
                {
                    CsrBtSapcFreeDownstreamMessageContents(msg_type, msg_data);
                    break;
                }

                case CSR_SCHED_PRIM:
                    break;

                default:
                    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrGeneralException("SAPC",
                                             0,
                                             msg_type,
                                             "Unknown PRIM In deinit");
#endif
                        break;
                    }
            }
            CsrPmemFree (msg_data);
        }
    }
    if (instData->sdpSapcSearchConData)
    {
        CsrBtUtilSdcRfcDeinit(&(instData->sdpSapcSearchConData));
        instData->sdpSapcSearchConData = NULL;
    }
    CsrPmemFree(instData);
}
#endif

void CsrBtSapcHandler(void **gash)
{
    SapcInstanceData_t    *instData;
    CsrUint16            eventClass=0;
    void                *msg=NULL;

    instData = (SapcInstanceData_t *) (*gash);

    if(!instData->restoreSapcFlag)
    {
        CsrSchedMessageGet(&eventClass , &msg);
    }
    else
    {
        if(!CsrMessageQueuePop(&instData->saveQueue, &eventClass, &msg))
        {
            instData->restoreSapcFlag = FALSE;
            CsrSchedMessageGet(&eventClass , &msg);
        }
    }
    instData->recvMsgP = msg;

    switch(eventClass)
    {
        case CSR_BT_SAPC_PRIM:
            {
                CsrBtSapcPrim         *primType;

                /* find the message type */
                primType = (CsrBtSapcPrim *)msg;
                if ((*primType < CSR_BT_SAPC_PRIM_DOWNSTREAM_COUNT) &&
                    (sapcStateHandlers[instData->state][*primType] != NULL))
                {
                    sapcStateHandlers[instData->state][*primType](instData);
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
    else if (*primType != CSR_BT_SAPC_HOUSE_CLEANING)
                {
                    /* State/Event ERROR! */
                    CsrGeneralException("SAPC",
                                         instData->state,
                                         (CsrUint16)(*primType - CSR_PRIM_UPSTREAM),
                                         "Unknown SAPC prim or undefined state");
                    CsrBtSapcFreeDownstreamMessageContents(eventClass, msg);
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
                else if (CsrBtUtilRfcConVerifyCmMsg(instData->recvMsgP))
                {
                    CsrBtUtilRfcConCmMsgHandler(instData, instData->sdpSapcSearchConData, instData->recvMsgP);
                }
                else if (*primType == CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM)
                {
                    /* Ignore CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                else
                {
                    CsrGeneralException("SAPC", instData->state, *primType, "Unknown CM prim or undefined state");
                    CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                }
#endif
                break;
            }

        case CSR_SCHED_PRIM:
            break;

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                /* State/Event ERROR! */
                CsrGeneralException("SAPC",
                                     instData->state,
                                     0xFF,
                                     "Unknown primitive type received in main");
            }
#endif
    }
    /* free the received message. if the ptr is NULL just CsrSched just ignores */
    CsrPmemFree(instData->recvMsgP);
}


